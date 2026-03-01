#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <err.h>
#include <errno.h>
#include "ps.h"
#include "globals.h"

const char *psName = "comm";
const char *status = "status";
const char *memInfo = "/proc/meminfo";
int fl = 0;
long strCount = 0;

int isCorrectDirectory(char *path, char *name);
int getProcMemoryData(int pid, mem* memory);
void getProcName(const char* pid, proc* ps);
long getTotalMemory();
int reallocPs(procList *pl);
void sort(procList* pl, options* opt);


int getAvailableProcs(procList *pl, options* opt) {
    DIR *dir = opendir("/proc");

    if (!dir) {
        err(1, "Failed to open directory");
    }

    struct dirent *entry;
    pl->size = 0;
    long totalMemory = getTotalMemory();
    while ((entry = readdir(dir)) != NULL) {
        char path[512];
        snprintf(path, sizeof(path), "/proc/%s", entry->d_name);
        if(!isCorrectDirectory(path, entry->d_name)){
            continue;
        }
        int res = reallocPs(pl);
        if(res != 0) {
            closedir(dir);
            err(1, "Memory allocation failed");
        }
        
        getProcName(entry->d_name, &(pl->ps[pl->size]));
        pl->ps[pl->size].pid = atoi(entry->d_name);
        mem memory = {0};
        res = getProcMemoryData(pl->ps[pl->size].pid, &memory);
        if(res == 0) {
            pl->ps[pl->size].memory = memory;
        } else {
            pl->ps[pl->size].memory.VmRSS = 0;
            pl->ps[pl->size].memory.VmSize = 0;
        }

        pl->ps[pl->size].memoryPercent = totalMemory > 0 ? (memory.VmRSS * 100.) / totalMemory : 0;


        pl->size++;

        if((opt->flags & STRING_RESTRICTION) && opt->sortMode == NOT_SORTED) {
            if(opt->limits == pl->size) {
                break;
            }
        }
    }
    closedir(dir);
    return 0;
}

void sortAvailableProcs(procList* pl, options* opt) {
    if(opt->sortMode == NOT_SORTED) {
        return;
    }
    sort(pl, opt);
}

void getProcName(const char* pid, proc* ps) {
    char comm[256] = "";
    char path[64];
    snprintf(path, sizeof(path), "/proc/%s/%s", pid, psName);
    FILE* psFile = fopen(path, "r");
    if(psFile == NULL) {
        return;
    }
    if (!fgets(comm, sizeof(comm), psFile)) {
        fclose(psFile);
        return;
    }
    comm[strcspn(comm, "\n")] = 0;
    snprintf(ps->name, sizeof(ps->name), "%s", comm);
    fclose(psFile);
}

long getTotalMemory() {
    char line[128];
    long totalMemory = -1;
    FILE* memInfoFile = fopen(memInfo, "r");
    if(memInfoFile == NULL) {
        return -1;
    }
    while(fgets(line, sizeof(line), memInfoFile)) {
        if(sscanf(line, "MemTotal: %ld", &totalMemory) == 1 ) {
            break;
        }
    }
    fclose(memInfoFile);
    return totalMemory;
}

int getProcMemoryData(int pid, mem* memory){
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/%s", pid, status);
    char line[128];
    long VmRSS = -1;
    long VmSize = -1;
    FILE* statusFile = fopen(path, "r");
    if(!statusFile) {
        return -1;
    }

    while(fgets(line, sizeof(line), statusFile)) {
        if(VmRSS != -1 && VmSize != -1) {
            break;
        }
        if(sscanf(line, "VmRSS: %ld", &VmRSS) == 1 ) {
            continue;
        }
        if(sscanf(line, "VmSize: %ld", &VmSize) == 1 ) {
            continue;
        }
    }
    if(VmRSS == -1) {
        VmRSS = 0;
    }

    if(VmSize == -1) {
        VmSize = 0;
    }

    memory->VmRSS = VmRSS;
    memory->VmSize = VmSize;
    fclose(statusFile);
    return 0;
}


int isCorrectDirectory(char *path, char *name) {
    struct stat st;
    int res = stat(path, &st);
    if (res != 0) 
        return 0;
    if (S_ISDIR(st.st_mode)) {
        char *end;
        strtol(name, &end, 10);
        if (*end == '\0') {           
            return 1;
        }
    }
        return 0;
}
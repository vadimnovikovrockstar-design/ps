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
int fl = 0;
long strCount = 0;

int isCorrectDirectory(char *path, char *name);
int getProcMemoryData(int pid, mem* memory);
void getProcName(const char* pid, proc* ps);
int reallocPs(proc** ps, int index);
void sort(proc** ps, int length);


int getAvaliableProcs(proc **ps, int* index, options* opt) {
    DIR *dir = opendir("/proc");

    if (!dir) {
        err(1, "Failed to open directory");
    }

    struct dirent *entry;
    *index = 0;
    while ((entry = readdir(dir)) != NULL) {
        char path[512];
        snprintf(path, sizeof(path), "/proc/%s", entry->d_name);
        if(!isCorrectDirectory(path, entry->d_name)){
            continue;
        }
        int res = reallocPs(ps, *index);
        if(res != 0) {
            closedir(dir);
            err(1, "Memory allocation failed");
        }
        
        getProcName(entry->d_name, &((*ps)[*index]));
        (*ps)[*index].pid = atoi(entry->d_name);
        mem memory;
        res = getProcMemoryData((*ps)[*index].pid, &memory);
        if(res == 0) {
            (*ps)[*index].memory = memory;
        }
        (*index)++;

        if(opt->flags & STRING_RESTRICTION) {
            if(opt->limits == *index) {
                break;
            }
        }
    }
    closedir(dir);
    sort(ps, *index);
    return 0;
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

int getProcMemoryData(int pid, mem* memory){
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/%s", pid, status);
    char line[128];
    long VmRSS = 0;
    long VmSize = 0;
    FILE* statusFile = fopen(path, "r");
    if(!statusFile) {
        return -1;
    }

    while(fgets(line, sizeof(line), statusFile)) {
        if(sscanf(line, "VmRSS: %ld", &VmRSS) == 1 ) {
            continue;
        }
        if(sscanf(line, "VmSize: %ld", &VmSize) == 1 ) {
            continue;
        }
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
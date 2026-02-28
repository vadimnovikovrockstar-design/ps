#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "myPs.h"

const char *psName = "comm";

int reallocPs(proc** ps, int index) {
    proc* tmp = *ps;
    tmp = realloc(tmp, (index + 1) * sizeof(proc));
    if (!tmp) {
        return -1;
    }
    memset(&tmp[index], 0, sizeof(proc));
    *ps = tmp;
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

void getProcName(const char* pid, proc* ps) {
    char comm[256] = "";
    char path[64];
    snprintf(path, sizeof(path), "/proc/%s/comm", pid);
    FILE* psName = fopen(path, "r");
    if(psName == NULL) {
        perror("Failed to open process file");
        return;
    }
    if (!fgets(comm, sizeof(comm), psName)) {
        fclose(psName);
        return;
    }
    comm[strcspn(comm, "\n")] = 0;
    snprintf(ps->name, sizeof(ps->name), "%s", comm);
    fclose(psName);
}

int getAvaliableProcs(proc **ps, int* index) {
    DIR *dir = opendir("/proc");

    if (!dir) {
        perror("Failed to open directory");
        return -1;
    }

    struct dirent *entry;
    *index = 0;
    while ((entry = readdir(dir)) != NULL) {
        char path[256];
        snprintf(path, sizeof(path), "/proc/%s", entry->d_name);
        if(!isCorrectDirectory(path, entry->d_name)){
            continue;
        }
        int res = reallocPs(ps, *index);
        if(res != 0) {
            perror("boo boo with memory...");
            closedir(dir);
            return -2;
        }
        getProcName(entry->d_name, &((*ps)[*index]));
        snprintf((*ps)[*index].pid, sizeof((*ps)[*index].pid), "%s", entry->d_name);
        (*index)++;
    }
    closedir(dir);
    return 0;
}

int main(int argc, char **argv) {
    proc *ps = NULL;
    int count;
    int res = getAvaliableProcs(&ps, &count);
    if(res != 0) {
        perror("something wrong...");
        return res;
    }
    for(int i = 0; i<count; ++i){
        printf("%-20.20s %10.10s\n", ps[i].name, ps[i].pid);
    }
    free(ps);
    return 0;
}
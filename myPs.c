#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <err.h>
#include "myPs.h"

const char *psName = "comm";
static int fl = 0;

int cmpByName(const void *a, const void *b) {
    const proc *pa = a;
    const proc *pb = b;
    return strcmp(pa->name, pb->name);
}

int cmpByPid(const void *a, const void *b) {
    const proc *pa = a;
    const proc *pb = b;
    return (pa->pid > pb->pid) - (pa->pid < pb->pid);
}

void sort(proc** ps, int length) {
    if((fl & sortByName)) {
        qsort(*ps, length, sizeof(proc), cmpByName);
    }
    
    if((fl & sortByPid)) {
        qsort(*ps, length, sizeof(proc), cmpByPid);
    }
}

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
    snprintf(path, sizeof(path), "/proc/%s/%s", pid, psName);
    FILE* psName = fopen(path, "r");
    if(psName == NULL) {
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
        (*index)++;
    }
    closedir(dir);
    sort(ps, *index);
    return 0;
}

void parseArgs(int argc, char **argv) {
    int opt;

    while ((opt = getopt(argc, argv, "np")) != -1) {
        switch (opt) {
        case 'n':
            fl |= sortByName;
            break;
        case 'p':
            fl |= sortByPid;
            break;
        default:
            errx(2, "Bad command line args");
        }
    }

    if ((fl & sortByName) && (fl & sortByPid)) {
        errx(2, "Bad sorting arguments");
    }
}

int main(int argc, char **argv) {
    parseArgs(argc, argv);
    proc *ps = NULL;    
    int count;
    getAvaliableProcs(&ps, &count);
    for(int i = 0; i<count; ++i){
        printf("%-20.20s %10d\n", ps[i].name, ps[i].pid);
    }
    free(ps);
    return 0;
}

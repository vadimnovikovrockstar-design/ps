#include <getopt.h>
#include <err.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "ps.h"
#include "globals.h"

int getAvaliableProcs(proc **ps, int* index);

void parseArgs(int argc, char **argv) {
    int opt;

    while ((opt = getopt(argc, argv, "sSrRnpN:")) != -1) {
        switch (opt) {
        case 'n':
            fl |= sortByName;
            break;
        case 'p':
            fl |= sortByPid;
            break;
        case 'N':
            errno = 0;
            fl |= stringCount;
            char *end;
            strCount = strtol(optarg, &end, 10);
            if (errno == ERANGE) {
                errx(2, "Number out of range");
            }
            if (*end != '\0' || strCount <=0) {
                errx(2, "Bad command line args");
            }
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
    printf("%-20s %10s %10s %10s\n", "Name", "PID", "VmRSS(KB)", "VmSize(KB)");
    for(int i = 0; i<count; ++i){
        printf("%-20.20s %10d %10ld %10ld\n", ps[i].name, ps[i].pid, ps[i].memory.VmRSS, ps[i].memory.VmSize);
    }
    free(ps);
    return 0;
}

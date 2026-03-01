#include <getopt.h>
#include <err.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "ps.h"
#include "globals.h"

int getAvaliableProcs(procList *pl, options* opt);

void parseArgs(int argc, char **argv, options* opt) {
    int args;

    while ((args = getopt(argc, argv, "sSrRnpN:")) != -1) {
        switch (args) {
        case 'n':
            opt->sortMode = SORT_BY_NAME;
            break;
        case 'p':
            opt->sortMode = SORT_BY_PID;
            break;
        case 'N':
            errno = 0;
            char *end;
            opt->flags |= STRING_RESTRICTION;
            opt->limits = strtol(optarg, &end, 10);
            if (errno == ERANGE) {
                errx(2, "Number out of range");
            }
            if (*end != '\0' || opt->limits <=0) {
                errx(2, "Bad command line args");
            }
            break;
        case 's':
            opt->sortMode = SORT_UP_BY_VM_RSS;
            break;
        case 'S':
            opt->sortMode = SORT_DOWN_BY_VM_RSS;
            break;
        case 'r':
            opt->sortMode = SORT_UP_BY_VM_SIZE;
            break;
        case 'R':
            opt->sortMode = SORT_DOWN_BY_VM_SIZE;
            break;
        default:
            errx(2, "Bad command line args");
        }
    }
}

int main(int argc, char **argv) {
    options opt = {.sortMode = NOT_SORTED, .flags = 0, .limits = 0};
    procList psList = {.ps = NULL, .capacity = 0, .size = 0};
    parseArgs(argc, argv, &opt);
    getAvaliableProcs(&psList, &opt);
    printf("%-20s %10s %10s %10s\n", "Name", "PID", "VmRSS(KB)", "VmSize(KB)");
    for(int i = 0; i<psList.size; ++i){
        printf("%-20.20s %10d %10ld %10ld\n", psList.ps[i].name, psList.ps[i].pid, psList.ps[i].memory.VmRSS, psList.ps[i].memory.VmSize);
    }
    free(psList.ps);
    return 0;
}

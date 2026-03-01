#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ps.h"
#include "globals.h"


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

int cmpByVmRssUp(const void *a, const void *b) {
    const proc *pa = a;
    const proc *pb = b;
    return (pa->memory.VmRSS > pb->memory.VmRSS) - (pa->memory.VmRSS < pb->memory.VmRSS);
}

int cmpByVmRssDown(const void *a, const void *b) {
    const proc *pa = a;
    const proc *pb = b;
    return (pa->memory.VmRSS < pb->memory.VmRSS) - (pa->memory.VmRSS > pb->memory.VmRSS);
}

int cmpByVmSizeUp(const void *a, const void *b) {
    const proc *pa = a;
    const proc *pb = b;
    return (pa->memory.VmSize > pb->memory.VmSize) - (pa->memory.VmSize < pb->memory.VmSize);
}

int cmpByVmSizeDown(const void *a, const void *b) {
    const proc *pa = a;
    const proc *pb = b;
    return (pa->memory.VmSize < pb->memory.VmSize) - (pa->memory.VmSize > pb->memory.VmSize);
}

int cmpByMemoryPercentUp(const void *a, const void *b) {
    const proc *pa = a;
    const proc *pb = b;
    return (pa->memoryPercent > pb->memoryPercent) - (pa->memoryPercent < pb->memoryPercent);
}

int cmpByMemoryPercentDown(const void *a, const void *b) {
    const proc *pa = a;
    const proc *pb = b;
    return (pa->memoryPercent < pb->memoryPercent) - (pa->memoryPercent > pb->memoryPercent);
}

void sort(procList* pl, options* opt) {
    if(opt->sortMode == SORT_BY_NAME) {
        qsort(pl->ps, pl->size, sizeof(proc), cmpByName);
    }
    
    if(opt->sortMode == SORT_BY_PID) {
        qsort(pl->ps, pl->size, sizeof(proc), cmpByPid);
    }

    if(opt->sortMode == SORT_UP_BY_VM_RSS) {
        qsort(pl->ps, pl->size, sizeof(proc), cmpByVmRssUp);
    }

    if(opt->sortMode == SORT_DOWN_BY_VM_RSS) {
        qsort(pl->ps, pl->size, sizeof(proc), cmpByVmRssDown);
    }

    if(opt->sortMode == SORT_UP_BY_VM_SIZE) {
        qsort(pl->ps, pl->size, sizeof(proc), cmpByVmSizeUp);
    }

    if(opt->sortMode == SORT_DOWN_BY_VM_SIZE) {
        qsort(pl->ps, pl->size, sizeof(proc), cmpByVmSizeDown);
    }

    if(opt->sortMode == SORT_UP_BY_MEMORY_PERCENT) {
        qsort(pl->ps, pl->size, sizeof(proc), cmpByMemoryPercentUp);
    }

    if(opt->sortMode == SORT_DOWN_BY_MEMORY_PERCENT) {
        qsort(pl->ps, pl->size, sizeof(proc), cmpByMemoryPercentDown);
    }

}

int reallocPs(procList *pl) {
    if(pl->size == pl->capacity) {
        int newCapacity = pl->capacity == 0 ? 1 : pl->capacity * 2;
        proc* newPs = realloc(pl->ps, newCapacity * sizeof(proc));
        if(newPs == NULL) {
            return -1;
        }
        memset(newPs + pl->capacity, 0, (newCapacity - pl->capacity) * sizeof(proc));
        pl->ps = newPs;
        pl->capacity = newCapacity;
        return 0;
    }

    return 0;
}

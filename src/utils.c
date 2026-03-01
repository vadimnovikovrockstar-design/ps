#include <string.h>
#include <stdlib.h>
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

void sort(proc** ps, int length, options* opt) {
    
    if(opt->sortMode == SORT_BY_NAME) {
        qsort(*ps, length, sizeof(proc), cmpByName);
    }
    
    if(opt->sortMode == SORT_BY_PID) {
        qsort(*ps, length, sizeof(proc), cmpByPid);
    }

    if(opt->sortMode == SORT_UP_BY_VM_RSS) {
        qsort(*ps, length, sizeof(proc), cmpByVmRssUp);
    }

    if(opt->sortMode == SORT_DOWN_BY_VM_RSS) {
        qsort(*ps, length, sizeof(proc), cmpByVmRssDown);
    }

    if(opt->sortMode == SORT_UP_BY_VM_SIZE) {
        qsort(*ps, length, sizeof(proc), cmpByVmSizeUp);
    }

    if(opt->sortMode == SORT_DOWN_BY_VM_SIZE) {
        qsort(*ps, length, sizeof(proc), cmpByVmSizeDown);
    }

}

int reallocPs(proc** ps, int index) {
    proc* tmp = *ps;
    tmp = realloc(tmp, (index + 1) * sizeof(proc));
    if (!tmp) {
        return -1;
    }
    memset(&tmp[index], 0, sizeof(proc));
    memset(&tmp[index].memory, 0, sizeof(mem));
    *ps = tmp;
    return 0;
}

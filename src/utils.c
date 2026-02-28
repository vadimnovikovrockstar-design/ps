#include "ps.h"

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

void sort(proc** ps, int length, int fl) {
    if(!(fl & sortByName) && !(fl & sortByPid)) {
        return;
    }
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
    memset(&tmp[index].memory, 0, sizeof(mem));
    *ps = tmp;
    return 0;
}

#ifndef MY_PS_H
#define MY_PS_H


enum {
    STRING_RESTRICTION = 1 << 1
};

enum sortMode {
    SORT_BY_NAME,
    SORT_BY_PID,
    SORT_UP_BY_VM_RSS,
    SORT_DOWN_BY_VM_RSS,
    SORT_UP_BY_VM_SIZE,
    SORT_DOWN_BY_VM_SIZE,
    NOT_SORTED
};

typedef struct options {
    int flags;
    enum sortMode sortMode;
    long limits;   
} options;

typedef struct {
    long VmRSS;
    long VmSize;
} mem;

typedef struct {
    char name[256];
    int pid;
    mem memory;
} proc;

#endif
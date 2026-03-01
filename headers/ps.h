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
    SORT_UP_BY_MEMORY_PERCENT,
    SORT_DOWN_BY_MEMORY_PERCENT,
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
    int pid;
    char name[256];
    mem memory;
    double memoryPercent;
} proc;

typedef struct {
    proc* ps;
    int capacity;
    int size;
} procList;

#endif
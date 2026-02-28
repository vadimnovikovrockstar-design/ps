#ifndef MY_PS_H
#define MY_PS_H

#define sortByPid 0x01
#define sortByName 0x02
#define stringCount 0x04

typedef struct {
    long VmRSS;
    long VmSize;
} mem;

typedef struct {
    char name[256];
    int pid;
    mem memory;
} proc;


int getAvaliableProcs(proc** proc, int* count);
int countDirs();


#endif
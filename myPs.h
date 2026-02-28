#ifndef MY_PS_H
#define MY_PS_H

#define flags 0
#define sortByPid 0x01
#define sortByName 0x02



typedef struct {
    char name[256];
    int pid;
} proc;


int getAvaliableProcs(proc** proc, int* count);
int countDirs();


#endif
#ifndef MY_PS_H
#define MY_PS_H

#define bool int
#define true 1
#define false 0

typedef struct {
    char name[256];
    char pid[16];
} proc;


int getAvaliableProcs(proc** proc, int count);
int countDirs();


#endif
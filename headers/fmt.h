#pragma once
#include <stddef.h>
#include "ps.h"

typedef void(*printFunc)(const proc* pl, int width);


typedef struct {
    const char* title;
    int width;
    int enable;
    int alignment;
    printFunc func;
} column;

void printTable(const procList* pl, options* opt);

void parseArgs(int argc, char **argv, options* opt);
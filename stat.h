#ifndef STAT_H_   /* Include guard */
#define STAT_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LINE_LENGTH 100
#define SIZE 1024

// Find variable status
typedef struct Variable
{
    int numOfVars;
    char *varType;
    char *varName;
    char *varScope;
    int varSize[SIZE];
} Variable;

// Find file status
typedef struct Stat{
    int numOfLines;
    int numOfFuncs;
    char *funcName;
    char *funcSize;
    Variable var;
} Stat;

char *getFuncName(char *line);
int isFunction(char *line);
char *getVarFuncName(char *line);
Stat findStatus(char *filename);
Variable findVariable(char *filename);
void findHeap(char *filename, char *varType, char *varName, char *varScope);
void findLiteral(char *filename);


#endif
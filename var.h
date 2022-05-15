#ifndef VAR_H_   /* Include guard */
#define VAR_H_

#include <ctype.h>
#include "stat.h"

char *trim(char *line);
int isVariable(char *line);
int isGlobal(char *line);
char *replaceChar(char *line, char target, char replace);
char *removeStars(char *token);
// char *removeComment(char *line);
int sizeOf(char *type, int num);
int checkDigit(char *s);
Variable getVar(char *line);

#endif
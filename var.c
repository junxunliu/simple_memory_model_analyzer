#include "var.h"

/**
 * @brief remove leading and ending whitespace of a string, return removed string
 *
 * @param line
 * @return char*
 */
char *trim(char *line)
{
    char *end;

    while (isspace((unsigned char)*line))
    {
        line++;
    }

    // all white space
    if (*line == 0)
    {
        return line;
    }

    end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end))
    {
        end--;
    }
    end[1] = '\0';
    return line;
}

/**
 * @brief return 1 if this line is used to declare a variable
 *
 * @param line
 * @return int
 */
int isVariable(char *line)
{
    if (strncmp(line, "  ", 2) != 0)
    {
        return 0;
    }

    line = trim(line);

    if (strncmp(line, "int ", 4) == 0 || strncmp(line, "char ", 5) == 0 || strncmp(line, "float ", 6) == 0 || strncmp(line, "void ", 5) == 0)
    {
        if (strstr(line, ";"))
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief return 1 if this line is used to declare a global variable
 *
 * @param line
 * @return int
 */
int isGlobal(char *line)
{
    if (strncmp(line, "int ", 4) == 0 || strncmp(line, "char ", 5) == 0 || strncmp(line, "float ", 6) == 0 || strncmp(line, "void ", 5) == 0)
    {
        if (strstr(line, ";"))
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief replace "target" char to "replace" char in a string,
 * return relpaced string
 *
 * @param str
 * @param find
 * @param replace
 * @return char*
 */
char *replaceChar(char *line, char target, char replace)
{
    char *curr = strchr(line, target);
    while (curr)
    {
        *curr = replace;
        curr = strchr(curr, target);
    }
    return line;
}

/**
 * @brief remove the leading '*' signs
 *
 * @param token
 * @return char*
 */
char *removeStars(char *token)
{
    while (strchr(token, '*'))
    {
        token++;
        // strcat(str, "*");
    }
    return token;
}

/**
 * @brief get the size of each type, types range is discussed in the assignment
 *
 * @param type
 * @param num
 * @return int
 */
int sizeOf(char *type, int num)
{
    if (strcmp(type, "int") == 0 || strstr(type, "int["))
    {
        return num * sizeof(int);
    }
    else if (strcmp(type, "float") == 0 || strstr(type, "float["))
    {
        return num * sizeof(float);
    }
    else if (strcmp(type, "char") == 0 || strstr(type, "char["))
    {
        return num * sizeof(char);
    }
    else if (strcmp(type, "int*") == 0)
    {
        return num * sizeof(int *);
    }
    else if (strcmp(type, "float*") == 0)
    {
        return num * sizeof(float *);
    }
    else if (strcmp(type, "char*") == 0)
    {
        return num * sizeof(char *);
    }
    else if (strcmp(type, "int**") == 0)
    {
        return num * sizeof(int **);
    }
    else if (strcmp(type, "float**") == 0)
    {
        return num * sizeof(float **);
    }
    else if (strcmp(type, "char**") == 0)
    {
        return num * sizeof(char **);
    }
    return 0;
}

/**
 * @brief return 1 if a string contains a digit at index 0
 *
 * @param s
 * @return int
 */
int checkDigit(char *s)
{

    if (s[0] == '0' || s[0] == '1' || s[0] == '2' || s[0] == '3' || s[0] == '5' || s[0] == '6' || s[0] == '7' || s[0] == '8' || s[0] == '9')
    {
        return 1;
    }
    return 0;
}

/**
 * @brief get the variable information in a line of a file
 * return the Variable object
 * @param line
 * @return Variable
 */
Variable getVar(char *line)
{
    Variable var;
    int numOfVars = 0;
    char *varType = malloc(sizeof(char) * SIZE);
    char *varName = malloc(sizeof(char) * SIZE);

    // char *name = malloc(sizeof(char) * 40);     // variable name who contains brackets
    // char *brackets = malloc(sizeof(char) * 40); // brackets
    // char *number = malloc(sizeof(char) * 10);   // size in brackets
    char number[10]; // size in brackets
    
    char *token;
    char *type;
    int numInBrackets = 1;
    int flag = 0, strLiteralFlag = 0, heapFlag = 0;
    int function = isFunction(line);
    int variable = isVariable(line);
    int global = isGlobal(line);

    strcpy(varType, "\0");
    strcpy(varName, "\0");
    // strcpy(varScope, "\0");

    replaceChar(line, ',', ' ');

    if (variable == 1 || global == 1)
    {
        int count = 1;
        line = trim(line);

        // remove ';'
        line[strlen(line) - 1] = '\0';

        if (strchr(line, '='))
        {
            const char *ptr1 = strchr(line, '{');
            if (ptr1)
            {
                int index1 = ptr1 - line; // index of '{'
                char arrayVar[LINE_LENGTH + 1];
                strncpy(arrayVar, &line[index1 + 1], strlen(line) - index1 - 3); // {a, b, c}
                char *tk = strtok(arrayVar, " ");
                while (tk)
                {
                    tk = strtok(NULL, " ");
                    if (tk)
                    {
                        count++;
                    }
                }
            }

            // check case string literal
            if (strchr(line, '*') && strchr(line, '"'))
            {
                strLiteralFlag = 1;
            }

            // check case heap
            if (strstr(line, "malloc") || strstr(line, "calloc"))
            {
                heapFlag = 1;
            }
        }

        token = strtok(line, " ");
        type = token;
        token = strtok(NULL, " ");

        while (token)
        {
            // int *a, b[], *c = 5;
            // int i[10] = "hello";
            if (token)
            {

                if (flag == 1)
                {
                    if (strchr(token, '}'))
                    {
                        flag = 0;
                    }
                    token = strtok(NULL, " ");
                    continue;
                }
                // printf("line=%s  strLiteralFlag=%d\n", lineNew, strLiteralFlag);
                if (strchr(token, '='))
                {
                    if (strLiteralFlag == 1 || heapFlag == 1)
                    {
                        break;
                    }

                    if (count != 0)
                    {
                        var.varSize[numOfVars - 1] = sizeOf(type, count);
                        // printf("varType=%s\n", varType);
                    }

                    if (atoi(number) != 0)
                    {
                        var.varSize[numOfVars - 1] = sizeOf(type, atoi(number));
                    }
                    else
                    {
                        var.varSize[numOfVars - 1] = sizeOf(type, 1);
                    }

                    token = strtok(NULL, " ");

                    for (int i = 0; i < strlen(token); i++) {
                        if (isalpha(token[i])) {
                            token = strtok(NULL, " ");
                            break;
                        }
                    }
                    continue;
                }
                else if (checkDigit(token) == 1)
                {
                    token = strtok(NULL, " ");
                    continue;
                }

                // check case int a[] = {1, 2, 3}
                else if (strchr(token, '{'))
                {
                    token = strtok(NULL, " ");
                    flag = 1;
                    continue;
                }

                else if (strchr(token, '"'))
                {
                    token = strtok(NULL, " ");
                    continue;
                }

                if (strchr(token, '*'))
                {
                    char *temp = malloc(sizeof(char) * 10);
                    strcpy(temp, type);
                    if (strstr(token, "**"))
                    {
                        strcat(temp, "**");
                    }
                    else
                    {
                        strcat(temp, "*");
                    }
                    token = removeStars(token);
                    strcat(varType, temp);
                    var.varSize[numOfVars] = sizeOf(temp, 1);
                    free(temp);
                }

                else
                {
                    strcat(varType, type);
                    var.varSize[numOfVars] = sizeOf(type, 1);
                }
                // b[]
                const char *ptr2 = strchr(token, '[');
                if (ptr2)
                {
                    int index2 = (int)(ptr2 - token); // index of '['
                    char name[40]; // variable name who contains brackets
                    char brackets[40]; // brackets

                    strcpy(name, "\0");
                    strcpy(brackets, "\0");
                    strcpy(number, "\0");

                    strncat(name, token, index2);                                    // get the variable name
                    strcpy(brackets, &token[index2]);                                // get the brackets
                    strncpy(number, &token[index2 + 1], strlen(token) - index2 - 2); // get the size of brackets

                    if (atoi(number) != 0)
                    {
                        // printf("numOfVars = %d\n", numOfVars);
                        numInBrackets = atoi(number);
                        var.varSize[numOfVars] = sizeOf(type, numInBrackets);
                        // printf("var.varSize[numOfVars] = %d\n",  var.varSize[4]);
                    }
                    strcat(varType, brackets);
                    strcat(varName, name);
                    // numOfVars++;
                    // free(name);
                }
                else
                {
                    strcat(varName, token);
                    // numOfVars++;
                }
                // printf("num=%d", numOfVars);
                numOfVars++;
                strcat(varName, " ");
                strcat(varType, " ");
            }
            token = strtok(NULL, " ");
        }

        if (global == 1)
        {
            var.varScope = "global";
        }
        else
        {
            var.varScope = "default";
        }
    }
    if (function == 1)
    {
        char *s;

        replaceChar(line, ')', ' ');

        strtok(line, "(");
        s = strtok(NULL, "(");
        token = strtok(s, " ");

        while (token)
        {
            if (token)
            {
                if (strncmp(token, "int", 3) == 0 || strncmp(token, "char", 4) == 0 || strncmp(token, "float", 5) == 0)
                {
                    type = token;
                    token = strtok(NULL, " ");
                    if (strchr(token, '*'))
                    {
                        char *temp2 = malloc(sizeof(char) * 10);
                        strcpy(temp2, type);
                        if (strstr(token, "**"))
                        {
                            strcat(temp2, "**");
                        }
                        else
                        {
                            strcat(temp2, "*");
                        }
                        token = removeStars(token);
                        strcat(varType, temp2);
                        var.varSize[numOfVars] = sizeOf(temp2, 1);
                    }
                    else
                    {
                        strcat(varType, type);
                        var.varSize[numOfVars] = sizeOf(type, 1);
                    }
                    strcat(varType, " ");
                }
                strcat(varName, token);
                numOfVars++;
                strcat(varName, " ");
            }
            token = strtok(NULL, " ");
        }
        var.varScope = getFuncName(line);
    }

    var.numOfVars = numOfVars;
    var.varName = varName;
    var.varType = varType;

    // free(name);
    // free(brackets);
    // free(number);
    return var;
}
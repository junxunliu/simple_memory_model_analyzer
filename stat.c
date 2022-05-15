#include "var.h"

/**
 * @brief Get the function name in a line
 *
 * @param line
 * @return char*
 */
char *getFuncName(char *line)
{
    int i = 1; // line[0] = " "
    int j = 0;
    char *s;
    char *name;

    name = malloc(sizeof(char) * LINE_LENGTH);

    strtok(line, "(");
    s = strchr(line, ' ');

    if (s[i] == '*')
        i++;

    while (s[i] != '\0')
    {
        name[j] = s[i];
        i++;
        j++;
    }
    name[j] = '\0';

    return name;
}

/**
 * @brief return 1 if this line is used to declare a function
 *
 * @param line
 * @return int
 */
int isFunction(char *line)
{
    if (strncmp(line, "int ", 4) == 0 || strncmp(line, "char ", 5) == 0 || strncmp(line, "float ", 6) == 0 || strncmp(line, "void ", 5) == 0)
    {
        if (!strstr(line, ";"))
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Get the file status
 * return a Stat object that contains all information
 * @param filename
 * @return Stat
 */
Stat findStatus(char *filename)
{
    FILE *fp;
    Stat s;
    int error;
    int numOfLines = 0;
    int numOfFuncs = 0;
    int numOfFuncLines = -2;           // do not include two brackets
    int leftBracket, rightBracket = 0; // a flag to check if is the line to declare a function
    char line[LINE_LENGTH + 1];
    // char funcName[SIZE];
    // char funcSize[SIZE];
    char *funcName = malloc(sizeof(char) * SIZE);
    char *funcSize = malloc(sizeof(char) * SIZE);

    strcpy(funcName, "\0");
    strcpy(funcSize, "\0");

    // funcName[0] = '\0';
    // funcSize[0] = '\0';

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file %s for reading\n", filename);
    }

    while (fgets(line, LINE_LENGTH + 1, fp) != NULL)
    {

        if (isFunction(line) == 1)
        {
            numOfFuncs += 1;
            char *name = getFuncName(line);
            strcat(funcName, name);
            strcat(funcName, " ");
            free(name);
        }
        numOfLines += 1;

        if (line[0] == '{')
        {
            leftBracket = 1;
        }

        if (line[0] == '}')
        {
            rightBracket = 1;
        }

        if (leftBracket == 1)
        {
            numOfFuncLines += 1;
        }

        if (rightBracket == 1)
        {
            char size[2];
            sprintf(size, "%d", numOfFuncLines);
            strcat(funcSize, size);
            strcat(funcSize, " ");
            numOfFuncLines = -2;
            leftBracket = 0;
            rightBracket = 0;
        }
    }

    error = fclose(fp);
    if (error != 0)
    {
        fprintf(stderr, "fclose failed\n");
    }

    s.numOfLines = numOfLines;
    s.numOfFuncs = numOfFuncs;
    s.funcName = funcName;
    s.funcSize = funcSize;
    s.var = findVariable(filename);

    return s;
}

Variable findVariable(char *filename)
{
    FILE *fp;
    int i, j;
    int error;
    char line[LINE_LENGTH + 1];
    Variable var;
    char *varType = malloc(sizeof(char) * SIZE);
    char *varName = malloc(sizeof(char) * SIZE);
    char *varScope = malloc(sizeof(char) * SIZE);

    var.numOfVars = 0;

    strcpy(varType, "\0");
    strcpy(varName, "\0");
    strcpy(varScope, "\0");

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file %s for reading\n", filename);
    }

    while (fgets(line, LINE_LENGTH + 1, fp) != NULL)
    {

        line[strcspn(line, "\n")] = 0;
        if (isFunction(line) == 1 || isVariable(line) == 1 || isGlobal(line) == 1)
        {
            Variable tempVar;
            char *tempScope;
            tempVar = getVar(line);
            i = var.numOfVars;
            if (isFunction(line) == 1)
            {
                tempScope = tempVar.varScope;
            }
            char *varCpy = malloc(sizeof(char) * SIZE);
            char *savePtr;
            strcpy(varCpy, tempVar.varName);
            char *varToken = strtok_r(varCpy, " ", &savePtr);
            // printf("|varName = %s|\n", tempVar.varName);
            // printf("|varCpy = %s|\n", varCpy);
            while (varToken)
            {
                if (strcmp(tempVar.varScope, "default") == 0)
                {
                    strcat(varScope, tempScope);
                }
                else
                {
                    strcat(varScope, tempVar.varScope);
                }
                strcat(varScope, " ");
                varToken = strtok_r(NULL, " ", &savePtr);
            }
            strcat(varName, tempVar.varName);
            strcat(varType, tempVar.varType);

            for (j = 0; j < tempVar.numOfVars; j++)
            {
                var.varSize[i] = tempVar.varSize[j];
                i++;
            }
            var.numOfVars += tempVar.numOfVars;

            free(tempVar.varName);
            free(tempVar.varType);
            free(varCpy);
        }
    }

    error = fclose(fp);
    if (error != 0)
    {
        fprintf(stderr, "fclose failed\n");
    }

    var.varType = varType;
    var.varName = varName;
    var.varScope = varScope;

    return var;
}

void findHeap(char *filename, char *varType, char *varName, char *varScope)
{
    FILE *fp;
    int error;
    char line[LINE_LENGTH + 1];
    char *scope;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file %s for reading\n", filename);
    }

    scope = "global";

    while (fgets(line, LINE_LENGTH + 1, fp) != NULL)
    {

        if (isFunction(line) == 1)
        {
            scope = getFuncName(line);
        }

        if (strstr(line, "malloc(") || strstr(line, "calloc("))
        {
            char *token;
            char name[40];
            char newName[40];
            char type[40];
            int number = 0;

            type[0] = '\0';
            name[0] = '\0';
            newName[0] = '\0';

            // get the number inside brackets if has
            char *numPtr = strchr(line, '(');
            for (int i = 0; i < strlen(numPtr); i++)
            {
                if (isdigit(numPtr[i]))
                {
                    number = number * 10 + (numPtr[i] - '0');
                }
            }

            if (number == 0)
            {
                number = 1;
            }

            token = strtok(line, " ");

            if (strcmp(token, "int") == 0 || strcmp(token, "char") == 0 || strcmp(token, "float") == 0)
            {
                strcpy(type, token);
                strcpy(name, strtok(NULL, " "));
                if (strstr(name, "**"))
                {
                    strcat(type, "*");
                    memmove(&name[0], &name[1], strlen(name) * sizeof(name[0]));
                }
                strcat(type, "*");
                memmove(&name[0], &name[1], strlen(name) * sizeof(name[0]));
            }
            else
            {
                strcpy(name, token);
                char *nameToken, *typeToken, *scopeToken;
                char *namePtr, *typePtr, *scopePtr;
                int j = 0, index = 0;

                char varScopeHeap[SIZE], varNameHeap[SIZE], varTypeHeap[SIZE];

                strcpy(varScopeHeap, varScope);
                strcpy(varNameHeap, varName);
                strcpy(varTypeHeap, varType);

                nameToken = strtok_r(varNameHeap, " ", &namePtr);
                typeToken = strtok_r(varTypeHeap, " ", &typePtr);
                scopeToken = strtok_r(varScopeHeap, " ", &scopePtr);

                // get the delimiter index;
                while (nameToken)
                {
                    if (strcmp(nameToken, name) == 0 && strcmp(scopeToken, scope) == 0)
                    {
                        break;
                    }
                    index++;
                    nameToken = strtok_r(NULL, " ", &namePtr);
                    scopeToken = strtok_r(NULL, " ", &scopePtr);
                }

                // get the variable type

                while (j < index)
                {
                    typeToken = strtok_r(NULL, " ", &typePtr);
                    j++;
                }
                strcpy(type, typeToken);
            }

            if (strstr(type, "**"))
            {
                strcat(newName, "*");
                type[strlen(type) - 1] = '\0';
            }
            strcat(newName, "*");
            strcat(newName, name);
            type[strlen(type) - 1] = '\0';

            printf("   %s   %s   %s   %d\n", newName, scope, type, sizeOf(type, number));
        }
    }

    error = fclose(fp);
    if (error != 0)
    {
        fprintf(stderr, "fclose failed\n");
    }
}

void findLiteral(char *filename)
{
    FILE *fp;
    int error;
    char line[LINE_LENGTH + 1];
    char *scope, *type, *name;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file %s for reading\n", filename);
    }

    scope = "global";
    type = "char[]";

    while (fgets(line, LINE_LENGTH + 1, fp) != NULL)
    {

        if (isFunction(line) == 1)
        {
            scope = getFuncName(line);
        }

        if (!strchr(line, '[') && strchr(line, '=') && strchr(line, '"'))
        {
            strcpy(line, trim(line));
            const char *ptr = strchr(line, '"');
            int count = 0;
            for (int i = 0; i < strlen(ptr) - 3; i++)
            {
                count++;
            }
            name = strtok(line, " ");
            if (strcmp(name, "char") == 0)
            {
                name = strtok(NULL, " ");
                printf("   %s   %s   %s   %d\n", name, scope, type, sizeOf("char", count));
            }
            else
            {
                char buf[40];
                buf[0] = '\0';
                strcpy(buf, "*");
                strcat(buf, name);
                printf("   %s   %s   %s   %d\n", buf, scope, type, sizeOf("char", count));
            }
        }
    }

    error = fclose(fp);
    if (error != 0)
    {
        fprintf(stderr, "fclose failed\n");
    }
}
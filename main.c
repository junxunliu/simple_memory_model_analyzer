#include "var.h" // var.h included stat.h


int main(int argc, char *argv[])
{
    Stat s;

    for (int i = 1; i < argc; i++)
    {

        char *input = argv[i];

        s = findStatus(input);

        printf(">>> Memory Model Layout <<<\n\n");
        printf("***  exec // text ***\n");
        printf("   %s\n\n", input);
        // ROData - string literals
  
        printf("### ROData ###       scope type  size\n");
        findLiteral(input);
        printf("\n");

        char *tScope, *tName, *tType;
        char *pScope, *pName, *pType;
        char varScope[SIZE], varName[SIZE], varType[SIZE];
        int  sizeIndex = 0;

        strcpy(varScope, s.var.varScope);
        strcpy(varName, s.var.varName);
        strcpy(varType, s.var.varType);

        tScope = strtok_r(varScope, " ", &pScope);
        tName = strtok_r(varName, " ", &pName);
        tType = strtok_r(varType, " ", &pType);

        // Static Data
        printf("### static data ###\n");

        while (strcmp(tScope, "global") == 0) {
            printf("   %s   %s   %s   %d\n", tName, tScope, tType, s.var.varSize[sizeIndex]);
            tScope = strtok_r(NULL, " ", &pScope);
            tName = strtok_r(NULL, " ", &pName);
            tType = strtok_r(NULL, " ", &pType);
            sizeIndex++;
        }
    
        printf("\n");

        // Heap
        printf("### heap ###\n");
        
        findHeap(input, s.var.varType, s.var.varName, s.var.varScope);

        printf("\n");

        // Unused space
        printf("####################\n### unused space ###\n####################\n");
        printf("\n");

        // Stack
        printf("### stack ###\n");

        while (tScope && tName && tType) {
            printf("   %s   %s   %s   %d\n", tName, tScope, tType, s.var.varSize[sizeIndex]);
            tScope = strtok_r(NULL, " ", &pScope);
            tName = strtok_r(NULL, " ", &pName);
            tType = strtok_r(NULL, " ", &pType);
            sizeIndex++;
        }
        
        printf("\n");

        // Status
        char *tokenName, *tokenNameL, *tokenSizeL;
        char *savePtrN, *savePtrNL, *savePtrSL, *savePtrNV, *savePtrSV;

        printf("**** STATS ****\n");

        printf("  - Total number of lines in the file: %d\n", s.numOfLines);

        // print list of function name
        printf("  - Total number of functions: %d\n", s.numOfFuncs);

        char funcNameCpy[SIZE];
        strcpy(funcNameCpy, s.funcName);

        tokenName = strtok_r(funcNameCpy, " ", &savePtrN);

        printf("    %s", tokenName);
        while (tokenName)
        {
            tokenName = strtok_r(NULL, " ", &savePtrN);
            if (tokenName)
            {
                printf(", %s", tokenName);
            }
        }
        printf("\n");

        // print list of function name with num of lines
        printf("  - Total number of lines per functions:\n");

        char funcNameCpy2[SIZE];
        char funcSizeCpy2[SIZE];
        strcpy(funcNameCpy2, s.funcName);
        strcpy(funcSizeCpy2, s.funcSize);

        tokenNameL = strtok_r(funcNameCpy2, " ", &savePtrNL);
        tokenSizeL = strtok_r(funcSizeCpy2, " ", &savePtrSL);

        char *funcNames = malloc(sizeof(char) * SIZE);
        strcpy(funcNames, "\0");

        while (tokenNameL && tokenSizeL)
        {
            printf("    %s: %s\n", tokenNameL, tokenSizeL);
            strcat(funcNames, tokenNameL);
            strcat(funcNames, " ");
            tokenNameL = strtok_r(NULL, " ", &savePtrNL);
            tokenSizeL = strtok_r(NULL, " ", &savePtrSL);
        }

        // print list of function name with num of variables
        printf("  - Total number of variables per function:\n");

        char varScopeCpy[SIZE];
        char funcNamesCpy[SIZE];

        int varNums[s.numOfFuncs];
        char *tokenFuncNames;
        char *tokenFuncNamesCpy;
        char *tokenScope;
        int count = 0;
        int index = 0;

        strcpy(varScopeCpy, s.var.varScope);
        strcpy(funcNamesCpy, funcNames);

        tokenFuncNames = strtok_r(funcNames, " ", &savePtrNV);
        tokenScope = strtok_r(varScopeCpy, " ", &savePtrSV);

        while (tokenScope)
        {

            if (tokenFuncNames)
            {   
                if (strcmp(tokenScope, "global") == 0)
                {
                    tokenScope = strtok_r(NULL, " ", &savePtrSV);
                    continue;
                }
                if (strcmp(tokenFuncNames, tokenScope) == 0)
                {   
                    count++;
                    varNums[index] = count;
                    tokenScope = strtok_r(NULL, " ", &savePtrSV);
                }

                else
                {
                    index++;
                    count = 0;
                    tokenFuncNames = strtok_r(NULL, " ", &savePtrNV);
                }
            }
            
        }

        char *savePtrFN;
        tokenFuncNamesCpy = strtok_r(funcNamesCpy, " ", &savePtrFN);
        int index2 = 0;

        while (tokenFuncNamesCpy)
        {

            printf("    %s: %d\n", tokenFuncNamesCpy, varNums[index2]);
            index2++;

            tokenFuncNamesCpy = strtok_r(NULL, " ", &savePtrFN);
        }

        printf("//////////////////////////////\n");

        // printf("varscope=%s\n", s.var.varScope);
        // printf("varName=%s\n", s.var.varName);
        // printf("varType=%s\n", s.var.varType);

        free(funcNames);
        
        free(s.funcName);
        free(s.funcSize);
        free(s.var.varName);
        free(s.var.varType);
        free(s.var.varScope);
    }
}
#include "../Headers/textFunc.hpp"
#include "../Headers/stack.hpp"
#include "../Headers/processor.hpp"

int main(int argc, char* argv[])
{
    assembler(argv[1]);

    return 0;
}

int assembler(char* codeFile)
{
    Text codeText {};
    codeData codeCoating {};

    //PrintTextStdout(&text);
    int error = codeDataCtor(codeFile, &codeText, &codeCoating);
    if (error)
        return error;

    assemble(&codeText, &codeCoating);

    return 0;
}

int codeDataCtor (char* codeFile, Text* codeText, codeData* codeCoating)
{
    int Error = TextReadFromFile(codeText, codeFile);
    if(Error)
    {
        fprintf(stderr, "%s\n", TextGetError(Error));
        return 0;
    }

    codeCoating->cmdArr = (char**)calloc(codeText->nLines, sizeof(char*));
    codeCoating->codeBuf = (char *)calloc(1, sizeof(Signature) + codeText->nLines + codeText->nLines * sizeof(elem_t));
    codeCoating->labels = (Label*)calloc(MAX_LABELS, sizeof(Label));

    if ((codeCoating->cmdArr == NULL) || (codeCoating->codeBuf == NULL) || (codeCoating->labels == NULL))
        return MEM_ERROR;

    for (int i = 0; i < codeText->nLines; i++)
    {
        codeCoating->cmdArr[i] = codeText->lines[i].line;
    }

    return 0;
}

int codeDataDtor (Text* codeText, codeData* codeCoating)
{
    free(codeCoating->cmdArr);
    free(codeCoating->codeBuf);
    free(codeCoating->labels);
    TextDestroy(codeText);

    return 0;
}

int assemble(Text* codeText, codeData* codeCoating)
{
    int error = firstPass(codeText->nLines, codeCoating);
    if (error < 0)
    {
        codeDataDtor(codeText, codeCoating);
        return error;
    }
    //debugPrintCode(&codeCoating);
    error = secondPass(codeText->nLines, codeCoating);
    if (error < 0)
    {
        codeDataDtor(codeText, codeCoating);
        return error;
    }

    FILE* code_txt = fopen("code.txt", "wb");
    fwrite(codeCoating->codeBuf, sizeof(char), codeCoating->ip, code_txt);
    fclose(code_txt);

    codeDataDtor(codeText, codeCoating);

    return 0;
}

int firstPass(int codeLines, codeData* codeCoating)
{
    int labelsNum = 1;
    int labelCallsNum = 0;
    char** cmdArr = codeCoating->cmdArr;

    for (int i = 0; i < codeLines; i++)
    {
        //debugPrintCode(codeCoating);

        char cmdBuf [BUFSIZ] = { 0 };
        sscanf(cmdArr[i], "%s", cmdBuf);

        if (checkIfLabel(cmdBuf))
        {
            //printf("got a label %s\n", cmdBuf);
            if (labelsNum == MAX_LABELS)
            {
                return LABEL_OVERFLOW;
            }
            codeCoating->labels[labelsNum].address = codeCoating->ip;
            codeCoating->labels[labelsNum].name = strdup(cmdBuf);
            labelsNum++;
            continue;
        }
        //printf("In cmdBuf: %s\n", cmdBuf);

        int error = cmdToCode(cmdBuf, codeCoating->codeBuf + codeCoating->ip);
        if (error == EMPTY_LINE)
            continue;
        if (error == UNKNOWN_CMD)
        {
            fprintf(stderr, "An error occured with line %d, scaned command \"%s\" not recognized\n", i, cmdBuf);
            return UNKNOWN_CMD;
        }
        (codeCoating->ip)++;

        //printf("\nbefore push check = %d\n", codeBuf[ip - 1]);
        if (codeCoating->codeBuf[codeCoating->ip - 1] == CMD_PUSH)
        {
            int cmdAddr = codeCoating->ip - 1;
            int len = strlen("push");
            //printf("\nbefore argument parse = %d\n", codeBuf[cmdAddr]);
            int argScanned = parseArgs(cmdArr[i] + len, codeCoating, cmdAddr);
            if (argScanned == INVALID_ARG)
            {
                fprintf(stderr, "An error occured with line %d, returned calue - %d\n", i, argScanned);
                return INVALID_ARG;
            }

        }

        else if (codeCoating->codeBuf[codeCoating->ip - 1] == CMD_POP)
        {
            //printf("\nin pop search cycle, ip = %d\n", ip);
            int cmdAddr = codeCoating->ip - 1;
            int len = strlen("pop");

            int argScanned = parseArgs(cmdArr[i] + len, codeCoating, cmdAddr);
            if (argScanned == INVALID_ARG)
            {
                fprintf(stderr, "An error occured with line %d, returned calue - %d\n", i, argScanned);
                return INVALID_ARG;
            }
            if (codeCoating->codeBuf[cmdAddr] & ARG_IMM && !(codeCoating->codeBuf[cmdAddr] & ARG_RAM))
            {
                fprintf(stderr, "Tried to pop into imm value\n\n");
                return INVALID_ARG;
            }
        }

        else if (checkIfJmp(codeCoating->codeBuf[codeCoating->ip - 1]))
        {
            codeCoating->codeBuf[codeCoating->ip] = -1;
            codeCoating->ip++;
        }

    }
    codeCoating->labelsNum = labelsNum;
    //printf("bytes in func needed - %d\n", codeCoating->ip);

    return 0;
}

int secondPass(int codeLines, codeData* codeCoating)
{
    char cmd = 0;
    char* placeForLabelAddr = codeCoating->codeBuf;
    int labelAddr = 0;
    char** cmdArr = codeCoating->cmdArr;


    for (int i = 0; i < codeLines; i++)
    {
        char cmdBuf [BUFSIZ] = { 0 };
        char* labelName;
        sscanf(cmdArr[i], "%s", cmdBuf);
        if (checkIfLabel(cmdBuf))
            continue;

        int res = cmdToCode(cmdBuf, &cmd);
        if (res == EMPTY_LINE)
            continue;
        if (checkIfJmp(cmd))
        {
            labelName = strtok(cmdArr[i] + strlen(cmdBuf), " ");
            //printf("need to go to label \"%s\"\n", labelName);
            labelAddr = searchForLabelAddr(labelName, codeCoating);
            if (labelAddr < 0)
            {
                fprintf(stderr, "could not get label address of %s\n", labelName);
                return labelAddr;
            }
            while (*placeForLabelAddr != -1)
                placeForLabelAddr++;
            //printf("place for label address - %p, labelAddr = %d\n", placeForLabelAddr, labelAddr);
            *placeForLabelAddr = labelAddr;
        }
    }

    return 0;
}

int parseArgs(char* argStr, codeData* codeCoating, int cmdAddr)
{
    int ip = codeCoating->ip;
    //printf("got ip = %d\n", ip);
    //printf("got arg %s, codebuf[cmdAddr] = %d\n", argStr, codeBuf[cmdAddr]);
    argStr = removeComment(argStr);
    //printf("after removing comment arg %s\n", argStr);
    int retVal = 0;
    if (checkForRAM(argStr))
    {
        codeCoating->codeBuf[cmdAddr] |= ARG_RAM;
        //printf("after removing RAM braces arg %s\n", argStr);
    }

    char* wordPtr, *savePtr;
    char delim[] = " ";
    int imm, reg;
    int scanningOp = 0;
    int scannedImm = 0;
    int scannedReg = 0;
    for(wordPtr = strtok_r(argStr, delim, &savePtr); (wordPtr != NULL); wordPtr = strtok_r(NULL, delim, &savePtr))
    {
        //printf("got %s\n", wordPtr);

        if (scanningOp)
        {
            if (strcmp(wordPtr, "+"))
            {
                fprintf(stderr, "got %s instead of a '+'\n", wordPtr);
                return INVALID_ARG;
            }

            scanningOp = 0;
        }
        else
        {
            int scanRes = sscanf(wordPtr, "%d", &imm);

            if (scanRes == 1)
            {
                if (scannedImm)
                {
                    fprintf(stderr, "got number as a argument twice\n");
                    return INVALID_ARG;
                }
                //printf("scanning imm\n");
                codeCoating->codeBuf[cmdAddr] |= ARG_IMM;

                scannedImm = 1;
            }
            else
            {
                if (scannedReg)
                {
                    fprintf(stderr, "got register as a argument twice\n");
                    return INVALID_ARG;
                }
                //printf("scanning reg \"%s\"\n", wordPtr);
                reg = regToCode(wordPtr);
                if (reg == SYNTAX_ERROR)
                    return INVALID_ARG;

                scannedReg = 1;
            }

            scanningOp = 1;
        }

    }
    if (scannedReg)
    {
        codeCoating->codeBuf[ip] = reg;
        codeCoating->codeBuf[cmdAddr] |= ARG_REG;
        ip += sizeof(char);
    }
    if (scannedImm)
    {
        *(int*)(codeCoating->codeBuf + ip) = imm;
        codeCoating->codeBuf[cmdAddr] |= ARG_IMM;
        ip += sizeof(elem_t);
    }
    //printf("returning %d\n", codeBuf[cmdAddr]);
    codeCoating->ip = ip;

    return 0; // wtf
}

char* removeComment(char *str)
{
    char* commPtr;
    if ((commPtr = strchr(str, ';')) != NULL)
        *commPtr = '\0';
    
    return str;
}

int checkForRAM (char* str)
{
    char *bracePtr;
    if ((bracePtr = strchr(str, '[')) != NULL)
    {
        *bracePtr = ' ';
        if ((bracePtr = strchr(str, ']')) == NULL)
            return INVALID_ARG;
        else
        *bracePtr = '\0';
    }
    else
        return 0;

    return 1;
}

int searchForLabelAddr(const char* labelName, const codeData* codeCoating)
{
    int labelsNum = codeCoating->labelsNum;
    for (int i = 1; i < labelsNum; i++)
    {
        //printf("comparing \"%s\" with \"%s\"\n", labelName, codeCoating->labels[i].name);
        if (!strcmp(labelName, codeCoating->labels[i].name))
            return codeCoating->labels[i].address;
    }
    return SYNTAX_ERROR;
}


int checkIfLabel(char* cmdBuf)
{
    char* labelPtr;
    cmdBuf = removeComment(cmdBuf);
    if ((labelPtr = strchr(cmdBuf, ':')) != NULL) // everything after ':' symbol counts as comments for now
    {
        *labelPtr = '\0';
        return 1;
    }
    return 0;
}

int checkIfJmp(int cmdNum)
{
    if (cmdNum >= CMD_JA && cmdNum <= CMD_CALL)
        return 1;
    return 0;
}


int cmdToCode(char* cmd, char* codeBuf)
{
    char* ifEmpty;
    if ( (ifEmpty = strtok(cmd, " ")) == NULL)
        return EMPTY_LINE;
    for (int i = 0; i < sizeof(commandsArr)/ sizeof(commandsArr[0]); i++)
    {
        if(strcmp(cmd, commandsArr[i].cmdName) == 0)
        {
            codeBuf[0] = commandsArr[i].cmdNum;
            return 1;
        }
    }
    fprintf(stderr, "got unknown command \"%s\"\n", cmd);
    return UNKNOWN_CMD;
}

int regToCode(char* reg)
{
    if (strcmp(reg, "ax") == 0)
    {
        return 0x01;
    }
    if (strcmp(reg, "bx") == 0)
    {
        return 0x02;
    }
    if (strcmp(reg, "cx") == 0)
    {
        return 0x03;
    }
    if (strcmp(reg, "dx") == 0)
    {
        return 0x04;
    }
    fprintf(stderr, "got unknown argument \"%s\"\n", reg);
    return SYNTAX_ERROR;
}


int debugPrintCode(codeData* codeCoating)
{
    printf("ip - %d\n", codeCoating->ip);
    fwrite(codeCoating->codeBuf, sizeof(char), codeCoating->ip, stdout);
    printf("\n");

    for (int i = 1; i < codeCoating->labelsNum; i++)
    {
        printf("label \"%s\" with addr %d\n", codeCoating->labels[i].name, codeCoating->labels[i].address);
    }
    printf("\n");

    return 0;
}
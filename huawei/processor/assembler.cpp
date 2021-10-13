#include <stdarg.h>
#include "struct.hpp"
#include "ReadWrite.hpp"
#include "stack.hpp"
#include "processor.hpp"

#define ErrorToLog(Error) \
{\
    if(Error)\
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);\
}

int CmdToCode(char* cmdbuf, int num, int* CodeBuf);

int main(int argc, char* argv[])
{
    TEXT text {};
    int Error = TEXT_ReadFromFile(&text, argv[1]);
    if(Error)
    {
        printf("%s\n", GetError(Error));
        return 0;
    }

    //PrintTextStdout(&text);

    char** CmdArr = (char**)calloc(text.nLines, sizeof(char*));
    int* CodeBuf = (int *)calloc(text.nLines * 2, sizeof(int));

    for (int i = 0; i < text.nLines; i++)
    {
        CmdArr[i] = text.lines[i].line;
    }

    char cmdbuffer [1000] = { 0 };
    int num = 0;
    int ip = 0;
    for (int i = 0; i < text.nLines; i++)
    {
        sscanf(CmdArr[i], "%s %d", cmdbuffer, &num);
        ip += CmdToCode(cmdbuffer, num, CodeBuf + ip);

        // printf("ip = %d\n", ip);
        // for (int i = 0; i < ip; i++)
        // {
        //     printf("%d ", CodeBuf[i]);
        // }
        // printf("\n");
    }

    FILE* code_txt = fopen("code.txt", "wb");
    fwrite(CodeBuf, sizeof(int), ip, code_txt);
    fclose(code_txt);

    TEXT_Destroy(&text);
    return 0;
}


int CmdToCode(char* cmd, int num, int* CodeBuf)
{
    if(strcmp(cmd, "push") == 0)
    {
        CodeBuf[0] = CMD_PUSH;
        CodeBuf[1] = num;
        return 2;
    }

    if(strcmp(cmd, "pop") == 0)
    {
        CodeBuf[0] = CMD_POP;
        return 1;
    }

    if(strcmp(cmd, "add") == 0)
    {
        CodeBuf[0] = CMD_ADD;
        return 1;
    }

    if(strcmp(cmd, "sub") == 0)
    {
        CodeBuf[0] = CMD_SUB;
        return 1;
    }

    if(strcmp(cmd, "mul") == 0)
    {
        CodeBuf[0] = CMD_MUL;
        return 1;
    }

    if(strcmp(cmd, "out") == 0)
    {
        CodeBuf[0] = CMD_OUT;
        return 1;
    }

    if(strcmp(cmd, "ver") == 0)
    {
        CodeBuf[0] = CMD_VER;
        return 1;
    }

    if(strcmp(cmd, "dmp") == 0)
    {
        CodeBuf[0] = CMD_DMP;
        return 1;
    }

    if(strcmp(cmd, "hlt") == 0)
    {
        CodeBuf[0] = CMD_HLT;
        return 1;
    }

    printf("Got an unknown command: \"%s\", stopping assembling\n", cmd);
    exit(1);
}

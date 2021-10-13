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

FILE* lst = fopen("lst.txt", "w");

int CmdToCode(char* cmdbuf, int num, char* CodeBuf, int ip);
int PrintToLst (int addr, const char* CmdName, char* Codebuf, int num = 0);

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
    char* CodeBuf = (char *)calloc(text.nLines * 2, sizeof(int));

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
        ip += CmdToCode(cmdbuffer, num, CodeBuf + ip, ip);
    }
    fclose(lst);

    // printf("ip = %d\n", ip);
    // for (int i = 0; i < ip; i++)
    //     {
    //         printf("%d ", CodeBuf[i]);
    //     }
    // printf("\n");

    FILE* code_txt = fopen("code.txt", "wb");
    fwrite(CodeBuf, sizeof(char), ip, code_txt);
    fclose(code_txt);

    TEXT_Destroy(&text);
    return 0;
}


int CmdToCode(char* cmd, int num, char* CodeBuf, int ip)
{
    if(strcmp(cmd, "push") == 0)
    {
        CodeBuf[0] = CMD_PUSH;
        *(int*)(CodeBuf + 1) = num;
        PrintToLst(ip, "PUSH", CodeBuf, num);
        return (sizeof(char) + sizeof(int));
    }

    if(strcmp(cmd, "pop") == 0)
    {
        CodeBuf[0] = CMD_POP;
        PrintToLst(ip, "POP", CodeBuf);
        return 1;
    }

    if(strcmp(cmd, "add") == 0)
    {
        CodeBuf[0] = CMD_ADD;
        PrintToLst(ip, "ADD", CodeBuf);
        return 1;
    }

    if(strcmp(cmd, "sub") == 0)
    {
        CodeBuf[0] = CMD_SUB;
        PrintToLst(ip, "SUB", CodeBuf);
        return 1;
    }

    if(strcmp(cmd, "mul") == 0)
    {
        CodeBuf[0] = CMD_MUL;
        PrintToLst(ip, "MUL", CodeBuf);
        return 1;
    }

    if(strcmp(cmd, "out") == 0)
    {
        CodeBuf[0] = CMD_OUT;
        PrintToLst(ip, "OUT", CodeBuf);
        return 1;
    }

    if(strcmp(cmd, "ver") == 0)
    {
        CodeBuf[0] = CMD_VER;
        PrintToLst(ip, "VER", CodeBuf);
        return 1;
    }

    if(strcmp(cmd, "dmp") == 0)
    {
        CodeBuf[0] = CMD_DMP;
        PrintToLst(ip, "DMP", CodeBuf);
        return 1;
    }

    if(strcmp(cmd, "hlt") == 0)
    {
        CodeBuf[0] = CMD_HLT;
        PrintToLst(ip, "HLT", CodeBuf);
        return 1;
    }

    printf("Got an unknown command: \"%s\", stopping assembling\n", cmd);
    exit(1);
}

int PrintToLst (int addr, const char* CmdName, char* Codebuf, int num)
{
    switch (Codebuf[0])
    {
        case CMD_PUSH:  fprintf(lst, "%04d | %02d | %02hhX %02hhX %02hhX %02hhX | %s %02d\n", addr, Codebuf[0], Codebuf[1], Codebuf[2], Codebuf[3], Codebuf[4], CmdName, num);
                        break;
        default: fprintf(lst, "%04d | %02d | %11c | %s\n", addr, Codebuf[0], ' ', CmdName);
                 break;
    }
    return 0;
}

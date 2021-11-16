#include <stdarg.h>
#include <errno.h>
#include "../Headers/struct.hpp"
#include "../Headers/ReadWrite.cpp"
#include "../Headers/stack.cpp"
#include "../Headers/processor.hpp"

enum Codes
{
    CHAR = 0,
    INT
};

int CmdToCode(char* cmdbuf, char* codeBuf);
int ArgToCode(char* arg, char* codeBuf, int* ip, int cp, int searchFor);
int RegToCode(char* reg, char* codeBuf);

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

    char** cmdArr = (char**)calloc(text.nLines, sizeof(char*));
    char* codeBuf = (char *)calloc(1, sizeof(Signature) + text.nLines * 2 * sizeof(elem_t));

    for (int i = 0; i < text.nLines; i++)
    {
        cmdArr[i] = text.lines[i].line;
    }

    int num = 0;
    int ip = 0;
    int scaned = 0;

    for (int i = 0; i < text.nLines; i++)
    {
        char cmdBuf [1000] = { 0 };
        sscanf(cmdArr[i], "%s", cmdBuf);

        ip += CmdToCode(cmdBuf, codeBuf + ip);

        if (codeBuf[ip - 1] == CMD_PUSH)
        {
            int cp = ip - 1;
            int len = strlen("push") + 1;
            int argscanned = 0;

            if (cmdArr[i][len] == '[')
            {
                codeBuf[cp] |= 0x80;
                len++;
            }

            int aboba = INT;

            for (int j = 0; (j < 2) && (cmdArr[i][len] != '\0') && (cmdArr[i][len] != '\n'); j++)
            {
                scaned = ArgToCode(cmdArr[i] + len, codeBuf, &ip, cp, aboba);
                if (scaned)
                    argscanned++;
                if ((scaned == 0) && (j == 1) && (argscanned == 0))
                {
                    printf("got an unknown argument for push in line %d: \"%s\"; exiting the programm", i, cmdArr[i] + len);
                    exit(1);
                }

                aboba = CHAR;
            }

            // scaned += sscanf(cmdArr[i] + len, "%s", cmdBuf + len);
            // printf("got additionally - %s, %d\n", cmdBuf + len, num);

            // len += strlen(cmdBuf + len);
            // printf("%c", cmdArr[i][len]);

            // if (cmdArr[i][len] == '\n' || cmdArr[i][len] == '\0')
            //     continue;
            
            // scaned += sscanf(cmdArr[i] + len, "%s", cmdBuf + len);

            // for (int j = 0; j < 20; j++)
            //     printf("%c", cmdBuf[j]);
            // printf("\n");

            // scaned = 0;
        }

    }

    FILE* code_txt = fopen("code.txt", "wb");
    fwrite(codeBuf, sizeof(char), ip, code_txt);
    fclose(code_txt);

    TEXT_Destroy(&text);
    return 0;
}


int ArgToCode(char* arg, char* codeBuf, int* ip, int cp, int searchFor)
{
    int num = 0;
    char regName [2] = {0};
    char forSkip[1000] = { 0 };
    int len = 0;
    int scaned = 0;

    if (searchFor == INT)
    {
        sscanf(arg, "%[^0-9\n]", forSkip);
        len = strlen(forSkip);

        scaned = sscanf(arg + len, "%d", &num);
        //printf("scaned = %d, int = %d, len of buf - %d, ABOBA = %d\n", scaned, num, len, searchFor);

        // for (int i = 0; i < len; i++)
        //     printf("%c", forSkip[i]);
        // printf("\n");

        if (scaned == 1)
        {
            *(int*)(codeBuf + *ip) = num;

            codeBuf[cp] |= 0x20;
            *ip += sizeof(elem_t);

            return (1 + strlen(forSkip));
        }
    }

    if (searchFor == CHAR)
    {
        sscanf(arg, "%[ +0-9]", forSkip);
        len = strlen(forSkip);
        if (arg[len] == '\n' || arg[len] == '\0')
            return 0;

        scaned = sscanf(arg + len, "%2s", regName);
        //printf("scaned = %d, regName - %2s len of buf - %d\n", scaned, regName, len);
        if (scaned == 0)
            return 0;

        RegToCode(regName, codeBuf + *ip);
        codeBuf[cp] |= 0x40;
        *ip += sizeof(char);

        return (2  + strlen(forSkip));
    }

    return 0;
}



int CmdToCode(char* cmd, char* codeBuf)
{
    if(strcmp(cmd, "push") == 0)
    {
        codeBuf[0] = CMD_PUSH;
        return 1;
    }

    if(strcmp(cmd, "pop") == 0)
    {
        codeBuf[0] = CMD_POP;
        return 1;
    }

    if(strcmp(cmd, "add") == 0)
    {
        codeBuf[0] = CMD_ADD;
        return 1;
    }

    if(strcmp(cmd, "sub") == 0)
    {
        codeBuf[0] = CMD_SUB;
        return 1;
    }

    if(strcmp(cmd, "mul") == 0)
    {
        codeBuf[0] = CMD_MUL;
        return 1;
    }

    if(strcmp(cmd, "out") == 0)
    {
        codeBuf[0] = CMD_OUT;
        return 1;
    }

    if(strcmp(cmd, "ver") == 0)
    {
        codeBuf[0] = CMD_VER;
        return 1;
    }

    if(strcmp(cmd, "dmp") == 0)
    {
        codeBuf[0] = CMD_DMP;
        return 1;
    }

    if(strcmp(cmd, "hlt") == 0)
    {
        codeBuf[0] = CMD_HLT;
        return 1;
    }

    printf("Got an unknown command: \"%s\", stopping assembling\n", cmd);
    exit(1);
}

int RegToCode(char* reg, char* codeBuf)
{
    if (strncmp(reg, "ax", 2) == 0)
    {
        codeBuf[0] = 0x01;
        return 1;
    }
    if (strncmp(reg, "bx", 2) == 0)
    {
        codeBuf[0] = 0x02;
        return 1;
    }
    if (strncmp(reg, "cx", 2) == 0)
    {
        codeBuf[0] = 0x03;
        return 1;
    }
    if (strncmp(reg, "dx", 2) == 0)
    {
        codeBuf[0] = 0x04;
        return 1;
    }
    printf("Got an unknown register name: \"%s\", stopping assembling\n", reg);
    exit(1);
 }
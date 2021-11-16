#include <stdarg.h>
#include "../Headers/struct.hpp"
#include "../Headers/ReadWrite.hpp"
#include "../Headers/stack.hpp"
#include "../Headers/processor.hpp"

#define ErrorToLog(Error) \
{\
    if(Error)\
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);\
}

enum Codes
{
    CHAR = 0,
    INT
};

int CmdToCode(char* cmdbuf, char* codeBuf);
int ArgToCode(char* arg, char* codeBuf, int* ip, int cp, int searchFor);
int RegToCode(char* reg, char* codeBuf);
int assembly(TEXT* text, char** cmdArr, char* codeBuf, Label* labels);

int main(int argc, char* argv[])
{
    TEXT text {};
    int Error = TEXT_ReadFromFile(&text, argv[1]);
    if(Error)
    {
        printf("%s\n", TEXT_GetError(Error));
        return 0;
    }

    //PrintTextStdout(&text);

    char** cmdArr = (char**)calloc(text.nLines, sizeof(char*));
    char* codeBuf = (char *)calloc(1, sizeof(Signature) + text.nLines * 2 * sizeof(elem_t));
    Label* labels = (Label*)calloc(20, sizeof(Label));

    for (int i = 0; i < text.nLines; i++)
    {
        cmdArr[i] = text.lines[i].line;
    }

    FILE* code_txt = fopen("code.txt", "wb");

    int nBytes = assembly(&text, cmdArr, codeBuf, labels);

    fwrite(codeBuf, sizeof(char), nBytes, code_txt);
    fclose(code_txt);

    TEXT_Destroy(&text);
    return 0;
}


int assembly(TEXT* text, char** cmdArr, char* codeBuf, Label* labels)
{
    int num = 0;
    int ip = 0;
    int scaned = 0;

    for (int i = 0; i < text->nLines; i++)
    {
        char cmdBuf [1000] = { 0 };
        sscanf(cmdArr[i], "%s", cmdBuf);
        printf("In cmdBuf: %s\n", cmdBuf);


        ip += CmdToCode(cmdBuf, codeBuf + ip);

        //printf("\nbefore push check = %d\n", codeBuf[ip - 1]);
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
                //printf("\nin push search cycle, ip = %d\n", ip);
                scaned = ArgToCode(cmdArr[i] + len, codeBuf, &ip, cp, aboba);
                if (scaned)
                    argscanned++;

                //printf("\nbefore in arg search: aboba = %d,codebuf[ip - 1] = %d\n", aboba, codeBuf[ip - 1]);

                if ((j == 1) && (argscanned == 0))
                {
                    printf("got an unknown argument for push in line %d: \"%s\"; exiting the programm", i, cmdArr[i] + len);
                    exit(1);
                }

                aboba = CHAR;
            }

        }

        else if (codeBuf[ip - 1] == CMD_POP)
        {
            //printf("\nin pop search cycle, ip = %d\n", ip);
            int cp = ip - 1;
            int len = strlen("pop");

            if (cmdArr[i][len] == '[')
            {
                codeBuf[cp] |= 0x80;
                len++;
            }

            int aboba = CHAR;

            scaned = ArgToCode(cmdArr[i] + len, codeBuf, &ip, cp, aboba);

            if (scaned == 0)
            {
                printf("got an unknown argument for pop in line %d: \"%s\"; exiting the programm", i, cmdArr[i] + len);
                exit(1);
            }
        }

    }

    return ip;
}


int ArgToCode(char* arg, char* codeBuf, int* ip, int cp, int searchFor)
{
    int num = 0;
    char regName [2] = {0};
    int len = 0;
    int scaned = 0;

    if (searchFor == INT)
    {
        sscanf(arg, "%*[^0-9\n]%n", &len);

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

            return (1 + len);
        }
    }

    if (searchFor == CHAR)
    {
        sscanf(arg, "%*[ +0-9]%n", &len);
        if (arg[len] == '\n' || arg[len] == '\0')
            return 0;

        scaned = sscanf(arg + len, "%2s", regName);
        //printf("%2s\n", arg);
        //printf("scaned = %d, regName - %2s len of buf - %d\n", scaned, regName, len);
        if (scaned != 0)
        {
            RegToCode(regName, codeBuf + *ip);
            codeBuf[cp] |= 0x40;
            *ip += sizeof(char);

            return (2 + len);
        }
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

    else if(strcmp(cmd, "pop") == 0)
    {
        codeBuf[0] = CMD_POP;
        return 1;
    }

    else if(strcmp(cmd, "add") == 0)
    {
        codeBuf[0] = CMD_ADD;
        return 1;
    }

    else if(strcmp(cmd, "sub") == 0)
    {
        codeBuf[0] = CMD_SUB;
        return 1;
    }

    else if(strcmp(cmd, "mul") == 0)
    {
        codeBuf[0] = CMD_MUL;
        return 1;
    }

    else if(strcmp(cmd, "out") == 0)
    {
        codeBuf[0] = CMD_OUT;
        return 1;
    }

    else if(strcmp(cmd, "ver") == 0)
    {
        codeBuf[0] = CMD_VER;
        return 1;
    }

    else if(strcmp(cmd, "dmp") == 0)
    {
        codeBuf[0] = CMD_DMP;
        return 1;
    }

    else if(strcmp(cmd, "hlt") == 0)
    {
        codeBuf[0] = CMD_HLT;
        return 1;
    }

    else 
    {
        printf("Got an unknown command: \"%s\", stopping assembling\n", cmd);
        exit(1);
    }
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

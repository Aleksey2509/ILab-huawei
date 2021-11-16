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

int CodeToCmd(char* CodeBuf, off_t size);
off_t CreateBuffer(const char* FileName, char** Buffer);

int main(int argc, char* argv[])
{
    
    char* CodeBuf;
    off_t size = CreateBuffer(argv[1], &CodeBuf);

    CodeToCmd(CodeBuf, size);

    return 0;
}

off_t CreateBuffer(const char* FileName, char** Buffer)
{
    struct stat FileInfo;
    int stat_flag = stat(FileName, &FileInfo);

    off_t size = FileInfo.st_size;
    *Buffer = (char* )calloc(size, sizeof(char));
    //printf("in crtbuf: bufptr: %p\n", Buffer);

    FILE* input = fopen(FileName, "rb");
    size_t buffsize = fread(*Buffer, sizeof(char), size, input);
    // for (int i = 0; i < buffsize; i++)
    // {
    //     printf("%d ", (*Buffer)[i]);
    // }
    fclose(input);

    return size;
}

int CodeToCmd(char* CodeBuf, off_t size)
{
    int Error = 0;
    int ip = 0;
    FILE* disasm = fopen("disassembled.txt", "w");

    while (ip < size)
    {
        //printf("\n\nin cycle: Codebuf[%d] = %d\n\n", ip, CodeBuf[ip]);
        switch(CodeBuf[ip])
        {
            case CMD_PUSH:  fprintf(disasm, "%s %d\n", "PUSH", *(int*)(CodeBuf + ip + 1));
                            ip += sizeof(char) + sizeof(int);
                            break;

            case CMD_POP:   fprintf(disasm, "%s\n", "POP");
                            ip += 1;
                            break;

            case CMD_ADD:   fprintf(disasm, "%s\n", "ADD");
                            ip += 1;
                            break;

            case CMD_SUB:   fprintf(disasm, "%s\n", "SUB");
                            ip += 1;
                            break;

            case CMD_MUL:   fprintf(disasm, "%s\n", "MUL");
                            ip += 1;
                            break;

            case CMD_DIV:   fprintf(disasm, "%s\n", "DIV");
                            ip += 1;
                            break;

            case CMD_OUT:   fprintf(disasm, "%s\n", "OUT");
                            ip += 1;
                            break;

            case CMD_VER:   fprintf(disasm, "%s\n", "VER");
                            ip += 1;
                            break;

            case CMD_DMP:   fprintf(disasm, "%s\n", "DMP");
                            ip += 1;
                            break;

            case CMD_HLT:   fprintf(disasm, "%s\n", "HLT");
                            ip += 1;
                            break;

            default: printf("\n\nUnknown code for command - %d; stopping working\n\n", CodeBuf[ip]);
                     return 1;
        }
    }

    return 1;
}
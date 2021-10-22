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


struct Processor
{
    Stack stack {};
    int reg[4] = {0};
    char* codeArr = 0;
    int ip = 0;
    int* RAM = 0;

};

int process (Stack* stack, const char* CodeBuf);
int CreateBuffer(const char* FileName, char** Buffer);

int push(Stack* stack, int a);
int pop(Stack* stack);
int add(Stack* stack);
int sub(Stack* stack);
int mul(Stack* stack);
int div(Stack* stack);
void out(Stack* stack);
int ver(Stack* stack);
void dmp(Stack* stack);



int main(int argc, char* argv[])
{
    Processor processor = {};

    int Error = CreateBuffer(argv[1], &(processor.codeArr));
    if (Error)
    {
        printf("%s\n", GetError(Error));
        return 0;
    }

    // for (int i = 0; i < 10; i++)
    // {
    //     printf("%d ", CodeBuf[i]);
    // }

    StackCtor(&(processor.stack), 20);

    Error = process(&processor.stack, processor.codeArr);
    free(processor.codeArr);
    StackDtor(&processor.stack);

    return 0;
}



int process (Stack* stack, const char* CodeBuf)
{
    int Error = 0;
    int ip = 0;

    while(1)
    {
        //printf("\n\nin cycle: Codebuf[%d] = %d\n\n", ip, CodeBuf[ip]);
        switch(CodeBuf[ip])
        {
            case CMD_PUSH: push(stack, *(int*)(CodeBuf + ip + 1));
                        ip += sizeof(char) + sizeof(int);
                        break;

            case CMD_POP: pop(stack);
                        ip += 1;
                        break;

            case CMD_ADD: add(stack);
                        ip += 1;
                        break;

            case CMD_SUB: sub(stack);
                        ip += 1;
                        break;

            case CMD_MUL: mul(stack);
                        ip += 1;
                        break;

            case CMD_DIV: div(stack);
                        ip += 1;
                        break;

            case CMD_OUT: out(stack);
                        ip += 1;
                        break;

            case CMD_VER: ver(stack);
                        ip += 1;
                        break;

            case CMD_DMP: dmp(stack);
                        ip += 1;
                        break;

            case CMD_HLT: return 0;

            default: printf("\n\nUnknown code for command; dumping and stopping working\n\n");
                     dmp(stack);
                     return 1;
        }
    }

    return 1;

}

//------------------------------------------------------------------------------------------------------------------------------------

int CreateBuffer(const char* FileName, char** Buffer)
{
    if (!FileName)
        return NULL_FL;

    struct stat FileInfo;
    int stat_flag = stat(FileName, &FileInfo);
    if (stat_flag == -1)
    {
        return CANT_GET_FL_INFO;
    }

    off_t size = FileInfo.st_size;
    *Buffer = (char* )calloc(size, sizeof(char));
    //printf("in crtbuf: bufptr: %p\n", Buffer);

    if ( (*Buffer) == NULL)
        return CANT_ALLOC_BUF;

    FILE* input = fopen(FileName, "rb");
    if ( input == NULL)
    {
        free(*Buffer);
        return NULL_FL;
    }

    size_t buffsize = fread(*Buffer, sizeof(char), size, input);
    // for (int i = 0; i < buffsize; i++)
    // {
    //     printf("%d ", (*Buffer)[i]);
    // }
    fclose(input);

    return OK;
}

//------------------------------------------------------------------------------------------------------------------------------------

int push(Stack* stack, int a)
{
    int Error = StackPush(stack, &a);
    
    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------------

int pop(Stack* stack)
{
    int Error = StackPop(stack);
    
    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------------

int add(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    TrackError(Error);

    Error = StackPop(stack, &b);
    TrackError(Error);

    int result = a + b;

    Error = StackPush(stack, &result);
    TrackError(Error);

    return result;
}

//------------------------------------------------------------------------------------------------------------------------------------

int sub(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    TrackError(Error);

    Error = StackPop(stack, &b);
    TrackError(Error);

    int result = b - a;

    Error = StackPush(stack, &result);
    TrackError(Error);

    return GOOD;
}

//------------------------------------------------------------------------------------------------------------------------------------

int mul(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    TrackError(Error);

    Error = StackPop(stack, &b);
    TrackError(Error);

    int result = a * b;

    Error = StackPush(stack, &result);
    TrackError(Error);

    return result;
}

//------------------------------------------------------------------------------------------------------------------------------------

int div(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    TrackError(Error);

    Error = StackPop(stack, &b);
    TrackError(Error);

    if (b == 0)
        return -1;

    int result = a / b;

    Error = StackPush(stack, &result);
    TrackError(Error);

    return result;
}

//------------------------------------------------------------------------------------------------------------------------------------

void out(Stack* stack)
{
    if (stack->size == 0)
        return;

    printf("%d\n", stack->data[stack->size - 1]);
}

//------------------------------------------------------------------------------------------------------------------------------------

int ver(Stack* stack)
{
    return (CheckStack(stack));
}

//------------------------------------------------------------------------------------------------------------------------------------

void dmp(Stack* stack)
{
    StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
}


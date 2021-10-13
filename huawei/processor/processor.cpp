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

int process (Stack* stack, const int* CodeBuf);

int push(Stack* stack, int a);
int pop(Stack* stack);
int add(Stack* stack);
int sub(Stack* stack);
int mul(Stack* stack);
int div(Stack* stack);
void out(Stack* stack);
int ver(Stack* stack);
void dmp(Stack* stack);
int hlt();



int main(int argc, char* argv[])
{
    Stack stack {};

    struct stat FileInfo;
    int stat_flag = stat(argv[1], &FileInfo);
    off_t size = FileInfo.st_size;
    int* CodeBuf = (int* )calloc(size, sizeof(int));

    FILE* input = fopen(argv[1], "rb");
    size_t buffSize = fread(CodeBuf, sizeof(int), size, input);

    int Error = StackCtor(&stack, 20);
    if(Error)
    {
        StackDump(&stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return 0;
    }

    Error = process(&stack, CodeBuf);
    free(CodeBuf);

    return 0;
}



int process (Stack* stack, const int* CodeBuf)
{
    int Error = 0;
    int ip = 0;

    // printf("printing codebuf\n");
    // for (int i = 0; i < 20; i++)
    // {
    //     printf("%d ", CodeBuf[i]);
    // }
    // printf("\n");

    while(1)
    {
        switch(CodeBuf[ip])
        {
            case CMD_PUSH: push(stack, CodeBuf[ip + 1]);
                        ip += 2;
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

            default: printf("\n\nUnknown code for command; dumping and stoppong working\n\n");
                    dmp(stack);
                    return 1;
        }
    }

    return 1;

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
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }

    Error = StackPop(stack, &b);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }

    int result = a + b;

    Error = StackPush(stack, &result);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
    }
    return result;
}

//------------------------------------------------------------------------------------------------------------------------------------

int sub(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }

    Error = StackPop(stack, &b);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }

    int result = b - a;

    Error = StackPush(stack, &result);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }
    return result;
}

//------------------------------------------------------------------------------------------------------------------------------------

int mul(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }

    Error = StackPop(stack, &b);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }

    int result = a * b;

    Error = StackPush(stack, &result);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }
    return result;
}

//------------------------------------------------------------------------------------------------------------------------------------

int div(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }

    Error = StackPop(stack, &b);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }

    if (b == 0)
        return -1;

    int result = a + b;

    Error = StackPush(stack, &result);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }
    return result;
}

//------------------------------------------------------------------------------------------------------------------------------------

void out(Stack* stack)
{
    ElemDump(stack->data + stack->size - 1);
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


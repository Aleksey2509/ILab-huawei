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


struct Processor
{
    Stack stack {};
    int reg[4] = {0};
    char* codeArr = 0;
    int ip = 0;
    int* RAM = 0;

};

int process (Processor* processor);
int CreateBuffer(const char* FileName, char** Buffer);

int push(Stack* stack, int a);
int pop(Stack* stack, elem_t* dst = 0);
int add(Stack* stack);
int sub(Stack* stack);
int mul(Stack* stack);
int div(Stack* stack);
void out(Stack* stack);
int ver(Stack* stack);
void dmp(Stack* stack);



int main(int argc, char* argv[])
{
    Processor processor {};

    int Error = CreateBuffer(argv[1], &(processor.codeArr));
    if (Error)
    {
        printf("%s\n", GetError(Error));
        return 0;
    }

    // for (int i = 0; i < 10; i++)
    // {
    //     printf("%d ", codeBuf[i]);
    // }

    StackCtor(&(processor.stack), 20);

    for (int i = 0; i < 4; i++)
    {
        processor.reg[i] = i + 1;
    }

    Error = process(&processor);
    free(processor.codeArr);
    StackDtor(&processor.stack);

    return 0;
}



int process (Processor* processor)
{
    Stack* stack = &processor->stack;
    const char* codeBuf = processor->codeArr;
    int Error = 0;
    int ip = 0;
    int type = 0;
    int arg = 0;
    int cmd = 0;

    while(1)
    {
        //printf("\n\nin cycle: codebuf[%d] = %d\n\n", ip, codeBuf[ip]);
        cmd = codeBuf[ip] & 0x1F;
        printf("%d, ip - %d\n", cmd, ip);
        switch(cmd)
        {
            case CMD_PUSH:
                        type = codeBuf[ip];
                        if (type & 0x20)
                        {
                            arg += *(elem_t*)(codeBuf + ip + 1);
                            ip += sizeof(elem_t);
                        }
                        if (type & 0x40)
                        {
                            arg += processor->reg[codeBuf[ip += 1] - 1];
                        }
                        ip += 1;
                        push(stack, arg);
                        arg = 0;
                        break;

            case CMD_POP:
                        pop(stack);
                        type = codeBuf[ip];
                        if (type & 0x40)
                        {
                            arg += processor->reg[codeBuf[ip += 1] - 1];
                        }
                        ip += 1;
                        pop (stack, &arg);
                        arg = 0;
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

int push(Stack* stack, elem_t a)
{
    int Error = StackPush(stack, a);
    
    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------------

int pop(Stack* stack, elem_t* dst)
{
    int Error = StackPop(stack, dst);
    
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

    Error = StackPush(stack, result);
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

    Error = StackPush(stack, result);
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

    Error = StackPush(stack, result);
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

    Error = StackPush(stack, result);
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
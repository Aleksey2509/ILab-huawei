#include <stdarg.h>
#include "struct.hpp"
#include "ReadWrite.hpp"
#include "stack.hpp"

#define ErrorToLog(Error) \
{\
    if(Error)\
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);\
}

const int END = -200;

int process (Stack* stack, const char* cmd, int val);
int processor (Stack* stack, char** CmdArr);

int push(Stack* stack, int a);
int pop(Stack* stack,int* a);
int add(Stack* stack);
int sub(Stack* stack);
int mul(Stack* stack);
int div(Stack* stack);
void out(Stack* stack);
int ver(Stack* stack);
void dmp(Stack* stack);
int hlt();

// char CmdArr[][5] = {"push",
//                     "pop",
//                     "add",
//                     "sub",
//                     "mul",
//                     "div",
//                     "out",
//                     "ver", 
//                     "dmp",
//                     "hlt" };

//int CmdCount = sizeof(CmdArr) / sizeof(CmdArr[0]);


int main(int argc, char* argv[])
{
    TEXT text {};
    Stack stack;
    int Error = TEXT_ReadFromFile(&text, argv[1]);
    if(Error)
    {
        printf("%s\n", GetError(Error));
        return 0;
    }
    //PrintTextStdout(&text);

    char** CmdArr = (char**)calloc(text.nLines, sizeof(char*));

    for (int i = 0; i < text.nLines; i++)
    {
        CmdArr[i] = text.lines[i].line;
    }

    Error = StackCtor(&stack, 20, sizeof(int));
    if(Error)
    {
        StackDump(&stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return 0;
    }

    //processor(&stack, CmdArr);
    char cmdbuffer [1000] = { 0 };
    int num = 0;
    for (int i = 0; i < text.nLines; i++)
    {
        sscanf(CmdArr[i], "%s %d", cmdbuffer, &num);
        process(&stack, cmdbuffer, num);
    }

    return 0;
}



int process (Stack* stack, const char* cmd, int val)
{
    int Error = 0;
    int tmp;

        if(strcmp(cmd, "push") == 0)
        {
            Error = push(stack, val);
            return Error;
        }

        if(strcmp(cmd, "pop") == 0)
        {
            Error = pop(stack, &tmp);
            return Error;
        }

        if(strcmp(cmd, "add") == 0)
        {
            Error = add(stack);
            return Error;
        }

        if(strcmp(cmd, "sub") == 0)
        {
            Error = sub(stack);
            return Error;
        }

        if(strcmp(cmd, "mul") == 0)
        {
            Error = mul(stack);
            return Error;
        }

        if(strcmp(cmd, "out") == 0)
        {
            out(stack);
            return 0;
        }

        if(strcmp(cmd, "ver") == 0)
        {
            Error = ver(stack);
            return Error;
        }

        if(strcmp(cmd, "dmp") == 0)
        {
            dmp(stack);
            return 0;
        }

        if(strcmp(cmd, "hlt") == 0)
        {
            return END;
        }

    return 0;
}

int push(Stack* stack, int a)
{
    int Error = StackPush(stack, &a);
    
    return Error;
}

int pop(Stack* stack,int* a)
{
    int Error = StackPop(stack, a);
    
    return Error;
}

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

    int result = a - b;

    Error = StackPush(stack, &result);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return Error;
    }
    return result;
}

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

void out(Stack* stack)
{
    ElemDump((int*)stack->data + stack->size - 1);
}

int ver(Stack* stack)
{
    return (CheckStack(stack));
}

void dmp(Stack* stack)
{
    StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
}





int processor (Stack* stack, char** CmdArr)
{
    int Error = 0;
    int tmp = 0;
    int num = 0;

    char cmdbuffer [1000] = { 0 };

    for (int i = 0; ;i++)
    {
        sscanf (CmdArr[i], "%s %d", cmdbuffer, &num);
        if(strcmp(cmdbuffer, "push") == 0)
        {
            Error = push(stack, num);
            ErrorToLog(Error);
        }

        if(strcmp(cmdbuffer, "pop") == 0)
        {
            Error = pop(stack, &tmp);
            ErrorToLog(Error);
        }

        if(strcmp(cmdbuffer, "add") == 0)
        {
            Error = add(stack);
            ErrorToLog(Error);
        }

        if(strcmp(cmdbuffer, "sub") == 0)
        {
            Error = sub(stack);
            ErrorToLog(Error);
        }

        if(strcmp(cmdbuffer, "mul") == 0)
        {
            Error = mul(stack);
            ErrorToLog(Error);
        }

        if(strcmp(cmdbuffer, "out") == 0)
        {
            out(stack);
        }

        if(strcmp(cmdbuffer, "ver") == 0)
        {
            Error = ver(stack);
            ErrorToLog(Error);
        }

        if(strcmp(cmdbuffer, "dmp") == 0)
        {
            dmp(stack);
        }

        if(strcmp(cmdbuffer, "hlt") == 0)
        {
            break;
        }

    }

    return 0;
}
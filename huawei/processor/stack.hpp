#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include "config.h"

#define StackCtor(stack, capacity, Datasize) StackCreator(stack, capacity, Datasize, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define TrackError(Error) \
{\
    if(Error)\
    {\
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);\
        return 0\
    }\
}

typedef long unsigned canary_t;

struct Stack // add debug mode in stack
{
    #if CNRY_DEF
    canary_t LeftCanary;
    #endif

    void* data;
    size_t capacity;
    size_t size;
    size_t DataSize; // deprecate

    const char* CreateFile;
    const char* CreateFunc;
    int CreateLine;

    #if CNRY_DEF
    canary_t RightCanary;
    #endif
};

enum ErrorCodes
{
    NULL_STK_PTR = -1000,
    NOT_INIT,
    NULL_DATA_PTR,
    ZERO_DATASIZE,
    CAP_LESS_SIZE,
    STK_CANARY_DMG,
    DATA_CANARY_DMG,
    NOT_RESIZABLE,
    NOTHING_TO_POP,
    GOOD = 0
};

//------------------------------------------------------------------------------------------------------------------------------

int StackCreator (Stack* stack, unsigned long capacity, unsigned long DataSize, const char* FileName, const char* FuncName, int LineNum);
int StackResize (Stack* stack, int mode);
int StackDtor (Stack* stack);
int StackPush(Stack* stack, void* src);
int StackPop(Stack* stack, void* dst);
int ElemDump(void* dataptr);
int StackDump (const Stack* stack, const char* FileName, int LineNum, const char* FuncName, int Error = 0, const char reason[] = "Just to check");
ErrorCodes CheckStack (const Stack* stack);
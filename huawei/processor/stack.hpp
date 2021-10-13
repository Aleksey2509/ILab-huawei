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

#define StackCtor(stack, capacity) StackCreator(stack, capacity, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define StackDtor(stack) StackDestructor(stack,  __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define TrackError(Error) \
{\
    if(Error)\
    {\
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);\
        return Error;\
    }\
}

typedef int elem_t;
typedef long unsigned canary_t;
typedef long unsigned hash_t;

const canary_t DATA_CANARY_VAL =  0xBAD00BAD;
const canary_t STK_CANARY_VAL  =  0xDED00DED;

static const int BAD_POINTER = 13;

enum ErrorMasks
{
    NULL_STK_PTR    = 0x10000000,
    NOT_INIT        = 0x01000000,
    NULL_DATA_PTR   = 0x00100000,
    CAP_LESS_SIZE   = 0x00010000,
    STK_CANARY_DMG  = 0x00001000,
    DATA_CANARY_DMG = 0x00000100,
    STK_HASH_ERR    = 0x00000010,
    DATA_HASH_ERR   = 0x00000001,
    GOOD = 0
};

enum UserError
{
    NULL_SRC_PTR = -1000,
    NOT_RESIZABLE,
    NOTHING_TO_POP,
    NOWHERE_TO_PUT,
};

struct CallPlace
{
    const char* FileName = 0;
    const char* FuncName = 0;
    size_t LineNum = 0;
};

struct Stack // add debug mode in stack
{
    #if CNRY_DEF
    canary_t LeftCanary = 0;
    #endif

    elem_t* data = 0;
    size_t capacity = 0;
    size_t size = 0;

    CallPlace PlaceCrtd;

    #if CNRY_DEF
    canary_t RightCanary = 0;
    #endif

    #if HASH_DEF
        hash_t DataHash = 0;
    #endif
    #if HASH_DEF
        hash_t StackHash = 0;
    #endif
};

//------------------------------------------------------------------------------------------------------------------------------



int StackCreator (Stack* stack, unsigned long capacity, const char* FileName, const char* FuncName, int LineNum);
int StackResize (Stack* stack, int mode);
int StackDestructor (Stack* stack, const char* FileName, const char* FuncName, int LineNum);
int StackPush(Stack* stack, elem_t* src);
int StackPop(Stack* stack, elem_t* dst = 0);
char* GetUserError (int Error);
int ElemDump(elem_t* ptr);
int StackDump (const Stack* stack, const char* FileName, int LineNum, const char* FuncName, int Error = 0, const char reason[] = "Just to check");
int CheckStack (const Stack* stack);

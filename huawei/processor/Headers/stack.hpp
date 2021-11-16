#ifndef STACK_HPP

#define STACK_HPP

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

typedef int elem_t;

extern const elem_t POISON;

static char UpperBorder[] = "//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\";
static char LowerBorder[] = "\n//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\///**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\";

#define StackCtor(stack, capacity) StackCreator(stack, capacity, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define StackDtor(stack) StackDestructor(stack,  __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define CheckStack(stack) VerifyStack(stack, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define TrackError(Error) \
{\
    if(Error)\
    {\
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);\
        return Error;\
    }\
}

#define TrackErrorWithCallPlc(Error, FileName, LineNum, FuncName) \
{\
    if(Error)\
    {\
        StackDump(stack, FileName, LineNum, FuncName, Error);\
        return Error;\
    }\
}

typedef long unsigned canary_t;
typedef long unsigned hash_t;

const canary_t DATA_CANARY_VAL =  0xBAD00BAD;
const canary_t STK_CANARY_VAL  =  0xDED00DED;

static elem_t* BAD_POINTER = (elem_t*)13;

enum CritErrorMasks // crit errors - positive codes
{
    NULL_STK_PTR    = 0x80,
    NOT_INIT        = 0x40,
    NULL_DATA_PTR   = 0x20,
    CAP_LESS_SIZE   = 0x10,
    STK_CANARY_DMG  = 0x08,
    DATA_CANARY_DMG = 0x04,
    STK_HASH_ERR    = 0x02,
    DATA_HASH_ERR   = 0x01,
};

enum UserError
{
    NULL_SRC_PTR = -1000,
    NOT_RESIZABLE,
    NOTHING_TO_POP,
    USE_AFTER_DESTRUCT,
    POP_FROM_POISONED
};

struct CallPlace
{
    const char* FileName = 0;
    const char* FuncName = 0;
    long int LineNum = 0;
};

struct Stack
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
    hash_t StackHash = 0;
    #endif
};

//------------------------------------------------------------------------------------------------------------------------------

static char StackErrors [][200] = {"Stack not properly initialized (has a null ptr)\n",
                                   "Warning! Stack not initialized \n",
                                   "ERROR! Capacity is less than size:\n",
                                   "Stack not properly initialized (has a data null ptr). The user probably forgot to use StackCtor()\n",
                                   "ERROR!!! THE CANARIES AROUND DATA WERE DAMAGED:",
                                   "ERROR!!! THE CANARIES WERE AROUND STACK STRUCT WERE DAMAGED:\n",
                                   "ERROR!!! STACK HASH HAS BEEN CHANGED!!! THE STACK HAS BEEN INTERACTED WITH IN AN WRONG WAY\n",
                                   "ERROR!!! DATA HASH HAS BEEN CHANGED!!! THE STACK DATA HAS BEEN INTERACTED WITH IN AN WRONG WAY\n"};

int StackCreator (Stack* stack, unsigned long capacity, const char* FileName, const char* FuncName, int LineNum);
int StackDestructor (Stack* stack, const char* FileName, const char* FuncName, int LineNum);
int StackPush(Stack* stack, elem_t src);
int StackPop(Stack* stack, elem_t* dst = 0);
int StackResize (Stack* stack, int mode, size_t NewSize = 0);
const char* GetUserError (int Error);
extern int ElemDump(elem_t* dataptr);
int StackDump (const Stack* stack, const char* FileName, int LineNum, const char* FuncName, int UserError = 0, const char reason[] = "Just to check");
int VerifyStack (const Stack* stack, const char* FileName, const char* FuncName, int LineNum);


#endif
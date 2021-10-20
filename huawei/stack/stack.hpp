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

static const int BAD_POINTER = 13; // static const elem_t* BAD_PTR = 13;
static const int MULT_CONST = 2;
static const int ENLARGE = 1;
static const int REDUCE = -1;

enum ErrorMasks // crit errors - positive codes
{
    NULL_STK_PTR    = 0x10000000, // 4000
    NOT_INIT        = 0x01000000, // 2000
    NULL_DATA_PTR   = 0x00100000, // 1000
    CAP_LESS_SIZE   = 0x00010000,
    STK_CANARY_DMG  = 0x00001000,
    DATA_CANARY_DMG = 0x00000100,
    STK_HASH_ERR    = 0x00000010,
    DATA_HASH_ERR   = 0x00000001,
    OK = 0
};

enum UserError // not crit errors
{
    NULL_SRC_PTR = -1000,
    NOT_RESIZABLE,
    NOTHING_TO_POP,
    USE_AFTER_DESTRUCT
};

struct CallPlace
{
    const char* FileName = 0;
    const char* FuncName = 0;
    long int LineNum = 0;
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
    hash_t StackHash = 0;
    #endif
};

//------------------------------------------------------------------------------------------------------------------------------

static char UserErrors [][150] = {"There is either a error with stack or stackDump was given unknown error code\n",
                                  "Tried to push, but did not give a valid pointer\n", 
                                  "Tried to pop from an empty stack\n",
                                  "Tried to push to a full stack, which could not be resized\n",
                                  "WARNING! WARNING! The stack is being used after it's destruction!!! Futher use could lead to segmentation fault!!!\n"};

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
int StackPush(Stack* stack, elem_t* src);
int StackPop(Stack* stack, elem_t* dst = 0);
int StackResize (Stack* stack, int mode, size_t NewSize = 0);
char* GetUserError (int Error);
extern int ElemDump(elem_t* dataptr);
int StackDump (const Stack* stack, const char* FileName, int LineNum, const char* FuncName, int UserError = 0, const char reason[] = "Just to check");
int VerifyStack (const Stack* stack, const char* FileName, const char* FuncName, int LineNum);

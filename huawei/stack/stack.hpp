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

char UpperBorder[] = "//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\";
char LowerBorder[] = "\n//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\///**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\";

#define StackCtor(stack, capacity, Datasize) StackCreator(stack, capacity, Datasize, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define TrackError(Error) \
{\
    if(Error)\
    {\
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);\
        return Error;\
    }\
}

#define BAD_POINTER 13

char ErrorStrings[][100] = {
                             "Stack not properly initialized (has a null ptr)\n",
                             "Warning!: Stack not initialized\n",
                             "ERROR! Capacity is less than size:\n",
                             "ERROR! The size of element type is 0:\n",
                             "Stack not properly initialized (has a data null ptr\n)",
                             "ERROR!!! THE CANARIES AROUND DATA WERE DAMAGED",
                             "ERROR!!! THE CANARIES WERE AROUND STACK STRUCT WERE DAMAGED"
                           };


static const int MULT_CONST = 2;
static const int ENLARGE = 1;
static const int REDUCE = -1;

typedef long unsigned canary_t;
typedef long unsigned hash_t;

const canary_t DATA_CANARY_VAL =  0xBAD00BAD;
const canary_t STK_CANARY_VAL  =  0xDED00DED;

enum ErrorCodes
{
    NULL_STK_PTR = -1000,
    NOT_INIT,
    NULL_DST_PTR,
    NULL_SRC_PTR,
    NULL_DATA_PTR,
    ZERO_DATASIZE,
    CAP_LESS_SIZE,
    STK_CANARY_DMG,
    DATA_CANARY_DMG,
    STK_HASH_ERR,
    DATA_HASH_ERR,
    NOT_RESIZABLE,
    NOTHING_TO_POP,
    NOWHERE_TO_PUT,
    OK = 0
};

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
    size_t CreateLine;

    #if CNRY_DEF
    canary_t RightCanary;
    #endif

    #if HASH_DEF
        hash_t DataHash;
    #endif
    #if HASH_DEF
        hash_t StackHash;
    #endif
};

//------------------------------------------------------------------------------------------------------------------------------

int StackCreator (Stack* stack, unsigned long capacity, unsigned long DataSize, const char* FileName, const char* FuncName, int LineNum);
int StackResize (Stack* stack, int mode);
int StackDtor (Stack* stack);
int StackPush(Stack* stack, void* src);
int StackPop(Stack* stack, void* dst);
extern int ElemDump(void* dataptr);
int StackDump (const Stack* stack, const char* FileName, int LineNum, const char* FuncName, int Error = 0, const char reason[] = "Just to check");
ErrorCodes CheckStack (const Stack* stack);

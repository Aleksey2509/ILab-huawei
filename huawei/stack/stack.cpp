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

FILE* logfile = fopen("log.txt", "w");

const int MULT_CONST = 2;
const int ENLARGE = 1;
const int REDUCE = -1;

typedef long unsigned canary_t;

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
    int CreateLine;

    #if CNRY_DEF
    canary_t RightCanary;
    #endif
};

//------------------------------------------------------------------------------------------------------------------------------

int StackCreator (Stack* stack, unsigned long capacity, unsigned long DataSize, const char* FileName, const char* FuncName, int LineNum);
int StackResize (Stack* stack, int mode);
int StackDtor (Stack* stack);
int StackPush(Stack* stack, void* src);
int StackPop(Stack* stack, void* dst);
int StackDump (const Stack* stack, const char* FileName, int LineNum, const char* FuncName, int Error = 0, const char reason[] = "Just to check");
extern int ElemDump(void* dataptr);
ErrorCodes CheckStack (const Stack* stack);

//------------------------------------------------------------------------------------------------------------------------------

int StackCreator (Stack* stack, unsigned long capacity, unsigned long DataSize, const char* FileName, const char* FuncName, int LineNum)
{
    if (CheckStack(stack) == NULL_STK_PTR)
    {
        StackDump(stack, FileName, LineNum, FuncName, NULL_STK_PTR, "to print null stk ptr");
        return NULL_STK_PTR;
    }


    #if CNRY_DEF
    {
        void* ActualStackPtr = calloc(1, capacity * DataSize + 2 * sizeof(canary_t));

        stack->data = (void*)((char*)ActualStackPtr + sizeof(canary_t));
    }
    #else 
        stack->data = calloc(capacity, DataSize);
    #endif

    stack->size = 0;
    stack->capacity = capacity;
    stack->DataSize = DataSize;
    
    #if CNRY_DEF
    {
        stack->LeftCanary = STK_CANARY_VAL;
        stack->RightCanary = STK_CANARY_VAL;

        ((canary_t*)stack->data)[-1] = DATA_CANARY_VAL;
        *(canary_t*)((char*)stack->data + stack->capacity * stack->DataSize) = DATA_CANARY_VAL;
    }
    #endif

    stack->CreateFile = FileName;
    stack->CreateFunc = FuncName;
    stack->CreateLine = LineNum;

    //printf("IN CTOR: LeftCanary = %lX, RightCanary = %lX\n", ((canary_t*)stack->data)[-1], *(canary_t*)((char*)stack->data + stack->capacity * stack->DataSize));
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int StackDtor (Stack* stack)
{
    int Error = CheckStack(stack);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return 0;
    }

    stack->size = -1;
    stack->DataSize = -1;

    #if CNRY_DEF
        free( ((char*)stack->data - sizeof(canary_t)) );
    #else 
        free(stack->data);
    #endif
    
    stack->data = (void* ) 13; // 13 - not good // add const

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int StackPush(Stack* stack, void* src)
{
    int Error = CheckStack(stack);
    if (Error) // move to define
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return 0;
    }
    if (src == 0)
    {
        printf("Invalid source pointer"); // remove
        return NULL_STK_PTR;
    }

    if (stack->size == stack->capacity)
    {
        Error = StackResize (stack, ENLARGE); // x2
        if (Error)
            return Error;
    }

    void* placeToCopy = (void*)( (char*)stack->data + stack->DataSize * stack->size);
    memcpy( placeToCopy , src, stack->DataSize);

    stack->size++;

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int StackPop(Stack* stack, void* dst)
{
    StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    int Error = CheckStack(stack);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, Error);
        return 0;
    }

    if (dst == 0)
    {
        printf("Invalid destination pointer");
        return NULL_STK_PTR;
    }

    if ( (stack->size < (stack->capacity / 2) ) )
    {
        //printf("Starting reducing\n");
        Error = StackResize(stack, REDUCE);
    }
    memcpy(dst, (void*)( (char*)stack->data + stack->DataSize * (stack->size - 1) ), stack->DataSize);

    stack->size--;

    return 0;

}

//------------------------------------------------------------------------------------------------------------------------------

int StackResize (Stack* stack, int mode)
{
    int Error = CheckStack(stack);
    if (Error)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return 0;
    }

    size_t NewSize = 0;
    if (mode == ENLARGE)
        if (stack->capacity == 0)
            NewSize = stack->DataSize * (stack->capacity + 2);
        else
            NewSize = stack->DataSize * stack->capacity * MULT_CONST;
    else
        NewSize = stack->DataSize * stack->capacity / MULT_CONST;

    #if CNRY_DEF
    {
        void* tmp = (void*)realloc((char*)stack->data - sizeof(canary_t), NewSize + 2 * sizeof(canary_t));
        if(tmp == 0)
            return NOT_RESIZABLE;

        stack->data = (void*)((char*)tmp + sizeof(canary_t));
        ((canary_t*)stack->data)[-1] = DATA_CANARY_VAL;
        *(canary_t*)((char*)stack->data + NewSize) = DATA_CANARY_VAL;
    }

    #else
    {
        void* tmp = (void*)realloc(stack->data, NewSize);
        if(tmp == 0)
            return NOT_RESIZABLE;

        stack->data = tmp;
    }
    #endif

    stack->capacity = NewSize / stack->DataSize;
    

    return OK;
}

//------------------------------------------------------------------------------------------------------------------------------

int StackDump (const Stack* stack, const char* FileName, int LineNum, const char* FuncName, int Error, const char reason[]) // Read about macros
{
    fprintf(logfile, "//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\"
                     "//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\n\n");

    fprintf(logfile, "Stack was created at %s, at %s(%d)\n", stack->CreateFunc, stack->CreateFile, stack->CreateLine);
    fprintf(logfile, "StackDump() Called from function: %s, File: %s line number %D for reason: %s, with error %d\n\n",FuncName, FileName, LineNum, reason, Error);
    
    if (!Error)
        Error = CheckStack(stack);
    
    if (Error == NULL_STK_PTR)
    {
        fprintf(logfile, "Stack not properly initialized (has a null ptr)\n"); // strerror
        return NULL_STK_PTR;
    }

    fprintf(logfile, "Stack [%p]\n", stack);
    if (Error == NOT_INIT)
    {
        fprintf(logfile, "Warning!: Stack not initialized \n");
    }

    #if CNRY_DEF
    {
        if (Error == DATA_CANARY_DMG)
        {
            fprintf(logfile, "ERROR!!! THE CANARIES AROUND DATA WERE DAMAGED:\nExpected values:\nLeft Canary - %X, RightCanary - %X;\ngot:\n", DATA_CANARY_VAL, DATA_CANARY_VAL);
        }
        fprintf(logfile, "Canaries around data: Left Canary - %lX, RightCanary - %lX\n\n", ((canary_t*)stack->data)[-1], 
                                                    *(canary_t*)((char*)stack->data + stack->capacity * stack->DataSize) );

        if (Error == STK_CANARY_DMG)
        {
            fprintf(logfile, "ERROR!!! THE CANARIES WERE AROUND STACK STRUCT WERE DAMAGED:\nExpected values:\nLeft Canary - %X, RightCanary - %X;\ngot:\n", STK_CANARY_VAL, STK_CANARY_VAL);
        }
        fprintf(logfile, "Canaries around stack: Left Canary - %lX, RightCanary - %lX\n\n", stack->LeftCanary, stack->RightCanary);
    }
    #endif

    if(Error == CAP_LESS_SIZE)
    {
        fprintf(logfile, "ERROR! Capacity is less than size:\n");
    }

    fprintf(logfile, "Size = %lu\n", stack->size);
    fprintf(logfile, "capacity = %lu\n", stack->capacity);

    if(Error == ZERO_DATASIZE)
    {
        fprintf(logfile, "ERROR! The size of element type is 0:\n");
    }

    fprintf(logfile, "Datasize = %lu\n\n", stack->DataSize);

    if (Error == NULL_DATA_PTR)
    {
        fprintf(logfile, "Stack not properly initialized (has a data null ptr\n)");
        return NULL_DATA_PTR;
    }

    fprintf(logfile, "dataptr = %p\n", stack->data);
    for (int i = 0; i < stack->size; i++)
    {
        fprintf(logfile, "data[%d]: ", i);
        ElemDump((void*)((char*)stack->data + i * stack->DataSize));
        fprintf(logfile, "\n");
    }

    fprintf(logfile, "\n//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\///**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\"
                     "//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\///**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\\n\n");
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

ErrorCodes CheckStack (const Stack* stack) //write log file
{
    if (stack == 0)
    return NULL_STK_PTR;

    if (stack->data == 0)
    {
        if ( (stack->capacity == stack->size) && (stack->size == 0) )
            return NOT_INIT;
        else
            return NULL_DATA_PTR;
    }
    
    if (stack->capacity < stack->size)
    return CAP_LESS_SIZE;

    if (stack->DataSize == 0)
    return ZERO_DATASIZE;

    #if CNRY_DEF
    if ( ( ((canary_t*)stack->data)[-1] != DATA_CANARY_VAL ) || ( *(canary_t*)((char*)stack->data + stack->capacity * stack->DataSize) != DATA_CANARY_VAL ) )
    {
        //printf("ERROR!!! CANARIES AROUND DATA GOT DAMAGED!!!\n");
        //printf("LeftCanary = %lX, RightCanary = %lX\n\n", ((canary_t*)stack->data)[-1], *(canary_t*)((char*)stack->data + stack->capacity * stack->DataSize));
        return DATA_CANARY_DMG;
    }
    if (stack->LeftCanary != STK_CANARY_VAL || stack->RightCanary != STK_CANARY_VAL)
    {
        return STK_CANARY_DMG;
    }
    #endif

    return OK;
}


/* Stack <int> [0x000F0840] ok called from StackPush() at stack.cpp "stk" at main() at math.cpp (20)         (typeid - type_info - .name() - RTTI)
 *
 * size = 3
 * capacity = 5
 * data [0x0003210]
 *   {
 *      *[0] = 4
 *      *[1] = 5
 *      *[2] = 10
 *      *[3] = 20
 *      *[4] = 30
 *      *[5] = 666 (poison)
 *      *[6] = 666 (poison)
 *   }
 * 
 * 
 * call place - __FIlE__, __LINE__ __PRETTY__PRETTY_FUNCTION__ or __FUNCSIG__
 * 
 * do a struct source location
 * 
 * 
 * 
 * MACROS ASSERT.OK (Stack, )
 * 
 */
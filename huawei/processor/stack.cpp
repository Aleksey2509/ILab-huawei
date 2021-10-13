#include "const.hpp"
#include "stack.hpp"


char UpperBorder[] = "\n//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\//\\\\";
char LowerBorder[] = "\n//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\///**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\//**\\\\";

char UserErrors [][100] = {"There is either a error with stack or stackDump was given unknown error code\n",
                           "Tried to push, but did not give a valid pointer\n", 
                           "Tried to pop from an empty stack\n",
                           "Tried to push to a full stack, which could not be resized\n"
                           "Tried to pop from a poisoned region\n"};

char StackErrors [][200] = {"Stack not properly initialized (has a null ptr)\n",
                            "Warning!: Stack not initialized \n",
                            "ERROR! Capacity is less than size:\n",
                            "Stack not properly initialized (has a data null ptr)\n",
                            "ERROR!!! THE CANARIES AROUND DATA WERE DAMAGED",
                            "ERROR!!! THE CANARIES WERE AROUND STACK STRUCT WERE DAMAGED\n",
                            "ERROR!!! STACK HASH HAS BEEN CHANGED!!! THE STACK HAS BEEN INTERACTED WITH IN AN WRONG WAY\n",
                            "ERROR!!! DATA HASH HAS BEEN CHANGED!!! THE STACK DATA HAS BEEN INTERACTED WITH IN AN WRONG WAY\n"};
//------------------------------------------------------------------------------------------------------------------------------


#if HASH_DEF

unsigned long Hash( const void* key, size_t len, unsigned long prime)
{
    unsigned long hash = (unsigned long) len;

    for (size_t i = 0; i < len; i++)
        hash = (hash << 5)^(hash >> 27) ^ ((char*)key)[i];

    return (hash % prime);
}

//------------------------------------------------------------------------------------------------------------------------------

hash_t GetStackHash (const Stack* stack)
{
    size_t byteLen = sizeof(Stack) - 2 * sizeof(hash_t);
    #if CNRY_DEF
    {
        byteLen -= 2 * sizeof(canary_t);
    }
    #endif

    hash_t StackHash = Hash(stack, byteLen, prime);

    return StackHash;
}

//------------------------------------------------------------------------------------------------------------------------------

hash_t GetDataHash (const Stack* stack)
{
    size_t byteLen = stack->size * sizeof(elem_t);
    hash_t DataHash = Hash(stack, byteLen, prime);

    return DataHash;
}

#endif

//------------------------------------------------------------------------------------------------------------------------------

// void FillPoison (elem_t data[], size_t size)
// {
//     for (int i = 0; i < size; i++)
//     {
//         data[i] = POISON;
//     }
// }


//------------------------------------------------------------------------------------------------------------------------------

FILE* logfile = fopen("log.txt", "w");

//------------------------------------------------------------------------------------------------------------------------------

int ElemDump(elem_t* ptr)
{
    fprintf(logfile, "%d", *(int*)ptr);
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int StackCreator (Stack* stack, unsigned long capacity, const char* FileName, const char* FuncName, const int LineNum)
{
    if ( (FileName == NULL) || (FuncName == NULL) || (LineNum < 0) )
    {
        printf("You freakin' dummy, you are not supposed to touch the funcname, filename or LineNum parametres, you were given a damn macros to autofill that\n");
        printf("exiting the programm");
        exit(1);
    }

    if (CheckStack(stack) == NULL_STK_PTR)
    {
        StackDump(stack, FileName, LineNum, FuncName, NULL_STK_PTR, "to print null stk ptr");
        return NULL_STK_PTR;
    }

    #if CNRY_DEF
    {
        elem_t* ActualStackPtr = (elem_t*)calloc(1, capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

        stack->data = (elem_t*)((char*)ActualStackPtr + sizeof(canary_t));
    }
    #else 
        stack->data = (elem_t* )calloc(capacity, sizeof(elem_t));
    #endif

    stack->size = 0;
    stack->capacity = capacity;

    #if CNRY_DEF
    {
        stack->LeftCanary = STK_CANARY_VAL;
        stack->RightCanary = STK_CANARY_VAL;

        ((canary_t*)stack->data)[-1] = DATA_CANARY_VAL;
        *(canary_t*)(stack->data + stack->capacity) = DATA_CANARY_VAL;
    }
    #endif

    stack->PlaceCrtd.FileName = FileName;
    stack->PlaceCrtd.FuncName = FuncName;
    stack->PlaceCrtd.LineNum = LineNum;

    #if HASH_DEF
    {
        stack->StackHash = GetStackHash(stack);
        stack->DataHash  = GetDataHash(stack);
    }
    #endif

    //printf("IN CTOR: LeftCanary = %lX, RightCanary = %lX\n", ((canary_t*)stack->data)[-1], *(canary_t*)((char*)stack->data + stack->capacity * stack->DataSize));
    return 0;
}



//------------------------------------------------------------------------------------------------------------------------------

int StackDestructor (Stack* stack, const char* FileName, const char* FuncName, int LineNum)
{
    if ( (FileName == NULL) || (FuncName == NULL) || (LineNum < 0) )
    {
        printf("You freakin' dummy, you are not supposed to touch the funcname, filename or LineNum parametres, you were given a damn macros to autofill that\n");
        printf("exiting the programm");
        exit(1);
    }

    int Error = CheckStack(stack);
    TrackError(Error);

    stack->size = -1;

    #if CNRY_DEF
        free( ((char*)stack->data - sizeof(canary_t)) );
    #else 
        free(stack->data);
    #endif
    
    stack->data = (elem_t* ) BAD_POINTER;

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int StackPush(Stack* stack, elem_t* src)
{
    int Error = CheckStack(stack);
    TrackError(Error);
    
    if (src == 0)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, NULL_SRC_PTR);
        return NULL_SRC_PTR;
    }

    if (stack->size == stack->capacity)
    {
        Error = StackResize (stack, ENLARGE);
        TrackError(Error);
    }

    (stack->data)[stack->size] = *src;

    stack->size++;

    #if HASH_DEF
    stack->StackHash = GetStackHash(stack);
    stack->DataHash  = GetDataHash(stack);
    #endif

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int StackPop(Stack* stack, elem_t* dst)
{
    int Error = CheckStack(stack);
    TrackError(Error);

    if (stack->size == 0)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, NOTHING_TO_POP);
        return NOTHING_TO_POP;
    }

    if ( (stack->size < (stack->capacity / 2) ) )
    {
        //printf("Starting reducing\n");
        Error = StackResize(stack, REDUCE);
        TrackError(Error);
    }

    if (dst != 0)
    {
        *dst = stack->data[stack->size - 1];
        //stack->data[stack->size - 1] = POISON;
        stack->size--;
    }

    #if HASH_DEF
    stack->StackHash = GetStackHash(stack);
    stack->DataHash  = GetDataHash(stack);
    #endif

    return 0;

}

//------------------------------------------------------------------------------------------------------------------------------

int StackResize (Stack* stack, int mode)
{
    int Error = CheckStack(stack);
    TrackError(Error);

    size_t NewSize = 0;
    if (mode == ENLARGE)
        if (stack->capacity == 0)
            NewSize = sizeof(elem_t) * (stack->capacity + 2);
        else
            NewSize = sizeof(elem_t) * stack->capacity * MULT_CONST;
    else
        NewSize = stack->capacity * (sizeof(elem_t) / MULT_CONST);

    #if CNRY_DEF
    {
        elem_t* tmp = (elem_t*)realloc((char*)stack->data - sizeof(canary_t), NewSize + 2 * sizeof(canary_t));
        if(tmp == 0)
            return NOT_RESIZABLE;
        stack->data = (elem_t*)((char*)tmp + sizeof(canary_t));

        ((canary_t*)stack->data)[-1] = DATA_CANARY_VAL;
        *((canary_t*)( ( stack->data + NewSize / sizeof(elem_t) ))) = DATA_CANARY_VAL;

        //printf("char + ... = %p, stack + ... = %p\n",(char*)stack->data + NewSize, stack->data + NewSize/sizeof(elem_t));
       
    }

    #else
    {
        void* tmp = realloc(stack->data, NewSize);
        if(tmp == 0)
            return NOT_RESIZABLE;

        stack->data = (elem_t*)tmp;
    }
    #endif

    stack->capacity = NewSize / sizeof(elem_t);

    return GOOD;
}

char* GetUserError (int Error)
{
    switch (Error)
    {
        case NULL_SRC_PTR:   return UserErrors[1];

        case NOTHING_TO_POP: return UserErrors[2];

        case NOT_RESIZABLE:  return UserErrors[3];

        default: return UserErrors[0];
    }
}

//------------------------------------------------------------------------------------------------------------------------------

int StackDump (const Stack* stack, const char* FileName, int LineNum, const char* FuncName, int Error, const char reason[])
{
    fprintf(logfile, "%s%s\n\n", UpperBorder, UpperBorder);

    if ( (FileName == NULL) || (FuncName == NULL) || (LineNum < 0) )
        fprintf(logfile, "Cant print where stack was created: this info was severly damaged or not properly initialised\n");
    else
        fprintf(logfile, "Stack was created at %s, at %s(%lu)\n", stack->PlaceCrtd.FileName, stack->PlaceCrtd.FuncName, stack->PlaceCrtd.LineNum);

    fprintf(logfile, "StackDump() Called from function: %s, File: %s line number %D for reason: %s, with error %d\n\n",FuncName, FileName, LineNum, reason, Error);

    if (Error)
    {
        fprintf(logfile, "%s", GetUserError(Error));
    }

    Error = CheckStack(stack);

    if (Error & NULL_STK_PTR)
    {
        fprintf(logfile, "%s", StackErrors[0]); // strerror
        return NULL_STK_PTR;
    }

    fprintf(logfile, "Stack [%p]\n", stack);
    if (Error & NOT_INIT)
    {
        fprintf(logfile, "%s", StackErrors[1]);
    }

    if(Error & CAP_LESS_SIZE)
    {
        fprintf(logfile, "%s", StackErrors[2]);
    }

    fprintf(logfile, "Size = %lu\n", stack->size);
    fprintf(logfile, "capacity = %lu\n", stack->capacity);

    if (Error & NULL_DATA_PTR)
    {
        fprintf(logfile, "%s", StackErrors[3]);
        return NULL_DATA_PTR;
    }

    #if CNRY_DEF
    {
        if (Error & DATA_CANARY_DMG)
        {
            fprintf(logfile, "%s: Expected values:\nLeft Canary - %lX, RightCanary - %lX;\ngot:\n", StackErrors[4], DATA_CANARY_VAL, DATA_CANARY_VAL);
        }
        fprintf(logfile, "Canaries around data: Left Canary - %lX, RightCanary - %lX\n\n", ((canary_t*)stack->data)[-1], 
                                                    *(canary_t*)(stack->data + stack->capacity) );

        if (Error & STK_CANARY_DMG)
        {
            fprintf(logfile, "%s:\nExpected values:\nLeft Canary - %lX, RightCanary - %lX;\ngot:\n", StackErrors[5], STK_CANARY_VAL, STK_CANARY_VAL);
        }
        fprintf(logfile, "Canaries around stack: Left Canary - %lX, RightCanary - %lX\n\n", stack->LeftCanary, stack->RightCanary);
    }
    #endif

    #if HASH_DEF
    {
       if (Error & STK_HASH_ERR)
       {
           fprintf (logfile, "%s", StackErrors[6]);
       }

       if (Error & DATA_HASH_ERR)
       {
           fprintf (logfile, "%s", StackErrors[7]);
       }
    }
    #endif

    fprintf(logfile, "dataptr = %p\n", stack->data);
    for (int i = 0; i < stack->size; i++)
    {
        fprintf(logfile, "data[%d]: ", i);
        ElemDump(stack->data + i);
        fprintf(logfile, "\n");
    }
    // for (int i = stack->size; i < stack->capacity; i++)
    // {
    //     fprintf(logfile, "data[%d]: ", i);
    //     ElemDump(stack->data + i);
    //     fprintf(logfile, "(POISON)\n");
    // }

    fprintf(logfile, "%s%s\n\n", UpperBorder, UpperBorder);
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int CheckStack (const Stack* stack)
{
    int Error = GOOD;

    if (stack == NULL)
    {
        return NULL_STK_PTR;
    }

    if (stack->capacity < stack->size)
        Error |= CAP_LESS_SIZE;

    if (stack->data == NULL)
    {
        
        if ( (stack->capacity == 0) && (stack->size == 0) )
        {
            return NOT_INIT;
        }
        else
        {
            
            return NULL_DATA_PTR;
        }
    }

    #if CNRY_DEF
    if ( ( ((canary_t*)stack->data)[-1] != DATA_CANARY_VAL ) || ( *(canary_t*)(stack->data + stack->capacity) != DATA_CANARY_VAL ) )
    {
        //printf("ERROR!!! CANARIES AROUND DATA GOT DAMAGED!!!\n");
        //printf("LeftCanary = %lX, RightCanary = %lX\n\n", ((canary_t*)stack->data)[-1], *(canary_t*)((char*)stack->data + stack->capacity * stack->DataSize));
        Error |= DATA_CANARY_DMG;
    }
    if (stack->LeftCanary != STK_CANARY_VAL || stack->RightCanary != STK_CANARY_VAL)
    {
        Error |= STK_CANARY_DMG;
    }
    #endif

    #if HASH_DEF
    {
        hash_t stackHash = GetStackHash(stack);
        if (stackHash != stack->StackHash)
            Error |= STK_HASH_ERR;

        hash_t dataHash = GetDataHash(stack);
        if (dataHash != stack->DataHash)
            Error |= DATA_HASH_ERR;
    }
    #endif

    return Error;
}
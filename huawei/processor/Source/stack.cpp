#include "../Headers/stack.hpp"


FILE* logfile = fopen("log.txt", "w");

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
    byteLen -= 2 * sizeof(canary_t);
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

void FillPoison (elem_t data[], size_t size)
{
    for (int i = 0; i < size; i++)
    {
        data[i] = POISON;
    }
}

//------------------------------------------------------------------------------------------------------------------------------

const char* GetUserError (int Error)
{
    switch (Error)
    {
        case NULL_SRC_PTR:   return "WARNING! User tried to push, but did not give a valid pointer\n\n";

        case NOTHING_TO_POP: return "WARNING! User tried to pop from an empty stack\n\n";

        case NOT_RESIZABLE:  return "WARNING! User tried to push to a full stack, which could not be resized\n\n";

        case USE_AFTER_DESTRUCT: return "WARNING! WARNING! The stack is being used after it's destruction!!! Futher use could lead to segmentation fault!!!\n\n";

        case POP_FROM_POISONED: return "WARNING! User tried to pop from poisoned area\n\n";

        default: return "There is either a error with stack or stackDump was given unknown error code\n\n";

        // "There is either a error with stack or stackDump was given unknown error code\n\n",
        //                           "WARNING! User tried to push, but did not give a valid pointer\n\n", 
        //                           "WARNING! User tried to pop from an empty stack\n\n",
        //                           "WARNING! User tried to push to a full stack, which could not be resized\n\n",
        //                           "WARNING! WARNING! The stack is being used after it's destruction!!! Futher use could lead to segmentation fault!!!\n\n",
        //                           "WARNING! User tried to pop from poisoned area\n\n"
    }
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

int StackCreator (Stack* stack, unsigned long capacity, const char* FileName, const char* FuncName, const int LineNum)
{
    if ( (FileName == NULL) || (FuncName == NULL) || (LineNum < 0) )
    {
        printf("You freakin' dummy, you are not supposed to touch the funcname, filename or LineNum parametres, you were given a damn macros to autofill that\n");
        return 0;
    }

    if (CheckStack(stack) == NULL_STK_PTR)
    {
        StackDump(stack, FileName, LineNum, FuncName, NULL_STK_PTR, "to print null stk ptr");
        return NULL_STK_PTR;
    }

    #if CNRY_DEF
    elem_t* ActualStackPtr = (elem_t*)calloc(1, capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

    stack->data = (elem_t*)((char*)ActualStackPtr + sizeof(canary_t));
    #else 
    stack->data = (elem_t* )calloc(capacity, sizeof(elem_t));
    #endif

    stack->size = 0;
    stack->capacity = capacity;

    stack->PlaceCrtd.FileName = FileName;
    stack->PlaceCrtd.FuncName = FuncName;
    stack->PlaceCrtd.LineNum = LineNum;

    FillPoison(stack->data, capacity);

    #if CNRY_DEF
    stack->LeftCanary = STK_CANARY_VAL;
    stack->RightCanary = STK_CANARY_VAL;

    ((canary_t*)stack->data)[-1] = DATA_CANARY_VAL;
    *(canary_t*)(stack->data + stack->capacity) = DATA_CANARY_VAL;
    #endif

    #if HASH_DEF
    stack->StackHash = GetStackHash(stack);
    stack->DataHash  = GetDataHash(stack);
    #endif

    //printf("IN CTOR: LeftCanary = %lX, RightCanary = %lX\n", ((canary_t*)stack->data)[-1], *(canary_t*)((char*)stack->data + stack->capacity * stack->DataSize));
    return 0;
}


//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

int StackDestructor (Stack* stack, const char* FileName, const char* FuncName, int LineNum)
{
    if ( (FileName == NULL) || (FuncName == NULL) || (LineNum < 0) )
    {
        printf("You freakin' dummy, you are not supposed to touch the funcname, filename or LineNum parametres, you were given a damn macros to autofill that\n");
        printf("exiting the programm");
        exit(1);
    }

    #if VRFCTR_DEF
    int Error = CheckStack(stack);
    if (Error)
    {
        TrackErrorWithCallPlc(Error, FileName, LineNum, FuncName);
    }
    #endif

    #if CNRY_DEF
    free( ((char*)stack->data - sizeof(canary_t)) );

    #else 
    free(stack->data);
    #endif

    stack->size = -1;
    stack->capacity = -1;
    stack->PlaceCrtd.FileName = (char*) BAD_POINTER;
    stack->PlaceCrtd.FuncName = (char*) BAD_POINTER;
    stack->PlaceCrtd.LineNum = -1;

    stack->data = BAD_POINTER;

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

int StackPush(Stack* stack, elem_t src)
{
    int Error = 0;
    #if VRFCTR_DEF
    Error = CheckStack(stack);
    TrackError(Error);
    #endif

    if (stack->size == stack->capacity)
    {
        Error = StackResize (stack, ENLARGE);
        TrackError(Error);
    }

    (stack->data)[stack->size] = src;

    stack->size++;

    #if HASH_DEF
    stack->StackHash = GetStackHash(stack);
    stack->DataHash  = GetDataHash(stack);
    #endif

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

int StackPop(Stack* stack, elem_t* dst)
{
    int Error = 0;
    #if VRFCTR_DEF
    Error = CheckStack(stack);
    TrackError(Error);
    #endif

    if (stack->size == 0)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, NOTHING_TO_POP);
        return NOTHING_TO_POP;
    }

    if (stack->data[stack->size - 1] == POISON)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, POP_FROM_POISONED);
        return POP_FROM_POISONED;
    }

    if ( stack->size < ((stack->capacity / 2) - 1)  )
    {
        //printf("Starting reducing\n");
        Error = StackResize(stack, REDUCE);
        TrackError(Error);
    }

    if (dst != 0)
    {
        *dst = stack->data[stack->size - 1];
    }

    stack->data[stack->size - 1] = POISON;
    stack->size--;

    #if HASH_DEF
    stack->StackHash = GetStackHash(stack);
    stack->DataHash  = GetDataHash(stack);
    #endif

    return 0;

}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

int StackResize (Stack* stack, int mode, size_t NewSize)
{
    #if VRFCTR_DEF
    int Error = CheckStack(stack);
    TrackError(Error);
    #endif

    if (NewSize == 0)
    {
        if (mode == ENLARGE)
            if (stack->capacity == 0)
                NewSize = sizeof(elem_t) * (stack->capacity + 4);
            else
                NewSize = sizeof(elem_t) * stack->capacity * MULT_CONST;
        else
            NewSize = stack->capacity * (sizeof(elem_t) / MULT_CONST);
    }

    #if CNRY_DEF

    elem_t* tmp = (elem_t*)realloc((char*)stack->data - sizeof(canary_t), NewSize + 2 * sizeof(canary_t));
    if(tmp == 0)
        return NOT_RESIZABLE;
    stack->data = (elem_t*)((char*)tmp + sizeof(canary_t));

    ((canary_t*)stack->data)[-1] = DATA_CANARY_VAL;
    *((canary_t*)( ( stack->data + NewSize / sizeof(elem_t) ))) = DATA_CANARY_VAL;

    //printf("char + ... = %p, stack + ... = %p\n",(char*)stack->data + NewSize, stack->data + NewSize/sizeof(elem_t));
    #else
    void* tmp = realloc(stack->data, NewSize);
    if(tmp == 0)
        return NOT_RESIZABLE;

    stack->data = (elem_t*)tmp;
    #endif

    stack->capacity = NewSize / sizeof(elem_t);

    FillPoison(stack->data + stack->size, stack->capacity - stack->size);

    #if HASH_DEF
    stack->StackHash = GetStackHash(stack);
    stack->DataHash  = GetDataHash(stack);
    #endif

    return OK;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

int StackDump (const Stack* stack, const char* FileName, int LineNum, const char* FuncName, int UserError, const char reason[])
{
    fprintf(logfile, "%s%s\n\n", UpperBorder, UpperBorder);

    if ( (stack->PlaceCrtd.LineNum < 0) || (stack->PlaceCrtd.FileName == NULL) || (stack->PlaceCrtd.FuncName == NULL) )
    {
        fprintf(logfile, "Cant print where stack was created: this info was severly damaged or not properly initialised\n");
    }
    else
    {
        fprintf(logfile, "Stack was created at %s, at %s (%lu)\n", stack->PlaceCrtd.FileName, stack->PlaceCrtd.FuncName, stack->PlaceCrtd.LineNum);
    }

    fprintf(logfile, "StackDump() Called from function: %s, File: %s line number %D for reason: %s, with error %d\n\n",FuncName, FileName, LineNum, reason, UserError);

    if (UserError < 0)
    {
        fprintf(logfile, "%s", GetUserError(UserError));
        if (UserError == USE_AFTER_DESTRUCT)
        {
            fprintf(logfile, "%s%s\n\n", LowerBorder, LowerBorder);
            return USE_AFTER_DESTRUCT;
        }
    }

    #if VRFCTR_DEF
    int StackError = CheckStack(stack);

    if (StackError & NULL_STK_PTR)
    {
        fprintf(logfile, "%s", StackErrors[0]);
        return NULL_STK_PTR;
    }

    fprintf(logfile, "Stack [%p]\n", stack);
    if (StackError & NOT_INIT)
    {
        fprintf(logfile, "%s", StackErrors[1]);
        fprintf(logfile, "%s%s\n\n", LowerBorder, LowerBorder);
        return NOT_INIT;
    }

    if(StackError & CAP_LESS_SIZE)
    {
        fprintf(logfile, "%s", StackErrors[2]);
    }

    fprintf(logfile, "Size = %lu\n", stack->size);
    fprintf(logfile, "capacity = %lu\n", stack->capacity);

    if (StackError & NULL_DATA_PTR)
    {
        fprintf(logfile, "%s", StackErrors[3]);
        fprintf(logfile, "%s%s\n\n", LowerBorder, LowerBorder);
        return NULL_DATA_PTR;
    }

    fflush(logfile);

    #if CNRY_DEF

    if (StackError & DATA_CANARY_DMG)
    {
        fprintf(logfile, "%s\nExpected values:\nLeft Canary - %lX, RightCanary - %lX;\ngot:\n", StackErrors[4], DATA_CANARY_VAL, DATA_CANARY_VAL);
    }
    fprintf(logfile, "Canaries around data: Left Canary - %lX, RightCanary - %lX\n\n", ((canary_t*)stack->data)[-1], 
                                                *(canary_t*)(stack->data + stack->capacity) );

    if (StackError & STK_CANARY_DMG)
    {
        fprintf(logfile, "%s\nExpected values:\nLeft Canary - %lX, RightCanary - %lX;\ngot:\n", StackErrors[5], STK_CANARY_VAL, STK_CANARY_VAL);
    }
    fprintf(logfile, "Canaries around stack: Left Canary - %lX, RightCanary - %lX\n\n", stack->LeftCanary, stack->RightCanary);

    #endif

    #if HASH_DEF

    if (StackError & STK_HASH_ERR)
    {
        fprintf (logfile, "%s", StackErrors[6]);
    }

    if (StackError & DATA_HASH_ERR)
    {
        fprintf (logfile, "%s", StackErrors[7]);
    }

    #endif

    fprintf(logfile, "dataptr = %p\n", stack->data);
    for (int i = 0; i < stack->size; i++)
    {
        fprintf(logfile, "data[%3d]: ", i);
        ElemDump(stack->data + i);
        if (stack->data[i] == POISON)
            fprintf(logfile, "     <------ WARNING!!! This is very strange. This area is concedered as containing usefull data, but this is poisoned\n");
        fprintf(logfile, "\n");
    }

    #if FULL_DATA_PRINT
    fprintf(logfile, "\n-----------Futher area not used-----------\n\n");

    for (int i = stack->size; i < stack->capacity; i++)
    {
        fprintf(logfile, "data[%3d]: ", i);
        ElemDump(stack->data + i);
        if (stack->data[i] == POISON)
            fprintf(logfile, "   (POISON)\n");
        else
            fprintf(logfile, "     <------ WARNING!!! This is very strange. This area is concedered as containing only poison, but this is something different\n");
    }
    #endif

    #endif

    fprintf(logfile, "%s%s\n\n", LowerBorder, LowerBorder);
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

int VerifyStack (const Stack* stack, const char* FileName, const char* FuncName, int LineNum)
{
    int Error = OK;

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

    if ((stack->data == BAD_POINTER) || (stack->PlaceCrtd.FileName == (char*) BAD_POINTER) || (stack->PlaceCrtd.FuncName == (char*) BAD_POINTER))
        return USE_AFTER_DESTRUCT;

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
    hash_t stackHash = GetStackHash(stack);
    if (stackHash != stack->StackHash)
        Error |= STK_HASH_ERR;

    hash_t dataHash = GetDataHash(stack);
    if (dataHash != stack->DataHash)
        Error |= DATA_HASH_ERR;
    #endif

    return Error;
}
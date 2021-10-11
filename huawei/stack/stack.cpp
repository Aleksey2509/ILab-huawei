#include "stack.hpp"

//------------------------------------------------------------------------------------------------------------------------------


#if HASH_DEF

unsigned long Hash( const void* key, size_t len, unsigned long prime)
{
    unsigned long hash = (unsigned long) len;

    for (size_t i = 0; i < len; i++)
        hash = (hash << 5)^(hash >> 27) ^ ((char*)key)[i];

    return (hash % prime);
}

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

hash_t GetDataHash (const Stack* stack)
{
    size_t byteLen = stack->size * stack->DataSize;
    hash_t DataHash = Hash(stack, byteLen, prime);

    return DataHash;
}

#endif



//------------------------------------------------------------------------------------------------------------------------------

FILE* logfile = fopen("log.txt", "w");

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

int StackDtor (Stack* stack)
{
    int Error = CheckStack(stack);
    TrackError(Error);

    stack->size = -1;
    stack->DataSize = -1;

    #if CNRY_DEF
        free( ((char*)stack->data - sizeof(canary_t)) );
    #else 
        free(stack->data);
    #endif
    
    stack->data = (void* ) BAD_POINTER;

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int StackPush(Stack* stack, void* src)
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

    void* placeToCopy = (void*)( (char*)stack->data + stack->DataSize * stack->size);
    memcpy( placeToCopy , src, stack->DataSize);

    stack->size++;

    #if HASH_DEF
    stack->StackHash = GetStackHash(stack);
    stack->DataHash  = GetDataHash(stack);
    #endif

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int StackPop(Stack* stack, void* dst)
{
    int Error = CheckStack(stack);
    TrackError(Error);

    if (stack->size == 0)
    {
        StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, NOTHING_TO_POP);
        return NOTHING_TO_POP;
    }

    if (dst == 0)
    {
        return NULL_DST_PTR;
    }

    if ( (stack->size < (stack->capacity / 2) ) )
    {
        //printf("Starting reducing\n");
        Error = StackResize(stack, REDUCE);
        TrackError(Error);
    }
    memcpy(dst, (void*)( (char*)stack->data + stack->DataSize * (stack->size - 1) ), stack->DataSize);

    stack->size--;

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

int StackDump (const Stack* stack, const char* FileName, int LineNum, const char* FuncName, int Error, const char reason[])
{
    fprintf(logfile, "%s%s\n\n", UpperBorder, UpperBorder);

    fprintf(logfile, "Stack was created at %s, at %s(%lu)\n", stack->CreateFunc, stack->CreateFile, stack->CreateLine);
    fprintf(logfile, "StackDump() Called from function: %s, File: %s line number %D for reason: %s, with error %d\n\n",FuncName, FileName, LineNum, reason, Error);
    
    if (!Error)
        Error = CheckStack(stack);

    if (Error == NOTHING_TO_POP)
    {
        fprintf(logfile, "Tried to pop when there is nothing to pop\n");
    }

    if (Error == NOWHERE_TO_PUT)
    {
        fprintf(logfile, "Tried to put data when the stack is full and there is not enough memory for resize\n");
    }

    if (Error == NULL_SRC_PTR)
    {
        fprintf(logfile, "Bad source data pointer\n");
    }
    
    if (Error == NULL_DST_PTR)
    {
        fprintf(logfile, "Bad destination pointer\n");
    }

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

    #if CNRY_DEF
    {
        if (Error == DATA_CANARY_DMG)
        {
            fprintf(logfile, "ERROR!!! THE CANARIES AROUND DATA WERE DAMAGED:\nExpected values:\nLeft Canary - %lX, RightCanary - %lX;\ngot:\n", DATA_CANARY_VAL, DATA_CANARY_VAL);
        }
        fprintf(logfile, "Canaries around data: Left Canary - %lX, RightCanary - %lX\n\n", ((canary_t*)stack->data)[-1], 
                                                    *(canary_t*)((char*)stack->data + stack->capacity * stack->DataSize) );

        if (Error == STK_CANARY_DMG)
        {
            fprintf(logfile, "ERROR!!! THE CANARIES WERE AROUND STACK STRUCT WERE DAMAGED:\nExpected values:\nLeft Canary - %lX, RightCanary - %lX;\ngot:\n", STK_CANARY_VAL, STK_CANARY_VAL);
        }
        fprintf(logfile, "Canaries around stack: Left Canary - %lX, RightCanary - %lX\n\n", stack->LeftCanary, stack->RightCanary);
    }
    #endif

    #if HASH_DEF
    {
       if (Error == STK_HASH_ERR)
       {
           fprintf (logfile, "ERROR!!! STACK HASH HAS BEEN CHANGED!!! THE STACK HAS BEEN INTERACTED WITH IN AN WRONG WAY\n");
       }

       if (Error == DATA_HASH_ERR)
       {
           fprintf (logfile, "ERROR!!! DATA HASH HAS BEEN CHANGED!!! THE STACK DATA HAS BEEN INTERACTED WITH IN AN WRONG WAY\n");
       }
    }
    #endif

    fprintf(logfile, "dataptr = %p\n", stack->data);
    for (int i = 0; i < stack->size; i++)
    {
        fprintf(logfile, "data[%d]: ", i);
        ElemDump((void*)((char*)stack->data + i * stack->DataSize));
        fprintf(logfile, "\n");
    }

    fprintf(logfile, "%s%s\n\n", UpperBorder, UpperBorder);
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

ErrorCodes CheckStack (const Stack* stack)
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

    #if HASH_DEF
    {
        hash_t stackHash = GetStackHash(stack);
        if (stackHash != stack->StackHash)
            return STK_HASH_ERR;

        hash_t dataHash = GetDataHash(stack);
        if (dataHash != stack->DataHash)
            return DATA_HASH_ERR;
    }
    #endif

    return OK;
}
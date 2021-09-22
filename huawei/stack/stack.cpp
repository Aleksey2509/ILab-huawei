#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

enum ErrorCodes
{
    NOT_RESIZABLE = 0,
    RESIZED,


};

struct Stack
{
    int* data;
    unsigned long capacity;
    unsigned long size;

};



int StackCtor (Stack* stack, unsigned long capacity)
{
    assert(stack);

    stack->data = (int*)calloc(capacity, sizeof(int));
    stack->size = 0;
    stack->capacity = capacity;
}


int StackDtor (Stack* stack)
{
    assert(stack);

    stack->size = -1;
    free(stack->data);
    stack->data = (int*) 13;
}

int StackPush(Stack* stack, int value)
{
    assert(stack);


    stack->data [stack->size++] = value;
}

int StackPop(Stack* stack)
{
    assert(stack);

    StackResize(stack);

    return(stack->data [stack->size--]);
}

int StackResize (Stack* stack)
{
    if (stack->size == stack->capacity)
    {
        int* tmp = (int*)realloc(stack->data, 2 * stack->capacity);

        if(tmp != 0)
        {
            stack->data = tmp;
            return RESIZED;

        }
        else return NOT_RESIZABLE;
    }
    if ((stack->size == (stack->capacity / 2) - 2) && (stack->size > 4))
    {
        int* tmp = (int*)realloc(stack->data, stack->capacity / 2);

        if(tmp != 0)
        {
            stack->data = tmp;
            return RESIZED;
        }
        else return NOT_RESIZABLE;
    }
}
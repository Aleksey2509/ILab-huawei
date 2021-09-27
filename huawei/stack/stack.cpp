#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

const int MULT_CONST = 2;

struct Stack
{
    int* data;
    unsigned long capacity;
    unsigned long size;

};

int StackCtor (Stack* stack, unsigned long capacity);
int StackResize (Stack* stack);
int StackDtor (Stack* stack);
int StackPush(Stack* stack, int value);
int StackPop(Stack* stack);
int printStack (Stack* stack);
int Enlarge (Stack* stack);
int Reduce (Stack* stack);


enum ErrorCodes
{
    NOT_RESIZABLE = 0,
    RESIZED,


};

int StackCtor (Stack* stack, unsigned long capacity)
{
    assert(stack);

    stack->data = (int*)calloc(capacity, sizeof(int));
    stack->size = 0;
    stack->capacity = capacity;

    return 0;
}


int StackDtor (Stack* stack)
{
    assert(stack);

    stack->size = -1;
    free(stack->data);
    stack->data = (int*) 13; // 13 - not good

    return 0;
}

int StackPush(Stack* stack, int value)
{
    assert(stack);

    if (stack->size == stack->capacity)
        Enlarge (stack);

    stack->data [stack->size++] = value;

    return 0;
}

int StackPop(Stack* stack)
{
    assert(stack);

    if ( (stack->size < (stack->capacity / 2) - 1 ) && (stack->capacity > 4) )
    {
        printf("Starting reducing\n");
        Reduce(stack);
    }
    return stack->data[--stack->size];

}

int printStack (Stack* stack)
{
    for (int i = 0; i < stack->size; i++)
    {
        printf("%d ", stack->data[i]);
    }
    printf("size = %lu, capacity = %lu\n", stack->size, stack->capacity);
    return 0;
}

int Enlarge (Stack* stack)
{
    assert(stack);

    int* tmp = (int*)realloc(stack->data, sizeof(int) * stack->capacity * MULT_CONST);
    if(tmp == 0)
        return NOT_RESIZABLE;

    stack->data = tmp;
    stack->capacity *= MULT_CONST;

    return RESIZED;
}

int Reduce (Stack* stack)
{
    assert(stack);
    
    int* tmp = (int*)realloc(stack->data, sizeof(int) * stack->capacity / MULT_CONST);
    if(tmp == 0)
        return NOT_RESIZABLE;

    stack->data = tmp;
    stack->capacity /= MULT_CONST;
    return RESIZED;
}
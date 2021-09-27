#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include "stack.cpp"


int main()
{
    Stack test;
    int capacity = 10;

    StackCtor(&test, capacity);

    StackPush(&test, 5);
    StackPush(&test, 6);
    StackPush(&test, 7);
    StackPush(&test, 8);

    printStack(&test);

    int a = StackPop(&test);
    int b = StackPop(&test);
    int c = StackPop(&test);

    printf("a = %D, b = %D, c = %d\n", a, b, c);


    printStack(&test);

    StackDtor(&test);
}
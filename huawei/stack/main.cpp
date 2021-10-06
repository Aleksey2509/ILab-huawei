#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include "stack.cpp"

int ElemDump(void* ptr)
{
    fprintf(logfile, "%d", *(int*)ptr);
    return 0;
}

int main()
{
    int a = 0, b = 0, c = 0;

    Stack test = {0};
    int capacity = 20;
    int valArr[4] = {120, 121, 122, 123};

    StackCtor(&test, capacity, sizeof(int));
    StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    StackPush(&test, (void *)(valArr));
    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    StackPush(&test, (void *)(valArr + 1));
    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    StackPush(&test, (void *)(valArr + 2));
    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    StackPush(&test, (void *)(valArr + 3));
    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    StackPop(&test, &a);
    StackPop(&test, &b);
    StackPop(&test, &c);

    printf("a = %D, b = %D, c = %d\n", a, b, c);

    //printf("left canaty ptr - %p,left counted canary ptr - %p", test.LeftCanary, test.RightCanary);

    StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    StackDtor(&test);
}
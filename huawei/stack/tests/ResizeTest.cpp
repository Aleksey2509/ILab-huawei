#include "../stack.hpp"

extern FILE* logfile;

const int POISON = -100;

int ElemDump(elem_t* ptr, FILE* logfile)
{
    fprintf(logfile, "%d", *(int*)ptr);
    return 0;
}

int main()
{
    Stack test {};
    int capacity = 5;
    int valArr[5] = {120, 121, 122, 123, 124};

    StackCtor(&test, capacity);
    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (int i = 0; i < sizeof(valArr)/sizeof(int); i++)
    {
        StackPush(&test, valArr[i]);
    }

    StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);


    for (int i = 0; i < sizeof(valArr)/sizeof(int); i++)
    {
        StackPop(&test, valArr + i);
    }

    StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    // printf("Printing valArr:\n");
    // for (int i = 0; i < sizeof(valArr)/sizeof(int); i++)
    // {
    //     printf("%d ", valArr[i]);
    // }
    printf("\n");

    StackDtor(&test);
}
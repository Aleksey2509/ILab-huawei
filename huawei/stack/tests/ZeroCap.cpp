#include "../stack.hpp"

extern FILE* logfile;

int ElemDump(elem_t* ptr)
{
    fprintf(logfile, "%d", *(int*)ptr);
    return 0;
}

elem_t POISON = 1;

int main()
{
    Stack test {};
    int capacity = 0;
    int valArr[4] = {120, 121, 122, 123};

    StackCtor(&test, capacity);
    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (int i = 0; i < sizeof(valArr)/sizeof(int); i++)
    {
        StackPush(&test, valArr + i);
        StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }

    StackDtor(&test);
}
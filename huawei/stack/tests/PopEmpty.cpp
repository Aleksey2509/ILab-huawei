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
    Stack test = {};
    int capacity = 20;
    int valArr[4] = {120, 121, 122, 123};

    StackCtor(&test, capacity);
    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    StackPush(&test, (valArr + 1));
    StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    StackPop(&test);
    StackPop(&test);

    StackDtor(&test);
}
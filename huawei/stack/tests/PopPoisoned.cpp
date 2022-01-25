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
    int capacity = 15;
    int valArr[4] = {120, 121, 122, 123};

    StackCtor(&test, capacity);
    StackPush(&test, valArr[0]);
    StackPush(&test, valArr[1]);
    StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);
    test.size = 3;

    StackPop(&test);

    StackDtor(&test);
}
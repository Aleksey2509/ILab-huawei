#include "../stack.hpp"

extern FILE* logfile;

int ElemDump(elem_t* ptr)
{
    fprintf(logfile, "%d", *(int*)ptr);
    return 0;
}

elem_t POISON = 1;

struct execution
{
    int rightExecuter[1];
    Stack victim;
    int leftExecuter[1];
};

int main()
{
    execution accident = {};

    accident.victim = {};
    int capacity = 20;
    int valArr[4] = {120, 121, 122, 123};

    StackCtor(&accident.victim, capacity);
    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    //StackDump(&test, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    StackPush(&accident.victim, (valArr + 1));
    StackDump(&accident.victim, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (int i = 0; i < 3; i++)
    {
        accident.rightExecuter[i] = 0;
    }

    for (int i = 0; i > -3; i--)
    {
        accident.leftExecuter[i] = 0;
    }

    StackPop(&accident.victim);

    StackDtor(&accident.victim);
}
#include "../headers/list.hpp"

int GraphicDump(List* lst);

static const char DotCmd[] = {"dot -Tps graph2.gv -o graph2.ps"};

int main()
{
    List testSubj {};

    ListCtor(&testSubj, 10);

    ListDump(&testSubj);

    int place10 = ListAdd(&testSubj, 1, 10);

    int place20 = ListAdd(&testSubj, place10, 20);

    int place30 = ListAdd(&testSubj, place20, 30);

    int place40 = ListAdd(&testSubj, place10, 40);

    ListDump(&testSubj);
    GraphicDump(&testSubj);
}

int GraphicDump(List* lst)
{
    FILE* log = fopen("graph2.gv", "w");
    fprintf(log, "digraph g {\n");
    fprintf(log, "    rankdir = TB\n");
    fprintf(log, "    node [shape = record];\n");

    for (int i = 0; i < lst->capacity; i++)
    {
        fprintf(log, "    node%d[label = \"{node %d | <f0>  %d|<f1> %d|<f2> %d}\"];\n",
                i, i, lst->node[i].prev, lst->node[i].value, lst->node[i].next);
    }

    fprintf(log, "\n");

    for (int i = 0; i < lst->capacity - 1; i++)
    {
        fprintf(log, "    \"node%d\":f2 -> \"node%d\":f0[style=\"invis\"];\n", i, i + 1);
    }

    fprintf(log, "\n");

    int head = lst->head;
    int tail = lst->tail;
    int currentNode = head;
    while( currentNode != 0 )
    {
        fprintf(log, "    \"node%d\":f0 -> \"node%d\":f1;\n", currentNode, lst->node[currentNode].prev);
        fprintf(log, "    \"node%d\":f2 -> \"node%d\":f1;\n", currentNode, lst->node[currentNode].next);
        currentNode = lst->node[currentNode].next;
        printf("in actual data: currentNode = %d\n", currentNode);
    }

    fprintf(log, "\n");

    for (currentNode = lst->free; (currentNode != 0);)
    {
        fprintf(log, "    \"node%d\":f2 -> \"node%d\":f1;\n",
                currentNode, lst->node[currentNode].next);
        currentNode = lst->node[currentNode].next;
        printf("in free: currentNode = %d\n", currentNode);
    }

    fprintf(log, "}");

    fclose(log);

    system(DotCmd);
    return 0;
}
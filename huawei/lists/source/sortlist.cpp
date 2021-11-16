#include "../headers/sortlist.hpp"

int SortList(List* lst, int nodeCount)
{
    qsort(lst->node + 1, nodeCount, sizeof(ListNode), Comparator);

    for (int i = 1; i < nodeCount; i++)
    {
        lst->node[i].next = i + 1;
        lst->node[i].prev = i - 1;
    }
    lst->node[nodeCount].next = 0;
    lst->node[nodeCount].prev = nodeCount - 1;

    lst->head = 1;
    lst->tail = nodeCount;
    lst->free = nodeCount + 1;

    return OK;
}

int Comparator (const void* arg1, const void* arg2)
{
    ListNode node1 = *(ListNode*)arg1;
    ListNode node2 = *(ListNode*)arg2;

    unsigned a = (unsigned)node1.prev;
    unsigned b = (unsigned)node2.prev;

    if (a == b)
        return node1.next - node2.next;
    else
        return a - b;
}
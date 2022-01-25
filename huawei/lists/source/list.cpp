#include "list.hpp"

int ListCtor (List* lst, size_t capacity)
{
    if (!lst)
        return NULL_LST_PTR;

    lst->node = (ListNode*)calloc(capacity + 1, sizeof(ListNode));

    if (lst->node == NULL)
        return CANT_ALLOC_MEM;

    lst->head = lst->tail = 0;
    lst->capacity = capacity;
    lst->free = 1;
    lst->sorted = 1;

    memset(lst->node + 1, FREE_SPOT, (capacity) * sizeof(ListNode));

    for (int i = 1; i < capacity; i++)
    {
        lst->node[i].next = i + 1;
    }

    lst->node[capacity].next = 0;

    return OK;
}

int ListDtor (List* lst)
{
    #if VRFCTR_CHECK
    int Error = ListVerify(lst);
    if (Error)
    {
        ListGraphicDump(lst);
        return Error;
    }
    #endif

    if (lst->node != NULL)
    {
        free(lst->node);
        lst->node = NULL;
    }

    lst->head = lst->tail = lst->free = 0;
    lst->capacity = 0;

    return OK;
}

int LogicalToPhysical (List* lst, int logicalPlace)
{
    #if VRFCTR_CHECK
    int Error = ListVerify(lst);
    if (Error)
    {
        ListGraphicDump(lst);
        return Error;
    }
    #endif

    if (lst->sorted)
        return logicalPlace;

    int physicalPlace = lst->head;
    for (int i = 0; (i < logicalPlace) && (physicalPlace != 0); i++)
    {
        physicalPlace = lst->node[physicalPlace].next;
    }

    return physicalPlace;
}

int ListResize (List* lst)
{
    #if VRFCTR_CHECK
    int Error = ListVerify(lst);
    if (Error)
    {
        ListGraphicDump(lst);
        return Error;
    }
    #endif

    ListNode* tmp = (ListNode*)realloc(lst->node, (2 * lst->capacity + 1) * sizeof(ListNode));
    if (tmp == NULL)
        return CANT_RESIZE_LIST;

    lst->node = tmp;

    memset(lst->node + lst->capacity + 1, FREE_SPOT, lst->capacity * sizeof(ListNode));

    for (int i = lst->capacity + 1; i < 2 * lst->capacity; i++)
    {
        lst->node[i].next = i + 1;
    }
    lst->node[2 * lst->capacity].next = 0;

    lst->free = lst->capacity + 1;
    lst->capacity = 2 * lst->capacity;

    return OK;
}

int ListAddBefore (List* lst, int index, int value)
{
    #if VRFCTR_CHECK
    int Error = ListVerify(lst);
    if (Error)
    {
        ListGraphicDump(lst);
        return Error;
    }
    #endif

    if ((lst->node[index].prev == FREE_SPOT) && (lst->head != 0))
        return 0;
    //printf("adding after %d val %d\n", index, val);

    int freeIndex = lst->free;

    //printf("free spot - %d\n", freeIndex);
    if (freeIndex == 0)
    {
        int Error = ListResize(lst);
        if (Error)
            return Error;
        freeIndex = lst->free;
    }
    lst->free = lst->node[freeIndex].next;

    lst->node[freeIndex].value = value;

    if (lst->head == 0)
    {
        lst->head = freeIndex;
        lst->tail = freeIndex;
        
        lst->node[freeIndex].next = 0;
        lst->node[freeIndex].prev = 0;
        lst->sorted = 1;

        return freeIndex;
    }

    lst->node[freeIndex].prev = lst->node[index].prev;
    lst->node[freeIndex].next = index;
    lst->node[index].prev = freeIndex;
    lst->sorted = 0;

    int prev = lst->node[freeIndex].prev;

    if (lst->node[prev].prev != 0)
        lst->node[prev].next = freeIndex;

    if (lst->tail == index)
        lst->tail = freeIndex;

    return freeIndex;
}

int ListAddAfter (List* lst, int index, int value)
{
    #if VRFCTR_CHECK
    int Error = ListVerify(lst);
    if (Error)
    {
        ListGraphicDump(lst);
        return Error;
    }
    #endif

    if ((lst->node[index].prev == FREE_SPOT) && (lst->head != 0))
        return 0;
    //printf("adding after %d val %d\n", index, val);

    int freeIndex = lst->free;

    //printf("free spot - %d\n", freeIndex);
    if (freeIndex == 0)
    {
        int Error = ListResize(lst);
        if (Error)
            return Error;
        freeIndex = lst->free;
    }
    lst->free = lst->node[freeIndex].next;

    lst->node[freeIndex].value = value;

    if (lst->head == 0)
    {
        lst->head = freeIndex;
        lst->tail = freeIndex;
        
        lst->node[freeIndex].next = 0;
        lst->node[freeIndex].prev = 0;
        lst->sorted = 1;

        return freeIndex;
    }


    lst->node[freeIndex].next = lst->node[index].next;
    lst->node[freeIndex].prev = index;
    lst->node[index].next = freeIndex;

    int next = lst->node[freeIndex].next;

    if (lst->node[next].prev != 0)
        lst->node[next].prev = freeIndex;

    if (lst->tail == index)
        lst->tail = freeIndex;
    else
        lst->sorted = 0;

    return freeIndex;

}


int ListRemove (List* lst, int index)
{
    #if VRFCTR_CHECK
    int Error = ListVerify(lst);
    if (Error)
    {
        ListGraphicDump(lst);
        return Error;
    }
    #endif

    if ((lst->node[index].next == FREE_SPOT) || (lst->node[index].prev == FREE_SPOT))
        return 0;

    int next = lst->node[index].next;
    int prev = lst->node[index].prev;
    int value = lst->node[index].value;
    lst->node[index].value = -1;

    if (index == lst->head)
    {
        lst->head = next;
    }

    if (index == lst->tail)
        lst->tail = prev;
    else
        lst->sorted = 0;

    if (lst->node[prev].next != 0)
        lst->node[prev].next = lst->node[index].next;

    if (lst->node[next].prev != 0)
        lst->node[next].prev = lst->node[index].prev;

    lst->node[index].next = lst->free;
    lst->free = index;
    lst->node[index].prev = FREE_SPOT;

    return value;
}

int ListDump (List* lst)
{
    FILE* logfile = fopen("log.txt", "a");

    int Error = ListVerify(lst);
    if (Error == NULL_LST_PTR)
        return Error;

    if (Error == NULL_CAP)
        fprintf(logfile, "Warning! Capacity of zero\n");
    if (Error == INIT_ERROR)
        fprintf(logfile, "Warning! The head an tail are corrupted!\n");
    if (Error == HEAD_NOT_PROPER)
        fprintf(logfile, "Warning! There is an element before the one called the head!\n");
    if (Error == TAIL_NOT_PROPER)
        fprintf(logfile, "Warning! There is an element after the one called the tail!\n");

    fprintf(logfile, "\nhead = %d, capacity - %zu, free - %d, tail - %d, sorted status - %d\n", lst->head, lst->capacity, lst->free, lst->tail, lst->sorted);

    if (Error == NULL_NODE_PTR)
        fprintf(logfile, "Warning! Null data ptr!!\n");
    else
    {
        if (Error == UNLINKED_LIST)
            fprintf(logfile, "Warning! There is a dislinked place in the list!\n");
        fprintf(logfile, "data arr -->");
        for (int i = 0; i < lst->capacity + 1; i++)
        {
            fprintf(logfile, "%3d ", lst->node[i].value);
        }

        fprintf(logfile, "\nnext arr -->");
        for (int i = 0; i < lst->capacity + 1; i++)
        {
            fprintf(logfile, "%3d ", lst->node[i].next);
        }

        fprintf(logfile, "\nprev arr -->");
        for (int i = 0; i < lst->capacity + 1; i++)
        {
            fprintf(logfile, "%3d ", lst->node[i].prev);
        }
    }

    if (Error == CYCLED_LIST)
        fprintf(logfile, "Warning! There is an cycle in the list!\n");
    fprintf(logfile, "\n\n");

    fclose(logfile);

    return OK;
}

int ListGraphicDump(List* lst)
{
    FILE* log = fopen("graph2.gv", "w");
    fprintf(log, "digraph g {\n");
    fprintf(log, "    rankdir = TB\n");
    fprintf(log, "    node [shape = record];\n");

    for (int i = 0; i < lst->capacity + 1; i++)
    {
        fprintf(log, "    { rank = same node%d[label = \"<name> node %d | <prev>  %d|<data> %d|<next> %d\"]; }\n",
                i, i, lst->node[i].prev, lst->node[i].value, lst->node[i].next);
    }

    fprintf(log, "\n");

    for (int i = 0; i < lst->capacity; i++)
    {
        fprintf(log, "    \"node%d\":next -> \"node%d\":prev[style=\"invis\"];\n", i, i + 1);
    }

    fprintf(log, "\n");

    int head = lst->head;
    int tail = lst->tail;
    int currentNode = head;
    while( currentNode != 0 )
    {
        fprintf(log, "    \"node%d\":prev -> \"node%d\":name;\n", currentNode, lst->node[currentNode].prev);
        fprintf(log, "    \"node%d\":next -> \"node%d\":name;\n", currentNode, lst->node[currentNode].next);
        currentNode = lst->node[currentNode].next;
        //printf("in actual data: currentNode = %d\n", currentNode);
    }

    fprintf(log, "\n");

    for (currentNode = lst->free; (currentNode != 0);)
    {
        fprintf(log, "    \"node%d\":next -> \"node%d\":name;\n",
                currentNode, lst->node[currentNode].next);
        currentNode = lst->node[currentNode].next;

        //printf("in free: currentNode = %d\n", currentNode);
    }

    fprintf(log, "}");

    fclose(log);

    system(DotCmd);
    return 0;
}

int ListVerify (List* lst)
{
    if (lst == NULL)
        return NULL_LST_PTR;

    if (lst->node == NULL)
        return NULL_NODE_PTR;

    if ( ((lst->head == 0) && (lst->tail != 0)) || ((lst->head != 0) && (lst->tail == 0)) )
        return INIT_ERROR;

    if (lst->capacity == 0)
        return NULL_CAP;

    if ((lst->head == 0) && (lst->tail == 0))
        return OK;

    int runThrough = lst->head;
    if (lst->node[runThrough].prev != 0)
        return HEAD_NOT_PROPER;

    int jumpCounter = 0;
    for (jumpCounter = 0; (runThrough != lst->tail) && (jumpCounter < lst->capacity + 2); jumpCounter++)
    {
        runThrough = lst->node[runThrough].next;

        int next = lst->node[runThrough].next;
        int prev = lst->node[runThrough].prev;

        if (((runThrough != lst->node[next].prev) && (next != 0))  || ((runThrough != lst->node[prev].next) && (prev != 0)))
        {
            // printf("Dislinked place: runThrogh - %d, next.prev - %d, prev.next - %D\n", runThrough, lst->node[next].prev, lst->node[next].prev);
            // printf("values: current - %d, next - %d, prev - %d\n", lst->node[runThrough].value, lst->node[next].value, lst->node[prev].value);
            return UNLINKED_LIST;
        }
    }
    //printf("\n\nin vrctr: runthrough = %d, cyclind = %d, lst->tail = %d\n\n", runThrough, jumpCounter, lst->tail);

    if (jumpCounter == lst->capacity + 1)
        return CYCLED_LIST;

    if (lst->node[runThrough].next != 0)
    {
        return TAIL_NOT_PROPER;
    }

    return OK;
}

int swapNodes(ListNode* node1, ListNode* node2)
{
    ListNode tmp;
    tmp = *node1;
    *node1 = *node2;
    *node2 = tmp;
    return 0;
}


int ListSort(List* lst)
{
    size_t nodeCount = lst->capacity;
    size_t takenNodes = 0;
    for(int runThrough = lst->head; lst->node[runThrough].next != lst->node[runThrough].prev; runThrough = lst->node[runThrough].next )
    {
        lst->node[runThrough].prev = takenNodes;
        takenNodes++;
    }

    int j = 1;
    for (int i = 1; i < takenNodes; )
    {
        if (lst->node[i].prev == FREE_SPOT)
        {
            swapNodes(lst->node + i, lst->node + takenNodes + j);
            j++;
        }
        else
            i++;
    }

    for (int i = 1; i < takenNodes + 1; )
    {
        if ((i - 1) != lst->node[i].prev)
            swapNodes(lst->node + i, lst->node + lst->node[i].prev + 1);
        else
            i++;
    }

    for (size_t i = 1; i < takenNodes + 1; i++)
    {
        lst->node[i].next = i + 1;
        lst->node[i].prev = i - 1;
    }
    lst->node[takenNodes].next = 0;

    for (size_t i = takenNodes; i < nodeCount; i++)
    {
        lst->node[i].next = i + 1 % (nodeCount + 1);
        lst->node[i].prev = FREE_SPOT;
    }

    lst->head = 1;
    lst->tail = takenNodes;
    lst->free = takenNodes + 1;
    lst->sorted = 1;

    return OK;
}
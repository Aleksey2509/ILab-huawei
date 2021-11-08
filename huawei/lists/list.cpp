#include "list.hpp"

int ListCtor (List* lst, size_t capacity)
{
    assert(lst);
    assert(capacity);

    lst->node = (ListNode*)calloc(capacity, sizeof(ListNode));

    if (lst->node == NULL)
        return CANT_ALLOC_MEM;

    lst->head = lst->tail = 0;
    lst->capacity = capacity;
    lst->free = 1;
    memset(lst->node + 1, FREE_SPOT, (capacity - 1) * sizeof(ListNode));

    for (int i = 1; i < capacity - 1; i++)
    {
        lst->node[i].next = i + 1;
    }

    lst->node[capacity - 1].next = 0;

    return OK;
}

int ListDtor (List* lst)
{
    assert(lst);

    if (lst->node != NULL)
    {
        free(lst->node);
        lst->node = NULL;
    }

    lst->head = lst->tail = lst->free =  0;
    lst->capacity = 0;

    return OK;
}

int LogicalToPhysical (List* lst, int logicalPlace)
{
    int Error = ListVerify(lst);
    if (Error)
    {
        ListDump(lst);
        return Error;
    }

    int physicalPlace = lst->head;

    for (int i = 0; i < logicalPlace; i++)
    {
        physicalPlace = lst->node[physicalPlace].next;
    }

    return physicalPlace;
}

int ResizeList (List* lst)
{
    ListNode* tmp = (ListNode*)realloc(lst->node, 2 * lst->capacity * sizeof(ListNode));
    if (tmp == NULL)
        return CANT_RESIZE_LIST;

    lst->node = tmp;

    memset(lst->node + lst->capacity, FREE_SPOT, lst->capacity * sizeof(ListNode));

    for (int i = lst->capacity; i < 2 * lst->capacity - 1; i++)
    {
        lst->node[i].next = i + 1;
    }
    lst->node[2 * lst->capacity - 1].next = 0;

    lst->free = lst->capacity;
    lst->capacity *= 2;

    return OK;
}

int ListAdd (List* lst, int index, int val) // add insert before
{
    int Error = ListVerify(lst);
    if (Error)
    {
        ListDump(lst);
        return Error;
    }

    if ((lst->node[index].next == FREE_SPOT) && (lst->head != 0))
        return 0;
    //printf("adding after %d val %d\n", index, val);

    int freeIndex = lst->free;

    //printf("free spot - %d\n", freeIndex);
    if (freeIndex == 0)
    {
        int Error = ResizeList(lst);
        if (Error)
            return Error;
        freeIndex = lst->capacity / 2;
    }
    lst->free = lst->node[freeIndex].next;

    lst->node[freeIndex].value = val;

    if (lst->head == 0)
    {
        lst->head = freeIndex;
        lst->tail = freeIndex;
        
        lst->node[freeIndex].next = 0;
        lst->node[freeIndex].prev = 0;

        return freeIndex;
    }


    lst->node[freeIndex].next = lst->node[index].next;
    lst->node[freeIndex].prev = index;
    lst->node[index].next = freeIndex;

    int next = lst->node[freeIndex].next;
    int prev = lst->node[freeIndex].prev;

    if (lst->node[next].prev != 0)
        lst->node[next].prev = freeIndex;

    if (lst->tail == index)
        lst->tail = freeIndex;

    return freeIndex;

}


int ListRemove (List* lst, int index)
{
    int Error = ListVerify(lst);
    if (Error)
    {
        ListDump(lst);
        return Error;
    }

    if ((lst->node[index].next == FREE_SPOT) || (lst->node[index].prev == FREE_SPOT))
        return 0;

    int next = lst->node[index].next;
    int prev = lst->node[index].prev;

    if (index == lst->head)
    {
        lst->head = next;
        //lst->node[next].prev = 0;
    }

    if (index == lst->tail)
    {
        lst->tail = prev;
        //lst->node[prev].next = 0;
    }

    if (lst->node[prev].next != 0)
        lst->node[prev].next = lst->node[index].next;

    if (lst->node[next].prev != 0)
        lst->node[next].prev = lst->node[index].prev;

    lst->node[index].next = lst->free;
    lst->free = index;
    lst->node[index].prev = FREE_SPOT;

    return OK;
}

int ListDump (List* lst)
{
    int Error = ListVerify(lst);
    if (Error == NULL_LST_PTR)
        return Error;

    if (Error == NULL_CAP)
        printf("Warning! Capacity of zero\n");
    if (Error == INIT_ERROR)
        printf("Warning! The head an tail are corrupted!\n");
    if (Error == HEAD_NOT_PROPER)
        printf("Warning! There is an element before the one called the head!\n");
    if (Error == TAIL_NOT_PROPER)
        printf("Warning! There is an element after the one called the tail!\n");

    printf("\nhead = %d, capacity - %zu, free - %d, tail - %d\n", lst->head, lst->capacity, lst->free, lst->tail);

    if (Error == NULL_NODE_PTR)
        printf("Warning! Null data ptr!!\n");
    else
    {
        if (Error == UNLINKED_LIST)
            printf("Warning! There is a dislinked place in the list!\n");
        printf("data arr -->");
        for (int i = 0; i < lst->capacity; i++)
        {
            printf("%3d ", lst->node[i].value);
        }

        printf("\nnext arr -->");
        for (int i = 0; i < lst->capacity; i++)
        {
            printf("%3d ", lst->node[i].next);
        }

        printf("\nprev arr -->");
        for (int i = 0; i < lst->capacity; i++)
        {
            printf("%3d ", lst->node[i].prev);
        }
    }

    if (Error == CYCLED_LIST)
        printf("Warning! There is an cycle in the next arr!\n");
    printf("\n\n");

    return OK;
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
    for (jumpCounter = 0; (runThrough != lst->tail) && (jumpCounter < lst->capacity + 1); jumpCounter++)
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
        return TAIL_NOT_PROPER;

    return OK;
}
#include "list.hpp"

int ListCtor (List* lst, size_t capacity)
{
    assert(lst);
    assert(capacity);

    lst->data = (int*)calloc(capacity, sizeof(int));
    if (lst->data == NULL)
        return CANT_ALLOC_MEM;

    lst->next = (int*)calloc(capacity, sizeof(int));
    if (lst->data == NULL)
        return CANT_ALLOC_MEM;

    lst->head = lst->tail = 0;
    memset(lst->next + 1, FREE_SPOT, (capacity - 1) * sizeof(int));
    lst->capacity = capacity;

    return OK;
}

int ListDtor (List* lst)
{
    assert(lst);

    if (lst->data != NULL)
    {
        free(lst->data);
        lst->data = NULL;
    }
    if (lst->next != NULL)
    {
        free(lst->next);
        lst->next = NULL;
    }

    lst->head = lst->tail = 0;
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
        physicalPlace = lst->next[physicalPlace];
    }

    return physicalPlace;
}

int FindFreeSpot(List* lst)
{
    int freeIndex = 0;
    for (freeIndex = 0; (freeIndex < lst->capacity) && (lst->next[freeIndex] != FREE_SPOT); freeIndex++)
    ;

    if (freeIndex == lst->capacity)
        return 0;

    return freeIndex;
}

int FindPrevElem(List* lst, int index)
{
    int prevIndex = lst->head;

    while ((prevIndex != lst->tail) && (lst->next[prevIndex] != index))
        prevIndex = lst->next[prevIndex];

    if (prevIndex == lst->tail)
        return 0;

    return prevIndex;
}

int ResizeList (List* lst)
{
    int* tmpdata = (int*)realloc(lst->data, 2 * lst->capacity * sizeof(int));
    int* tmpnext = (int*)realloc(lst->next, 2 * lst->capacity * sizeof(int));
    if ((tmpdata == NULL) || (tmpnext == NULL))
        return CANT_RESIZE_LIST;

    lst->data = tmpdata;
    lst->next = tmpnext;

    memset(lst->data + lst->capacity, 0, lst->capacity * sizeof(int));
    memset(lst->next + lst->capacity, FREE_SPOT, lst->capacity * sizeof(int));

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

    if (lst->next[index] == FREE_SPOT && (lst->head != 0))
        return 0;
    //printf("adding after %d val %d\n", index, val);

    int freeIndex = FindFreeSpot(lst);

    printf("free spot - %d\n", freeIndex);
    if (freeIndex == 0)
    {
        int Error = ResizeList(lst);
        if (Error)
            return Error;
        freeIndex = FindFreeSpot(lst);
    }

    lst->data[freeIndex] = val;

    if (lst->head == 0)
    {
        lst->head = freeIndex;

        if (lst->tail != 0)
            return CORRUPTED_LIST;

        lst->next[freeIndex] = 0;
        lst->tail = freeIndex;

        return freeIndex;
    }

    lst->next[freeIndex] = lst->next[index];
    lst->next[index] = freeIndex;

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

    if (lst->next[index] == FREE_SPOT)
        return 0;

    if (index == lst->head)
    {
        lst->head = lst->next[index];
        lst->next[index] = FREE_SPOT;

        return OK;
    }

    int prev = FindPrevElem(lst, index); // remove

    if (index == lst->tail)
    {
        lst->tail = prev;
        lst->next[index] = FREE_SPOT;
        lst->next[prev] = 0;
        return OK;
    }

    lst->next[prev] = lst->next[index];
    lst->next[index] = FREE_SPOT;

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
        printf("Warning! The head an tail are corrupted!");
    if (Error == TAIL_NOT_PROPER)
        printf("Warning! There is an element after the one called the tail!");

    printf("\nhead = %d, capacity - %zu, tail - %d\n", lst->head, lst->capacity, lst->tail);

    if (Error == NULL_DATA_PTR)
        printf("Warning! Null data ptr!!\n");
    else
    {
        printf("data arr -->");
        for (int i = 0; i < lst->capacity; i++)
        {
            printf("%3d ", lst->data[i]);
        }
    }

    if (Error == NULL_NEXT_PTR)
        printf("Warning! Null next ptr!!\n");
    else
    {
        printf("\nnext arr -->");
        for (int i = 0; i < lst->capacity; i++)
        {
            printf("%3d ", lst->next[i]);
        }
    }
    if (Error == CYCLED_LIST)
        printf("Warning! There is an cycle in the next arr!");
    printf("\n\n");

    return OK;
}

int ListVerify (List* lst)
{
    if (lst == NULL)
        return NULL_LST_PTR;

    if (lst->data == NULL)
        return NULL_DATA_PTR;

    if (lst->next == NULL)
        return NULL_NEXT_PTR;

    if ( ((lst->head == 0) && (lst->tail != 0)) || ((lst->head != 0) && (lst->tail == 0)) )
        return INIT_ERROR;

    if (lst->capacity == 0)
        return NULL_CAP;

    if ((lst->head == 0) && (lst->tail == 0))
        return OK;

    int runThrough = lst->head;
    int jumpCounter = 0;

    for (jumpCounter = 0; (runThrough != lst->tail) && (jumpCounter < lst->capacity + 1); jumpCounter++)
    {
        runThrough = lst->next[runThrough];
    }
    //printf("\n\nin vrctr: runthrough = %d, cyclind = %d, lst->tail = %d\n\n", runThrough, jumpCounter, lst->tail);

    if (jumpCounter == lst->capacity + 1)
        return CYCLED_LIST;

    if (lst->next[runThrough] != 0)
        return TAIL_NOT_PROPER;

    return OK;
}
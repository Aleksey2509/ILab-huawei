#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

const int FREE_SPOT = -1;

struct ListNode
{
    int value = 0;
    int next = 0;
    int prev = 0;
};

struct List
{
    int head = 0;
    int tail = 0;
    ListNode* node;
    size_t capacity = 0;
};

enum Error
{
    NULL_LST_PTR = -1000,
    ADD_AFTER_FREE,
    REMOVE_FREE,
    NULL_NODE_PTR,
    INIT_ERROR,
    NULL_CAP,
    TAIL_NOT_PROPER,
    CORRUPTED_LIST,
    CYCLED_LIST,
    CANT_ALLOC_MEM,
    CANT_RESIZE_LIST,
    OK = 0
};

int ListCtor (List* lst, size_t capacity);
int ListDtor (List* lst);

int FindFreeSpot(List* lst);
int FindPrevElem(List* lst, int index);
int LogicalToPhysical (List* lst, int logicalPlace);

int ResizeList (List* lst);

int ListAdd (List* lst, int index, int val);
int ListRemove (List* lst, int index);

int ListDump(List* lst);
int ListVerify (List* lst);
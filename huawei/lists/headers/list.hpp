#ifndef LIST_HPP

#define LIST_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

#define VRFCTR_CHECK 1

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
    int free = 0;
    int sorted = 0;
    size_t capacity = 0;
    ListNode* node;
};

enum CriticalError
{
    NULL_LST_PTR = -1000,
    NULL_NODE_PTR,
    INIT_ERROR,
    NULL_CAP,
    HEAD_NOT_PROPER,
    UNLINKED_LIST,
    TAIL_NOT_PROPER,
    CYCLED_LIST,
    OK = 0
};

enum UseError
{
    ADD_AFTER_FREE = -500,
    REMOVE_FREE,
    CANT_ALLOC_MEM,
    CANT_RESIZE_LIST,
};

static const char DotCmd[] = {"dot -Tps graph2.gv -o graph2.ps"};

int ListCtor (List* lst, size_t capacity);
int ListDtor (List* lst);

int ListLogicalToPhysical (List* lst, int logicalPlace);

int ListResize (List* lst);

int ListAddBefore (List* lst, int index, int val);
int ListAddAfter (List* lst, int index, int val);
int ListRemove (List* lst, int index);

int ListDump(List* lst);
int ListGraphicDump(List* lst);
int ListVerify (List* lst);

int ListNodeComparator (const void* arg1, const void* arg2);
int ListSort(List* lst);

#endif
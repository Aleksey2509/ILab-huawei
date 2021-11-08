#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include "list.hpp"

int main()
{
    List testSubj {};

    ListCtor(&testSubj, 5);

    ListDump(&testSubj);

    int place10 = ListAdd(&testSubj, 1, 10);
    int Error = ListDump(&testSubj);
    if (Error)
        printf("Error - %d\n", Error);

    int place20 = ListAdd(&testSubj, place10, 20);
    Error = ListDump(&testSubj);
    if (Error)
        printf("Error - %d\n", Error);

    int place30 = ListAdd(&testSubj, place20, 30);
    Error = ListDump(&testSubj);
    if (Error)
        printf("Error - %d\n", Error);

    int place40 = ListAdd(&testSubj, place10, 40);
    Error = ListDump(&testSubj);
    if (Error)
        printf("Error - %d\n", Error);


    int place50 = ListAdd(&testSubj, place20, 50);
    Error = ListDump(&testSubj);
    if (Error)
        printf("Error - %d\n", Error);
    // printf("first - %d, second - %d, third - %d\n", LogicalToPhysical(&testSubj, 0), LogicalToPhysical(&testSubj, 1), LogicalToPhysical(&testSubj, 2));

    printf("Starting removing staff");

    ListRemove(&testSubj, place10);
    Error = ListDump(&testSubj);
    if (Error)
        printf("Error - %d\n", Error);

    ListRemove(&testSubj, place40);
    Error = ListDump(&testSubj);
    if (Error)
        printf("Error - %d\n", Error);

    ListRemove(&testSubj, place30);
    Error = ListDump(&testSubj);
    if (Error)
        printf("Error - %d\n", Error);

    ListDtor(&testSubj);
}
#include "compare.h"

//------------------------------------------------------------------------------------------------------------------------------

int straightComparator (const void* arg1, const void* arg2)
{
    const char* line1 = *(const char** )arg1;
    const char* line2 = *(const char** )arg2;

    //printf("Comparing %s and %s\n", line1, line2);
    int len = min(strlen(line1), strlen(line2));

    for (int i = 0; i < len + 1; i++)
    {
        //printf("\nline1 - %c, line2 - %c\n", line1[i], line2[i]);
        int compRes = charComparator(line1[i], line2[i]);
        if (compRes)
            return compRes;
    }
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int charComparator (char a, char b)
{
    a = tolower(a);
    b = tolower(b);

    if (a > b)
        return 1;
        else if (a < b)
                return -1;

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int memswap (char** line1Ptr, char** line2Ptr)
{
    char* tmp = *line1Ptr;
    *line1Ptr = *line2Ptr;
    *line2Ptr = tmp;

    return 0;
}


//------------------------------------------------------------------------------------------------------------------------------


int min (int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}

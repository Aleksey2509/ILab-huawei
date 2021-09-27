#include "compare.h"

//------------------------------------------------------------------------------------------------------------------------------

int straightComparator (const void* arg1, const void* arg2)
{
    const line* lineStruct1 = (const line* )arg1;
    const line* lineStrcut2 = (const line* )arg2;

    const char* line1 = lineStruct1->line;
    const char* line2 = lineStrcut2->line;

    // printf("Comparing %s and %s\n", line1.line, line2.line);
    int len = min(strlen(line1), strlen(line2));

    for (int i = 0, j = 0; (i < len + 1) && (j < len + 1); )
    {
        while ( !isalpha(line1[i]) && line1[i] != '\0' )
        {
            i++;
            continue;
        }
        while ( !isalpha(line2[j]) && line2[j] != '\0' )
        {
            j++;
            continue;
        }
        //printf("\nline1 - %c, line2 - %c\n", line1[i], line2[i]);
        int compRes = charComparator(line1[i], line2[j]);
        if (compRes != 0)
            return compRes;
        i++;
        j++;
        
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

int memswap (line* line1Ptr, line* line2Ptr)
{
    line tmp = *line1Ptr;
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

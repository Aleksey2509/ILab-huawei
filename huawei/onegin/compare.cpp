#include "compare.h"

//------------------------------------------------------------------------------------------------------------------------------

int StraightComparator (const void* arg1, const void* arg2)
{
    const line* lineStruct1 = (const line* )arg1;
    const line* lineStruct2 = (const line* )arg2;

    const char* line1 = lineStruct1->line;
    const char* line2 = lineStruct2->line;

    const int len1 = lineStruct1->lineLen;
    const int len2 = lineStruct2->lineLen;

    int charlen1 = len1;
    int charlen2 = len2;

    //printf("Comparing %s and %s\n", line1, line2);

    for (int i = 0, j = 0; (i < len1 + 1) && (j < len2 + 1); )
    {
        while ( !(isalpha(line1[i])) && (line1[i + 1] != '\0') && (line1[i + 1] != '\n') )
        {
            i++;
            charlen1--;
        }
        while ( !(isalpha(line2[j])) && (line2[j + 1] != '\0') && (line1[i + 1] != '\n') )
        {
            j++;
            charlen2--;
        }

        //printf("charlen of %s = %d, charlen of %s = %d\n", line1, charlen1, line2, charlen2);

        if ( (line1[i + 1] == 0) || (line2[j + 1] == 0) )
        {
            return (Min(charlen1, charlen2));
        }

        //printf("\nline1 - %c, line2 - %c\n", line1[i], line2[i]);
        int compRes = CharComparator(line1[i], line2[j]);
        if (compRes != 0)
            return compRes;
        i++;
        j++;
        
    }
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int ReverseComparator (const void* arg1, const void* arg2)
{
    const line* lineStruct1 = (const line* )arg1;
    const line* lineStruct2 = (const line* )arg2;

    const char* line1 = lineStruct1->line;
    const char* line2 = lineStruct2->line;

    const int len1 = lineStruct1->lineLen;
    const int len2 = lineStruct2->lineLen;

    int charlen1 = len1;
    int charlen2 = len2;

    //printf("Comparing %s and %s\n", line1, line2);

    for (int i = len1, j = len2; (i > 0) && (j > 0); )
    {
        while ( !(isalpha(line1[i])) && (i > 0))
        {
            i--;
            charlen1--;
        }
        while ( !(isalpha(line2[j])) && (i > 0) )
        {
            j--;
            charlen2--;
        }

        //printf("charlen of %s = %d, charlen of %s = %d\n", line1, charlen1, line2, charlen2);

        if ( (i == 0) || (j == 0) )
        {
            //printf("charlen1 = %d, charlen2 = %D\n", charlen1, charlen2);
            return (Min(charlen1, charlen2));
        }

        //printf("\nline1 - %c, line2 - %c\n", line1[i], line2[i]);
        int compRes = CharComparator(line1[i], line2[j]);
        if (compRes != 0)
            return compRes;
        i--;
        j--;
        
    }
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int CharComparator (char a, char b)
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

int MemSwap (line* line1Ptr, line* line2Ptr)
{
    line tmp = *line1Ptr;
    *line1Ptr = *line2Ptr;
    *line2Ptr = tmp;

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int VoidMemSwap (void* arg1, void* arg2, unsigned int DataSize)
{
    void* tmp = calloc(1, DataSize);
    tmp = memcpy(tmp, arg1, DataSize);
    arg1 = memcpy(arg1, arg2, DataSize);
    arg2 = memcpy(arg2, tmp, DataSize);
    free (tmp);

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------


int Min (int a, int b)
{
    if (a < b)
        return -1;
    else if (a > b)
            return 1;
    
    return 0;
}

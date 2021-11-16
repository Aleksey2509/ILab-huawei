#include "../Headers/struct.hpp"
#include "../Headers/Compare.hpp"

//------------------------------------------------------------------------------------------------------------------------------

int StraightComparator (const void* arg1, const void* arg2)
{
    const line_t* lineStruct1 = (const line_t* )arg1;
    const line_t* lineStruct2 = (const line_t* )arg2;

    const char* line1 = lineStruct1->line;
    const char* line2 = lineStruct2->line;

    const int len1 = lineStruct1->lineLen;
    const int len2 = lineStruct2->lineLen;

    int charlen1 = len1;
    int charlen2 = len2;

    //printf("Comparing %.*s and %.*s\n", len1, line1, len2, line2);

    int i = 0;
    int j = 0;
    while (true)
    {
        while ( !(isalpha(line1[i])) && (line1[i] != '\0') && (line1[i] != '\n') )
        {
            i++;
            charlen1--;
        }
        while ( !(isalpha(line2[j])) && (line2[j] != '\0') && (line1[j] != '\n') )
        {
            j++;
            charlen2--;
        }

        //printf("charlen of %s = %d, charlen of %s = %d\n", line1, charlen1, line2, charlen2);

        if ( (line1[i] == '\0') || (line2[j] == '\0') )
        {
            return (charlen1 - charlen2);
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
    const line_t* lineStruct1 = (const line_t* )arg1;
    const line_t* lineStruct2 = (const line_t* )arg2;

    const char* line1 = lineStruct1->line;
    const char* line2 = lineStruct2->line;

    const int len1 = lineStruct1->lineLen;
    const int len2 = lineStruct2->lineLen;

    int charlen1 = len1;
    int charlen2 = len2;

    //printf("Comparing %s and %s\n", line1, line2);
    int i = 0;
    int j = 0;
    while(true)
    {
        while ( !(isalpha(line1[i])) && (i >= 0))
        {
            i--;
            charlen1--;
        }
        while ( !(isalpha(line2[j])) && (j >= 0) )
        {
            j--;
            charlen2--;
        }

        //printf("charlen of %s = %d, charlen of %s = %d\n", line1, charlen1, line2, charlen2);

        if ( (i < 0) || (j < 0) )
        {
            //printf("charlen1 = %d, charlen2 = %D\n", charlen1, charlen2);
            return (charlen1 - charlen2);
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

    return a - b;
}

//------------------------------------------------------------------------------------------------------------------------------

void VoidMemSwap (void* arg1, void* arg2, unsigned int DataSize)
{
    char tmp[BUFSIZ] = { 0 };
    memcpy(tmp, arg1, DataSize);
    memcpy(arg1, arg2, DataSize);
    memcpy(arg2, tmp, DataSize);
}

//------------------------------------------------------------------------------------------------------------------------------
void Myqsort (void* base, size_t nElem, size_t elemSize, int (*comparator)(const void *, const void *))
{
    if (nElem < 2)
    return;

    char* baseptr = (char*) base;

    int i = 0;
    int j = nElem - 1;
    int pivotPos = (i + j) / 2;

    char pivot[BUFSIZ] = { 0 };
    memcpy(pivot, baseptr + pivotPos * elemSize, elemSize);

    do
    {   
        //printf("i = %d, j = %d\n", i, j);
        while( (i < nElem - 1) && (comparator( baseptr + i * elemSize, pivot ) < 0) )
            i++;
        while( (j > 0) && (comparator( baseptr + j * elemSize, pivot ) > 0) )
            j--;
        if (i <= j)
        {
            //printf("SWAAAPING %.*s, %.*s\n", ((line*)base)[i].lineLen, ((line*)base)[i].line, ((line*)base)[j].lineLen, ((line*)base)[j].line);
            VoidMemSwap (baseptr + i * elemSize, baseptr + j * elemSize, elemSize);
            j--;
            i++;
        }
        //printf("at the end of cycle: i = %d, j = %d\n\n", i, j);
    }while(i <= j);

    if (i < nElem)
    	Myqsort (baseptr + i * elemSize, nElem - i, elemSize, comparator);
    if (j > 0)
        Myqsort (baseptr, j + 1, elemSize, comparator);

}
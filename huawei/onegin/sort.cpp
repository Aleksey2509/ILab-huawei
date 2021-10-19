#include "struct.hpp"
#include "Compare.hpp"

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
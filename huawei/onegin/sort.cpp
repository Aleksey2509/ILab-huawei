void Myqsort (void* base, size_t nElem, size_t elemSize, int (*comparator)(const void *, const void *))
{
    int i = 0;
    int j = nElem - 1;
    int pivot = 0;

    do
    {   
        //printf("i = %d, j = %d\n", i, j);
        while( (i < nElem - 1) && (comparator( (char *)base + i * elemSize, (char *)base + pivot * elemSize ) < 0) )
            i++;
        while( (j > 0) && (comparator( (char *)base + pivot * elemSize, (char *)base + j * elemSize ) < 0 ) )
            j--;
        if (i <= j)
        {
            //printf("SWAAAPING %.*s, %.*s\n", ((line*)base)[i].lineLen, ((line*)base)[i].line, ((line*)base)[j].lineLen, ((line*)base)[j].line);
            VoidMemSwap ((char*)base + i * elemSize, (char *)base + j * elemSize, elemSize);
            i++;
            j--;
        }
        //printf("at the end of cycle: i = %d, j = %d\n\n", i, j);
    }
    while (i <= j);

    if (i < nElem)
    	Myqsort ((char *)base + i * elemSize, nElem - i, elemSize, comparator);
    if (j > 0)
        Myqsort ((char *)base, j + 1, elemSize, comparator);

}

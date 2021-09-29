#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include "struct.h"
#include "compare.cpp"
#include "ReadWrite.cpp"

int main(int argc, char* argv[])
{
    TEXT text;

    if (argc < 3)
    {
        printf("You did not specify name of the input or the output file\n");
        return(1);
    }

    int Error = TEXT_ReadFromFile(&text, argv[1]);

    if (Error)
        PrintError(Error);

    qsort(text.lines, text.nLines, sizeof(line), ReverseComparator);

    Error = PrintTextToFile(argv[2], &text);
    if (Error)
        PrintError(Error);

    Destroy(&text);
    
    return 0;
}
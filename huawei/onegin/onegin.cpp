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
    TEXT text = {0, 0, 0, 0};

    if (argc < 3)
    {
        printf("You did not specify name of the input or the output file\n");
        exit(1);
    }


    text.lineArray = (char**) calloc(MAX_LINES, sizeof(char*));

    int Error = ReadFromFile(&text, argv[1]);

    //printf("Starting printing gotten string array: nLines = %d\n", text.nLines);
    //printText(&text);

    qsort(text.lineArray, text.nLines, sizeof(char *), straightComparator);

    //printf("Printing gotten string array: nLines = %d\n", text.nLines);
    //printText(&text);

    FILE* output = fopen(argv[2], "w");

    PrintToFile(output, &text);

    Destroy(&text);
}
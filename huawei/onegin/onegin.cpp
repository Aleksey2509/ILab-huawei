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
{ // remove all from main!!!

    TEXT text;

    if (argc < 3)
    {
        printf("You did not specify name of the input or the output file\n");
        return(1);
    }
    
    text.lines = (line *) calloc(MAX_LINES, sizeof(line)); // close in same file???
    // nullptr check!!!


    int Error = ReadFromFile(&text, argv[1]);

    //printf("Starting printing gotten string array: nLines = %d\n", text.nLines);
    //printText(&text);

    
    qsort(text.lines, text.nLines, sizeof(line), reverseComparator);

    //printf("Printing gotten string array: nLines = %d\n", text.nLines);
    //printText(&text);

    FILE* output = fopen(argv[2], "w"); // close in same function!!!

    PrintToFile(output, &text);

    Destroy(&text);
}
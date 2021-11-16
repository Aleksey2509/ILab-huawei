#include "../Headers/struct.hpp"
#include "../Headers/ReadWrite.hpp"
#include "../Headers/Compare.hpp"

int testoutput(char* FileName)
{
    TEXT text {};

    int Error = TEXT_ReadFromFile(&text, FileName);
    if (Error)
    {
        printf("%s\n", TEXT_GetError(Error));
        TEXT_Destroy(&text);
        return Error;
    }

    int i = 0;
    int* WrongOrder = (int *)calloc(text.nLines, sizeof(int));
    int WrongLines = 0;

    printf("Checking with straightcomparator\n\n");

    for (i = 0; i < text.nLines - 1; i++)
    {
        if (StraightComparator(text.lines + i, text.lines + i + 1) > 0)
        {
            WrongOrder[WrongLines] = i;
            WrongLines++;
        }
    }

    for (i = 0; i < WrongLines; i++)
    {
        printf("Wrong order: lines %3d and %3d\n", WrongOrder[i], WrongOrder[i] + 1);
    }
    if (WrongLines == 0)
    {
        printf("Everything OK\n");
    }

    return 0;
}
#include "../Headers/struct.hpp"
#include "../Headers/ReadWrite.hpp"
#include "../Headers/Compare.hpp"


int Onegin (const char* FileFrom, const char* FileTo)
{
    TEXT text {};

    int Error = TEXT_ReadFromFile(&text, FileFrom);
    if (Error)
    {
        printf("%s\n", TEXT_GetError(Error));
        TEXT_Destroy(&text);
        return Error;
    }

    //qsort(text.lines, text.nLines, sizeof(line_t), StraightComparator);
    Myqsort(text.lines, text.nLines, sizeof(line_t), StraightComparator);

    Error = PrintTextToFile(FileTo, &text);
    if (Error)
    {
        printf("%s\n", TEXT_GetError(Error));
        TEXT_Destroy(&text);
        return Error;
    }

    TEXT_Destroy(&text);

    return 0;
}
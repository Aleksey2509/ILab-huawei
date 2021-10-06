int Onegin (int FileCount, char* FileArr[]) // in name & out name? + const for ptrs
{
    TEXT text {};

    if (FileCount < 2)
    {
        printf("You did not specify name of the input or the output file\n");
        return 1;
    }

    int Error = TEXT_ReadFromFile(&text, FileArr[0]);
    if (Error)
    {
        PrintError(Error);
        TEXT_Destroy(&text);
        return Error;
    }

    //qsort(text.lines, text.nLines, sizeof(line_t), ReverseComparator);
    Myqsort(text.lines, text.nLines, sizeof(line_t), ReverseComparator);

    Error = PrintTextToFile(FileArr[1], &text);
    if (Error)
    {
        PrintError(Error);
        TEXT_Destroy(&text);
        return Error;
    }

    TEXT_Destroy(&text);

    return 0;
}
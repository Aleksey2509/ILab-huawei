int Onegin (int FileCount, char* FileArr[])
{
    TEXT text = {0};

    if (FileCount < 2)
    {
        printf("You did not specify name of the input or the output file\n");
        return(1);
    }

    int Error = TEXT_ReadFromFile(&text, FileArr[0]);
    if (Error)
    {
        PrintError(Error);
        Destroy(&text);
        return Error;
    }

    Myqsort(text.lines, text.nLines, sizeof(line), StraightComparator);

    Error = PrintTextToFile(FileArr[1], &text);
    if (Error)
    {
        PrintError(Error);
        Destroy(&text);
        return Error;
    }

    Destroy(&text);

    return 0;
}
#include "ReadWrite.h"

//------------------------------------------------------------------------------------------------------------------------------

int TEXT_ReadFromFile(TEXT* text, const char* FileName)
{
    unsigned int AllocedMemory = MAX_INPUT_LINES * sizeof(line);

    text->lines = (line *) calloc(MAX_INPUT_LINES, sizeof(line));
    while ( !text->lines )
    {
        text->lines = ReallocLineArr(text->lines, &AllocedMemory, REDUCE);
    }

    int Error = CreateBuffer(FileName, text);
    if (Error)
    {
        return Error;
    }

    for (int i = 0; i < text->buffSize ;i++)
    {
        size_t len = mystrlen(text->buffer + i);
            if (len == 0)
                continue;

        text->lines[text->nLines].line = (text->buffer + i);
        text->lines[text->nLines].lineLen = len;

        i += text->lines[text->nLines].lineLen;
        text->nLines++;

        if (text->nLines > (AllocedMemory / sizeof(line)))
            text->lines = ReallocLineArr(text->lines, &AllocedMemory, ENLARGE);
        
    }

    return 0;

}

//------------------------------------------------------------------------------------------------------------------------------

int Fgets_ReadFromFile( char* Index[], const char* FileName)
{
    FILE* input = fopen(FileName, "r");

    int nLines = 0;

    char buffer[MAX_LINE_LENGTH] = { 0 };

    for (nLines = 0; nLines < MAX_INPUT_LINES; nLines++)
    {

        printf("Here, i = %d\n", nLines);

        if (fgets(buffer, MAX_LINE_LENGTH, input) == 0)
            break;

        Index[nLines] = strdup (buffer);

        printf("Here, after getting i = %d, the line i got: %s\n", nLines, Index[nLines]);

        
    }

    fclose(input);

    return nLines;

}

//------------------------------------------------------------------------------------------------------------------------------

int CreateBuffer (const char* FileName, TEXT* text)
{
    assert (FileName);
    assert(text);

    struct stat FileInfo;
    int stat_flag = stat(FileName, &FileInfo);
    if (stat_flag == -1)
    {
        return (CANT_GET_FL_INFO);
    }

    long long size = FileInfo.st_size;
    text->buffer = (char*)calloc(size, sizeof(char));

    if ( text->buffer == NULL)
        return CANT_ALLOC_BUF;

    FILE* input = fopen(FileName, "r");
    if ( input == NULL)
        return NULL_FL;

    text->buffSize = fread(text->buffer, sizeof(char), FileInfo.st_size, input);
    fclose(input);

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int PrintTextStdout(TEXT* text)
{
    for (int i = 0; i < text->nLines; i++)
    {
        PrintLineToFile(text->lines + i, stdout);
    }
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int PrintTextToFile (char* path, TEXT* text)
{

	assert(path != NULL);
	assert(text != NULL);

    FILE* file = fopen(path, "w");
    if ( file == NULL)
        return NULL_FL;

	for(int i = 0; i < text->nLines; i++)
	{
		PrintLineToFile(text->lines + i, file);
	}
    fclose(file);

    return 0;

}
//------------------------------------------------------------------------------------------------------------------------------

int PrintLineToFile(const line* str, FILE* stream)
{
    fwrite(str->line, sizeof(char), str->lineLen, stream);
    fwrite("\n", sizeof(char), 1, stream);

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int PrintError (int Error)
{
    switch (Error)
        {
            case CANT_ALLOC_BUF: printf("Your file is too big\n");
                                break;
            case NULL_FL: printf("Could not open file\n");
                          break;
            case CANT_GET_FL_INFO: printf("Could not get file info\n");
                                   break;
        }
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

size_t mystrlen(const char* string)
{
    size_t len = 0;
    for (len = 0; (*string != '\n') && (*string != '\0') ; len++)
        string++;
    return (len);
}

//------------------------------------------------------------------------------------------------------------------------------

line* ReallocLineArr (line* lines, unsigned int* AllocedMemory, int mode)
{
    assert((mode == REDUCE) || (mode == ENLARGE));
    
    if (mode == REDUCE)
        *AllocedMemory /= MULT_CONST;
    else
        *AllocedMemory *= MULT_CONST;

    lines = (line*)realloc(lines, *AllocedMemory);

    return lines;
}

//------------------------------------------------------------------------------------------------------------------------------

int Destroy (TEXT* text)
{
    if (text->buffer != NULL)
        free(text->buffer);
    if (text->lines != NULL)
        free(text->lines);

    return 0;
}
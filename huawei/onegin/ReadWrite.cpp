#include "ReadWrite.h"

//------------------------------------------------------------------------------------------------------------------------------

int TEXT_ReadFromFile(TEXT* text, const char* FileName)
{
    int Error = TEXT_CreateBuffer(FileName, text);
    if (Error)
    {
        return Error;
    }

    Error = Parcer(text);

    return Error; // if everything is alright after parcer, the Error will be set to state OK = 0

}

int Parcer (TEXT* text)
{
    unsigned int AllocedMemory = MAX_INPUT_LINES * sizeof(line_t);

    text->lines = (line_t *) calloc(MAX_INPUT_LINES, sizeof(line_t));
    if ( !text->lines )
    {
        return NOT_ENGH_MEM;
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

        if (text->nLines > (AllocedMemory / sizeof(line_t)))
        {
            text->lines = ReallocLineArr(text->lines, &AllocedMemory);
            if (!text->lines)
                return NOT_ENGH_MEM;
        }
        
    }
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

int TEXT_CreateBuffer (const char* FileName, TEXT* text)
{
    if (!FileName)
        return NULL_FL;
    if (!text)
        return NULL_TEXT_PTR;

    struct stat FileInfo;
    int stat_flag = stat(FileName, &FileInfo);
    if (stat_flag == -1)
    {
        return CANT_GET_FL_INFO;
    }

    off_t size = FileInfo.st_size;
    text->buffer = (char*)calloc(size, sizeof(char));

    if ( text->buffer == NULL)
        return CANT_ALLOC_BUF;

    FILE* input = fopen(FileName, "r");
    if ( input == NULL)
    {
        free(text->buffer);
        return NULL_FL;
    }

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

int PrintLineToFile(const line_t* str, FILE* stream)
{
    fwrite(str->line, sizeof(char), str->lineLen, stream);
    fwrite("\n", sizeof(char), 1, stream);

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int PrintError (int Error) // strerror?
{
    switch (Error)
        {
            case CANT_ALLOC_BUF: printf("Your file is too big\n");
                                break;
            case NULL_FL: printf("Bad file pointer\n");
                          break;
            case NULL_TEXT_PTR: printf("Null text ptr\n");
                          break;
            case NOT_ENGH_MEM: printf("Not enough memory for line memory allocation\n");
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
    for (len = 0; (string[len] != '\n') && (string[len] != '\0') ; len++)
    ;
    return len;
}

//------------------------------------------------------------------------------------------------------------------------------

line_t* ReallocLineArr (line_t* lines, unsigned int* AllocedMemory)
{
    *AllocedMemory *= MULT_CONST;
    lines = (line_t*)realloc(lines, *AllocedMemory);

    return lines;
}

//------------------------------------------------------------------------------------------------------------------------------

int TEXT_Destroy (TEXT* text)
{
    if (text->buffer != NULL)
        free(text->buffer);
    if (text->lines != NULL)
        free(text->lines);

    text->buffer = NULL;
    text->lines = NULL;

    return 0;
}
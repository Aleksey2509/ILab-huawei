#include "../Headers/struct.hpp"
#include "../Headers/ReadWrite.hpp"

//------------------------------------------------------------------------------------------------------------------------------

int TEXT_ReadFromFile(TEXT* text, const char* FileName)
{
    int Error = TEXT_CreateBuffer(FileName, text);
    if (Error)
    {
        return Error;
    }

    Error = TEXT_Parcer(text);

    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------

int TEXT_Parcer (TEXT* text)
{
    size_t allocatedLines = MAX_INPUT_LINES;

    text->lines = (line_t *) calloc(allocatedLines, sizeof(line_t));
    if ( !text->lines )
        return NOT_ENGH_MEM;

    for (int i = 0; i < text->buffSize ;i++)
    {
        size_t len = mystrlen(text->buffer + i);
            if (len == 0)
                continue;

        text->lines[text->nLines].line = (text->buffer + i);
        text->lines[text->nLines].lineLen = len;

        i += len; // len
        text->nLines++;

        if (text->nLines > (allocatedLines))
        {
            line_t* tmp = (line_t*)realloc(text->lines, 2 * allocatedLines * sizeof(line_t));
            if (!tmp)
                return NOT_ENGH_MEM;
            text->lines = tmp;
            
            allocatedLines *= 2;
        }
        
    }

    return OK;
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
        fwrite(text->lines->line, sizeof(char), text->lines->lineLen, stdout);
        fputc('\n', stdout);
    }
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int PrintTextToFile (const char* path, const TEXT* text)
{

	if (!text)
        return NULL_TEXT_PTR;
    if (!text->lines)
        return NULL_TEXT_PTR;
    if (!path)
        return NULL_FL;

    FILE* file = fopen(path, "w");
    if ( file == NULL)
        return NULL_FL;

	for(int i = 0; i < text->nLines; i++)
	{
        fwrite(text->lines->line, sizeof(char), text->lines->lineLen, file);
        fputc('\n', file);
    }
    fclose(file);

    return 0;

}

//------------------------------------------------------------------------------------------------------------------------------

const char* TEXT_GetError (int Error)
{
    switch (Error)
        {
            case OK:               return "No error\n";

            case CANT_ALLOC_BUF:   return "Your file is too big\n";

            case NULL_FL:          return "Bad file pointer\n";

            case NULL_TEXT_PTR:    return "Null text ptr\n";

            case NOT_ENGH_MEM:     return "Not enough memory for line memory allocation\n";

            case CANT_GET_FL_INFO: return "Could not get file info\n";

            default:               return "Unknown error\n";
        }

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


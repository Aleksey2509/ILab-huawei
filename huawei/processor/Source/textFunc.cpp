#include "../Headers/const.hpp"
#include "../Headers/textFunc.hpp"

//------------------------------------------------------------------------------------------------------------------------------

int TextReadFromFile(Text* text, const char* FileName)
{
    int Error = TextCreateBuffer(FileName, text);
    if (Error)
    {
        return Error;
    }

    Error = TextParcer(text);

    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------

int TextParcer (Text* text)
{
    size_t allocatedLines = MAX_INPUT_LINES;

    text->lines = (line_t *) calloc(allocatedLines, sizeof(line_t));
    if ( !text->lines )
        return NOT_ENGH_MEM;

    for (int i = 0; i < text->buffSize; i++)
    {
        size_t len = strlen(text->buffer + i);
            if (len == 0)
                continue;

        text->lines[text->nLines].line = (text->buffer + i);
        text->lines[text->nLines].lineLen = len;

        i += len;
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

int TextCreateBuffer (const char* FileName, Text* text)
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

    char* newlinePtr = text->buffer;
    int i = 0;
    while ( (newlinePtr = strchr(newlinePtr, '\n')) != NULL )
    {
        //printf("i - %d, %s\n", ++i, newlinePtr);
        *newlinePtr = '\0';
        newlinePtr++;
    }

    fclose(input);

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int PrintTextStdout(Text* text)
{
    fprintf(stdout, "nLines = %d\n", text->nLines);
    for (int i = 0; i < text->nLines; i++)
    {
        fputs(text->lines->line + i, stdout);
        fputc('\n', stdout);
    }
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int PrintTextToFile (const char* path, const Text* text)
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
        fputs(text->lines->line, stdout);
    }
    fclose(file);

    return 0;

}

//------------------------------------------------------------------------------------------------------------------------------

const char* TextGetError (int Error)
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

int TextDestroy (Text* text)
{
    if (text->buffer != NULL)
        free(text->buffer);
    if (text->lines != NULL)
        free(text->lines);

    text->buffer = NULL;
    text->lines = NULL;

    return 0;
}


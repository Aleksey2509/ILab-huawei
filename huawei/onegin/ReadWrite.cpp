#include "ReadWrite.h"

/*static*/ const int MAX_LINES = 10000; // IN FILE???

const int MAX_LENGTH = 1000; // MAX LINE LENGTH???

//------------------------------------------------------------------------------------------------------------------------------

int ReadFromFile(TEXT* text, const char* FileName)
{

    int Error = createBuffer(FileName, text);

    unsigned int ReallocTimes = 0; // remove

    // split

    //printf("\n%llD\n", text->buffSize);

    for (int i = 0; i < text->buffSize; i++) // remove copying
    {

        if(text->buffer[i] == '\n')
        {
            text->buffer[i] = '\0';
        }
    }

    for (int i = 0; i < text->buffSize ;i++)
    {
        if( (text->buffer[i] == 0) && ( (text->buffer[i - 1] == 0) || (i == 0) ) ) //skipping all consecutive \0
            continue;
        
        text->lines[text->nLines].line = strdup(text->buffer + i); // remove copying
        
        //printf("\ngot %s, ", text->lines[text->nLines].line);
        text->lines[text->nLines].lineLen = strlen(text->lines[text->nLines].line);
        i += strlen(text->lines[text->nLines].line);
        text->nLines++;
        //printf("len = %d Now there are %d lines\n", text->lines[text->nLines].lineLen, text->nLines);
        if (text->nLines > (ReallocTimes + 1) * MAX_LINES)
            text->lines = (line*)realloc (text->lines, (++ReallocTimes) * MAX_LINES * sizeof(line));
        
        
    }

    //printf("\n\nthere are %d lines\n", text->nLines);
    if (text->nLines < MAX_LINES) // remove
        text->lines = (line*)realloc (text->lines, text->nLines * sizeof(line));

    // for (int j = 0; j < text->nLines; j++)
    // {
    //     printf("\n%s\n", text->lines[j].line);
    // }

    return 0;

}

//------------------------------------------------------------------------------------------------------------------------------

int AnotherReadFromFile( char* Index[], const char* FileName)
{
    FILE* input = fopen(FileName, "r");

    int nLines = 0;

    char buffer[MAX_LENGTH] = { 0 };

    for (nLines = 0; nLines < MAX_LINES; nLines++)
    {

        printf("Here, i = %d\n", nLines);

        if (fgets(buffer, MAX_LENGTH, input) == 0)
            break;

        Index[nLines] = strdup (buffer);

        printf("Here, after getting i = %d, the line i got: %s\n", nLines, Index[nLines]);

        
    }

    fclose(input);

    return nLines;

}

//------------------------------------------------------------------------------------------------------------------------------

int createBuffer (const char* FileName, TEXT* text)
{
    assert (FileName);
    assert(text);

    struct stat FileInfo;
    int stat_flag = stat(FileName, &FileInfo);

    long long size = FileInfo.st_size;
    text->buffer = (char*)calloc(size, sizeof(char));
    // ptr check
    //printf("pointer is zero %d", text->buffer == NULL);

    FILE* input = fopen(FileName, "r");
    // ptr check
    text->buffSize = fread(text->buffer, sizeof(char), FileInfo.st_size, input);
    fclose(input);

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int printText(TEXT* text)
{
    for (int i = 0; i < text->nLines; i++)
    {
        printf("%s\n", text->lines[i].line);
    }
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

void PrintToFile (FILE * file, TEXT* text)
{

	assert(file != NULL);
	assert(text != NULL);

	for(int i = 0; i < text->nLines; i++)
	{
		fputs(text->lines[i].line, file);
		fputc('\n', file); 
	}
    fclose(file);

}

//------------------------------------------------------------------------------------------------------------------------------

int Destroy (TEXT* text)
{
    free(text->buffer);
    for (int i = 0; i < text->nLines; i++)
        free(text->lines[i].line);

    free(text->lines);

    return 0;
}
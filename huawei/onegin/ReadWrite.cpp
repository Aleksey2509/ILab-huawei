#include "ReadWrite.h"

const int MAX_LINES = 10000;

const int MAX_LENGTH = 1000;

int ReadFromFile(TEXT* text, const char* FileName)
{

    int Error = createBuffer(FileName, text);

    //printf("\n%llD\n", text->buffSize);

    for (int i = 0; i < text->buffSize; i++)
    {

        if(text->buffer[i] == '\n')
        {
            text->buffer[i] = '\0';
        }
    
    }

   

    for (int i = 0; i < text->buffSize ;i++)
    {
        
        text->lineArray[text->nLines] = strdup(text->buffer + i);
        //printf("\ngot %s, ", text->lineArray[text->nLines]);
        i += strlen(text->lineArray[text->nLines]);
        text->nLines++;
        //printf("Now there are %d lines", text->nLines);
    }

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

int createBuffer (const char* FileName, TEXT* text)
{
    assert (FileName);
    assert(text);

    struct stat FileInfo;
    int stat_flag = stat(FileName, &FileInfo);

    long long size = FileInfo.st_size;
    text->buffer = (char*)calloc(size, sizeof(char));

    //printf("pointer is zero %d", text->buffer == NULL);

    FILE* input = fopen(FileName, "r");
    text->buffSize = fread(text->buffer, sizeof(char), FileInfo.st_size, input);
    fclose(input);

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------

int printText(TEXT* text)
{
    for (int i = 0; i < text->nLines; i++)
    {
        printf("%s\n", text->lineArray[i]);
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
		fputs(text->lineArray[i], file);
		fputc('\n', file); 
	}
    fclose(file);

}

//------------------------------------------------------------------------------------------------------------------------------

int Destroy (TEXT* text)
{
    for (int i = 0; i < text->nLines; i++)
        free(text->lineArray[i]);

    free(text->lineArray);

    return 0;
}
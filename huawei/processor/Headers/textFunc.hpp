#ifndef READWRITE_HPP_INCL
#define READWRITE_HPP_INCL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

struct line_t
{
    char* line = 0;
    int lineLen = 0;
};

struct Text
{
    char* buffer = 0;
    long long buffSize = 0;
    line_t* lines = 0;
    int nLines = 0;
};

static const int MAX_INPUT_LINES = 10000;
static const int MAX_LINE_LENGTH = 4096;

enum ErrorCodes
{
    NULL_FL = -1000,
    CANT_ALLOC_BUF,
    CANT_GET_FL_INFO,
    NULL_TEXT_PTR,
    NOT_ENGH_MEM,
    RESIZE_ERR,
};


/*! 
 * \brief This function reads from file and creates TEXT struct
 *
 * It receives a file, gives it to \ref TEXT_CreateBuffer where buffer with the text from the whole file is created
 * Then it puts gives this buffer to \ref TEXT_Parcer where it is parced
 * 
 * Return values: NULL_FL  = -1000 - if was given a bad file
 *                CANT_ALLOC_BUF = -999 - if could not get mem for buffer with whole text
 *                CANT_GET_FL_INFO = -998 - if could not get info about your file
 *                NULL_TEXT_PTR = -997 - if was given null ptr to txt
 *                NOT_ENGH_MEM = -996 - if could not get mem for lines
 *                OK - everything is OK
 */
int TextReadFromFile(Text* text, const char* FileName);

/*! 
 * \brief This function reads from buffer with text, allocates memory for TEXT line_t array and fills it
 *
 * It receives a buffer with whole text
 * Then it puts into text->lines->line pointers to new lines from buffer
 * And to text->lines->lineLen it's length
 * 
 * Return values: NOT_ENGH_MEM = -996 - if could not get mem for lines
 *                OK - everything is OK
 */
int TextParcer (Text* text);

/*! 
 * \brief This function reads from file and creates TEXT struct
 *
 * It receives a file, creates buffer with the text from the whole file
 * 
 * Return values: NULL_FL  = -1000 - if was given a bad file
 *                CANT_ALLOC_BUF = -999 - if could not get mem for buffer with whole text
 *                CANT_GET_FL_INFO = -998 - if could not get info about your file
 *                NULL_TEXT_PTR = -997 - if was given null ptr to txt
 *                OK - everything is OK
 */
int TextCreateBuffer (const char* FileName, Text* text);

/*! 
 * \brief This function reallocates memory for line_t array
 *
 * It receives a line_t array, and pointer to value with allocated memory to that array
 * It doubles the array and changes the corresponding memory taken value
 * 
 * Returns an reallocated array or NULL if unsuccessful
 */
line_t* ReallocLineArr (line_t* lines, unsigned int* AllocedMemory);

/*! 
 * \brief This function prints text to file
 *
 * It checks whether the path to file and TEXT pointer are valid
 * Then prints the text to pointed file
 */
int PrintText(const char* path, Text* text);

/*! 
 * \brief This function prints text to file
 *
 * It checks whether  TEXT pointer is valid
 * Then prints the text to Stdout
 */
int PrintTextStdout(Text* text);

/*! 
 * \brief This function prints text to file
 *
 * It checks whether the path to file and TEXT pointer are valid
 * Then prints the text to pointed file
 */
int PrintTextToFile (const char* path, const Text* text);

/*! 
 * \brief This function prints text line to file
 */
int PrintLineToFile(const line_t* str, FILE* stream);

/*! 
 * \brief This function checks if error code corresponds to some state from enum ErrorCode 
 * and returns pointer to string with message, describing the error
 */
const char* TextGetError (int Error);

/*! 
 * \brief equvalent to strlen, but also counts '\n' as end of sting
 */
size_t mystrlen(const char* string);

/*! 
 * \brief This function free the memory aloocated for TEXT structure, if it was not free'd before
 */
int TextDestroy (Text* text);


#endif
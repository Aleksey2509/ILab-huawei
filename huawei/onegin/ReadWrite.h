static const int MAX_INPUT_LINES = 10000;
static const int MAX_LINE_LENGTH = 1000;

static const int REDUCE  = -5;
static const int ENLARGE = -4; 

static const int MULT_CONST = 2;

enum ERRORCODES
{
    NULL_FL = -1000,
    CANT_ALLOC_BUF,
    CANT_GET_FL_INFO,
    NULL_TEXT_PTR,
    NOT_ENGH_MEM,
    RESIZE_ERR
};

int TEXT_ReadFromFile(TEXT* text, const char* FileName);

int Fgets_ReadFromFile( char* Index[], const char* FileName);

int TEXT_CreateBuffer (const char* FileName, TEXT* text);

line_t* ReallocLineArr (line_t* lines, unsigned int* AllocedMemory);

int PrintTextStdout(TEXT* text);

int PrintTextToFile (char* path, TEXT* text);

int PrintLineToFile(const line_t* str, FILE* stream);

int PrintError (int Error);

size_t mystrlen(const char* string);

int TEXT_Destroy (TEXT* text);
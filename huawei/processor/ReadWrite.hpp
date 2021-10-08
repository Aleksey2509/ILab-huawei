static const int MAX_INPUT_LINES = 10000;
static const int MAX_LINE_LENGTH = 4096;

static const int REDUCE  = -5;
static const int ENLARGE = -4; 

static const int MULT_CONST = 2;

enum ERROR_CODES
{
    NULL_FL = -1000,
    CANT_ALLOC_BUF,
    CANT_GET_FL_INFO,
    NULL_TEXT_PTR,
    NOT_ENGH_MEM,
    RESIZE_ERR,
    OK = 0
};

const char ErrStrPtr [][100] = {"Unknown error\n",
                                "No error\n",
                                "Your file is too big\n", 
                                "Bad file pointer\n",
                                "Null text ptr\n",
                                "Not enough memory for line memory allocation\n",
                                "Could not get file info\n"};

int TEXT_ReadFromFile(TEXT* text, const char* FileName);

int TEXT_Parcer (TEXT* text);

int TEXT_CreateBuffer (const char* FileName, TEXT* text);

line_t* ReallocLineArr (line_t* lines, unsigned int* AllocedMemory);

int PrintText(const char* path, TEXT* text);

int PrintTextStdout(TEXT* text);

int PrintTextToFile (char* path, TEXT* text);

int PrintLineToFile(const line_t* str, FILE* stream);

const char* GetError (int Error);

size_t mystrlen(const char* string);

int TEXT_Destroy (TEXT* text);

int Fgets_ReadFromFile( char* Index[], const char* FileName);

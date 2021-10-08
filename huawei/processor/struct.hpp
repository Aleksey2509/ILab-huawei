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

struct TEXT
{
    char* buffer = 0;
    long long buffSize = 0;
    line_t* lines = 0;
    int nLines = 0;
};

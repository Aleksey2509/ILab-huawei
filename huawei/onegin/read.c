#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 10000

int ReadFromFile(const char* Index[], const char* name);

int main()
{
    
}

int ReadFromFile(const char* Index[], const char* name)
{
    FILE* input = fopen(name, 'r');

    char buffer[1000] = { 0 };

    

}
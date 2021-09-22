#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

int mystrlen (const char* str);

int main()
{
    char str[] = "MYGOD";
    printf("%d",mystrlen(str));

}

int mystrlen (const char* str)
{
    int len = 0;
    for (len = 0; str[len] != '\0'; len++)
        printf("\n%c\n", str[len] );
    return len;
}
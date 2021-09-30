#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include "struct.h"
#include "compare.cpp"
#include "ReadWrite.cpp"
#include "Onegin.cpp"

int main(int argc, char* argv[])
{
    Onegin(argc - 1, argv + 1);

    return 0;
}
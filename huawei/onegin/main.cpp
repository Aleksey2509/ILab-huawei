#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
// add all needed includes in hpp files
#include "struct.h"
// bad practice
#include "compare.cpp"
#include "sort.cpp"
#include "ReadWrite.cpp"
#include "Onegin.cpp"

int main(int argc, char* argv[]) // end to end test
{
    Onegin(argc - 1, argv + 1);

    return 0;
}
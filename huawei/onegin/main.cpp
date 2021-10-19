#include "struct.hpp"
#include "Onegin.hpp"

#define SORTCHECK 1

int main(int argc, char* argv[]) // end to end test
{
    Onegin(argv[1], argv[2]);

    #if SORTCHECK
    printf("Onegin ended working\n");
    testoutput(argv[2]);
    #endif

    return 0;
}
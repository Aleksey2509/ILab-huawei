#include "../Headers/struct.hpp"
#include "../Headers/Onegin.hpp"

#define SORTCHECK 1

int main(int argc, char* argv[])
{
    Onegin(argv[1], argv[2]);

    #if SORTCHECK
    printf("Onegin ended working\n");
    testoutput(argv[2]);
    #endif

    return 0;
}
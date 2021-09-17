#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "quadraticFunctions.cpp"

int main()
{
    UnitTest();

    return 0;
}

int UnitTest()
{
    int rootState = 0;
    double x1 = 0, x2 = 0;

    QuadraticTest TestArray [] = {  {0,   1,  2,      One,  -2,  -2}, 
                                    {0,   0,  2,     Zero,   0,   0},
                                    {0,   0,  0, Infinite,   0,   0},
                                    {1,   1,  1,     Zero,   0,   0},
                                    {1,   2,  1,      One,  -1,  -1},
                                    {1,   8, 16,      One,  -4,  -4},
                                    {1,  -6,  9,      One,   3,   3},
                                    {1,   4,  3,      Two,  -1,  -3},
                                    {1, -16, 63,      Two,   9,   7},
                                    {1,   1, -1,      Two, -0.5 + 0.5 * sqrt(5), -0.5 - 0.5 * sqrt(5)} };

    int TestCount = sizeof(TestArray) / sizeof(QuadraticTest);

    for (int i = 0; i < TestCount; i++)
    {
        rootState = solveQuadratic(TestArray[i].a, TestArray[i].b, TestArray[i].c, &x1, &x2);

        if ( (rootState == TestArray[i].ExpectedRootState) && ( doubleCompare(x1, TestArray[i].x1) == 0) && ( doubleCompare(x2, TestArray[i].x2) == 0 ) )
        {
            printf("Test %d: solveQuadratic (%7.3lf, %7.3lf, %7.3lf) - %s OK%s\n" , i, TestArray[i].a, TestArray[i].b, TestArray[i].c, 
                    set_green, set_default);
        }
        else
        {
            switch (rootState)
            {
                case (Fatal):    printf ("Test %d: solveQuadratic (%7.3lf, %7.3lf, %7.3lf) - %sthe programm seriously messed up%s\n"
                                         "You got x1 = %.3lf, x2 = %.3lf", i, TestArray[i].a, TestArray[i].b, TestArray[i].c, set_red, set_default, x1, x2);
                                 break;

                case (Infinite): printf ("Test %d: solveQuadratic (%7.3lf, %7.3lf, %7.3lf) - %sFAILED%s : expected infinite roots, got %d root/-s\n", 
                                          i, TestArray[i].a, TestArray[i].b, TestArray[i].c, set_red, set_default, rootState);
                                 break;

                default:         printf ("Test %d: solveQuadratic (%7.3lf, %7.3lf, %7.3lf) - %sFAILED%s : expected root x1 = %.3lf, x2 = %.3lf, got x1 = %.3lf, x2 = %.3lf\n",
                                          i, TestArray[i].a, TestArray[i].b, TestArray[i].c, set_red, set_default, TestArray[i].x1, TestArray[i].x2, x1, x2);
            }
        }

        x1 = x2 = 0;
    }

    return 0;
}
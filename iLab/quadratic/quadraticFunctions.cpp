#include "quadratic.h"

int Quadratic ()
{
    double a = NAN, b = NAN, c = NAN;

    getCoefficients (&a, &b, &c);

    double x1 = NAN, x2 = NAN;

    RootsState CurrentState = solveQuadratic (a, b, c, &x1, &x2);

    printRoots (CurrentState, x1, x2);

    return 0;
}
//--------------------------------------------------------------------------------------------------------------

void getCoefficients (double* a, double* b, double* c)
{
    int varCount = 0;

    printf("\nHello! This is the programm, which solves quadratic equations, v0.1!\n"
           "Given the standart form of quadratic equation as ax^2+bx+c=0\n"
           "Enter coefficient a b c with that punctuation (just spaces): ");
    varCount += scanf ("%lf %lf %lf", a, b, c);

    if (varCount != 3)
    {
        printf("You probably did not follow the punctuation mentioned earlier; exiting the programm\n");
        exit(1);
    }
}

//--------------------------------------------------------------------------------------------------------------

RootsState solveQuadratic (double a, double b, double c, double* x1, double* x2)
{

    assert( x1 != x2 );

    if (doubleCompare(a,0) == 0)
    {
        return solveLinear (b, c, x1, x2);
    }

    else
    { 
        double D = b*b - 4*a*c;

        if (D < 0)
        {
            return Zero;
        }

        else    if (doubleCompare(D, 0) == 0)
                {
                    *x1 = (-b / (2*a));
                    *x2 = *x1;

                    return One;
                }

                else
                {
                    double SqrtD = sqrt(D);

                    *x1 = (-b + SqrtD) / (2 * a);
                    *x2 = (-b - SqrtD) / (2 * a);

                    return Two;
                }

        return Fatal;
    }

    return Fatal;
}

//----------------------------------------------------------------------------------------------------------------

RootsState solveLinear (double a, double b, double* x1, double* x2)
{
    assert( x1 != x2 );

    if (a != 0)
    {
        *x1 = *x2 = -b/(a);
        
        return One;
    }

    else if (b != 0)
    {
        return Zero;
    }

        else
        {
            return Infinite;
        } 

    return Fatal;
}

//----------------------------------------------------------------------------------------------------------------


int printRoots (int RootsState, double x1, double x2)
{
    switch(RootsState)
    {
        case Fatal : printf ("There is something seriously wrong with this programm. "
                             "You got x1 = %lf, x2 = %lf, and rootstate = %d.\n"
                             "If you did not,tell the programmer there is a serious problem\n", x1, x2, RootsState);
                       break;

        case Infinite : printf ("There is Infinite amount of roots\n");
                       break;

        case Zero : printf ("There are no roots\n");
                       break;

        case One : printf ("There is one root:\nx = %.3lf\n", x1);
                       break;

        case Two : printf ("There are two roots:\nx1 = %.3lf, x2 = %.3lf\n", x1, x2);
                       break;

        default  : printf ("Okay now, you're not supposed to get this\n"
                           "You got x1 = %lf, x2 = %lf, and rootstate = %d\n"
                           "Go to the programmer, tell him your entries and what you got "
                           "then tell him he is a complete moron", x1, x2, RootsState);
                   break;
    }

    return 0;
}


int doubleCompare (double a, double b)
{
    int difference = a - b;

    if (difference > EPSILON)
        return 1;
    else if (difference < -EPSILON)
            return -1;
         else
            return 0;
}

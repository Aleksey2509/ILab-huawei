
/* This enum helps to show the state of roots of quadratic equations */
enum RootsState {Fatal = -1000, Infinite, EntryError, Zero = 0, One, Two}; // Fatal = 'f';


/* These strings are for coloring when printing test results */
const char set_red[] = "\033[31m";
const char set_green[] = "\033[32m";
const char set_default[] = "\033[0m";


/* This structure is created for testing 
 * It keeps coefficients, which it gives to the quadraticsolver
 * Also keeps the expected rootsState and roots, if there are any
 */
typedef struct T
{
    double a;
    double b;
    double c;
    RootsState ExpectedRootState;
    double x1;
    double x2;
} QuadraticTest;


/* Constant serving as persion - decider for comparing doubles*/
const double EPSILON = 0.00001;

int Quadratic (); // bad name

/* Solves quadratic equation
 *
 * Gets coefficients and pointers to variables, where solutions will be put
 * 
 * Returns a rootsState constant
 */
RootsState solveQuadratic (double a, double b, double c, double* x1, double* x2);

/* Solves linear equation
 *
 * Gets coefficients and pointers to variables, where solutions will be put
 * 
 * Returns a rootsState constant
 */
RootsState solveLinear (double a, double b, double* x1, double* x2);

/* Prints roots
 *
 * Gets rootState and variables
 * 
 * Printing depends on rootsState value
 * 
 */
int printRoots (int RootsState, double x1, double x2);


/* Compares double values
 *
 */
int doubleCompare (double a, double b);

/* Gets coefficients
 *
 * Gets pointers to coefficients and puts the values there
 * 
 * Also, if it does not get three doubles as coefficients, immidiatly stops programm
 */
void getCoefficients (double* a, double* b, double* c);


/* Function with unit tests */
int UnitTest();
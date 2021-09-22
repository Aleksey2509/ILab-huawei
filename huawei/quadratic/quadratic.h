
/*! \brief This enum helps to show the state of roots of quadratic equations */
enum RootsState 
{
    Fatal = -1000, /*!< value for a fatal error */
    Infinite,      /*!< value for infinite roots */ 
    EntryError,    /*!< value for error when trying to get coefficients */
    Zero = 0,      /*!< value to show that there are zero roots of a particular equation */
    One,           /*!< value to show that there are one roots of a particular equation */
    Two            /*!< value to show that there are two roots of a particular equation */
};


/*! These strings are for coloring when printing test results
 * set_red - sets the color of terminal to red 
 */
const char set_red[] = "\033[31m";
/*!
 * set_green - sets the color of terminal to green
 */
const char set_green[] = "\033[32m";
/*!
 * set_default - sets the color of terminal to white
 */
const char set_default[] = "\033[0m";


/*! 
 *\brief This structure is created for testing 
 *
 * It keeps coefficients, which it gives to the quadraticsolver
 * Also keeps the expected rootsState and roots, if there are any
 * 
 */
typedef struct Test
{
    double a;   /*!< coefficient a of test input */
    double b;   /*!< coefficient b of test input */
    double c;   /*!< coefficient c of test input*/
    RootsState ExpectedRootState;   /*!< Expected rootState. */
    double x1;  /*!< Expected bigger root */
    double x2;  /*!< Expected smaller root */
} QuadraticTest;


/*! \brief Constant serving as presion - decider for comparing doubles */
const double EPSILON = 0.00001;

/*! \brief A function, that solves the equation
 *
 *  Uses \ref getCoefficients, then \ref solveQuadratic, then \ref printRoots
 * 
 */
int Quadratic ();

/*! 
 * \brief Solves quadratic equation
 *
 * \details Gets coefficients and pointers to variables, where solutions will be put
 * 
 * \param a - coefficient of x^2 
 * \param b - coefficient of x
 * \param c - free coefficient
 * \param x1 - pointer to a double, where the bigger root will be put 
 * \param x2 - pointer to a double, where the smaller roots will be put
 * Returns a rootsState constant
 */
RootsState solveQuadratic (double a, double b, double c, double* x1, double* x2);

/*!
 * \brief Solves linear equation
 *
 * Gets coefficients and pointers to variables, where solutions will be put
 * 
 * \param a - coefficient of x 
 * \param b - free coefficient  
 * \param x1 - pointer to a double, where the bigger root will be put 
 * \param x2 - pointer to a double, where the smaller roots will be put
 * 
 * Returns a rootsState constant
 */
RootsState solveLinear (double a, double b, double* x1, double* x2);

/*! 
 * \brief Prints roots
 *
 * Gets rootState and variables
 * 
 * Printing depends on rootsState value
 * 
 * \param RootsState - state of the quadratic equation (solved or not, if solved: how many roots)  
 * \param x1 - the bigger root
 * \param x2 - the smaller root
 * 
 */
int printRoots (int RootsState, double x1, double x2);


/*! \brief Compares double values
 * 
 * \param a - a double to compare
 * \param b - a double to compare
 */
int doubleCompare (double a, double b);

/*! \brief Gets coefficients for quadratic equation
 *
 * Gets pointers to coefficients and puts the values there
 * 
 * \note if it does not get three doubles as coefficients, immidiately stops programm with exit(1)
 */
void getCoefficients (double* a, double* b, double* c);


/*! \brief Function with unit tests 
 *
 * It uses a array of \ref QuadraticTest structures, which are then give to \ref solveQuadratic
 * 
 * The given outputs are compared to expected, and the result of whether successful or failed test is printed
 * 
 * \note Uses coloring the terminal for better visualisation :)
 */
int UnitTest();
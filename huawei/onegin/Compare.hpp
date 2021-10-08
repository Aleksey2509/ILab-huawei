/*! 
 *\brief This function compares strings from the beginning 
 *
 * It compares 2 strings from beginning
 * String that would come earlier in alphabetic order is considered less
 * \note skips all non-letter characters
 * \note does not care for capital letters
 * 
 */
int StraightComparator (const void* arg1, const void* arg2);

/*! 
 *\brief This function compares strings from the end
 *
 * It compares 2 strings from end
 * String that would come earlier in alphabetic order if being reversed is considered less
 * \note skips all non-letter characters
 * \note does not care for capital letters
 * 
 */
int ReverseComparator (const void* arg1, const void* arg2);

/*! 
 *\brief This function compares 2 chars
 * Character that is earlier in alphabetic ored is considered the lesser one
 * \note does not care for capital letters
 * 
 */
int CharComparator (char a, char b);

/*! 
 *\brief This function swaps two lines
 */
void LineSwap (line_t* line1Ptr, line_t* line2Ptr);

/*! 
 *\brief This function swaps two values, pointed by arg1 and arg2 with size = DataSize
 *\warning if DataSize is more than 1024 bytes, the behaviour is undefined
 */
void VoidMemSwap (void* arg1, void* arg2, unsigned int DataSize);

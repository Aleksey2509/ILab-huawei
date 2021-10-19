/*! 
 * \brief This function is an absolute analog to standart qsort, with exception, that if objects that are being compared 
 * take more than 1024 bytes the behaviour is undefined
 */
void Myqsort (void* base, size_t nElem, size_t elemSize, int (*comparator)(const void *, const void *));
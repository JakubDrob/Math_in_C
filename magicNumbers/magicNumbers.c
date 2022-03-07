#include "magicNumbers.h"
#include <stdlib.h>

/**
 * @brief Check error indicator of uInt number.
 * Returns the error indicator value of the given uInt number.
 * @param number Pointer to the uInt structure.
 * @return Error code or 0 if no erro is detected.
 */
int uInt_error(uInt* number)
{
    return number->err;
}


/**
 * @brief Create new number with the specified size.
 * Creates number with the specific size passed
 * in the parameter size. Returns the uInt structure.
 * If size is set to 0 then empty but initialised structure is returned.
 * If memory error occurs the structure will have the error indicator set to
 * UINT_EMEM.
 * @param size Size of the new number (in digits).
 * @return Initialised uInt number structure.
 */
uInt uInt_newNumber(size_t size)
{
    uInt num;
    num.err = 0;

    //just return empty initialised structure if size is 0
    if(size == 0)
    {
        num.num = 0;
        num.size = 0;
        return num;
    }

    ///otherwise allocate the memory for the number and set it to 0
    num.size = size;
    num.num = calloc(size, 1);
    if(!num.num) num.err = UINT_EMEM;//set the error indicator to memory error

    return num;
}

/**
 * @brief Create the array of uInt numbers.
 * Creates the array of uInt numbers. Returns 0 if everything went ok or
 * UINT_EMEM if memory error occured. If it did occur then all previously allocated memory
 * will be dealocated.
 * @warning Do not use on initialised arrays since it will delete those numbers and cause a memory leak.
 * @param numbers Pointer to the array of uInt structures where the numbers will be stored.
 * @param len Length of the array numbers.
 * @param size Size of each number.
 * @return 0 If success and UINT_EMEM if memory error occurs.
 */
int uInt_newNumbers(uInt numbers[], size_t len, size_t numSize)
{
    for(size_t i=0; i<len; i++)
    {
        numbers[i] = uInt_newNumber(numSize);
        
        //if memory error occurs free whatever was alocated up to that point and leave
        if(uInt_error(&numbers[i]) == UINT_EMEM)
        {
            uInt_destroyNumbers(numbers, i);//just call destroy numbers
            return UINT_EMEM;
        }
    }
    return 0;
}

/**
 * @brief Destroy the uInt number.
 * Deallocates all memory taken by the uInt structure.
 * Also sets the size to 0 and clears the error indicator.
 * @param number - Pointer to the uInt strcture that will be destroyed.
 */
void uInt_destroyNumber(uInt* number)
{
    if(number->num)//free block and set pointer to 0
    {
        free(number->num);
        number->num = 0;//you should always set freed pointer to 0
        //to avoid hard to trace errors when you forget that you freed something
    }
    number->size = 0;
    number->err = 0;
    //NOTE: -> operator allows you to access the members of the
    //structure when having a pointer to the structure.
}

/**
 * @brief Destroy the numbers contained in the array.
 * Destroys the numbers contained in the array.
 * @param numbers Pointer to the array of uInt structures.
 * @param len Length of the array numbers.
 */
void uInt_destroyNumbers(uInt numbers[], size_t len)
{
    for(size_t i=0; i<len; i++)
    {
        uInt_destroyNumber(&numbers[i]);
    }
}

/**
 * @brief Change the size of the number.
 * Changes the size of the number. The number itself will be kept.
 * Well, unless you shring it, then it will be truncated.
 * Sets error indicator to UINT_EMEM if memory error occurs.
 * @param number - Pointer to the uInt strcture.
 * @param newSize - New size of the number in digits.
 */
void uInt_resizeNumber(uInt* number, size_t newSize)
{
    char* newBlock = realloc(number->num, newSize);
    if(!newBlock) number->err = UINT_EMEM;//set the error indicator

    number->num = newBlock;
    number->size = newSize;
}

/**
 * @brief Print number.
 * Prints number to the standard output.
 * @param num Pointer to the number structure.
 */
void uInt_print(const uInt* num)
{
    if(num->num==0 || num->size==0)//don't cause nullptr error when number is empty
    {
        printf("Nothing to print number is empty!\n");
        return;
    }

    for(size_t i=0; i<num->size; i++)
    {
        putc(num->num[i], stdout);
    }
}

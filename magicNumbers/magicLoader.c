#include "magicNumbers.h"
#include <string.h>
#include <ctype.h>//for isspace

/**
 * @brief Find integer in the array of integers.
 * Finds integer value in the array of integers and then returns
 * the pointer to that integer. Returns NULL if value was not found.
 * @param arr Pointer to the array that will be searched.
 * @param len Length of the array.
 * @param val Value that we want to find.
 * @return Pointer to the found element or NULL if element was not found.
 */
// NOTE: Static keyword is used to make this function invisible outside 
// this translation unit (this file).
static int* arrInt(const int* arr, size_t len, int val)
{
    for(size_t i=0; i<len; i++)
    {
        if(*arr == val) return (int*) arr;
        arr++;
    }
    return NULL;
}

/**
 * @brief Load a sequence of arbitrary length unsigned integer from the file.
 * Loads the sequence of arbitary length unsigned integers from the file.
 * Each number is stored as a text.
 * The pointer to the input file descriptor is passed as file.
 * The pointer to the array of arbitrary unsigned number structures is
 * passed in numbers argument.
 * The maximum number of numbers that will be loaded from the file
 * is passed in an argument 'count'.
 * The function returns the number of arbitary length numbers that was actually
 * loaded from the file. It returns -1 if error has occured.
 * @note All numbers in the array pointed to by 'numbers' will be overwitten.
 * @param numbers Pointer to the array of arbitrary length unsigned integers sturctures.
 * @param count The maximum number of numbers that we can load from the file.
 * @param separator Character that is used to separate the variables.
 * @param endChar Character at the end of the number sequence.
 * @param file File descriptor.
 * @return The number of numbers loaded from the file or -1 if invalid character is detected,
 * -2 if IO error occurs and -3 if your computer is a potato (memory error occurs).
 */
int uInt_fLoad(uInt numbers[], int count, char separator, char endChar, FILE* file)
{ 
    int i=0;
    for(; i<count; i++)
    {
        size_t numSize = 0;//size of the number
        while(1)
        {
            int c = getc(file);

            //check for read error
            if(ferror(file))
            {
                return -2;
            }

            //check for invalid character
            //NOTE: \r and \n characters are there because many editors add them at the end of
            //the line automatically which would cause a loading error.
            /*const int validvalues[10] = {separator, endChar, '0', '1', EOF, '\n', '\r', '\v', '\f', '\t'};
            if(!arrInt(validvalues, 10, c))
            {
                return -1;
            }*/

            // UNCOMMENT FOR DECIMAL insead of binary (and comment the block above)
            /*const int validvalues[18] = {separator, endChar, '0', '1',
                '2', '3', '4', '5', '6', '7', '8', '9', EOF, '\n', '\r', '\v', '\f', '\t'};
            if(!arrInt(validvalues, 18, c))
            {
                return -1;
            }*/
            // UNCOMMENT FOR HEXADECIMAL
            const int validvalues[24] = {separator, endChar, '0', '1',
                '2', '3', '4', '5', '6', '7', '8', '9','A','B','C','D','E','F', EOF, '\n', '\r', '\v', '\f', '\t'};
            if(!arrInt(validvalues, 24, c))
            {
                return -1;
            }

            //check for the end condition
            if(c==endChar || c==EOF) return i+1;

            //check for separator
            if(c==separator && numSize>0) break;

            //skip all white characters
            if(isspace(c)) continue;
    
            //resize the number to fit new digit
            uInt_resizeNumber(&numbers[i], numSize+1);
            if(uInt_error(&numbers[i]) == UINT_EMEM)
            {
                return -3;
            }

            //add new character to the number
            numbers[i].num[numSize] = c;
            numSize++;
        }
    }

    return i;
}

#ifndef MAGIC_NUMBERS_H_INCLUDED
#define MAGIC_NUMBERS_H_INCLUDED

//headers are included in the header file because difintions of types
//FILE is defined defined inside them.
#include <stdio.h> //for FILE descriptor

/** @brief Arbitarty length unsigned integer. */
typedef struct
{
    char* num; /** @brief Number stored as text. */
    size_t size; /** @brief Size of the number in digits. */
    int err; /** @brief Error indicator. */
} uInt; 
//NOTE: we define the custom type so we don't have to write struct all the time.
//In C++ this is done automatically but this is C so we have to do this.

//define error codes for the error flag
#define UINT_EMEM 1 /** @brief Memory error code. */
#define UINT_EUNDFL 2 /** @brief Underflow error code. */

int uInt_fLoad(uInt numbers[], int count, char separator, char endChar, FILE* file);
uInt uInt_newNumber(size_t size);
int uInt_newNumbers(uInt numbers[], size_t len, size_t numSize);
void uInt_destroyNumber(uInt* number);
void uInt_destroyNumbers(uInt numbers[], size_t len);
void uInt_resizeNumber(uInt* number, size_t newSize);
void uInt_print(const uInt* num);
int uInt_error(uInt* number);
uInt uInt_addBIN(const uInt* num1, const uInt* num2);
uInt uInt_subBIN(const uInt* num1, const uInt* num2);
uInt uInt_addDEC(const uInt* num1, const uInt* num2);
uInt uInt_addHEX(const uInt* num1, const uInt* num2);

#endif

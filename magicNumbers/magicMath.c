#include "magicNumbers.h"
#include <stdbool.h>//for bool type
//bool type can have value of true or false

/**
 * @brief Determines required length of the output
 * during the addition operation so that the overflow will not occur.
 * @param num1 Number 1
 * @param num2 Number 2
 * @return Required length in digits.
 */
static size_t determineLength(const uInt* num1, const uInt* num2)
{
    size_t outputLength;
    if(num1->size > num2->size)
    {
        outputLength = num1->size+1;
    }
    else
    {
        outputLength = num2->size+1;
    }

    return outputLength;
}

void reverse(char *given, int size)
{
    int i;
    char temp[size];
    for(i=0; i<size; i++)
    temp[i] = given[i];
    for(i=0; i<size; i++)
    given[i] = temp[size-i-1];
}
/**
 * @brief Add two numbers stored in the binary format.
 * Adds two numbers stored in num1 and num2 and returns the result
 * as a uInt number.
 * Size of the output is adjusted automatically to match the sizes of the number.
 * If memory error occured the output number will have the 
 * error indicator set to UINT_EMEM.
 * @param num1 Number 1
 * @param num2 Number 2
 * @return Sum of tnum1 and num2.
 */
uInt uInt_addBIN(const uInt* num1, const uInt* num2)
{
    //create the result number
    uInt result = uInt_newNumber(determineLength(num1, num2));
    //check for memory error
    if(uInt_error(&result) == UINT_EMEM) return result;

    //now perform the addition
    //use full adder formula
    //sum = (X ⊕ Y) ⊕ C
    //C = (A ⋅ B) + (Cin ⋅ (A ⊕ B)). 
    //more on wikipedia https://en.wikipedia.org/wiki/Adder_%28electronics%29#Full_adder
    //also this code is based on https://stackoverflow.com/questions/13282825/adding-binary-numbers-in-c
    bool c = 0;//carry
    for(size_t i=1; i<=result.size; i++)
    {
        bool x = 0;
        bool y = 0;
        bool sum;

        //get x 
        if(i<=num1->size)
        {
            x = num1->num[num1->size-i] - '0';
            //NOTE: A simple trick to convert from character to number.
            //Just substract the value of character 0!
            //Look at the ASCI table to see why this works!
        }

        //get y
        if(i<=num2->size)
        {
            y = num2->num[num2->size-i] - '0';
        }

        //perform the addition
        sum = (x ^ y) ^ c;
        c = ((x & y) | (x & c)) | (y & c); 

        //printf("x=%d, y=%d, sum=%d, c=%d\n", x, y, sum, c);

        //write the result as a string
        result.num[result.size-i] = '0' + sum;
        //NOTE: A simple trick to convert from number back to character.
        //Just add the value of character 0!
        //Look at the ASCI table to see why this works!
    }

    return result;
}

/**
 * @brief Substract two numbers stored in the binary format.
 * Substracts num2 from num1. Returns the result of the operation.
 * If underflow occurs (uInt is an unsigned number) result will have
 * error indicator set to UINT_EUNDFL. If memory error occurs the
 * result will have the error indicator set to UINT_EMEM.
 * @param num1 Number from which we are substracting.
 * @param num2 Number that we are substracting from num1.
 * @return The result of the operation.
 */
uInt uInt_subBIN(const uInt* num1, const uInt* num2)
{
    uInt result = uInt_newNumber(num1->size);
    //check for memory error
    if(uInt_error(&result) == UINT_EMEM) return result;

    //perform the substraction using the formula for the full substractor
    //d = (X ⊕ Y) ⊕ B
    //b = X.Y + (X ⊕ Y)BIN

    bool b = 0;//borrowed
    for(size_t i=1; i<=result.size; i++)
    {
        bool x = 0;
        bool y = 0;
        bool d;

        //get x 
        x = num1->num[num1->size-i] - '0';

        //get y
        if(i<=num2->size)
        {
            y = num2->num[num2->size-i] - '0';
        }

        d = ((x ^ y) ^ b);
        b = (((!x) & y) | (!(x ^ y) & b));

        //printf("x=%d, y=%d, d=%d, b=%d\n", x, y, d, b);

        //write the result as a string
        result.num[result.size-i] = '0' + d;
    }

    //if b is 1 then we are fucked because we just underflowed
    //uInt is an unsigned number so it cannot store negative values
    //so let's set error to underflow
    if(b) result.err = UINT_EUNDFL;

    return result;
}

/**
 * @brief Add two numbers stored in the decimal format.
 * Adds two numbers stored in num1 and num2 and returns the result
 * as a uInt number.
 * Size of the output is adjusted automatically to match the sizes of the number.
 * If memory error occured the output number will have the 
 * error indicator set to UINT_EMEM.
 * @param num1 Number 1
 * @param num2 Number 2
 * @return Sum of tnum1 and num2.
 */
uInt uInt_addDEC(const uInt* num1, const uInt* num2)
{
    
    size_t numSize = 0;
    uInt result= uInt_newNumber(numSize);
    //check for memory error
    if(uInt_error(&result) == UINT_EMEM) return result;

    //now perform the addition
    bool c = 0;//carry
    int maxSize = determineLength(num1, num2);
    for(size_t i=1; i<=maxSize; i++)
    {
        int x = 0;
        int y = 0;
        int sum;

        //get x 
        if(i<=num1->size)
        {
            x = num1->num[num1->size-i] - '0';
        }

        //get y
        if(i<=num2->size)
        {
            y = num2->num[num2->size-i] - '0';
        }

        //perform the addition
        sum = (x + y + c) % 10;
        c = (x + y + c) > 9;
        
        if(i==maxSize && sum==0) break;

        uInt_resizeNumber(&result, numSize+1);

        //write the result as a string
        result.num[numSize] = '0' + sum;
        numSize++;
    }
    reverse(result.num, numSize);
    return result;
}

uInt uInt_addHEX(const uInt* num1, const uInt* num2)
{
    size_t numSize = 0;
    uInt result= uInt_newNumber(numSize);

    if(uInt_error(&result) == UINT_EMEM) return result;

    bool c = 0;
    int maxSize = determineLength(num1, num2);
    for(size_t i=1; i<=maxSize; i++)
    {
        int x = 0;
        int y = 0;
        int sum;

        //get x 
        if(i<=num1->size && (num1->num[num1->size-i]- '0')<10)
        {
            x = num1->num[num1->size-i] - '0';
        }
        if(i<=num1->size && (num1->num[num1->size-i]- '0')>10)
        {
            x = num1->num[num1->size-i] - 'A' + 10;
        }
        

        //get y
        if(i<=num2->size && (num2->num[num2->size-i] - '0')<10)
        {
            y = num2->num[num2->size-i] - '0';
        }
        if(i<=num2->size && (num2->num[num2->size-i] - '0')>10)
        {
            y = num2->num[num2->size-i] - 'A' + 10 ;
        }
        

        //perform the addition
        sum = (x + y + c) % 16;
        c = (x + y + c) > 15;
        
        if(i==maxSize && sum==0) break;

        uInt_resizeNumber(&result, numSize+1);

        //write the result as a string
        if(sum<10)
        {
        result.num[numSize] = '0' + sum;
        }
        else
        {
        result.num[numSize] = 'A' + sum - 10;
        }
        numSize++;
    }
    reverse(result.num, numSize);
    return result;
}




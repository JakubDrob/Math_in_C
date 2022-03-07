#include <stdio.h>
#include "magicNumbers/magicNumbers.h"

//NOTE: The use of goto for error handling is a legitemate pattern called goto chain pattern
//and in this case it reduces the code complexity. 
int main(int argc, char** argv)
{
    //Check if the number of arguments is correct.
    //First argument passed to a program
    //is always the command that was used to run the program.
    //We expect 1 argument besides that - the path to the file.
    if(argc != 2)
    {
        fprintf(stderr, "ERR: Invalid argument number! 1 Expected, %d given\n", argc-1);
        return 0;
    }

    //NOTE: fprintf is just like printf but it allows you to choose the file
    //In this case we are are printing the message to the error output stream

    //create two empty numbers for storing those that were loaded from the file
    uInt numbers[2];//array for two arbitrary unsigned numbers
    if(uInt_newNumbers(numbers, 2, 0) == UINT_EMEM)
    {
        fprintf(stderr, "ERR: Memory error occured!\nMaybe your computer is a potato?\n");
        return 0;
    }
    
    //open file
    FILE* file = fopen(argv[1], "r");//open the file for read
    if(!file)//error occured while opening the file
    {
        fprintf(stderr, "ERR: Cannot open the file '%s'\n", argv[1]);
        fprintf(stderr, "Maybe the file does not exists?\n");
        goto arrCleanup;//clean the arrays and end the program
    }

    //load numbers from the file
    const int status = uInt_fLoad(numbers, 2, ' ', 0, file);//load 2 numbers from the file
    fclose(file);//close file since we do not need it any more
    if(status != 2 && status >= 0)
    {
        fprintf(stderr, "ERR: Invalid amount of numbers, expect 2! %d given\n", status);
        goto arrCleanup;
    }
    switch(status)
    {
        case -1://print error message for uInt_fLoad returning code -1 (invalid character)
            fprintf(stderr, "ERR: Invalid character detected in the file!\n");
        goto arrCleanup;

        case -2://print error message for other error (code -2)
            fprintf(stderr, "ERR: IO error occured while loading numbers from the file!\n");
        goto arrCleanup;

        case -3://print error message for other error (code -2)
            fprintf(stderr, "ERR: Memory error occured!\nMaybe your computer is a potato?\n");
        goto arrCleanup;
    }

    //perform the operarion on the numbers from the file and print the result
    //uInt result = uInt_addBIN(&numbers[0], &numbers[1]);
    uInt result = uInt_addHEX(&numbers[0], &numbers[1]);
    if(uInt_error(&result) == UINT_EMEM)
    {
        fprintf(stderr, "ERR: Memory error occured!\nMaybe your computer is a potato?\n");
        goto cleanup;
    }

    //print the result
    uInt_print(&numbers[0]);
    putc('+', stdout);
    uInt_print(&numbers[1]);
    putc('=', stdout);
    uInt_print(&result);
    putc('\n', stdout);
    if(uInt_error(&result) == UINT_EUNDFL)//check for underflow
    {
        printf("UNDERFLOW DETECTED!\n");
    }

    //free memory taken by our numbers
    cleanup:
    uInt_destroyNumber(&result);

    arrCleanup:
    uInt_destroyNumbers(numbers, 2);

    return 0;
}

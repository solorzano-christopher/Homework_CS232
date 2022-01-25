/* File: primes.c */
/* Author: Britton Wolfe */
/* Date: July 16, 2009 */
/* This program outputs all the primes in the range given */
/* by the command line arguments */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, const char** argv){

    int lowerBound, upperBound;

    if(argc != 3){
        fprintf(stderr, "USAGE: %s lowerBound upperBound\n", argv[0]);
        return -1;
    }
  
    lowerBound = atoi(argv[1]);
    upperBound = atoi(argv[2]);
  
    if(lowerBound < 1 || upperBound < 1) //if they are lower than 1 the inputs are invalid. Either or.
    {
        fprintf(stderr, "ERROR: both the lowerBound (%d) and the upperBound (%d)"
	          " must be positive.\n", lowerBound, upperBound);
        return -2;
    }
        int increment; // a interger to increment to check if % == 0.
        for(lowerBound; lowerBound <= upperBound; lowerBound++)//must increment lowerBounds until it equals upperBound.
                                                               //This will check all numbers from the given input.
        {
            for(increment = 2; increment <= lowerBound; increment ++) //increasing increment intil value == 0
            {
                if(lowerBound % increment == 0) //If the remainder is 0 after divison. 
                {
                    break; //will break indicating that it has found a value that % == 0
                }
            }
            if(lowerBound == increment)//if the values are the same it must be a prime number.
                                       //Since % == 0 can only happen if they are the same in this point in the program. 
            {
                printf("%d\n", lowerBound); //returning the prime number to screen or any tests that use text.
            }
        }

    return 0;
}
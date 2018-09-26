/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    //check that n is positive
    if (n>0)
    {
        int start = 0;
        int end = n - 1;
        
        //search for the specified value by reducing the search space in half each time
        while (end>=start)
        {
            int middle = (start + end)/2;
        
            if (value == values[middle])
            {
                return true;
            }
            else if (value > values[middle])
            {
                start = middle + 1;
            }
            else if (value < values[middle])
            {
                end = middle - 1;
            }
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    //declare array
    int max = 65456;
    int count_array[max];
    
    //create an array of zero's of size 65456
    for (int i=0; i<max; i++)
    {
        count_array[i]=0;
    }
    
    //count the number of times a value appears in the original array by matching the value with the index of the zero array
    for (int i=0; i<n; i++)
    {
        count_array[values[i]]++;
    }
    int c=0;
    //loop through the array
    for (int i=0; i<max; i++)
    {
        //rearrange the original array so that the values appear in order
        while (count_array[i]!=0)
        {
            values[c]=i;
            count_array[i]-=1;
            c++;
        }
        //terminates the loop once all the values in the original array have been accounted for
        if (c==n)
        {
            break;
        }
    }
}

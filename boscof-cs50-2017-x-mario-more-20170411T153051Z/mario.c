#include <cs50.h>
#include <stdio.h>

int main(void){
int n;
    //ask user for valid input
    do 
    {
    printf("Height:\n");
    n = get_int();
    }
    while (n>23||n<0);
    
    //prints pyramid according to input specifications
    for(int i=0; i<n; i++)
    {
        //adds in the intial spaces
        for (int j=0; j<n-i-1; j++)
            printf(" ");
        //adds in the blocks for the first half of the pyramid along with the spaces in the middle
        for (int j=0; j<i+1; j++)
            printf("#");
        printf("  ");
        //adds in the blocks for the second half of the pyramid
        for (int j=0; j<i+1; j++)
            printf("#");
        printf("\n");
    }
}
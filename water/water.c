#include <cs50.h>
#include <stdio.h>

int main(void)
{
int n;
    //ensure n is a positive integer
    do
    {
        //ask user for input
        printf("Minutes:\n");
        n = get_int();
        
        //calculate bottles of water used
        n = n * 12;
        printf("Bottles:%i\n", n);
    }
    while (n<0);
}


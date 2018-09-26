#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("please enter valid string to compare to\n");
        return false;
    }
    
    printf("Enter in string:");
    string s = get_string();
    if (strcasecmp(argv[1], s)==0)
    {
        printf("The string is the same\n");
    }
    else
    {
        printf("The string is not the same\n");
    }
    return true;
}

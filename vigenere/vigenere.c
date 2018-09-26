#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    //check that the input is valid
    if (argc!=2)
    {
        printf("Please enter a valid encryption key\n");
        return 1;
    }
    
    //obtain the length of the entered encryption key
    int m=strlen(argv[1]);
    //check that the input is composed of alphabetical characters
    for (int i=0; i<m;i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Please enter a valid encryption key\n");
            return 1;
        }
    }
    
    //allow the user to input the plain text to be encrypted
    printf("plaintext:");
    string p = get_string();
    
    printf("ciphertext:");
    //encrypt all the characters in the string
    for (int i=0, c=0, n=strlen(p);i<n;i++)
    {
        //if the character is alphabetical, maintain the capitalisation and encrypt
        if (isalpha(p[i]))
        {
            //calculate the ASCII code and then shift by the number of required places, wrapping around the keyword if the keyword is reached
            if (islower(p[i]) && islower(argv[1][c % m]))
            {
                printf("%c", (char) ((((int) p[i] - 97 + (int) argv[1][c % m] - 97) % 26) + 97));
                c++;
            }
            else if (islower(p[i]) && isupper(argv[1][c % m]))
            {
                printf("%c", (char) ((((int) p[i] - 97 + (int) argv[1][c % m] - 65) % 26) + 97));
                c++;
            }
            else if (isupper(p[i]) && islower(argv[1][c % m]))
            {
                printf("%c", (char) ((((int) p[i] - 65 + (int) argv[1][c % m] - 97) % 26) + 65));
                c++;
            }
            else if (isupper(p[i]) && isupper(argv[1][c % m]))
            {
                printf("%c", (char) ((((int) p[i] - 65 + (int) argv[1][c % m] - 65) % 26) + 65));
                c++;
            }
        }
        //otherwise just print the character as it is
        else 
        {
            printf("%c", p[i]);
        }
    }
    printf("\n");
    return 0;
}

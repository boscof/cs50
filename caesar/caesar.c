#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    //check that input is valid
    if (argc !=2)
    {
        printf("Please enter a valid encryption key\n");
        return 1;
    }
    
    //obtain the key
    int k = atoi(argv[1]);
    
    //allow the user to input the plain text to be encoded
    printf("plaintext:");
    string p = get_string();
    
    printf("ciphertext:");
    //shift each letter by k places in the alphabet while preserving the original capitalisation
    for (int i=0, n=strlen(p);i<n;i++)
    {
        if (islower(p[i]))
        {
            printf("%c", (char) ((((int) p[i] - 97 + k) % 26) + 97));
        }
        else if (isupper(p[i]))
        {
            printf("%c", (char) ((((int) p[i] - 65 + k) % 26) + 65));
        }
        else 
        {
            printf("%c", p[i]);
        }
    }
    printf("\n");
    return 0;
}

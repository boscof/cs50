#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string s=get_string();
    
    if (s!=NULL)
    {
        if (s[0]!=' ')
        {
            //prints the first letter of the person's initials
            printf("%c", toupper(s[0]));
        }
        
        //finds the subsequent letters of the person's initials, ensuring that the program does not print a null char
        for (int i=0, n=strlen(s);i<n;i++)
        {
            if (s[i]==' ' && s[i+1]!=' ' && s[i+1]!='\0')
            {
                printf("%c", toupper(s[i+1]));
            }
        }
        printf("\n");
    }
}
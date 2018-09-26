#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long long n;
    //ask user for valid input
    do
    {
        printf("Number:\n");
        n = get_long_long();
    }
    while (n<0);
        
    long long m=n;
    int product=0;
    int even_sum=0;
    //multiply every second digit by two, starting from the number's second-to-last digit
    do
    {
        m/=10;
        product = (m % 10)*2;
        //sums the products digits
        do
        {
            even_sum = even_sum + product % 10;
            product/=10;
        }
        while(product>0);
        m/=10;
    }
    while (m>0);
    
    m=n;
    int odd_sum=0;
    //sum every second digit, starting from the numbers last digit
    do
    {
        odd_sum = odd_sum + (m % 10);
        m/=100;
    }
    while (m>0);
    int sum=odd_sum + even_sum;
    
    int count=0;
    m=n;
    //counts the number of digits in the original number
    while (m!=0)
    {
        m/=10;
        ++count;
    }
    m=n;
    
    m=n;
    //assigns the first two digits of the number to new variables
    while(m>100)
    {
        m/=10;
    }
    int two_digit=m;
    int first_digit=two_digit/10;
    
    //checks the validity of the credit card number
    if (sum % 10==0)
    {
        //determines the credit card type
        if ((count==13 && first_digit==4)||(count==16 && first_digit==4))
        {
            printf("VISA\n");
        }
        else if (count==16 && (two_digit==51||two_digit==52||two_digit==53||two_digit==54||two_digit==55))
        {
            printf("MASTERCARD\n");
        }
        else if (count==15 && (two_digit==34||two_digit==37))
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
import cs50

def main():
    #ask user for valid input
    while True:
        print("Number:", end="")
        f = cs50.get_float()
        if f>0:
            break
    #calculate the sum of the even digits multiplied by 2
    sum = get_even_sum(f)
    
    #add on the sum of the remaining digits
    sum += get_odd_sum(f)
    
    #count the number of digits
    count = get_digits(f)
    
    #find the starting two digits
    two_digit = get_two_digits(f)
    
    #find the first digit
    first_digit = get_first_digit(f)
    
    #checks the validity of the credit card
    credit_card(sum, count, two_digit, first_digit)

#calculate the sum of the even digits according to the Luhn algorithm
def get_even_sum(n):
    product=0
    even_sum=0
    #multiply every even digit by two and then take a cumulative sum
    while n>0:
        n//=10
        product = (n % 10)*2
        while product > 0:
            even_sum += product % 10
            product//=10
        n//=10
    return even_sum

#calculates the sum of the odd digits in a number
def get_odd_sum(n):
    odd_sum=0
    #take the cumulative sum of every odd digit 
    while n>0:
        odd_sum+=(n %10)
        n//=100
    return odd_sum
    
#calculates the number of digits in an umber
def get_digits(n):
    count=0
    while n!=0:
        n//=10
        count+=1
    return count

#finds the first two digits of a number    
def get_two_digits(n):
    while n>100:
        n//=10
    return n

#finds the first digit of a number
def get_first_digit(n):
    n = get_two_digits(n)
    n //=10
    return n

#checks the validity of the credit card according to credit card specifications
def credit_card(sum, count, two_digit, first_digit):
    if sum % 10==0:
        if (count==13 or count==16) and first_digit==4:
            print("VISA")
        elif count==16 and 51<=two_digit<=55:
            print("MASTERCARD")
        elif count==15 and (two_digit==34 or two_digit==37):
            print("AMEX")
        else:
            print("INVALID")
    else:
        print("INVALID")

if __name__ == "__main__":
    main()
    
        
    
    


    
    

    
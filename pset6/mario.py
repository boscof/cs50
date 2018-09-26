import cs50

#ask user for valid input
while True:
    print("Height:", end ="")
    n = cs50.get_int()
    if n<24 and n>0:
        break
    
for i in range(1,n+1):
    #add in the initial spaces
    print(" " * (n-i), end="")
    #add in the blocks for the first half of the pyramid
    print("#" * i, end="")
    #add in the gap in the middle of the pyramid
    print("  ", end="")
    #add in the blocks for the second half of the pyramid
    print("#" * i, end="")
    #move on to the next line of the pyramid
    print("")

    
        
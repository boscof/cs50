import cs50
import sys

def main(argv):
    #check that input is valid
    if len(sys.argv)!=2:
        print("Please enter a valid encryption key")
        exit(1)
    
    #obtain the key
    k = int(sys.argv[1])
    
    #allow the user to input the plain text to be encoded
    print("plaintext:", end="")
    s = cs50.get_string()
    
    #encrypt the plain text
    encrypt_text(s, k)
    
    #return 0 if successful
    exit(0)
    
#encrypts the plain text    
def encrypt_text(s, k):
    print("ciphertext:", end="")
    #shift each letter by k places in the alphabet while preserving case sensitivity
    for c in s:
        if c.islower():
            i = ((ord(c) - 97 + k) % 26)+97
            print("{}".format(chr(i)), end="")
        elif c.isupper():
            i = ((ord(c) - 65 + k) % 26)+65
            print("{}".format(chr(i)), end="")
        else:
            print("{}".format(c), end="")
    print("")

if __name__ == "__main__":
    main(sys.argv)
    
    
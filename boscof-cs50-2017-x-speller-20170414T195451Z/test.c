#include <stdio.h>

int main(void)
{
    int myInt = 5;
    int *myIntPointer;
    myIntPointer = &myInt;  //create a pointer that points to the address of myInt


    printf("Value stored in myInt = %d\n", myInt);
    printf("Address stored in myIntPointer = %p\n", myIntPointer);
    printf("Address of myInt (&myInt) is %p\n", &myInt);
    printf("Value stored at address in *myIntPointer is %d\n", *myIntPointer);
}

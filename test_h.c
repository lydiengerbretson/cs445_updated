#include <stdio.h>
#include "test_h.h"

void print_hi(void)
{
    printf("Hi\n");
}

int main (void)
{
    char * name1= "bob";
    char * name2 = "steve";
    char * name3 = "lucy";
    
    names[0] = name1;
    names[1] = name2;
    names[2] = name3;
    
    printf("Names: %s, %s, %s\n", names[0], names[1], names[2]);
    
    print_hi();

    return 0;
}
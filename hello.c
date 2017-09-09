//#include "foo.h"
#include "hello.h"

int main(){

    "This is /*not a comment*/ \n"
    /* This is a comment  */
    /* C++ does not support /* nested comments */ */
    /* This also contains a //nested comment */
    
    "Keep this string  \n" // this is a comment "//... "
    "This // is not a comment... \t"
    KEEPTHIS; // Another comment
    x = 1; // Also a comment, after x = 1;  
    return 0; 
}
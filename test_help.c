#include <iostream>
#include <string>
using namespace std; 

int main()
{
    int z; 
	int yay;
    z = 0;
	//string s; 
 
    z = use_foo(11);
    
    return 0;
}

int foo()
{
    int yas;
    yas = 3;
   
   
    return yas;
}

int use_foo(int c)
{
    int woah;
    woah = foo();
	 
	//yes = foo(); 
    
    return woah + c;

}
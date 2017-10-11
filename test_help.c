#include <iostream>
#include <string>

using namespace std; 


int main()
{
	//ofstream cout; 
	//ofstream cin; 
    int z; // shows up as double if #include string is commented!!
	int yay;
	int x; // shows up as double if it is x whether or not string is commented
	int yowzer; // shows up as double if it is y whether or not string is commented
    int zoo = 0;
	int y; 
	int zane; 
	int a; 
	int b; 
	//int zane;

	string sstr; // fails if it is string s 
 
    zoo = use_foo(11);
	z = 10;
    
    return 0;
}

int foo()
{
    int yas;
	   
    int y; 
    int x; 
    yas = 3;
	//int yas; 
	int zane;
	
	y = 3; // will fail with zoo
	//yay = 3; 

    
    return y;
}

int help(int a)
{
	int z; 
	int x; 
	int y; 
	
	y = foo(); 
}

int use_foo(int c)
{
    int woah;
	//double woah; 
    woah = foo();
	 
	//yes = foo(); 
    
    return woah + c;

}

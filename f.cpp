#include <iostream>
using namespace std;

double myfunc(double, double);
double goof(double);


int main()
{
double x, y;
	double z;
	//int a[10];
	z = myfunc(x, y);
	cout << "zed was " << z << endl;
	cout << "yay\n";
	
	z = 0;

	
	return goof();
}


double myfunc(double x, double y)
{
	x = goof(2.0);
	return x;
}

double goof(double branflakes)
{
    double bf2 = branflakes + 2.0;
    cout << "bf2 is " << bf2 << endl;
    return myfunc(bf2);
}

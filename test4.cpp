using namespace std;

class Foo {

double d;

public:

Foo(){

  d = 5.5;

}

int fib(int i)

{

   double x = d * 2.0;

   return 11;

}

};


int main()

{

   Foo f;

   int i; i = 0 + f.fib(3);

   i = 5;

   i = f.fib("main"); // type error

   return 0;

}
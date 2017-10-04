
int main()
{
    int z;
    z = 0;
    
    z = use_foo(11);
    
    return 0;
}

int foo()
{
    int y;
    y = 3;
    
    return y;
}

int use_foo(int c)
{
    int w;
    w = foo();
    
    return w + c;

}
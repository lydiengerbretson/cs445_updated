#include <iostream>

using namespace std;
int
main ()
{
   char a[5][9] = { "every", "good", "bear", "deserves", "fudge" };
   char *p = &(a[2][1]);
   cout << "a: " << p << endl;
// semantic error: can't assign an array-at-a-time
   char b[5][9];
//   b[0] = "every"; b[1] = "good"; b[2] = "bear";
//   b[3] = "deserves"; b[4] = "fudge";
// warning, non-standard but works

}



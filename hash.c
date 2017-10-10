#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
//#include "lookup3.h"

// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/symt.c
/*int hash( char *s)
{
	// try park2331's hash but calloc instead
   register int h = 0;
   register char c;
   while (c = *s++) {
      h += c & 0377;
      h *= 37;
      }
   if (h < 0) h = -h;
   return h % 8741       ; // does 10007 fix it?
}*/

/* hash function */
// adapted from https://github.com/park2331/compiler/blob/master/tomorrow/hasht.c
unsigned long sdbm (char *str) {
  unsigned long hash = 0;
  int c;
  while (c = *str++) {
    hash = c + (hash << 6) + (hash << 16) - hash;
  }
  return hash;                                                                 
};

// adapted from https://github.com/park2331/compiler/blob/master/tomorrow/hasht.c
int get_key(char *str)
{
	unsigned long key; 
	key = sdbm(str) % 10000; 
	return key; 
}

// adapted from https://stackoverflow.com/questions/14409466/simple-hash-functions
int adler32(const char *buf, int buflength) {
     const char *buffer = (const char*)buf;

	 int n; 
     int s1 = 1;
     int s2 = 0;

     for (n = 0; n < buflength; n++) {
        s1 = (s1 + buffer[n]) % 65521;
        s2 = (s2 + s1) % 65521;
     }     
     return (s2 << 16) | s1;
}
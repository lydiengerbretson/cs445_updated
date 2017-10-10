#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
//#include "lookup3.h"

// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/symt.c
int hash( char *s)
{
	// try park2331's hash but calloc instead
   register int h = 0;
   register char c;
   while (c = *s++) {
      h += c & 0377;
      h *= 37;
      }
   if (h < 0) h = -h;
   return h % 10007       ; // does 10007 fix it?
}

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




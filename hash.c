#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"

// adapted from http://www2.cs.uidaho.edu/~jeffery/courses/445/symt.c
int hash( char *s)
{
   register int h = 0;
   register char c;
   while (c = *s++) {
      h += c & 0377;
      h *= 37;
      }
   if (h < 0) h = -h;
   return h % 8741       ; // does 10007 fix it?
}

// adapted from https://github.com/park2331/compiler/blob/master/tomorrow/hasht.c
int get_key(char *str)
{
	unsigned long key; 
	key = hash(str); 
	return key; 
}
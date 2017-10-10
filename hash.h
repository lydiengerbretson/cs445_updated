/* hash.h                                                                                                          
 * Interface for a hash table implementation.                                                                           
 */
#ifndef HASH_H
#define HASH_H

/* Hash function */

int hash (char*);
int get_key (char*);
int adler32(const char *buf, int buflength); 
unsigned long sdbm (char *str); 

#endif /* HASHT_H */
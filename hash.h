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
unsigned fnv_hash_1a_32 ( char *key, int len ); 
unsigned long hash_test(unsigned char *str); 

#endif /* HASHT_H */
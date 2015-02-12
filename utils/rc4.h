#ifndef _RC4_H
#define _RC4_H

 /* rc4.h */
typedef struct rc4_key
{      
   unsigned char state[256];       
   unsigned char x;        
   unsigned char y;
} rc4_key;
void prepare_key(const char *key_str,int key_str_len, rc4_key *key);
void rc4(unsigned char *buffer, int buffer_len, rc4_key * key);

#endif

#ifndef _CRYPTO_H
#define _CRYPTO_H

typedef unsigned char byte;
typedef struct doublechar {
    char a;
    char b;
} doublechar;

char bytetobase(byte in);
char bitstohex(byte in);
byte hextobits(char hex);
char* xorstrings(char input1[], char input2[]);
doublechar asciitohex(char in);
char hextoascii(doublechar in);
byte doublechartobyte(doublechar in);

#endif

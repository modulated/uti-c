#include "crypto.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

char const BASE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
char const HEX[] = "0123456789abcdef";

doublechar asciitohex(char in) {
	byte hexa = (unsigned char)in >> 4;
	byte hexb = (unsigned char)in & 0x0F;
		 
	
	doublechar out;
	out.a = bitstohex(hexa);
	out.b = bitstohex(hexb);
	
	return out;
}

char hextoascii(doublechar in) {
	byte out;
	out = (in.a << 4) + in.b;
	
	return out;
}

byte doublechartobyte(doublechar in) {
	return (hextobits(in.a)<<4) + hextobits(in.b);
}

byte hextobits(char hex) {
	
	for (int i = 0; i < 17; i++) {
		if ( (char)tolower(hex) == HEX[i]) {
			return i;
		}
	}
	printf("HexToBits ERROR");
	exit(1);
	return 0x0;
}

char bitstohex(byte in) {
	if (in < 16) {
		return HEX[in];
	}
	printf("BitsToHex ERROR");
	exit(1);
	return HEX[0]; 
}

char bytetobase(byte in) {

	if (in > strlen(BASE)) {
		printf("BytesToBase ERROR");
		exit(1);
		return 1;
		
	}
	
	return BASE[in];
}

char* xorstrings(char input1[], char input2[]) {
	
	int length1 = strlen(input1);
	
	int length2 = strlen(input2);
	
	
	int length = (length1 > length2) ? length1 : length2;
	
	
	if (length1 == 1) {
		char repeat = input1[0];
		for (int i = 1; i < length; i++) {
			input1[i] = repeat;
		}
		//printf("Repeat: %s\n",input1);
	}
	
	if (length2 == 1) {
		char repeat = input2[0];
		for (int i = 1; i < length; i++) {
			input2[i] = repeat;
		}
		//printf("Repeat: %s\n",input2);
	}
	
	char* out = calloc(length, sizeof(char));
	int counter = 0;
	
	for(int i = 0; i < length; i++) {
		
		byte a = hextobits(input1[i]);
		byte b = hextobits(input2[i]);
		byte c = 0xF & (a^b);
		
		// printf("Bit A: %d\n",a);
		// printf("Bit B: %d\n",b);
		// printf("A^B: %d\n",c);
		
		char hexval = bitstohex(c);
		
		//printf("Hex: %c\n",hexval);
		
		out[counter++] = hexval;
		
	}
	
	return out;
}

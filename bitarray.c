void bitflip(unsigned long in, unsigned long out[]) {
	// can potentially optimise modulo to rounding
	unsigned long mod = in & 7;
	unsigned long bitindex = (in - mod)/8;
	out[bitindex] = out[bitindex] ^ 0x1<<(mod-1);
}

unsigned long bitread(unsigned long in, unsigned long out[]) {
	unsigned long mod = in & 7;
	unsigned long bitindex = (in - mod)/8;	
	
	return (out[bitindex] >> (mod-1) ) & 0x1;
}

void biton(unsigned long in, unsigned long out[]) {
	// can potentially optimise modulo to rounding
	unsigned long mod = in & 7;
	unsigned long bitindex = (in - mod)/8;
	out[bitindex] = out[bitindex] | 0x1<<(mod-1);
}

void bitoff(unsigned long in, unsigned long out[]) {
	// can potentially optimise modulo to rounding
	unsigned long mod = in & 7;
	unsigned long bitindex = (in - mod)/8;
	out[bitindex] = out[bitindex] & !0x1<<(mod-1);
}
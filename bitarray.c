

void bitflip(int in, unsigned out[]) {
	// can potentially optimise modulo to rounding
	int mod = in%8;
	int bitindex = (in - mod)/8;
	out[bitindex] = out[bitindex] ^ 0x1<<(mod-1);
}

int bitread(int in, unsigned out[]) {
	int mod = in%8;
	int bitindex = (in - mod)/8;	
	
	return (out[bitindex] >> (mod-1) ) & 0x1;
}

void biton(int in, unsigned out[]) {
	// can potentially optimise modulo to rounding
	int mod = in%8;
	int bitindex = (in - mod)/8;
	out[bitindex] = out[bitindex] | 0x1<<(mod-1);
}

void bitoff(int in, unsigned out[]) {
	// can potentially optimise modulo to rounding
	int mod = in%8;
	int bitindex = (in - mod)/8;
	out[bitindex] = out[bitindex] & !0x1<<(mod-1);
}
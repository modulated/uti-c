# Bitarray Module

## Functions

### bitarray_create
`struct bitarray bitarray_create(int length)`

Creates a bitarray of size *length* bits and returns the bitarray object. Returns NULL if **not** successful.

### bitarray_resize
`int bitarray_resize(struct bitarray *ba, int length)`

Resizes the existing bitarray to *length* bits. Currently does not retain contents (NEED TO FIX). Returns 0 if successful.


### bitarray_destroy
`int bitarray_destroy(struct bitarray *ba)`

Destroys a bitarray once you are finished using it (Otherwise you get a memory leak). Note that it must be a pointer to a bitarray struct. Returns 0 if completed without error.

### bitarray_read
`int bitarray_read(struct bitarray *ba, int position)`

Reads and returns either 0 or 1 from the bitarray at the given position (bit).

### bitarray_on
`int bitarray_on(struct bitarray *ba, int position)`

Sets bitarray to 1 at the given position (bit). Returns 0 if successful.

### bitarray_off
`int bitarray_off(struct bitarray *ba, int position)`

Sets bitarray to 0 at the given position (bit). Returns 0 if successful.

### bitarray_flip
`int bitarray_flip(struct bitarray *ba, int position)`

Flips the bit at the given position. Returns 0 if successful.

### bitarray_resize
`int bitarray_on(struct bitarray *ba, int position)`

Sets bitarray to 1 at the given position (bit). Returns 0 if successful.

## Example

```c
#include <stdio.h>
#include “bitarray.h”

int main() {
	struct bitarray mybitarray = bitarray_create(16);
	bitarray_flip(&mybitarray, 1);	/* sets bit 1 to 1 */
	printf(“Bit 1 = %d”,bitarray_read(&mybitarray, 1));
	bitarray_destroy(&mybitarray);
}
```
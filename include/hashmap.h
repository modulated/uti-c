#ifndef _HASHMAP_H
#define _HASHMAP_H

// ERROR values
#define HASHMAP_MISSING -3  /* No such element */
#define HASHMAP_FULL    -2 	/* Hashmap is full */
#define HASHMAP_OMEM    -1  /* Out of Memory */
#define HASHMAP_OK       0  /* OK */

/* Pointer to arbitrary data */
typedef void* any_t; 

/* Pointer to function that can take two arguments
 * and return an int as a status code.
 */
typedef int (*FPany)(any_t, any_t); 

/* Pointer to abstracted map structure. */
typedef any_t map_t;

/* Return empty hashmap. Returns NULL if empty. */
extern map_t hashmap_new();

/* Iterativly call *fp* with args (item, data) for each
 * element *data* in hashmap. Function must return status 
 * code. Anything other than MAP_OK terminates traversal.
 */
extern int hashmap_itr(map_t map, FPany fp, any_t item);

/* Add element to hashmap. Returns MAP_OK or MAP_OMEM. */
extern int hashmap_add(map_t map, char* key, any_t value);

/* Gets element from hashmap. Returns MAP_OK or MAP_MISSING. */
extern int hashmap_get(map_t map, char* key, any_t* arg);

/* Delete element from hashmap. Returns MAP_OK or MAP_MISSING. */
extern int hashmap_del(map_t map, char* key);

extern int hashmap_len(map_t map);

/* Frees memory associated with hashmap. */
extern void hashmap_free(map_t map);

/** Fills buffer with error string. Buffer should be >16 bytes. */
extern void hashmap_error(int error, char* buffer, unsigned long buffer_len);

#endif

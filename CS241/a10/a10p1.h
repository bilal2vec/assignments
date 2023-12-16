#ifndef A10P1
#define A10P1
#include <cstdint>

/*
 * Returns the starting address of a block of memory ("the arena") that may be used for dynamic allocation. 
 * This function does NOT allocate memory itself. The arena is allocated once, and calling arena() just returns
 * the starting address of the arena. Repeated calls to arena() will not "re-allocate" a new block of memory.
 * Initially, the size of the arena (in bytes) is stored in the first word. That is, the expressions *arena() 
 * and arena()[0] will return the size of the arena, which will not be less than 2^23 or greater than 2^31. 
 * You may assume the size is a multiple of 8 (since each word is 8 bytes).
 * You are free to modify or overwrite words in the arena, including the first word. Calls to arena() will
 * not reset or change the data you have stored in the arena.
 */
int64_t *arena();

#endif

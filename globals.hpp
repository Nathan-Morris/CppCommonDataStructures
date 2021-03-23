#ifndef NULL
	#define NULL 0
#endif

#pragma once

static inline void nm_memset(void* memaddr, unsigned int len, char value) {
	while (len--)
		((char*)memaddr)[len] = value;
}

static inline void nm_memcpy(void* dest, void* src, unsigned int len) {
	while (len--)
		((char*)dest)[len] = ((char*)src)[len];
}

static inline int nm_memcmp(void* mem1, void* mem2, unsigned int len) {
	while (len--) {
		if (((char*)mem1)[len] != ((char*)mem2)[len]) {
			return (
				((unsigned char*)mem1)[len] > ((unsigned char*)mem2)[len] ? 1 : -1
			);
		}
	}
	return 0;
}
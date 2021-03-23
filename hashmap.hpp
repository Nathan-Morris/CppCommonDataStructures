#include "dynarray.hpp"

/*
* Very much so a work in progress
* will be coming back to this later
* it works but I dont think it works 
* well
*/

#pragma once

typedef unsigned long(*HashCallback)(void*, unsigned int);

// see:
// http://www.cse.yorku.ca/~oz/hash.html
static inline unsigned long djb2Hash(void* _ptr, unsigned int len) {
	char* ptr = (char*)_ptr;

	unsigned long hash = 5381;
	char c;

	for (unsigned int i = 0, c = ptr[i]; i < len; i++) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

/*
an associative array.
thought about using a similair naming
scheme to "dynamic array -> dynarray",
I however dont think "assarray" would 
be well recieved, It uses a hash table
ish thing. and ive used java, so its 
now named hashmap
*/
template<typename _Key, typename _Val>
class hashmap {
private:
	HashCallback toHashCallback;
	dynarray<_Val> buckets;
	unsigned int bucketsSz;
	// possibly have an already zeroed out element

public:
	hashmap(unsigned int bucketsSz = 15, HashCallback hashFuncCb = djb2Hash) {
		this->toHashCallback = hashFuncCb;
		this->bucketsSz = bucketsSz;
		char* zeroedOut = new char[sizeof(_Val)];
		nm_memset(zeroedOut, sizeof(_Val), 0);
		while (bucketsSz--) {
			this->buckets.push(*(_Val*)zeroedOut);
		}
		delete[] zeroedOut;
	}

	hashmap(const hashmap<_Key, _Val>& ref) {
		this->toHashCallback = ref.toHashCallback;
		this->bucketsSz = ref.bucketsSz;
		this->buckets = ref.buckets;
	}

	~hashmap() { }

	void put(_Key key, _Val val) {
		unsigned long hashed = this->toHashCallback(&key, sizeof(_Key));
		unsigned int index = hashed & (this->bucketsSz - 1);
		this->buckets[index] = val;
	}

	_Val& get(_Key key) {
		unsigned long hashed = this->toHashCallback(&key, sizeof(_Key));
		unsigned int index = hashed & (this->bucketsSz - 1);
		return this->buckets[index];
	}

	void remove(_Key key) {
		char* zeroedOut = new char[sizeof(_Val)];
		nm_memset(zeroedOut, sizeof(_Val), 0);
		unsigned long hashed = this->toHashCallback(&key, sizeof(_Key));
		unsigned int index = hashed & (this->bucketsSz - 1);
		this->buckets[index] = *(_Val*)zeroedOut;
		delete[] zeroedOut;
	}
};
#include "globals.hpp"

#pragma once

template<typename _Type>
class dynarray {
private:
	_Type* arr = NULL;
	unsigned int arrSz = 0;

public:
	dynarray() : dynarray(0) { }

	dynarray(unsigned int len) {
		if (!len)
			return;
		this->arr = new _Type[len];
		this->arrSz = len;
	}

	dynarray(_Type t, unsigned int count) {
		if (!count)
			return;
		this->arr = new _Type[count];
		this->arrSz = count;

		while (count--) {
			this->arr[count] = t;
		}
	}

	dynarray(_Type* ptr, unsigned int sz) {
		if (!sz || !ptr)
			return;

		this->arrSz = sz;
		this->arr = new _Type[sz];

		while (sz--) {
			this->arr[sz] = ptr[sz];
		}
	}

	dynarray(const dynarray<_Type>& ref) {
		if (!ref.arr || !ref.arrSz)
			return;

		// might be able to do this better, maybe not.
		if (!this->arr) {
			this->arr = new _Type[ref.arrSz];
			this->arrSz = ref.arrSz;
		}
		else if (this->arrSz != ref.arrSz) {
			delete[] this->arr;
			this->arr = new _Type[ref.arrSz];
			this->arrSz = ref.arrSz;
		}

		for (unsigned int i = 0; i < this->arrSz; i++) {
			this->arr[i] = ref.arr[i];
		}
	}

	~dynarray() {
		if (this->arr)
			delete[] this->arr;
	}

	_Type& getAt(unsigned int i) const {
		return this->arr[i];
	}

	unsigned int getSize() const {
		return this->arrSz;
	}

	//
	//

	void push(_Type t) {
		if (!this->arr) {
			this->arr = new _Type[this->arrSz + 1];
			this->arr[this->arrSz++] = t;
			return;
		}

		unsigned int newSz = this->arrSz + 1;
		_Type* newArr = new _Type[newSz];
		newArr[this->arrSz] = t;

		while (this->arrSz--) {
			newArr[this->arrSz] = this->arr[this->arrSz];
		}

		this->arrSz = newSz;
		delete[] this->arr;
		this->arr = newArr;
	}

	void push_front(_Type t) {
		this->insert(t, 0);
	}

	void pop() {
		if (!this->arr || !this->arrSz)
			return;

		unsigned int newSz = this->arrSz - 1;
		_Type* newArr = new _Type[newSz];
		
		while (newSz--) {
			newArr[newSz] = this->arr[newSz];
		}

		this->arrSz--;
		delete[] this->arr;
		this->arr = newArr;
	}

	void insert(_Type t, unsigned int i) {
		if (!this->arr) {
			this->arr = new _Type[this->arrSz + 1];
			this->arr[this->arrSz++] = t;
			return;
		}

		if (i >= this->arrSz) {
			i = 0;
		}

		unsigned int newSz = this->arrSz + 1;
		_Type* newArr = new _Type[newSz];
		newArr[i] = t;

		for (unsigned int ii = 0; ii != i; ii++) {
			newArr[ii] = this->arr[ii];
		}

		for (unsigned int ii = i + 1; ii < newSz; ii++) {
			newArr[ii] = this->arr[ii - 1];
		}

		this->arrSz = newSz;
		delete[] this->arr;
		this->arr = newArr;
	}

	unsigned int find(_Type t, bool useByteComparison = false) {
		if (useByteComparison) {
			for (unsigned int i = 0; i < this->arrSz; i++)
				if (!nm_memcmp(&this->arr[i], &t, sizeof(_Type)))
					return i;
		}
		else {
			for (unsigned int i = 0; i < this->arrSz; i++)
				if (this->arr[i] == t)
					return i;
		}
		return 0xFFFFFFFF; // uint max
	}

	// removes { beg -> end } indices
	void remove(unsigned int beg, unsigned int end = 0xFFFFFFFF) {
		unsigned int szChange = 1;

		if (beg < this->arrSz || beg > end) {
			beg = 0;
		}

		if (end != 0xFFFFFFFF && end < this->arrSz) {
			szChange += end - beg;
		}

		unsigned newSz = this->arrSz - szChange;
		_Type* newArr = new _Type[newSz];

		for (unsigned int i = 0; i < beg; i++) {
			newArr[i] = this->arr[i];
		}

		for (unsigned int i = beg; i < newSz; i++) {
			newArr[i] = this->arr[i + szChange];
		}

		this->arrSz = newSz;
		delete[] this->arr;
		this->arr = newArr;
	}

	//
	//

	dynarray<_Type>& operator=(const dynarray<_Type>& ref) {
		if (!ref.arr || !ref.arrSz)
			return *this;
		// might be able to do this better, maybe not.
		if (!this->arr) {
			this->arr = new _Type[ref.arrSz];
			this->arrSz = ref.arrSz;
		}
		else if (this->arrSz != ref.arrSz) {
			delete[] this->arr;
			this->arr = new _Type[ref.arrSz];
			this->arrSz = ref.arrSz;
		}
		for (unsigned int i = 0; i < this->arrSz; i++) {
			this->arr[i] = ref.arr[i];
		}
		return *this;
	}

	_Type& operator[](unsigned int i) {
		return this->getAt(i);
	}
};
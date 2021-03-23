#include "globals.hpp"

#pragma once

template<typename _Type>
struct linkedlist_node {
	_Type value;
	linkedlist_node* next = NULL;
};

template<typename _Type>
class linkedlist {
private:
	linkedlist_node<_Type>* first = NULL;
	unsigned int sz = 0;

	void clean() {
		if (!this->sz || !this->first)
			return;

		linkedlist_node<_Type>* current = this->first, * next;
		while (current) {
			next = current->next;
			delete current;
			current = next;
		}
	}

	linkedlist_node<_Type>* getNodeAt(unsigned int i) const {
		linkedlist_node<_Type>* ptr = this->first;
		while (i--)
			ptr = ptr->next;
		return ptr;
	}

	void cleanRemove(unsigned int i) {
		if (i == 0) {
			linkedlist_node<_Type>* ptr = this->first->next;
			delete this->first;
			this->first = ptr;
		}
		else if (i == this->sz - 1) {
			delete getNodeAt(this->sz - 1);
			getNodeAt(this->sz - 2)->next = NULL;
		}
		else {
			linkedlist_node<_Type>* Lptr = getNodeAt(i - 1);
			linkedlist_node<_Type>* ptr = getNodeAt(i);
			Lptr->next = ptr->next;
			delete ptr;
		}
		this->sz--;
	}

public:

	linkedlist() {}

	linkedlist(unsigned int len) {
		if (!len)
			return;
		char* randHeapData = new char[sizeof(_Type)];
		while (len--) {
			this->push(*(_Type*)randHeapData);
		}
		delete[] randHeapData;
	}

	linkedlist(_Type t, unsigned int len) {
		while (len--) {
			this->push(t);
		}
	}

	linkedlist(_Type* ptr, unsigned int len) {
		for (unsigned int i = 0; i != len; i++) {
			this->push(ptr[i]);
		}
	}

	// expects at least one node
	_Type& getAt(unsigned int i) {
		if (i >= this->sz)
			i = 0;
		return this->getNodeAt(i)->value;
	}

	linkedlist(const linkedlist<_Type>& ref) {
		for (unsigned int i = 0; i < ref.getSize(); i++) {
			this->push(ref.getAt(i));
		}
	}

	~linkedlist() {
		this->clean();
	}

	unsigned int getSize() const {
		return this->sz;
	}

	void push(_Type t) {
		this->sz++;
		if (!this->first) {
			this->first = new linkedlist_node<_Type>;
			this->first->value = t;
			return;
		}
		linkedlist_node<_Type>* last = getNodeAt(this->sz - 2);
		last->next = new linkedlist_node<_Type>;
		last->next->value = t;
	}

	void pop() {
		if (!this->sz || !this->first)
			return;

		this->sz--;

		if (!this->sz) {
			delete this->first;
			this->first = NULL;
			return;
		}

		linkedlist_node<_Type>* secondToLast = getNodeAt(this->sz - 1);
		delete secondToLast->next;
		secondToLast->next = NULL;
	}

	void push_front(_Type t) {
		this->insert(t, 0);
	}

	void insert(_Type t, unsigned int i) {
		if (!this->first || i >= this->sz) {
			return;
		}

		linkedlist_node<_Type>* newNode = new linkedlist_node<_Type>;
		newNode->value = t;
		
		if (i == 0) {
			newNode->next = this->first;
			this->first = newNode;
		}
		else if (i == this->sz - 1) {
			getNodeAt(this->sz - 1)->next = newNode;
		}
		else {
			linkedlist_node<_Type>* atIndex = this->getNodeAt(i);
			linkedlist_node<_Type>* beforeIndex = this->getNodeAt(i - 1);

			beforeIndex->next = newNode;
			newNode->next = atIndex;
		}

		this->sz++;
	}

	unsigned int find(_Type t, bool useByteComparison = false) {
		if (useByteComparison) {
			for (unsigned int i = 0; i < this->arrSz; i++)
				if (!nm_memcmp(&this->getNodeAt(i)->value, &t, sizeof(_Type)))
					return i;
		}
		else {
			for (unsigned int i = 0; i < this->arrSz; i++)
				if (this->getNodeAt(i)->value == t)
					return i;
		}
		return 0xFFFFFFFF; // uint max
	}

	void remove(unsigned int beg, unsigned int end = 0xFFFFFFFF) {
		unsigned int szChange = 1;

		if (beg < this->sz || beg > end) {
			beg = 0;
		}

		if (end != 0xFFFFFFFF && end < this->sz) {
			szChange += end - beg;
		}

		while (szChange--) {
			this->cleanRemove(beg);
		}
	}

	linkedlist_node<_Type>* getInitial() const {
		return this->first;
	}

	//
	//
	//
	//
	//
	linkedlist<_Type>& operator=(linkedlist<_Type>& ref) {
		for (unsigned int i = 0; i < ref.getSize(); i++) {
			this->push(ref.getAt(i));
		}
		return *this;
	}


	_Type& operator[](unsigned int i) {
		return this->getAt(i);
	}
};
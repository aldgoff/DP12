/*
 * swpc.h
 *
 *  Created on: Aug 28, 2015
 *      Author: aldgoff
 *
 * Description:
 *   Place to try out code for my SWPC tutorial on design patterns..
 *
 * Titles:
 *   Design Patterns Accelerated
 */

#ifndef SWPC_H_
#define SWPC_H_

namespace swpc {

char str[80+1];

namespace common_code {

bool linkedList = false;
bool mustBeStable = false;
bool keysWellBehaved = false;

}

namespace legacy {

using namespace common_code;

unsigned shiftLeft(const unsigned& reg, unsigned bits) {
	unsigned shiftedReg = reg;

	if(bits == 0) shiftedReg = reg << 0;
	else if(bits == 1) shiftedReg = reg << 1;
	else if(bits == 2) shiftedReg = reg << 2;
	else if(bits == 3) shiftedReg = reg << 3;
	else if(bits == 4) shiftedReg = reg << 4;
	else if(bits == 5) shiftedReg = reg << 5;
	else if(bits == 6) shiftedReg = reg << 6;
	else if(bits == 7) shiftedReg = reg << 7;
	else if(bits == 8) shiftedReg = reg << 8;
	else if(bits == 9) shiftedReg = reg << 9;
	else if(bits == 10) shiftedReg = reg << 10;
	else if(bits == 11) shiftedReg = reg << 11;
	else if(bits == 12) shiftedReg = reg << 12;
	else if(bits == 13) shiftedReg = reg << 13;
	else if(bits == 14) shiftedReg = reg << 14;
	else if(bits == 15) shiftedReg = reg << 15;
	else {
		cout << "  OOPS!\n";
	}

	return shiftedReg;
}

void insertionSort(int* data, size_t size) { cout << "  insertion sort\n"; }
void bubbleSort(int* data, size_t size) { cout << "  bubble sort\n"; }
void quickSort(int* data, size_t size) { cout << "  quick sort\n";}
void heapSort(int* data, size_t size) { cout << "  heap sort\n";}
void mergeSort(int* data, size_t size) { cout << "  merge sort\n";}
void radixSort(int* data, size_t size) { cout << "  radix sort\n";}

void clientCode(int* data, size_t size) {
	if(linkedList)		 		insertionSort(data, size);
	else if(mustBeStable)		heapSort(data, size);
	else if(keysWellBehaved)	radixSort(data, size);
	else if(size < 10)			bubbleSort(data, size);
	else if(size < 100000)		quickSort(data, size);
	else if(size < 100000000)	mergeSort(data, size);
	else {
		cout << "  OOPS!\n";
	}
}

void demo() {
	cout << "<< legacy >>\n";
	unsigned reg = 0x00FF;

	sprintf(str, "  %04X - %04X\n", reg, shiftLeft(reg, 4));
	cout << str;

	int data[7] = { 5, 2, 3, 7, 1, 6, 4 };

	size_t sizes[] = { 7, 100, 200000 };
	for(size_t i=0; i<sizeof(sizes)/sizeof(*sizes); i++) {
		clientCode(data, sizes[i]);
	}

	cout << endl;
}

}

namespace design_pattern {

using namespace common_code;

unsigned shiftLeft(const unsigned& reg, unsigned bits) {
	return(reg << bits);
}

class Sort {
public: virtual ~Sort() {}
public:
	virtual void algorithm(int* data, size_t size) {
		cout << "  default sort\n";
	}
public:
	static Sort* decisionLogic(size_t size);
};
class InsertionSort : public Sort {
public:
	void algorithm(int* data, size_t size) { cout << "  insertion sort\n"; }
};
class BubbleSort : public Sort {
public:
	void algorithm(int* data, size_t size) { cout << "  bubble sort\n"; }
};
class QuickSort : public Sort {
public:
	void algorithm(int* data, size_t size) { cout << "  quick sort\n"; }
};
class HeapSort : public Sort {
public:
	void algorithm(int* data, size_t size) { cout << "  heap sort\n"; }
};
class MergeSort : public Sort {
public:
	void algorithm(int* data, size_t size) { cout << "  merge sort\n"; }
};
class RadixSort : public Sort {
public:
	void algorithm(int* data, size_t size) { cout << "  radix sort\n"; }
};
// Seam point - add another sort algorithm.

Sort* Sort::decisionLogic(size_t size) {
	if(mustBeStable)			return new HeapSort;
	else if(keysWellBehaved)	return new RadixSort;
	else if(size < 10)			return new BubbleSort;
	else if(size < 100000)		return new QuickSort;
	else if(size >= 100000) 	return new MergeSort;
	else {
		cout << "  OOPS!";
		return new Sort;
	}
}

void clientCode(int* data, size_t size, Sort* sort) {
	sort->algorithm(data, size);
}

void demo() {
	cout << "<< design_patterns >>\n";
	int reg = 0x00FF;

	sprintf(str, "  %04X - %04X\n", reg, shiftLeft(reg, 4));
	cout << str;

	int data[7] = { 5, 2, 3, 7, 1, 6, 4 };

	size_t sizes[] = { 7, 100, 200000 };
	for(size_t i=0; i<sizeof(sizes)/sizeof(*sizes); i++) {
		clientCode(data, sizes[i], Sort::decisionLogic(sizes[i]));
	}

	cout << endl;
}

} // design_pattern

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "swpc") {}
	virtual ~Observer() { DTOR("~SWPCObserver ", Architecture); }
public:
	virtual void refactoring() {
		cout << seqNo << ") << swpc::refactoring >>\n";
		swpc::legacy::demo();
		swpc::design_pattern::demo();
	}
};

} // swpc

#endif /* SWPC_H_ */

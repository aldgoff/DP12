/*
 * macros.h
 *
 *  Created on: Sep 1, 2015
 *      Author: aldgoff
 */

#ifndef MACROS_H_
#define MACROS_H_


const unsigned flags = 0x0E;	// Dtor instrumentation controlled by bit flags.
#define DTOR(x, flag) if(flag & flags) { cout << x; }
enum DtorFlags {
	Architecture	= 0x01,
	Lecture			= 0x02,
	Homework		= 0x0C,
	Problem			= 0x04,
	Solution		= 0x08,
	Final			= 0x10,
};

#define COUNT(x) (sizeof(x)/sizeof(*x))


#endif /* MACROS_H_ */

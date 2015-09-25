/*
 * decoratorCommon.h
 *
 *  Created on: Sep 24, 2015
 *      Author: aldgoff
 */

#ifndef DECORATORCOMMON_H_
#define DECORATORCOMMON_H_

namespace common {

class Car {
protected:
	string str;
public:
	Car(string str="Undefined car") : str(str) {}
	virtual ~Car() {
		DTOR("~Car\n", Homework);
	}
public:
	virtual string getDesc() { return str; }
	virtual double getCost()=0;
};

} // common

#endif /* DECORATORCOMMON_H_ */

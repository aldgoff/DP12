/*
 * decorator.h
 *
 *  Created on: Sep 24, 2015
 *      Author: aldgoff
 *
 * Varies: If the options are varying, use the Decorator pattern.
 *
 * Desc: Dynamically attach additional responsibilities to an object.
 *
 * Category: Structural
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Decorator
 *  	http://www.dofactory.com/net/decorator-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter17
 *  	http://sourcemaking.com/design_patterns/decorator
 */

#ifndef PROBLEMS_DECORATOR_H_
#define PROBLEMS_DECORATOR_H_

/* Consider options for a car.
 * One might subclass a base class, Car, for each set of options, but
 * then the options code gets duplicated leading to scaling problems,
 * and the options are statically bound.
 * For the 4 classes and 9 options implied by the following OOP code,
 * rewrite with the Decorator pattern to
 *   1. eliminate the combinatorial explosion
 *   2. reduce duplication
 *   3. improve code locality
 *   4. allow for selection of options at runtime
 *   5. Extra points if you don't have to change the common::Car class.
 * Pay particular attention to the problem of memory leaks with this pattern.
 */

#include "../Problems/decoratorCommon.h"

namespace legacy {

using namespace common;

class RunAbout : public Car {
public:
	RunAbout() : Car("RunAbout") {}
	~RunAbout() {
		DTOR("  ~RunAbout ", Homework);
	}
public:
	string getDesc() {
		string desc = str + " with 4 wheels"
			+ ", 2 doors";
		return desc;
	}
	double getCost() {
		double cost = 12000.00
			+ 2000.00;
		return cost;
	}
};
class SUV : public Car {
public:
	SUV() : Car("SUV") {}
	~SUV() {
		DTOR("  ~SUV ", Homework);
	}
public:
	string getDesc() {
		string desc = str + " with 4 wheels"
			+ ", 4 doors"
			+ ", AC"
			+ ", automatic transmission";
		return desc;
	}
	double getCost() {
		double cost = 12000.00
			+ 4000.00
			+ 1500.00
			+ 3000.00;
		return cost;
	}
};

void demo(int seqNo) {
	Car* cars[] = { new RunAbout, new SUV };

	for(size_t i=0; i<COUNT(cars); i++) {
		cout << "  " << cars[i]->getDesc();
		cout << " costs $" << cars[i]->getCost() << ".\n";
	}
	cout << endl;

	for(size_t i=0; i<COUNT(cars); i++)
		delete cars[i];
	cout << endl;
}

} // legacy

namespace problem {

using namespace common;

class RunAbout : public Car {
public:
	RunAbout() : Car("RunAbout") {}
	~RunAbout() {
		DTOR("  ~RunAbout ", Homework);
	}
public:
	string getDesc() {
		string desc = str + " with 4 wheels"
			+ ", 2 doors";
		return desc;
	}
	double getCost() {
		double cost = 12000.00
			+ 2000.00;
		return cost;
	}
};
class SUV : public Car {
public:
	SUV() : Car("SUV") {}
	~SUV() {
		DTOR("  ~SUV ", Homework);
	}
public:
	string getDesc() {
		string desc = str + " with 4 wheels"
			+ ", 4 doors"
			+ ", AC"
			+ ", automatic transmission";
		return desc;
	}
	double getCost() {
		double cost = 12000.00
			+ 4000.00
			+ 1500.00
			+ 3000.00;
		return cost;
	}
};
class Status : public Car {
public:
	Status() : Car("Status") {}
	~Status() {
		DTOR("  ~Status ", Homework);
	}
public:
	string getDesc() {
		string desc = str + " with 4 wheels"
			+ ", 4 doors"
			+ ", AC"
			+ ", premium sound system"
			+ ", navigation"
			+ ", automatic transmission"
			// Seam: new options scales as the number of car models.
		;
		return desc;
	}
	double getCost() {
		double cost = 12000.00
			+ 4000.00
			+ 1500.00
			+ 1000.00
			+ 2000.00
			+ 3000.00
		;
		return cost;
	}
};
class Performance : public Car {
public:
	Performance() : Car("Performance") {}
	~Performance() {
		DTOR("  ~Performance ", Homework);
	}
public:
	string getDesc() {
		string desc = str + " with 4 wheels"
			+ ", 2 doors"
			+ ", AC"
			+ ", premium sound system"
			+ ", navigation"
			+ ", manual transmission"
			+ ", V8"
			+ ", super-charger"
		;
		return desc;
	}
	double getCost() {
		double cost = 12000.00
			+ 2000.00
			+ 1500.00
			+ 1000.00
			+ 2000.00
			+ 2500.00
			+ 6000.00
			+ 3000.00
		;
		return cost;
	}
};
// Seam point - add another set of options.

void demo(int seqNo) {
	Car* mine  = new RunAbout;
	Car* yours = new SUV;
	Car* hers  = new Status;
	Car* boss  = new Performance;

	Car* cars[] = { mine, yours, hers, boss };

	for(size_t i=0; i<COUNT(cars); i++) {
		cout << "  " << cars[i]->getDesc();
		cout << " costs $" << cars[i]->getCost() << ".\n";
	}
	cout << endl;

	for(size_t i=0; i<COUNT(cars); i++)
		delete cars[i];
	cout << endl;
}

} // problem

#endif /* PROBLEMS_DECORATOR_H_ */

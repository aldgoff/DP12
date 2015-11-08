/*
 * lambda.h
 *
 *  Created on: Nov 7, 2015
 *      Author: aldgoff
 *
 * Varies: lambda
 *
 * Thesis: variation that must be modeled by code is C++'s hackish
 * 		way of implementing a lambda calculus; which if implemented
 * 		in the language proper would make most design patterns mute.
 *
 * Category: whatever
 *
 */

#ifndef WIP_LAMBDA_H_
#define WIP_LAMBDA_H_

#include "../DP.h"

namespace lambda {

namespace lecture {

class State { // Variation that can be modeled by state.
public:
	int		value;
	string	desc;
};

class Lambda { // Variation that must be modeled by code (Strategy pattern).
protected:
	uint64_t		value;
	const string	desc;
public:
	Lambda(uint64_t value, string desc) : value(value), desc(desc) {}
	virtual ~Lambda() { DTOR(" ~Lambda\n", Lecture); }
public:
	virtual uint64_t getValue() { return 0; }
	virtual string getDesc() { return ""; }
};
class Arithmetical : public Lambda {
public:
	Arithmetical(uint64_t value) : Lambda(value, "Arithimetical") {}
	~Arithmetical() {
		DTOR("  ~Arithmetical", Lecture);
	}
public:
	uint64_t getValue() { return value += 1; }
};
class Geometrical : public Lambda {
public:
	Geometrical(uint64_t value) : Lambda(value, "Geometrical") {}
	~Geometrical() {
		DTOR("  ~Geometrical", Lecture);
	}
public:
	uint64_t getValue() { return value *= 2; }
};
class Exponential : public Lambda {
public:
	Exponential(uint64_t value) : Lambda(value, "Exponential") {}
	~Exponential() {
		DTOR("  ~Exponential", Lecture);
	}
public:
	uint64_t getValue() { return value = value*value; } // value ^= 2.
};
class Random : public Lambda {
public:
	Random(uint64_t value) : Lambda(value, "Random") {
		srand(value);
	}
	~Random() {
		DTOR("  ~Random", Lecture);
	}
public:
	uint64_t getValue() { return rand()%100; }
};
// Seam point - add another Lambda.

void clientCode(Lambda* lambda) { // Want independent of derived classes.
	cout << "  " << lambda->getDesc();
	for(size_t j=0; j<5; j++) {
		cout << " " << lambda->getValue();
	}
	cout << endl;
}

void demo() { // Test variations.
	cout << "Hello, lambda::lecture::demo().\n";

	Lambda* lambdas[] = { new Arithmetical(0), new Geometrical(1),
						  new Exponential(2), new Random(3) };
	for(size_t i=0; i<COUNT(lambdas); i++) {
		clientCode(lambdas[i]);
	}
	cout << endl;

	for(size_t i=0; i<COUNT(lambdas); i++) {
		delete lambdas[i];
	}
	cout << endl;

	cout << "Aloha, lambda::lecture::demo().\n";
}
#ifdef OUTPUT
Hello, lambda::lecture::demo().
   1 2 3 4 5
   2 4 8 16 32
   4 16 256 65536 4294967296
   48 96 94 91 31

  ~Arithmetical ~Lambda
  ~Geometrical ~Lambda
  ~Exponential ~Lambda
  ~Random ~Lambda

Aloha, lambda::lecture::demo().
#endif

namespace solution {

class Sandwich { // Decorator pattern when only state varies.
	Sandwich* decorator;
	string	desc;
	double	cost;
public:
	Sandwich(string desc, double cost, Sandwich* decorator=0)
		: decorator(decorator)
		, desc(desc), cost(cost)
	{}
	~Sandwich() {
		delete decorator;
		DTOR("  ~Sandwich", Lecture);
	}
public:
	string getDesc() {
		if(decorator)	return decorator->getDesc() + " + " + desc;
		else			return desc;
	}
	double getCost() {
		if(decorator)	return decorator->getCost() + cost;
		else			return cost;
	}
};

void demo() {
	cout << "Hello, lambda::lecture::solution::demo().\n";

	Sandwich *sandwich = new Sandwich("WholeGrain", 2.0);
	cout << "  " << sandwich->getDesc() << " sandwich: costs $" << sandwich->getCost() << endl;

	// Double Cheese Burger :)
	sandwich = new Sandwich("Cheese", 1.5, sandwich);
	sandwich = new Sandwich("Cheese", 1.5, sandwich);
	cout << "  " << sandwich->getDesc() << " sandwich: costs $" << sandwich->getCost() << endl;

	sandwich = new Sandwich("Veggie", 1.0, sandwich);
	cout << "  " << sandwich->getDesc() << " sandwich: costs $" << sandwich->getCost() << endl;

	delete sandwich; cout << endl;

	cout << "Aloha, lambda::lecture::solution::demo().\n";
}

} // solution

} // lecture

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "lambda") {}
	virtual ~Observer() { DTOR("~lambda::Observer ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << lambda::lecture::legacy >>\n";
//		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << lambda::lecture::problem >>\n";
//		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << lambda::lecture::solution >>\n";
		lecture::solution::demo();
		lecture::demo();
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << lambda::homework::legacy >>\n";
//		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << lambda::homework::problem >>\n";
//		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << lambda::homework::solution >>\n";
//		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << lambda::skeleton >>\n";
//		skeleton::demo();
	}
	virtual void recognition() {
		cout << seqNo << ") << lambda::recognition >>\n";
//		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << lambda::refactoring >>\n";
//		refactoring::bad::demo();
//		refactoring::good::demo();
	}
};

} // lambda

#endif /* WIP_LAMBDA_H_ */

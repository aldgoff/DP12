/*
 * factoryMethod.h
 *
 *  Created on: May 23, 2015
 *      Author: aldgoff
 *
 * Varies: If the classes are varying, use the Factory Method pattern.
 *
 * Desc: Provide a method to create derived classes w/o coupling them to the client.
 *
 * Category: Creational
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Factory
 *  	http://www.dofactory.com/net/factoryMethod-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter7
 *  	http://sourcemaking.com/design_patterns/factoryMethod
 */

#ifndef LECTURES_FACTORYMETHOD_
#define LECTURES_FACTORYMETHOD_

#include "../DP.h"

namespace factory_method {	// Command line: factoryMethod [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << factory_method::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << factory_method::lecture::problem::demo() >>\n";
}

}

namespace solution {

class FactoryMethod {	// If the classes are varying...
public:
	FactoryMethod() {}
	virtual ~FactoryMethod() { DTOR("~FactoryMethodObserver\n", Architecture); }
public:
	virtual void run() {}
public:
	static FactoryMethod* decisionLogic(const string& criteria);
};
class Derived : public FactoryMethod {
public:
	Derived() {}
	virtual ~Derived() { DTOR("~Derived ", Architecture); }
public:
	void run() {}
};
// Seam point - add another Derived.

FactoryMethod* FactoryMethod::decisionLogic(const string& criteria) {
	if(		criteria == "whatever")	return new FactoryMethod;
	else if(criteria == "whatever")	return new FactoryMethod;
	// Seam point - add another FactoryMethod.

	else {
		return new FactoryMethod;
	}
}

void demo(int seqNo) {
	cout << seqNo << ") << factory_method::lecture::solution::demo() >>\n";
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		FactoryMethod* derived = FactoryMethod::decisionLogic(criteria[i]);
		derived->run();
	}
	cout << endl;
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << factory_method::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << factory_method::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << factory_method::homework::solution::demo() >>\n";
}

}

} // homework

namespace skeleton {

class FactoryMethod {	// If the classes are varying...
public: virtual ~FactoryMethod() {}
public:
	virtual void run() { cout << "  Oops!\n"; }
public:
	static FactoryMethod* makeObject(const string& criteria);
};
class Type1 : public FactoryMethod {
public:
	void run() { cout << "  Type1\n"; }
};
class Type2 : public FactoryMethod {
public:
	void run() { cout << "  Type2\n"; }
};
class Type3 : public FactoryMethod {
public:
	void run() { cout << "  Type3\n"; }
};
// Seam point - add another class.

FactoryMethod* FactoryMethod::makeObject(const string& criteria) {
	if(		criteria == "Type1")	return new Type1;
	else if(criteria == "Type2")	return new Type2;
	else if(criteria == "Type3")	return new Type3;

	else {
		return new FactoryMethod;	// Opts: null, exception, base, default, ABC.
	}
}

void demo(int seqNo) {	// Decouples client from creation.
	string criteria[] = { "Type1", "Type2", "Type3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		FactoryMethod* type = FactoryMethod::makeObject(criteria[i]);
		type->run();
	}
	cout << endl;
}

} // skeleton

namespace recognition {

class Car {
public: virtual ~Car() {}
public:
	virtual void drive() { cout << "  Oops!\n"; }
public:
	static Car* makeObject(const string& criteria);
};
class Mercedes : public Car {
public:
	void drive() { cout << "  Mercedes\n"; }
};
class BMW : public Car {
public:
	void drive() { cout << "  BMW\n"; }
};
class Tesla : public Car {
public:
	void drive() { cout << "  Tesla\n"; }
};
// Seam point - add another car.

Car* Car::makeObject(const string& criteria) {
	if(		criteria == "ChickMagnet")	return new Mercedes;
	else if(criteria == "Status")	return new BMW;
	else if(criteria == "Electric")	return new Tesla;

	else {
		return 0;	// Null.
	}
}

void demo() {	// Decouples client from creation.
	string criteria[] = { "ChickMagnet", "Status", "Electric" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Car* type = Car::makeObject(criteria[i]);
		type->drive();
	}
	cout << endl;
}

} // recognition

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "factoryMethod") {}
	virtual ~Observer() { DTOR("~FactoryMethodObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << factory_method::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << factory_method::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << factory_method::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << factory_method::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << factory_method::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << factory_method::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << factory_method::skeleton >>\n";
		skeleton::demo(seqNo);
	}
	virtual void recognition() {
		cout << seqNo << ") << factory_method::recognition >>\n";
		recognition::demo();
	}
};

} // factory_method

#endif /* LECTURES_FACTORYMETHOD_ */

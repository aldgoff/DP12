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

void clientCode_Trip(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  Fly to the Bahamas.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Load car.\n";
	}
	// Seam point - add another vacation.
}
void clientCode_Plan(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  Schedule dives.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Mark up map.\n";
	}
	// Seam point - add another vacation.
}
void clientCode_Experience(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  See pretty fish.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Take pictures.\n";
	}
	// Seam point - add another vacation.
}

void demo(int seqNo) {
	cout<<seqNo<<") << factory_method::lecture::legacy::demo() >>\n";
	string criteria[] = { "Bahamas", "RoadTrip" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		clientCode_Trip(criteria[i]);
		clientCode_Plan(criteria[i]);
		clientCode_Experience(criteria[i]);
	}
	cout << endl;
}

}

namespace problem {

void clientCode_Trip(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  Fly to the Bahamas.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Load car.\n";
		}
	else if(criteria == "Disneyland") {
		cout << "  Get to Disneyland.\n";
		}
	// Seam point - add another vacation.
	else {
		cout << "  Oops!";
	}
}
void clientCode_Plan(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  Schedule dives.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Mark up map.\n";
		}
	else if(criteria == "Disneyland") {
		cout << "  Plan rides.\n";
		}
	// Seam point - add another vacation.
	else {
		cout << "  Oops!";
	}
}
void clientCode_Experience(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  See pretty fish.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Take pictures.\n";
		}
	else if(criteria == "Disneyland") {
		cout << "  Wait in line.\n";
		}
	// Seam point - add another vacation.
	else {
		cout << "  Oops!\n";
	}
}

void demo(int seqNo) {
	cout<<seqNo<<") << factory_method::lecture::problem::demo() >>\n";
	string criteria[] = {"Bahamas","RoadTrip","Disneyland","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		clientCode_Trip(criteria[i]);
		clientCode_Plan(criteria[i]);
		clientCode_Experience(criteria[i]);
	}
	cout << endl;
}

}

namespace solution {

class Vacation { // Factory Method design pattern.
public:	virtual ~Vacation() {
		DTOR(" ~Vacation\n", Lecture);
	}
public:
	virtual void getThere() { cout << "  Oops!"; }
	virtual void schedule() { cout << " Oops!"; }
	virtual void haveFun() { cout << " Oops!"; }
public:
	static Vacation* makeObject(const string& criteria);
};
class Bahamas : public Vacation {
public:	virtual ~Bahamas() {
		DTOR("    ~Bahamas", Lecture);
	}
public:
	void getThere() { cout << "  Fly to the Bahamas.\n";}
	void schedule() { cout << "  Schedule dives.\n";}
	void haveFun() { cout << "  See pretty fish.\n";}
};
class RoadTrip : public Vacation {
public:	virtual ~RoadTrip() {
		DTOR("    ~RoadTrip", Lecture);
	}
public:
	void getThere() { cout << "  Load car.\n";}
	void schedule() { cout << "  Mark up map.\n";}
	void haveFun() { cout << "  Take pictures.\n";}
};
class Disneyland : public Vacation {
public:	virtual ~Disneyland() {
		DTOR("    ~Disneyland", Lecture);
	}
public:
	void getThere() { cout << "  Get to Disneyland.\n";}
	void schedule() { cout << "  Plan rides.\n";}
	void haveFun() { cout << "  Wait in line.\n";}
};
// Seam point - add another vacation.

Vacation* Vacation::makeObject(const string& criteria) {
	if(criteria == "Bahamas")	return new Bahamas;
	if(criteria == "RoadTrip")	return new RoadTrip;
	if(criteria == "Disneyland")	return new Disneyland;
	// Seam point - insert another vacation.
	return new Vacation; // Base, default, null, exception.
}

void clientCode_Trip(Vacation* vacation) {
	vacation->getThere();
}
void clientCode_Plan(Vacation* vacation) {
	vacation->schedule();
}
void clientCode_Experience(Vacation* vacation) {
	vacation->haveFun();
}

void demo(int seqNo) {
	cout<<seqNo<<") << factory_method::lecture::solution::demo() >>\n";
	string criteria[] = {"Bahamas","RoadTrip","Disneyland","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Vacation* vacation = Vacation::makeObject(criteria[i]);
		clientCode_Trip(vacation);
		clientCode_Plan(vacation);
		clientCode_Experience(vacation);
		delete vacation;
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

class FactoryMethod { // If the classes are varying...
public: virtual ~FactoryMethod() {
		DTOR("~FactoryMethod\n", Lecture);
	}
public:
	virtual void method() { cout << "  Oops!  "; }
public:
	static FactoryMethod* makeObject(string& criteria);
};
class Type1 : public FactoryMethod {
public: ~Type1() {
		DTOR("  ~Type1 ", Lecture);
	}
public:
	void method() { cout << "  Type1."; }
};
class Type2 : public FactoryMethod {
public: ~Type2() {
		DTOR("  ~Type2 ", Lecture);
	}
public:
	void method() { cout << "  Type2."; }
};
class Type3 : public FactoryMethod {
public: ~Type3() {
		DTOR("  ~Type3 ", Lecture);
	}
public:
	void method() { cout << "  Type3."; }
};
// Seam point - add another class.

FactoryMethod* FactoryMethod::makeObject(string& criteria) {
	if(criteria == "Type1")	return new Type1;
	if(criteria == "Type2")	return new Type2;
	if(criteria == "Type3")	return new Type3;
	// Seam point - insert another class.
	return new FactoryMethod; // Base, default, null, exception.
}

void demo(int seqNo) {	// Test variations.
	string criteria[] = { "Type1", "Type2", "Type3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		FactoryMethod* type = FactoryMethod::makeObject(criteria[i]);
		type->method();
		delete type;
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
	void drive() { cout << "  Mercedes.\n"; }
};
class BMW : public Car {
public:
	void drive() { cout << "  BMW.\n"; }
};
class Tesla : public Car {
public:
	void drive() { cout << "  Tesla.\n"; }
};
// Seam point - add another car.

Car* Car::makeObject(const string& criteria) {
	if(criteria == "Safety")	return new Mercedes;
	if(criteria == "Status")	return new BMW;
	if(criteria == "Electric")	return new Tesla;
	// Seam point - insert another criteria.
	return 0;	// Null.
}

void demo() {	// Test variations.
	string criteria[] = { "Safety", "Status", "Electric" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Car* type = Car::makeObject(criteria[i]);
		type->drive();
	}
	cout << endl;
}

} // recognition

namespace refactoring {

namespace bad { // Do two things...

struct Trailer {
	void roll() { cout << "  Trailer rolling.\n"; }
	void park() { cout << "  Trailer parking.\n"; }
};
struct CTainer {
	void ship() { cout << "  CTainer shipping.\n"; }
	void store() { cout << "  CTainer storing.\n"; }
};
struct Warehouse {
	void stock() { cout << "  Warehouse stocked.\n"; }
	void lock() { cout << "  Warehouse locked.\n"; }
};

void clientCode(int criteria) {
	switch(criteria) {
	case 0:	{ Trailer trailer;
		trailer.roll();
		trailer.park();
		} break;
	case 1:	{ CTainer ctainer;
		ctainer.ship();
		ctainer.store();
		} break;
	case 2:	{ Warehouse warehouse;
		warehouse.stock();
		warehouse.lock();
		} break;
	default: cout << "  Oops!\n"; break;
	}
}

void demo() {
	cout << "  refactoring::bad::demo().\n";
	int test[] = { 0, 1, 2, 3 };
	for(size_t i=0; i<COUNT(test); i++) {
		clientCode(test[i]);
	}
	cout << endl;
}

} // bad

namespace good { // Do two things...

class FactoryMethod {
public: virtual ~FactoryMethod() {}
public:
	virtual void doTwoThings() { cout << "  Oops!\n"; }
public:
	static FactoryMethod* makeObject(string& criteria);
};
class Trailer_FM : public FactoryMethod {
public:
	void doTwoThings() {
		bad::Trailer trailer;
		trailer.roll();
		trailer.park();
	}
};
class CTainer_FM : public FactoryMethod {
public:
	void doTwoThings() {
		bad::CTainer ctainer;
		ctainer.ship();
		ctainer.store();
	}
};
class Warehouse_FM : public FactoryMethod {
public:
	void doTwoThings() {
		bad::Warehouse warehouse;
		warehouse.stock();
		warehouse.lock();
	}
};
// Seam point - add another class.

FactoryMethod* FactoryMethod::makeObject(string& criteria) {
	if(criteria == "Trailer")	return new Trailer_FM;
	if(criteria == "CTainer")	return new CTainer_FM;
	if(criteria == "Warehouse")	return new Warehouse_FM;
	// Seam point - insert another criteria.
	else { return new FactoryMethod; }
}

void clientCode(FactoryMethod* type) {
	type->doTwoThings();
}

void demo() {
	cout << "  refactoring::good::demo().\n";
	string criteria[] = {"Trailer","CTainer","Warehouse","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		FactoryMethod* type = FactoryMethod::makeObject(criteria[i]);
		clientCode(type);
		delete type;
	}
	cout << endl;
}

} // good

} // refactoring

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "factoryMethod") {}
	virtual ~Observer() { DTOR("~FactoryMethodObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
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
	virtual void refactoring() {
		cout << seqNo << ") << factory_method::refactoring >>\n";
		refactoring::bad::demo();
		refactoring::good::demo();
	}
};

} // factory_method

#endif /* LECTURES_FACTORYMETHOD_ */

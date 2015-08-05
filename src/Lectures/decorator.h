/*
 * decorator.h
 *
 *  Created on: Aug 4, 2015
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

#ifndef LECTURES_DECORATOR_H_
#define LECTURES_DECORATOR_H_

#include "../DP.h"

namespace decorator {	// Command line: decorator [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << decorator::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << decorator::lecture::problem::demo() >>\n";
}

}

namespace solution {

class Decorator {	// If the options are varying...
public:
	Decorator() {}
	virtual ~Decorator() { DTOR("~DecoratorObserver\n", Architecture); }
public:
	virtual void run() {}
public:
	static Decorator* makeObject(const string& criteria);
};
class Derived : public Decorator {
public:
	Derived() {}
	virtual ~Derived() { DTOR("~Derived ", Architecture); }
public:
	void run() {}
};
// Seam point - add another Derived.

Decorator* Decorator::makeObject(const string& criteria) {
	if(		criteria == "whatever")	return new Decorator;
	else if(criteria == "whatever")	return new Decorator;
	// Seam point - add another Decorator.

	else {
		return new Decorator;
	}
}

void demo(int seqNo) {
	cout << seqNo << ") << decorator::lecture::solution::demo() >>\n";
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Decorator* derived = Decorator::makeObject(criteria[i]);
		derived->run();
	}
	cout << endl;
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << decorator::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << decorator::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << decorator::homework::solution::demo() >>\n";
}

}

} // homework

namespace skeleton {

class Component {
public: virtual ~Component() {}
public:
	virtual void behavior()=0;
};
class Base : public Component {	// Base object to be decorated.
public:
	void behavior() { cout << "  Base"; }
};
class Decorator : public Component {	// If the options are varying...
protected:
	Component* decorator;
public:
	Decorator(Component* decorator) : decorator(decorator) {}
public:
	void behavior() {}
public:
	static Component* makeObject(Component* thing, const string& criteria);
};
class Option1 : public Decorator {
public:
	Option1(Component* decorator) : Decorator(decorator) {}
public:
	void behavior() { decorator->behavior(); cout << " Opt1"; }
};
class Option2 : public Decorator {
public:
	Option2(Component* decorator) : Decorator(decorator) {}
public:
	void behavior() { decorator->behavior(); cout << " Opt2"; }
};
class Option3 : public Decorator {
public:
	Option3(Component* decorator) : Decorator(decorator) {}
public:
	void behavior() { decorator->behavior(); cout << " Opt3"; }
};
// Seam point - add another option.

Component* Decorator::makeObject(Component* thing, const string& criteria) {
	if(		criteria == "Option1")	return new Option1(thing);
	else if(criteria == "Option2")	return new Option2(thing);
	else if(criteria == "Option3")	return new Option3(thing);

	else { return thing; }	// Typically: warning, exception or log.
}

void demo(int seqNo) {	// Decouples client from creation.
	string criteria[] = { "Option1", /*"Option2",*/ "Option3", "oops" };
	Component* thing = new Base;
	for(size_t i=0; i<COUNT(criteria); i++) {
		thing = Decorator::makeObject(thing, criteria[i]);
	}
	thing->behavior();	cout << "\n";
	cout << endl;
}

} // skeleton

namespace recognition {

class Vehicle {
public: virtual ~Vehicle() {}
public:
	virtual void features()=0;
};
class Car : public Vehicle {
public:
	void features() { cout << "  Car"; }
};
class Accessory : public Vehicle {
protected:
	Vehicle* acc;
public:
	Accessory(Vehicle* addOn) : acc(addOn) {}
public:
	void features() {}
public:
	static Vehicle* create(Vehicle* car, string& criteria);
};
class Audio : public Accessory {
public:
	Audio(Vehicle* soundSystem) : Accessory(soundSystem) {}
public:
	void features() { acc->features(); cout<<" audio";}
};
class SunRoof : public Accessory {
public:
	SunRoof(Vehicle* roof) : Accessory(roof) {}
public:
	void features() { acc->features(); cout<<" sun-roof";}
};
class MagWheels : public Accessory {
public:
	MagWheels(Vehicle* wheels) : Accessory(wheels) {}
public:
	void features() { acc->features(); cout<<" mag-wheels";}
};
// Seam point - add another accessory.

Vehicle* Accessory::create(Vehicle* car, string& criteria) {
	if(		criteria == "Music")	return new Audio(car);
	else if(criteria == "View")	return new SunRoof(car);
	else if(criteria == "Hip")	return new MagWheels(car);
	else { return car; }	// Do nothing.
}

void demo() {	// Decouples client from creation.
	string criteria[] = { "Music", "View", "Hip" };
	Vehicle* car = new Car;
	for(size_t i=0; i<COUNT(criteria); i++)
		car = Accessory::create(car, criteria[i]);
	car->features();	cout << "\n";
	cout << endl;
}

} // recognition

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "decorator") {}
	virtual ~Observer() { DTOR("~DecoratorObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << decorator::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << decorator::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << decorator::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << decorator::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << decorator::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << decorator::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << decorator::skeleton >>\n";
		skeleton::demo(seqNo);
	}
	virtual void recognition() {
		cout << seqNo << ") << decorator::recognition >>\n";
		recognition::demo();
	}
};

} // decorator

#endif /* LECTURES_DECORATOR_H_ */

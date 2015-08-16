/*
 * bridge.h   dpTemplate dp_template DPTemplate derived
 *
 *  Created on: May 23, 2015
 *      Author: aldgoff
 *
 * Varies: If the abstractions and implementations are varying, use the Bridge pattern.
 *
 * Desc:
 *
 * Category: whatever
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Bridge
 *  	http://www.dofactory.com/net/bridge-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter10
 *  	http://sourcemaking.com/design_patterns/bridge
 */

#ifndef LECTURES_BRIDGE_H_
#define LECTURES_BRIDGE_H_

#include "../DP.h"

namespace bridge {	// Command line: bridge [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << bridge::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << bridge::lecture::problem::demo() >>\n";
}

}

namespace solution {

class Bridge {	// If the abstractions and implementations are varying...
public:
	Bridge() {}
	virtual ~Bridge() { DTOR("~BridgeObserver\n", Architecture); }
public:
	virtual void run() {}
public:
	static Bridge* makeObject(const string& criteria);
};
class Derived : public Bridge {
public:
	Derived() {}
	virtual ~Derived() { DTOR("~Derived ", Architecture); }
public:
	void run() {}
};
// Seam point - add another Derived.

Bridge* Bridge::makeObject(const string& criteria) {
	if(		criteria == "whatever")	return new Bridge;
	else if(criteria == "whatever")	return new Bridge;
	// Seam point - add another Bridge.

	else {
		return new Bridge;
	}
}

void demo(int seqNo) {
	cout << seqNo << ") << bridge::lecture::solution::demo() >>\n";
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Bridge* derived = Bridge::makeObject(criteria[i]);
		derived->run();
	}
	cout << endl;
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << bridge::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << bridge::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << bridge::homework::solution::demo() >>\n";
}

}

} // homework

namespace skeleton {

class Platform { // If the implementations are varying...
public: virtual ~Platform() {}
public:
	virtual string execute() { return "Oops"; }
public:
	static Platform* makeObject(string& crit);
};
class Platform1 : public Platform {
public:
	string execute() { return "Platform1"; }
};
class Platform2 : public Platform {
public:
	string execute() { return "Platform2"; }
};
class Platform3 : public Platform {
public:
	string execute() { return "Platform3"; }
};
// Seam point - add another implementation.

Platform* Platform::makeObject(string& crit) {
	if(		crit == "Platform1")	return new Platform1;
	else if(crit == "Platform2")	return new Platform2;
	else if(crit == "Platform3")	return new Platform3;
	// Seam point - insert another criteria.
	else {						// Options:
		return new Platform;	// null, exception,
	}							// base, default, ABC.
}

class Abstraction { // If the abstractions are varying...
protected:
	Platform* platform;
public:
	Abstraction(Platform* platform=0) : platform(platform) {}
	virtual ~Abstraction() {}
public:
	virtual string execute() { return "Oops"; }
public:
	static Abstraction* makeObject(string& crit, Platform* plat);
};
class Abstract1 : public Abstraction {
public:
	Abstract1(Platform* platform) : Abstraction(platform) {}
public:
	string execute() {
		return "Abstract1 via " + platform->execute();
	}
};
class Abstract2 : public Abstraction {
public:
	Abstract2(Platform* platform) : Abstraction(platform) {}
public:
	string execute() {
		return "Abstract2 via " + platform->execute();
	}
};
class Abstract3 : public Abstraction {
public:
	Abstract3(Platform* platform) : Abstraction(platform) {}
public:
	string execute() {
		return "Abstract3 via " + platform->execute();
	}
};
// Seam point - add another abstraction.

Abstraction* Abstraction::makeObject(string& crit, Platform* plat) {
	if(		crit == "Abstract1")	return new Abstract1(plat);
	else if(crit == "Abstract2")	return new Abstract2(plat);
	else if(crit == "Abstract3")	return new Abstract3(plat);
	// Seam point - insert another criteria.
	else {						// Options:
		return new Abstraction;	// null, exception,
	}							// base, default, ABC.
}

void demo() {	// Test variation.
	string critA[] = {"Abstract1","Abstract2","Abstract3","oops" };
	string critP[] = {"Platform1","Platform2","Platform3","oops" };
	for(size_t i=0; i<COUNT(critA); i++) {
		for(size_t j=0; j<COUNT(critP); j++) {
			Platform* platform = Platform::makeObject(critP[j]);
			Abstraction* abstract = Abstraction::makeObject(critA[i], platform);
			cout << "  " << abstract->execute() << ".\n";
		}
	}
	cout << endl;
}

} // skeleton

namespace recognition {

class Bridge {	// If the abstractions and implementations are varying...
public: virtual ~Bridge() {}
public:
	virtual void run() {}
public:
	static Bridge* makeObject(const string& criteria);
};
class Derived : public Bridge {
public:
	void run() {}
};
// Seam point - add another derived.

Bridge* Bridge::makeObject(const string& criteria) {
	if(		criteria == "Derived1")	return new Bridge;
	else if(criteria == "Derived2")	return new Bridge;
	else if(criteria == "Derived3")	return new Bridge;

	else {
		return new Bridge;	// Opts: null, exception, base, default, ABC.
	}
}

void demo() {	// Decouples client from creation.
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Bridge* derived = Bridge::makeObject(criteria[i]);
		derived->run();
	}
	cout << endl;
}

} // recognition

namespace refactoring {

namespace bad {

void clientCode(int criteria) {
	switch(criteria) {
	case 0:	{
		} break;
	case 1:	{
		} break;
	case 2:	{
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

namespace good {

class DesignPattern {
public: virtual ~DesignPattern() {}
public:
	virtual void method() { cout << "  Oops!\n"; }
public:
	static DesignPattern* makeObject(const string& criteria);
};
class Derived1 : public DesignPattern {
public:
	void method() {}
};
class Derived2 : public DesignPattern {
public:
	void method() {}
};
class Derived3 : public DesignPattern {
public:
	void method() {}
};
// Seam point - add another class.

DesignPattern* DesignPattern::makeObject(const string& criteria) {
	if(		criteria == "Derived1")	return new Derived1;
	else if(criteria == "Derived2")	return new Derived2;
	else if(criteria == "Derived3")	return new Derived3;
	// Seam point - insert another criteria.
	else {
		return new DesignPattern;
	}
}

void clientCode(DesignPattern* type) {
	type->method();
}

void demo() {
	cout << "  refactoring::good::demo().\n";
	string criteria[] = {"Derived1","Derived2","Derived3","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		DesignPattern* type = DesignPattern::makeObject(criteria[i]);
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
	: observer::DPObserver(subject, seqNo, "bridge") {}
	virtual ~Observer() { DTOR("~BridgeObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << bridge::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << bridge::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << bridge::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << bridge::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << bridge::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << bridge::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << bridge::skeleton >>\n";
		skeleton::demo();
	}
	virtual void recognition() {
		cout << seqNo << ") << bridge::recognition >>\n";
		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << bridge::refactoring >>\n";
		refactoring::bad::demo();
		refactoring::good::demo();
	}
};

} // bridge

#endif /* LECTURES_BRIDGE_H_ */

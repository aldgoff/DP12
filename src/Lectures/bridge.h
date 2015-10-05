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
public: virtual ~Platform() {
		DTOR("~Platform ", Lecture);
	}
public:
	virtual string implement() { return "Oops"; }
public:
	static Platform* makeObject(string& crit);
};
class Platform1 : public Platform {
public: ~Platform1() {
		DTOR(" ~Platform1 ", Lecture);
	}
public:
	string implement() { return "Platform1"; }
};
class Platform2 : public Platform {
public: ~Platform2() {
		DTOR(" ~Platform2 ", Lecture);
	}
public:
	string implement() { return "Platform2"; }
};
class Platform3 : public Platform {
public: ~Platform3() {
		DTOR(" ~Platform3 ", Lecture);
	}
public:
	string implement() { return "Platform3"; }
};
// Seam point - add another implementation.
Platform* Platform::makeObject(string& crit) {
	if(crit == "Platform1")	return new Platform1;
	if(crit == "Platform2")	return new Platform2;
	if(crit == "Platform3")	return new Platform3;
	// Seam point - insert another criteria.
	return new Platform; // Base, default, null, exception.
}

class Abstraction { // If the abstractions are varying...
protected:
	Platform* platform;
public:
	Abstraction(Platform* platform=0) : platform(platform) {}
	virtual ~Abstraction() { delete platform;
		DTOR("~Abstraction\n", Lecture);
	}
public:
	virtual string execute() { return "Oops"; }
public:
	static Abstraction* makeObject(string& crit, Platform* plat);
};
class Abstract1 : public Abstraction {
public: ~Abstract1() {
		DTOR("  ~Abstract1", Lecture);
	}
public:
	Abstract1(Platform* platform) : Abstraction(platform) {}
public:
	string execute() {
		return "Abstract1 via " + platform->implement();
	}
};
class Abstract2 : public Abstraction {
public: ~Abstract2() {
		DTOR("  ~Abstract2", Lecture);
	}
public:
	Abstract2(Platform* platform) : Abstraction(platform) {}
public:
	string execute() {
		return "Abstract2 via " + platform->implement();
	}
};
class Abstract3 : public Abstraction {
public: ~Abstract3() {
		DTOR("  ~Abstract3", Lecture);
	}
public:
	Abstract3(Platform* platform) : Abstraction(platform) {}
public:
	string execute() {
		return "Abstract3 via " + platform->implement();
	}
};
// Seam point - add another abstraction.
Abstraction* Abstraction::makeObject(string& crit, Platform* plat){
	if(crit == "Abstract1")	return new Abstract1(plat);
	if(crit == "Abstract2")	return new Abstract2(plat);
	if(crit == "Abstract3")	return new Abstract3(plat);
	// Seam point - insert another criteria.
	return new Abstraction; // Base, default, null, exception.
}

void demo() {	// Test variations.
	string critA[] = {"Abstract1","Abstract2","Abstract3","oops" };
	string critP[] = {"Platform1","Platform2","Platform3" };
	for(size_t i=0; i<COUNT(critA); i++) {
		Abstraction* abstracts[COUNT(critP)];
		for(size_t j=0; j<COUNT(critP); j++) {
			Platform* platform = Platform::makeObject(critP[j]);
			abstracts[j] = Abstraction::makeObject(critA[i], platform);
			cout << "  " << abstracts[j]->execute() << ".\n";
		}
		for(size_t j=0; j<COUNT(critP); j++) delete abstracts[j];
	}
	cout << endl;
}

} // skeleton

namespace recognition {

class Display { // If the display tech is varying...
public: virtual ~Display() {}
	virtual string render() { return "Oops"; }
	static Display* makeObject(string& crit);
};
class Raster : public Display {
	public: string render() { return "Raster"; } };
class Vector : public Display {
	public: string render() { return "Vector"; } };
class Hologram : public Display {
	public: string render() { return "Hologram";}};
// Seam point - add another display.
Display* Display::makeObject(string& crit) {
	if(crit == "Raster")	return new Raster;
	if(crit == "Vector")	return new Vector;
	if(crit == "Hologram")	return new Hologram;
	// Seam point - insert another criteria.
	return new Display;	// Base.
}

class Shape { // If the shapes are varying...
protected: Display* dis;
public: Shape(Display* dis=0) : dis(dis) {}
	virtual ~Shape() {}
	virtual string draw() { return "Oops"; }
	static Shape* makeObject(string& crit, Display* plat);
};
class Duck : public Shape {
public: Duck(Display* dis) : Shape(dis) {}
	string draw() { return "Draw duck via " + dis->render(); }
};
class Hero : public Shape {
public: Hero(Display* dis) : Shape(dis) {}
	string draw() { return "Draw hero via " + dis->render(); }
};
class Dino : public Shape {
public: Dino(Display* dis) : Shape(dis) {}
	string draw() { return "Draw dino via " + dis->render(); }
};
// Seam point - add another shape.
Shape* Shape::makeObject(string& crit, Display* plat) {
	if(crit == "Duck")	return new Duck(plat);
	if(crit == "Hero")	return new Hero(plat);
	if(crit == "Dino")	return new Dino(plat);
	// Seam point - insert another criteria.
	return new Shape;	// Base.
}

void demo() {	// Test variations.
	string image[] = {"Duck","Hero","Dino","oops"};
	string render[] = {"Raster","Vector","Hologram","oops"};
	for(size_t i=0; i<COUNT(image); i++) {
		for(size_t j=0; j<COUNT(render); j++) {
			Display* dis = Display::makeObject(render[j]);
			Shape* shape = Shape::makeObject(image[i], dis);
			cout << "  " << shape->draw() << ".\n";
			delete dis; delete shape;
		}
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

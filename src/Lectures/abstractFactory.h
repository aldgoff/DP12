/*
 * abstractFactory.h
 *
 *  Created on: Aug 15, 2015
 *      Author: aldgoff
 *
 * Varies: If the families are varying, use the Abstract Factory pattern.
 *
 * Desc:
 *
 * Category: whatever
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Abstract_Factory
 *  	http://www.dofactory.com/net/abstract-factory-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter11
 *  	http://sourcemaking.com/design_patterns/abstract_factory
 */

#ifndef LECTURES_ABSTRACTFACTORY_H_
#define LECTURES_ABSTRACTFACTORY_H_

#include "../DP.h"

namespace abstract_factory {	// Command line: abstractFactory [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << abstract_factory::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << abstract_factory::lecture::problem::demo() >>\n";
}

}

namespace solution {

class AbstractFactory {	// If the families are varying...
public:
	AbstractFactory() {}
	virtual ~AbstractFactory() { DTOR("~AbstractFactoryObserver\n", Architecture); }
public:
	virtual void run() {}
public:
	static AbstractFactory* makeObject(const string& criteria);
};
class Derived : public AbstractFactory {
public:
	Derived() {}
	virtual ~Derived() { DTOR("~Derived ", Architecture); }
public:
	void run() {}
};
// Seam point - add another Derived.

AbstractFactory* AbstractFactory::makeObject(const string& criteria) {
	if(		criteria == "whatever")	return new AbstractFactory;
	else if(criteria == "whatever")	return new AbstractFactory;
	// Seam point - add another AbstractFactory.

	else {
		return new AbstractFactory;
	}
}

void demo(int seqNo) {
	cout << seqNo << ") << abstract_factory::lecture::solution::demo() >>\n";
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		AbstractFactory* factory = AbstractFactory::makeObject(criteria[i]);
		factory->run();
	}
	cout << endl;
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << abstract_factory::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << abstract_factory::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << abstract_factory::homework::solution::demo() >>\n";
}

}

} // homework

namespace skeleton {

class Type1 {
public: virtual ~Type1() {
		DTOR("~Type1\n", Lecture);
	}
public:	virtual void id()=0;
};
class Type1_Able : public Type1 {
public: ~Type1_Able() {
		DTOR("  ~Type1_Able ", Lecture);
	}
public:	void id() { cout << "  Type 1 Able.\n"; }
};
class Type1_Bill : public Type1 {
public: ~Type1_Bill() {
		DTOR("  ~Type1_Bill ", Lecture);
	}
public:	void id() { cout << "  Type 1 Bill.\n"; }
};
class Type1_Cain : public Type1 {
public: ~Type1_Cain() {
		DTOR("  ~Type1_Cain ", Lecture);
	}
public:	void id() { cout << "  Type 1 Cain.\n"; }
};

class Type2 {
public: virtual ~Type2() {
		DTOR("~Type2\n", Lecture);
	}
public:	virtual void id()=0;
};
class Type2_Able : public Type2 {
public: ~Type2_Able() {
		DTOR("  ~Type2_Able ", Lecture);
	}
public:	void id() { cout << "  Type 2 Able.\n"; }
};
class Type2_Bill : public Type2 {
public: ~Type2_Bill() {
		DTOR("  ~Type2_Bill ", Lecture);
	}
public:	void id() { cout << "  Type 2 Bill.\n"; }
};
class Type2_Cain : public Type2 {
public: ~Type2_Cain() {
		DTOR("  ~Type2_Cain ", Lecture);
	}
public:	void id() { cout << "  Type 2 Cain.\n"; }
};

class Type3 {
public: virtual ~Type3() {
		DTOR("~Type3\n", Lecture);
	}
public:	virtual void id()=0;
};
class Type3_Able : public Type3 {
public: ~Type3_Able() {
		DTOR("  ~Type3_Able ", Lecture);
	}
public:	void id() { cout << "  Type 3 Able.\n"; }
};
class Type3_Bill : public Type3 {
public: ~Type3_Bill() {
		DTOR("  ~Type3_Bill ", Lecture);
	}
public:	void id() { cout << "  Type 3 Bill.\n"; }
};
class Type3_Cain : public Type3 {
public: ~Type3_Cain() {
		DTOR("  ~Type3_Cain ", Lecture);
	}
public:	void id() { cout << "  Type 3 Cain.\n"; }
};

class AbstractFactory {	// If whole families are varying...
public: virtual ~AbstractFactory() {
		DTOR("~AbstractFactory\n", Lecture);
	}
public:
	virtual Type1* create_type_1()=0;
	virtual Type2* create_type_2()=0;
	virtual Type3* create_type_3()=0;
public:
	static AbstractFactory* makeObject(string& criteria);
};
class Factory_Able : public AbstractFactory {
public: ~Factory_Able() {
		DTOR("  ~Factory_Able ", Lecture);
	}
public:
	Type1* create_type_1() { return new Type1_Able; }
	Type2* create_type_2() { return new Type2_Able; }
	Type3* create_type_3() { return new Type3_Able; }
};
class Factory_Bill : public AbstractFactory {
public: ~Factory_Bill() {
		DTOR("  ~Factory_Bill ", Lecture);
	}
public:
	Type1* create_type_1() { return new Type1_Bill; }
	Type2* create_type_2() { return new Type2_Bill; }
	Type3* create_type_3() { return new Type3_Bill; }
};
class Factory_Cain : public AbstractFactory {
public: ~Factory_Cain() {
		DTOR("  ~Factory_Cain ", Lecture);
	}
public:
	Type1* create_type_1() { return new Type1_Cain; }
	Type2* create_type_2() { return new Type2_Cain; }
	Type3* create_type_3() { return new Type3_Cain; }
};
// Seam point - add another abstract factory.

AbstractFactory* AbstractFactory::makeObject(string& criteria) {
	if(criteria == "Able")	return new Factory_Able;
	if(criteria == "Bill")	return new Factory_Bill;
	if(criteria == "Cain")	return new Factory_Cain;
	// Seam point - insert another family.
	return 0;	// ABS null.
}

void demo() {	// Test variations.
	string criteria[] = { "Able", "Bill", "Cain", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		AbstractFactory* factory
			= AbstractFactory::makeObject(criteria[i]);
		if(!factory) {
			cout << "  Oops!\n\n";
			return;
		}

		Type1* type1 = factory->create_type_1();
		Type2* type2 = factory->create_type_2();
		Type3* type3 = factory->create_type_3();

		type1->id();
		type2->id();
		type3->id();

		delete type1; delete type2; delete type3;
		delete factory;
	}
	cout << endl;
}

} // skeleton

namespace recognition {

class Menu {	// If the menus are varying...
public: virtual ~Menu() {}
public:	virtual void id()=0;
};
class MenuMac : public Menu {
public:	void id() { cout << "  Menu Mac.\n"; }
};
class MenuUnix : public Menu {
public:	void id() { cout << "  Menu Unix.\n"; }
};
class MenuMicro : public Menu {
public:	void id() { cout << "  Menu Micro.\n"; }
};

class Window {	// If the windows are varying...
public: virtual ~Window() {}
public:	virtual void id()=0;
};
class WindowMac : public Window {
public:	void id() { cout << "  Window Mac.\n"; }
};
class WindowUnix : public Window {
public:	void id() { cout << "  Window Unix.\n"; }
};
class WindowMicro : public Window {
public:	void id() { cout << "  Window Micro.\n"; }
};

class Touch {	// If touch interface is varying...
public: virtual ~Touch() {}
public:	virtual void id()=0;
};
class TouchMac : public Touch {
public:	void id() { cout << "  Touch Mac.\n"; }
};
class TouchUnix : public Touch {
public:	void id() { cout << "  Touch Unix.\n"; }
};
class TouchMicro : public Touch {
public:	void id() { cout << "  Touch Micro.\n"; }
};

class OS {	// If the operating system is varying...
public: virtual ~OS() {}
public:
	virtual Menu* create_menus()=0;
	virtual Window* create_windows()=0;
	virtual Touch* create_touch()=0;
public:
	static OS* makeObject(string& criteria);
};
class Mac : public OS {
public:
	Menu* create_menus() { return new MenuMac; }
	Window* create_windows() { return new WindowMac; }
	Touch* create_touch() { return new TouchMac; }
};
class Unix : public OS {
public:
	Menu* create_menus() { return new MenuUnix; }
	Window* create_windows() { return new WindowUnix; }
	Touch* create_touch() { return new TouchUnix; }
};
class Micro : public OS {
public:
	Menu* create_menus() { return new MenuMicro; }
	Window* create_windows() { return new WindowMicro; }
	Touch* create_touch() { return new TouchMicro; }
};
// Seam point - add another operating system.

OS* OS::makeObject(string& criteria) {
	if(criteria == "Mac")	return new Mac;
	if(criteria == "Unix")	return new Unix;
	if(criteria == "Micro")	return new Micro;
	// Seam point - insert another criteria.
	return 0;	// ABC null.
}

void demo() {	// Test variations.
	string criteria[] = { "Mac", "Unix", "Micro", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		OS* factory = OS::makeObject(criteria[i]);
		if(!factory) {
			cout << "  Oops!\n\n";
			return;
		}

		Menu* menus = factory->create_menus();
		Window* windows = factory->create_windows();
		Touch* touch = factory->create_touch();

		menus->id();
		windows->id();
		touch->id();
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
	: observer::DPObserver(subject, seqNo, "abstractFactory") {}
	virtual ~Observer() { DTOR("~AbstractFactoryObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << abstract_factory::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << abstract_factory::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << abstract_factory::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << abstract_factory::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << abstract_factory::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << abstract_factory::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << abstract_factory::skeleton >>\n";
		skeleton::demo();
	}
	virtual void recognition() {
		cout << seqNo << ") << abstract_factory::recognition >>\n";
		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << abstract_factory::refactoring >>\n";
		refactoring::bad::demo();
		refactoring::good::demo();
	}
};

} // abstract_factory

#endif /* LECTURES_ABSTRACTFACTORY_H_ */

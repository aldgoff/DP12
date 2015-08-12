/*
 * dptemplate.h deriveds DP Template dpTemplate dp_template DPTemplate derived
 *
 *  Created on: May 23, 2015
 *      Author: aldgoff
 *
 * Varies: If the deriveds are varying, use the DP Template pattern.
 *
 * Desc:
 *
 * Category: whatever
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#DPTemplate
 *  	http://www.dofactory.com/net/dptemplate-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter7
 *  	http://sourcemaking.com/design_patterns/dptemplate
 */

#ifndef LECTURES_DPTEMPLATE_H_
#define LECTURES_DPTEMPLATE_H_

#include "DP.h"		// Delete, uncomment below.
//#include "../DP.h"
// Add to the 2 seam points in DP.cpp, then delete this line.

namespace dp_template {	// Command line: dpTemplate [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << dp_template::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << dp_template::lecture::problem::demo() >>\n";
}

}

namespace solution {

class DPTemplate {	// If the deriveds are varying...
public:
	DPTemplate() {}
	virtual ~DPTemplate() { DTOR("~DPTemplateObserver\n", Architecture); }
public:
	virtual void run() {}
public:
	static DPTemplate* makeObject(const string& criteria);
};
class Derived : public DPTemplate {
public:
	Derived() {}
	virtual ~Derived() { DTOR("~Derived ", Architecture); }
public:
	void run() {}
};
// Seam point - add another Derived.

DPTemplate* DPTemplate::makeObject(const string& criteria) {
	if(		criteria == "whatever")	return new DPTemplate;
	else if(criteria == "whatever")	return new DPTemplate;
	// Seam point - add another DPTemplate.

	else {
		return new DPTemplate;
	}
}

void demo(int seqNo) {
	cout << seqNo << ") << dp_template::lecture::solution::demo() >>\n";
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		DPTemplate* derived = DPTemplate::makeObject(criteria[i]);
		derived->run();
	}
	cout << endl;
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << dp_template::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << dp_template::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << dp_template::homework::solution::demo() >>\n";
}

}

} // homework

namespace skeleton {

class DPTemplate {	// If the deriveds are varying...
public: virtual ~DPTemplate() {}
public:
	virtual void run() {}
public:
	static DPTemplate* makeObject(const string& criteria);
};
class Derived : public DPTemplate {
public:
	void run() {}
};
// Seam point - add another derived.

DPTemplate* DPTemplate::makeObject(const string& criteria) {
	if(		criteria == "Derived1")	return new DPTemplate;
	else if(criteria == "Derived2")	return new DPTemplate;
	else if(criteria == "Derived3")	return new DPTemplate;

	else {
		return new DPTemplate;	// Opts: null, exception, base, default, ABC.
	}
}

void demo() {	// Decouples client from creation.
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		DPTemplate* derived = DPTemplate::makeObject(criteria[i]);
		derived->run();
	}
	cout << endl;
}

} // skeleton

namespace recognition {

class DPTemplate {	// If the deriveds are varying...
public: virtual ~DPTemplate() {}
public:
	virtual void run() {}
public:
	static DPTemplate* makeObject(const string& criteria);
};
class Derived : public DPTemplate {
public:
	void run() {}
};
// Seam point - add another derived.

DPTemplate* DPTemplate::makeObject(const string& criteria) {
	if(		criteria == "Derived1")	return new DPTemplate;
	else if(criteria == "Derived2")	return new DPTemplate;
	else if(criteria == "Derived3")	return new DPTemplate;

	else {
		return new DPTemplate;	// Opts: null, exception, base, default, ABC.
	}
}

void demo() {	// Decouples client from creation.
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		DPTemplate* derived = DPTemplate::makeObject(criteria[i]);
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
	: observer::DPObserver(subject, seqNo, "dpTemplate") {}
	virtual ~Observer() { DTOR("~DPTemplateObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << dp_template::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << dp_template::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << dp_template::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << dp_template::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << dp_template::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << dp_template::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << dp_template::skeleton >>\n";
		skeleton::demo();
	}
	virtual void recognition() {
		cout << seqNo << ") << dp_template::recognition >>\n";
		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << dp_template::refactoring >>\n";
		refactoring::bad::demo();
		refactoring::good::demo();
	}
};

} // dp_template

#endif /* LECTURES_DPTEMPLATE_H_ */

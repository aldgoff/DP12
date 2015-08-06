/*
 * adapter.h
 *
 *  Created on: May 11, 2015
 *      Author: aldgoff
 *
 * Varies: If the interfaces are varying, use the Adapter pattern.
 *
 * Desc: Adapt interfaces so classes can work together polymorphically.
 *
 * Category: Structural
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Adapter
 *  	http://www.dofactory.com/net/adapter-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter7
 *  	http://sourcemaking.com/design_patterns/adapter
 */

#ifndef LECTURES_ADAPTER_H_
#define LECTURES_ADAPTER_H_

#include "../DP.h"

namespace adapter {	// Command line: adapter [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << adapter::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << adapter::lecture::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << adapter::lecture::solution::demo() >>\n";
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << adapter::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << adapter::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << adapter::homework::solution::demo() >>\n";
}

}

} // homework

namespace skeleton {

struct Wrapped1 { string thisWay() { return "this way.\n"; } };
struct Wrapped2 { string thatWay() { return "that way.\n"; } };
struct Wrapped3 { string yourWay() { return "your way.\n"; } };

class Adapter {	// If the interfaces are varying...
public: virtual ~Adapter() {}
public:
	virtual void run() {}
};
class Interface1 : public Adapter {
	Wrapped1 wrapped;
public:
	void run() { cout << "  Interface1: wrapped " << wrapped.thisWay(); }
};
class Interface2 : public Adapter {
	Wrapped2 wrapped;
public:
	void run() { cout << "  Interface2: wrapped " << wrapped.thatWay(); }
};
class Interface3 : public Adapter {
	Wrapped3 wrapped;
public:
	void run() { cout << "  Interface3: wrapped " << wrapped.yourWay(); }
};
// Seam point - add another interface.

void demo(int seqNo) {
	Adapter* interfaces[] = { new Interface1, new Interface2, new Interface3 };
	for(size_t i=0; i<COUNT(interfaces); i++) {
		interfaces[i]->run();
	}
	cout << endl;
}

namespace TM {

struct Wrapped1 { string thisWay() { return "this way.\n"; } };
struct Wrapped2 { string thatWay() { return "that way.\n"; } };
struct Wrapped3 { string yourWay() { return "your way.\n"; } };

class Adapter {	// If the interfaces are varying...
public: virtual ~Adapter() {}
public:
	virtual void run() { cout << "  Oops!\n"; }
public:
	static Adapter* makeObject(const string& criteria) ;
};
class Interface1 : public Adapter {
	Wrapped1 wrapped;
public:
	void run() { cout << "  Interface1: wrapped " << wrapped.thisWay(); }
};
class Interface2 : public Adapter {
	Wrapped2 wrapped;
public:
	void run() { cout << "  Interface2: wrapped " << wrapped.thatWay(); }
};
class Interface3 : public Adapter {
	Wrapped3 wrapped;
public:
	void run() { cout << "  Interface3: wrapped " << wrapped.yourWay(); }
};
// Seam point - add another interface.

Adapter* Adapter::makeObject(const string& criteria) {
	if(		criteria == "Interface1")	return new Interface1;
	else if(criteria == "Interface2")	return new Interface2;
	else if(criteria == "Interface3")	return new Interface3;
	else { return new Adapter; }	// Opts: null, exception, base, default, ABC.
}

void demo(int seqNo) {	// Decouples client from creation.
	string criteria[] = { "Interface1", "Interface2", "Interface3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Adapter* interface = Adapter::makeObject(criteria[i]);
		interface->run();
	}
	cout << endl;
}

} // TM

} // skeleton

namespace recognition {

struct Pot { string boil() { return "  boil in water\n"; } };
struct Lamp { string expose() { return "  5 min shine\n"; } };
struct Bath { string emerse() { return "  3 min soak\n"; } };
struct Carbonize { string burn() { return "  grill\n"; } };

class Sanitation {
public: virtual ~Sanitation() {}
public:
	virtual void clean() {}
public:
	static Sanitation* makeObject(const string& criteria) ;
};
class Heat : public Sanitation {
	Pot pot;
public:
	void clean() { cout << pot.boil(); }
};
class UV : public Sanitation {
	Lamp lamp;
public:
	void clean() { cout << lamp.expose(); }
};
class Ozone : public Sanitation {
	Bath bath;
public:
	void clean() { cout << bath.emerse(); }
};
// Seam point - add another sanitation.

Sanitation* Sanitation::makeObject(const string& criteria) {
	if(		criteria == "Pathogen")	return new Heat;
	else if(criteria == "Virus")	return new UV;
	else if(criteria == "ExtraTerrestrial")	return new Ozone;

	else { throw "Infected!"; }	// Exception.
}

void demo() {	// Decouples client from creation.
	string criteria[] = { "Pathogen", "Virus", "ExtraTerrestrial" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Sanitation* purify = Sanitation::makeObject(criteria[i]);
		purify->clean();
	}
	cout << endl;
}

} // recognition

namespace refactoring {

namespace bad {

struct TwoQuarter { void twoStep() { cout << "  2 steps\n"; } };
struct ThreeQuarter { void threeStep() { cout << "  3 steps\n"; } };
struct FourQuarter { void fourStep() { cout << "  4 steps\n"; } };

void clientCode(int criteria) {
	switch(criteria) {
	case 0: { TwoQuarter beg; beg.twoStep();	 } break;
	case 1: { ThreeQuarter beg; beg.threeStep(); } break;
	case 2: { FourQuarter beg; beg.fourStep();	 } break;
	default: cout << "  Stumble!\n";
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

struct TwoQuarter { void twoStep() { cout << "  2 steps\n"; } };
struct ThreeQuarter { void threeStep() { cout << "  3 steps\n"; } };
struct FourQuarter { void fourStep() { cout << "  4 steps\n"; } };

class Adapter {
public: virtual ~Adapter() {}
public:
	virtual void sequence() { cout << "  Stumble!\n"; }
public:
	static Adapter* makeObject(const string& criteria);
};
class TwoStep : public Adapter {
	TwoQuarter	twoQtr;
public:
	void sequence() { twoQtr.twoStep(); }
};
class ThreeStep : public Adapter {
	ThreeQuarter	threeQtr;
public:
	void sequence() { threeQtr.threeStep(); }
};
class FourStep : public Adapter {
	FourQuarter	fourQtr;
public:
	void sequence() { fourQtr.fourStep(); }
};
// Seam point - add another dance.

Adapter* Adapter::makeObject(const string& criteria) {
	if(		criteria == "Polka")	 return new TwoStep;
	else if(criteria == "Waltz")	 return new ThreeStep;
	else if(criteria == "Foxtrot")	 return new FourStep;
	// Seam point - insert another dance.
	else {
		return new Adapter;
	}
}

void clientCode(Adapter* dance) {	// Obeys open/closed principle.
	dance->sequence();				// Trivial to understand.
}

void demo() {	// Separates use from creation.
	cout << "  refactoring::good::demo().\n";
	string criteria[] = {"Polka","Waltz","Foxtrot","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Adapter* dance = Adapter::makeObject(criteria[i]);
		clientCode(dance);
		delete dance;
	}
	cout << endl;
}

} // good

} // refactoring

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "adapter") {}
	virtual ~Observer() { DTOR("~AdapterObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << adapter::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << adapter::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << adapter::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << adapter::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << adapter::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << adapter::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << adapter::skeleton >>\n";
		skeleton::demo(seqNo);
		skeleton::TM::demo(seqNo);
	}
	virtual void recognition() {
		cout << seqNo << ") << adapter::recognition >>\n";
		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << adapter::refactoring >>\n";
		refactoring::bad::demo();
		refactoring::good::demo();
	}
};

} // adapter

#endif /* LECTURES_ADAPTER_H_ */

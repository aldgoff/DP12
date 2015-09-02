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

class Codec78 {
public: ~Codec78() {
		DTOR("  ~Codec78", Lecture);
	}
public:
	void spin(int mono) {
		if(mono == 1) {
			cout << "  Codec78 mono.\n";
			}
		else if(mono == 2) {
			cout << "  Codec78 stereo.\n";
		}
	}
};

void clientCode(bool mono=false) {
	Codec78 codec;
	if(mono)
		codec.spin(1);
	else
		codec.spin(2);
}

void demo(int seqNo) {
	cout << seqNo << ") << adapter::lecture::legacy::demo() >>\n";
	clientCode(true);	cout << endl;
	clientCode(false);	cout << endl;
	cout << endl;
}

}

namespace problem {

class Codec78 {
public: ~Codec78() {
		DTOR("  ~Codec78", Lecture);
	}
public:
	void spin(int mono) {
		if(mono == 1) {
			cout << "  Codec78 mono.\n";
			}
		else if(mono == 2) {
			cout << "  Codec78 stereo.\n";
		}
	}
};

class Codec45 {
public: ~Codec45() {
		DTOR("  ~Codec45", Lecture);
	}
public:
	void scan() { cout << "  Codec45 stereo.\n"; }
};

class Codec33 {
public: ~Codec33() {
		DTOR("  ~Codec33", Lecture);
	}
public:
	void transcode() { cout << "  Codec33 stereo.\n"; }
};

void clientCode(unsigned format, bool mono=false) {
	switch(format) {
	case 78: {
			Codec78 codec;
			if(mono)	codec.spin(1);
			else		codec.spin(2);
		}
		break;
	case 45: {
			Codec45 codec;
			codec.scan();
		}
		break;
	case 33: {
			Codec33 codec;
			codec.transcode();
		}
		break;
	default:
		cout << "  Oops!\n";
		break;
	}
	cout << endl;
}

void demo(int seqNo) {
	cout << seqNo << ") << adapter::lecture::problem::demo() >>\n";
	clientCode(78, true);
	clientCode(78, false);
	clientCode(45);
	clientCode(33);
	clientCode(0);
}

}

namespace solution {

using namespace problem;

class Codec {
public: virtual ~Codec() {
		DTOR("  ~Adapter\n", Lecture);
	}
public:
	virtual void decode() { cout << "  Oops!\n"; }
};
class Mono78 : public Codec {
	Codec78 codec;
public: ~Mono78() {
		DTOR("  ~Mono78 ", Lecture);
	}
public:
	void decode() { codec.spin(1); }
};
class Stereo78 : public Codec {
	Codec78 codec;
public: ~Stereo78() {
		DTOR("  ~Stereo78 ", Lecture);
	}
public:
	void decode() { codec.spin(2); }
};
class Stereo45 : public Codec {
	Codec45 codec;
public: ~Stereo45() {
		DTOR("  ~Stereo45 ", Lecture);
	}
public:
	void decode() { codec.scan(); }
};
class Stereo33 : public Codec {
	Codec33 codec;
public: ~Stereo33() {
		DTOR("  ~Stereo33 ", Lecture);
	}
public:
	void decode() { codec.transcode(); }
};
// Seam point - add another interface.

void clientCode(Codec* format) {
	format->decode();
}

void demo(int seqNo) {
	cout << seqNo << ") << adapter::lecture::solution::demo() >>\n";
	Codec* interfaces[] = {
		new Mono78,
		new Stereo78,
		new Stereo45,
		new Stereo33,
		new Codec
	};
	for(size_t i=0; i<COUNT(interfaces); i++) {
		clientCode(interfaces[i]);
	}
	for(size_t i=0; i<COUNT(interfaces); i++)
		delete interfaces[i];
	cout << endl;
}

}

namespace FM {

//	string criteria[] = { "78Mono", "78", "45", "33", "0" };

}

} // lecture

namespace homework {

#include "../Problems/adapter.h"

#include "../Solutions/adapter.h"

} // homework

namespace skeleton {

struct Legacy1 {string thisWay() {return "this way.\n";}
	~Legacy1() {
		DTOR("~Legacy1 ", Lecture);
	}
};
struct Legacy2 {string thatWay() {return "that way.\n";}
	~Legacy2() {
		DTOR("~Legacy2 ", Lecture);
	}
};
struct Legacy3 {string yourWay() {return "your way.\n";}
	~Legacy3() {
		DTOR("~Legacy3 ", Lecture);
	}
};

class Adapter { // If the interfaces are varying...
public: virtual ~Adapter() {
		DTOR("~Adapter\n", Lecture);
	}
public:
	virtual void myWay() {}
};
class Interface1 : public Adapter {
	Legacy1 wrapped;
public: ~Interface1() {
		DTOR("  ~Interface1 ", Lecture);
	}
public:
	void myWay() { cout << "  Interface1: wrapped "
						<< wrapped.thisWay(); }
};
class Interface2 : public Adapter {
	Legacy2 wrapped;
public: ~Interface2() {
		DTOR("  ~Interface2 ", Lecture);
	}
public:
	void myWay() { cout << "  Interface2: wrapped "
						<< wrapped.thatWay(); }
};
class Interface3 : public Adapter {
	Legacy3 wrapped;
public: ~Interface3() {
		DTOR("  ~Interface3 ", Lecture);
	}
public:
	void myWay() { cout << "  Interface3: wrapped "
						<< wrapped.yourWay(); }
};
// Seam point - add another interface.

void demo(int seqNo) {	// Test variations.
	Adapter* interfaces[] = {
		new Interface1,
		new Interface2,
		new Interface3
	};
	for(size_t i=0; i<COUNT(interfaces); i++) {
		interfaces[i]->myWay();
	}
	for(size_t i=0; i<COUNT(interfaces); i++) {
		delete interfaces[i];
	}
	cout << endl;
}

namespace FM {

struct Legacy1 {string thisWay() {return "this way.\n";}
	~Legacy1() {
		DTOR("~Legacy1 ", Lecture);
	}
};
struct Legacy2 {string thatWay() {return "that way.\n";}
	~Legacy2() {
		DTOR("~Legacy2 ", Lecture);
	}
};
struct Legacy3 {string yourWay() {return "your way.\n";}
	~Legacy3() {
		DTOR("~Legacy3 ", Lecture);
	}
};

class Adapter { // If the interfaces are varying...
public: virtual ~Adapter() {
		DTOR("~Adapter\n", Lecture);
	}
public:
	virtual void myWay() { cout << "  Oops!\n"; }
public:
	static Adapter* makeObject(string& criteria);
};
class Interface1 : public Adapter {
	Legacy1 wrapped;
public: ~Interface1() {
		DTOR("  ~Interface1 ", Lecture);
	}
public:
	void myWay() { cout << "  Interface1: wrapped "
						<< wrapped.thisWay(); }
};
class Interface2 : public Adapter {
	Legacy2 wrapped;
public: ~Interface2() {
		DTOR("  ~Interface2 ", Lecture);
	}
public:
	void myWay() { cout << "  Interface2: wrapped "
						<< wrapped.thatWay(); }
};
class Interface3 : public Adapter {
	Legacy3 wrapped;
public: ~Interface3() {
		DTOR("  ~Interface3 ", Lecture);
	}
public:
	void myWay() { cout << "  Interface3: wrapped "
						<< wrapped.yourWay(); }
};
// Seam point - add another interface.

Adapter* Adapter::makeObject(string& criteria) {
	if(criteria == "Interface1")	return new Interface1;
	if(criteria == "Interface2")	return new Interface2;
	if(criteria == "Interface3")	return new Interface3;
	// Seam point - insert another interface.
	return new Adapter; // Base, default, null, exception.
}

void demo(int seqNo) {	// Test variations.
	string criteria[] = { "Interface1", "Interface2",
						  "Interface3", "oops" };
	vector<Adapter*> interfaces;
	for(size_t i=0; i<COUNT(criteria); i++) {
		Adapter* interface = Adapter::makeObject(criteria[i]);
		interface->myWay();
		interfaces.push_back(interface);
	}
	for(size_t i=0; i<COUNT(criteria); i++)
		delete interfaces[i];
	cout << endl;
}

} // FM

} // skeleton

namespace recognition {

struct Pot { string boil() { return "  boil in water.\n"; } };
struct Lamp { string expose() { return "  5 min shine.\n"; } };
struct Bath { string emerse() { return "  3 min soak.\n"; } };
struct Carbonize { string burn() { return "  grill.\n"; } };

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
	if(criteria == "Pathogen")	return new Heat;
	if(criteria == "Virus")			return new UV;
	if(criteria == "ExtraTerrestrial")	return new Ozone;
	// Seam point - insert another criteria.
	throw "Infected!";	// Exception.
}

void demo() {	// Test variations.
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
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << adapter::skeleton >>\n";
		skeleton::demo(seqNo);
		skeleton::FM::demo(seqNo);
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

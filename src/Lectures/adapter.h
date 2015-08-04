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

struct Wrapped1 { string thisWay() { return "this way"; } };
struct Wrapped2 { string thatWay() { return "that way"; } };
struct Wrapped3 { string yourWay() { return "your way"; } };

class Adapter {	// If the interfaces are varying...
public: virtual ~Adapter() {}
public:
	virtual void run() { cout << "  Oops!\n"; }
public:
	static Adapter* decisionLogic(const string& data) ;
};
class Interface1 : public Adapter {
	Wrapped1 wrapped;
public:
	void run() {
		cout << "  Interface1: wrapped " << wrapped.thisWay() << ".\n";
	}
};
class Interface2 : public Adapter {
	Wrapped2 wrapped;
public:
	void run() {
		cout << "  Interface2: wrapped " << wrapped.thatWay() << ".\n";
	}
};
class Interface3 : public Adapter {
	Wrapped3 wrapped;
public:
	void run() {
		cout << "  Interface3: wrapped " << wrapped.yourWay() << ".\n";
	}
};

Adapter* Adapter::decisionLogic(const string& data) {
	if(		data == "Interface1")	return new Interface1;
	else if(data == "Interface2")	return new Interface2;
	else if(data == "Interface3")	return new Interface3;

	else {
		return new Adapter;	// Or throw exception, or a default, or an ABC.
	}
}

void demo(int seqNo) {
	string data[] = { "Interface1", "Interface2", "Interface3", "oops" };
	for(size_t i=0; i<COUNT(data); i++) {
		Adapter* interface = Adapter::decisionLogic(data[i]);
		interface->run();
	}
	cout << endl;
}

} // skeleton

class AdapterObserver : public observer::DPObserver {
public:
	AdapterObserver(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "adapter") {}
	virtual ~AdapterObserver() { DTOR("~AdapterObserver ", Architecture); }
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
	}
};

} // adapter

#endif /* LECTURES_ADAPTER_H_ */

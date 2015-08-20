/*
 * chainOfResp.h
 *
 *  Created on: Aug 15, 2015
 *      Author: aldgoff
 *
 * Varies: If the responders are varying, use the Chain of Responsibility pattern.
 *
 * Desc:
 *
 * Category: whatever
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#ChainOfReponsibility
 *  	http://www.dofactory.com/net/chain-of-responsibility-design-pattern
 *
 *  	http://sourcemaking.com/design_patterns/chain_of_responsibility
 */

#ifndef LECTURES_CHAIN_OF_RESP_H_
#define LECTURES_CHAIN_OF_RESP_H_

#include "../DP.h"

namespace chain_of_resp {	// Command line: chainOfResp [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << chain_of_resp::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << chain_of_resp::lecture::problem::demo() >>\n";
}

}

namespace solution {

class ChainOfResp {	// If the responders are varying...
public:
	ChainOfResp() {}
	virtual ~ChainOfResp() { DTOR("~ChainOfRespObserver\n", Architecture); }
public:
	virtual void run() {}
public:
	static ChainOfResp* makeObject(const string& criteria);
};
class Derived : public ChainOfResp {
public:
	Derived() {}
	virtual ~Derived() { DTOR("~Derived ", Architecture); }
public:
	void run() {}
};
// Seam point - add another Derived.

ChainOfResp* ChainOfResp::makeObject(const string& criteria) {
	if(		criteria == "whatever")	return new ChainOfResp;
	else if(criteria == "whatever")	return new ChainOfResp;
	// Seam point - add another ChainOfResp.

	else {
		return new ChainOfResp;
	}
}

void demo(int seqNo) {
	cout << seqNo << ") << chain_of_resp::lecture::solution::demo() >>\n";
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		ChainOfResp* responder = ChainOfResp::makeObject(criteria[i]);
		responder->run();
	}
	cout << endl;
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << chain_of_resp::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << chain_of_resp::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << chain_of_resp::homework::solution::demo() >>\n";
}

}

} // homework

namespace skeleton {

class ChainOfResp {	// If the responders are varying...
protected:
	ChainOfResp* successor;
public:
	ChainOfResp(ChainOfResp* successor=0):successor(successor){}
	virtual ~ChainOfResp() {
		DTOR("~ChainOfResp\n", Lecture);
	}
public:
	virtual void delegate(int criteria) {
		cout << "  The buck stops here.\n";
	}
public:
	static ChainOfResp* setupChain();
};
class Derived1 : public ChainOfResp {
public:
	Derived1(ChainOfResp* successor) : ChainOfResp(successor){}
	~Derived1() {
		DTOR("  ~Derived1 ", Lecture);
	}
public:
	void delegate(int criteria) {
		if(criteria == 1)	cout << "  Derived1 handled.";
		else 				successor->delegate(criteria);
	}
};
class Derived2 : public ChainOfResp {
public:
	Derived2(ChainOfResp* successor) : ChainOfResp(successor){}
	~Derived2() {
		DTOR("  ~Derived2 ", Lecture);
	}
public:
	void delegate(int criteria) {
		if(criteria == 2)	cout << "  Derived2 handled.";
		else 				successor->delegate(criteria);
	}
};
class Derived3 : public ChainOfResp {
public:
	Derived3(ChainOfResp* successor) : ChainOfResp(successor){}
	~Derived3() {
		DTOR("  ~Derived3 ", Lecture);
	}
public:
	void delegate(int criteria) {
		if(criteria == 3)	cout << "  Derived3 handled.";
		else 				successor->delegate(criteria);
	}
};
// Seam point - add another responder.

ChainOfResp* ChainOfResp::setupChain() {
	return
		new Derived1(
		new Derived2(
		new Derived3(
		new ChainOfResp
	)));
}

void demo() {	// Test variations.
	int criteria[] = { 1, 2, 3, 0 };
	for(size_t i=0; i<COUNT(criteria); i++) {
		ChainOfResp* responder = ChainOfResp::setupChain();
		responder->delegate(criteria[i]);
		delete responder;
	}
	cout << endl;
}

} // skeleton

namespace recognition {

class ChainOfCommand {
protected:
	ChainOfCommand* superior;
public:
	ChainOfCommand(ChainOfCommand* superior=0) : superior(superior) {}
	virtual ~ChainOfCommand() {}
public:
	virtual void passTheBuck(string& criteria) { cout<<"  Political.\n";}
	static ChainOfCommand* setup();
};
class Major : public ChainOfCommand {
public:
	Major(ChainOfCommand* superior) : ChainOfCommand(superior) {}
	void passTheBuck(string& criteria) {
		if(criteria == "Engage")	cout << "  Ready, aim, fire.\n";
		else 						superior->passTheBuck(criteria);
	}
};
class Colonal : public ChainOfCommand {
public:
	Colonal(ChainOfCommand* superior) : ChainOfCommand(superior) {}
	void passTheBuck(string& criteria) {
		if(criteria == "Tactical")	cout << "  Tactical.\n";
		else 						superior->passTheBuck(criteria);
	}
};
class General : public ChainOfCommand {
public:
	General(ChainOfCommand* superior) : ChainOfCommand(superior) {}
	void passTheBuck(string& criteria) {
		if(criteria == "Strategic")	cout << "  Strategic.\n";
		else 						superior->passTheBuck(criteria);
	}
};
// Seam point - add another officer.

ChainOfCommand* ChainOfCommand::setup() {
	return
		new Major(
		new Colonal(
		new General(
		new ChainOfCommand
	)));
}

void demo() {
	string criteria[] = { "Engage", "Tactical",
						  "Strategic", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		ChainOfCommand* officer
			= ChainOfCommand::setup();
		officer->passTheBuck(criteria[i]);
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
	: observer::DPObserver(subject, seqNo, "chainOfResp") {}
	virtual ~Observer() { DTOR("~ChainOfRespObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << chain_of_resp::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << chain_of_resp::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << chain_of_resp::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << chain_of_resp::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << chain_of_resp::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << chain_of_resp::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << chain_of_resp::skeleton >>\n";
		skeleton::demo();
	}
	virtual void recognition() {
		cout << seqNo << ") << chain_of_resp::recognition >>\n";
		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << chain_of_resp::refactoring >>\n";
		refactoring::bad::demo();
		refactoring::good::demo();
	}
};

} // chain_of_resp

#endif /* LECTURES_CHAIN_OF_RESP_H_ */

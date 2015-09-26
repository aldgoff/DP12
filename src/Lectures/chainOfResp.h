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

/* Consider a retail store that offers refunds.
 * Approval authorization depends on amount.
 * Low level employees can authorize small amounts,
 * higher level employees can authorize larger amounts.
 */

namespace legacy {

class Clerk {
public:
	Clerk() { cout << "  +Clerk\n"; }
	~Clerk() {
		DTOR("~Clerk\n", Lecture);
	}
public:
	void approves(float amount) {
		cout << "  Clerk approves $" << amount << ".\n";
	}
};

class Owner {
public:
	Owner() { cout << "  +Owner\n"; }
	~Owner() {
		DTOR("~Owner\n", Lecture);
	}
public:
	void approves(float amount) {
		cout << "  Owner approves $" << amount << ".\n";
	}
};

void clientCode(float amount) {
	{
	static Clerk	clerk;	// Client coupled with
	static Owner	owner;	// lots of classes.
	// Seam point - insert another approval class.

	if(amount < 20.00)
		clerk.approves(amount);
	else if(amount < 500.00)
		owner.approves(amount);
	// Seam point - insert another approval level.
	else
		cout << "  Denied $" << amount << ", the buck stops here.\n";
	}
}

void demo(int seqNo) {
	float data[] = {10, 44.77, 111.88, 333.66, 555.22, 999.99, 1010.55};
	for(size_t i=0; i<sizeof(data)/sizeof(*data); i++) {
		clientCode(data[i]);
	}

	cout << endl;
}

} // legacy

namespace problem {

class Clerk {
public:
	Clerk() { cout << "  +Clerk\n"; }
	~Clerk() {
		DTOR("~Clerk\n", Lecture);
	}
public:
	void approves(float amount) {
		cout << "  Clerk approves $" << amount << ".\n";
	}
};

class Supervisor {
public:
	Supervisor() { cout << "  +Supervisor\n"; }
	~Supervisor() {
		DTOR("~Supervisor\n", Lecture);
	}
public:
	void approves(float amount) {
		cout << "  Supervisor approves $" << amount << ".\n";
	}
};

class Manager {
public:
	Manager() { cout << "  +Manager\n"; }
	~Manager() {
		DTOR("~Manager\n", Lecture);
	}
public:
	void approves(float amount) {
		cout << "  Manager approves $" << amount << ".\n";
	}
};

class Director {
public:
	Director() { cout << "  +Director\n"; }
	~Director() {
		DTOR("~Director\n", Lecture);
	}
public:
	void approves(float amount) {
		cout << "  Director approves $" << amount << ".\n";
	}
};

class VP {
public:
	VP() { cout << "  +VP\n"; }
	~VP() {
		DTOR("~VP\n", Lecture);
	}
public:
	void approves(float amount) {
		cout << "  Veep approves $" << amount << ".\n";
	}
};

class President {
public:
	President() { cout << "  +President\n"; }
	~President() {
		DTOR("~President\n", Lecture);
	}
public:
	void approves(float amount) {
		cout << "  President approves $" << amount << ".\n";
	}
};

void clientCode(float amount) {
	{
	static Clerk		clerk;		// Client
	static Supervisor	supervisor;	// coupled
	static Manager		manager;	// with
	static Director		director;	// lots
	static VP			veep;		// of
	static President	president;	// classes.
	// Seam point - insert another approval class.

	if(amount < 20.00)				// Mixing
		clerk.approves(amount);		// deciding
	else if(amount < 100.00)		// with doing.
		supervisor.approves(amount);
	else if(amount < 200.00)
		manager.approves(amount);
//	else if(amount < 400.00)		// Director fired, have
//		director.approves(amount);	// to change client code.
	else if(amount < 600.00)
		veep.approves(amount);
	else if(amount < 1000.00)
		president.approves(amount);
	// Seam point - insert another approval level.

	else
		cout << "  Denied $" << amount << ", the buck stops here.\n";
	}
}

void demo(int seqNo) {
	float data[] = {10, 44.77, 111.88, 333.66, 555.22, 999.99, 1010.55};
	for(size_t i=0; i<COUNT(data); i++) {
		clientCode(data[i]);
	}

	cout << endl;
}

} // problem

namespace solution {

class Approver { // Chain of Responsibility design pattern.
protected:
	Approver* successor;
public:
	Approver() : successor(0) {}
	virtual ~Approver() { delete successor;
		DTOR("~Approver\n", Lecture);
	}
public:
	void setSuccessor(Approver* next) { successor = next; }
public:
	virtual void handleRequest(float amount) {
		cout<<"  Denied $"<<amount<<", the buck stops here.\n";
	}
public:
	static Approver* makeObject(const string& criteria);
};
class Clerk : public Approver {
public:
	Clerk() { cout << "  +Clerk\n"; }
	~Clerk() {
		DTOR("~Clerk ", Lecture);
	}
public:
	void handleRequest(float amount) {
		if(amount < 20.00)
			cout << "  Clerk approves $" << amount << ".\n";
		else successor->handleRequest(amount);
	}
};
class Supervisor : public Approver {
public:
	Supervisor() { cout << "  +Supervisor\n"; }
	~Supervisor() {
		DTOR("~Supervisor ", Lecture);
	}
public:
	void handleRequest(float amount) {
		if(amount < 100.00)
			cout << "  Supervisor approves $" << amount << ".\n";
		else successor->handleRequest(amount);
	}
};
class Manager : public Approver {
public:
	Manager() { cout << "  +Manager\n"; }
	~Manager() {
		DTOR("~Manager ", Lecture);
	}
public:
	void handleRequest(float amount) {
		if(amount < 200.00)
			cout << "  Manager approves $" << amount << ".\n";
		else successor->handleRequest(amount);
	}
};
class Director : public Approver {
public:
	Director() { cout << "  +Director\n"; }
	~Director() {
		DTOR("~Director ", Lecture);
	}
public:
	void handleRequest(float amount) {
		if(amount < 400.00)
			cout << "  Director approves $" << amount << ".\n";
		else successor->handleRequest(amount);
	}
};
class VP : public Approver {
public:
	VP() { cout << "  +VP\n"; }
	~VP() {
		DTOR("~VP ", Lecture);
	}
public:
	void handleRequest(float amount) {
		if(amount < 600.00)
			cout << "  Veep approves $" << amount << ".\n";
		else successor->handleRequest(amount);
	}
};
class President : public Approver {
public:
	President() { cout << "  +President\n"; }
	~President() {
		DTOR("~President ", Lecture);
	}
public:
	void handleRequest(float amount) {
		if(amount < 1000.00)
			cout << "  President approves $" << amount << ".\n";
		else successor->handleRequest(amount);
	}
};
// Seam point - add another Approver.

Approver* Approver::makeObject(const string& criteria) {
	if(criteria == "Clerk")			return new Clerk;
	if(criteria == "Supervisor")	return new Supervisor;
	if(criteria == "Manager")		return new Manager;
	if(criteria == "Director")		return new Director;
	if(criteria == "VP")			return new VP;
	if(criteria == "President")		return new President;
	// Seam point - add another Approver.
	throw "OOPS!"; // Base, default, null, exception.
}
Approver* SetupChain() {
	string chain[] = {
		"Clerk", "Supervisor", "Manager",
		/*"Director",*/	// Fired director handled dynamically.
		"VP", "President"
	};
	Approver* responder = Approver::makeObject(chain[0]);

	Approver* current = responder;
	for(size_t i=1; i<COUNT(chain); i++) {
		Approver* next = Approver::makeObject(chain[i]);
		current->setSuccessor(next);
		current = next;
	}
	current->setSuccessor(new Approver);

	return responder;
}

void clientCode(Approver* chain, float amount) {
	chain->handleRequest(amount);
}

void demo() { // Test variations.
	Approver* approver = SetupChain();

	float data[] = {10, 44.77, 111.88, 333.66, 555.22, 999.99, 1010.55};
	for(size_t i=0; i<COUNT(data); i++) {
		clientCode(approver, data[i]);
	}

	delete approver;
	cout << endl;
}

} // solution

} // lecture

namespace homework {

#include "../Problems/chainOfResp.h"

#include "../Solutions/chainOfResp.h"

} // homework

namespace skeleton {

class ChainOfResp {	// If the responders are varying...
protected:
	ChainOfResp* successor;
public:
	ChainOfResp() : successor(0){}
	virtual ~ChainOfResp() {
		DTOR(" ~ChainOfResp\n", Lecture);
		delete successor;
	}
public:
	void setSuccessor(ChainOfResp* next) { successor = next; }
public:
	virtual void delegate(int criteria) {
		cout << "  The buck stops here.\n";
	}
public:
	static ChainOfResp* makeObject(const string& criteria);
};
class Derived1 : public ChainOfResp {
public:
	~Derived1() {
		DTOR("  ~Derived1", Lecture);
	}
public:
	void delegate(int criteria) {
		if(criteria == 1)	cout << "  Derived1 handled.\n";
		else 				successor->delegate(criteria);
	}
};
class Derived2 : public ChainOfResp {
public:
	~Derived2() {
		DTOR("  ~Derived2", Lecture);
	}
public:
	void delegate(int criteria) {
		if(criteria == 2)	cout << "  Derived2 handled.\n";
		else 				successor->delegate(criteria);
	}
};
class Derived3 : public ChainOfResp {
public:
	~Derived3() {
		DTOR("  ~Derived3", Lecture);
	}
public:
	void delegate(int criteria) {
		if(criteria == 3)	cout << "  Derived3 handled.\n";
		else 				successor->delegate(criteria);
	}
};
// Seam point - add another responder.

ChainOfResp* ChainOfResp::makeObject(const string& criteria) {
	if(criteria == "Derived1")	return new Derived1;
	if(criteria == "Derived2")	return new Derived2;
	if(criteria == "Derived3")	return new Derived3;
	// Seam point - add another Approver.
	throw "OOPS!"; // Base, default, null, exception.
}
ChainOfResp* SetupChain() {
	string chain[] = { "Derived1", "Derived2", "Derived3" };
	ChainOfResp* responder = ChainOfResp::makeObject(chain[0]);

	ChainOfResp* current = responder;
	for(size_t i=1; i<COUNT(chain); i++) {
		ChainOfResp* next = ChainOfResp::makeObject(chain[i]);
		current->setSuccessor(next);
		current = next;
	}
	current->setSuccessor(new ChainOfResp);

	return responder;
}

void demo() {	// Test variations.
	ChainOfResp* responder = SetupChain();

	int criteria[] = { 1, 2, 3, 0 };
	for(size_t i=0; i<COUNT(criteria); i++) {
		responder->delegate(criteria[i]);
	}

	delete responder;
	cout << endl;
}

} // skeleton

namespace swpc {

class ChainOfResp {	// If the responders are varying...
protected:
	ChainOfResp* successor;
public:
	ChainOfResp(ChainOfResp* successor=0):successor(successor){}
	virtual ~ChainOfResp() {
		DTOR(" ~ChainOfResp\n", Lecture);
		delete successor;
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
		DTOR("  ~Derived1", Lecture);
	}
public:
	void delegate(int criteria) {
		if(criteria == 1)	cout << "  Derived1 handled.\n";
		else 				successor->delegate(criteria);
	}
};
class Derived2 : public ChainOfResp {
public:
	Derived2(ChainOfResp* successor) : ChainOfResp(successor){}
	~Derived2() {
		DTOR("  ~Derived2", Lecture);
	}
public:
	void delegate(int criteria) {
		if(criteria == 2)	cout << "  Derived2 handled.\n";
		else 				successor->delegate(criteria);
	}
};
class Derived3 : public ChainOfResp {
public:
	Derived3(ChainOfResp* successor) : ChainOfResp(successor){}
	~Derived3() {
		DTOR("  ~Derived3", Lecture);
	}
public:
	void delegate(int criteria) {
		if(criteria == 3)	cout << "  Derived3 handled.\n";
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
	ChainOfResp* responder = ChainOfResp::setupChain();
	for(size_t i=0; i<COUNT(criteria); i++) {
		responder->delegate(criteria[i]);
	}
	cout << endl;
	delete responder;
	cout << endl;
}

} // swpc

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
		// Seam point - insert another officer.
		new ChainOfCommand
	)));
}

void demo() {	// Test variations.
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
		lecture::solution::demo();
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << chain_of_resp::homework::legacy >>\n";
		homework::legacy::demo();
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << chain_of_resp::homework::problem >>\n";
		homework::problem::demo();
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << chain_of_resp::homework::solution >>\n";
		homework::solution::demo();
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

/*
 * observer.h
 *
 *  Created on: Aug 4, 2015
 *      Author: aldgoff
 *
 * Varies: If the listeners are varying, use the Observer pattern.
 *
 * Desc: Notify observers automatically.
 *
 * Category: Behavioral
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Observer
 *  	http://www.dofactory.com/net/observer-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter18
 *  	http://sourcemaking.com/design_patterns/observer
 */

#ifndef LECTURES_OBSERVER_H_
#define LECTURES_OBSERVER_H_

#include "../DP.h"

namespace observer {	// Command line: observer [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

/* Client wants to welcome new customers.
 * Client wants to verify address and send a welcome letter.
 * Eventually client wants to offer coupons or credit cards.
 * The Customer class is in danger of becoming coupled
 * to a growing list of welcome features.
 * The Observer pattern can be used to decouple them.
 */

namespace legacy {

class Customer;

class VerifyAddress {
public:
	void verify(Customer* customer);
};
class WelcomeLetter {
public:
	void send(Customer* customer);
};
// Seam point - add another feature.

class Customer {
	VerifyAddress	address;
	WelcomeLetter	letter;
	// Seam point - insert another feature.
public:
	const string name;
public:
	Customer(const string& name) : name(name) {}
public:
	string getState() { return "State info"; }
public:
	void welcome() {
		address.verify(this);
		letter.send(this);
		// Seam point - insert another feature.
	}
};

void VerifyAddress::verify(Customer* customer) {
	cout << "  Verify address of ";
	cout << customer->name << ".\n";
}
void WelcomeLetter::send(Customer* customer) {
	cout << "  Welcome letter to ";
	cout << customer->name << ".\n";
}

void demo(int seqNo) {
	Customer newCustomer("Sam");
	newCustomer.welcome();
	cout << endl;
}

}

namespace problem {

class Customer;

class VerifyAddress {
public:
	void verify(Customer* customer);
};
class WelcomeLetter {
public:
	void send(Customer* customer);
};
class Coupon {
public:
	void issue(Customer* customer);
};
class CreditCard {
public:
	void offer(Customer* customer);
};
// Seam point - add another feature.

class Customer {
	string name;
	VerifyAddress	address;
	WelcomeLetter	letter;
	Coupon			coupon;
	CreditCard		creditCard;
	// Seam point - insert another feature.
public:
	Customer(const string& name) : name(name) {}
public:
	string getName() { return name; }
	string getState() { return "State info"; }
public:
	void welcome() {
		address.verify(this);
		letter.send(this);
		coupon.issue(this);
		creditCard.offer(this);
		// Seam point - insert another feature.
	}
};

void VerifyAddress::verify(Customer* customer) {
	cout << "  Verify address of ";
	cout << customer->getName() << ".\n";
}
void WelcomeLetter::send(Customer* customer) {
	cout << "  Welcome letter to ";
	cout << customer->getName() << ".\n";
}
void Coupon::issue(Customer* customer) {
	cout << "  Coupon: ";
	cout << customer->getState() << ".\n";
}
void CreditCard::offer(Customer* customer) {
	cout << "  Credit card offer: ";
	cout << customer->getState() << ".\n";
}

void demo(int seqNo) {
	Customer newCustomer("Sam");
	newCustomer.welcome();
	cout << endl;
}

}

namespace solution {

class Welcome;

class Customer { // Subject in Observer design pattern.
	string name;
	list<Welcome*> observers;
public:
	Customer(const string& name) : name(name) {}
	~Customer() {
		DTOR("~Customer\n", Lecture);
		cout << "  Observers left to process (should be zero) = ";
		cout << observers.size() << ".\n";
	}
public:
	void attach(Welcome* obs) { observers.push_back(obs); }
	void detach(Welcome* obs) { observers.remove(obs); }
	string getName() { return name; }
	string getState() { return "State info"; }
public:
	void welcome();
};

class Welcome {	// Observer in Observer design pattern.
public:	virtual ~Welcome() {
		DTOR("~Welcome\n", Lecture);
	}
public:
	virtual void update(Customer* customer)=0;
public:
	static Welcome* makeObject(const string& criteria);
};
class VerifyAddress : public Welcome {
public:	virtual ~VerifyAddress() {
		DTOR("  ~VerifyAddress ", Lecture);
	}
public:
	void update(Customer* customer) {
		cout << "  Verify address of ";
		cout << customer->getName() << ".\n";
	}
};
class Letter : public Welcome {
public:	virtual ~Letter() {
		DTOR("  ~Letter ", Lecture);
	}
public:
	void update(Customer* customer) {
		cout << "  Welcome letter to ";
		cout << customer->getName() << ".\n";
	}
};
class Coupon : public Welcome {
public:	virtual ~Coupon() {
		DTOR("  ~Coupon ", Lecture);
	}
public:
	void update(Customer* customer) {
		cout << "  Coupon: ";
		cout << customer->getState() << ".\n";
	}
};
class CreditCardOffer : public Welcome {
public:	virtual ~CreditCardOffer() {
		DTOR("  ~CreditCardOffer ", Lecture);
	}
public:
	void update(Customer* customer) {
		cout << "  Credit card offer: ";
		cout << customer->getState() << ".\n";
	}
};
// Seam point - add another Observer.

Welcome* Welcome::makeObject(const string& criteria) {
	if(criteria == "VerifyAddress")   return new VerifyAddress;
	if(criteria == "Letter")		  return new Letter;
	if(criteria == "Coupon")		  return new Coupon;
	if(criteria == "CreditCardOffer") return new CreditCardOffer;
	// Seam point - insert another criteria.
	throw "Oops!";
}

void Customer::welcome() {
	list<Welcome*>::iterator it=observers.begin();
	for(; it!=observers.end(); ++it) {
		(*it)->update(this);
	}
}

void demo(int seqNo) {
	Customer* newCustomer = new Customer("Sam");
	string criteria[] = { // Observers set at run time.
		"VerifyAddress","Letter","Coupon","CreditCardOffer"
	};
	Welcome* welcome[COUNT(criteria)];
	for(size_t i=0; i<COUNT(criteria); i++) {
		welcome[i] = Welcome::makeObject(criteria[i]);
		newCustomer->attach(welcome[i]);
	}

	newCustomer->welcome();	// Notify event.

	for(size_t i=0; i<COUNT(welcome); i++) {
		newCustomer->detach(welcome[i]);
		delete welcome[i];
	}
	delete newCustomer;
	cout << endl;
}

}

} // lecture

namespace homework {

#include "../Problems/observer.h"

#include "../Solutions/observer.h"

} // homework

namespace skeleton {

class Observer;

class Subject {
	list<Observer*>	observers;
public: ~Subject() {
		DTOR("~Subject\n\n", Lecture);
	}
public:
	void attach(Observer* obs) { observers.push_back(obs); }
	void detach(Observer* obs) { observers.remove(obs); }
	void notify();
};
class Observer { // If the listeners are varying...
protected:
	Subject& subject;
public:
	Observer(Subject& target) : subject(target) {
		subject.attach(this);
	}
	virtual ~Observer() { subject.detach(this);
		DTOR("~Observer\n", Lecture);
	}
public:
	virtual void update() {}
};
class Listener1 : public Observer {
public: ~Listener1() {
		DTOR("  ~Listener1 ", Lecture);
	}
public:
	Listener1(Subject& subject) : Observer(subject) {}
	void update() { cout << "  Listener1 updating.\n"; }
};
class Listener2 : public Observer {
public: ~Listener2() {
		DTOR("  ~Listener2 ", Lecture);
	}
	Listener2(Subject& subject) : Observer(subject) {}
public:
	void update() { cout << "  Listener2 updating.\n"; }
};
class Listener3 : public Observer {
public: ~Listener3() {
		DTOR("  ~Listener3 ", Lecture);
	}
public:
	Listener3(Subject& subject) : Observer(subject) {}
	void update() { cout << "  Listener3 updating.\n"; }
};
// Seam point - add another listener.

void Subject::notify() { // The subject code.
	list<Observer*>::iterator it=observers.begin();
	for(; it!=observers.end(); ++it) {
		(*it)->update();
	}
}

void demo(int seqNo) { // Client knows the derived classes.
	Subject subject;
	Listener1 listen1(subject);
	Listener2 listen2(subject);
	Listener3 listen3(subject);
	subject.notify();
	cout << endl;
}

} // skeleton

namespace recognition {

class Resident;
class CatInTheHat {
	list<Resident*>	residents;
public:
	void attach(Resident* res) { residents.push_back(res); }
	void detach(Resident* res) { residents.remove(res); }
public:
	void makesMischief();
};
class Resident {
protected:
	CatInTheHat& perp;
public:
	Resident(CatInTheHat& demon) : perp(demon) {
		perp.attach(this);
	}
	virtual ~Resident() {
		perp.detach(this);
	}
public:
	virtual void respond() {}
};
class Boy : public Resident {
public:
	Boy(CatInTheHat& subject) : Resident(subject) {}
public:
	void respond() { cout << "  Boy laughs.\n"; }
};
class Girl : public Resident {
public:
	Girl(CatInTheHat& subject) : Resident(subject) {}
public:
	void respond() { cout << "  Girl giggles.\n"; }
};
class Fish : public Resident {
public:
	Fish(CatInTheHat& subject) : Resident(subject) {}
public:
	void respond() { cout << "  Fish complains.\n"; }
};
// Seam point - add another resident.

void CatInTheHat::makesMischief() {
	list<Resident*>::iterator it;
	for(it=residents.begin(); it!=residents.end(); ++it) {
		(*it)->respond();
	}
}

void demo() {
	CatInTheHat perp;
	new Boy(perp), new Girl(perp), new Fish(perp);
	perp.makesMischief();
	cout << endl;
}

} // recognition

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "observer") {}
	virtual ~Observer() { DTOR("~ObserverObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << observer::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << observer::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << observer::lecture::solution >>\n";
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
		cout << seqNo << ") << observer::skeleton >>\n";
		skeleton::demo(seqNo);
	}
	virtual void recognition() {
		cout << seqNo << ") << observer::recognition >>\n";
		recognition::demo();
	}
};

} // observer

#endif /* LECTURES_OBSERVER_H_ */

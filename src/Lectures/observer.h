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
 * Client wants to send a welcome letter.
 * Eventually client wants to offer coupons or credit cards.
 * The Customer class is in danger of becoming coupled
 * to a growing list of welcome features.
 * The Observer pattern can be used to decouple them.
 */

namespace legacy {

class VerifyAddress {
public:
	void verify(const string& name) {
		cout << "  Verify address of " << name << ".\n";
	}
};
class WelcomeLetter {
public:
	void send(const string& name) {
		cout << "  Welcome letter to " << name << ".\n";
	}
};
// Seam point - add another feature.

class Customer {
	string name;
	VerifyAddress	address;
	WelcomeLetter	letter;
	// Seam point - insert another feature.
public:
	Customer(const string& name) : name(name) {}
public:
	void welcome() {
		address.verify(name);
		letter.send(name);
		// Seam point - insert another feature.
	}
};

void demo(int seqNo) {
	cout<<seqNo<<") << observer::lecture::legacy::demo() >>\n";
	Customer newCustomer("Sam");
	newCustomer.welcome();
	cout << endl;
}

}

namespace problem {

class VerifyAddress {
public:
	void verify(const string& name) {
		cout << "  Verify address of " << name << ".\n";
	}
};
class WelcomeLetter {
public:
	void send(const string& name) {
		cout << "  Welcome letter to " << name << ".\n";
	}
};
class Coupon {
public:
	void issue(const string& name) {
		cout << "  Coupon: ";
		cout << "State info.\n";
	}
};
class CreditCard {
public:
	void offer(const string& name) {
		cout << "  Credit card offer: ";
		cout << "State info.\n";
	}
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
	void welcome() {
		address.verify(name);
		letter.send(name);
		coupon.issue(name);
		creditCard.offer(name);
		// Seam point - insert another feature.
	}
};

void demo(int seqNo) {
	cout<<seqNo<<") << observer::lecture::problem::demo() >>\n";
	Customer newCustomer("Sam");
	newCustomer.welcome();
	cout << endl;
}

}

namespace solution {

class Welcome;

class Customer { // Subject in Observer design pattern.
	list<Welcome*> observers;
public:
	const string name;
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
	void welcome();
	string getState() { return "State info"; }
};

class Welcome {	// Observer in Observer design pattern.
public:
	Welcome() {}
	virtual ~Welcome() {
		DTOR("~Welcome\n", Lecture);
	}
public:
	virtual void update(Customer* customer)=0;
};
class VerifyAddress : public Welcome {
public:
	VerifyAddress() {}
	virtual ~VerifyAddress() {
		DTOR("  ~VerifyAddress ", Lecture);
	}
public:
	void update(Customer* customer) {
		cout << "  Verify address of ";
		cout << customer->name << ".\n";
	}
};
class Letter : public Welcome {
public:
	Letter() {}
	virtual ~Letter() {
		DTOR("  ~Letter ", Lecture);
	}
public:
	void update(Customer* customer) {
		cout << "  Welcome letter to ";
		cout << customer->name << ".\n";
	}
};
class Coupon : public Welcome {
public:
	Coupon() {}
	virtual ~Coupon() {
		DTOR("  ~Coupon ", Lecture);
	}
public:
	void update(Customer* customer) {
		cout << "  Coupon: ";
		cout << customer->getState() << ".\n";
	}
};
class CreditCardOffer : public Welcome {
public:
	CreditCardOffer() {}
	virtual ~CreditCardOffer() {
		DTOR("  ~CreditCardOffer ", Lecture);
	}
public:
	void update(Customer* customer) {
		cout << "  Credit card offer: ";
		cout << customer->getState() << ".\n";
	}
};
// Seam point - add another Observer.

void Customer::welcome() {
	list<Welcome*>::iterator it=observers.begin();
	for(; it!=observers.end(); ++it) {
		(*it)->update(this);
	}
}

void demo(int seqNo) {
	cout<<seqNo<<") << observer::lecture::solution::demo() >>\n";
	{
		Customer newCustomer("Sam");
		Welcome* obs[] = {	// Register.
				new VerifyAddress,
				new Letter,
				new Coupon,
				new CreditCardOffer};
		for(size_t i=0; i<COUNT(obs); i++)
			newCustomer.attach(obs[i]);

		newCustomer.welcome();	// Notify.

		for(size_t i=0; i<COUNT(obs); i++) {
			newCustomer.detach(obs[i]);	delete obs[i];	}
	}
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

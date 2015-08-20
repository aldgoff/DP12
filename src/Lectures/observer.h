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

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << observer::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << observer::lecture::problem::demo() >>\n";
}

}

namespace solution {

class Observer {	// If the listeners are varying...
public:
	Observer() {}
	virtual ~Observer() { DTOR("~ObserverObserver\n", Architecture); }
public:
	virtual void run() {}
public:
	static Observer* makeObject(const string& criteria);
};
class Derived : public Observer {
public:
	Derived() {}
	virtual ~Derived() { DTOR("~Derived ", Architecture); }
public:
	void run() {}
};
// Seam point - add another Derived.

Observer* Observer::makeObject(const string& criteria) {
	if(		criteria == "whatever")	return new Observer;
	else if(criteria == "whatever")	return new Observer;
	// Seam point - add another Observer.

	else {
		return new Observer;
	}
}

void demo(int seqNo) {
	cout << seqNo << ") << observer::lecture::solution::demo() >>\n";
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Observer* listener = Observer::makeObject(criteria[i]);
		listener->run();
	}
	cout << endl;
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << observer::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << observer::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << observer::homework::solution::demo() >>\n";
}

}

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
public:
	void update() { cout << "  Listener1 updating.\n"; }
};
class Listener2 : public Observer {
public: ~Listener2() {
		DTOR("  ~Listener2 ", Lecture);
	}
public:
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
public:
	void update() { cout << "  Listener3 updating.\n"; }
};
// Seam point - add another listener.

void Subject::notify() {	// The client code.
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
		cout << seqNo << ") << observer::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << observer::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << observer::homework::solution >>\n";
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

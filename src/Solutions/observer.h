/*
 * Solutions/Observer.h
 *
 *  Created on: Sep 15, 2015
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

#ifndef SOLUTIONS_OBSERVER_H_
#define SOLUTIONS_OBSERVER_H_

#include "../macros.h"

namespace solution {

/* Consider the Dr. Seuss children's story, "The Cat in the Hat." When
 * the cat in the hat says something, things & children should respond.
 * In the approach below, the Perpetrator class has to know about all the
 * other classes. When they get changed Perpetrator has to be recompiled.
 * In addition, Perpetrator manages multiple objects, of multiple classes,
 * with multiple interfaces, which leads to long, hard to follow code.
 * Adding in the Mom and the Fish requires multiple changes in multiple
 * places, reveals multiple seam points; violates open/closed principle.
 * Use the Observer pattern to decouple the Perpetrator class.
 */

class Listener;

class Perpetrator { // Subject class in Observer DP.
	const string name;
	list<Listener*> listeners;
public:
	Perpetrator(const string& name) : name(name) {}
	~Perpetrator() {
		DTOR("~Perpetrator\n", Homework);
		DTOR("  Listeners left should be zero = ", Homework);
		char size[10];
		sprintf(size, "%d", listeners.size());
		DTOR(string(size) + ".\n", Homework);
	}
public:
	void attach(Listener* obs) {
		listeners.push_back(obs);
	}
	void detach(Listener* obs) {
		listeners.remove(obs);
	}
	void says(const string& phrase);
};

class Listener { // Observer class in Observer DP.
protected:
	string name;
public:
	Listener(string name) : name(name) {}
	virtual ~Listener() {
		string inst = name + " ~Listener\n";
		DTOR(inst, Homework);
	}
public:
	virtual void update(Perpetrator* perp) {}
};
class Thing : public Listener {
public:
	Thing(string name) : Listener(name) {}
	virtual ~Thing() {
		DTOR("  ~Thing ", Homework);
	}
public:
	void update(Perpetrator* perp) {
		cout << "    Thing " << name << " makes mischief.\n";
	}
};
class Child : public Listener {
public:
	Child(string name) : Listener(name) {}
	virtual ~Child() {
		DTOR("  ~Child ", Homework);
	}
public:
	void update(Perpetrator* perp) {
		cout << "    " << name << " laughs.\n";
	}
};
class Fish : public Listener {
public:
	Fish(string name) : Listener(name) {}
	virtual ~Fish() { name = "";
		DTOR("  ~Fish ", Homework);
	}
public:
	void update(Perpetrator* perp) {
		cout << "    " << name << " complains.\n";
	}
};
class Mom : public Listener {
public:
	Mom(string name) : Listener(name) {}
	virtual ~Mom() { name = "";
		DTOR("  ~Mom ", Homework);
	}
public:
	void update(Perpetrator* perp) {
		cout <<"    " <<name <<" asks \"how was your day?\"\n";
	}
};
// Seam point - add another Listener.

void Perpetrator::says(const string& phrase) {
	cout << "  " << name << " says " << phrase << ".\n";
	list<Listener*>::iterator it=listeners.begin();
	for(; it!=listeners.end(); ++it) {
		(*it)->update(this);
	}
}

void demo(int seqNo) {
	cout<<seqNo<<") << observer::homework::solution::demo() >>\n";
	{
	Perpetrator perp("Cat in the Hat");

	Listener*	thing1 = new Thing("1");
	Listener*	thing2 = new Thing("2");
	Listener*	boy = new Child("Boy");
	Listener*	girl = new Child("Girl");
	Listener*	fish = new Fish("Fish");
	Listener*	mom = new Mom("Mom");

	perp.attach(thing1);
	perp.attach(thing2);
	perp.says("Hello");

	perp.attach(boy);
	perp.attach(girl);
	perp.says("Let's play");

	perp.detach(thing2);	// Thing 2 leaves.
	perp.attach(fish);
	perp.says("Rock n Roll");

	perp.detach(thing1);	// Thing 1 leaves.
	perp.detach(boy);		// Children go mum.
	perp.detach(girl);
	perp.attach(mom);
	perp.says("Bye");

	perp.detach(fish);
	perp.detach(mom);

	delete thing1;	delete thing2;
	delete boy;		delete girl;
	delete fish;	delete mom;
	}
	cout << endl;
}

} // solution

#endif /* SOLUTIONS_OBSERVER_H_ */

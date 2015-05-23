/*
 * headfirst.h
 *
 *  Created on: May 11, 2015
 *      Author: aldgoff
 *
 * Varies: If the whatever are varying, use the DPTemplate pattern.
 *
 * Desc:
 *
 * Category: whatever
 *
 *  URLs:
 *  	http://www.dcc.ufrj.br/~comp2/TextosJava/Head%20First%20-%20Design%20Patterns.pdf
 */

#ifndef LECTURES_HEADFIRST_H_
#define LECTURES_HEADFIRST_H_

#include "../DP.h"

namespace headfirst {	// Command line: headfirst [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

// Chapter 1: The duck example used the Strategy pattern.

class Fly {
public:
	Fly() {}
	virtual ~Fly() { DTOR("~Fly ", Lecture); }
public:
	virtual void fly() { cout << "  Fly.\n"; }
public:
	static Fly* create(string choice);
};
class DontFly : public Fly {
public:
	DontFly() {}
	virtual ~DontFly() { DTOR("~DontFly ", Lecture); }
public:
	void fly() { cout << "  Don't fly.\n"; }
};
class FlyThisWay : public Fly {
public:
	FlyThisWay() {}
	virtual ~FlyThisWay() { DTOR("~FlyThisWay ", Lecture); }
public:
	void fly() { cout << "  Fly this way.\n"; }
};
class FlyThatWay : public Fly {
public:
	FlyThatWay() {}
	virtual ~FlyThatWay() { DTOR("~FlyThatWay ", Lecture); }
public:
	void fly() { cout << "  Fly that way.\n"; }
};
class BrokenWing : public Fly {
public:
	BrokenWing() {}
	virtual ~BrokenWing() { DTOR("~BrokenWing ", Lecture); }
public:
	void fly() { cout << "  Can't fly with broken wing.\n"; }
};
// Seam point - add another Fly.

Fly* Fly::create(string choice) {
	if(		choice == "DontFly")		return new DontFly;
	else if(choice == "FlyThisWay")		return new FlyThisWay;
	else if(choice == "FlyThatWay")		return new FlyThatWay;
	else if(choice == "BrokenWing")		return new BrokenWing;
	// Seam point - add another Fly.
	else {
		return new Fly;
	}
}

class Quack {
public:
	Quack() {}
	virtual ~Quack() { DTOR("~Quack ", Lecture); }
public:
	virtual void quack() { cout << "  Quack.\n"; }
public:
	static Quack* create(string choice);
};
class Quackle : public Quack {
public:
	Quackle() {}
	virtual ~Quackle() { DTOR("~Quackle ", Lecture); }
public:
	void quack() { cout << "  Quackle.\n"; }
};
class Squeak : public Quack {
public:
	Squeak() {}
	virtual ~Squeak() { DTOR("~Squeak ", Lecture); }
public:
	void quack() { cout << "  Squeak.\n"; }
};
class Silent : public Quack {
public:
	Silent() {}
	virtual ~Silent() { DTOR("~Silent ", Lecture); }
public:
	void quack() { cout << "  Silent.\n"; }
};
// Seam point - add another Quack.

Quack* Quack::create(string choice) {
	if(		choice == "Quackle")	return new Quackle;
	else if(choice == "Squeak")		return new Squeak;
	else if(choice == "Silent")		return new Silent;
	// Seam point - add another Quack.
	else {
		return new Quack;
	}
}

class Duck {
protected:
	Fly*	fly;
	Quack*	quack;
public:
	Duck(Fly* fly = new Fly, Quack* quack = new Quack)
	  :	fly(fly), quack(quack) {}
	virtual ~Duck() {
		delete quack;
		delete fly;
		DTOR("~Duck\n", Lecture);
	}
public:
	void swim() {}
public:
	void setFly(Fly* fly) { delete Duck::fly; Duck::fly = fly; }
	void setQuack(Quack* quack) {delete Duck::quack; Duck::quack = quack; }
public:
	void behaviors() {
		fly->fly();
		quack->quack();
		cout << endl;
	}
public:
	virtual void display() {	// Fine, all ducks must be displayed.
		cout << "  Display whatever duck.\n";
	}
//	virtual void quack() {}		// Not all quack.
//	virtual void fly() {}		// Not all fly.
public:
	static Duck* create(string choice);
};
class Mallard : public Duck {
public:
	Mallard()
	  :	Duck(Fly::create("FlyThisWay"), Quack::create("Quackle")) {}
	virtual ~Mallard() { DTOR("~Mallard ", Lecture); }
public:
	void display() {
		cout << "  Display Mallard duck.\n";
		Duck::behaviors();
	}
};
class Redhead : public Duck {
public:
	Redhead()
	  :	Duck(Fly::create("FlyThatWay"), Quack::create("Quackle")) {}
	virtual ~Redhead() { DTOR("~Redhead ", Lecture); }
public:
	void display() {
		cout << "  Display Redhead duck.\n";
		Duck::behaviors();
	}
};
class Rubber : public Duck {
public:
	Rubber()
	  :	Duck(Fly::create("DontFly"), Quack::create("Squeak")) {}
	virtual ~Rubber() { DTOR("~Rubber ", Lecture); }
public:
	void display() {
		cout << "  Display Rubber duck.\n";
		Duck::behaviors();
	}
};
class Decoy : public Duck {
public:
	Decoy()
	  :	Duck(Fly::create("DontFly"), Quack::create("Silent")) {}
	virtual ~Decoy() { DTOR("~Decoy ", Lecture); }
public:
	void display() {
		cout << "  Display Decoy duck.\n";
		Duck::behaviors();
	}
};
// Seam point - add another Duck.

Duck* Duck::create(string choice) {
	if(		choice == "Mallard")	return new Mallard;
	else if(choice == "Redhead")	return new Redhead;
	else if(choice == "Rubber")		return new Rubber;
	else if(choice == "Decoy")		return new Decoy;
	// Seam point - add another Duck.
	else {
		return new Duck;
	}
}

void clientCode(Duck* duck) {
	duck->display();	// Polymorphic use.
}

void demo(int seqNo) {
	// Client code decision logic chooses the concrete duck, but as a string.
	string choices[] = { "Mallard", "Redhead", "Rubber", "Decoy"};
	vector<Duck*>	ducks;

	for(size_t i=0; i<sizeof(choices)/sizeof(*choices); i++) {
		ducks.push_back(Duck::create(choices[i]));
		clientCode(ducks[i]);
	}

	// Runtime example.
	Duck* duck = ducks[0];
	duck->setFly(Fly::create("BrokenWing"));
	cout << endl;
	duck->display();

	for(size_t i=0; i<sizeof(choices)/sizeof(*choices); i++) {
		delete ducks[i];
	}
	cout << endl;
}

/* Note the loose coupling:
 * Client has to know only about Duck and string.
 * Duck has to know only about its derived types, and Fly & Quack.
 * Fly has to know only about its derived types.
 * Quack has to know only about its derived types.
 */

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << headfirst::lecture::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << headfirst::lecture::solution::demo() >>\n";
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << headfirst::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << headfirst::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << headfirst::homework::solution::demo() >>\n";
}

}

} // homework

class HeadfirstObserver : public observer::DPObserver {
public:
	HeadfirstObserver(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "headfirst") {}
	virtual ~HeadfirstObserver() { DTOR("~HeadfirstObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << headfirst::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << headfirst::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << headfirst::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << headfirst::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << headfirst::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << headfirst::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
};

} // headfirst

#endif /* LECTURES_HEADFIRST_H_ */

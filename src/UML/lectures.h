/*
 * UML/lectures.h
 *
 *  Created on: Aug 5, 2015
 *      Author: aldgoff
 */

#ifndef UML_LECTURES_H_
#define UML_LECTURES_H_

#include "../DP.h"

namespace u {


namespace k { // Strategy

class Strategy {	// If the algorithms are varying...
public:	virtual ~Strategy() {}
public:
	virtual void compute() {}
};
class Algorithm1 : public Strategy {
public:
	void compute() { cout << "  Algorithm1\n"; }
};
class Algorithm2 : public Strategy {
public:
	void compute() { cout << "  Algorithm2\n"; }
};
class Algorithm3 : public Strategy {
public:
	void compute() { cout << "  Algorithm3\n"; }
};

class Base {	// If the algorithms are varying...
public:	virtual ~Base() {}
public:
	virtual void method() {}
};
class Derived1 : public Base {
public:
	void method() { cout << "  Derived1\n"; }
};
class Derived2 : public Base {
public:
	void method() { cout << "  Derived2\n"; }
};
class Derived3 : public Base {
public:
	void method() { cout << "  Derived3\n"; }
};

} // k

namespace l { // Adapter

struct Wrapped1 { string thisWay() { return "this way.\n"; } };
struct Wrapped2 { string thatWay() { return "that way.\n"; } };
struct Wrapped3 { string yourWay() { return "your way.\n"; } };

class Adapter {	// If the interfaces are varying...
public: virtual ~Adapter() {}
public:
	virtual void method() {}
};
class Interface1 : public Adapter {
	Wrapped1 wrapped;
public:
	void method() { cout << "  Interface1: wrapped " << wrapped.thisWay(); }
};
class Interface2 : public Adapter {
	Wrapped2 wrapped;
public:
	void method() { cout << "  Interface2: wrapped " << wrapped.thatWay(); }
};
class Interface3 : public Adapter {
	Wrapped3 wrapped;
public:
	void method() { cout << "  Interface3: wrapped " << wrapped.yourWay(); }
};

struct WarpDrive { string bend() { return "this way.\n"; } };
struct Impulse { string push() { return "that way.\n"; } };
struct Ion { string reaction() { return "your way.\n"; } };

class SciFi {	// If the interfaces are varying...
public: virtual ~SciFi() {}
public:
	virtual void method() {}
};
class FTL : public SciFi {
	WarpDrive warp;
public:
	///
	/// \return void
	void method() { cout << "  FTL: warp " << warp.bend(); }
};
class Scoot : public SciFi {
	Impulse impulse;
public:
	void method() { cout << "  Scoot: impulse " << impulse.push(); }
};
class Lumber : public SciFi {
	Ion thrust;
public:
	void method() { cout << "  Lumber: thrust " << thrust.reaction(); }
};

} // iam

namespace m { // Factory Method

class FactoryMethod {	// If the classes are varying...
public: virtual ~FactoryMethod() {}
public:
	virtual void methods() { cout << "  Oops!\n"; }
public:
	static FactoryMethod* makeObject(const string& criteria);
};
class Type1 : public FactoryMethod {
public:
	void methods() { cout << "  Type1\n"; }
};
class Type2 : public FactoryMethod {
public:
	void methods() { cout << "  Type2\n"; }
};
class Type3 : public FactoryMethod {
public:
	void methods() { cout << "  Type3\n"; }
};

class Base {	// If the classes are varying...
public: virtual ~Base() {}
public:
	virtual void methods() { cout << "  Oops!\n"; }
public:
	static Base* makeObject(const string& criteria);
};
class Derived1 : public Base {
public:
	void methods() { cout << "  Derived1\n"; }
};
class Derived2 : public Base {
public:
	void methods() { cout << "  Derived2\n"; }
};
class Derived3 : public Base {
public:
	void methods() { cout << "  Derived3\n"; }
};

} // mam


namespace n { // Template Method

class TemplateMethod {	// If the steps are varying...
public: virtual ~TemplateMethod() {}
public:
	virtual void run() {
		cout << "  run - " << sameStep1();
		cout << " - " << sameStep2();
		cout << " - " << diffStep3();
		cout << " - " << sameStep4();
		cout << " - " << sameStep5();
		cout << endl;
	}
protected:
	string sameStep1() { return "step1"; }
	string sameStep2() { return "step2"; }
	virtual string diffStep3() { return "oops!"; }
	string sameStep4() { return "step4"; }
	string sameStep5() { return "step5"; }
public:
	static TemplateMethod* makeObject(const string& criteria);
};
class Diff1Step3 : public TemplateMethod {
public:
	string diffStep3() { return "Diff1.step3"; }
};
class Diff2Step3 : public TemplateMethod {
public:
	string diffStep3() { return "Diff2.step3"; }
};
class Diff3Step3 : public TemplateMethod {
public:
	string diffStep3() { return "Diff3.step3"; }
};

class Base {	// If the steps are varying...
public: virtual ~Base() {}
public:
	virtual void run() {
		cout << "  run - " << open();
		cout << " - " << setup();
		cout << " - " << play();
		cout << " - " << score();
		cout << " - " << putaway();
		cout << endl;
	}
protected:
	string open() { return "step1"; }
	string setup() { return "step2"; }
	virtual string play() { return "oops!"; }
	string score() { return "step4"; }
	string putaway() { return "step5"; }
public:
	static Base* makeObject(const string& criteria);
};
class Derived1 : public Base {
public:
	string play() { return "Derived1.step3"; }
};
class Derived2 : public Base {
public:
	string play() { return "Derived2.step3"; }
};
class Derived3 : public Base {
public:
	string play() { return "Derived3.step3"; }
};

} // tam

namespace o { // Decorator

class Component {
public: virtual ~Component() {}
public:
	virtual void behavior()=0;
};
class Base : public Component {	// Base object to be decorated.
public:
	void behavior() { cout << "  Base"; }
};
class Decorator : public Component {	// If the options are varying...
protected:
	Component* decorator;
public:
	Decorator(Component* decorator) : decorator(decorator) {}
public:
	void behavior() {}
public:
	static Component* makeObject(Component* thing, const string& criteria);
};
class Option1 : public Decorator {
public:
	Option1(Component* decorator) : Decorator(decorator) {}
public:
	void behavior() { decorator->behavior(); cout << " Opt1"; }
};
class Option2 : public Decorator {
public:
	Option2(Component* decorator) : Decorator(decorator) {}
public:
	void behavior() { decorator->behavior(); cout << " Opt2"; }
};
class Option3 : public Decorator {
public:
	Option3(Component* decorator) : Decorator(decorator) {}
public:
	void behavior() { decorator->behavior(); cout << " Opt3"; }
};

class WI {
public: virtual ~WI() {}
public:
	virtual void respond()=0;
};
class Window : public WI {	// Window object to be decorated.
public:
	void respond() { cout << "  Window"; }
};
class Control : public WI {	// If the options are varying...
protected:
	WI* control;
public:
	Control(WI* control) : control(control) {}
public:
	void respond() {}
public:
	static WI* makeObject(WI* thing, const string& criteria);
};
class HScrollBar : public Control {
public:
	HScrollBar(WI* control) : Control(control) {}
public:
	void respond() { control->respond(); cout << " Opt1"; }
};
class VScrollBar : public Control {
public:
	VScrollBar(WI* control) : Control(control) {}
public:
	void respond() { control->respond(); cout << " Opt2"; }
};
class ToolBar : public Control {
public:
	ToolBar(WI* control) : Control(control) {}
public:
	void respond() { control->respond(); cout << " Opt3"; }
};

} // dam

namespace p { // Observer

class Observer;

class Subject {
	list<Observer*>	observers;
public:
	void attach(Observer* obs) { observers.push_back(obs); }
	void detach(Observer* obs) { observers.remove(obs); }
public:
	void notify();
};
class Observer {	// If the listeners are varying...
protected:
	Subject& subject;
public:
	Observer(Subject& target) : subject(target) {
		subject.attach(this);
	}
	virtual ~Observer() {
		subject.detach(this);
	}
public:
	virtual void update() {}
};
class Listener1 : public Observer {
public:
	Listener1(Subject& subject) : Observer(subject) {}
public:
	void update() { cout << "  Listener1 updating\n"; }
};
class Listener2 : public Observer {
public:
	Listener2(Subject& subject) : Observer(subject) {}
public:
	void update() { cout << "  Listener2 updating\n"; }
};
class Listener3 : public Observer {
public:
	Listener3(Subject& subject) : Observer(subject) {}
public:
	void update() { cout << "  Listener3 updating\n"; }
};

class Hobbit;

class Gandalf {
	list<Hobbit*>	hobbits;
public:
	void volunteer(Hobbit* obs) { hobbits.push_back(obs); }
	void runaway(Hobbit* obs) { hobbits.remove(obs); }
public:
	void speek();
};
class Hobbit {	// If the listeners are varying...
protected:
	Gandalf& wizard;
public:
	Hobbit(Gandalf& target) : wizard(target) {
		wizard.volunteer(this);
	}
	virtual ~Hobbit() {
		wizard.runaway(this);
	}
public:
	virtual void listen() {}
};
class Bilbo : public Hobbit {
public:
	Bilbo(Gandalf& subject) : Hobbit(subject) {}
public:
	void listen() { cout << "  Bilbo updating\n"; }
};
class Samwise : public Hobbit {
public:
	Samwise(Gandalf& subject) : Hobbit(subject) {}
public:
	void listen() { cout << "  Samwise updating\n"; }
};
class Gollum : public Hobbit {
public:
	Gollum(Gandalf& subject) : Hobbit(subject) {}
public:
	void listen() { cout << "  Gollum updating\n"; }
};

} // som


namespace q { // Chain of Responsibility

} // cam

namespace r { // Bridge

} // bam

namespace s { // Abstract Factory

} // fam


} // u


#endif /* UML_LECTURES_H_ */

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

class Sort {	// If the algorithms are varying...
public:	virtual ~Sort() {}
public:
	virtual void order() {}
};
class Linear : public Sort {
public:
	void order() { cout << "  Linear\n"; }
};
class Parallel : public Sort {
public:
	void order() { cout << "  Parallel\n"; }
};
class Asymptotic : public Sort {
public:
	void order() { cout << "  Asymptotic\n"; }
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
	virtual void makeItSo() {}
};
class FTL : public SciFi {
	WarpDrive warp;
public:
	void makeItSo() { cout << "  FTL: warp " << warp.bend(); }
};
class Scoot : public SciFi {
	Impulse impulse;
public:
	void makeItSo() { cout << "  Scoot: impulse " << impulse.push(); }
};
class Lumber : public SciFi {
	Ion thrust;
public:
	void makeItSo() { cout << "  Lumber: thrust " << thrust.reaction(); }
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

class Politician {	// If the classes are varying...
public: virtual ~Politician() {}
public:
	virtual void promises() { cout << "  Oops!\n"; }
public:
	static Politician* makeObject(const string& criteria);
};
class Republican : public Politician {
public:
	void promises() { cout << "  Republican\n"; }
};
class Democrat : public Politician {
public:
	void promises() { cout << "  Democrat\n"; }
};
class Indpendent : public Politician {
public:
	void promises() { cout << "  Indpendent\n"; }
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

class Game {	// If the steps are varying...
public: virtual ~Game() {}
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
	string open() { return "open"; }
	string setup() { return "setup"; }
	virtual string play() { return "oops!"; }
	string score() { return "score"; }
	string putaway() { return "putaway"; }
public:
	static Game* makeObject(const string& criteria);
};
class Checkers : public Game {
public:
	string play() { return "Checkers.play"; }
};
class Chess : public Game {
public:
	string play() { return "Chess.play"; }
};
class Go : public Game {
public:
	string play() { return "Go.play"; }
};

} // tam

namespace o { // Decorator

class Component {
public: virtual ~Component() {}
public:
	virtual void behavior()=0;
};
class Base : public Component {		// Base object to be decorated.
public:
	void behavior() { cout << "  Base"; }
};
class Decorator : public Component {// If the options are varying...
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
	void speak();
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

class ChainOfResponsibility {	// If the responders are varying...
protected:
	ChainOfResponsibility* successor;
public:
	ChainOfResponsibility(ChainOfResponsibility* successor=0) : successor(successor) {}
	virtual ~ChainOfResponsibility() { delete successor; }
public:
	virtual void delegate(const string& criteria) {
		cout << "  I give up, unknown responder.";
	}
public:
	static ChainOfResponsibility* setupChain();
};
class Responder1 : public ChainOfResponsibility {
public:
	Responder1(ChainOfResponsibility* successor=0) : ChainOfResponsibility(successor) {}
public:
	void delegate(const string& criteria) {
		if(criteria == "Responder1")	cout << "  Responder1 handling.\n";
		else							successor->delegate(criteria);
	}
};
class Responder2 : public ChainOfResponsibility {
public:
	Responder2(ChainOfResponsibility* successor=0) : ChainOfResponsibility(successor) {}
public:
	void delegate(const string& criteria) {
		if(criteria == "Responder2")	cout << "  Responder2 handling.\n";
		else							successor->delegate(criteria);
	}
};
class Responder3 : public ChainOfResponsibility {
public:
	Responder3(ChainOfResponsibility* successor=0) : ChainOfResponsibility(successor) {}
public:
	void delegate(const string& criteria) {
		if(criteria == "Responder3")	cout << "  Responder3 handling.\n";
		else							successor->delegate(criteria);
	}
};

ChainOfResponsibility* ChainOfResponsibility::setupChain() {
	return
		new Responder1(
		new Responder2(
		new Responder3(
		new ChainOfResponsibility
	)));
}

class Language {	// If the responders are varying...
protected:
	Language* successor;
public:
	Language(Language* successor=0) : successor(successor) {}
	virtual ~Language() { delete successor; }
public:
	virtual void checkGrammar(const string& criteria) {
		cout << "  I give up, unknown language.";
	}
public:
	static Language* setupChain();
};
class English : public Language {
public:
	English(Language* successor=0) : Language(successor) {}
public:
	void checkGrammar(const string& criteria) {
		if(criteria == "English")	cout << "  Parsing English.\n";
		else						successor->checkGrammar(criteria);
	}
};
class French : public Language {
public:
	French(Language* successor=0) : Language(successor) {}
public:
	void checkGrammar(const string& criteria) {
		if(criteria == "French")	cout << "  Parsing French.\n";
		else						successor->checkGrammar(criteria);
	}
};
class German : public Language {
public:
	German(Language* successor=0) : Language(successor) {}
public:
	void checkGrammar(const string& criteria) {
		if(criteria == "German")	cout << "  Parsing German.\n";
		else						successor->checkGrammar(criteria);
	}
};

Language* Language::setupChain() {
	return
		new English(
		new French(
		new German(
		new Language
	)));
}

} // cam

namespace r { // Bridge

class Implementation {	// If the implementations are varying...
public: virtual ~Implementation() {}
public:
	virtual void execute() {}
public:
	static Implementation* makeObject(const string& criteria);
};
class Implement1 : public Implementation {
public:
	void execute() {}
};
class Implement2 : public Implementation {
public:
	void execute() {}
};
class Implement3 : public Implementation {
public:
	void execute() {}
};

Implementation* Implementation::makeObject(const string& criteria) {
	if(criteria == "Implement1")	return new Implement1;
	if(criteria == "Implement2")	return new Implement2;
	if(criteria == "Implement3")	return new Implement3;
	return new Implementation;
}

class Abstraction {	// If the abstractions are varying...
protected:
	Implementation* implement;
public:
	Abstraction(Implementation* implement=0) : implement(implement) {}
	virtual ~Abstraction() { delete implement; }
public:
	virtual void run() {}
public:
	static Abstraction* makeObject(const string& criteria, Implementation* implement);
};
class Abstract1 : public Abstraction {
public:
	Abstract1(Implementation* implement) : Abstraction(implement) {}
public:
	void run() { implement->execute(); }
};
class Abstract2 : public Abstraction {
public:
	Abstract2(Implementation* implement) : Abstraction(implement) {}
public:
	void run() { implement->execute(); }
};
class Abstract3 : public Abstraction {
public:
	Abstract3(Implementation* implement) : Abstraction(implement) {}
public:
	void run() { implement->execute(); }
};

Abstraction* Abstraction::makeObject(const string& criteria, Implementation* implement) {
	if(criteria == "Abstract1")	return new Abstract1(implement);
	if(criteria == "Abstract2")	return new Abstract2(implement);
	if(criteria == "Abstract3")	return new Abstract3(implement);
	return new Abstraction;
}

class Light {	// If the implementations are varying...
public: virtual ~Light() {}
public:
	virtual void shine() {}
public:
	static Light* makeObject(const string& criteria);
};
class Incandescent : public Light {
public:
	void shine() {}
};
class Halogen : public Light {
public:
	void shine() {}
};
class LED : public Light {
public:
	void shine() {}
};

Light* Light::makeObject(const string& criteria) {
	if(criteria == "Incandescent")	return new Incandescent;
	if(criteria == "Halogen")	return new Halogen;
	if(criteria == "LED")	return new LED;
	return new Light;
}

class Mood {	// If the abstractions are varying...
protected:
	Light* light;
public:
	Mood(Light* light=0) : light(light) {}
	virtual ~Mood() { delete light; }
public:
	virtual void illuminate() {}
public:
	static Mood* makeObject(const string& criteria, Light* implement);
};
class Daylight : public Mood {
public:
	Daylight(Light* implement) : Mood(implement) {}
public:
	void illuminate() { light->shine(); }
};
class Disco : public Mood {
public:
	Disco(Light* implement) : Mood(implement) {}
public:
	void illuminate() { light->shine(); }
};
class Romantic : public Mood {
public:
	Romantic(Light* implement) : Mood(implement) {}
public:
	void illuminate() { light->shine(); }
};

Mood* Mood::makeObject(const string& criteria, Light* implement) {
	if(criteria == "Daylight")	return new Daylight(implement);
	if(criteria == "Disco")	return new Disco(implement);
	if(criteria == "Romantic")	return new Romantic(implement);
	return new Mood;
}

} // bam

namespace s { // Abstract Factory

class Type1 {
public: virtual ~Type1() {}
public:
	virtual void run() {}
};
class Type1A : public Type1 {
public:
	void run() {}
};
class Type1B : public Type1 {
public:
	void run() {}
};
class Type1C : public Type1 {
public:
	void run() {}
};

class Type2 {
public: virtual ~Type2() {}
public:
	virtual void calc() {}
};
class Type2A : public Type2 {
public:
	void calc() {}
};
class Type2B : public Type2 {
public:
	void calc() {}
};
class Type2C : public Type2 {
public:
	void calc() {}
};

class Type3 {
public: virtual ~Type3() {}
public:
	virtual void compute() {}
};
class Type3A : public Type3 {
public:
	void compute() {}
};
class Type3B : public Type3 {
public:
	void compute() {}
};
class Type3C : public Type3 {
public:
	void compute() {}
};

class AbstractFactory {	// If whole families are varying...
public: virtual ~AbstractFactory() {}
public:
	virtual Type1* createType1() { return new Type1; }
	virtual Type2* createType2() { return new Type2; }
	virtual Type3* createType3() { return new Type3; }
public:
	static AbstractFactory* makeObject(const string& criteria);
};
class FamilyA : public AbstractFactory {
public:
	Type1* createType1() { return new Type1A; }
	Type2* createType2() { return new Type2A; }
	Type3* createType3() { return new Type3A; }
};
class FamilyB : public AbstractFactory {
public:
	Type1* createType1() { return new Type1B; }
	Type2* createType2() { return new Type2B; }
	Type3* createType3() { return new Type3B; }
};
class FamilyC : public AbstractFactory {
public:
	Type1* createType1() { return new Type1C; }
	Type2* createType2() { return new Type2C; }
	Type3* createType3() { return new Type3C; }
};

AbstractFactory* AbstractFactory::makeObject(const string& criteria) {
	if(criteria == "FamilyA")	return new FamilyA;
	if(criteria == "FamilyB")	return new FamilyB;
	if(criteria == "FamilyC")	return new FamilyC;
	return new AbstractFactory;
}

} // fam

namespace t { // Abstract Factory

class IJM {
public: virtual ~IJM() {}
public:
	virtual void run() {}
};
class IJM1 : public IJM {
public:
	void run() {}
};
class IJM2 : public IJM {
public:
	void run() {}
};
class IJM4 : public IJM {
public:
	void run() {}
};

class Belt {
public: virtual ~Belt() {}
public:
	virtual void calc() {}
};
class Linear : public Belt {
public:
	void calc() {}
};
class YSplit : public Belt {
public:
	void calc() {}
};
class VSplit : public Belt {
public:
	void calc() {}
};

class Box {
public: virtual ~Box() {}
public:
	virtual void compute() {}
};
class Cardboard : public Box {
public:
	void compute() {}
};
class Pallot : public Box {
public:
	void compute() {}
};
class Crate : public Box {
public:
	void compute() {}
};

class InjectionLine {	// If whole families are varying...
public: virtual ~InjectionLine() {}
public:
	virtual IJM* createIJM() { return new IJM; }
	virtual Belt* createBelt() { return new Belt; }
	virtual Box* createBox() { return new Box; }
public:
	static InjectionLine* makeObject(const string& criteria);
};
class Cavity1 : public InjectionLine {
public:
	IJM* createIJM() { return new IJM1; }
	Belt* createBelt() { return new Linear; }
	Box* createBox() { return new Cardboard; }
};
class Cavity2 : public InjectionLine {
public:
	IJM* createIJM() { return new IJM2; }
	Belt* createBelt() { return new YSplit; }
	Box* createBox() { return new Pallot; }
};
class Cavity4 : public InjectionLine {
public:
	IJM* createIJM() { return new IJM4; }
	Belt* createBelt() { return new VSplit; }
	Box* createBox() { return new Crate; }
};

InjectionLine* InjectionLine::makeObject(const string& criteria) {
	if(criteria == "Cavity1")	return new Cavity1;
	if(criteria == "Cavity2")	return new Cavity2;
	if(criteria == "Cavity4")	return new Cavity4;
	return new InjectionLine;
}

} // fam


} // u


#endif /* UML_LECTURES_H_ */

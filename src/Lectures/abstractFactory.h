/*
 * abstractFactory.h
 *
 *  Created on: Aug 15, 2015
 *      Author: aldgoff
 *
 * Varies: If the families are varying, use the Abstract Factory pattern.
 *
 * Desc: Provide an interface for creating families of related or
 * 		 dependent objects without specifying their concrete classes.
 *
 * Category: Creational
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Abstract_Factory
 *  	http://www.dofactory.com/net/abstract-factory-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter11
 *  	http://sourcemaking.com/design_patterns/abstract_factory
 */

#ifndef LECTURES_ABSTRACTFACTORY_H_
#define LECTURES_ABSTRACTFACTORY_H_

#include "../DP.h"

namespace abstract_factory {	// Command line: abstractFactory [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

/* Consider the Fast Hierarchy, a schema for creating algebras:
 *   0) Counting
 *   1) Addition - fast counting
 *   2) Multiplication - fast addition
 *   3) Exponentiation - fast multiplication
 * At every level we need the correct instance from each of these families:
 *   1) Identity element (like zero & one)
 *   2) Scale progression (tick mark, arithmetic, geometric, exponential)
 *   	1. inc
 *   	2. dec
 *   3) Forward operation (|, +, *, ^)
 *   	1. op
 *   	2. symbol
 *   4) Inverse operation (~, -, /, r)
 *   	1. op
 *   	2. symbol
 * We'll use the Abstract Factory design pattern to create the right factory
 * that the client can use to create the correct instance of each family
 * and then execute example operations demonstrating that creation has
 * occurred as intended.
 * All destructors will be instrumented to confirm the structure of the
 * class hierarchy.
 * We'll skip the legacy and problem precursors.
 */

namespace legacy { // Not used.

void demo(int seqNo) {
	cout << seqNo << ") << abstract_factory::lecture::legacy::demo() >>\n";
}

} // legacy

namespace problem { // Not used.

void demo(int seqNo) {
	cout << seqNo << ") << abstract_factory::lecture::problem::demo() >>\n";
}

} // problem

namespace solution {

class Identity { // An interface class in the AF DP.
public: virtual ~Identity() {
	DTOR("~Identity ", Lecture);
}
public:
	virtual string value() { return "[ ]"; }
};
class Nothing : public Identity {
public: virtual ~Nothing() {
	DTOR("~Nothing ", Lecture);
}
public:
	string value() { return "[ ]"; }
};
class Zero : public Identity {
public: virtual ~Zero() {
	DTOR("~Zero ", Lecture);
}
public:
	string value() { return "[0]"; }
};
class One : public Identity {
public: virtual ~One() {
	DTOR("~One ", Lecture);
}
public:
	string value() { return "[1.0]"; }
};
class Range : public Identity {
public: virtual ~Range() {
	DTOR("~Range ", Lecture);
}
public:
	string value() { return "[2.0, x-root-x]"; } // Math fudge.
};

class Scale { // An interface class in the AF DP.
protected:
	char str[80];
public: virtual ~Scale() {
	DTOR("~Scale ", Lecture);
}
public:
	virtual double inc(double x) { return x; }
	virtual double dec(double x) { return x; }
	virtual string convert(double x) { return ""; }
};
class Ticks : public Scale {
public: ~Ticks() {
	DTOR("~Ticks ", Lecture);
}
public:
	double inc(double x) { return x + 1; }
	double dec(double x) { return (x>0) ? x - 1 : -1; }
	string convert(double x) {
		string ticks = "";
		if(x <= 0)	return "_";
		for(int i=0; i<x; i++)
			ticks += "|";
		return ticks;
	}
};
class Successor : public Scale {
public: ~Successor() {
	DTOR("~Successor ", Lecture);
}
public:
	double inc(double x) { return x + 1; }
	double dec(double x) { return x - 1; }
	string convert(double x) {
		sprintf(str, "%.0f", x);
		return str;
	}
};
class Product : public Scale {
public: ~Product() {
	DTOR("~Product ", Lecture);
}
public:
	double inc(double x) { return x * 2; }
	double dec(double x) { return x / 2; }
	string convert(double x) {
		sprintf(str, "%.2f", x);
		return str;
	}
};
class Expo : public Scale {
public: ~Expo() {
	DTOR("~Expo ", Lecture);
}
public:
	double inc(double x) { return x * x; } // x^2.
	double dec(double x) { return sqrt(x); }
	string convert(double x) {
		sprintf(str, "%.2f", x);
		return str;
	}
};

class ForwardOp { // An interface class in the AF DP.
protected:
	char str[80];
public: virtual ~ForwardOp() {
	DTOR("~ForwardOp ", Lecture);
}
	virtual string op(double a, double b) { return "_"; }
	virtual string symbol() { return " "; }
};
class Count : public ForwardOp {
public: ~Count() {
	DTOR("~Count ", Lecture);
}
public:
	string op(double a, double b) {
		string ticks = "";
		if(a + b == 0)	return "_";
		for(int i=0; i<a; i++)
			ticks += "|";
		for(int i=0; i<b; i++)
			ticks += "|";
		return ticks;
	}
	string symbol() { return "|"; }
};
class Add : public ForwardOp {
public: ~Add() {
	DTOR("~Add ", Lecture);
}
public:
	string op(double a, double b) {
		sprintf(str, "%.0f", a+b);
		return str;
	}
	string symbol() { return "+"; }
};
class Multiply : public ForwardOp {
public: ~Multiply() {
	DTOR("~Multiply ", Lecture);
}
public:
	string op(double a, double b) {
		sprintf(str, "%.3f", a*b);
		return str;
	}
	string symbol() { return "*"; }
};
class Exponentiate : public ForwardOp {
public: ~Exponentiate() {
	DTOR("~Exponentiate ", Lecture);
}
public:
	string op(double a, double b) {
		double val = 1;
		for(int i=0; i<b; i++)
			val = a*val;
		sprintf(str, "%.3f", val);
		return str;
	}
	string symbol() { return "^"; }
};

class InverseOp { // An interface class in the AF DP.
protected:
	char str[80];
public: virtual ~InverseOp() {
	DTOR("~InverseOp ", Lecture);
}
	virtual string op(double a, double b) { return "_"; }
	virtual string symbol() { return "|"; }
};
class TakeAway : public InverseOp {
public: ~TakeAway() {
	DTOR("~TakeAway ", Lecture);
}
public:
	string op(double a, double b) {
		string ticks = "";
		if(a - b <= 0)	return "_";
		for(int i=0; i<a-b; i++)
			ticks += "|";
		return ticks;
	}
	string symbol() { return "~"; }
};
class Subtract : public InverseOp {
public: ~Subtract() {
	DTOR("~Subtract ", Lecture);
}
public:
	string op(double a, double b) {
		sprintf(str, "%.0f", a-b);
		return str;
	}
	string symbol() { return "-"; }
};
class Divide : public InverseOp {
public: ~Divide() {
	DTOR("~Divide ", Lecture);
}
public:
	string op(double a, double b) {
		sprintf(str, "%.3f", a/b);
		return str;
	}
	string symbol() { return "/"; }
};
class Root : public InverseOp {
public: ~Root() {
	DTOR("~Root ", Lecture);
}
public:
	string op(double a, double b) {
		sprintf(str, "%.3f", pow(a,1/b));
		return str;
	}
	string symbol() { return "r"; }
};

class FastHierarchy { // Abstract Factory design pattern.
public:	virtual ~FastHierarchy() {
	DTOR("~AbstractFactory\n", Lecture);
}
public:
  virtual Identity*  createIdentityElement(){return new Identity;};
  virtual Scale* 	 createScale() {   return new Scale; };
  virtual ForwardOp* createForwardOp(){return new ForwardOp;};
  virtual InverseOp* createInverseOp(){return new InverseOp;};
public:
	static FastHierarchy* createAlgebra(const string& criteria);
};
class Counting : public FastHierarchy {
public:	~Counting() {
	DTOR("  ~Counting ", Lecture);
}
public:
  Identity*  createIdentityElement(){return new Nothing; };
  Scale* 	 createScale() {		 return new Ticks; };
  ForwardOp* createForwardOp() {	 return new Count; };
  InverseOp* createInverseOp() {	 return new TakeAway;};
};
class Addition : public FastHierarchy {
public:	~Addition() {
	DTOR("  ~Addition ", Lecture);
}
public:
  Identity*  createIdentityElement(){return new Zero; };
  Scale* 	 createScale() {		 return new Successor;};
  ForwardOp* createForwardOp() {	 return new Add; };
  InverseOp* createInverseOp() {	 return new Subtract;};
};
class Multiplication : public FastHierarchy {
public:	~Multiplication() {
	DTOR("  ~Multiplication ", Lecture);
}
public:
  Identity*  createIdentityElement(){return new One; };
  Scale* 	 createScale() {		 return new Product; };
  ForwardOp* createForwardOp() {	 return new Multiply;};
  InverseOp* createInverseOp() {	 return new Divide; };
};
class Exponentiation : public FastHierarchy {
public:	~Exponentiation() {
	DTOR("  ~Exponentiation ", Lecture);
}
public:
  Identity*  createIdentityElement(){return new Range; };
  Scale* 	 createScale() {	 return new Expo; };
  ForwardOp* createForwardOp() { return new Exponentiate;};
  InverseOp* createInverseOp() { return new Root; };
};
// Seam point - add another algebra.

FastHierarchy* FastHierarchy::createAlgebra(const string& criteria) {
	if(criteria == "Counting")			return new Counting;
	if(criteria == "Addition")			return new Addition;
	if(criteria == "Multiplication")	return new Multiplication;
	if(criteria == "Exponentiation")	return new Exponentiation;
	// Seam point - add another AbstractFactory.
	return new FastHierarchy;
}

class ClientCode {
	Identity*	ident;
	Scale*		scale;
	ForwardOp*	forward;
	InverseOp*	inverse;
public:
	ClientCode(FastHierarchy* algebra)
		: ident(algebra->createIdentityElement())
		, scale(algebra->createScale())
		, forward(algebra->createForwardOp())
		, inverse(algebra->createInverseOp())
	{}
	~ClientCode() {
		DTOR("  ~ClientCode\n", Lecture);
		delete ident;
		delete scale;
		delete forward;
		delete inverse;
	}
public:
void calc() {
	cout << "  The identity element is "// Identity.
		 << ident->value() << ".\n";

	double value = atoi(&(ident->value()[1]));

	cout << "  ";						// Up scale.
	for(int i=0; i<3; i++) {
		value = scale->inc(value);
		cout << scale->convert(value) << "  ";
	}
	value = scale->inc(value);
	cout << scale->convert(value);

	cout << " <> ";						// Down scale.
	for(int i=0; i<5; i++) {
		value = scale->dec(value);
		cout << scale->convert(value) << "  ";
	}
	value = scale->dec(value);
	cout << scale->convert(value) << "\n";

										// Forward op.
	cout << "  6 " << forward->symbol() << " 2 = ";
	cout << forward->op(6, 2) << "\n";
										// Inverse op.
	cout << "  6 " << inverse->symbol() << " 2 = ";
	cout << inverse->op(6, 2) << "\n";
	cout << "  2 " << inverse->symbol() << " 6 = ";
	cout << inverse->op(2, 6) << "\n";
}
};

void demo() {
	string criteria[] = { "Counting", "Addition",
						  "Multiplication", "Exponentiation" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		FastHierarchy* factory = FastHierarchy::createAlgebra(criteria[i]);
		ClientCode* client = new ClientCode(factory);
		delete factory;
		client->calc();

		delete client;
		cout << endl << endl;
	}
}

} // solution

} // lecture

namespace homework {

/* Consider a wild life simulation with different animals and continents.
 * The major complexity is the relationships between the animals.
 * Animals are grouped according to their continent, so the simulation
 * runs on a continent by continent basis.
 * Legacy namespace has two continents, each with one prey and one predator:
 *   1) North America
 *   	1.1 Bison
 *   	1.2 Wolf
 *   2) Africa
 *   	2.1 Zebra
 *   	2.2 Lion
 * Problem namespace adds another continent:
 *   3) South America
 *   	3.1 Llama
 *   	3.2 Puma
 * Prey graze, mate, and evade; predators eat, mate and hunt.
 * With a procedural approach, scaling is quadratic.
 * The effort to add a relationship scales as the number of continents.
 * The effort to add a continent scales as the number of relationships.
 * Use the Abstract Factory design pattern to reduce coupling,
 * enhance cohesion, and avoid quadratic scaling.
 */

#include "../Problems/abstractFactory.h"

#include "../Solutions/abstractFactory.h"

} // homework

namespace skeleton {

class Type1 {
public: virtual ~Type1() {
		DTOR("~Type1\n", Lecture);
	}
public:	virtual void id()=0;
};
class Type1_Able : public Type1 {
public: ~Type1_Able() {
		DTOR("  ~Type1_Able ", Lecture);
	}
public:	void id() {
	cout << "  Type 1 Able.\n";
}
};
class Type1_Bill : public Type1 {
public: ~Type1_Bill() {
		DTOR("  ~Type1_Bill ", Lecture);
	}
public:	void id() {
	cout << "  Type 1 Bill.\n";
}
};
class Type1_Cain : public Type1 {
public: ~Type1_Cain() {
		DTOR("  ~Type1_Cain ", Lecture);
	}
public:	void id() {
	cout << "  Type 1 Cain.\n";
}
};

class Type2 {
public: virtual ~Type2() {
		DTOR("~Type2\n", Lecture);
	}
public:	virtual void id()=0;
};
class Type2_Able : public Type2 {
public: ~Type2_Able() {
		DTOR("  ~Type2_Able ", Lecture);
	}
public:	void id() {
	cout << "  Type 2 Able.\n";
}
};
class Type2_Bill : public Type2 {
public: ~Type2_Bill() {
		DTOR("  ~Type2_Bill ", Lecture);
	}
public:	void id() {
	cout << "  Type 2 Bill.\n";
}
};
class Type2_Cain : public Type2 {
public: ~Type2_Cain() {
		DTOR("  ~Type2_Cain ", Lecture);
	}
public:	void id() {
	cout << "  Type 2 Cain.\n";
}
};

class Type3 {
public: virtual ~Type3() {
		DTOR("~Type3\n", Lecture);
	}
public:	virtual void id()=0;
};
class Type3_Able : public Type3 {
public: ~Type3_Able() {
		DTOR("  ~Type3_Able ", Lecture);
	}
public:	void id() {
	cout << "  Type 3 Able.\n";
}
};
class Type3_Bill : public Type3 {
public: ~Type3_Bill() {
		DTOR("  ~Type3_Bill ", Lecture);
	}
public:	void id() {
	cout << "  Type 3 Bill.\n";
}
};
class Type3_Cain : public Type3 {
public: ~Type3_Cain() {
		DTOR("  ~Type3_Cain ", Lecture);
	}
public:	void id() {
	cout << "  Type 3 Cain.\n";
}
};

class AbstractFactory {	// If whole families are varying...
public: virtual ~AbstractFactory() {
		DTOR("~FastHierarchy\n", Lecture);
	}
public:
	virtual Type1* create_type_1()=0;
	virtual Type2* create_type_2()=0;
	virtual Type3* create_type_3()=0;
public:
	static AbstractFactory* makeObject(string& criteria);
};
class Family_Able : public AbstractFactory {
public: ~Family_Able() {
		DTOR("  ~Family_Able ", Lecture);
	}
public:
	Type1* create_type_1() { return new Type1_Able; }
	Type2* create_type_2() { return new Type2_Able; }
	Type3* create_type_3() { return new Type3_Able; }
};
class Family_Bill : public AbstractFactory {
public: ~Family_Bill() {
		DTOR("  ~Family_Bill ", Lecture);
	}
public:
	Type1* create_type_1() { return new Type1_Bill; }
	Type2* create_type_2() { return new Type2_Bill; }
	Type3* create_type_3() { return new Type3_Bill; }
};
class Family_Cain : public AbstractFactory {
public: ~Family_Cain() {
		DTOR("  ~Family_Cain ", Lecture);
	}
public:
	Type1* create_type_1() { return new Type1_Cain; }
	Type2* create_type_2() { return new Type2_Cain; }
	Type3* create_type_3() { return new Type3_Cain; }
};
// Seam point - add another abstract factory.

AbstractFactory* AbstractFactory::makeObject(string& criteria) {
	if(criteria == "Able")	return new Family_Able;
	if(criteria == "Bill")	return new Family_Bill;
	if(criteria == "Cain")	return new Family_Cain;
	// Seam point - insert another family.
	return 0;	// ABS null.
}

void demo() {	// Test variations.
	string criteria[] = { "Able", "Bill", "Cain", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		AbstractFactory* factory
			= AbstractFactory::makeObject(criteria[i]);
		if(!factory) {
			cout << "  Oops!\n\n";
			return;
		}

		Type1* type1 = factory->create_type_1();
		Type2* type2 = factory->create_type_2();
		Type3* type3 = factory->create_type_3();

		type1->id();
		type2->id();
		type3->id();

		delete type1; delete type2; delete type3;
		delete factory;
	}
	cout << endl;
}

} // skeleton

namespace recognition {

class Menu {	// If the menus are varying...
public: virtual ~Menu() {}
public:	virtual void id()=0;
};
class MenuMac : public Menu {
public:	void id() { cout << "  Menu Mac.\n"; }
};
class MenuUnix : public Menu {
public:	void id() { cout << "  Menu Unix.\n"; }
};
class MenuMicro : public Menu {
public:	void id() { cout << "  Menu Micro.\n"; }
};

class Window {	// If the windows are varying...
public: virtual ~Window() {}
public:	virtual void id()=0;
};
class WindowMac : public Window {
public:	void id() { cout << "  Window Mac.\n"; }
};
class WindowUnix : public Window {
public:	void id() { cout << "  Window Unix.\n"; }
};
class WindowMicro : public Window {
public:	void id() { cout << "  Window Micro.\n"; }
};

class Touch {	// If touch interface is varying...
public: virtual ~Touch() {}
public:	virtual void id()=0;
};
class TouchMac : public Touch {
public:	void id() { cout << "  Touch Mac.\n"; }
};
class TouchUnix : public Touch {
public:	void id() { cout << "  Touch Unix.\n"; }
};
class TouchMicro : public Touch {
public:	void id() { cout << "  Touch Micro.\n"; }
};

class OS {	// If the operating system is varying...
public: virtual ~OS() {}
public:
	virtual Menu* create_menus()=0;
	virtual Window* create_windows()=0;
	virtual Touch* create_touch()=0;
public:
	static OS* makeObject(string& criteria);
};
class Mac : public OS {
public:
	Menu* create_menus() { return new MenuMac; }
	Window* create_windows() { return new WindowMac; }
	Touch* create_touch() { return new TouchMac; }
};
class Unix : public OS {
public:
	Menu* create_menus() { return new MenuUnix; }
	Window* create_windows() { return new WindowUnix; }
	Touch* create_touch() { return new TouchUnix; }
};
class Micro : public OS {
public:
	Menu* create_menus() { return new MenuMicro; }
	Window* create_windows() { return new WindowMicro; }
	Touch* create_touch() { return new TouchMicro; }
};
// Seam point - add another operating system.

OS* OS::makeObject(string& criteria) {
	if(criteria == "Mac")	return new Mac;
	if(criteria == "Unix")	return new Unix;
	if(criteria == "Micro")	return new Micro;
	// Seam point - insert another criteria.
	return 0;	// ABC null.
}

void demo() {	// Test variations.
	string criteria[] = { "Mac", "Unix", "Micro", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		OS* factory = OS::makeObject(criteria[i]);
		if(!factory) {
			cout << "  Oops!\n\n";
			return;
		}

		Menu* menus = factory->create_menus();
		Window* windows = factory->create_windows();
		Touch* touch = factory->create_touch();

		menus->id();
		windows->id();
		touch->id();
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
	: observer::DPObserver(subject, seqNo, "abstractFactory") {}
	virtual ~Observer() { DTOR("~AbstractFactoryObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << abstract_factory::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << abstract_factory::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << abstract_factory::lecture::solution >>\n";
		lecture::solution::demo();
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << abstract_factory::homework::legacy >>\n";
		homework::legacy::demo();
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << abstract_factory::homework::problem >>\n";
		homework::problem::demo();
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << abstract_factory::homework::solution >>\n";
		homework::solution::demo();
	}
	virtual void skeleton() {
		cout << seqNo << ") << abstract_factory::skeleton >>\n";
		skeleton::demo();
	}
	virtual void recognition() {
		cout << seqNo << ") << abstract_factory::recognition >>\n";
		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << abstract_factory::refactoring >>\n";
		refactoring::bad::demo();
		refactoring::good::demo();
	}
};

} // abstract_factory

#endif /* LECTURES_ABSTRACTFACTORY_H_ */

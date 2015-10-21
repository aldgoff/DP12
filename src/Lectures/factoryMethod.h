/*
 * factoryMethod.h
 *
 *  Created on: May 23, 2015
 *      Author: aldgoff
 *
 * Varies: If the classes are varying, use the Factory Method pattern.
 *
 * Desc: Provide a method to create derived classes w/o coupling them to the client.
 *
 * Category: Creational
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Factory
 *  	http://www.dofactory.com/net/factoryMethod-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter7
 *  	http://sourcemaking.com/design_patterns/factoryMethod
 */

#ifndef LECTURES_FACTORYMETHOD_
#define LECTURES_FACTORYMETHOD_

#include "../DP.h"

namespace factory_method {	// Command line: factoryMethod [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void clientCode_Trip(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  Fly to the Bahamas.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Load car.\n";
	}
	// Seam point - add another vacation.
}
void clientCode_Plan(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  Schedule dives.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Mark up map.\n";
	}
	// Seam point - add another vacation.
}
void clientCode_Experience(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  See pretty fish.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Take pictures.\n";
	}
	// Seam point - add another vacation.
}

void demo(int seqNo) {
	cout<<seqNo<<") << factory_method::lecture::legacy::demo() >>\n";
	string criteria[] = { "Bahamas", "RoadTrip" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		clientCode_Trip(criteria[i]);
		clientCode_Plan(criteria[i]);
		clientCode_Experience(criteria[i]);
	}
	cout << endl;
}

}

namespace problem {

void clientCode_Trip(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  Fly to the Bahamas.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Load car.\n";
		}
	else if(criteria == "Disneyland") {
		cout << "  Get to Disneyland.\n";
		}
	// Seam point - add another vacation.
	else {
		cout << "  Oops!";
	}
}
void clientCode_Plan(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  Schedule dives.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Mark up map.\n";
		}
	else if(criteria == "Disneyland") {
		cout << "  Plan rides.\n";
		}
	// Seam point - add another vacation.
	else {
		cout << "  Oops!";
	}
}
void clientCode_Experience(const string& criteria) {
	if(criteria == "Bahamas") {
		cout << "  See pretty fish.\n";
		}
	else if(criteria == "RoadTrip") {
		cout << "  Take pictures.\n";
		}
	else if(criteria == "Disneyland") {
		cout << "  Wait in line.\n";
		}
	// Seam point - add another vacation.
	else {
		cout << "  Oops!\n";
	}
}

void demo(int seqNo) {
	cout<<seqNo<<") << factory_method::lecture::problem::demo() >>\n";
	string criteria[] = {"Bahamas","RoadTrip","Disneyland","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		clientCode_Trip(criteria[i]);
		clientCode_Plan(criteria[i]);
		clientCode_Experience(criteria[i]);
	}
	cout << endl;
}

}

namespace solution {

class Vacation { // Factory Method design pattern.
public:	virtual ~Vacation() {
		DTOR(" ~Vacation\n", Lecture);
	}
public:
	virtual void getThere() { cout << "  Oops!"; }
	virtual void schedule() { cout << " Oops!"; }
	virtual void haveFun() { cout << " Oops!"; }
public:
	static Vacation* makeObject(const string& criteria);
};
class Bahamas : public Vacation {
public:	virtual ~Bahamas() {
		DTOR("    ~Bahamas", Lecture);
	}
public:
	void getThere() { cout << "  Fly to the Bahamas.\n";}
	void schedule() { cout << "  Schedule dives.\n";}
	void haveFun() { cout << "  See pretty fish.\n";}
};
class RoadTrip : public Vacation {
public:	virtual ~RoadTrip() {
		DTOR("    ~RoadTrip", Lecture);
	}
public:
	void getThere() { cout << "  Load car.\n";}
	void schedule() { cout << "  Mark up map.\n";}
	void haveFun() { cout << "  Take pictures.\n";}
};
class Disneyland : public Vacation {
public:	virtual ~Disneyland() {
		DTOR("    ~Disneyland", Lecture);
	}
public:
	void getThere() { cout << "  Get to Disneyland.\n";}
	void schedule() { cout << "  Plan rides.\n";}
	void haveFun() { cout << "  Wait in line.\n";}
};
// Seam point - add another vacation.

Vacation* Vacation::makeObject(const string& criteria) {
	if(criteria == "Bahamas")	return new Bahamas;
	if(criteria == "RoadTrip")	return new RoadTrip;
	if(criteria == "Disneyland")	return new Disneyland;
	// Seam point - insert another vacation.
	return new Vacation; // Base, default, null, exception.
}

void clientCode_Trip(Vacation* vacation) {
	vacation->getThere();
}
void clientCode_Plan(Vacation* vacation) {
	vacation->schedule();
}
void clientCode_Experience(Vacation* vacation) {
	vacation->haveFun();
}

void demo(int seqNo) {
	cout<<seqNo<<") << factory_method::lecture::solution::demo() >>\n";
	string criteria[] = {"Bahamas","RoadTrip","Disneyland","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Vacation* vacation = Vacation::makeObject(criteria[i]);
		clientCode_Trip(vacation);
		clientCode_Plan(vacation);
		clientCode_Experience(vacation);
		delete vacation;
	}
	cout << endl;
}

}

} // lecture

namespace homework {

#include "../Problems/factoryMethod.h"

#include "../Solutions/factoryMethod.h"

} // homework

namespace skeleton {

class FactoryMethod { // If the classes are varying...
public: virtual ~FactoryMethod() {
		DTOR("~FactoryMethod\n", Lecture);
	}
public:
	virtual void method() { cout << "  Oops!  "; }
public:
	static FactoryMethod* makeObject(string& criteria);
};
class Type1 : public FactoryMethod {
public: ~Type1() {
		DTOR("  ~Type1 ", Lecture);
	}
public:
	void method() { cout << "  Type1."; }
};
class Type2 : public FactoryMethod {
public: ~Type2() {
		DTOR("  ~Type2 ", Lecture);
	}
public:
	void method() { cout << "  Type2."; }
};
class Type3 : public FactoryMethod {
public: ~Type3() {
		DTOR("  ~Type3 ", Lecture);
	}
public:
	void method() { cout << "  Type3."; }
};
// Seam point - add another class.

FactoryMethod* FactoryMethod::makeObject(string& criteria) {
	if(criteria == "Type1")	return new Type1;
	if(criteria == "Type2")	return new Type2;
	if(criteria == "Type3")	return new Type3;
	// Seam point - insert another class.
	return new FactoryMethod; // Base, default, null, exception.
}

void demo(int seqNo) {	// Test variations.
	string criteria[] = { "Type1", "Type2", "Type3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		FactoryMethod* type = FactoryMethod::makeObject(criteria[i]);
		type->method();
		delete type;
	}
	cout << endl;
}

} // skeleton

namespace evolution_of_concerns { // Todo: move to acceleration.h.

class ClientHierarchy { // Some design pattern.		// Abstract doing.
public: virtual ~ClientHierarchy() {
	DTOR("~ClientHierarchy\n", Lecture);
}
public:
	virtual string process() { return "Oops!"; }
public:
	static ClientHierarchy* makeObject(string& criteria);
};
class Type1 : public ClientHierarchy {
public: ~Type1() {
		DTOR("  ~Type1 ", Lecture);
	}
public:
	string process() { return "Type1"; }
};
class Type2 : public ClientHierarchy {
public: ~Type2() {
		DTOR("  ~Type2 ", Lecture);
	}
public:
	string process() { return "Type2"; }
};
class Type3 : public ClientHierarchy {
public: ~Type3() {
		DTOR("  ~Type3 ", Lecture);
	}
public:
	string process() { return "Type3"; }
};
// Seam point - add another Type.

class FactoryMethod {								// Abstract creation.
public: virtual ~FactoryMethod() {
	DTOR("~FactoryMethod\n", Lecture);
}
public:
	virtual ClientHierarchy* make() { return new ClientHierarchy; }
};
class Factory1 : public FactoryMethod {
public: ~Factory1() {
		DTOR("  ~Factory1 ", Lecture);
	}
public:
	ClientHierarchy* make() { return new Type1; }
};
class Factory2 : public FactoryMethod {
public: ~Factory2() {
		DTOR("  ~Factory2 ", Lecture);
	}
public:
	ClientHierarchy* make() { return new Type2; }
};
class Factory3 : public FactoryMethod {
public: ~Factory3() {
		DTOR("  ~Factory3 ", Lecture);
	}
public:
	ClientHierarchy* make() { return new Type3; }
};
// Seam point - add another Factory.

namespace idiomatic {

FactoryMethod* makeFactory(const string& criteria) { // Abstract deciding.
	if(criteria == "Factory1")	return new Factory1;
	if(criteria == "Factory2")	return new Factory2;
	if(criteria == "Factory3")	return new Factory3;
	// Seam point - insert another Factory.
	return new FactoryMethod;
}

ClientHierarchy* makeObject(string& criteria) {
	FactoryMethod* factory = 0;

	// Factory implementation:
	if(		criteria == "Type1")	factory = makeFactory("Factory1");
	else if(criteria == "Type2")	factory = makeFactory("Factory2");
	else if(criteria == "Type3")	factory = makeFactory("Factory3");
	// Seam point - insert another Factory.

	else							factory = new FactoryMethod;

	ClientHierarchy* type = factory->make();
	delete factory;

	return type;
}

} // idiomatic

namespace realistic {

FactoryMethod* makeFactory(const string& criteria) { // Abstract deciding.
	if(criteria == "Factory1") {
		//...
		//...
		return new Factory1;
	}
	if(criteria == "Factory2") {
		//...
		//...
		return new Factory2;
	}
	if(criteria == "Factory3") {
		//...
		//...
		return new Factory3;
	}
	// Seam point - insert another Factory.

	//...
	//...
	return new FactoryMethod;
}

namespace original {

ClientHierarchy* makeObject(string& criteria) {
	if(criteria == "Type1")	return new Type1;
	if(criteria == "Type2")	return new Type2;
	if(criteria == "Type3")	return new Type3;
	// Seam point - insert another class.
	return new ClientHierarchy; // Base, default, null, exception.
}

} // original

namespace leaky { // Memory leak: factories never destroyed.

ClientHierarchy* makeObject(string& criteria) {
	if(criteria == "Type1")	return makeFactory("Factory1")->make();
	if(criteria == "Type2")	return makeFactory("Factory2")->make();
	if(criteria == "Type3")	return makeFactory("Factory3")->make();
	// Seam point - insert another Type.
	else 					return new ClientHierarchy;
}

} // leaky

namespace object {

ClientHierarchy* makeObject(string& criteria) {
	if(criteria == "Type1")	return Factory1().make();
	if(criteria == "Type2")	return Factory2().make();
	if(criteria == "Type3")	return Factory3().make();
	// Seam point - insert another Type.
	else 					return new ClientHierarchy;
}

} // leaky

namespace final {

ClientHierarchy* makeObject(string& criteria) {
	FactoryMethod* factory = 0;

	// Factory implementation:
	if(		criteria == "Type1"
			//...
			//...
		)
	{
		factory = makeFactory("Factory1");
	}
	else if(criteria == "Type2"
			//...
			//...
		)
	{
		factory = makeFactory("Factory2");
	}
	else if(criteria == "Type3"
			//...
			//...
			)
	{
		factory = makeFactory("Factory3");
	}
	// Seam point - insert another Factory.

	else {
		factory = new FactoryMethod;
	}

	ClientHierarchy* type = factory->make();
	delete factory;

	return type;
}

} // final

} // realistic

ClientHierarchy* ClientHierarchy::makeObject(string& criteria) {
	return realistic::object::makeObject(criteria);
}

void demo(int seqNo) {	// Test variations.
	string criteria[] = { "Type1", "Type2", "Type3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		ClientHierarchy* type = ClientHierarchy::makeObject(criteria[i]);
		cout << "  " << type->process() << ".\n";
		delete type;
	}
	cout << endl;
}

}

namespace recognition {

class Car {
public: virtual ~Car() {}
public:
	virtual void drive() { cout << "  Oops!\n"; }
public:
	static Car* makeObject(const string& criteria);
};
class Mercedes : public Car {
public:
	void drive() { cout << "  Mercedes.\n"; }
};
class BMW : public Car {
public:
	void drive() { cout << "  BMW.\n"; }
};
class Tesla : public Car {
public:
	void drive() { cout << "  Tesla.\n"; }
};
// Seam point - add another car.

Car* Car::makeObject(const string& criteria) {
	if(criteria == "Safety")	return new Mercedes;
	if(criteria == "Status")	return new BMW;
	if(criteria == "Electric")	return new Tesla;
	// Seam point - insert another criteria.
	return 0;	// Null.
}

void demo() {	// Test variations.
	string criteria[] = { "Safety", "Status", "Electric" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Car* type = Car::makeObject(criteria[i]);
		type->drive();
	}
	cout << endl;
}

} // recognition

namespace refactoring {

namespace bad { // Do two things...

struct Trailer {
	void roll() { cout << "  Trailer rolling.\n"; }
	void park() { cout << "  Trailer parking.\n"; }
};
struct CTainer {
	void ship() { cout << "  CTainer shipping.\n"; }
	void store() { cout << "  CTainer storing.\n"; }
};
struct Warehouse {
	void stock() { cout << "  Warehouse stocked.\n"; }
	void lock() { cout << "  Warehouse locked.\n"; }
};

void clientCode(int criteria) {
	switch(criteria) {
	case 0:	{ Trailer trailer;
		trailer.roll();
		trailer.park();
		} break;
	case 1:	{ CTainer ctainer;
		ctainer.ship();
		ctainer.store();
		} break;
	case 2:	{ Warehouse warehouse;
		warehouse.stock();
		warehouse.lock();
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

namespace good { // Do two things...

class FactoryMethod {
public: virtual ~FactoryMethod() {}
public:
	virtual void doTwoThings() { cout << "  Oops!\n"; }
public:
	static FactoryMethod* makeObject(string& criteria);
};
class Trailer_FM : public FactoryMethod {
public:
	void doTwoThings() {
		bad::Trailer trailer;
		trailer.roll();
		trailer.park();
	}
};
class CTainer_FM : public FactoryMethod {
public:
	void doTwoThings() {
		bad::CTainer ctainer;
		ctainer.ship();
		ctainer.store();
	}
};
class Warehouse_FM : public FactoryMethod {
public:
	void doTwoThings() {
		bad::Warehouse warehouse;
		warehouse.stock();
		warehouse.lock();
	}
};
// Seam point - add another class.

FactoryMethod* FactoryMethod::makeObject(string& criteria) {
	if(criteria == "Trailer")	return new Trailer_FM;
	if(criteria == "CTainer")	return new CTainer_FM;
	if(criteria == "Warehouse")	return new Warehouse_FM;
	// Seam point - insert another criteria.
	else { return new FactoryMethod; }
}

void clientCode(FactoryMethod* type) {
	type->doTwoThings();
}

void demo() {
	cout << "  refactoring::good::demo().\n";
	string criteria[] = {"Trailer","CTainer","Warehouse","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		FactoryMethod* type = FactoryMethod::makeObject(criteria[i]);
		clientCode(type);
		delete type;
	}
	cout << endl;
}

} // good

namespace gof_factory {

class Seuss {
public: virtual ~Seuss() {}
public:
	virtual void id() { cout << "  Seuss.\n"; }
};
class SamIAm : public Seuss {
public: void id() { cout << "  SamIAm.\n"; }
};
class GreenEggsAndHam : public Seuss {
public: void id() { cout << "  GreenEggsAndHam.\n"; }
};
class DoNotLikeThem : public Seuss {
public: void id() { cout << "  DoNotLikeThem.\n"; }
};

class Factory {
public: virtual ~Factory() {}
public:
	virtual Seuss* make()=0;
};
class RedFish : public Factory {
public:	Seuss* make() { return new SamIAm; }
};
class BlueFish : public Factory {
public:	Seuss* make() { return new GreenEggsAndHam; }
};
class OneFish : public Factory {
public:	Seuss* make() { return new DoNotLikeThem; }
};

Seuss* makeObject(const string& criteria) {
	if(criteria == "SamIAm")			return new SamIAm;
	if(criteria == "GreenEggsAndHam")	return new GreenEggsAndHam;
	if(criteria == "DoNotLikeThem")		return new DoNotLikeThem;
	throw "Oops!";
}

void demo() {	// Test variations.
	Factory* fms[] = { new RedFish, new BlueFish, new OneFish };
	for(size_t i=0; i<COUNT(fms); i++) {
		Seuss* reader = fms[i]->make();
		reader->id();
	}
	cout << endl;
	string criteria[] = { "SamIAm", "GreenEggsAndHam", "DoNotLikeThem" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Seuss* reader = makeObject(criteria[i]);
		reader->id();
	}
	cout << endl;
}

} // gof_factory

namespace wiki_factory {

/* URLs
 * https://en.wikipedia.org/wiki/Factory_method_pattern
 * https://en.wikipedia.org/wiki/Factory_(object-oriented_programming)
 */

class Quantum {};

Quantum Factory_Obj() { return Quantum(); }
Quantum* Factory_Ptr() { return new Quantum(); }
Quantum& Factory_Singleton() {
	static Quantum quantum;
	return quantum;
}
int Factory_Int() { return int(); }

class Lepton {
public: virtual ~Lepton() {}
	virtual string id() { return "Lepton"; }
};
class Proton : public Lepton {
	string id() { return "Proton"; }
};
class Neutron : public Lepton {
	string id() { return "Neutron"; }
};

class LeptonFactory { // Factory Method.
public: virtual ~LeptonFactory() {}
virtual Lepton* make() { return new Lepton; }
};
class ProtonFactory : public LeptonFactory {
public: Lepton* make() { return new Proton; }
};
class NeutronFactory : public LeptonFactory {
public: Lepton* make() { return new Neutron; }
};

class Boson {
public: virtual ~Boson() {}
	virtual string id() { return "Boson"; }
};
class Photon : public Boson {
	string id() { return "Photon"; }
};
class Graviton : public Boson {
	string id() { return "Graviton"; }
};

class BosonFactory { // Factory Method.
public: virtual ~BosonFactory() {}
virtual Boson* make() { return new Boson; }
};
class PhotonFactory : public BosonFactory {
public: Boson* make() { return new Photon; }
};
class GravitonFactory : public BosonFactory {
public: Boson* make() { return new Graviton; }
};

Boson* bosonFactory(string criteria) {
	if(criteria == "Photon")	return new Photon;
	if(criteria == "Graviton")	return new Graviton;
	return new Boson;
}

//class BuildUniverse { // Orthogonal.
//public:
//	Lepton* makeLepton(LeptonFactory* factory) { return factory->make(); }
//	Boson* makeBoson(BosonFactory* factory) { return factory->make(); }
//};

class UniverseFactory { // Abstract Factory.
public: virtual ~UniverseFactory() {}
public:
	virtual Lepton* makeLepton() { return new Lepton; }
	virtual Boson* makeBoson() { return new Boson; }
};
class PlasmaFactory : public UniverseFactory {
public:
	Lepton* makeLepton() { return new Proton; }
	Boson* makeBoson() { return new Photon; }
};
class DarkMatterFactory : public UniverseFactory {
public:
	Lepton* makeLepton() { return new Neutron; }
	Boson* makeBoson() { return new Graviton; }
};

void demo() {
	LeptonFactory* leptons[] = { new ProtonFactory, new NeutronFactory };
//	for(size_t i=0; i<COUNT(leptons); i++) {
//		Lepton* particle = leptons[i]->make();
//		cout << "  " << particle->id() << "\n";
//	}
//	cout << endl;
//
	BosonFactory* bosons[] = { new PhotonFactory, new GravitonFactory };
//	for(size_t i=0; i<COUNT(bosons); i++) {
//		Boson* particle = bosons[i]->make();
//		cout << "  " << particle->id() << "\n";
//	}
//	cout << endl;
//
//	BuildUniverse universe;
	for(size_t i=0; i<COUNT(leptons); i++) {
		for(size_t j=0; j<COUNT(bosons); j++) {
			Lepton* lepton = leptons[i]->make();
			Boson* boson = bosons[j]->make();
//			Lepton* lepton = universe.makeLepton(leptons[i]);
//			Boson* boson = universe.makeBoson(bosons[j]);
			cout << "  " << lepton->id() << " - " << boson->id() << "\n";
		}
	}
	cout << endl;

	UniverseFactory* universes[] = { new PlasmaFactory, new DarkMatterFactory };
	for(size_t i=0; i<COUNT(universes); i++) {
		Lepton* lepton = universes[i]->makeLepton();
		Boson* boson = universes[i]->makeBoson();
		cout << "  " << lepton->id() << " - " << boson->id() << "\n";
	}
	cout << endl;
}

} // wiki_factory

namespace singletons {

static int num = 0;

class InputDevice {
protected:
	const int n;
	char str[9];
public:
	InputDevice() : n(++num) {}
	virtual ~InputDevice() {}
public:
	virtual string id() { return "InputDevice"; }
public:
	static InputDevice* makeObject(const string& criteria);
};
class Keyboard : public InputDevice {
	string id() {
		sprintf(str, "%d", n);
		return (string("Keyboard - ") = str);
	}
};
class Mic : public InputDevice {
	string id() {
		sprintf(str, "%d", n);
		return (string("Mic - ") = str);
	}
};
class TouchScreen : public InputDevice {
	string id() {
		sprintf(str, "%d", n);
		return (string("TouchScreen - ") = str);
	}
};

InputDevice* InputDevice::makeObject(const string& criteria) {
	if(criteria == "Keyboard") {
		static Keyboard device;
		return (InputDevice*)&device;
	}
	if(criteria == "Mic") {
		static Mic device;
		return (InputDevice*)&device;
	}
	if(criteria == "TouchScreen") {
		static TouchScreen device;
		return (InputDevice*)&device;
	}
	return new InputDevice;
}

class SingletonDevice { // Factory Method
public: virtual ~SingletonDevice() {}
virtual InputDevice* make() { return new InputDevice; }
};
class KeyboardFactory : public SingletonDevice {
	InputDevice* make() {
		static Keyboard device;
		return (InputDevice*)&device;
	}
};
class MicFactory : public SingletonDevice {
	InputDevice* make() {
		static Mic device;
		return (InputDevice*)&device;
	}
};
class TouchScreenFactory : public SingletonDevice {
	InputDevice* make() {
		static TouchScreen device;
		return (InputDevice*)&device;
	}
};

SingletonDevice* makeFactory(const string& criteria) {
	if(criteria == "Keyboard")		return new KeyboardFactory;
	if(criteria == "Mic")			return new MicFactory;
	if(criteria == "TouchScreen")	return new TouchScreenFactory;
	return new SingletonDevice;
}

void demo() {
	string criteria[] = { "Keyboard", "Mic", "TouchScreen" };
	InputDevice* device = 0;

	for(size_t i=0; i<COUNT(criteria); i++) {
		SingletonDevice* factory = makeFactory(criteria[i]);
		device = factory->make();
		cout << "  " << device->id() << ".\n";
		device = factory->make();
		cout << "  " << device->id() << ".\n";
		cout << endl;
	}

	for(size_t i=0; i<COUNT(criteria); i++) {
		device = InputDevice::makeObject(criteria[i]);
		cout << "  " << device->id() << ".\n";
		device = InputDevice::makeObject(criteria[i]);
		cout << "  " << device->id() << ".\n";
		cout << endl;
	}
}

} // singletons

namespace separation_of_concerns { // Todo: move to acceleration.h.

class ClientHierarchy { // Some design pattern.		// Abstract doing.
public: virtual ~ClientHierarchy() {}
public:
	virtual string process() { return "ClientHierarchy"; }
public:
	static ClientHierarchy* makeObject(const string& criteria);
};
class Concrete1 : public ClientHierarchy {
public:
	string process() { return "Concrete1"; }
};
class Concrete2 : public ClientHierarchy {
public:
	string process() { return "Concrete2"; }
};
class Concrete3 : public ClientHierarchy {
public:
	string process() { return "Concrete3"; }
};
// Seam point - add another Concrete.

class FactoryMethod {								// Abstract creation.
public: virtual ~FactoryMethod() {}
public:
	virtual ClientHierarchy* make() { return new ClientHierarchy; }
	void log() {
		cout << "  Deciding...\n";
		cout << "  Creating...\n";
		cout << "  Doing ";
	}
};
class Factory1 : public FactoryMethod {
public:
	ClientHierarchy* make() { log(); return new Concrete1; }
};
class Factory2 : public FactoryMethod {
public:
	ClientHierarchy* make() { log(); return new Concrete2; }
};
class Factory3 : public FactoryMethod {
public:
	ClientHierarchy* make() { log(); return new Concrete3; }
};
// Seam point - add another Factory.

FactoryMethod* makeFactory(const string& criteria) { // Abstract deciding.
	if(criteria == "Factory1")	return new Factory1;
	if(criteria == "Factory2")	return new Factory2;
	if(criteria == "Factory3")	return new Factory3;
	// Seam point - insert another criteria.
	return new FactoryMethod;
}

ClientHierarchy* ClientHierarchy::makeObject(const string& criteria) { // Evolution.
	// Succinct implementation:
	if(criteria == "Concrete1")	return Factory1().make();
	if(criteria == "Concrete2")	return Factory2().make();
	if(criteria == "Concrete3")	return Factory3().make();
	// Seam point - insert another Concrete.
	else 						return new ClientHierarchy;

	// Concise but leaky implementation:
	if(criteria == "Concrete1")	return makeFactory("Factory1")->make();
	if(criteria == "Concrete2")	return makeFactory("Factory2")->make();
	if(criteria == "Concrete3")	return makeFactory("Factory3")->make();
	// Seam point - insert another Concrete.
	else 						return new ClientHierarchy;

	// Factory implementation:
	FactoryMethod* factory = 0;
	if(		criteria == "Concrete1")	factory = makeFactory("Factory1");
	else if(criteria == "Concrete2")	factory = makeFactory("Factory2");
	else if(criteria == "Concrete3")	factory = makeFactory("Factory3");
	// Seam point - insert another Factory.
	else 								factory = new FactoryMethod;
	return factory->make();

	// Obsolete implementation:
	if(criteria == "Concrete1")	return new Concrete1;
	if(criteria == "Concrete2")	return new Concrete2;
	if(criteria == "Concrete3")	return new Concrete3;
	// Seam point - insert another Concrete.
	return new ClientHierarchy;
}

void clientCode(const string& criteria) {
	FactoryMethod* factory = makeFactory(criteria);	// Complicated deciding;
	ClientHierarchy* concrete = factory->make();	// Complicated creation;
	cout << concrete->process() << ".\n";			// Complicated doing;
}

void demo() { // Test variation.
	string criteria[] = { "Factory1", "Factory2", "Factory3" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		clientCode(criteria[i]);
		cout << endl;
	}
}

} // separation_of_concerns

} // refactoring

namespace obsolete {

// This is really the Bridge pattern.
void clientCode(int fr, int* res, string display, string crypto) {
	if(display == "DisplayPort") {
		cout << "  Display " << "DisplayPort()";
		cout << " at " << fr << " frames/sec";
		cout << " with [" << res[0] << ", " << res[1] << "]";
		if(crypto == "PVP") {
			cout << " via " << "PVP()" << ".\n";
			}
		else if(crypto == "ID1") {
			cout << " via " << "ID1()" << ".\n";
			}
		else if(crypto == "RSA") {
			cout << " via " << "RSA()" << ".\n";
			}
		// Seam point - insert another crypto protocol.
		else {
			cout << " via " << "crypto->protocol()" << ".\n";
		}
		}
	else if(display == "HDMI") {
		cout << "  Display " << "HDMI()";
		cout << " at " << fr << " frames/sec";
		cout << " with [" << res[0] << ", " << res[1] << "]";
		if(crypto == "PVP") {
			cout << " via " << "PVP()" << ".\n";
			}
		else if(crypto == "ID1") {
			cout << " via " << "ID1()" << ".\n";
			}
		else if(crypto == "RSA") {
			cout << " via " << "RSA()" << ".\n";
			}
		// Seam point - insert another crypto protocol.
		else {
			cout << " via " << "crypto protocol()" << ".\n";
		}
		}
	else if(display == "MIPI") {
		cout << "  Display " << "MIPI()";
		cout << " at " << fr << " frames/sec";
		cout << " with [" << res[0] << ", " << res[1] << "]";
		if(crypto == "PVP") {
			cout << " via " << "PVP()" << ".\n";
			}
		else if(crypto == "ID1") {
			cout << " via " << "ID1()" << ".\n";
			}
		else if(crypto == "RSA") {
			cout << " via " << "RSA()" << ".\n";
			}
		// Seam point - insert another crypto protocol.
		else {
			cout << " via " << "crypto protocol()" << ".\n";
		}
		}
	else if(display == "Widi") {
		cout << "  Display " << "Widi()";
		cout << " at " << fr << " frames/sec";
		cout << " with [" << res[0] << ", " << res[1] << "]";
		if(crypto == "PVP") {
			cout << " via " << "PVP()" << ".\n";
			}
		else if(crypto == "ID1") {
			cout << " via " << "ID1()" << ".\n";
			}
		else if(crypto == "RSA") {
			cout << " via " << "RSA()" << ".\n";
			}
		// Seam point - insert another crypto protocol.
		else {
			cout << " via " << "crypto protocol()" << ".\n";
		}
		}
	// Seam point - insert another display format.
	else {
		cout << "  Display " << "display format()";
		cout << " at " << fr << " frames/sec";
		cout << " with [" << res[0] << ", " << res[1] << "]";
		if(crypto == "PVP") {
			cout << " via " << "PVP()" << ".\n";
			}
		else if(crypto == "ID1") {
			cout << " via " << "ID1()" << ".\n";
			}
		else if(crypto == "RSA") {
			cout << " via " << "RSA()" << ".\n";
			}
		// Seam point - insert another crypto protocol.
		else {
			cout << " via " << "crypto protocol()" << ".\n";
		}
	}
}

} // obsolete

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "factoryMethod") {}
	virtual ~Observer() { DTOR("~FactoryMethodObserver ", Architecture); }
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
		cout << seqNo << ") << factory_method::skeleton >>\n";
		skeleton::demo(seqNo);
		evolution_of_concerns::demo(seqNo);

	}
	virtual void recognition() {
		cout << seqNo << ") << factory_method::recognition >>\n";
		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << factory_method::refactoring >>\n";
		refactoring::bad::demo();
		refactoring::good::demo();
		refactoring::gof_factory::demo();
		refactoring::wiki_factory::demo();
		refactoring::singletons::demo();
		refactoring::separation_of_concerns::demo();
	}
};

} // factory_method

#endif /* LECTURES_FACTORYMETHOD_ */

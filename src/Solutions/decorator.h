/*
 * decorator.h
 *
 *  Created on: Sep 24, 2015
 *      Author: aldgoff
 *
 * Varies: If the options are varying, use the Decorator pattern.
 *
 * Desc: Dynamically attach additional responsibilities to an object.
 *
 * Category: Structural
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Decorator
 *  	http://www.dofactory.com/net/decorator-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter17
 *  	http://sourcemaking.com/design_patterns/decorator
 */

#ifndef SOLUTIONS_DECORATOR_H_
#define SOLUTIONS_DECORATOR_H_

/* Consider options for a car.
 * One might subclass a base class, Car, for each set of options, but
 * then the options code gets duplicated leading to scaling problems,
 * and the options are statically bound.
 * For the 4 classes and 9 options implied by the following OOP code,
 * rewrite with the Decorator pattern to
 *   1. eliminate the combinatorial explosion
 *   2. reduce duplication
 *   3. improve code locality
 *   4. allow for selection of options at runtime
 *   5. Extra points if you don't have to change the common::Car class.
 * Pay particular attention to the problem of memory leaks with this pattern.
 */

#include "../Problems/decoratorCommon.h"

namespace solution {

namespace common {

class Car {
protected:
	string str;
public:
	Car(string str="Undefined car") : str(str) {}
	virtual ~Car() {
		DTOR("~Car\n", Homework);
	}
public:
	virtual string getDesc() { return str; }
	virtual double getCost()=0;
};

} // common

using namespace common;

class BaseModel : public Car {
public:
	BaseModel(string name="missing") : Car(name+" with 4 wheels") {}
	~BaseModel() {
		DTOR("  ~BaseModel ", Homework);
	}
public:
	string getDesc() {
		return string(str);
	}
	double getCost() { return 12000.00; }
};

class OptionsDecorator : public Car {
// This is the surprise, options are not cars,
// inheriting from Car violates the "is-a" principle,
// but it's the key to dynamically attaching additional options.
protected:
	Car* build;
public:
	OptionsDecorator(Car* build, string str="OD") : Car(str), build(build) {}
	~OptionsDecorator() { build->~Car();
		DTOR("    ~OptionsDecorator ", Homework);
	}
public:
	string getDesc() { return build->getDesc() + ", " + str; }
public:
	static Car* makeObject(Car* decoratr, string& crit);
};
class TwoDoors : public OptionsDecorator {
public:
	TwoDoors(Car* build) : OptionsDecorator(build, "2 doors") {}
	~TwoDoors() {
		DTOR("  ~TwoDoors", Homework);
	}
public:
	double getCost() { return build->getCost() + 2000.00; }
};
class FourDoors : public OptionsDecorator {
public:
	FourDoors(Car* build) : OptionsDecorator(build, "4 doors") {}
	~FourDoors() {
		DTOR("  ~FourDoors", Homework);
	}
public:
	double getCost() { return build->getCost() + 4000.00; }
};
class AC : public OptionsDecorator {
public:
	AC(Car* build) : OptionsDecorator(build, "AC") {}
	~AC() {
		DTOR("  ~AC", Homework);
	}
public:
	double getCost() { return build->getCost() + 1500.00; }
};
class PremiumSoundSystem : public OptionsDecorator {
public:
	PremiumSoundSystem(Car* build) : OptionsDecorator(build, "premium sound system") {}
	~PremiumSoundSystem() {
		DTOR("  ~PremiumSoundSystem", Homework);
	}
public:
	double getCost() { return build->getCost() + 1000.00; }
};
class Navigation : public OptionsDecorator {
public:
	Navigation(Car* build) : OptionsDecorator(build, "navigation") {}
	~Navigation() {
		DTOR("  ~Navigation", Homework);
	}
public:
	double getCost() { return build->getCost() + 2000.00; }
};
class ManualTransmission : public OptionsDecorator {
public:
	ManualTransmission(Car* build) : OptionsDecorator(build, "manual transmission") {}
	~ManualTransmission() {
		DTOR("  ~ManualTransmission", Homework);
	}
public:
	double getCost() { return build->getCost() + 2500.00; }
};
class AutomaticTransmission : public OptionsDecorator {
public:
	AutomaticTransmission(Car* build) : OptionsDecorator(build, "automatic transmission") {}
	~AutomaticTransmission() {
		DTOR("  ~AutomaticTransmission", Homework);
	}
public:
	double getCost() { return build->getCost() + 3000.00; }
};
class V8 : public OptionsDecorator {
public:
	V8(Car* build) : OptionsDecorator(build, "V8") {}
	~V8() {
		DTOR("  ~V8", Homework);
	}
public:
	double getCost() { return build->getCost() + 6000.00; }
};
class SuperCharger : public OptionsDecorator {
public:
	SuperCharger(Car* build) : OptionsDecorator(build, "super-charger") {}
	~SuperCharger() {
		DTOR("  ~SuperCharger", Homework);
	}
public:
	double getCost() { return build->getCost() + 3000.00; }
};
// Seam point - add another option.

Car* OptionsDecorator::makeObject(Car* dec, string& crit) {
	if(crit == "TwoDoors")				return new TwoDoors(dec);
	if(crit == "FourDoors")				return new FourDoors(dec);
	if(crit == "AC")					return new AC(dec);
	if(crit == "PremiumSoundSystem")	return new PremiumSoundSystem(dec);
	if(crit == "Navigation")			return new Navigation(dec);
	if(crit == "ManualTransmission")	return new ManualTransmission(dec);
	if(crit == "AutomaticTransmission")	return new AutomaticTransmission(dec);
	if(crit == "V8")	 				return new V8(dec);
	if(crit == "SuperCharger")			return new SuperCharger(dec);
	// Seam point - add another criteria.
	return dec;
}

void demo(int seqNo) {
	Car* mine = new BaseModel("RunAbout");
	mine = new TwoDoors(mine);

	Car* yours = new BaseModel("SUV");
	yours = new FourDoors(yours);
	yours = new AC(yours);
	yours = new AutomaticTransmission(yours);

	Car* hers = new BaseModel("Status");
	hers = new FourDoors(hers);
	hers = new AC(hers);
	hers = new PremiumSoundSystem(hers);
	hers = new Navigation(hers);
	hers = new AutomaticTransmission(hers);

	Car* boss = new BaseModel("Performance"); // Use Factory Method.
	string criteria[] = { "TwoDoors", "AC", "PremiumSoundSystem",
		"Navigation", "ManualTransmission", "V8", "SuperCharger"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		boss = OptionsDecorator::makeObject(boss, criteria[i]);
	}

	Car* cars[] = { mine, yours, hers, boss };

	for(size_t i=0; i<COUNT(cars); i++) {
		cout << "  " << cars[i]->getDesc();
		cout << " costs $" << cars[i]->getCost() << ".\n";
	}
	cout << endl;

	for(size_t i=0; i<COUNT(cars); i++)
		delete cars[i];
	cout << endl;
}

} // solution

#endif /* SOLUTIONS_DECORATOR_H_ */

/*
 * templateMethod.h
 *
 *  Created on: Sep 14, 2015
 *      Author: aldgoff
 *
 * Varies: If the steps are varying, use the Template Method pattern.
 *
 * Desc: Define the skeleton of an algorithm.
 *
 * Category: Behavioral
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#TemplateMethod
 *  	http://www.dofactory.com/net/templateMethod-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter19
 *  	http://sourcemaking.com/design_patterns/templateMethod
 */

#ifndef SOLUTIONS_TEMPLATEMETHOD_H_
#define SOLUTIONS_TEMPLATEMETHOD_H_

#include "../macros.h"

/* Consider a 6 step protective coating process as implied by
 * the common and legacy namespaces below.
 * Optimization is the only difference between the processes,
 * fast() and economic().
 * Consider new specs (ala new_specs & problem namespaces below):
 * 	 2 more processes with different optimizations, critical() & npc().
 *   For both of these the cleanup step must be better and different.
 * Refactor using the Template Method design pattern.
 */

namespace solution {

class Protective {	// Template Method design pattern.
	bool morning() { return true; }
public: virtual ~Protective() {
	DTOR("~Coating\n", Lecture);
}
public:
	void coating() {	// A 6 step process.
		setup();		// Most steps the same.
		schedule();
		if(morning())
			highHeat();
		else
			lowHeat();
		optimize();		// Must differ.
		cleanup();		// May differ.
		putaway();
		cout << endl;
	}
private:
	void setup()    { cout << "  setup\n"; }	// Same steps.
	void schedule() { cout << "  schedule\n"; }
	void highHeat() { cout << "  highHeat\n"; }
	void lowHeat()  { cout << "  lowHeat\n"; }
protected:
	virtual void optimize()=0;	// Subclasses must supply.
	virtual void cleanup() { cout << "  usual cleanup\n"; }	// Default behavior.
private:
	void putaway() { cout << "  putaway\n"; }	// Same steps.
public:
	static Protective* makeObject(string& criteria);
};
class Fast : public Protective {
public: ~Fast() {
		DTOR("  ~Fast ", Lecture);
	}
public:
	void optimize() { cout << "  Fast optimized\n"; }
};
class Economic : public Protective {
public: ~Economic() {
		DTOR("  ~Economic ", Lecture);
	}
public:
	void optimize() { cout << "  Economic optimized\n"; }
};
class Critical : public Protective {
public: ~Critical() {
		DTOR("  ~Critical ", Lecture);
	}
public:
	void optimize() { cout << "  Critical optimized\n"; }
	void cleanup()  { cout << "  detailed cleanup\n"; }
};
class NPC : public Protective {
public: ~NPC() {
		DTOR("  ~NPC ", Lecture);
	}
public:
	void optimize() { cout << "  NPC optimized\n"; }
	void cleanup()  { cout << "  meticulous cleanup\n"; }
};
// Seam point - add another step.

Protective* Protective::makeObject(string& criteria) {
	if(criteria == "Fast")		return new Fast;
	if(criteria == "Economic")	return new Economic;
	if(criteria == "Critical")	return new Critical;
	if(criteria == "NPC")		return new NPC;
	// Seam point - insert another step.
	return new Fast; // Default.
}

void demo(int seqNo) {
	cout<<seqNo<<") << template_method::homework::solution::demo() >>\n";
	string criteria[] = { "Fast", "Economic", "Critical", "NPC" };
	vector<Protective*> diffs;
	for(size_t i=0; i<COUNT(criteria); i++) {
		Protective* steps = Protective::makeObject(criteria[i]);
		steps->coating();
		diffs.push_back(steps);
	}
	for(size_t i=0; i<COUNT(criteria); i++)
		delete diffs[i];
	cout << endl;
}

}

#endif /* SOLUTIONS_TEMPLATEMETHOD_H_ */

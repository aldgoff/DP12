/*
 * templateMethod.h
 *
 *  Created on: Aug 3, 2015
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

#ifndef LECTURES_TEMPLATEMETHOD_H_
#define LECTURES_TEMPLATEMETHOD_H_

#include "../DP.h"

namespace template_method {	// Command line: templateMethod [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << template_method::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << template_method::lecture::problem::demo() >>\n";
}

}

namespace solution {

class TemplateMethod {
public:
	TemplateMethod() {}
	virtual ~TemplateMethod() { DTOR("~TemplateMethodObserver\n", Architecture); }
public:
	virtual void run() {}
public:
	static TemplateMethod* decisionLogic(const string& criteria);
};
class Derived : public TemplateMethod {
public:
	Derived() {}
	virtual ~Derived() { DTOR("~Derived ", Architecture); }
public:
	void run() {}
};
// Seam point - add another step.

TemplateMethod* TemplateMethod::decisionLogic(const string& criteria) {
	if(		criteria == "whatever")	return new TemplateMethod;
	else if(criteria == "whatever")	return new TemplateMethod;
	// Seam point - add another TemplateMethod.

	else {
		return new TemplateMethod;
	}
}

void demo(int seqNo) {
	cout << seqNo << ") << template_method::lecture::solution::demo() >>\n";
	string criteria[] = { "Derived1", "Derived2", "Derived3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		TemplateMethod* derived = TemplateMethod::decisionLogic(criteria[i]);
		derived->run();
	}
	cout << endl;
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << template_method::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << template_method::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << template_method::homework::solution::demo() >>\n";
}

}

} // homework

namespace skeleton {

class TemplateMethod { // If the steps are varying...
public: virtual ~TemplateMethod() {}
public:
	void run() {
		cout << "  run - " << sameStep1();
		cout << " - " << sameStep2();
		cout << " - " << diffStep3();
		cout << " - " << sameStep4();
		cout << endl;
	}
protected:
	string sameStep1() { return "step1"; }
	string sameStep2() { return "step2"; }
	virtual string diffStep3() { return "oops!"; }
	string sameStep4() { return "step4"; }
public:
	static TemplateMethod* makeObject(string& criteria);
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
// Seam point - add another step.

TemplateMethod* TemplateMethod::makeObject(string& criteria) {
	if(		criteria == "Diff1Step3")	return new Diff1Step3;
	else if(criteria == "Diff2Step3")	return new Diff2Step3;
	else if(criteria == "Diff3Step3")	return new Diff3Step3;
	// Seam point - insert another step.
	else {							// Options:
		return new TemplateMethod;	// null, exception,
	}								// base, default, ABC.
}

void demo(int seqNo) {	// Test variations.
	string criteria[] = { "Diff1Step3", "Diff2Step3",
						  "Diff3Step3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		TemplateMethod* steps = TemplateMethod::makeObject(criteria[i]);
		steps->run();
	}
	cout << endl;
}

} // skeleton

namespace recognition {

class Process {
public: virtual ~Process() {}
public:
	void test() {
		cout << "  test - " << init();
		cout << " - " << start();
		cout << " - " << validate();
		cout << " - " << stop();
		cout << endl;
	}
protected:
	string init() { return "initialize"; }
	string start() { return "start"; }
	virtual string validate()=0;
	string stop() { return "stop"; }
public:
	static Process* makeObject(const string& criteria);
};
class Audio : public Process {
public:
	string validate() { return "Audio"; }
};
class CSME : public Process {
public:
	string validate() { return "CSME"; }
};
class WFST : public Process {
public:
	string validate() { return "WFST"; }
};
// Seam point - add another test sequence.

Process* Process::makeObject(const string& criteria) {
	if(		criteria == "Music")	return new Audio;
	else if(criteria == "Security")	return new CSME;
	else if(criteria == "Speech")	return new WFST;
	else { return 0; }	// ABC.
}

void demo() {	// Test variations.
	string criteria[] = { "Music", "Security", "Speech" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Process* sequence = Process::makeObject(criteria[i]);
		sequence->test();
	}
	cout << endl;
}

} // recognition

namespace refactoring {

namespace bad {

void soak() { cout << "    soak"; }
void wash() { cout << " - wash"; }
void boil() { cout << " - boil"; }		// Sterilize 1.
void ozone() { cout << " - ozone"; }	// Sterilize 2.
void uv() { cout << " - uv"; }			// Sterilize 3.
void rinse() { cout << " - rinse.\n"; }

void clientCode_A(int criteria) {	// Clutter.
	soak();
	wash();
	switch(criteria) {
	case 0:	boil(); break;	// Sterilize 1.
	case 1:	ozone(); break;	// Sterilize 2.
	case 2:	uv(); break;	// Sterilize 3.
	default: cout << " - Oops!"; break;	// Sterilize ?.
	}
	rinse();
}

void clientCode_B(int criteria) {	// Duplication.
	switch(criteria) {
	case 0:
		soak();
		wash();
		boil();		// Sterilize 1.
		rinse();
		break;
	case 1:
		soak();
		wash();
		ozone();	// Sterilize 2.
		rinse();
		break;
	case 2:
		soak();
		wash();
		uv();		// Sterilize 3.
		rinse();
		break;
	default:
		soak();
		wash();
		cout << " = Oops!";	// Sterilize ?.
		rinse();
		break;
	}
}

void demo() {
	cout << "  refactoring::bad::demo().\n";
	int test[] = { 0, 1, 2, 3 };
	for(size_t i=0; i<COUNT(test); i++)
		clientCode_A(test[i]);
	cout << endl;
	for(size_t i=0; i<COUNT(test); i++)
		clientCode_B(test[i]);
	cout << endl;
}

} // bad

namespace good {

class Sterilize {	// Template Method design pattern.
public: virtual ~Sterilize() {}
public:
	void run() { soak(); wash(); sanitize(); rinse(); }
public:
	void soak() { cout << "    soak"; }
	void wash() { cout << " - wash"; }
	virtual void sanitize() {cout << " - Oops!";}//Sterilize.
	void rinse() { cout << " - rinse.\n"; }
public:
	static Sterilize* makeObject(const string& criteria);
};
class Boil : public Sterilize {
public:
	void sanitize() { cout << " - boil"; }
};
class Ozone : public Sterilize {
public:
	void sanitize() { cout << " - ozone"; }
};
class UV : public Sterilize {
public:
	void sanitize() { cout << " - uv"; }
};
// Seam point - add another class.

Sterilize* Sterilize::makeObject(const string& criteria) {
	if(criteria == "Boil")	return new Boil;
	if(criteria == "Ozone")	return new Ozone;
	if(criteria == "UV")	return new UV;
	// Seam point - insert another criteria.
	else { return new Sterilize; }
}

void clientCode_A(Sterilize* clean) { clean->run(); }
void clientCode_B(Sterilize* clean) { clean->run(); }

void demo() {
	cout << "  refactoring::good::demo().\n";
	string criteria[] = { "Boil", "Ozone", "UV", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Sterilize* clean = Sterilize::makeObject(criteria[i]);
		clientCode_A(clean);
		delete clean;
	}
	cout << endl;
	for(size_t i=0; i<COUNT(criteria); i++) {
		Sterilize* clean = Sterilize::makeObject(criteria[i]);
		clientCode_B(clean);
		delete clean;
	}
	cout << endl;
}

} // good

} // refactoring

namespace practical_issues {

namespace story_1 {

void soak() { cout << "    soak"; }
void wash() { cout << " - wash"; }
void boil() { cout << " - boil"; }		// Sterilize 1.
void ozone() { cout << " - ozone"; }	// Sterilize 2.
void uv() { cout << " - uv"; }			// Sterilize 3.
void rinse() { cout << " - rinse.\n"; }

void clientCode_A(const string& criteria) {	// Clutter.
	soak();
	wash();
	if(		criteria == "Boil")		boil();
	else if(criteria == "Ozone")	ozone();
	else if(criteria == "UV")		uv();
	// Seam point - insert another criteria.
	else { cout << " - Oops!"; }
	rinse();
}

void clientCode_B(const string& criteria) {	// Duplication.
	if(criteria == "Boil") {
		soak();
		wash();
		boil();		// Sterilize 1.
		rinse();
		}
	else if(criteria == "Ozone") {
		soak();
		wash();
		ozone();	// Sterilize 2.
		rinse();
		}
	else if(criteria == "UV") {
		soak();
		wash();
		uv();		// Sterilize 3.
		rinse();
		}
	else {
		soak();
		wash();
		cout << " = Oops!";	// Sterilize ?.
		rinse();
	}
}

void demo() {
	cout << "  practical_issues::story_1::demo().\n";
	string criteria[] = {"Boil","Ozone","UV","oops"};
	for(size_t i=0; i<COUNT(criteria); i++)
		clientCode_A(criteria[i]);
	cout << endl;
	for(size_t i=0; i<COUNT(criteria); i++)
		clientCode_B(criteria[i]);
	cout << endl;
}

}

namespace refactor_1 {	// TM

class Sterilize {	// Template Method design pattern.
public: virtual ~Sterilize() {}
public:
	void run() { soak(); wash(); sanitize(); rinse(); }
public:
	void soak() { cout << "    soak"; }
	void wash() { cout << " - wash"; }
	virtual void sanitize() {cout << " - Oops!";}//Sterilize.
	void rinse() { cout << " - rinse.\n"; }
public:
	static Sterilize* makeObject(const string& criteria);
};
class Boil : public Sterilize {
public:
	void sanitize() { cout << " - boil"; }
};
class Ozone : public Sterilize {
public:
	void sanitize() { cout << " - ozone"; }
};
class UV : public Sterilize {
public:
	void sanitize() { cout << " - uv"; }
};
// Seam point - add another class.

Sterilize* Sterilize::makeObject(const string& criteria) {
	if(criteria == "Boil")	return new Boil;
	if(criteria == "Ozone")	return new Ozone;
	if(criteria == "UV")	return new UV;
	// Seam point - insert another criteria.
	else { return new Sterilize; }
}

void clientCode(Sterilize* clean) {
	clean->run();
}

void demo() {
	cout << "  practical_issues::refactor_1::demo().\n";
	string criteria[] = {"Boil","Ozone","UV","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Sterilize* clean = Sterilize::makeObject(criteria[i]);
		clientCode(clean);
		delete clean;
	}
	cout << endl;
}

}

namespace refactor_2 {	// Add another sanitize method (bleach).

class Sterilize {	// Template Method design pattern.
public: virtual ~Sterilize() {}
public:
	void run() { soak(); wash(); sanitize(); rinse(); }
public:
	void soak() { cout << "    soak"; }
	void wash() { cout << " - wash"; }
	virtual void sanitize() {cout << " - Oops!";}//Sterilize.
	void rinse() { cout << " - rinse.\n"; }
public:
	static Sterilize* makeObject(const string& criteria);
};
class Boil : public Sterilize {
public:
	void sanitize() { cout << " - boil"; }
};
class Ozone : public Sterilize {
public:
	void sanitize() { cout << " - ozone"; }
};
class UV : public Sterilize {
public:
	void sanitize() { cout << " - uv"; }
};
// Seam point - add another class.
class Bleach : public Sterilize {
public:
	void sanitize() { cout << " - bleach"; }
};

Sterilize* Sterilize::makeObject(const string& criteria) {
	if(criteria == "Boil")	return new Boil;
	if(criteria == "Ozone")	return new Ozone;
	if(criteria == "UV")	return new UV;
	// Seam point - insert another criteria.
	if(criteria == "Bleach")	return new Bleach;
	else { return new Sterilize; }
}

void clientCode(Sterilize* clean) {
	clean->run();
}

void demo() {
	cout << "  practical_issues::refactor_2::demo().\n";
	string criteria[] = {"Boil","Ozone","UV","Bleach","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Sterilize* clean = Sterilize::makeObject(criteria[i]);
		clientCode(clean);
		delete clean;
	}
	cout << endl;
}

}

namespace refactor_3 {	// Add constant step (polish).

class Sterilize {	// Template Method design pattern.
public: virtual ~Sterilize() {}
public:
	void run() { soak(); wash(); sanitize(); rinse(); polish(); }
public:
	void soak() { cout << "    soak"; }
	void wash() { cout << " - wash"; }
	virtual void sanitize() {cout << " - Oops!";}//Sterilize.
	void rinse() { cout << " - rinse"; }
	void polish() { cout << " - polish.\n"; }
public:
	static Sterilize* makeObject(const string& criteria);
};
class Boil : public Sterilize {
public:
	void sanitize() { cout << " - boil"; }
};
class Ozone : public Sterilize {
public:
	void sanitize() { cout << " - ozone"; }
};
class UV : public Sterilize {
public:
	void sanitize() { cout << " - uv"; }
};
// Seam point - add another class.
class Bleach : public Sterilize {
public:
	void sanitize() { cout << " - bleach"; }
};

Sterilize* Sterilize::makeObject(const string& criteria) {
	if(criteria == "Boil")	return new Boil;
	if(criteria == "Ozone")	return new Ozone;
	if(criteria == "UV")	return new UV;
	// Seam point - insert another criteria.
	if(criteria == "Bleach")	return new Bleach;
	else { return new Sterilize; }
}

void clientCode(Sterilize* clean) {
	clean->run();
}

void demo() {
	cout << "  practical_issues::refactor_3::demo().\n";
	string criteria[] = {"Boil","Ozone","UV","Bleach","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Sterilize* clean = Sterilize::makeObject(criteria[i]);
		clientCode(clean);
		delete clean;
	}
	cout << endl;
}

}

namespace refactor_4 {	// Make a constant step variable (wash -> agitate).

class Sterilize {	// Template Method design pattern.
public: virtual ~Sterilize() {}
public:
	void run() { soak(); wash(); sanitize(); rinse(); polish(); }
public:
	void soak() { cout << "    soak"; }
	virtual void wash() { cout << " - wash"; }
	virtual void sanitize() {cout << " - Oops!";}//Sterilize.
	void rinse() { cout << " - rinse"; }
	void polish() { cout << " - polish.\n"; }
public:
	static Sterilize* makeObject(const string& criteria);
};
class Boil : public Sterilize {
public:
	void sanitize() { cout << " - boil"; }
};
class Ozone : public Sterilize {
public:
	void sanitize() { cout << " - ozone"; }
};
class UV : public Sterilize {
public:
	void sanitize() { cout << " - uv"; }
};
// Seam point - add another class.
class Bleach : public Sterilize {
public:
	void wash() { cout << " - agitate"; }
	void sanitize() { cout << " - bleach"; }
};

Sterilize* Sterilize::makeObject(const string& criteria) {
	if(criteria == "Boil")	return new Boil;
	if(criteria == "Ozone")	return new Ozone;
	if(criteria == "UV")	return new UV;
	// Seam point - insert another criteria.
	if(criteria == "Bleach")	return new Bleach;
	else { return new Sterilize; }
}

void clientCode(Sterilize* clean) {
	clean->run();
}

void demo() {
	cout << "  practical_issues::refactor_4::demo().\n";
	string criteria[] = {"Boil","Ozone","UV","Bleach","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Sterilize* clean = Sterilize::makeObject(criteria[i]);
		clientCode(clean);
		delete clean;
	}
	cout << endl;
}

}

namespace refactor_5 {	// Acceleration (run: sand abrasion, steel wool finish).

class Sterilize {	// Template Method design pattern.
public: virtual ~Sterilize() {}
public:
	virtual void run() { soak(); wash(); sanitize(); rinse(); polish(); }
public:
	void soak() { cout << "    soak"; }
	virtual void wash() { cout << " - wash"; }
	virtual void sanitize() {cout << " - Oops!";}//Sterilize.
	void rinse() { cout << " - rinse"; }
	void polish() { cout << " - polish.\n"; }
public:
	static Sterilize* makeObject(const string& criteria);
};
class Boil : public Sterilize {
public:
	void sanitize() { cout << " - boil"; }
};
class Ozone : public Sterilize {
public:
	void sanitize() { cout << " - ozone"; }
};
class UV : public Sterilize {
public:
	void sanitize() { cout << " - uv"; }
};
class Bleach : public Sterilize {
public:
	void wash() { cout << " - agitate"; }
	void sanitize() { cout << " - bleach"; }
};
// Seam point - add another class.
class Abrasion : public Sterilize {
public:
	void run() {
		cout << "    run";
		cout << " - sand abrasion";
		cout << " - steel wool finish.\n";
	}
};

Sterilize* Sterilize::makeObject(const string& criteria) {
	if(criteria == "Boil")	return new Boil;
	if(criteria == "Ozone")	return new Ozone;
	if(criteria == "UV")	return new UV;
	if(criteria == "Bleach")	return new Bleach;
	// Seam point - insert another criteria.
	if(criteria == "Abrasion")	return new Abrasion;
	else { return new Sterilize; }
}

void clientCode(Sterilize* clean) {
	clean->run();
}

void demo() {
	cout << "  practical_issues::refactor_5::demo().\n";
	string criteria[] = {"Boil","Ozone","UV","Bleach","Abrasion"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Sterilize* clean = Sterilize::makeObject(criteria[i]);
		clientCode(clean);
		delete clean;
	}
	cout << endl;
}

}

namespace refactor_6 {	// Acceleration (rinse has different criteria).

class Strategy {
public:
	virtual ~Strategy() {}
public:
	virtual void algorithm() { cout << " - rinse"; }
public:
	static Strategy* makeObject(string& logic);
};
class Tap : public Strategy {
public:
	void algorithm() { cout << " - rinse(tap)"; }
};
class Distilled : public Strategy {
public:
	void algorithm() { cout << " - rinse(distilled)"; }
};
class FuelCell : public Strategy {
public:
	void algorithm() { cout << " - rinse(fuel-cell)"; }
};
// Seam point - add another rinse algorithm.

Strategy* Strategy::makeObject(string& logic) {
	if(logic == "Tap")			return new Tap;
	if(logic == "Distilled")	return new Distilled;
	if(logic == "FuelCell")		return new FuelCell;
	// Seam point - insert another logic.
	else { return new Strategy; }
}

class Sterilize {	// Template Method design pattern.
public:
	Strategy* strategy;
public:
	Sterilize() : strategy(0) {}
	virtual ~Sterilize() { delete strategy; }
public:
	virtual void run() { soak(); wash(); sanitize(); rinse(); polish(); }
public:
	void soak() { cout << "    soak"; }
	virtual void wash() { cout << " - wash"; }
	virtual void sanitize() {cout << " - Oops!";}//Sterilize.
	void rinse() { strategy->algorithm(); }
	void polish() { cout << " - polish.\n"; }
public:
	static Sterilize* makeObject(const string& criteria);
};
class Boil : public Sterilize {
public:
	void sanitize() { cout << " - boil"; }
};
class Ozone : public Sterilize {
public:
	void sanitize() { cout << " - ozone"; }
};
class UV : public Sterilize {
public:
	void sanitize() { cout << " - uv"; }
};
class Bleach : public Sterilize {
public:
	void wash() { cout << " - agitate"; }
	void sanitize() { cout << " - bleach"; }
};
// Seam point - add another class.
class Abrasion : public Sterilize {
public:
	void run() {
		cout << "    run";
		cout << " - sand abrasion";
		cout << " - steel wool finish.\n";
	}
};

Sterilize* Sterilize::makeObject(const string& criteria) {
	if(criteria == "Boil")	return new Boil;
	if(criteria == "Ozone")	return new Ozone;
	if(criteria == "UV")	return new UV;
	if(criteria == "Bleach")	return new Bleach;
	// Seam point - insert another criteria.
	if(criteria == "Abrasion")	return new Abrasion;
	else { return new Sterilize; }
}

void clientCode(Sterilize* clean) {
	clean->run();
}

void demo(int seqNo) {	// Test variations.
	cout << seqNo << ") << template_method::practical_issues >>\n";
	cout << "  practical_issues::refactor_6::demo().\n";
	string criteria[] = {"Boil","Ozone","UV","Bleach","Abrasion"};
	string logic[] = {"Tap","Distilled","FuelCell"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Sterilize* clean = Sterilize::makeObject(criteria[i]);
		for(size_t j=0; j<COUNT(logic); j++) {
			clean->strategy = Strategy::makeObject(logic[j]);
			clientCode(clean);
		}
		delete clean;
	}
	cout << endl;
}

}

}

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "templateMethod") {}
	virtual ~Observer() { DTOR("~TemplateMethodObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << template_method::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << template_method::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << template_method::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << template_method::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << template_method::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << template_method::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << template_method::skeleton >>\n";
		skeleton::demo(seqNo);
	}
	virtual void recognition() {
		cout << seqNo << ") << template_method::recognition >>\n";
		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << template_method::refactoring >>\n";
		refactoring::bad::demo();
		refactoring::good::demo();
	}
	virtual void practicalIssues() {
		cout << seqNo << ") << template_method::practical_issues >>\n";
		practical_issues::story_1::demo();
		practical_issues::refactor_1::demo();
		practical_issues::refactor_2::demo();
		practical_issues::refactor_3::demo();
		practical_issues::refactor_4::demo();
		practical_issues::refactor_5::demo();
		practical_issues::refactor_6::demo(seqNo);
	}
};

} // template_method

#endif /* LECTURES_TEMPLATEMETHOD_H_ */

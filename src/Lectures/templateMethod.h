/*
 * templateMethod.h
 *
 *  Created on: Aug 2, 2015
 *      Author: aldgoff
 *
 * Varies: If the steps are varying, use the Template Method pattern.
 *
 * Desc:
 *
 * Category: whatever
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
// Seam point - add another Derived.

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

class TemplateMethod {	// If the steps are varying...
public: virtual ~TemplateMethod() {}
public:
	virtual void run() {
		cout << "  run - " << sameStep1();
		cout << " - " << sameStep2();
		cout << " - " << diffStep3();
		cout << " - " << sameStep4();
		cout << endl;
	}
protected:
	string sameStep1() { return "step1"; }
	string sameStep2() { return "step2"; }
	virtual string diffStep3() {return "oops!"; }
	string sameStep4() { return "step4"; }
public:
	static TemplateMethod* decisionLogic(const string& criteria);
};
class DiffStep1 : public TemplateMethod {
public:
	string diffStep3() { return "DiffStep1.step3"; }
};
class DiffStep2 : public TemplateMethod {
public:
	string diffStep3() { return "DiffStep2.step3"; }
};
class DiffStep3 : public TemplateMethod {
public:
	string diffStep3() { return "DiffStep3.step3"; }
};
// Seam point - add another derived.

TemplateMethod* TemplateMethod::decisionLogic(const string& criteria) {
	if(		criteria == "DiffStep1")	return new DiffStep1;
	else if(criteria == "DiffStep2")	return new DiffStep2;
	else if(criteria == "DiffStep3")	return new DiffStep3;

	else {
		return new TemplateMethod;	// Or throw exception, default, or an ABC.
	}
}

void demo(int seqNo) {
	string criteria[] = { "DiffStep1", "DiffStep2", "DiffStep3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		TemplateMethod* steps = TemplateMethod::decisionLogic(criteria[i]);
		steps->run();
	}
	cout << endl;
}

} // skeleton

class TemplateMethodObserver : public observer::DPObserver {
public:
	TemplateMethodObserver(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "templateMethod") {}
	virtual ~TemplateMethodObserver() { DTOR("~TemplateMethodObserver ", Architecture); }
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
};

} // template_method

#endif /* LECTURES_TEMPLATEMETHOD_H_ */

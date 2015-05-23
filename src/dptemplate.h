/*
 * dptemplate.h
 *
 *  Created on: May 23, 2015
 *      Author: aldgoff
 *
 * Varies: If the whatever are varying, use the DPTemplate pattern.
 *
 * Desc:
 *
 * Category: whatever
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Dptemplate
 *  	http://www.dofactory.com/net/dptemplate-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter7
 *  	http://sourcemaking.com/design_patterns/dptemplate
 */

#ifndef LECTURES_DPTEMPLATE_H_
#define LECTURES_DPTEMPLATE_H_

#include "DP.h"		// Delete, uncomment below.
//#include "../DP.h"

namespace dptemplate {	// Command line: dptemplate [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << dptemplate::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << dptemplate::lecture::problem::demo() >>\n";
}

}

namespace solution {

class DPTemplate {
public:
	DPTemplate() {}
	virtual ~DPTemplate() { DTOR("~DPtemplateObserver\n", Architecture); }
public:
	virtual void run() {}
public:
	static DPTemplate* create(string criteria);
};
class Derived : public DPTemplate {
public:
	Derived() {}
	virtual ~Derived() { DTOR("~Derived ", Architecture); }
public:
	void run() {}
};
// Seam point - add another DPTemplate.

DPTemplate* DPTemplate::create(string criteria) {
	if(		criteria == "whatever")	return new DPTemplate;
	else if(criteria == "whatever")	return new DPTemplate;
	// Seam point - add another DPTemplate.
	else {
		return new DPTemplate;
	}
}

void demo(int seqNo) {
	cout << seqNo << ") << dptemplate::lecture::solution::demo() >>\n";
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << dptemplate::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << dptemplate::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << dptemplate::homework::solution::demo() >>\n";
}

}

} // homework

class DPtemplateObserver : public observer::DPObserver {
public:
	DPtemplateObserver(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "dptemplate") {}
	virtual ~DPtemplateObserver() { DTOR("~DPtemplateObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << dptemplate::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << dptemplate::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << dptemplate::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << dptemplate::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << dptemplate::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << dptemplate::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
};

} // dptemplate

#endif /* LECTURES_DPTEMPLATE_H_ */

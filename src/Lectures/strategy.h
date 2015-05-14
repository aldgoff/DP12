/*
 * strategy.h
 *
 *  Created on: May 11, 2015
 *      Author: aldgoff
 *
 * Varies: If the algorithms are varying, use the Strategy pattern.
 *
 * Desc: Define a family of interchangeable algorithms which can vary independently from the clients.
 *
 * Category: Behavioral
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Strategy
 *  	http://www.dofactory.com/net/strategy-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter9
 *  	http://sourcemaking.com/design_patterns/strategy
 */

#ifndef LECTURES_STRATEGY_H_
#define LECTURES_STRATEGY_H_

#include "../DP.h"

namespace strategy {	// Command line: strategy [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << strategy::lecture::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << strategy::lecture::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << strategy::lecture::solution::demo() >>\n";
}

}

} // lecture

namespace homework {

#include "../Problems/strategy.h"

#include "../Solutions/strategy.h"

} // homework

class StrategyObserver : public observer::DPObserver {
public:
	StrategyObserver(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "strategy") {}
	virtual ~StrategyObserver() { DTOR("~StrategyObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << strategy::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << strategy::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << strategy::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << strategy::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << strategy::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << strategy::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
};

} // strategy

#endif /* LECTURES_STRATEGY_H_ */

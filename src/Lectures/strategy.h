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

namespace skeleton {

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
// Seam point - add another algorithm.

void demo(int seqNo) {
	Strategy* algorithms[] = { new Algorithm1, new Algorithm2, new Algorithm3 };
	for(size_t i=0; i<COUNT(algorithms); i++) {
		algorithms[i]->compute();
	}
	cout << endl;
}

namespace TM {

class Strategy {	// If the algorithms are varying...
public:	virtual ~Strategy() {}
public:
	virtual void compute() { cout << "  Oops!\n"; }
public:
	static Strategy* makeObject(const string& criteria);
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
// Seam point - add another algorithm.

Strategy* Strategy::makeObject(const string& criteria) {
	if(		criteria == "Algorithm1")	return new Algorithm1;
	else if(criteria == "Algorithm2")	return new Algorithm2;
	else if(criteria == "Algorithm3")	return new Algorithm3;

	else { return new Strategy; }	// Opts: null, exception, base, default, ABC.
}

void demo(int seqNo) {	// Decouples client from creation.
	string criteria[] = { "Algorithm1", "Algorithm2", "Algorithm3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Strategy* algorithm = Strategy::makeObject(criteria[i]);
		algorithm->compute();
	}
	cout << endl;
}

} // TM

} // skeleton

namespace recognition {

float spline(float x, float y, float a, float b) {
	// Code omitted...
	return 2.7182;	// Simulated answer.
}

class Fit {
protected:
	float x, m, a, b, y;
public:
	Fit() : x(1.1), m(1), a(.5), b(1), y(0) {}
	virtual ~Fit() {}
public:
	virtual float est() { return 0; }
public:
	static Fit* makeObject(const string& criteria);
};
class Linear : public Fit {
public:
	float est() { return y = m*x + b; }
};
class Poly : public Fit {
public:
	float est() { return y = a*x*x + m*x + b; }
};
class Spline : public Fit {
public:
	float est() { return y = spline(x, m, a, b); }
};

Fit* Fit::makeObject(const string& criteria) {
	if(		criteria == "BackOfTheEnvelope")	return new Linear;
	else if(criteria == "QuickAndDirty")	return new Poly;
	else if(criteria == "Exact")		return new Spline;
	else { return new Spline; }	// Default.
}

void demo() {	// Decouples client from creation.
	string criteria[] = {"BackOfTheEnvelope","QuickAndDirty","Exact"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Fit* fit = Fit::makeObject(criteria[i]);
		cout << "  " << fit->est() << "\n";
	}
	cout << endl;
}

} // recognition

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "strategy") {}
	virtual ~Observer() { DTOR("~StrategyObserver ", Architecture); }
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
	virtual void skeleton() {
		cout << seqNo << ") << strategy::skeleton >>\n";
		skeleton::demo(seqNo);
		skeleton::TM::demo(seqNo);
	}
	virtual void recognition() {
		cout << seqNo << ") << strategy::recognition >>\n";
		recognition::demo();
	}
};

} // strategy

#endif /* LECTURES_STRATEGY_H_ */

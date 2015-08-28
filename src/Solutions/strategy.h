/*
 * strategy.h
 *
 *  Created on: Aug 27, 2015
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

#ifndef SOLUTIONS_STRATEGY_H_
#define SOLUTIONS_STRATEGY_H_

namespace solution {

enum FirmWare {
	PROD_FW,
	DIAG_FW,
	SYM_FW,
	APP_FW
};

class ValidateIP {	// Strategy design pattern.
public:
	virtual ~ValidateIP() {
		DTOR("~ValidateIP\n", Homework);
	}
public:
	virtual void start() {}
	virtual void test() {}
	virtual void validate() {}
	virtual void stop() {}
};
class ProdFW : public ValidateIP {
public:
	virtual ~ProdFW() {
		DTOR("  ~ProdFW ", Homework);
	}
public:
	void start() { cout << "  ProdFW.start()"; }
	void test() { cout << " ProdFW.test()"; }
	void validate() { cout << " ProdFW.validate()"; }
	void stop() { cout << " ProdFW.stop().\n"; }
};
class DiagFW : public ValidateIP {
public:
	virtual ~DiagFW() {
		DTOR("  ~DiagFW ", Homework);
	}
public:
	void start() { cout << "  DiagFW.start()"; }
	void test() { cout << " DiagFW.test()"; }
	void validate() { cout << " DiagFW.validate()"; }
	void stop() { cout << " DiagFW.stop().\n"; }
};
class SymFW : public ValidateIP {
public:
	virtual ~SymFW() {
		DTOR("  ~SymFW ", Homework);
	}
public:
	void start() { cout << "  SymFW.start()"; }
	void test() { cout << " SymFW.test()"; }
	void validate() { cout << " SymFW.validate()"; }
	void stop() { cout << " SymFW.stop().\n"; }
};
// Seam point - add another strategy.
class AppFW : public ValidateIP {
public:
	virtual ~AppFW() {
		DTOR("  ~AppFW ", Homework);
	}
public:
	void start() { cout << "  AppFW.start()"; }
	void test() { cout << " AppFW.test()"; }
	void validate() { cout << " AppFW.validate()"; }
	void stop() { cout << " AppFW.stop().\n"; }
};

void clientCode(ValidateIP* algorithm) {	// Client code polymorphic.
	algorithm->start();
	algorithm->test();
	algorithm->validate();
	algorithm->stop();
}

void demo(int seqNo) {	// Test all the daughter classes.
	cout << seqNo << ") << strategy::homework::solution::demo() >>\n";
	FirmWare criteria[] = { PROD_FW, DIAG_FW, SYM_FW, APP_FW };
	ValidateIP*	val[COUNT(criteria)] = {0};

	for(size_t i=0; i<COUNT(criteria); i++) {
		switch(criteria[i]) {
		case PROD_FW: val[i] = new ProdFW; break;
		case DIAG_FW: val[i] = new DiagFW; break;
		case SYM_FW:  val[i] = new SymFW;  break;
		case APP_FW:  val[i] = new AppFW;  break;
		}
		clientCode(val[i]);
	}

	for(size_t i=0; i<COUNT(criteria); i++) {
		delete val[i];
	}
	cout << endl;

}

}

#endif /* SOLUTIONS_STRATEGY_H_ */

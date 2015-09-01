/*
 * strategy.h
 *
 *  Created on: May 12, 2015
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

#ifndef PROBLEMS_STRATEGY_H_
#define PROBLEMS_STRATEGY_H_

/* Consider the challenge of testing hardware.
 * 1) There are multiple operations to be performed (start, stop, etc.).
 * 2) The firmware installed on the hardware can also differ.
 *
 * Below is legacy code that might result from the first user story.
 * Initially it doesn't look too bad, but as each new user story is implemented,
 * the quadratic scaling law of change in two dimensions steadily erodes
 * programmer velocity.
 *
 * Use the strategy pattern to
 * 1) Avoid the quadratic scaling
 * 2) Eliminate the duplicated decision making
 * 3) Follow the Open/Closed principle
 * 4) Simplify the client code
 */

namespace legacy {

enum FirmWare {
	PROD_FW,
	DIAG_FW,
};

void clientCode(FirmWare firmware) {
	// Start.
	switch(firmware) {
	case PROD_FW:
		cout << "  ProdFW.start()";
		break;
	case DIAG_FW:
		cout << "  DiagFW.start()";
		break;
	// Seam point - insert another type of firmware.
	default:
		cout << "  Oops!";
		break;
	}

	// Seam point - insert another operation.

	// Stop.
	switch(firmware) {
	case PROD_FW:
		cout << " ProdFW.stop().\n";
		break;
	case DIAG_FW:
		cout << " DiagFW.stop().\n";
		break;
	// Seam point - insert another type of firmware.
	default:
		cout << " Oops!";
		break;
	}
};

void demo(int seqNo) {
	cout << seqNo << ") << strategy::homework::legacy::demo() >>\n";
	FirmWare criteria[] = { PROD_FW, DIAG_FW };
	for(size_t i=0; i<COUNT(criteria); i++) {
		clientCode(criteria[i]);
	}
	cout << endl;
}

}

namespace problem {

enum FirmWare {
	PROD_FW,
	DIAG_FW,
	SYM_FW,
	APP_FW
};

void clientCode(FirmWare firmware) {
	// Start.
	switch(firmware) {
	case PROD_FW:
		cout << "  ProdFW.start()";
		break;
	case DIAG_FW:
		cout << "  DiagFW.start()";
		break;
	case SYM_FW:
		cout << "  SymFW.start()";
		break;
	case APP_FW:
		cout << "  AppFW.start()";
		break;
	// Seam point - insert another type of firmware.
	default:
		cout << "  Oops!";
		break;
	}

	// Test.
	switch(firmware) {
	case PROD_FW:
		cout << " ProdFW.test()";
		break;
	case DIAG_FW:
		cout << " DiagFW.test()";
		break;
	case SYM_FW:
		cout << " SymFW.test()";
		break;
	case APP_FW:
		cout << " AppFW.test()";
		break;
	// Seam point - insert another type of firmware.
	default:
		cout << " Oops!";
		break;
	}

	// Validate.
	switch(firmware) {
	case PROD_FW:
		cout << " ProdFW.validate()";
		break;
	case DIAG_FW:
		cout << " DiagFW.validate()";
		break;
	case SYM_FW:
		cout << " SymFW.validate()";
		break;
	case APP_FW:
		cout << " AppFW.validate()";
		break;
	// Seam point - insert another type of firmware.
	default:
		cout << " Oops!";
		break;
	}

	// Seam point - insert another operation.

	// Stop.
	switch(firmware) {
	case PROD_FW:
		cout << " ProdFW.stop().\n";
		break;
	case DIAG_FW:
		cout << " DiagFW.stop().\n";
		break;
	case SYM_FW:
		cout << " SymFW.stop().\n";
		break;
	case APP_FW:
		cout << " AppFW.stop().\n";
		break;
	// Seam point - insert another type of firmware.
	default:
		cout << " Oops!";
		break;
	}
}

void demo(int seqNo) {
	cout << seqNo << ") << strategy::homework::problem::demo() >>\n";
	FirmWare criteria[] = { PROD_FW, DIAG_FW, SYM_FW, APP_FW };
	for(size_t i=0; i<COUNT(criteria); i++) {
		clientCode(criteria[i]);
	}
	cout << endl;
}

}

#endif /* PROBLEMS_STRATEGY_H_ */

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

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << strategy::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << strategy::homework::problem::demo() >>\n";
}

}

#endif /* PROBLEMS_STRATEGY_H_ */

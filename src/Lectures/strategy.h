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

namespace story_1 {	// Single variation & criteria.

void clientCode_A(int criteria) {	// In file A.
	switch(criteria) {
	case 1:	cout << "  Pre-order traversal.\n";		break;
	case 2:	cout << "  In-order traversal.\n";		break;
	case 3:	cout << "  Post-order traversal.\n";	break;
	// Seam point - insert another criteria.
	default: cout << "  Oops!\n";	break;
	}
}

void demo() {
	cout << "  legacy::story_1::demo().\n";
	int test[] = { 1, 2, 3, 0 };
	for(size_t i=0; i<COUNT(test); i++) {
		clientCode_A(test[i]);	// traversal
	}
	cout << endl;
}

} // story_1

void demo(int seqNo) {
	story_1::demo();
}

}

namespace problem {

namespace story_2 {	// Add second variation but with same criteria.

void clientCode_A(int criteria) {	// In file A.
	switch(criteria) {	// See file B.
	case 1:	cout << "  Pre-order traversal";	break;
	case 2:	cout << "  In-order traversal";		break;
	case 3:	cout << "  Post-order traversal";	break;
	// Seam point - insert another criteria.
	default: cout << "  Oops";	break;
	}
}
void clientCode_B(int criteria) {	// In file B.
	switch(criteria) {	// See file A.
	case 1:	cout << " - heap parse.\n";			break;
	case 2:	cout << " - token parse.\n";		break;
	case 3:	cout << " - predictive parse.\n";	break;
	// Seam point - insert another criteria.
	default: cout << " - oops!\n";	break;
	}
}

void demo() {
	cout << "  problem::story_2::demo().\n";
	int test[] = { 1, 2, 3, 0 };
	for(size_t i=0; i<COUNT(test); i++) { // Correlated:
		clientCode_A(test[i]);	// traversal
		clientCode_B(test[i]);	// parse
	}
	cout << endl;
}

} // story_2

namespace story_3 {	// Add third variation also with same criteria.

void clientCode_A(int criteria) {	// In file A.
	switch(criteria) {	// See files B,C.
	case 1:	cout << "  Pre-order traversal";	break;
	case 2:	cout << "  In-order traversal";		break;
	case 3:	cout << "  Post-order traversal";	break;
	// Seam point - insert another criteria.
	default: cout << "  Oops";	break;
	}
}
void clientCode_B(int criteria) {	// In file B.
	switch(criteria) {	// See files A,C.
	case 1:	cout << " - heap parse";		break;
	case 2:	cout << " - token parse";		break;
	case 3:	cout << " - predictive parse";	break;
	// Seam point - insert another criteria.
	default: cout << " - oops";	break;
	}
}
void clientCode_C(int criteria) {	// In file C.
	switch(criteria) {	// See files A,B.
	case 1:	cout << " - preHeap swarm.\n";		break;
	case 2:	cout << " - inToken swarm.\n";		break;
	case 3:	cout << " - postPredict swarm.\n";	break;
	// Seam point - insert another criteria.
	default: cout << " - oops!\n";	break;
	}
}

void demo() {
	cout << "  problem::story_3::demo().\n";
	int test[] = { 1, 2, 3, 0 };
	for(size_t i=0; i<COUNT(test); i++) { // Correlated:
		clientCode_A(test[i]);	// traversal
		clientCode_B(test[i]);	// parse
		clientCode_C(test[i]);	// swarm
	}
	cout << endl;
}

} // story_3

namespace story_4 {	// Extend all three variations.

void clientCode_A(int criteria) {	// In file A.
	switch(criteria) {	// See files B,C.
	case 1:	cout << "  Pre-order traversal";	break;
	case 2:	cout << "  In-order traversal";		break;
	case 3:	cout << "  Post-order traversal";	break;
	// Seam point - insert another criteria.
	case 4:	cout << "  Sample traversal";		break;
	default: cout << "  Oops";	break;
	}
}
void clientCode_B(int criteria) {	// In file B.
	switch(criteria) {	// See files A,C.
	case 1:	cout << " - heap parse";		break;
	case 2:	cout << " - token parse";		break;
	case 3:	cout << " - predictive parse";	break;
	// Seam point - insert another criteria.
	case 4:	cout << " - neural parse";		break;
	default: cout << " - oops";	break;
	}
}
void clientCode_C(int criteria) {	// In file C.
	switch(criteria) {	// See files A,B.
	case 1:	cout << " - preHeap swarm.\n";		break;
	case 2:	cout << " - inToken swarm.\n";		break;
	case 3:	cout << " - postPredict swarm.\n";	break;
	// Seam point - insert another criteria.
	case 4:	cout << " - sampleNeural swarm.\n";	break;
	default: cout << " - oops!\n";	break;
	}
}

void demo() {
	cout << "  problem::story_4::demo().\n";
	int test[] = { 1, 2, 3, 4, 0 };
	for(size_t i=0; i<COUNT(test); i++) { // Correlated:
		clientCode_A(test[i]);	// traversal
		clientCode_B(test[i]);	// parse
		clientCode_C(test[i]);	// swarm
	}
	cout << endl;
}

} // story_4

void demo(int seqNo) {
	story_2::demo();
	story_3::demo();
	story_4::demo();
}

}

namespace solution {

namespace refactor_3 {	// Refactor with Strategy design pattern.

class Strategy {
public:
	virtual ~Strategy() {
		DTOR("~Strategy\n", Lecture);
	}
public:
	virtual void traverse() { cout << "  Oops"; }
	virtual void parse() { cout << " - oops"; }
	virtual void swarm() { cout << " - oops!\n"; }
};
class Scheme1 : public Strategy {
public: ~Scheme1() {
		DTOR("  ~Scheme1 ", Lecture);
	}
public:
	void traverse() { cout << "  Pre-order traversal"; }
	void parse() { cout << " - heap parse"; }
	void swarm() { cout << " - preHeap swarm.\n"; }
};
class Scheme2 : public Strategy {
public: ~Scheme2() {
		DTOR("  ~Scheme2 ", Lecture);
	}
public:
	void traverse() { cout << "  In-order traversal"; }
	void parse() { cout << " - token parse"; }
	void swarm() { cout << " - inToken swarm.\n"; }
};
class Scheme3 : public Strategy {
public: ~Scheme3() {
		DTOR("  ~Scheme3 ", Lecture);
	}
public:
	void traverse() { cout << "  Post-order traversal"; }
	void parse() { cout << " - predictive parse"; }
	void swarm() { cout << " - postPredict swarm.\n"; }
};
// Seam point - add another strategy.

void clientCode_A(Strategy* algorithm) {	// In file A.
	algorithm->traverse();
}
void clientCode_B(Strategy* algorithm) {	// In file B.
	algorithm->parse();
}
void clientCode_C(Strategy* algorithm) {	// In file C.
	algorithm->swarm();
}

void demo() {
	cout << "  solution::refactor_3::demo().\n";
	int criteria[] = { 1, 2, 3 };
	Strategy* scheme[COUNT(criteria)] = {0};
	for(size_t i=0; i<COUNT(criteria); i++) {
		switch(criteria[i]) {
		case 1:	scheme[i] = new Scheme1; break;
		case 2:	scheme[i] = new Scheme2; break;
		case 3:	scheme[i] = new Scheme3; break;
		}						// Correlated:
		clientCode_A(scheme[i]);	// traversal
		clientCode_B(scheme[i]);	// parse
		clientCode_C(scheme[i]);	// swarm
	}
	for(size_t i=0; i<COUNT(criteria); i++) {
		delete scheme[i];
	}
	cout << endl;
}

} // refactor_3

namespace refactor_4 {	// Refactor with Strategy design pattern.

class Strategy {
public:
	virtual ~Strategy() {
		DTOR("~Strategy\n", Lecture);
	}
public:
	virtual void traverse() { cout << "  Oops"; }
	virtual void parse() { cout << " - oops"; }
	virtual void swarm() { cout << " - oops!\n"; }
};
class Scheme1 : public Strategy {
public: ~Scheme1() {
		DTOR("  ~Scheme1 ", Lecture);
	}
public:
	void traverse() { cout << "  Pre-order traversal"; }
	void parse() { cout << " - heap parse"; }
	void swarm() { cout << " - preHeap swarm.\n"; }
};
class Scheme2 : public Strategy {
public: ~Scheme2() {
		DTOR("  ~Scheme2 ", Lecture);
	}
public:
	void traverse() { cout << "  In-order traversal"; }
	void parse() { cout << " - token parse"; }
	void swarm() { cout << " - inToken swarm.\n"; }
};
class Scheme3 : public Strategy {
public: ~Scheme3() {
		DTOR("  ~Scheme3 ", Lecture);
	}
public:
	void traverse() { cout << "  Post-order traversal"; }
	void parse() { cout << " - predictive parse"; }
	void swarm() { cout << " - postPredict swarm.\n"; }
};
// Seam point - add another strategy.
class Scheme4 : public Strategy {
public: ~Scheme4() {
		DTOR("  ~Scheme4 ", Lecture);
	}
public:
	void traverse() { cout << "  Sample traversal"; }
	void parse() { cout << " - neural parse"; }
	void swarm() { cout << " - sampleNeural swarm.\n"; }
};

void clientCode_A(Strategy* algorithm) {	// In file A.
	algorithm->traverse();
}
void clientCode_B(Strategy* algorithm) {	// In file B.
	algorithm->parse();
}
void clientCode_C(Strategy* algorithm) {	// In file C.
	algorithm->swarm();
}

void demo() {
	cout << "  solution::refactor_4::demo().\n";
	int criteria[] = { 1, 2, 3, 4 };
	Strategy* scheme[COUNT(criteria)] = {0};
	for(size_t i=0; i<COUNT(criteria); i++) {
		switch(criteria[i]) {
		case 1:	scheme[i] = new Scheme1; break;
		case 2:	scheme[i] = new Scheme2; break;
		case 3:	scheme[i] = new Scheme3; break;
		case 4:	scheme[i] = new Scheme4; break;
		}						// Correlated:
		clientCode_A(scheme[i]);	// traversal
		clientCode_B(scheme[i]);	// parse
		clientCode_C(scheme[i]);	// swarm
	}
	for(size_t i=0; i<COUNT(criteria); i++)
		delete scheme[i];
	cout << endl;
}

} // refactor_4

void demo(int seqNo) {
	refactor_3::demo();
	refactor_4::demo();
}

}

} // lecture

namespace homework {

#include "../Problems/strategy.h"

#include "../Solutions/strategy.h"

} // homework

namespace skeleton {

class Strategy { // If the algorithms are varying...
public:
	virtual ~Strategy() {
		DTOR("~Strategy\n", Lecture);
	}
public:
	virtual void compute() {}
};
class Algorithm1 : public Strategy {
public: ~Algorithm1() {
		DTOR("  ~Algorithm1 ", Lecture);
	}
public:
	void compute() { cout << "  Algorithm1."; }
};
class Algorithm2 : public Strategy {
public: ~Algorithm2() {
		DTOR("  ~Algorithm2 ", Lecture);
	}
public:
	void compute() { cout << "  Algorithm2."; }
};
class Algorithm3 : public Strategy {
public: ~Algorithm3() {
		DTOR("  ~Algorithm3 ", Lecture);
	}
public:
	void compute() { cout << "  Algorithm3."; }
};
// Seam point - add another algorithm.

void demo(int seqNo) {	// Test variations.
	Strategy* algorithms[] = {
		new Algorithm1,
		new Algorithm2,
		new Algorithm3
	};
	for(size_t i=0; i<COUNT(algorithms); i++) {
		algorithms[i]->compute();
		delete algorithms[i];
	}
	cout << endl;
}

namespace FM {

class Strategy { // If the algorithms are varying...
public:
	virtual ~Strategy() {
		DTOR("~Strategy\n", Lecture);
	}
public:
	virtual void compute() { cout << "  Oops!  "; }
public:
	static Strategy* makeObject(string& criteria);
};
class Algorithm1 : public Strategy {
public: ~Algorithm1() {
		DTOR("  ~Algorithm1 ", Lecture);
	}
public:
	void compute() { cout << "  Algorithm1."; }
};
class Algorithm2 : public Strategy {
public: ~Algorithm2() {
		DTOR("  ~Algorithm2 ", Lecture);
	}
public:
	void compute() { cout << "  Algorithm2."; }
};
class Algorithm3 : public Strategy {
public: ~Algorithm3() {
		DTOR("  ~Algorithm3 ", Lecture);
	}
public:
	void compute() { cout << "  Algorithm3."; }
};
// Seam point - add another algorithm.

Strategy* Strategy::makeObject(string& criteria) {
	if(criteria == "Algorithm1")	return new Algorithm1;
	if(criteria == "Algorithm2")	return new Algorithm2;
	if(criteria == "Algorithm3")	return new Algorithm3;
	// Seam point - insert another algorithm.
	return new Strategy; // Base, default, null, exception.
}

void demo(int seqNo) {	// Test variations.
	string criteria[] = { "Algorithm1", "Algorithm2",
						  "Algorithm3", "oops" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Strategy* algorithm = Strategy::makeObject(criteria[i]);
		algorithm->compute();
		delete algorithm;
	}
	cout << endl;
}

} // FM

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
	if(criteria == "BackOfTheEnvelope")	return new Linear;
	if(criteria == "QuickAndDirty")	return new Poly;
	if(criteria == "Exact")		return new Spline;
	// Seam point - insert another criteria.
	return new Spline;	// Default.
}

void demo() {	// Test variations.
	string criteria[] = {"BackOfTheEnvelope","QuickAndDirty","Exact"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		Fit* fit = Fit::makeObject(criteria[i]);
		cout << "  " << fit->est() << "\n";
	}
	cout << endl;
}

} // recognition

namespace practical_issues {

namespace story_1 {	// Single variation & criteria.

void clientCode_A(int criteria) {	// In file A.
	switch(criteria) {
	case 1:	cout << "  Pre-order traversal.\n";		break;
	case 2:	cout << "  In-order traversal.\n";		break;
	case 3:	cout << "  Post-order traversal.\n";	break;
	// Seam point - insert another criteria.
	default: cout << "  Oops!\n";	break;
	}
}

void demo() {
	cout << "  practical_issues::story_1::demo().\n";
	int test[] = { 1, 2, 3, 0 };
	for(size_t i=0; i<COUNT(test); i++) {
		clientCode_A(test[i]);	// traversal
	}
	cout << endl;
}

} // story_1

namespace story_2 {	// Add second variation but with same criteria.

void clientCode_A(int criteria) {	// In file A.
	switch(criteria) {	// See file B.
	case 1:	cout << "  Pre-order traversal";	break;
	case 2:	cout << "  In-order traversal";		break;
	case 3:	cout << "  Post-order traversal";	break;
	// Seam point - insert another criteria.
	default: cout << "  Oops";	break;
	}
}
void clientCode_B(int criteria) {	// In file B.
	switch(criteria) {	// See file A.
	case 1:	cout << " - heap parse.\n";			break;
	case 2:	cout << " - token parse.\n";		break;
	case 3:	cout << " - predictive parse.\n";	break;
	// Seam point - insert another criteria.
	default: cout << " - oops!\n";	break;
	}
}

void demo() {
	cout << "  practical_issues::story_2::demo().\n";
	int test[] = { 1, 2, 3, 0 };
	for(size_t i=0; i<COUNT(test); i++) { // Correlated:
		clientCode_A(test[i]);	// traversal
		clientCode_B(test[i]);	// parse
	}
	cout << endl;
}

} // story_2

namespace story_3 {	// Add third variation also with same criteria.

void clientCode_A(int criteria) {	// In file A.
	switch(criteria) {	// See files B,C.
	case 1:	cout << "  Pre-order traversal";	break;
	case 2:	cout << "  In-order traversal";		break;
	case 3:	cout << "  Post-order traversal";	break;
	// Seam point - insert another criteria.
	default: cout << "  Oops";	break;
	}
}
void clientCode_B(int criteria) {	// In file B.
	switch(criteria) {	// See files A,C.
	case 1:	cout << " - heap parse";		break;
	case 2:	cout << " - token parse";		break;
	case 3:	cout << " - predictive parse";	break;
	// Seam point - insert another criteria.
	default: cout << " - oops";	break;
	}
}
void clientCode_C(int criteria) {	// In file C.
	switch(criteria) {	// See files A,B.
	case 1:	cout << " - preHeap swarm.\n";		break;
	case 2:	cout << " - inToken swarm.\n";		break;
	case 3:	cout << " - postPredict swarm.\n";	break;
	// Seam point - insert another criteria.
	default: cout << " - oops!\n";	break;
	}
}

void demo() {
	cout << "  practical_issues::story_3::demo().\n";
	int test[] = { 1, 2, 3, 0 };
	for(size_t i=0; i<COUNT(test); i++) { // Correlated:
		clientCode_A(test[i]);	// traversal
		clientCode_B(test[i]);	// parse
		clientCode_C(test[i]);	// swarm
	}
	cout << endl;
}

} // story_3

namespace story_4 {	// Extend all three variations.

void clientCode_A(int criteria) {	// In file A.
	switch(criteria) {	// See files B,C.
	case 1:	cout << "  Pre-order traversal";	break;
	case 2:	cout << "  In-order traversal";		break;
	case 3:	cout << "  Post-order traversal";	break;
	// Seam point - insert another criteria.
	case 4:	cout << "  Sample traversal";		break;
	default: cout << "  Oops";	break;
	}
}
void clientCode_B(int criteria) {	// In file B.
	switch(criteria) {	// See files A,C.
	case 1:	cout << " - heap parse";		break;
	case 2:	cout << " - token parse";		break;
	case 3:	cout << " - predictive parse";	break;
	// Seam point - insert another criteria.
	case 4:	cout << " - neural parse";		break;
	default: cout << " - oops";	break;
	}
}
void clientCode_C(int criteria) {	// In file C.
	switch(criteria) {	// See files A,B.
	case 1:	cout << " - preHeap swarm.\n";		break;
	case 2:	cout << " - inToken swarm.\n";		break;
	case 3:	cout << " - postPredict swarm.\n";	break;
	// Seam point - insert another criteria.
	case 4:	cout << " - sampleNeural swarm.\n";	break;
	default: cout << " - oops!\n";	break;
	}
}

void demo() {
	cout << "  practical_issues::story_4::demo().\n";
	int test[] = { 1, 2, 3, 4, 0 };
	for(size_t i=0; i<COUNT(test); i++) { // Correlated:
		clientCode_A(test[i]);	// traversal
		clientCode_B(test[i]);	// parse
		clientCode_C(test[i]);	// swarm
	}
	cout << endl;
}

} // story_4

namespace refactor_2 {	// Refactor with Strategy design pattern.

class Strategy {
public: virtual ~Strategy() {}
public:
	virtual void traverse() { cout << "  Oops"; }
	virtual void parse() { cout << " - oops!\n"; }
public:
	static Strategy* makeObject(int criteria);
};
class Scheme1 : public Strategy {
public:
	void traverse() { cout << "  Pre-order traversal"; }
	void parse() { cout << " - heap parse.\n"; }
};
class Scheme2 : public Strategy {
public:
	void traverse() { cout << "  In-order traversal"; }
	void parse() { cout << " - token parse.\n"; }
};
class Scheme3 : public Strategy {
public:
	void traverse() { cout << "  Post-order traversal"; }
	void parse() { cout << " - predictive parse.\n"; }
};
// Seam point - add another strategy.

Strategy* Strategy::makeObject(int criteria) {
	switch(criteria) {
	case 1:	return new Scheme1;
	case 2:	return new Scheme2;
	case 3:	return new Scheme3;
	// Seam point - insert another criteria.
	default: return new Strategy; }
}

void clientCode_A(Strategy* algorithm) {	// In file A.
	algorithm->traverse();
}
void clientCode_B(Strategy* algorithm) {	// In file B.
	algorithm->parse();
}

void demo() {
	cout << "  practical_issues::refactor_2::demo().\n";
	int criteria[] = { 1, 2, 3, 0 };
	for(size_t i=0; i<COUNT(criteria); i++) {	// Correlated:
		Strategy* scheme = Strategy::makeObject(criteria[i]);
		clientCode_A(scheme);	// traversal
		clientCode_B(scheme);	// parse
		delete scheme;
	}
	cout << endl;
}

} // refactor_2

namespace refactor_3 {	// Refactor with Strategy design pattern.

class Strategy {
public: virtual ~Strategy() {}
public:
	virtual void traverse() { cout << "  Oops"; }
	virtual void parse() { cout << " - oops"; }
	virtual void swarm() { cout << " - oops!\n"; }
public:
	static Strategy* makeObject(int criteria);
};
class Scheme1 : public Strategy {
public:
	void traverse() { cout << "  Pre-order traversal"; }
	void parse() { cout << " - heap parse"; }
	void swarm() { cout << " - preHeap swarm.\n"; }
};
class Scheme2 : public Strategy {
public:
	void traverse() { cout << "  In-order traversal"; }
	void parse() { cout << " - token parse"; }
	void swarm() { cout << " - inToken swarm.\n"; }
};
class Scheme3 : public Strategy {
public:
	void traverse() { cout << "  Post-order traversal"; }
	void parse() { cout << " - predictive parse"; }
	void swarm() { cout << " - postPredict swarm.\n"; }
};
// Seam point - add another strategy.

Strategy* Strategy::makeObject(int criteria) {
	switch(criteria) {
	case 1:	return new Scheme1;
	case 2:	return new Scheme2;
	case 3:	return new Scheme3;
	// Seam point - insert another criteria.
	default: return new Strategy; }
}

void clientCode_A(Strategy* algorithm) {	// In file A.
	algorithm->traverse();
}
void clientCode_B(Strategy* algorithm) {	// In file B.
	algorithm->parse();
}
void clientCode_C(Strategy* algorithm) {	// In file C.
	algorithm->swarm();
}
void demo() {
	cout << "  practical_issues::refactor_3::demo().\n";
	int criteria[] = { 1, 2, 3, 0 };
	for(size_t i=0; i<COUNT(criteria); i++) {	// Correlated:
		Strategy* scheme = Strategy::makeObject(criteria[i]);
		clientCode_A(scheme);	// traversal
		clientCode_B(scheme);	// parse
		clientCode_C(scheme);	// swarm
		delete scheme;
	}
	cout << endl;
}

} // refactor_3

namespace refactor_4 {	// Refactor with Strategy design pattern.

class Strategy {
public: virtual ~Strategy() {}
public:
	virtual void traverse() { cout << "  Oops"; }
	virtual void parse() { cout << " - oops"; }
	virtual void swarm() { cout << " - oops!\n"; }
public:
	static Strategy* makeObject(int criteria);
};
class Scheme1 : public Strategy {
public:
	void traverse() { cout << "  Pre-order traversal"; }
	void parse() { cout << " - heap parse"; }
	void swarm() { cout << " - preHeap swarm.\n"; }
};
class Scheme2 : public Strategy {
public:
	void traverse() { cout << "  In-order traversal"; }
	void parse() { cout << " - token parse"; }
	void swarm() { cout << " - inToken swarm.\n"; }
};
class Scheme3 : public Strategy {
public:
	void traverse() { cout << "  Post-order traversal"; }
	void parse() { cout << " - predictive parse"; }
	void swarm() { cout << " - postPredict swarm.\n"; }
};
// Seam point - add another strategy.
class Scheme4 : public Strategy {
public:
	void traverse() { cout << "  Sample traversal"; }
	void parse() { cout << " - neural parse"; }
	void swarm() { cout << " - sampleNeural swarm.\n"; }
};

Strategy* Strategy::makeObject(int criteria) {
	switch(criteria) {
	case 1:	return new Scheme1;
	case 2:	return new Scheme2;
	case 3:	return new Scheme3;
	// Seam point - insert another criteria.
	case 4:	return new Scheme4;
	default: return new Strategy; }
}

void clientCode_A(Strategy* algorithm) {	// In file A.
	algorithm->traverse();
}
void clientCode_B(Strategy* algorithm) {	// In file B.
	algorithm->parse();
}
void clientCode_C(Strategy* algorithm) {	// In file C.
	algorithm->swarm();
}

void demo() {
	cout << "  practical_issues::refactor_4::demo().\n";
	int criteria[] = { 1, 2, 3, 4, 0 };
	for(size_t i=0; i<COUNT(criteria); i++) {	// Correlated:
		Strategy* scheme = Strategy::makeObject(criteria[i]);
		clientCode_A(scheme);	// traversal
		clientCode_B(scheme);	// parse
		clientCode_C(scheme);	// swarm
		delete scheme;
	}
	cout << endl;
}

} // refactor_4

namespace story_5 {	// Extend all three variations.

void clientCode_A(int criteria) {	// In file A.
	switch(criteria) {	// See files B,C.
	case 1:	cout << "  Pre-order traversal";	break;
	case 2:	cout << "  In-order traversal";		break;
	case 3:	cout << "  Post-order traversal";	break;
	// Seam point - insert another criteria.
	case 4:	cout << "  Sample traversal";		break;
	default: cout << "  Oops";	break;
	}
}
void clientCode_B(int criteria) {	// In file B.
	switch(criteria) {	// See files A,C.
	case 1:	cout << " - heap parse";		break;
	case 2:	cout << " - token parse";		break;
	case 3:	cout << " - predictive parse";	break;
	// Seam point - insert another criteria.
	case 4:	cout << " - neural parse";		break;
	default: cout << " - oops";	break;
	}
}
void clientCode_C(int criteria) {	// In file C.
	switch(criteria) {	// See files A,B.
	case 1:	cout << " - preHeap swarm.\n";		break;
	case 2:	cout << " - inToken swarm.\n";		break;
	case 3:	cout << " - postPredict swarm.\n";	break;
	// Seam point - insert another criteria.
	case 4:	cout << " - sampleNeural swarm.\n";	break;
	default: cout << " - oops!\n";	break;
	}
}
void clientCode_M(int logic) {	// In file M.
	switch(logic) {	// See file N.
	case 7:	cout << "    Seven";	break;
	case 8:	cout << "    Eight";	break;
	case 9:	cout << "    Nine";		break;
	// Seam point - insert another logic.
	default: cout << " - oops!\n";	break;
	}
}
void clientCode_N(int logic) {	// In file N.
	switch(logic) {	// See file M.
	case 7:	cout << " - nip7 maleficent.\n";	break;
	case 8:	cout << " - nip8 maleficent.\n";	break;
	case 9:	cout << " - nip9 maleficent.\n";	break;
	// Seam point - insert another logic.
	default: cout << " - oops!\n";	break;
	}
}

void demo() {
	cout << "  practical_issues::story_5::demo().\n";
	int test[] = { 1, 2, 3, 4, 0 };
	int indi[] = { 7, 8, 9 };
	for(size_t i=0; i<COUNT(test); i++) {	 // Independent.
		clientCode_A(test[i]);	// traversal (correlates:)
		clientCode_B(test[i]);	// parse
		clientCode_C(test[i]);	// swarm
		for(size_t j=0; j<COUNT(indi); j++) {// Independent.
			clientCode_M(indi[j]);	// tuck (correlates:)
			clientCode_N(indi[j]);	// nip
		}
	}
	cout << endl;
}

} // story_5

namespace refactor_5 {	// Refactor with Strategy design pattern.

class Strategy {
public: virtual ~Strategy() {}
public:
	virtual void traverse() { cout << "  Oops"; }
	virtual void parse() { cout << " - oops"; }
	virtual void swarm() { cout << " - oops!\n"; }
public:
	static Strategy* makeObject(int criteria);
};
class Scheme1 : public Strategy {
public:
	void traverse() { cout << "  Pre-order traversal"; }
	void parse() { cout << " - heap parse"; }
	void swarm() { cout << " - preHeap swarm.\n"; }
};
class Scheme2 : public Strategy {
public:
	void traverse() { cout << "  In-order traversal"; }
	void parse() { cout << " - token parse"; }
	void swarm() { cout << " - inToken swarm.\n"; }
};
class Scheme3 : public Strategy {
public:
	void traverse() { cout << "  Post-order traversal"; }
	void parse() { cout << " - predictive parse"; }
	void swarm() { cout << " - postPredict swarm.\n"; }
};
// Seam point - add another strategy.
class Scheme4 : public Strategy {
public:
	void traverse() { cout << "  Sample traversal"; }
	void parse() { cout << " - neural parse"; }
	void swarm() { cout << " - sampleNeural swarm.\n"; }
};

Strategy* Strategy::makeObject(int criteria) {
	switch(criteria) {
	case 1:	return new Scheme1;
	case 2:	return new Scheme2;
	case 3:	return new Scheme3;
	// Seam point - insert another criteria.
	case 4:	return new Scheme4;
	default: return new Strategy; }
}

class Maleficent {	// Strategy pattern.
public: virtual ~Maleficent() {}
public:
	virtual void tuck() { cout << "    Number"; }
	virtual void nip() { cout << " - nipN maleficent.\n"; }
public:
	static Maleficent* makeObject(int logic);
};
class Mal1 : public Maleficent {
public:
	void tuck() { cout << "    Seven"; }
	void nip() { cout << " - nip7 maleficent.\n"; }
};
class Mal2 : public Maleficent {
public:
	void tuck() { cout << "    Eight"; }
	void nip() { cout << " - nip8 maleficent.\n"; }
};
class Mal3 : public Maleficent {
public:
	void tuck() { cout << "    Nine"; }
	void nip() { cout << " - nip9 maleficent.\n"; }
};
// Seam point - add another maleficent strategy.

Maleficent* Maleficent::makeObject(int logic) {
	switch(logic) {
	case 7:	return new Mal1;
	case 8:	return new Mal2;
	case 9:	return new Mal3;
	// Seam point - insert another logic.
	default: return new Maleficent; }
}

void clientCode_A(Strategy* algorithm) {	// In file A.
	algorithm->traverse();
}
void clientCode_B(Strategy* algorithm) {	// In file B.
	algorithm->parse();
}
void clientCode_C(Strategy* algorithm) {	// In file C.
	algorithm->swarm();
}
void clientCode_M(Maleficent* algorithm) {	// In file M.
	algorithm->tuck();
}
void clientCode_N(Maleficent* algorithm) {	// In file N.
	algorithm->nip();
}

void demo() {
	cout << "  practical_issues::refactor_5::demo().\n";
	int criteria[] = { 1, 2, 3, 4, 0 };
	int logic[] = { 7, 8, 9 };
	for(size_t i=0; i<COUNT(criteria); i++) {	// Correlated:
		Strategy* scheme = Strategy::makeObject(criteria[i]);
		clientCode_A(scheme);	// traversal
		clientCode_B(scheme);	// parse
		clientCode_C(scheme);	// swarm
		delete scheme;
		for(size_t j=0; j<COUNT(logic); j++) {	// Independent.
			Maleficent* obj = Maleficent::makeObject(logic[j]);
			clientCode_M(obj);	// tuck (correlates:)
			clientCode_N(obj);	// nip
			delete obj;
		}
	}
	cout << endl;
}

} // refactor_5

} // practical_issues

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
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << strategy::skeleton >>\n";
		skeleton::demo(seqNo);
		skeleton::FM::demo(seqNo);
	}
	virtual void recognition() {
		cout << seqNo << ") << strategy::recognition >>\n";
		recognition::demo();
	}
	virtual void practicalIssues() {
		cout << seqNo << ") << strategy::practical_issues >>\n";
		practical_issues::story_1::demo();
		practical_issues::story_2::demo();
		practical_issues::story_3::demo();
		practical_issues::story_4::demo();
		practical_issues::refactor_2::demo();
		practical_issues::refactor_3::demo();
		practical_issues::refactor_4::demo();
		practical_issues::story_5::demo();
		practical_issues::refactor_5::demo();
	}
};

} // strategy

#endif /* LECTURES_STRATEGY_H_ */

/*
 * adapter.h
 *
 *  Created on: Sep 1, 2015
 *      Author: aldgoff
 *
 * Varies: If the interfaces are varying, use the Adapter pattern.
 *
 * Desc: Adapt interfaces so classes can work together polymorphically.
 *
 * Category: Structural
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Adapter
 *  	http://www.dofactory.com/net/adapter-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter7
 *  	http://sourcemaking.com/design_patterns/adapter
 */

#ifndef SOLUTIONS_ADAPTER_H_
#define SOLUTIONS_ADAPTER_H_

#include "../macros.h"

/* Consider the case where an installed and heavily used class hierarchy
 * for simple shapes needs to be extended by incorporating a commercial
 * class hierarchy containing more complex shapes.
 * The problem is that the two class hierarchies use different API's.
 * The original uses the function "draw()" the other the function "display()".
 * This breaks the polymorphism that kept the client code simple.
 * This causes 4 maintenance headaches:
 *   1) Client code will become more complicated.
 *   2) Changes will be required in existing client code.
 *   3) Likely become littered w/ if statements of the sort "what type are you"?
 *   4) Need for error detection.
 * Solve this problem by writing new classes that 'adapt' the new interface
 * (namespace commercial) to the old (namespace home_grown).
 */

namespace solution {

namespace home_grown {

class ShapeInterfaceDraw {	// Home grown interface class.
public:
	virtual ~ShapeInterfaceDraw() {
		DTOR("  ~ShapeInterfaceDraw\n", Homework);
	}
public:
	virtual void draw()=0;
};
class Point : public ShapeInterfaceDraw {
public:	~Point() {
		DTOR("  ~Point", Homework);
	}
public:
	void draw() { cout << "  Draw point.\n";}
};
class Line : public ShapeInterfaceDraw {
public:	~Line() {
		DTOR("  ~Line", Homework);
	}
public:
	void draw() { cout << "  Draw line.\n"; }
};
class Rect : public ShapeInterfaceDraw {
public:	~Rect() {
		DTOR("  ~Rect", Homework);
	}
public:
	void draw() { cout << "  Draw rectangle.\n"; }
};

} // home_grown

namespace commercial {

class ShapeInterfaceDisplay {	// Commercial interface class.
public:
	virtual ~ShapeInterfaceDisplay() {
		DTOR("  ~ShapeInterfaceDisplay", Homework);
	}
public:
	virtual void display()=0;
};
class Polygon : public ShapeInterfaceDisplay {
public:	~Polygon() {
		DTOR("  ~commercial::Polygon", Homework);
	}
public:
	void display() { cout << "  Display polygon.\n";}
};
class Torus : public ShapeInterfaceDisplay {
public:	~Torus() {
		DTOR("  ~commercial::Torus", Homework);
	}
public:
	void display() { cout << "  Display torus.\n";}
};
class Bezel : public ShapeInterfaceDisplay {
public:	~Bezel() {
		DTOR("  ~commercial::Bezel", Homework);
	}
public:
	void display() { cout << "  Display bezel.\n";}
};

} // commercial

using namespace home_grown;

class Polygon : public ShapeInterfaceDraw {	// New code.
public:	~Polygon() {
		DTOR("  ~Polygon", Homework);
	}
	commercial::Polygon poly;
public:
	void draw() { poly.display(); }
};
class Torus : public ShapeInterfaceDraw {
public:	~Torus() {
		DTOR("  ~Torus", Homework);
	}
	commercial::Torus torus;
public:
	void draw() { torus.display(); }
};
class Bezel : public ShapeInterfaceDraw {
public:	~Bezel() {
		DTOR("  ~Bezel", Homework);
	}
	commercial::Bezel bezel;
public:
	void draw() { bezel.display(); }
};
// Seam point - add another interface.

void clientCode(ShapeInterfaceDraw* shape) {	// Interface adapts to Client.
	shape->draw();
}

void demo(int seqNo) {
	cout << seqNo << ") << adapter::homework::solution::demo() >>\n";
	vector<ShapeInterfaceDraw*> shapes;	// Old client code stays the same.
	shapes.push_back(new Point);
	shapes.push_back(new Line);
	shapes.push_back(new Rect);
	shapes.push_back(new Polygon);		// Just add new code.
	shapes.push_back(new Torus);
	shapes.push_back(new Bezel);

	for(size_t i=0; i<shapes.size(); i++) {	// Client code stays simple because
		clientCode(shapes[i]);				// API's have been converged.
	}
	for(size_t i=0; i<shapes.size(); i++)
		delete shapes[i];
	cout << endl;
}

} // solution


#endif /* SOLUTIONS_ADAPTER_H_ */

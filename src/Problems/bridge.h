/*
 * bridge.h
 *
 *  Created on: Oct 21, 2015
 *      Author: aldgoff
 *
 * Varies: If the abstractions and implementations are varying, use the Bridge pattern.
 *
 * Desc: Decouples an abstraction from its implementations so they can vary independently.
 *
 * Category: Structural
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Bridge
 *  	http://www.dofactory.com/net/bridge-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter10
 *  	http://sourcemaking.com/design_patterns/bridge
 */

#ifndef PROBLEMS_BRIDGE_H_
#define PROBLEMS_BRIDGE_H_

/* Consider a graphics program that must run on multiple platforms.
 * Each platform provides different drawing primitives for lines & arcs.
 * Basic shapes can be constructed from these primitives, such as
 *   rectangles, circles, etc.
 *
 * Use the bridge pattern to write code that avoids the combinatorial explosion
 * of classes and methods which a procedural approach entails.
 * Show how adding another shape or platform requires only a linear amount of
 * additional code.
 *
 * Rectangle, draw 4 lines (upper left corner: x1,y1, lower right corner: x2,y2).
 *	 draw(x1,y1, x2,y1) draw(x2,y1, x2,y2) draw(x2,y2, x1,y2) draw(x1,y2, x1,y1)
 * Circle, draw 4 arcs (90 degrees, clockwise, center is x,y).
 *	 draw(x,y+r, x+r,y) draw(x+r,y, x,y-r) draw(x,y-r, x-r,y) draw(x-r,y, x,y+r)
 * Triangle, draw 3 lines between 3 vertices.
 *   draw(vert1.first,vert1.second, vert2.first,vert2.second)
 *   draw(vert2.first,vert2.second, vert3.first,vert3.second)
 *   draw(vert3.first,vert3.second, vert1.first,vert1.second)
 *
 * In this problem, the Shape hierarchy has already been implemented
 * with Rectangle and Circle for the legacy code.
 * A Triangle and a plotter platform were added for the problem code.
 * Note the quadratic scaling in the source code.
 *
 * Refactor using the Bridge pattern showing how it avoids the quadratic scaling.
 *
 * For extra credit redo using the GoF creational pattern Factory Method
 * to encapsulate the creation of the appropriate derived Shape class.
 */

namespace common_p {

namespace platform_vector {

void drawLine(double x1, double y1, double x2, double y2) {
	cout << "    Draw vector line: ["<<x1<<","<<y1<<"] ["<<x2<<","<<y2<<"]\n";
}
void drawArc(double x1, double y1, double x2, double y2) {
	cout << "    Draw vector arc: ("<<x1<<","<<y1<<") ("<<x2<<","<<y2<<")\n";
}

} // platform_vector

namespace platform_raster {

void draw_line(double x1, double y1, double x2, double y2) {
	cout << "    Draw raster line: ["<<x1<<","<<y1<<"] ["<<x2<<","<<y2<<"]\n";
}
void draw_arc(double x1, double y1, double x2, double y2) {
	cout << "    Draw raster arc: ("<<x1<<","<<y1<<") ("<<x2<<","<<y2<<")\n";;
}

} // platform_raster

namespace platform_plotter {

void draw_a_line(double x1, double y1, double x2, double y2) {
	cout << "    Draw plotter line: ["<<x1<<","<<y1<<"] ["<<x2<<","<<y2<<"]\n";
}
void draw_an_arc(double x1, double y1, double x2, double y2) {
	cout << "    Draw plotter arc: ("<<x1<<","<<y1<<") ("<<x2<<","<<y2<<")\n";;
}

} // platform_plotter

} // common_p

namespace legacy {

using namespace common_p::platform_vector;
using namespace common_p::platform_raster;

enum Render {
	VECTOR,
	RASTER,
	// Seam point - insert another enumerated type.
};

class Shape {
public: virtual ~Shape() {
		DTOR(" ~Shape\n", Homework);
	}
	virtual void draw(Render plat)=0;
};
class Rectangle : public Shape {
	double x1;
	double y1;
	double x2;
	double y2;
public:
	Rectangle(double x1, double y1, double x2, double y2)
		: x1(x1), y1(y1), x2(x2), y2(y2) {
	}
	~Rectangle() {
		DTOR("  ~Rectangle", Homework);
	}
public:
	void draw(Render platform) {
		switch(platform) {
		case VECTOR:
			cout << "  Rectangle.draw(x1,y1, x2,y2)<vector>\n";
			drawLine(x1,y1, x2,y1);
			drawLine(x2,y1, x2,y2);
			drawLine(x2,y2, x1,y2);
			drawLine(x1,y2, x1,y1);
			break;
		case RASTER:
			cout << "  Rectangle.draw(x1,y1, x2,y2)<raster>\n";
			draw_line(x1,y1, x2,y1);
			draw_line(x2,y1, x2,y2);
			draw_line(x2,y2, x1,y2);
			draw_line(x1,y2, x1,y1);
			break;
		// Seam point - insert another render platform.
		default: cout << "  OOPS!\n"; break;
		}
	}
};
class Circle : public Shape {
	double x;
	double y;
	double r;
public:
	Circle(double x, double y, double radius)
		: x(x), y(y), r(radius) {
	}
	~Circle() {
		DTOR("  ~Circle", Homework);
	}
public:
	void draw(Render platform) {
		switch(platform) {
		case VECTOR:
			cout << "  Circle.draw(x,y, radius)<vector>\n";
			drawArc(x,y+r, x+r,y);
			drawArc(x+r,y, x,y-r);
			drawArc(x,y-r, x-r,y);
			drawArc(x-r,y, x,y+r);
			break;
		case RASTER:
			cout << "  Circle.draw(x,y, radius)<raster>\n";
			draw_arc(x,y+r, x+r,y);
			draw_arc(x+r,y, x,y-r);
			draw_arc(x,y-r, x-r,y);
			draw_arc(x-r,y, x,y+r);
			break;
		// Seam point - insert another render platform.
		default: cout << "  OOPS!\n"; break;
		}
	}
};
// Seam point - add another shape.

void clientCode(Shape* shape, Render platform) {
	shape->draw(platform);
}

void demo() {
	Shape*	shapes[] = { new Rectangle(1,1, 2,3),
						 new Circle(4,4, 1.0) };
	Render	platforms[] = { VECTOR, RASTER };

	for(size_t i=0; i<COUNT(shapes); i++) {
		for(size_t j=0; j<COUNT(platforms); j++) {
			clientCode(shapes[i], platforms[j]);
		}
		cout << endl;
	}

	for(size_t i=0; i<COUNT(shapes); i++)	delete shapes[i];
	cout << endl;
}

} // legacy

namespace problem {

using namespace common_p::platform_vector;
using namespace common_p::platform_raster;
using namespace common_p::platform_plotter;

enum Render {
	VECTOR,
	RASTER,
	PLOTTER,
	// Seam point - insert another enumerated type.
};

class Shape {
public: virtual ~Shape() {
		DTOR(" ~Shape\n", Homework);
	}
	virtual void draw(Render plat)=0;
};
class Rectangle : public Shape {
	double x1;
	double y1;
	double x2;
	double y2;
public:
	Rectangle(double x1, double y1, double x2, double y2)
		: x1(x1), y1(y1), x2(x2), y2(y2) {
	}
	~Rectangle() {
		DTOR("  ~Rectangle", Homework);
	}
public:
	void draw(Render platform) {
		switch(platform) {
		case VECTOR:
			cout << "  Rectangle.draw(x1,y1, x2,y2)<vector>\n";
			drawLine(x1,y1, x2,y1);
			drawLine(x2,y1, x2,y2);
			drawLine(x2,y2, x1,y2);
			drawLine(x1,y2, x1,y1);
			break;
		case RASTER:
			cout << "  Rectangle.draw(x1,y1, x2,y2)<raster>\n";
			draw_line(x1,y1, x2,y1);
			draw_line(x2,y1, x2,y2);
			draw_line(x2,y2, x1,y2);
			draw_line(x1,y2, x1,y1);
			break;
		case PLOTTER:
			cout << "  Rectangle.draw(x1,y1, x2,y2)<plotter>\n";
			draw_a_line(x1,y1, x2,y1);
			draw_a_line(x2,y1, x2,y2);
			draw_a_line(x2,y2, x1,y2);
			draw_a_line(x1,y2, x1,y1);
			break;
		// Seam point - insert another render platform.
		default: cout << "  OOPS!\n"; break;
		}
	}
};
class Circle : public Shape {
	double x;
	double y;
	double r;
public:
	Circle(double x, double y, double radius)
		: x(x), y(y), r(radius) {
	}
	~Circle() {
		DTOR("  ~Circle", Homework);
	}
public:
	void draw(Render platform) {
		switch(platform) {
		case VECTOR:
			cout << "  Circle.draw(x,y, radius)<vector>\n";
			drawArc(x,y+r, x+r,y);
			drawArc(x+r,y, x,y-r);
			drawArc(x,y-r, x-r,y);
			drawArc(x-r,y, x,y+r);
			break;
		case RASTER:
			cout << "  Circle.draw(x,y, radius)<raster>\n";
			draw_arc(x,y+r, x+r,y);
			draw_arc(x+r,y, x,y-r);
			draw_arc(x,y-r, x-r,y);
			draw_arc(x-r,y, x,y+r);
			break;
		case PLOTTER:
			cout << "  Circle.draw(x,y, radius)<plotter>\n";
			draw_an_arc(x,y+r, x+r,y);
			draw_an_arc(x+r,y, x,y-r);
			draw_an_arc(x,y-r, x-r,y);
			draw_an_arc(x-r,y, x,y+r);
			break;
		// Seam point - insert another render platform.
		default: cout << "  OOPS!\n"; break;
		}
	}
};
class Triangle : public Shape {
	pair<double,double>	vert1;
	pair<double,double>	vert2;
	pair<double,double>	vert3;
public:
	Triangle(pair<double,double> vert1,
			 pair<double,double> vert2,
			 pair<double,double> vert3)
		: vert1(vert1), vert2(vert2), vert3(vert3) {
	}
	~Triangle() {
		DTOR("  ~Triangle", Homework);
	}
public:
	void draw(Render platform) {
		switch(platform) {
		case VECTOR:
			cout << "  Triangle.draw(vert1, vert2, vert3)<vector>\n";
			drawLine(vert1.first,vert1.second, vert2.first,vert2.second);
			drawLine(vert2.first,vert2.second, vert3.first,vert3.second);
			drawLine(vert3.first,vert3.second, vert1.first,vert1.second);
			break;
		case RASTER:
			cout << "  Triangle.draw(vert1, vert2, vert3)<raster>\n";
			draw_line(vert1.first,vert1.second, vert2.first,vert2.second);
			draw_line(vert2.first,vert2.second, vert3.first,vert3.second);
			draw_line(vert3.first,vert3.second, vert1.first,vert1.second);
			break;
		case PLOTTER:
			cout << "  Triangle.draw(vert1, vert2, vert3)<plotter>\n";
			draw_a_line(vert1.first,vert1.second, vert2.first,vert2.second);
			draw_a_line(vert2.first,vert2.second, vert3.first,vert3.second);
			draw_a_line(vert3.first,vert3.second, vert1.first,vert1.second);
			break;
		// Seam point - insert another render platform.
		default: cout << "  OOPS!\n"; break;
		}
	}
};
// Seam point - add another shape.

void clientCode(Shape* shape, Render platform) {
	shape->draw(platform);
}

void demo() {
	Shape*	shapes[] = { new Rectangle(1,1, 2,3),
						 new Circle(4,4, 1.0),
						 new Triangle(make_pair(1.0,1.0),
									  make_pair(2.0,2.0),
									  make_pair(2.0,1.0)) };
	Render	platforms[] = { VECTOR, RASTER, PLOTTER };

	for(size_t i=0; i<COUNT(shapes); i++) {
		for(size_t j=0; j<COUNT(platforms); j++) {
			clientCode(shapes[i], platforms[j]);
		}
		cout << endl;
	}

	for(size_t i=0; i<COUNT(shapes); i++)	delete shapes[i];
	cout << endl;
}

} // problem

#endif /* PROBLEMS_BRIDGE_H_ */

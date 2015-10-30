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

#ifndef SOLUTIONS_BRIDGE_H_
#define SOLUTIONS_BRIDGE_H_

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

namespace common_s {

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

} // common_s

namespace solution {

using namespace common_s::platform_vector;
using namespace common_s::platform_raster;
using namespace common_s::platform_plotter;

enum Render {
	VECTOR,
	RASTER,
	PLOTTER,
	// Seam point - insert another render platform.
};

namespace simple_FM_solution {

class Platform { // Implementation class in Bridge pattern.
public: virtual ~Platform() {
		DTOR("~Platform\n", Homework);
	}
public:
	virtual void line(double x1, double y1, double x2, double y2)=0;
	virtual void arc(double x1, double y1, double x2, double y2)=0;
public:
	static Platform* makeObject(const Render renderPlatform);
};
class Vector : public Platform {
public: ~Vector() {
		DTOR("~Vector ", Homework);
	}
public:
	void line(double x1, double y1, double x2, double y2) {
		drawLine(x1,y1, x2,y1);
	}
	void arc(double x1, double y1, double x2, double y2) {
		drawArc(x1,y1, x2,y1);
	}
};
class Raster : public Platform {
public: ~Raster() {
		DTOR("~Raster ", Homework);
	}
public:
	void line(double x1, double y1, double x2, double y2) {
		draw_line(x1,y1, x2,y1);
	}
	void arc(double x1, double y1, double x2, double y2) {
		draw_arc(x1,y1, x2,y1);
	}
};
class Plotter : public Platform {
public: ~Plotter() {
		DTOR("~Plotter ", Homework);
	}
public:
	void line(double x1, double y1, double x2, double y2) {
		draw_a_line(x1,y1, x2,y1);
	}
	void arc(double x1, double y1, double x2, double y2) {
		draw_an_arc(x1,y1, x2,y1);
	}
};
// Seam point - add another platform.
Platform* Platform::makeObject(const Render renderPlatform) {
	switch(renderPlatform) {
	case VECTOR:	return new Vector;	break;
	case RASTER:	return new Raster;	break;
	case PLOTTER:	return new Plotter;	break;
	// Seam point - insert another Render platform.
	default:		throw "Oops!";	break;
	}
}

class Shape { // Abstraction class in Bridge pattern.
protected:
	Platform* platform;
public:	virtual ~Shape() {
		DTOR("~Shape\n", Homework);
	}
	void setPlatform(Platform* platform) {this->platform = platform;}
	virtual void draw()=0;
};
class Rectangle : public Shape {
	double x1;
	double y1;
	double x2;
	double y2;
public:
	Rectangle(double x1, double y1, double x2, double y2)
		: x1(x1), y1(y1), x2(x2), y2(y2)
	{}
	~Rectangle() {
		DTOR("~Rectangle ", Homework);
	}
public:
	void draw() {
		cout << "  Rectangle.draw(x1,y1, x2,y2)<platform>\n";
		platform->line(x1,y1, x2,y1);
		platform->line(x2,y1, x2,y2);
		platform->line(x2,y2, x1,y2);
		platform->line(x1,y2, x1,y1);
	}
};
class Circle : public Shape {
	double x;
	double y;
	double r;
public:
	Circle(double x, double y, double radius)
		: x(x), y(y), r(radius)
	{}
	~Circle() {
		DTOR("~Circle ", Homework);
	}
public:
	void draw() {
		cout << "  Circle.draw(x,y, radius)<platform>\n";
		platform->arc(x,y+r, x+r,y);
		platform->arc(x+r,y, x,y-r);
		platform->arc(x,y-r, x-r,y);
		platform->arc(x-r,y, x,y+r);
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
		: vert1(vert1), vert2(vert2), vert3(vert3)
	{}
	~Triangle() {
		DTOR("~Triangle ", Homework);
	}
public:
	void draw() {
		cout << "  Triangle.draw(vert1, vert2, vert3)<platform>\n";
		platform->line(vert1.first,vert1.second, vert2.first,vert2.second);
		platform->line(vert2.first,vert2.second, vert3.first,vert3.second);
		platform->line(vert3.first,vert3.second, vert1.first,vert1.second);
	}
};
// Seam point - add another shape.

void clientCode(Shape* shape) {
	shape->draw();
}

void demo() {
	Shape*	shapes[] = { new Rectangle(1,1, 2,3),
						 new Circle(4,4, 1.0),
						 new Triangle(make_pair(1.0,1.0),
									  make_pair(2.0,2.0),
									  make_pair(2.0,1.0)) };
	const Render plats[] = { VECTOR, RASTER, PLOTTER };
	vector<Platform*> plat;
	for(size_t j=0; j<COUNT(plats); j++)
		plat.push_back(Platform::makeObject(plats[j]));
	for(size_t i=0; i<COUNT(shapes); i++) {
		for(size_t j=0; j<COUNT(plats); j++) {
			shapes[i]->setPlatform(plat[j]);
			clientCode(shapes[i]);
		}
		cout << endl;
	}

	for(size_t i=0; i<COUNT(shapes); i++)	delete shapes[i];
	for(size_t j=0; j<COUNT(plats); j++)	delete plat[j];
	cout << endl;
}

} // simple_FM_solution

namespace Gof_FM_solution {

string gb_dtor_hack;	// Just so all dtor instrumentation
string gd_dtor_hack;	// appears after data drawing.

enum Primative {
	RECTANGLE,
	CIRCLE,
	TRIANGLE,
	// Seam point - insert another shape primative.
};

class Platform { // Implementation class in Bridge pattern.
public: virtual ~Platform() {
		DTOR("~Platform\n", Homework);
	}
public:
	virtual void line(double x1, double y1, double x2, double y2)=0;
	virtual void arc(double x1, double y1, double x2, double y2)=0;
public:
	static Platform* makeObject(const Render renderPlatform);
};
class Vector : public Platform {
public: ~Vector() {
		DTOR("~Vector ", Homework);
	}
public:
	void line(double x1, double y1, double x2, double y2) {
		drawLine(x1,y1, x2,y1);
	}
	void arc(double x1, double y1, double x2, double y2) {
		drawArc(x1,y1, x2,y1);
	}
};
class Raster : public Platform {
public: ~Raster() {
		DTOR("~Raster ", Homework);
	}
public:
	void line(double x1, double y1, double x2, double y2) {
		draw_line(x1,y1, x2,y1);
	}
	void arc(double x1, double y1, double x2, double y2) {
		draw_arc(x1,y1, x2,y1);
	}
};
class Plotter : public Platform {
public: ~Plotter() {
		DTOR("~Plotter ", Homework);
	}
public:
	void line(double x1, double y1, double x2, double y2) {
		draw_a_line(x1,y1, x2,y1);
	}
	void arc(double x1, double y1, double x2, double y2) {
		draw_an_arc(x1,y1, x2,y1);
	}
};
// Seam point - add another platform.
Platform* Platform::makeObject(const Render renderPlatform) { // (DFM).
	switch(renderPlatform) {
	case VECTOR:	return new Vector;	break;
	case RASTER:	return new Raster;	break;
	case PLOTTER:	return new Plotter;	break;
	// Seam point - insert another Render platform.
	default:		throw "Oops!";	break;
	}
}

class Shape { // Abstraction class in Bridge pattern.
protected:
	Platform* platform;
public:	virtual ~Shape() {
		DTOR("~Shape\n", Homework);
	}
	void setPlatform(Platform* platform) {this->platform = platform;}
	virtual void draw()=0;
public:
	static Shape* makeObject(Platform* platform, // (EFM).
		const Primative primative, const float* params);
};
class Rectangle : public Shape {
	double x1;
	double y1;
	double x2;
	double y2;
public:
	Rectangle(double x1, double y1, double x2, double y2)
		: x1(x1), y1(y1), x2(x2), y2(y2)
	{}
	~Rectangle() {
		DTOR("  ~Rectangle ", Homework);
	}
public:
	void draw() {
		cout << "  Rectangle.draw(x1,y1, x2,y2)<platform>\n";
		platform->line(x1,y1, x2,y1);
		platform->line(x2,y1, x2,y2);
		platform->line(x2,y2, x1,y2);
		platform->line(x1,y2, x1,y1);
	}
};
class Circle : public Shape {
	double x;
	double y;
	double r;
public:
	Circle(double x, double y, double radius)
		: x(x), y(y), r(radius)
	{}
	~Circle() {
		DTOR("  ~Circle ", Homework);
	}
public:
	void draw() {
		cout << "  Circle.draw(x,y, radius)<platform>\n";
		platform->arc(x,y+r, x+r,y);
		platform->arc(x+r,y, x,y-r);
		platform->arc(x,y-r, x-r,y);
		platform->arc(x-r,y, x,y+r);
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
		: vert1(vert1), vert2(vert2), vert3(vert3)
	{}
	~Triangle() {
		DTOR("  ~Triangle ", Homework);
	}
public:
	void draw() {
		cout << "  Triangle.draw(vert1, vert2, vert3)<platform>\n";
		platform->line(vert1.first,vert1.second, vert2.first,vert2.second);
		platform->line(vert2.first,vert2.second, vert3.first,vert3.second);
		platform->line(vert3.first,vert3.second, vert1.first,vert1.second);
	}
};
// Seam point - add another shape.

class ShapeFactory { // Gof Factory Method (CFM).
public:	virtual ~ShapeFactory() {
		if(Homework & flags) gb_dtor_hack = "~ShapeFactory";
	}
public:
	virtual Shape* create(Platform* platform, const float* params)=0;
};
class RectangleFactory : public ShapeFactory {
public:	virtual ~RectangleFactory() {
		if(Homework & flags) gd_dtor_hack = "  ~RectangleFactory ";
	}
public:
	Shape* create(Platform* platform, const float* params) {
		float x1 = params[0];
		float y1 = params[1];
		float x2 = params[2];
		float y2 = params[3];
		Shape* shape = new Rectangle(x1,y1, x2,y2);
		shape->setPlatform(platform);
		return shape;
	}
};
class CircleFactory : public ShapeFactory {
public:	virtual ~CircleFactory() {
		if(Homework & flags) gd_dtor_hack = "  ~CircleFactory ";
	}
public:
	Shape* create(Platform* platform, const float* params) {
		float x = params[0];
		float y = params[1];
		float r = params[2];
		Shape* shape = new Circle(x,y, r);
		shape->setPlatform(platform);
		return shape;
	}
};
class TriangleFactory : public ShapeFactory {
public:	virtual ~TriangleFactory() {
		if(Homework & flags) gd_dtor_hack = "  ~TriangleFactory ";
	}
public:
	Shape* create(Platform* platform, const float* params) {
		Shape* shape = new Triangle(
			make_pair(params[0], params[1]),
			make_pair(params[2], params[3]),
			make_pair(params[4], params[5])
		);
		shape->setPlatform(platform);
		return shape;
	}
};
// Seam point - add another factory.

Shape* Shape::makeObject(Platform* platform, // (EFM)
						 const Primative primative,
						 const float* params)
{
	switch(primative) {
	case RECTANGLE:
		return RectangleFactory().create(platform, params); // CFM.
		break;
	case CIRCLE:
		return CircleFactory().create(platform, params); // CFM.
		break;
	case TRIANGLE:
		return TriangleFactory().create(platform, params); // CFM.
		break;
		// Seam point - insert another decision.
	default:
		throw "Oops!";
		break;
	}
}

void clientCode(Shape* shape) {
	shape->draw();
}

void demo() { // Bridge, DFM, CFM, & EFM.
	const Primative primatives[] = { RECTANGLE, CIRCLE, TRIANGLE };
	float rectParams[] = { 1.0,1.0, 2.0,3.0 };
	float circleParams[] = { 4.0,4.0, 1.0 };
	float triangleParams[] = { 1.0,1.0,
							   2.0,2.0,
							   2.0,1.0 };
	float* params[] = { rectParams, circleParams, triangleParams };

	const Render plats[] = { VECTOR, RASTER, PLOTTER };
	vector<Platform*> plat;
	for(size_t j=0; j<COUNT(plats); j++)
		plat.push_back(Platform::makeObject(plats[j])); // DFM.

	for(size_t i=0; i<COUNT(primatives); i++) {
		for(size_t j=0; j<COUNT(plats); j++) {
			Shape* shape = Shape::makeObject(	// EFM & CFM.
					plat[j], primatives[i], params[i]);
			clientCode(shape);
			cout << gd_dtor_hack << gb_dtor_hack;
			gd_dtor_hack = gb_dtor_hack = "";
			delete shape;
		}
		cout << endl;
	}

	for(size_t j=0; j<COUNT(plats); j++)	delete plat[j];
	cout << endl;
}

} // Gof_FM_solution

void demo() {
	simple_FM_solution::demo();
	Gof_FM_solution::demo();
}

} // solution

#endif /* SOLUTIONS_BRIDGE_H_ */

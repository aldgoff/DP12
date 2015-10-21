/*
 * bridge.h   dpTemplate dp_template DPTemplate derived
 *
 *  Created on: May 23, 2015
 *      Author: aldgoff
 *
 * Varies: If the abstractions and implementations are varying, use the Bridge pattern.
 *
 * Desc:
 *
 * Category: whatever
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Bridge
 *  	http://www.dofactory.com/net/bridge-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter10
 *  	http://sourcemaking.com/design_patterns/bridge
 */

#ifndef LECTURES_BRIDGE_H_
#define LECTURES_BRIDGE_H_

#include "../DP.h"

namespace bridge {	// Command line: bridge [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

/* The client code needs different thread schedulers, such as
 *   preemptive, roundRobin, timeSliced, prioritized, etc.
 * but the implementations for each of these is platform dependent, that is
 *   Unix, Windows, OS10, iOS, JVM, etc.
 */

/* Initial requirements:
 *   Two kinds of thread schedulers:
 *     preemptive and round robin.
 *   Two kinds of platforms:
 *     Unix and Windows.
 */

/* New requirements:
 *   Add two kinds of thread schedulers:
 *     time sliced and prioritized.
 *   Add three kinds of platforms:
 *     OS10, iOS and JVM.
 */

namespace legacy {

enum Scheduler {
	PRE_EMPTIVE,
	ROUND_ROBIN,
	// Seam point - insert another enumerated type.
};
enum Platform {
	UNIX,
	WINDOWS,
	// Seam point - insert another enumerated type.
};

class Preemptive {
private:
	void runOnUnix() {
		cout << "  Preemptive thread scheduler running on Unix platform.\n";
	}
	void runOnWindows() {
		cout << "  Preemptive thread scheduler running on Windows platform.\n";
	}
	// Seam point - insert another method.
public:
	void run(Platform plat) {
		switch(plat) {
		case UNIX: runOnUnix();	break;
		case WINDOWS: runOnWindows(); break;
		// Seam point - insert another case.
		default: cout << "  OOPS!\n"; break;
		}
	}
};

class RoundRobin {
private:
	void runOnUnix() {
		cout << "  RoundRobin thread scheduler running on Unix platform.\n";
	}
	void runOnWindows() {
		cout << "  RoundRobin thread scheduler running on Windows platform.\n";
	}
	// Seam point - insert another method.
public:
	void run(Platform plat) {
		switch(plat) {
		case UNIX: runOnUnix();	break;
		case WINDOWS: runOnWindows(); break;
		// Seam point - insert another case.
		default: cout << "  OOPS!\n"; break;
		}
	}
};

void clientCode(Scheduler type, Platform plat) {
	switch(type) {
	case PRE_EMPTIVE:
		{
			Preemptive scheduler;
			scheduler.run(plat);
		}
		break;
	case ROUND_ROBIN:
		{
			RoundRobin scheduler;
			scheduler.run(plat);
		}
		break;
	// Seam point - insert another case.
	default:
		cout << "  OOPS!\n";
		break;
	}
}

void demo() {
	Scheduler types[] = { PRE_EMPTIVE, ROUND_ROBIN };
	Platform plats[] = { UNIX, WINDOWS };
	for(size_t i=0; i<COUNT(types); i++) {
		for(size_t j=0; j<COUNT(plats); j++) {
			clientCode(types[i], plats[j]);
		}
		cout << endl;
	}
}

}

namespace problem {

enum Scheduler {
	PRE_EMPTIVE,
	ROUND_ROBIN,
	TIME_SLICED,
	PRIORITIZED,
	// Seam point - insert another enumerated type.
};
enum OS {
	UNIX,
	WINDOWS,
	OS_10,
	IOS,
	JAVA,
	// Seam point - insert another enumerated type.
};

class Preemptive {
private:
	void runOnUnix() {
		cout << "  Preemptive thread scheduler running on Unix platform.\n";
	}
	void runOnWindows() {
		cout << "  Preemptive thread scheduler running on Windows platform.\n";
	}
	void runOnOS10() {
		cout << "  Preemptive thread scheduler running on OS10 platform.\n";
	}
	void runOnIOS() {
		cout << "  Preemptive thread scheduler running on iOS platform.\n";
	}
	void runOnJVM() {
		cout << "  Preemptive thread scheduler running on JVM platform.\n";
	}
	// Seam point - insert another method.
public:
	void run(OS plat) {
		switch(plat) {
		case UNIX: runOnUnix();	break;
		case WINDOWS: runOnWindows(); break;
		case OS_10: runOnOS10();	break;
		case IOS: runOnIOS();	break;
		case JAVA: runOnJVM();	break;
		// Seam point - insert another case.
		default: cout << "  OOPS!\n"; break;
		}
	}
};

class RoundRobin {
private:
	void runOnUnix() {
		cout << "  RoundRobin thread scheduler running on Unix platform.\n";
	}
	void runOnWindows() {
		cout << "  RoundRobin thread scheduler running on Windows platform.\n";
	}
	void runOnOS10() {
		cout << "  RoundRobin thread scheduler running on OS10 platform.\n";
	}
	void runOnIOS() {
		cout << "  RoundRobin thread scheduler running on iOS platform.\n";
	}
	void runOnJVM() {
		cout << "  RoundRobin thread scheduler running on JVM platform.\n";
	}
	// Seam point - insert another method.
public:
	void run(OS plat) {
		switch(plat) {
		case UNIX: runOnUnix();	break;
		case WINDOWS: runOnWindows(); break;
		case OS_10: runOnOS10();	break;
		case IOS: runOnIOS();	break;
		case JAVA: runOnJVM();	break;
		// Seam point - insert another case.
		default: cout << "  OOPS!\n"; break;
		}
	}
};

class TimeSliced {
private:
	void runOnUnix() {
		cout << "  TimeSliced thread scheduler running on Unix platform.\n";
	}
	void runOnWindows() {
		cout << "  TimeSliced thread scheduler running on Windows platform.\n";
	}
	void runOnOS10() {
		cout << "  TimeSliced thread scheduler running on OS10 platform.\n";
	}
	void runOnIOS() {
		cout << "  TimeSliced thread scheduler running on iOS platform.\n";
	}
	void runOnJVM() {
		cout << "  TimeSliced thread scheduler running on JVM platform.\n";
	}
	// Seam point - insert another method.
public:
	void run(OS plat) {
		switch(plat) {
		case UNIX: runOnUnix();	break;
		case WINDOWS: runOnWindows(); break;
		case OS_10: runOnOS10();	break;
		case IOS: runOnIOS();	break;
		case JAVA: runOnJVM();	break;
		// Seam point - insert another case.
		default: cout << "  OOPS!\n"; break;
		}
	}
};

class Prioritized {
private:
	void runOnUnix() {
		cout << "  Prioritized thread scheduler running on Unix platform.\n";
	}
	void runOnWindows() {
		cout << "  Prioritized thread scheduler running on Windows platform.\n";
	}
	void runOnOS10() {
		cout << "  Prioritized thread scheduler running on OS10 platform.\n";
	}
	void runOnIOS() {
		cout << "  Prioritized thread scheduler running on iOS platform.\n";
	}
	void runOnJVM() {
		cout << "  Prioritized thread scheduler running on JVM platform.\n";
	}
	// Seam point - insert another method.
public:
	void run(OS plat) {
		switch(plat) {
		case UNIX: runOnUnix();	break;
		case WINDOWS: runOnWindows(); break;
		case OS_10: runOnOS10();	break;
		case IOS: runOnIOS();	break;
		case JAVA: runOnJVM();	break;
		// Seam point - insert another case.
		default: cout << "  OOPS!\n"; break;
		}
	}
};

void clientCode(Scheduler type, OS plat) {
	switch(type) {
	case PRE_EMPTIVE:
		{
			Preemptive scheduler;
			scheduler.run(plat);
		}
		break;
	case ROUND_ROBIN:
		{
			RoundRobin scheduler;
			scheduler.run(plat);
		}
		break;
	case TIME_SLICED:
		{
			TimeSliced scheduler;
			scheduler.run(plat);
		}
		break;
	case PRIORITIZED:
		{
			Prioritized scheduler;
			scheduler.run(plat);
		}
		break;
		// Seam point - insert another case.
	default:
		cout << "  OOPS!\n";
		break;
	}
}

void demo() {
	Scheduler types[] = { PRE_EMPTIVE, ROUND_ROBIN,
						  TIME_SLICED, PRIORITIZED };
	OS plats[] = { UNIX, WINDOWS, OS_10, IOS, JAVA };
	for(size_t i=0; i<COUNT(types); i++) {
		for(size_t j=0; j<COUNT(plats); j++) {
			clientCode(types[i], plats[j]);
		}
		cout << endl;
	}
}

}

namespace solution {

enum Scheduler {
	PRE_EMPTIVE,
	ROUND_ROBIN,
	TIME_SLICED,
	PRIORITIZED,
	// Seam point - insert another enumerated type.
};
enum OS {
	UNIX,
	WINDOWS,
	OS_10,
	IOS,
	JAVA,
	// Seam point - insert another enumerated type.
};

class Platform { // Implementation class in Bridge pattern.
public: virtual ~Platform() {
	DTOR("~Platform ", Lecture);
}
public:
	virtual void implement() {
		cout << "  Derived class needs to override this.\n";
	}
public:
	static Platform* makeObject(OS platform);
};
class Unix : public Platform {
public:	virtual ~Unix() {
	DTOR("~Unix ", Lecture);
}
public:
	void implement() { cout << " Unix platform.\n"; }
};
class Windows : public Platform {
public:	virtual ~Windows() {
	DTOR("~Windows ", Lecture);
}
	void implement() { cout << " Windows platform.\n"; }
};
class OS10 : public Platform {
public:	virtual ~OS10() {
	DTOR("~OS10 ", Lecture);
}
	void implement() { cout << " OS10 platform.\n"; }
};
class iOS : public Platform {
public:	virtual ~iOS() {
	DTOR("~iOS ", Lecture);
}
	void implement() { cout << " iOS platform.\n"; }
};
class JVM : public Platform {
public:	virtual ~JVM() {
	DTOR("~JVM ", Lecture);
}
	void implement() { cout << " JVM platform.\n"; }
};
// Seam point - add another Platform Implementation.
Platform* Platform::makeObject(OS platform) {
	switch(platform) {
	case UNIX:		return new Unix;	break;
	case WINDOWS:	return new Windows;	break;
	case OS_10:		return new OS10;	break;
	case IOS:		return new iOS;		break;
	case JAVA:		return new JVM;		break;
	default:		return new Platform;	break;
	}
}

class ThreadScheduler {	// Abstraction class in Bridge pattern.
public:
	Platform* platform;
public:
	ThreadScheduler(Platform* platform) : platform(platform) {}
	virtual ~ThreadScheduler() { if(platform) delete platform;
		DTOR("~ThreadScheduler\n", Lecture);
	}
public:
	virtual void run() { platform->implement(); }
public:
	static ThreadScheduler*
		makeObject(Scheduler scheduler, Platform* platform);
};
class Preemptive : public ThreadScheduler {
public:
	Preemptive(Platform* platform) : ThreadScheduler(platform) {}
	virtual ~Preemptive() {
		DTOR("  ~Preemptive ", Lecture);
	}
public:
	void run() {
		cout << "  Preemptive thread scheduler running on";
		platform->implement(); }
};
class RoundRobin : public ThreadScheduler {
public:
	RoundRobin(Platform* platform) : ThreadScheduler(platform) {}
	virtual ~RoundRobin() {
		DTOR("  ~RoundRobin ", Lecture);
	}
public:
	virtual void run() {
		cout << "  RoundRobin thread scheduler running on";
		platform->implement(); }
};
class TimeSliced : public ThreadScheduler {
public:
	TimeSliced(Platform* platform) : ThreadScheduler(platform) {}
	virtual ~TimeSliced() {
		DTOR("  ~TimeSliced ", Lecture);
	}
public:
	virtual void run() {
		cout << "  TimeSliced thread scheduler running on";
		platform->implement(); }
};
class Prioritized : public ThreadScheduler {
public:
	Prioritized(Platform* platform) : ThreadScheduler(platform) {}
	virtual ~Prioritized() {
		DTOR("  ~Prioritized ", Lecture);
	}
public:
	virtual void run() {
		cout << "  Prioritized thread scheduler running on";
		platform->implement(); }
};
// Seam point - add another ThreadScheduler Abstraction.
ThreadScheduler* ThreadScheduler::makeObject(
		Scheduler scheduler, Platform* platform
	) {
	switch(scheduler) {
	case PRE_EMPTIVE: return new Preemptive(platform);	break;
	case ROUND_ROBIN: return new RoundRobin(platform);	break;
	case TIME_SLICED: return new TimeSliced(platform);	break;
	case PRIORITIZED: return new Prioritized(platform);	break;
	default:	return new ThreadScheduler(platform);	break;
	}
}

void clientCode(ThreadScheduler* scheduler) {
	scheduler->run();
}

void demo() {
	Scheduler types[] = { PRE_EMPTIVE, ROUND_ROBIN,
						  TIME_SLICED, PRIORITIZED };
	OS plats[] = { UNIX, WINDOWS, OS_10, IOS, JAVA };
	ThreadScheduler* destroy[COUNT(types)][COUNT(plats)] = {0};
	for(size_t i=0; i<COUNT(types); i++) {
		for(size_t j=0; j<COUNT(plats); j++) {
			Platform* platform = Platform::makeObject(plats[j]);
			ThreadScheduler* scheduler =
				ThreadScheduler::makeObject(types[i], platform);
			clientCode(scheduler);
			destroy[i][j] = scheduler;
		}
		cout << endl;
	}
	for(size_t i=0; i<COUNT(types); i++) {
		for(size_t j=0; j<COUNT(plats); j++)
			delete destroy[i][j];
		cout << endl;
	}
}

}

} // lecture

namespace homework {

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
 *
 * In this problem, the Shape hierarchy has already been implemented.
 * Create a Platform hierarchy and add the plotter platform.
 *
 * For extra credit redo using the GoF creational pattern Factory Method.
 */

namespace common {

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

} // common

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << bridge::homework::legacy::demo() >>\n";
}

} // legacy

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << bridge::homework::problem::demo() >>\n";
}

} // problem

namespace solution {

using namespace common::platform_vector;
using namespace common::platform_raster;
using namespace common::platform_plotter;

namespace simple_FM_solution {

class Platform {
public: virtual ~Platform() {
		DTOR("~Platform\n", Homework);
	}
public:
	virtual void line(double x1, double y1, double x2, double y2)=0;
	virtual void arc(double x1, double y1, double x2, double y2)=0;
public:
	static Platform* makeObject(const string& criteria);
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
Platform* Platform::makeObject(const string& criteria) {
	if(criteria == "Vector")	return new Vector;
	if(criteria == "Raster")	return new Raster;
	if(criteria == "Plotter")	return new Plotter;
	// Seam point - insert another criteria.
	throw "Oops!";
}

class Shape {
protected:
	Platform* platform;
public:	virtual ~Shape() {
		DTOR("~Shape\n", Homework);
	}
	void setPlatform(Platform* platform) { this->platform = platform; }
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
// Seam point - add another shape.

void clientCode(Shape* shape) {
	shape->draw();
}

void demo() {
	Shape*		shapes[] = { new Rectangle(1,1, 2,3), new Circle(4,4, 1.0) };
	const string plats[] = { "Vector", "Raster", "Plotter" };
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

class Platform { // Implementation class in Bridge pattern.
public: virtual ~Platform() {
		DTOR("~Platform\n", Homework);
	}
public:
	virtual void line(double x1, double y1, double x2, double y2)=0;
	virtual void arc(double x1, double y1, double x2, double y2)=0;
public:
	static Platform* makeObject(const string& criteria);
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
Platform* Platform::makeObject(const string& criteria) { // (DFM).
	if(criteria == "Vector")	return new Vector;
	if(criteria == "Raster")	return new Raster;
	if(criteria == "Plotter")	return new Plotter;
	// Seam point - insert another criteria.
	throw "Oops!";
}

class Shape { // Abstraction class in Bridge pattern.
protected:
	Platform* platform;
public:	virtual ~Shape() {
		DTOR("~Shape\n", Homework);
	}
public:
	void setPlatform(Platform* platform) {
		this->platform = platform;
	}
public:
	virtual void draw()=0;
public:
	static Shape* makeObject(Platform* platform, // (EFM).
							 const string& shape,
							 const float* params);
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
// Seam point - add another shape.

class ShapeFactory { // Gof Factory Method (CFM).
public:	virtual ~ShapeFactory() {
		DTOR("~ShapeFactory\n", Homework);
	}
public:
	virtual Shape* create(Platform* platform, const float* params)=0;
};
class RectangleFactory : public ShapeFactory {
public:	virtual ~RectangleFactory() {
		DTOR("  ~RectangleFactory ", Homework);
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
		DTOR("  ~CircleFactory ", Homework);
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
// Seam point - add another factory.

Shape* Shape::makeObject(Platform* platform, // (EFM)
						 const string& shape,
						 const float* params)
{
	if(shape == "Rectangle")
		return RectangleFactory().create(platform, params); // CFM.
	if(shape == "Circle")
		return CircleFactory().create(platform, params); // CFM.
	// Seam point - insert another decision.

	throw "Oops!";
}

void clientCode(Shape* shape) {
	shape->draw();
}

void demo() { // Bridge, DFM, CFM, & EFM.
	const string plats[] = { "Vector", "Raster", "Plotter" };
	vector<Platform*> plat;
	for(size_t j=0; j<COUNT(plats); j++)
		plat.push_back(Platform::makeObject(plats[j])); // DFM.

	const string shapes[] = { "Rectangle", "Circle" };
	float rectParams[] = { 1.0,1.0, 2.0,3.0 };
	float circleParams[] = { 4.0,4.0, 1.0 };
	float* params[] = { rectParams, circleParams };

	for(size_t i=0; i<COUNT(shapes); i++) {
		for(size_t j=0; j<COUNT(plats); j++) {
			Shape* shape = Shape::makeObject(	// EFM & CFM.
					plat[j], shapes[i], params[i]);
			clientCode(shape);
			delete shape;
			cout << endl;
		}
	}

	for(size_t j=0; j<COUNT(plats); j++)	delete plat[j];
	cout << endl;
}

} // Gof_FM_solution

void demo() {
//	simple_FM_solution::demo();
	Gof_FM_solution::demo();
}

} // solution

} // homework

namespace skeleton {

class Platform { // If the implementations are varying...
public: virtual ~Platform() {
		DTOR("~Platform ", Lecture);
	}
public:
	virtual string implement() { return "Oops"; }
public:
	static Platform* makeObject(string& crit);
};
class Platform1 : public Platform {
public: ~Platform1() {
		DTOR(" ~Platform1 ", Lecture);
	}
public:
	string implement() { return "Platform1"; }
};
class Platform2 : public Platform {
public: ~Platform2() {
		DTOR(" ~Platform2 ", Lecture);
	}
public:
	string implement() { return "Platform2"; }
};
class Platform3 : public Platform {
public: ~Platform3() {
		DTOR(" ~Platform3 ", Lecture);
	}
public:
	string implement() { return "Platform3"; }
};
// Seam point - add another implementation.
Platform* Platform::makeObject(string& crit) {
	if(crit == "Platform1")	return new Platform1;
	if(crit == "Platform2")	return new Platform2;
	if(crit == "Platform3")	return new Platform3;
	// Seam point - insert another criteria.
	return new Platform; // Base.
}

class Abstraction { // If the abstractions are varying...
protected:
	Platform* platform;
public:
	Abstraction(Platform* platform=0) : platform(platform) {}
	virtual ~Abstraction() { delete platform;
		DTOR("~Abstraction\n", Lecture);
	}
public:
	virtual string execute() { return "Oops"; }
public:
	static Abstraction* makeObject(string& crit, Platform* plat);
};
class Abstract1 : public Abstraction {
public: ~Abstract1() {
		DTOR("  ~Abstract1", Lecture);
	}
public:
	Abstract1(Platform* platform) : Abstraction(platform) {}
public:
	string execute() {
		return "Abstract1 via " + platform->implement();
	}
};
class Abstract2 : public Abstraction {
public: ~Abstract2() {
		DTOR("  ~Abstract2", Lecture);
	}
public:
	Abstract2(Platform* platform) : Abstraction(platform) {}
public:
	string execute() {
		return "Abstract2 via " + platform->implement();
	}
};
class Abstract3 : public Abstraction {
public: ~Abstract3() {
		DTOR("  ~Abstract3", Lecture);
	}
public:
	Abstract3(Platform* platform) : Abstraction(platform) {}
public:
	string execute() {
		return "Abstract3 via " + platform->implement();
	}
};
// Seam point - add another abstraction.
Abstraction*
Abstraction::makeObject(string& crit, Platform* plat) {
	if(crit == "Abstract1")	return new Abstract1(plat);
	if(crit == "Abstract2")	return new Abstract2(plat);
	if(crit == "Abstract3")	return new Abstract3(plat);
	// Seam point - insert another criteria.
	return new Abstraction; // Base.
}

void demo() {	// Test variations.
	string critA[] = {"Abstract1","Abstract2","Abstract3","oops" };
	string critP[] = {"Platform1","Platform2","Platform3" };
	for(size_t i=0; i<COUNT(critA); i++) {
		Abstraction* abstracts[COUNT(critP)];
		for(size_t j=0; j<COUNT(critP); j++) {
			Platform* platform = Platform::makeObject(critP[j]);
			abstracts[j] = Abstraction::makeObject(critA[i], platform);
			cout << "  " << abstracts[j]->execute() << ".\n";
		}
		for(size_t j=0; j<COUNT(critP); j++) delete abstracts[j];
	}
	cout << endl;
}

} // skeleton

namespace recognition {

class Display { // If the display tech is varying...
public: virtual ~Display() {}
	virtual string render() { return "Oops"; }
	static Display* makeObject(string& crit);
};
class Raster : public Display {
	public: string render() { return "Raster"; } };
class Vector : public Display {
	public: string render() { return "Vector"; } };
class Hologram : public Display {
	public: string render() { return "Hologram";}};
// Seam point - add another display.
Display* Display::makeObject(string& crit) {
	if(crit == "Raster")	return new Raster;
	if(crit == "Vector")	return new Vector;
	if(crit == "Hologram")	return new Hologram;
	// Seam point - insert another criteria.
	return new Display;	// Base.
}

class Shape { // If the shapes are varying...
protected: Display* dis;
public: Shape(Display* dis=0) : dis(dis) {}
	virtual ~Shape() {}
	virtual string draw() { return "Oops"; }
	static Shape* makeObject(string& crit, Display* plat);
};
class Duck : public Shape {
public: Duck(Display* dis) : Shape(dis) {}
	string draw() { return "Draw duck via " + dis->render(); }
};
class Hero : public Shape {
public: Hero(Display* dis) : Shape(dis) {}
	string draw() { return "Draw hero via " + dis->render(); }
};
class Dino : public Shape {
public: Dino(Display* dis) : Shape(dis) {}
	string draw() { return "Draw dino via " + dis->render(); }
};
// Seam point - add another shape.
Shape* Shape::makeObject(string& crit, Display* plat) {
	if(crit == "Duck")	return new Duck(plat);
	if(crit == "Hero")	return new Hero(plat);
	if(crit == "Dino")	return new Dino(plat);
	// Seam point - insert another criteria.
	return new Shape;	// Base.
}

void demo() {	// Test variations.
	string image[] = {"Duck","Hero","Dino","oops"};
	string render[] = {"Raster","Vector","Hologram","oops"};
	for(size_t i=0; i<COUNT(image); i++) {
		for(size_t j=0; j<COUNT(render); j++) {
			Display* dis = Display::makeObject(render[j]);
			Shape* shape = Shape::makeObject(image[i], dis);
			cout << "  " << shape->draw() << ".\n";
			delete dis; delete shape;
		}
	}
	cout << endl;
}

} // recognition

namespace refactoring {

namespace bad {

void clientCode(int criteria) {
	switch(criteria) {
	case 0:	{
		} break;
	case 1:	{
		} break;
	case 2:	{
		} break;
	default: cout << "  Oops!\n"; break;
	}
}

void demo() {
	cout << "  refactoring::bad::demo().\n";
	int test[] = { 0, 1, 2, 3 };
	for(size_t i=0; i<COUNT(test); i++) {
		clientCode(test[i]);
	}
	cout << endl;
}

} // bad

namespace good {

class DesignPattern {
public: virtual ~DesignPattern() {}
public:
	virtual void method() { cout << "  Oops!\n"; }
public:
	static DesignPattern* makeObject(const string& criteria);
};
class Derived1 : public DesignPattern {
public:
	void method() {}
};
class Derived2 : public DesignPattern {
public:
	void method() {}
};
class Derived3 : public DesignPattern {
public:
	void method() {}
};
// Seam point - add another class.

DesignPattern* DesignPattern::makeObject(const string& criteria) {
	if(		criteria == "Derived1")	return new Derived1;
	else if(criteria == "Derived2")	return new Derived2;
	else if(criteria == "Derived3")	return new Derived3;
	// Seam point - insert another criteria.
	else {
		return new DesignPattern;
	}
}

void clientCode(DesignPattern* type) {
	type->method();
}

void demo() {
	cout << "  refactoring::good::demo().\n";
	string criteria[] = {"Derived1","Derived2","Derived3","oops"};
	for(size_t i=0; i<COUNT(criteria); i++) {
		DesignPattern* type = DesignPattern::makeObject(criteria[i]);
		clientCode(type);
		delete type;
	}
	cout << endl;
}

} // good

} // refactoring

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "bridge") {}
	virtual ~Observer() { DTOR("~BridgeObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << bridge::lecture::legacy >>\n";
		lecture::legacy::demo();
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << bridge::lecture::problem >>\n";
		lecture::problem::demo();
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << bridge::lecture::solution >>\n";
		lecture::solution::demo();
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << bridge::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << bridge::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << bridge::homework::solution >>\n";
		homework::solution::demo();
	}
	virtual void skeleton() {
		cout << seqNo << ") << bridge::skeleton >>\n";
		skeleton::demo();
	}
	virtual void recognition() {
		cout << seqNo << ") << bridge::recognition >>\n";
		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << bridge::refactoring >>\n";
		refactoring::bad::demo();
		refactoring::good::demo();
	}
};

} // bridge

#endif /* LECTURES_BRIDGE_H_ */

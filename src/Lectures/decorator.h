/*
 * decorator.h
 *
 *  Created on: Aug 4, 2015
 *      Author: aldgoff
 *
 * Varies: If the options are varying, use the Decorator pattern.
 *
 * Desc: Dynamically attach additional responsibilities to an object.
 *
 * Category: Structural
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Decorator
 *  	http://www.dofactory.com/net/decorator-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter17
 *  	http://sourcemaking.com/design_patterns/decorator
 */

#ifndef LECTURES_DECORATOR_H_
#define LECTURES_DECORATOR_H_

#include "../DP.h"

namespace decorator {	// Command line: decorator [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

class Window {
public: virtual ~Window() {}
public:
	virtual void draw() { cout << "  Draw window\n"; }
};
class WindowWithBorder : public Window {
public:
	void draw() { cout << "  Draw window with border\n"; }
};
class ScrollingHWindow : public Window {
public:
	void draw() { cout << "  Draw window with H scroll bar\n"; }
};
class ScrollingVWindow : public Window {
public:
	void draw() { cout << "  Draw window with V scroll bar\n"; }
};
class ScrollingHVWindow : public Window {
public:
	void draw() { cout << "  Draw window with scroll bars\n"; }
};
class ScrollingHWindowWithBorder : public Window {
public:
	void draw() { cout << "  Draw window with border H scroll bar\n"; }
};
class ScrollingVWindowWithBorder : public Window {
public:
	void draw() { cout << "  Draw window with border V scroll bar\n"; }
};
class ScrollingHVWindowWithBorder : public Window {
public:
	void draw() { cout << "  Draw window with border and scroll bars\n";}
};
// Seam points - add another window decorator to each class.
// !! Exponential scaling (2, 4, 8, 16).
// !! Can't adjust at run time.
// !! Client pays for what they don't use.

void demo(int seqNo) {
	Window().draw();
	WindowWithBorder().draw();
	ScrollingHWindow().draw();
	ScrollingVWindow().draw();
	ScrollingHVWindow().draw();
	ScrollingHWindowWithBorder().draw();
	ScrollingVWindowWithBorder().draw();
	ScrollingHVWindowWithBorder().draw();
	cout << endl;
}

} // legacy

namespace problem {

class Window {
public: ~Window() {
		DTOR("  ~Window\n", Lecture);
	}
public:
	void drawWindow() { cout << "  Display Window.\n"; }
};
class Border {
public: ~Border() {
		DTOR("  ~Border\n", Lecture);
	}
public:
	void drawBorder() { cout << "  Display Border.\n"; }
};
class VScrollBar {
public: ~VScrollBar() {
		DTOR("  ~VScrollBar\n", Lecture);
	}
public:
	void drawVSBar() { cout << "  Display VScrollBar.\n"; }
};
class HScrollBar {
public: ~HScrollBar() {
		DTOR("  ~HScrollBar\n", Lecture);
	}
public:
	void drawHSBar() { cout << "  Display HScrollBar.\n"; }
};
class Jitter3D {
public: ~Jitter3D() {
		DTOR("  ~Jitter3D\n", Lecture);
	}
public:
	void draw3DJit() { cout << "  Display Jitter3D.\n"; }
};
// Seam point - add another window decorator.

class NeedInThisApp // Multiple inheritance, bad scaling.
	  : public Window, public Border,
		public VScrollBar, public Jitter3D {
public: ~NeedInThisApp() {
		DTOR("  ~NeedInThisApp\n", Lecture);
	}
};
// Avoids exponential scaling only for small apps.
// Still can't set at run time.

class AggregateWindow {	// Composition.
	Window		window;
	Border		border;
	VScrollBar	vScrollBar;
	HScrollBar	hScrollBar;
	Jitter3D	jitter3D;
	// Seam point - insert another window decorator.
public: ~AggregateWindow() {
		DTOR("  ~AggregateWindow\n", Lecture);
	}
public:
	void drawWindow(bool b, bool vs, bool hs, bool j3d
			/*, Seam point in the interface!*/ ) {
		cout << "  Display Aggregate Window.\n";
		window.drawWindow();
		if(b)	border.drawBorder();
		if(vs)	vScrollBar.drawVSBar();
		if(hs)	hScrollBar.drawHSBar();
		if(j3d)	jitter3D.draw3DJit();
		// Seam point - insert another window decorator.
	}
};
// !! Client pays for things not used
// (at least there's less of it).

void demo(int seqNo) {
	NeedInThisApp* myWindow = new NeedInThisApp;
	myWindow->drawWindow();
	myWindow->drawBorder();
	myWindow->drawVSBar();
	myWindow->draw3DJit();
	delete myWindow;
	cout << endl;

	AggregateWindow* agWindow = new AggregateWindow;
	agWindow->drawWindow(true, true, false, true);
	delete agWindow;
	cout << endl;
}

} // problem

namespace solution {

class View { // Component class in Decorator design pattern.
public: virtual ~View() {
		DTOR("~View\n", Lecture);
	}
	virtual void display()=0;
};
class Window : public View { // Base object to be decorated.
public:	~Window() {
		DTOR("  ~Window ", Lecture);
	}
	void display() { cout << "  Display Window.\n"; }
};
class WinDec : public View { // Decorator class: Decorator DP.
protected:
	View* decorator;
public:
	WinDec(View* decorator) : decorator(decorator) {}
	~WinDec() {  delete decorator;
		DTOR("  ~WinDec ", Lecture);
	}
	void display() {}
public:
	static View* makeObject(View* decoratr, string& criteria);
};
class Border : public WinDec {
public:
	Border(View* decorator) : WinDec(decorator) {}
	~Border() {
		DTOR("  ~Border ", Lecture);
	}
	void display() { decorator->display();
		cout << "  Display Border.\n";
	}
};
class VScrollBar : public WinDec {
public:
	VScrollBar(View* decorator) : WinDec(decorator) {}
	~VScrollBar() {
		DTOR("  ~VScrollBar ", Lecture);
	}
	void display() { decorator->display();
		cout << "  Display VScrollBar.\n";
	}
};
class HScrollBar : public WinDec {
public:
	HScrollBar(View* decorator) : WinDec(decorator) {}
	~HScrollBar() {
		DTOR("  ~HScrollBar ", Lecture);
	}
	void display() { decorator->display();
		cout << "  Display HScrollBar.\n";
	}
};
class Jitter3D : public WinDec {
public:
	Jitter3D(View* decorator) : WinDec(decorator) {}
	~Jitter3D() {
		DTOR("  ~Jitter3D ", Lecture);
	}
	void display() { decorator->display();
		cout << "  Display Jitter3D.\n";
	}
};
// Seam point - insert another window decorator.

View* WinDec::makeObject(View* decoratr, string& criteria) {
	if(criteria == "Border")	 return new Border(decoratr);
	if(criteria == "VScrollBar") return new VScrollBar(decoratr);
	if(criteria == "HScrollBar") return new HScrollBar(decoratr);
	if(criteria == "Jitter3D")	 return new Jitter3D(decoratr);
	// Seam point - add another criteria.
	return decoratr;
}

void demo(int seqNo) {
	string criteria[] = { "Border", "VScrollBar",
		/*"HScrollBar",*/ "Jitter3D", "oops" };
	View* window = new Window;
	for(size_t i=0; i<COUNT(criteria); i++) {
		window = WinDec::makeObject(window, criteria[i]);
	}
	window->display();
	delete window;
	cout << endl;
}

} // solution

} // lecture

namespace homework {

#include "../Problems/decorator.h"

#include "../Solutions/decorator.h"

} // homework

namespace skeleton {

class ABC {
public: virtual ~ABC() {
		DTOR("~ABC\n", Lecture);
	}
public:
	virtual void behavior()=0;
};
class Object : public ABC {	// Base object to be decorated.
public:	~Object() {
		DTOR("  ~Object ", Lecture);
	}
	void behavior() { cout << "  Object"; }
};
class Decorator : public ABC { // If options are varying...
protected:
	ABC* decorator;
public:
	Decorator(ABC* decorator) : decorator(decorator) {}
	~Decorator() { delete decorator;
		DTOR("  ~Decorator ", Lecture);
	}
	void behavior() {}
public:
	static ABC* makeObject(ABC* decorator, string& criteria);
};
class Option1 : public Decorator {
public:
	Option1(ABC* decorator) : Decorator(decorator) {}
	~Option1() {
		DTOR("  ~Option1", Lecture);
	}
public:
	void behavior() { decorator->behavior(); cout << " Opt1";}
};
class Option2 : public Decorator {
public:
	Option2(ABC* decorator) : Decorator(decorator) {}
	~Option2() {
		DTOR("  ~Option2", Lecture);
	}
public:
	void behavior() { decorator->behavior(); cout << " Opt2";}
};
class Option3 : public Decorator {
public:
	Option3(ABC* decorator) : Decorator(decorator) {}
	~Option3() {
		DTOR("  ~Option3", Lecture);
	}
public:
	void behavior() { decorator->behavior(); cout << " Opt3";}
};
// Seam point - add another option.

ABC* Decorator::makeObject(ABC* decorator, string& criteria){
	if(criteria == "Option1") return new Option1(decorator);
	if(criteria == "Option2") return new Option2(decorator);
	if(criteria == "Option3") return new Option3(decorator);
	return decorator;	// Return current decorator.
}

void demo(int seqNo) {	// Test variations.
	string criteria[] = { "Option1", /*"Option2",*/
						  "Option3", "oops" };
	ABC* thing = new Object;
	for(size_t i=0; i<COUNT(criteria); i++) {
		thing = Decorator::makeObject(thing, criteria[i]);
	}
	thing->behavior();	cout << "\n";
	delete thing;
	cout << endl;
}

} // skeleton

namespace recognition {

class Vehicle {
public: virtual ~Vehicle() {}
public:
	virtual void features()=0;
};
class Car : public Vehicle {
public:
	void features() { cout << "  Car"; }
};
class Accessory : public Vehicle {
protected:
	Vehicle* acc;
public:
	Accessory(Vehicle* addOn) : acc(addOn) {}
	~Accessory() { delete acc; }
public:
	void features() {}
public:
	static Vehicle* create(Vehicle* car, string& criteria);
};
class Audio : public Accessory {
public:
	Audio(Vehicle* soundSystem) : Accessory(soundSystem) {}
public:
	void features() { acc->features(); cout<<" audio";}
};
class SunRoof : public Accessory {
public:
	SunRoof(Vehicle* roof) : Accessory(roof) {}
public:
	void features() { acc->features(); cout<<" sun-roof";}
};
class MagWheels : public Accessory {
public:
	MagWheels(Vehicle* wheels) : Accessory(wheels) {}
public:
	void features() { acc->features(); cout<<" mag-wheels.";}
};
// Seam point - add another accessory.

Vehicle* Accessory::create(Vehicle* car, string& criteria) {
	if(criteria == "Music")	return new Audio(car);
	if(criteria == "View")	return new SunRoof(car);
	if(criteria == "Hip")	return new MagWheels(car);
	// Seam point - insert another criteria.
	return car;	// Do nothing.
}

void demo() {	// Test variations.
	string criteria[] = { "Music", "View", "Hip" };
	Vehicle* car = new Car;
	for(size_t i=0; i<COUNT(criteria); i++)
		car = Accessory::create(car, criteria[i]);
	car->features();	cout << "\n";
	cout << endl;
}

} // recognition

namespace refactoring {

/* Decorate a UI
 *   Add help
 *   Add template
 *   Add demo
 */

namespace bad {

class UI {
public: void action() { cout << "  UI\n"; }
};
class Help : public UI {
public: void action() { cout << "  Help"; UI::action(); }
};
class Template : public UI {
public: void action() { cout << "  Template"; UI::action(); }
};
class Demo : public UI {
public: void action() { cout << "  Demo"; UI::action(); }
};
// Seam point - add another single UI enhancement.
class HelpTemplate : public Template {
public: void action() { cout << "  Help"; Template::action(); }
};
class HelpDemo : public Demo {
public: void action() { cout << "  Help"; Demo::action(); }
};
class TemplateDemo : public Demo {
public: void action() { cout << "  Template"; Demo::action(); }
};
// Seam point - add more double UI enhancements.
class HelpTemplateDemo : public TemplateDemo {
public: void action() { cout << "  Help"; TemplateDemo::action(); }
};
// Seam point - add more triple UI enhancements.
// Seam point - add the first quad UI enhancements.

void clientCode(int criteria, int addSecond, int addThird) {
	switch(criteria) {
	case 0:	{ UI ui; ui.action(); } break;
	case 1:	{	// Help.
		switch(addSecond) {
		case 0:	{ Help help;
			help.action();
			} break;
		case 2:	{
			switch(addThird) {
			case 0:	{ HelpTemplate helpTemplate;
				helpTemplate.action();
				} break;
			case 3:	{ HelpTemplateDemo helpTemplateDemo;
				helpTemplateDemo.action();
				} break;
			}
			} break;
		case 3:	{ HelpDemo helpDemo;
			helpDemo.action();
			} break;
		}
		} break;
	case 2:	{	// Template.
		switch(addSecond) {
		case 0:	{ Template Templatte;
			Templatte.action();
			} break;
		case 3:	{ TemplateDemo templateDemo;
			templateDemo.action();
			} break;
		}
		} break;
	case 3:	{ Demo demo; demo.action(); } break;
	}
}

struct Indexes {
	int i;
	int j;
	int k;
};

Indexes perm[] = {
	{ 0, 0, 0},
	{ 1, 0, 0},
	{ 1, 2, 0},
	{ 1, 2, 3},
	{ 1, 3, 0},
	{ 2, 0, 0},
	{ 2, 3, 0},
	{ 3, 0, 0},
};

void demo() {
	cout << "  refactoring::bad::demo().\n";
	for(size_t n=0; n<COUNT(perm); n++) {
		clientCode(perm[n].i,perm[n].j,perm[n].k);
	}
};

} // bad

namespace good {

using namespace bad;

class UI {
public: virtual ~UI() {}
public:
	virtual void action()=0;
};
class UIBase : public UI {
public:
	void action() { cout << "  UI\n"; }
};
class Enhance : public UI  {
protected:
	UI* decorator;
public:
	Enhance(UI* decorator) : decorator(decorator) {}
	~Enhance() { delete decorator; }
public:
	void action() { cout << "  Oops!\n"; }
public:
	static UI* addDecorator(UI* ui, string& criteria);
};
class Help : public Enhance {
public:
	Help(UI* decorator) : Enhance(decorator) {}
public:
	void action() { cout << "  Help"; decorator->action();}
};
class Template : public Enhance {
public:
	Template(UI* decorator) : Enhance(decorator) {}
public:
	void action() {cout<<"  Template";decorator->action();}
};
class Demo : public Enhance {
public:
	Demo(UI* decorator) : Enhance(decorator) {}
public:
	void action() { cout << "  Demo"; decorator->action();}
};
// Seam point - add another user interface enhancement.

UI* Enhance::addDecorator(UI* ui, string& criteria) {
	if(		criteria == "Help")		return new Help(ui);
	else if(criteria == "Template")	return new Template(ui);
	else if(criteria == "Demo")		return new Demo(ui);
	// Seam point - insert another criteria.
	else { return ui; }	// Do nothing.
}

void demo() {	// Spin through all the permutations.
	cout << "  refactoring::good::demo().\n";
	for(size_t n=0; n<COUNT(perm); n++) {
		vector<string>	dec;
		if(perm[n].i==3||perm[n].j==3||perm[n].k==3) dec.push_back("Demo");
		if(perm[n].i==2||perm[n].j==2)	dec.push_back("Template");
		if(perm[n].i==1)	dec.push_back("Help");
		UI* ui = new UIBase;
		for(size_t i=0; i<dec.size(); i++)
			ui = Enhance::addDecorator(ui, dec[i]);
		ui->action();
		delete ui;
	}
	cout << endl;
}

} // good

} // refactoring

class Observer : public observer::DPObserver {
public:
	Observer(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "decorator") {}
	virtual ~Observer() { DTOR("~DecoratorObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << decorator::lecture::legacy::demo() >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << decorator::lecture::problem::demo( >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << decorator::lecture::solution::demo() >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << decorator::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << decorator::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << decorator::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
	virtual void skeleton() {
		cout << seqNo << ") << decorator::skeleton >>\n";
		skeleton::demo(seqNo);
	}
	virtual void recognition() {
		cout << seqNo << ") << decorator::recognition >>\n";
		recognition::demo();
	}
	virtual void refactoring() {
		cout << seqNo << ") << decorator::refactoring >>\n";
		refactoring::bad::demo();	cout << endl;
		refactoring::good::demo();
	}
};

} // decorator

#endif /* LECTURES_DECORATOR_H_ */

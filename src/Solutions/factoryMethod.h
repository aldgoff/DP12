/*
 * factoryMethod.h
 *
 *  Created on: Sep 3, 2015
 *      Author: aldgoff
  *
 * Varies: If the classes are varying, use the Factory Method pattern.
 *
 * Desc: Provide a method to create derived classes w/o coupling them to the client.
 *
 * Category: Creational
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Factory
 *  	http://www.dofactory.com/net/factoryMethod-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter7
 *  	http://sourcemaking.com/design_patterns/factoryMethod
 */

#ifndef SOLUTIONS_FACTORYMETHOD_H_
#define SOLUTIONS_FACTORYMETHOD_H_

namespace solution {

/* Problem: sending encrypted video streams.
 * What varies
 *   1) Display Types (DisplayPort,HDMI,MIPI,Widi) (HEVC)
 *   2) Crypto Protocols (PVP, ID1, RSE) (RDX)
 *   3) Resolutions ([640,480], [1920,1080], [3840,2160])
 *   4) Framerates (48, 50, 60, 75, 120)
 * Model as
 *   1) Classes
 *   2) Classes
 *   3) Pair of integers
 *   4) Integer
 * Use factory method to reduce coupling & isolate change.
 * Don't need to model the variations in resolution and
 * framerates, use 1920,1080 & 60 fps per the legacy
 * and problem. Do model the new display format (HEVC)
 * and the new crypto protocol (RDX).
 */

int framerate = 0;

int res[] = {0, 0};

class Display {
public: virtual ~Display() {
		DTOR(" ~Display\n", Homework);
	}
public:
	virtual string format() { return "display-format()"; }
public:
	static Display* makeObject(const string& criteria);
};
class DisplayPort : public Display {
public: virtual ~DisplayPort() {
		DTOR(" ~DisplayPort", Homework);
	}
public:
	virtual string format() { return "DisplayPort()"; }
};
class HDMI : public Display {
public: virtual ~HDMI() {
		DTOR(" ~HDMI", Homework);
	}
public:
	virtual string format() { return "HDMI()"; }
};
class MIPI : public Display {
public: virtual ~MIPI() {
		DTOR(" ~MIPI", Homework);
	}
public:
	virtual string format() { return "MIPI()"; }
};
class Widi : public Display {
public: virtual ~Widi() {
		DTOR(" ~Widi", Homework);
	}
public:
	virtual string format() { return "Widi()"; }
};
class HEVC : public Display {
public: virtual ~HEVC() {
		DTOR(" ~HEVC", Homework);
	}
public:
	virtual string format() { return "HEVC()"; }
};
// Seam point - add another factory.

Display* Display::makeObject(const string& criteria) {
	if(criteria == "DisplayPort")	return new DisplayPort;
	else if(criteria == "HDMI")	return new HDMI;
	else if(criteria == "MIPI")	return new MIPI;
	else if(criteria == "Widi")	return new Widi;
	else if(criteria == "HEVC")	return new HEVC;
	// Seam point - insert another criteria.
	return new Display;
}

class Crypto {
public: virtual ~Crypto() {
		DTOR(" ~Crypto;", Homework);
	}
public:
	virtual string protocol() {return"crypto-protocol()";}
public:
	static Crypto* makeObject(const string& criteria);
};
class PVP : public Crypto {
public: virtual ~PVP() {
		DTOR(" ~PVP", Homework);
	}
public:
	virtual string protocol() { return "PVP()"; }
};
class ID1 : public Crypto {
public: virtual ~ID1() {
		DTOR(" ~ID1", Homework);
	}
public:
	virtual string protocol() { return "ID1()"; }
};
class RSA : public Crypto {
public: virtual ~RSA() {
		DTOR(" ~RSA", Homework);
	}
public:
	virtual string protocol() { return "RSA()"; }
};
class RDX : public Crypto {
public: virtual ~RDX() {
		DTOR(" ~RDX", Homework);
	}
public:
	virtual string protocol() { return "RDX()"; }
};
// Seam point - add another factory.

Crypto* Crypto::makeObject(const string& criteria) {
	if(		criteria == "PVP")	return new PVP;
	else if(criteria == "ID1")	return new ID1;
	else if(criteria == "RSA")	return new RSA;
	else if(criteria == "RDX")	return new RDX;
	// Seam point - insert another criteria.
	return new Crypto;
}

void clientCode(int fr, int*res, Display*display, Crypto*crypto){
	cout << "  Display " << display->format();
	cout << " at " << fr << " frames/sec";
	cout << " with [" << res[0] << ", " << res[1] << "]";
	cout << " via " << crypto->protocol() << ".\n";
}

void demo(int seqNo) {
	cout<<seqNo<<") << factory_method::homework::solution::demo() >>\n";
	framerate = 60;
	res[0] = 1920, res[1] = 1080;
	string displays[] = { "DisplayPort", "HDMI", "MIPI", "Widi", "HEVC"};
	string cryptos[] = { "PVP", "ID1", "RSA", "RDX"};
	for(size_t i=0; i<COUNT(displays); i++) {
		Display* display = Display::makeObject(displays[i]);
		vector<Crypto*> crypto;
		for(size_t j=0; j<COUNT(cryptos); j++) {
			crypto.push_back(Crypto::makeObject(cryptos[j]));
			clientCode(framerate, res, display, crypto[j]);
		}
		for(size_t j=0; j<COUNT(cryptos); j++)
			delete crypto[j];
		delete display;
	}
}

} // solution

#endif /* SOLUTIONS_FACTORYMETHOD_H_ */

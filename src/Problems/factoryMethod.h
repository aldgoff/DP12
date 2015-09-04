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

#ifndef PROBLEMS_FACTORYMETHOD_H_
#define PROBLEMS_FACTORYMETHOD_H_

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

namespace legacy {

string DisplayPort() { return "DisplayPort()"; }
string HDMI() { return "HDMI()"; }

string PVP() { return "PVP()"; }

void clientCode(int fr, int* res, string display, string crypto) {
	if(display == "DisplayPort") {
		cout << "  Display " << DisplayPort();
		}
	else if(display == "HDMI") {
		cout << "  Display " << HDMI();
		}
	// Seam point - insert another display format.
	else {
		cout << "  Display " << "display format()";
	}

	cout << " at " << fr << " frames/sec";
	cout << " with [" << res[0] << ", " << res[1] << "]";
	// Hidden seam point - insert another crypto protocol.
	cout << " via " << PVP() << ".\n";
}

void demo(int seqNo) {
	cout<<seqNo<<") << factory_method::homework::legacy::demo() >>\n";
	framerate = 60;
	res[0] = 1920, res[1] = 1080;
	string displays[] = { "DisplayPort", "HDMI"};
	for(size_t i=0; i<COUNT(displays); i++) {
		clientCode(framerate, res, displays[i], "PVP");
	}
}
} // legacy

namespace problem {

string DisplayPort() { return "DisplayPort()"; }
string HDMI() { return "HDMI()"; }
string MIPI() { return "MIPI()"; }
string Widi() { return "Widi()"; }

string PVP() { return "PVP()"; }
string ID1() { return "ID1()"; }
string RSA() { return "RSA()"; }

void clientCode(int fr, int* res, string display, string crypto) {
	if(display == "DisplayPort") {
		cout << "  Display " << DisplayPort();
		}
	else if(display == "HDMI") {
		cout << "  Display " << HDMI();
		}
	else if(display == "MIPI") {
		cout << "  Display " << MIPI();
		}
	else if(display == "Widi") {
		cout << "  Display " << Widi();
		}
	// Seam point - insert another display format.
	else {
		cout << "  Display " << "display format()";
	}

	cout << " at " << fr << " frames/sec";
	cout << " with [" << res[0] << ", " << res[1] << "]";

	if(crypto == "PVP") {
		cout << " via " << PVP() << ".\n";
		}
	else if(crypto == "ID1") {
		cout << " via " << ID1() << ".\n";
		}
	else if(crypto == "RSA") {
		cout << " via " << RSA() << ".\n";
		}
	// Seam point - insert another crypto protocol.
	else {
		cout << " via " << "crypto protocol()" << ".\n";
	}
}

void demo(int seqNo) {
	cout<<seqNo<<") << factory_method::homework::problem::demo() >>\n";
	framerate = 60;
	res[0] = 1920, res[1] = 1080;
	string displays[] = { "DisplayPort", "HDMI", "MIPI", "Widi"};
	string cryptos[] = { "PVP", "ID1", "RSA"};
	for(size_t i=0; i<COUNT(displays); i++) {
		for(size_t j=0; j<COUNT(cryptos); j++) {
			clientCode(framerate, res, displays[i], cryptos[j]);
		}
	}
}
} // problem

#endif /* PROBLEMS_FACTORYMETHOD_H_ */

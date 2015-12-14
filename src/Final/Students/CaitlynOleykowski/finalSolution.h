/*
 * finalDesign.h
 *
 *  Created on: <MMDDCCYY>
 *      Author: <student name>
 */

#ifndef FINAL_FINAL_DESIGN_H_
#define FINAL_FINAL_DESIGN_H_

namespace final_design {

const bool on = true;	// Useful macro to turn dtor instrumentation on/off.
#define DTORF(x) if(on) { cout << x; }

namespace legacy_classes {	// Can't change these.

int cycleTime_sec(string metal, int volume_cc) {
	int heatCapacityAdjustment = 0;

	if(		metal == "aluminum")	heatCapacityAdjustment = -2;
	else if(metal == "steel")		heatCapacityAdjustment = +5;

	return 20 + heatCapacityAdjustment + 0.1*volume_cc;
}

const int setupAvg_min = 118;
const int teardownAvg_min = 54;
const float rejectRateLow_pcnt = 0.2;
const float rejectRateHigh_pcnt = 1.1;

class CleanABS {
public: ~CleanABS() { DTORF("~legacy_classes::CleanABS "); }
	void clean() {
		cout << "    Clean ABS mold: soak in alcohol, rinse with water, dry.\n";
	}
};

class CleanPolys {
public: ~CleanPolys() { DTORF("~legacy_classes::CleanPolys "); }
	void prepForReuse() {
		cout << "    Clean Poly mold: rinse with acetone, dry.\n";
	}
};

class PETCleanup {
public: ~PETCleanup() { DTORF("~legacy_classes::PETCleanup "); }
	void carbonize() { // Use only on stainless steel.
		cout << "    Clean PET steel mold: heat to 895 C.\n";
	}
	void purify() {	// Use only on aluminum.
		cout << "    Clean PET aluminum mold: rinse with soap and water, dry.\n";
	}
};

void defaulting(map<string,string>& order, const string& option, string def="simulation") {
	cout << "  <>Unknown " << option << " |";
	cout << order[option] << "|";
	order[option] = def;
	cout << " defaulting to '" << order[option] << "'.\n";
}

}

namespace adapter {			// DP 2.
// use for clean mold

class Size {
	unsigned int size;
public:
	Size(string sizeStr) {
		if (sizeStr == "") {
			cout << "  <>No size specified, defaulting to 100.\n";
			size = 100;
		}
		else {
			size = stoul(sizeStr);
		
			if (size > 50000) {
				cout << "  <>Size exceeds mold lifetime |" << size << "| defaulting to MediumOrder of 50000.\n";
				size = 50000;
			}
		}
	}
	~Size() { cout << "~Size" << endl; }
	unsigned int value() { return size; }
};

class Mold {
protected:
	string type;
public:
	Mold(string type) : type(type) {}
	string getType() { return type; }
	static Mold * makeObject(unsigned int size);
};
class Aluminum : public Mold {
public:
	Aluminum() : Mold("Aluminum") {}
	~Aluminum() { cout << "~Aluminum" << endl; }
};
class Steel : public Mold {
public:
	Steel() : Mold("Steel") {}
	~Steel() { cout << "~Steel" << endl; }
};

Mold * Mold::makeObject(unsigned int size) {
	if (size <= 10000)
		return new Aluminum();
	else if (size <= 50000)
		return new Steel();
	else {
		cout << "  <>Size exceeds mold lifetime |" << size  << "| defaulting to MediumOrder of 50000.\n";
		return new Aluminum();
	}
}


// Seam point - add another interface.

}

namespace strategy {		// DP 1.

// This should be how you calculate the runtime estimate

// Seam point - add another algorithm.

}

namespace observer {		// DP 5.

// Bin observer goes here

// Seam point - add another listener.

}

namespace abstract_factory {// DP 9.

// InjectionLine goes here

class Packager {
public:
	static Packager * makeObject(string packager);
};
class Bulk : public Packager {
	
};
class ShrinkWrap : public Packager {
	
};
class HardPack : public Packager {
	
};

Packager * Packager::makeObject(string packager) {
	// 3.1 Bulk (default, just dump part to output bin)
	if (packager == "Bulk")
		return new Bulk();
	// 3.2 ShrinkWrap
	else if (packager == "ShrinkWrap")
		return new ShrinkWrap();
	// 3.3 HardPack
	else if (packager == "HardPack")
		return new HardPack();
	// 3.4 If Packager unknown, default to Bulk (none)
	else {
		cout << "  <>Unknown packager |<unknown packager>| defaulting to 'None'.\n";
		return new Bulk();
	}
}

class Bin;
class ConveyerBelt {
protected:
	string name;
public:
	ConveyerBelt(string name) : name(name) {}
	virtual ~ConveyerBelt() {
		cout << " ~ConveyerBelt " << endl;
	}
	string Name() { return name; };
	//virtual void interact1(ConveyerBelt * p)=0;
};
class Bin {
protected:
	string name;
public:
	Bin(string name) : name(name) {}
	virtual ~Bin() {
		cout << " ~Bin " << endl;
	}
	string Name() { return name; };
	//virtual void interact2(ConveyerBelt * p)=0;
};
class InjectionLine {
public:
	~InjectionLine() {
		cout << " ~InjectionLine" << endl;
	}
	virtual Bin * createBin()=0;
	virtual ConveyerBelt * createConveyerBelt()=0;
	static InjectionLine * makeObject(unsigned int size, string packager);
};

class LinearConveyerBelt : public ConveyerBelt {
public:
	LinearConveyerBelt() : ConveyerBelt("Linear conveyer belt") {}
};
class YSplitConveyerBelt : public ConveyerBelt {
public:
	YSplitConveyerBelt() : ConveyerBelt("Y-Split conveyer belt") {}
};

class CardboardBox : public Bin {
public:
	CardboardBox() : Bin("CardboardBox") {}
};
class ShellBox : public Bin {
public:
	ShellBox() : Bin("ShellBox") {}
};
class PallotBox : public Bin {
public:
	PallotBox() : Bin("PallotBox") {}
};

class PilotOrder : public InjectionLine {
public:
	~PilotOrder() {
		cout << " ~PilotOrder" << endl;
	}
	Bin * createBin() { return new CardboardBox(); }
	ConveyerBelt * createConveyerBelt() { return new LinearConveyerBelt(); }
};
class SmallOrder : public InjectionLine {
public:
	~SmallOrder() {
		cout << " ~SmallOrder" << endl;
	}
	Bin * createBin() { return new ShellBox(); }
	ConveyerBelt * createConveyerBelt() { return new YSplitConveyerBelt(); }
};
class MediumOrder : public InjectionLine {
public:
	~MediumOrder() {
		cout << " ~MediumOrder" << endl;
	}
	Bin * createBin() { return new PallotBox(); }
	ConveyerBelt * createConveyerBelt() { return new LinearConveyerBelt(); }
};

InjectionLine * InjectionLine::makeObject(unsigned int size, string packager) {
	cout << "  Setup injection line for " << size << " order with " << packager << " packager:\n";
	if (size <= 10000) {
		return new PilotOrder();
	}
	else if (size <= 20000) {
		return new SmallOrder();
	}
	else if (size <= 50000) {
		return new MediumOrder();
	}
	else {
		cout << "Error in AbstractFactory::makeObject" << endl;
		return 0;
	}
}
// Seam point - add another type 1.
// Seam point - add another type 2.
// Seam point - add another type 3.
// Seam point - add another ...
// Seam point - add another type N.
// Seam point - add another family.

}

namespace bridge {			// DP 8.

// Seam Point - add another implementation.
// Seam Point - add another abstraction.

class Platform {
protected:
	string metal;
	string finish;
public:
	Platform(string metal, string finish) : metal(metal), finish(finish) {}
	virtual ~Platform() {
		cout << " ~Platform/n" << endl;
	}
	virtual void formShape(string shapeName)=0;
	static Platform * makeObject(string metal, string finish);
};

class Shape {
protected:
	Platform * plat;
	unsigned int volume_cc;
	string name;
public:
	Shape(string name, unsigned int volume_cc, Platform * plat) :
	name(name),
	volume_cc(volume_cc),
	plat(plat)
	{}
	virtual ~Shape() {  cout << "~Shape/n" << endl; }
	void makeShape() { plat->formShape(name); }
	static Shape * makeObject(string criteria, Platform * plat);
};

class HighCarbon : public Platform {
public:
	HighCarbon(string metal, string finish) : Platform(metal, finish) {}
	void formShape(string shape) {
		cout << "      using HighCarbon tools (drill, cut, and high speed grind) to mill " << metal << " block into <n> " << shape << " shape(s) with " << finish << " finish.\n";
	}
};
class Carbide : public Platform {
public:
	Carbide(string metal, string finish) : Platform(metal, finish) {}
	void formShape(string shape) {
		cout << "      using Carbide tools (high speed drill, cross cut, and layer grind) to mill " << metal << " block into <n> " << shape << " shape(s) with " << finish << " finish.\n";
	}
};
class DiamondTipped : public Platform {
public:
	DiamondTipped(string metal, string finish) : Platform(metal, finish) {}
	void formShape(string shape) {
		cout << "      using DiamondTipped tools (precision drill, oil cooled cut, and cartoid grind) to mill " << metal << " block into <n> " << shape << " shape(s) with " << finish << " finish.\n";
	}
};

class Duck : public Shape {
public:
	Duck(Platform * plat) : Shape("Duck", 35, plat) {}
	~Duck() {  cout << "~Duck" << endl; }
};
class Car : public Shape {
public:
	Car(Platform * plat) : Shape("Car", 40, plat) {}
	~Car() {  cout << "~Car" << endl; }
};
class Hero : public Shape {
public:
	Hero(Platform * plat) : Shape("Hero", 50, plat) {}
	~Hero() {  cout << "~Hero" << endl; }
};

Platform * Platform::makeObject(string metal, string finish) {
	// 15.1 HighCarbon tools (suitable for aluminum, and steel with smooth finish)
	if (metal == "aluminum" || (metal == "steel" && finish == "smooth"))
		return new HighCarbon(metal, finish);
	// 15.2 Carbide tools (suitable for steel)
	else if (metal == "steel" && finish == "rippled")
		return new Carbide(metal, finish);
	// 15.3 Diamond tipped (suitable for steel) (default)
	else
		return new DiamondTipped(metal, finish);
}

Shape * Shape::makeObject(string criteria, Platform * plat) {
	if (criteria == "duck")
		return new Duck(plat);
	else if (criteria == "car")
		return new Car(plat);
	else if (criteria == "hero")
		return new Hero(plat);
	else {
		cout << "  <>Unknown mold || defaulting to 'duck'." << endl;
		return new Duck(plat);
	}
}

}


namespace chain_of_resp {	// DP 7.

// Seam points - add another responder.

// GetMold is the makeDecision of this cofr
}

namespace decorator {		// DP 6.

// Seam point - add another option.

// Plastic should be decorator
class Plastic {
	
public:
	virtual ~Plastic() {  cout << "~Plastic\n" << endl; }
	static Plastic * makeObject(string criteria);
};
class ABS : public Plastic {
public:
	~ABS() {  cout << "~ABS" << endl; }
};
class Polypropylene : public Plastic {
public:
	~Polypropylene() {  cout << "~Polypropylene" << endl; }
};
class Polyethelene : public Plastic {
public:
	~Polyethelene() {  cout << "~Polyethelene" << endl; }
};
class PET : public Plastic {
public:
	~PET() {  cout << "~PET" << endl; }
};

Plastic * Plastic::makeObject(string criteria) {
	if (criteria == "ABS")
		return new ABS();
	else if (criteria == "Polypropylene")
		return new Polypropylene();
	else if (criteria == "Polyethelene")
		return new Polyethelene();
	else {
		cout << "  <>Unknown plastic || defaulting to 'ABS'." << endl;
		return new ABS();
	}
}
}

namespace factory_method {	// DP 4.

// Seam point - add another class.

}

namespace template_method {	// DP 3.

// Should be used for process order
class ProcessOrder{	
public:	
	enum class Procedures {
		procedureType1
		// Seam point - add another procedure
	};
	virtual ~ProcessOrder() { cout << "~ProcessOrder\n" << endl;}
	
	static ProcessOrder * makeObject(Procedures p);
	
	void run() {
		cout << "  Process order:\n";
		// 10.1 Get mold
		// 10.2 Insert tags into mold
		// 10.3 Load plastic, color die, and additive bins
		// 10.4 Simulate cycling the IJM for the specified plastic <run size> times (see specs 20-23)
		// 10.4.1 Just once, specify the injection cycle (depends on plastic type)
		// 10.4.2 Just once, simulate a full parts bin asking upline machines to pause
		// 10.5 Clean the mold (ignore molds that are used up, clean them all)
	}
	
private:
	string setup() { return "  setup "; }
protected:
	virtual string func() {return "Oops!";}
};

// Seam point - add another polymorphic step.
// Seam point - add another constant step.
// Seam point - convert a constant step into a polymorphic step.

}

void process(map<string,string>& order) {
	// Fill in the namespaces above with your design pattern class hierarchies.
	// Call your order processing class from here <myProcess>->run(order);
	decorator::Plastic::makeObject(order["plastic"]);
	adapter::Size size(order["size"]);
	abstract_factory::Packager * packager = abstract_factory::Packager::makeObject(order["packager"]);
	adapter::Mold * mold = adapter::Mold::makeObject(size.value());

	bridge::Platform * plat = bridge::Platform::makeObject(mold->getType(), order["finish"]);
	bridge::Shape * shape = bridge::Shape::makeObject(order["mold"], plat);
	shape->makeShape();
}

pair<string,string> parse(string line) {
	char key[83];
	char val[83] = {0};

	sscanf(line.c_str(), "%s", key);
	char * cstr = new char [line.length()+1];
	std::strcpy (cstr, line.c_str());
	char* equalSign = strchr(cstr, '=');
	string value = "";
	if(equalSign) {	// tags = sam i am
		char*	nextToken = equalSign += 2;
		while(nextToken) {
			sscanf(nextToken, "%s", val);
			value += val;
			nextToken = strchr(nextToken+1, ' ');
			value += " ";
		}
		value.erase(value.length()-1, 1);
	}

	return make_pair(key, value);
}

map<string,string> getCompleteOrder(FILE* orderFilePtr) {
	map<string,string> order;
	char line[80+3];
	pair<string,string>	keyValue;

	while(fgets(line, 80, orderFilePtr)) {
		cout << line;
		if(strstr(line, "endOfOrder")) {
			return(order);
			}
		else {
			keyValue = parse(line);
			string key = keyValue.first;
			if(strchr(key.c_str(), ':'))	// Skip left justified order number.
				continue;
			string value = keyValue.second;
			order[key] = value;
		}
	}

	return order;
}

void demo(const string& orderFile) {
	cout << "<<< final original >>>\n";

	FILE* orderFilePtr = fopen(orderFile.c_str(), "r");

	for(;;) {	// Get and process each order.
		map<string,string> order = getCompleteOrder(orderFilePtr);
		if(order.size() == 0)
			break;
		process(order);
	}
}

} // final_design

#endif /* FINAL_FINAL_DESIGN_H_ */

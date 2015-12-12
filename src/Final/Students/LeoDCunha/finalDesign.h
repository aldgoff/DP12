/*
 * finalDesign.h
 *
 *  Created on: <12102015>
 *      Author: Leonardo da Cunha
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

int estimateRunTime(int orderSize, int cavities, string metal, int volume) {
	if (orderSize <= 10000) return (float)orderSize/cavities*((float)1/60);
	if (orderSize <= 25000) return ((float)orderSize/cavities)*(float)legacy_classes::cycleTime_sec(metal, volume)/3600;
	int cycletime = legacy_classes::cycleTime_sec(metal, volume);
	float runTime = (legacy_classes::setupAvg_min + (orderSize/cavities)*cycletime/60 + legacy_classes::teardownAvg_min)/60;
	return runTime;
}

namespace adapter {			// DP 2.

class CleanMold {
protected:
	string &name;
public:
	CleanMold(string& name) : name(name) {}
	virtual ~CleanMold() { DTORF("~adapter::CleanMold\n"); };
	string& getName() { return name; }
	virtual void Clean(string& metal) = 0;
	virtual void Inject() = 0;
	static CleanMold* createObject(map<string,string>& order);
};

class ABS : public CleanMold {
	legacy_classes::CleanABS wrapped;
public:
	ABS(string& name) : CleanMold(name) {}
	~ABS() { DTORF("~ABS "); }
	void Clean(string& metal) {
		wrapped.clean();
	}
	void Inject() {
		cout << "      Close - mix - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject.\n";
	}
};

class Poly : public CleanMold {
	legacy_classes::CleanPolys wrapped;
public:
	Poly(string& name) : CleanMold(name) {}
	~Poly() { DTORF("~Poly "); }
	void Clean(string& metal) {
		wrapped.prepForReuse();
	}
	void Inject() {
		cout << "      Close - mix - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject.\n";
	}
};

class PET : public CleanMold {
	legacy_classes::PETCleanup wrapped;
public:
	PET(string& name) : CleanMold(name) {}
	~PET() { DTORF("~PET "); }
	void Clean(string& metal) {
		if (metal == "Aluminum") wrapped.purify();
		if (metal == "Steel") wrapped.carbonize();
	}
	void Inject() {
		cout << "      Close - mix - heat to 404 - inject at 110 PSI - cool to 340 - separate - smooth eject.\n";
	}
};
// Seam point - add another algorithm.

CleanMold* CleanMold::createObject(map<string,string>& order) {
	string plasticType = order["plastic"];
	if ((plasticType == "Polypropylene") || (plasticType=="Polyethelene")) return new Poly(plasticType);
	if (plasticType=="PET") return new PET(plasticType);
	if (plasticType != "ABS") 
		legacy_classes::defaulting(order, "plastic", "ABS");
	return new ABS(plasticType);
}
}

namespace strategy {		// DP 1.

// Seam point - add another algorithm.

}

namespace observer {		// DP 5.

// Seam point - add another listener.
class BinObserver;
class BinSubject {
	list<BinObserver*> observers;
	string name;
public:
	BinSubject(string name) : name(name) {}
	virtual ~BinSubject() {
		DTORF("~observer::BinSubject\n"); 
		cout << " BinObservers left to process (should be zero) = " << observers.size() <<".\n";
	}
	string getName() { return name; }
	void attach(BinObserver* obs) { observers.push_back(obs); }
	void detach(BinObserver* obs) { observers.remove(obs); }
	void full() ;
};

class CardboardBox : public BinSubject {
public:
	CardboardBox() : BinSubject("CardboardBox") {}
	~CardboardBox() { DTORF("~CardboardBox "); }
};

class ShellBox : public BinSubject {
public:
	ShellBox() : BinSubject("ShellBox") {}
	~ShellBox() { DTORF("~ShellBox "); }
};

class PalletBox : public BinSubject {
public:
	PalletBox() : BinSubject("PalletBox") {}
	~PalletBox() { DTORF("~PalletBox "); }
};

class BinObserver {
protected:
	BinSubject& bin;
public:
	BinObserver(BinSubject& target) : bin(target) {
		bin.attach(this);
	}
	virtual ~BinObserver() { 
		bin.detach(this);
		DTORF("~observer::BinObserver\n");
	}
	virtual void pause() {}
};

void BinSubject::full() {
	cout << "      Whenever "<< name << " parts bin was full...\n"; 
	for (auto ob: observers){
		ob->pause();
		cout << " while "<< name << " parts bin was swapped.\n";
	}
}

}

namespace abstract_factory {// DP 9.

class IJM : public observer::BinObserver {
public:
	IJM(observer::BinSubject& bin) : BinObserver(bin) {}
	virtual ~IJM() { DTORF("~IJM "); }
	virtual string getName() = 0;
	virtual void pause()= 0;
};
class IJM_110 : public IJM {
public:
	IJM_110(observer::BinSubject& bin) : IJM(bin) {}
	virtual ~IJM_110() { DTORF("~IJM_110 "); }
	string getName() { return "IJM_110"; }
	void pause() { cout << "        IJM_110 paused"; }
};
class IJM_120 : public IJM {
public:
	IJM_120(observer::BinSubject& bin) : IJM(bin) {}
	virtual ~IJM_120() { DTORF("~IJM_120 "); }
	string getName() { return "IJM_120"; }
	void pause() { cout << "        IJM_120 paused"; }
};
class IJM_210 : public IJM {
public:
	IJM_210(observer::BinSubject& bin) : IJM(bin) {}
	virtual ~IJM_210() { DTORF("~IJM_210 "); }
	string getName() { return "IJM_210"; }
	void pause() { cout << "        IJM_210 paused"; }
};

class ConveyerBelt : public observer::BinObserver {
public:
	ConveyerBelt(observer::BinSubject& bin) : BinObserver(bin) {}
	~ConveyerBelt() { DTORF("~ConveyerBelt "); }
	virtual string getName() = 0;
	virtual void pause() = 0;
};
class LinearBelt : public ConveyerBelt {
public:
	LinearBelt(observer::BinSubject& bin) : ConveyerBelt(bin) {}
	~LinearBelt() { DTORF("~LinearBelt "); }
	string getName() { return "Linear conveyer belt"; }
	void pause() { cout << "        Linear conveyer belt paused"; }
};
class YSplitBelt : public ConveyerBelt {
public:
	YSplitBelt(observer::BinSubject& bin) : ConveyerBelt(bin) {}
	~YSplitBelt() { DTORF("~YSplitBelt "); }
	string getName() { return "Y-Split conveyer belt"; }
	void pause() { cout << "        Y-Split conveyer belt paused"; }
};

class Mold {
protected:
	int cavities;
public:
	Mold(int cavities) : cavities(cavities) {}
	virtual ~Mold() { DTORF("~Mold\n"); }
	virtual string getMetal() = 0;
	virtual string getMetalLowercase() = 0;
	int getCavities() { return cavities; }
	//static Mold* createObject(map<string,string>& order);
};

class Aluminum : public Mold {

public:
	Aluminum(int cavities) : Mold(cavities) {}
	~Aluminum() { DTORF("~Aluminum "); }
	string getMetal() { return "Aluminum"; }
	string getMetalLowercase() { return "aluminum"; }
};

class Steel : public Mold {
public:
	Steel(int cavities) : Mold(cavities) {}
	~Steel() { DTORF("~Steel "); }
	string getMetal() { return "Steel"; }
	string getMetalLowercase() { return "steel"; }
};

class InjectionLine  {
protected:
	int ordSize;
	observer::BinSubject* bin;
public:
	virtual ~InjectionLine() { DTORF("~abstract_factory::InjectionLine\n"); }
	virtual observer::BinSubject* createPartsBin() = 0;
	virtual IJM* createIJM() = 0;
	virtual Mold* createMold() = 0;
	virtual ConveyerBelt* createConveyerBelt() =0;
	static InjectionLine* makeObject(string& orderSize);
};

class PilotOrder : public InjectionLine {
public:
	~PilotOrder() { DTORF("~PilotOrder "); }
	observer::BinSubject* createPartsBin() { return bin = new  observer::CardboardBox; }
	IJM* createIJM(){ return new IJM_110(*bin); }
	Mold* createMold() { return new Aluminum(1); }
	ConveyerBelt* createConveyerBelt() { return new LinearBelt(*bin); }
};

class SmallOrder : public InjectionLine {
public:
	~SmallOrder() { DTORF("~SmallOrder "); }
	observer::BinSubject* createPartsBin() { return bin = new  observer::ShellBox; }
	IJM* createIJM(){ return new IJM_120(*bin); }
	Mold* createMold() { return new Aluminum(2); }
	ConveyerBelt* createConveyerBelt() { return new YSplitBelt(*bin); }
};

class MediumOrder : public InjectionLine {
public:
	~MediumOrder() { DTORF("~MediumOrder "); }
	observer::BinSubject* createPartsBin() { return bin = new  observer::PalletBox; }
	IJM* createIJM(){ return new IJM_210(*bin); }
	Mold* createMold() { return new Steel(1); }
	ConveyerBelt* createConveyerBelt() { return new LinearBelt(*bin); }
};

InjectionLine* InjectionLine::makeObject(string& orderSize) {
	if (orderSize.size() == 0) {
		cout  <<  "  <>No size specified, defaulting to 100.\n";
		orderSize = "100";
	} 
	int ordSize = stoi(orderSize); // c++11 only
	
	if (ordSize <=10000) return new PilotOrder;
	else if (ordSize <=20000) return new SmallOrder;
	else if (ordSize > 50000) {
		cout << "  <>Size exceeds mold lifetime |"<< orderSize <<"| defaulting to MediumOrder of 50000.\n";
		orderSize = "50000";
	}
	return new MediumOrder;
}

// Seam point - add another type 1.
// Seam point - add another type 2.
// Seam point - add another type 3.
// Seam point - add another ...
// Seam point - add another type N.
// Seam point - add another family.

}

namespace bridge {			// DP 8.
class Shape {
public:
	virtual ~Shape() { DTORF("~bridge::Shape\n"); };
	virtual string getName() = 0;
	virtual int getVol() = 0;
	virtual string getMillSteps() = 0;
	static Shape* createObject(map<string,string>& order);
};

class Duck : public Shape {
public:
	~Duck() { DTORF("~Duck "); };
	string getName() {return "duck";}
	int getVol() {return 35;}
	string getMillSteps() {return "D3C2D2G4";}
};

class Car : public Shape {
public:
	~Car() { DTORF("~Car "); };
	string getName() {return "car";}
	int getVol() {return 40;}
	string getMillSteps() {return "D2C8D1G3";}
};

class Hero : public Shape {
public:
	~Hero() { DTORF("~Hero "); };
	string getName() {return "hero";}
	int getVol() {return 50;}
	string getMillSteps() {return "D1C1D1G7";}
};


Shape* Shape::createObject(map<string,string>& order) {
	string& shapeName = order["mold"];
	if (shapeName == "car") return new Car();
	if (shapeName == "hero") return new Hero();
	if (shapeName != "duck") {
		cout << "  ";
		legacy_classes::defaulting(order, "mold", "duck");
	}
	return new Duck();
}
class Platform {
protected:
	string description;
	string finish;
	abstract_factory::Mold* moldType; 
	bridge::Shape* shape;
public:
	Platform(abstract_factory::Mold* moldType, bridge::Shape* shape, string description, string finish) :
		moldType(moldType), shape(shape), description(description), finish(finish) {}
	virtual ~Platform() {
		//delete shape;
		DTORF("~bridge::Platform\n"); 
	}
	void Mill() {
		string plural = moldType->getCavities() == 1 ? "" : "s";
		cout << "      using " << description << " to mill " << moldType->getMetalLowercase() << " block into " << moldType->getCavities() << " "<< shape->getName() << " shape" << plural << " with " << finish << " finish.\n";
	}
	static Platform* makeObject(map<string,string>& order, bridge::Shape* shape, abstract_factory::Mold* moldType);
};
class HighCarbon : public Platform {
public:
	HighCarbon(abstract_factory::Mold* moldType, bridge::Shape* shape, string finish) :
		Platform(moldType, shape, "HighCarbon tools (drill, cut, and high speed grind)", finish) {} 
	~HighCarbon() { DTORF("~HighCarbon "); }
};
class Carbide : public Platform {
public:
	Carbide(abstract_factory::Mold* moldType, bridge::Shape* shape, string finish) :
		Platform(moldType, shape, "Carbide tools (high speed drill, cross cut, and layer grind)", finish) {} 
	~Carbide() { DTORF("~Carbide "); }
};
class DiamondTipped : public Platform {
public:
	DiamondTipped(abstract_factory::Mold* moldType, bridge::Shape* shape, string finish) :
		Platform(moldType, shape, "DiamondTipped tools (precision drill, oil cooled cut, and cartoid grind)", finish) {} 
	~DiamondTipped() { DTORF("~DiamondTipped "); }
};
// Seam Point - add another implementation.
// Seam Point - add another abstraction.
Platform* Platform::makeObject(map<string,string>& order, bridge::Shape* shape, abstract_factory::Mold* moldType) {
	string finish = order["finish"];
	if (finish == "smooth") return new HighCarbon(moldType, shape, finish);
	if (finish == "rippled") return new Carbide(moldType, shape, finish);
	if (finish == "dimpled") return new DiamondTipped(moldType, shape, finish);
	throw "error";
}

}

namespace chain_of_resp {	// DP 7.
class GetMold {
protected:
	GetMold* sucessor;
public:
	GetMold() : sucessor(0) {}
	virtual ~GetMold() { 
		delete sucessor; 
		DTORF("~CofR::GetMold ");
	}
	void setSucessor(GetMold* next) { sucessor = next; }
	virtual void Get(map<string,string>& order, bridge::Shape* shape, abstract_factory::Mold* moldType) {
		cout  << "    <>Can't find place |" << order["moldLoc"] << "| to get |"<< order["mold"] << "| mold from with |"<<order["finish"] <<"| finish, defaulting to smooth duck from inventory.\n";
		order["moldLoc"] = "inventory";
		order["finish"] = "smooth";
	}
};

class Inventory : public GetMold {
public:
	~Inventory() { DTORF("~Inventory "); }
	void Get(map<string,string>& order, bridge::Shape* shape, abstract_factory::Mold* moldType) {
		if (order["moldLoc"] == "inventory") cout << "    Pull " << shape->getName() << " mold from inventory.\n";
		else sucessor->Get(order, shape, moldType);
	}
};

class SisterCompany : public GetMold {
public:
	~SisterCompany() { DTORF("~SisterCompany "); }
	void Get(map<string,string>& order, bridge::Shape* shape, abstract_factory::Mold* moldType) {
		if (order["moldLoc"] == "sister company") cout << "    Borrow " << shape->getName() << " mold from sister company.\n";
		else sucessor->Get(order, shape, moldType);
	}
};

class Mill : public GetMold {
public:
	~Mill() { DTORF("~Mill "); }
	void Get(map<string,string>& order, bridge::Shape* shape, abstract_factory::Mold* moldType) {
		if (order["moldLoc"] == "mill") {
			int cavities = moldType->getCavities();
			cout << "    Create " << shape->getName() << " mold from mill with " << cavities << " cavities - steps: " << shape->getMillSteps() << ".\n";
			bridge::Platform* tool = bridge::Platform::makeObject(order, shape, moldType);
			tool->Mill();
			delete tool;
		} else sucessor->Get(order, shape, moldType);
	}
};

GetMold* setupLocationChain() {
	GetMold* inventory = new Inventory();
	GetMold* sisterCompany = new SisterCompany();
	GetMold* mill = new Mill();
	inventory->setSucessor(sisterCompany);
	sisterCompany->setSucessor(mill);
	mill->setSucessor(new GetMold);
	return inventory;
}

}

namespace decorator {		// DP 6.
class Cavity {
public:
	virtual ~Cavity() { DTORF("~dec::Cavity "); }
	virtual string getName() = 0;
	virtual int getWidth() = 0;
	void insert() {
		cout << "    Insert tags ["<< getName() <<"] of width " << getWidth() << "/20 mm, blank tag is " << 20 - getWidth() << " mm.\n";
	}
};
class Blank : public Cavity {
public:
	~Blank() { DTORF("~Blank "); }
	string getName() { return ""; }
	int getWidth() { return 0; }
};

class Tags : public Cavity {
protected:
	Cavity* decorator;
public:
	Tags(Cavity* decorator) : decorator(decorator) {}
	virtual ~Tags() {
		DTORF("~Tags ");
		delete decorator;
	}
	static Cavity* makeObject(Cavity* decorator, string& criteria);
	string getName() { return ""; }
	int getWidth() { return 0; }
};
class ModelNumber : public Tags {
public:
	ModelNumber(Cavity* decorator) : Tags(decorator) {}
	~ModelNumber() { DTORF("~ModelNumber "); }
	string getName() {
		string all = decorator->getName();
		if (all != "") all = all + " ";
		return all + "ModelNumber";
	}
	int getWidth() { return 2 + decorator->getWidth(); }
};
class Country : public Tags {
public:
	Country(Cavity* decorator) : Tags(decorator) {}
	~Country() { DTORF("~Country "); }
	string getName() {
		string all = decorator->getName();
		if (all != "") all = all + " ";
		return all + "Country";
	}
	int getWidth() { return 2 + decorator->getWidth(); }
};
class Date : public Tags {
public:
	Date(Cavity* decorator) : Tags(decorator) {}
	~Date() { DTORF("~Date "); }
	string getName() {
		string all = decorator->getName();
		if (all != "") all = all + " ";
		return all + "Date";
	}
	int getWidth() { return 2 + decorator->getWidth(); }
};

Cavity* Tags::makeObject(Cavity* decorator, string& criteria) {
	//if (criteria == "Blank") return new Blank(decorator);
	if (criteria == "ModelNumber") return new ModelNumber(decorator);
	if (criteria == "Country") return new Country(decorator);
	if (criteria == "Date") return new Date(decorator);
	cout << "    Ignoring unknown tag " << criteria << ".\n";
	return decorator;
}

// Seam point - add another option.

class Polymer {
public:
	virtual ~Polymer() { DTORF("~dec::Polymer "); }
	virtual void Show() {};
	virtual int getVolume() { return 0; };
	static Polymer* Create(map<string,string>& order, int volume);
};

class Ingredient : public Polymer {
protected:
	Polymer* decorator;
	string name;
	int volume;
public:
	Ingredient(Polymer* decorator, string name, int volume) : decorator(decorator), name(name), volume(volume){}
	~Ingredient() {
		DTORF("~Ingredient(");DTORF(name);DTORF(") ");
		delete decorator;
	}
	void Show() { cout << name << "(" << volume << ") "; decorator->Show(); }
	int getVolume() { return decorator->getVolume() + volume; }
};

Polymer* Polymer::Create(map<string,string>& order, int volume) {
	Polymer* recipe = new Polymer;
	if( order["Hydrophilic"] != "") recipe = new Ingredient(recipe, "Hydrophilic", stoi(order["Hydrophilic"]));
	if( order["AntiBacterial"] != "") recipe = new Ingredient(recipe, "AntiBacterial", stoi(order["AntiBacterial"]));
	if( order["UVInhibiter"] != "") recipe = new Ingredient(recipe, "UVInhibiter", stoi(order["UVInhibiter"]));
	if( order["color"] != "") recipe = new Ingredient(recipe, order["color"], volume / 10);
	recipe = new Ingredient(recipe, order["plastic"], volume - recipe->getVolume());
	return recipe;
}

}
// Split string into tokens
std::vector<std::string> split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    int start = 0, end = 0;
    while ((int)(end = text.find(sep, start)) != (int)std::string::npos) {
        std::string temp=text.substr(start, end - start);
        if(temp!="")tokens.push_back(temp);
        start = end + 1;
    }
    std::string temp=text.substr(start);
    if(temp!="")tokens.push_back(temp);
    return tokens;
}

namespace factory_method {	// DP 4.
class Packager : public observer::BinObserver {
public:
	Packager(observer::BinSubject& bin) : BinObserver(bin) {}
	~Packager() { DTORF("~factory_method::Packager "); }
	virtual string getName() = 0;
	virtual string getNameCamelCase() = 0;
	string getDescription() {
		string name = getName();
		if (name != "") return " with " + name;
		return "";
	}
	void pause() { cout << "        " << getName() << " paused"; }
	static Packager* createObject(map<string,string>& order, observer::BinSubject& bin);
};

class ShrinkWrap : public Packager {
public:
	ShrinkWrap(observer::BinSubject& bin) : Packager(bin) {}
	~ShrinkWrap() { DTORF("~ShrinkWrap "); }
	string getName() { return "Shrink wrap packager"; }
	string getNameCamelCase() { return "ShrinkWrap packager"; }
};

class HardPack : public Packager {
public:
	HardPack(observer::BinSubject& bin) : Packager(bin) {}
	~HardPack() { DTORF("~HardPack "); }
	string getName() { return "Hard pack packager"; }
	string getNameCamelCase() { return "HardPack packager"; }
};
// Seam point - add another algorithm.

Packager* Packager::createObject(map<string,string>& order, observer::BinSubject& bin) {
	string packagerType = order["packager"];
	if (packagerType=="ShrinkWrap") return new factory_method::ShrinkWrap(bin);
	else if (packagerType=="HardPack") return new factory_method::HardPack(bin);
	else if (packagerType != "Bulk") {
		legacy_classes::defaulting(order, "packager", "None");
	}
	return nullptr;
}
// Seam point - add another class.

}

namespace template_method {	// DP 3.

// Seam point - add another polymorphic step.
// Seam point - add another constant step.
// Seam point - convert a constant step into a polymorphic step.

class ProcessOrder {
private:
	adapter::CleanMold* plastic;
	abstract_factory::InjectionLine* injectionLineParts;
	abstract_factory::Mold* moldType;
	abstract_factory::ConveyerBelt* conveyerBelt;
	factory_method::Packager *packager;
	chain_of_resp::GetMold* mold;
	observer::BinSubject* partsBin;
	bridge::Shape* shape;
	decorator::Cavity* tags;
	decorator::Polymer* recipe;
	abstract_factory::IJM* injectionMachine;
public:
	~ProcessOrder() {
		delete packager;
		delete conveyerBelt;
		delete moldType;
		delete injectionMachine;
		delete partsBin;
		delete injectionLineParts;
		delete shape;
		delete mold;
		cout << "\n";
		delete recipe;
		cout << "\n";
		delete tags;
		cout << "\n";
		delete plastic;
		DTORF("~template_method::ProcessOrder\n");
	}
	void run(map<string,string>& order) {

		Setup(order);
		cout << "  Process order:\n";
		GetMold(order);
		InsertTags(order);
		LoadPlasticBin(order);
		EstimateTime(order);
		Inject();
		Full();
		Clean();
	}
	void Setup (map<string,string>& order) {
		plastic = adapter::CleanMold::createObject(order);
		injectionLineParts = abstract_factory::InjectionLine::makeObject(order["size"]);

		partsBin = injectionLineParts->createPartsBin();
		injectionMachine = injectionLineParts->createIJM();
		moldType = injectionLineParts->createMold();

		conveyerBelt = injectionLineParts->createConveyerBelt();

		//9. Setup the injection line: (domains: order size, Packager)
		packager = factory_method::Packager::createObject(order, *partsBin);
		
		cout << "  Setup injection line for " << order["size"] <<" order";
		if (packager) cout << " with "<< packager->getNameCamelCase();
		cout << ":\n";
		
		cout << "    " << injectionMachine->getName() << " - " << moldType->getMetal() << "(" << moldType->getCavities() << ") - " << conveyerBelt->getName();
		cout << " - " << partsBin->getName() << ".\n";
	}
	void GetMold (map<string,string>& order) {
		shape = bridge::Shape::createObject(order);
		mold = chain_of_resp::setupLocationChain();
		mold->Get(order, shape, moldType);
	}
	void InsertTags(map<string,string>& order){
		tags = new decorator::Blank();
		
		vector<string> tagNames = split(order["tags"], ' ');
		for (auto tagName: tagNames) {
			tags = decorator::Tags::makeObject(tags, tagName);
		}
		tags->insert();
	}
	void LoadPlasticBin (map<string,string>& order) {
		
		cout << "    Load plastic bin with " << order["plastic"];
		if (order["color"] != "") cout <<" and color bin with " << order["color"];
		cout << ".\n";
		
		cout << "      Recipe: ";
		recipe = decorator::Polymer::Create(order, shape->getVol());
		recipe->Show();
		cout << "= " << recipe->getVolume() <<" cc.\n";
		
		cout << "      Volume: " << shape->getName() << "(" << shape->getVol() << ") * ";
		int cavities = moldType->getCavities();
		cout << cavities << (cavities > 1 ? " cavities = ": " cavity = ")<< cavities * shape->getVol();
		cout << " cc.\n";
	}
	void EstimateTime(map<string,string>& order) {
		int orderSize = stoi(order["size"]); // c++11 only
		int runTime = estimateRunTime(orderSize, moldType->getCavities(), moldType->getMetalLowercase(), shape->getVol());
		string addS = runTime != 1 ? "s" : "";
		cout << "    Cycle " << injectionMachine->getName() << " for " << order["plastic"] << " " <<orderSize / moldType->getCavities() <<" times, estimated run time = " << runTime << " hour" << addS << ".\n";
	}
	void Clean() {
		string metal = moldType->getMetal();
		plastic->Clean(metal);
	}
	void Inject() {
		plastic->Inject();
	}
	void Full() {
		partsBin->full();
	}
};

}

void process(map<string,string>& order) {
	// Fill in the namespaces above with your design pattern class hierarchies.
	// Call your order processing class from here <myProcess>->run(order);
	
	template_method::ProcessOrder processOrder;
	
	processOrder.run(order);
}

pair<string,string> parse(string line) {
	char key[83];
	char val[83] = {0};

	sscanf(line.c_str(), "%s", key);

	const char* equalSign = strchr(line.c_str(), '=');
	string value = "";
	if(equalSign) {	// tags = sam i am
		const char*	nextToken = equalSign += 2;
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
	cout << "<<< final design >>>\n";

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

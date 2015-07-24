/*
 * finalSolution.h
 *
 *  Created on: <MMDDCCYY>
 *      Author: <student name>
 */

#ifndef FINAL_FINAL_SOLUTION_H_
#define FINAL_FINAL_SOLUTION_H_

#include <map>
using namespace std;

namespace final_solution {

/* Design Patterns Chart
 * x 1 - Strategy: runtime estimate (domain: size)
 * x 2 - Adapter: cleaning mold (domains: plastic, metal)
 *   3 - Facade: milling mold
 * x 4 - Template Method: order processing, injection cycle (domain: plastic)
 * x 5 - Factory Method: packaging (domain: order), stuffing (domain: order) (Observer)
 * x 6 - Decorator: tags, additives (domain: order[tags,additives])
 * x 7 - Observer: bins(Subject) (IJM, conveyer belt, packager, & stuffer pause on full)
 * x 8 - ChainOfR: mold location (inventory, borrow, mill, purchase)
 * x 9 - Bridge: milling
 * x10 - Abstract Factory: IJM_X, mold(metal, cavities), conveyer belt, bins (domain: size)
 */

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
const float rejectRate_pcnt = 1.3;

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

namespace adapter {			// DP 2 - done.

class CleanMold {	// If the interfaces are varying...
public:
	CleanMold() {}
	virtual ~CleanMold() { DTORF("~adapter::CleanMold\n"); }
public:
	virtual void clean(const string& metal) {
		 cout << "  CleanMold base.\n";
	}
};
class ABS : public CleanMold {
	legacy_classes::CleanABS plastic;
public: ~ABS() { DTORF("~ABS "); }
	virtual void clean(const string& metal) {
		plastic.clean();
	}
};
class Poly : public CleanMold {
	legacy_classes::CleanPolys plastic;
public: ~Poly() { DTORF("~Poly "); }
	virtual void clean(const string& metal) {
		plastic.prepForReuse();
	}
};
class PET : public CleanMold {
	legacy_classes::PETCleanup plastic;
public: ~PET() { DTORF("~PET "); }
	virtual void clean(const string& metal) {
		if(metal == "steel")
			plastic.carbonize();
		else
			plastic.purify();
	}
};
// Seam point - add another interface.
class Synthetic : public CleanMold {
public: ~Synthetic() { DTORF("~Synthetic "); }
	virtual void clean(const string& metal) {
		cout << "    Clean synthetic mold: ozone wash.\n";
	}
};

}

namespace strategy {		// DP 1 - done except for todos.

// 1) BackOfTheEnvelope(10k): runTime = orderSize/cavities*(1/60);
// 2) Calculation(25k):       runTime = orderSize/cavities*legacy::cycletime(metal, mold->volume);
// 3) Historical(50k):        runTime = setupAvg_min + orderSize/cavities*mold->cycletime + teardownAvg_min;
// Seam point -
// 4) Projection(100k): runTime = ijm->setupTime_mins()
//							    + (1 + 0.01*rejectRate_pcnt)*orderSize/cavities*mold->cycletime()
//						        + ijm->teardownTime_mins();

class RuntimeEstimate {	// If the algorithms are varying...
protected:
	int orderSize;
	int cavities;
public:
	RuntimeEstimate() : orderSize(0), cavities(0) {}
	virtual ~RuntimeEstimate() { DTORF("~strategy::RuntimeEstimate\n"); }
protected:
	void runTimeEst_hrs(map<string,string>& order) {
		orderSize = atoi(order["size"].c_str());
		cavities = atoi(order["cavities"].c_str());
		if(cavities <= 0)	cavities = 1;
	}
public:
	virtual int operator()(map<string,string>& order) {
		return 999;
	}
public:
	static RuntimeEstimate* selectEstimationAlgorithm(map<string,string>& order);
};
class BackOfTheEnvelope : public RuntimeEstimate {
public:
	BackOfTheEnvelope() {}
	~BackOfTheEnvelope() { DTORF("~BackOfTheEnvelope "); }
public:
	virtual int operator()(map<string,string>& order) {
		RuntimeEstimate::runTimeEst_hrs(order);
		return (orderSize/cavities)/60;
	}
};
class Calculation : public RuntimeEstimate {
public:
	Calculation() {}
	~Calculation() { DTORF("~Calculation "); }
public:
	virtual int operator()(map<string,string>& order) {
		RuntimeEstimate::runTimeEst_hrs(order);
		int volume_cc = atoi(order["volume"].c_str());
		int cycleTime = legacy_classes::cycleTime_sec(order["metal"], volume_cc);
		return (orderSize/cavities)*cycleTime/3600;
	}
};
class Historical : public RuntimeEstimate {	// TODO: mold->cycletime_sec().
public:
	Historical() {}
	~Historical() { DTORF("~Historical "); }
public:
	virtual int operator()(map<string,string>& order) {
		using namespace legacy_classes;
		RuntimeEstimate::runTimeEst_hrs(order);
		int cycleTime = 30; // Get from mold->cycletime_sec();
		return (setupAvg_min + (orderSize/cavities)*cycleTime/60 + teardownAvg_min)/60;
	}
};
// Seam point - add another algorithm.
class Projection : public RuntimeEstimate {	// TODO: mold->cycletime_sec() & ijm->times_mins().
public:
	Projection() {}
	~Projection() { DTORF("~Projection "); }
public:
	virtual int operator()(map<string,string>& order) {
		using namespace legacy_classes;
		RuntimeEstimate::runTimeEst_hrs(order);
		int cycleTime = 30; // Get from mold->cycletime_sec();
		int setupTime_min    = setupAvg_min;	// Get from ijm->setupTime_mins();
		int tearDownTime_min = teardownAvg_min;	// Get from ijm->tearDownTime_mins();
		return (setupTime_min
			 + (1 + 0.01*rejectRate_pcnt)*(orderSize/cavities)*cycleTime/60
			 + tearDownTime_min)/60;
	}
};

RuntimeEstimate* RuntimeEstimate::selectEstimationAlgorithm(map<string,string>& order) {
	int orderSize = atoi(order["size"].c_str());

	if(		orderSize <=  10000)	return new BackOfTheEnvelope;
	else if(orderSize <=  25000)	return new Calculation;
	else if(orderSize <=  50000)	return new Historical;
	// Seam point - add another runtime estimation algorithm.
	else if(orderSize <= 100000)	return new Projection;

	else {
		return new Projection;
	}
}

}

namespace observer {		// DP 7.

// Seam point - add another listener.

}

namespace abstract_factory {// DP 10 - done except for Observer.

// Seam point - add another type 1.
// Seam point - add another type 2.
// Seam point - add another type 3.
// Seam point - add another ...
// Seam point - add another type N.

class IJM {
public:
	IJM() {}
	virtual ~IJM() { DTORF("~IJM "); }
public:
	virtual string setup() { return "IJM base"; }
};
class IJM_110 : public IJM {					// PilotOrder.
public:
	~IJM_110() { cout << "~IJM_110 "; }
public:
	string setup() { return "IJM_110"; }
};
class IJM_120 : public IJM {					// SmallOrder.
public:
	~IJM_120() { cout << "~IJM_120 "; }
public:
	string setup() { return "IJM_120"; }
};
class IJM_210 : public IJM {					// MediumOrder.
public:
	~IJM_210() { cout << "~IJM_210 "; }
public:
	string setup() { return "IJM_210"; }
};
// Seam point - add another Injection Molding machine.
class IJM_140 : public IJM {					// FastOrder.
public:
	~IJM_140() { cout << "~IJM_140 "; }
public:
	string setup() { return "IJM_140"; }
};
class IJM_220 : public IJM {					// LargeOrder.
public:
	~IJM_220() { cout << "~IJM_220 "; }
public:
	string setup() { return "IJM_220"; }
};
class IJM_240 : public IJM {					// HugeOrder.
public:
	~IJM_240() { cout << "~IJM_240 "; }
public:
	string setup() { return "IJM_240"; }
};

class Mold {
	unsigned m_cavities;
public:
	Mold(unsigned cavities=0) : m_cavities(cavities) {}
	virtual ~Mold() { DTORF("~Mold "); }
public:
	virtual string setup() { return "Mold base"; }
//	virtual string metal() { return "unobtainium"; }
	unsigned cavities() { return m_cavities; }
	string cavitiesAsString() {
		char cav[] = "( )";
		cav[1] = cavities() + '0';
		return string(cav);
	}
};
class Aluminum : public Mold {
public:
	Aluminum(unsigned cavities=0) : Mold(cavities) {}
	~Aluminum() { cout << "~Aluminum "; }
public:
	string setup() { return "Aluminum" + cavitiesAsString(); }
};
class Steel : public Mold {
public:
	Steel(unsigned cavities=0) : Mold(cavities) {}
	~Steel() { cout << "~Steel "; }
public:
	string setup() { return "Steel" + cavitiesAsString(); }
};

class ConveyerBelt {
public:
	ConveyerBelt() {}
	virtual ~ConveyerBelt() { DTORF("~ConveyerBelt "); }
public:
	virtual string setup() { return "ConveyerBelt base"; }
};
class LinearBelt : public ConveyerBelt {
public:
	LinearBelt() {}
	~LinearBelt() { cout << "~LinearBelt "; }
public:
	string setup() {return "Linear conveyer belt"; }
};
class YSplitBelt : public ConveyerBelt {
public:
	YSplitBelt() {}
	~YSplitBelt() { cout << "~YSplitBelt "; }
public:
	string setup() {return "Y-Split conveyer belt"; }
};
// Seam point - add another conveyer belt.
class VLevelBelt : public ConveyerBelt {
public:
	VLevelBelt() {}
	~VLevelBelt() { cout << "~VLevelBelt "; }
public:
	string setup() {return "V-Level conveyer belt"; }
};

class PackageBin {
public:
	PackageBin() {}
	virtual ~PackageBin() { DTORF("~PackageBin "); }
public:
	virtual string setup() { return "PackageBin base"; }
};
class CardboardBox : public PackageBin {
public:
	~CardboardBox() { cout << "~CardboardBox "; }
public:
	string setup() { return "CardboardBox"; }
};
class PallotBox : public PackageBin {
public:
	~PallotBox() { cout << "~PallotBox "; }
public:
	string setup() { return "PallotBox"; }
};
// Seam point - add another package bin.
class Crate : public PackageBin {
public:
	~Crate() { cout << "~Crate "; }
public:
	string setup() { return "Crate"; }
};

class InjectionLine {	// If the families are varying...
public:
	InjectionLine() {}
	virtual ~InjectionLine() { DTORF("~abstract_factory::InjectionLine\n"); }
public:
	virtual IJM* createIJM(map<string,string>& order) {
		return new IJM();
	}
	virtual Mold* createMold(map<string,string>& order) {
		return new Mold();
	}
	virtual ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new ConveyerBelt();
	}
	virtual PackageBin* createPackageBin(map<string,string>& order) {
		return new PackageBin();
	}
public:
	static InjectionLine* createInjectionLine(map<string,string>& order);
};
class PilotOrder : public InjectionLine {
public:	virtual ~PilotOrder() { DTORF("~PilotOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_110();
	}
	Mold* createMold(map<string,string>& order) {
		return new Aluminum(1);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new LinearBelt();
	}
	PackageBin* createPackageBin(map<string,string>& order) {
		return new CardboardBox();
	}
};
class SmallOrder : public InjectionLine {
public:	virtual ~SmallOrder() { DTORF("~SmallOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_120();
	}
	Mold* createMold(map<string,string>& order) {
		return new Aluminum(2);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new YSplitBelt();
	}
	PackageBin* createPackageBin(map<string,string>& order) {
		return new CardboardBox();
	}
};
class MediumOrder : public InjectionLine {
public:	virtual ~MediumOrder() { DTORF("~MediumOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_210();
	}
	Mold* createMold(map<string,string>& order) {
		return new Steel(1);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new LinearBelt();
	}
	PackageBin* createPackageBin(map<string,string>& order) {
		return new PallotBox();
	}
};
// Seam point - add another family.
class FastOrder : public InjectionLine {
public:	virtual ~FastOrder() { DTORF("~FastOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_140();
	}
	Mold* createMold(map<string,string>& order) {
		return new Aluminum(4);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new VLevelBelt();
	}
	PackageBin* createPackageBin(map<string,string>& order) {
		return new PallotBox();
	}
};
class LargeOrder : public InjectionLine {
public:	virtual ~LargeOrder() { DTORF("~LargeOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_220();
	}
	Mold* createMold(map<string,string>& order) {
		return new Steel(2);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new YSplitBelt();
	}
	PackageBin* createPackageBin(map<string,string>& order) {
		return new Crate();
	}
};
class HugeOrder : public InjectionLine {
public:	virtual ~HugeOrder() { DTORF("~HugeOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_240();
	}
	Mold* createMold(map<string,string>& order) {
		return new Steel(4);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new VLevelBelt();
	}
	PackageBin* createPackageBin(map<string,string>& order) {
		return new Crate();
	}
};

InjectionLine* InjectionLine::createInjectionLine(map<string,string>& order) {
	unsigned size = atoi(order["size"].c_str());

	if(		size <= 10000)		return new PilotOrder;
	else if(size <= 20000)		return new SmallOrder;
	// Seam point - add fast order.
	else if(size <= 40000)		return new FastOrder;
	else if(size <= 50000)		return new MediumOrder;
	// Seam point - add larger orders.
	else if(size <= 100000)		return new LargeOrder;
	else if(size <= 200000)		return new HugeOrder;

	else {						// Defaulting to HugeOrder.
		cout << "  <>Size exceeds mold lifetime |" << size << "|";
		order["size"] = "200000";
		cout << " defaulting to HugeOrder of " << order["size"] << ".\n";
		return new HugeOrder;
	}
}

}

namespace bridge {			// DP 9.

// Seam Point - add another implementation.
// Seam Point - add another abstraction.

}

namespace chain_of_resp {	// DP 8.

// Seam points - add another responder.

}

namespace decorator {		// DP 6 - done.

class TagCavity {	// If the options are varying...
public:
	const unsigned space_mm;
private:
	unsigned blank_mm;
protected:
	unsigned tags_mm;
public:
	TagCavity(unsigned width_mm=20)
	  : space_mm(width_mm), blank_mm(space_mm), tags_mm(0)
	{}
	virtual ~TagCavity() { DTORF("~decorator::TagCavity\n"); }
public:
	virtual unsigned width_mm() { return 0; }
	virtual string list() { return ""; }
public:
	unsigned computeBlankWidth(unsigned tags) {
		tags_mm = tags;
		blank_mm = space_mm - tags_mm;
		return blank_mm;
	}
public:
	static TagCavity* addTags(TagCavity* cavity, const string& list);
};
class Blank : public TagCavity {
public:
	Blank(unsigned width_mm=20) : TagCavity(width_mm) {}
	virtual ~Blank() { DTORF("~Blank "); }
public:
	virtual unsigned width_mm() { return 0; }
	string list() { return ""; }
};
class Tags : public TagCavity {
protected:
	TagCavity* delegate;
public:
	Tags(TagCavity* delegate) : delegate(delegate) {}
	virtual ~Tags() { cout << "~Tags "; delete delegate; }
};
class ModelNumber : public Tags {
public:
	ModelNumber(TagCavity* delegate) : Tags(delegate) {}
	virtual ~ModelNumber() { cout << "~ModelNumber "; }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "ModelNumber "; }
};
class Country : public Tags {
public:
	Country(TagCavity* delegate) : Tags(delegate) {}
	virtual ~Country() { cout << "~Country "; }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "Country "; }
};
class Date : public Tags {
public:
	Date(TagCavity* delegate) : Tags(delegate) {}
	virtual ~Date() { cout << "~Date "; }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "Date "; }
};
// Seam point - add another option.
class IncCounter : public Tags {
public:
	IncCounter(TagCavity* delegate) : Tags(delegate) {}
	virtual ~IncCounter() { cout << "~IncCounter "; }
public:
	unsigned width_mm() { return delegate->width_mm() + 4; }
	string list() { return delegate->list() + "IncCounter "; }
};
class PartNumber : public Tags {
public:
	PartNumber(TagCavity* delegate) : Tags(delegate) {}
	virtual ~PartNumber() { cout << "~PartNumber "; }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "PartNumber "; }
};
class RecycleCode : public Tags {
public:
	RecycleCode(TagCavity* delegate) : Tags(delegate) {}
	virtual ~RecycleCode() { cout << "~RecycleCode "; }
public:
	unsigned width_mm() { return delegate->width_mm() + 6; }
	string list() { return delegate->list() + "RecycleCode "; }
};

TagCavity* TagCavity::addTags(TagCavity* cavity, const string& list) {
	char val[83] = {0};
	const char*	remainingTokens = list.c_str();
	while(remainingTokens) {
		if(0 == sscanf(remainingTokens, "%s", val))
			break;

		if(		!strcmp(val, "ModelNumber"))	cavity = new ModelNumber(cavity);
		else if(!strcmp(val, "Country"))		cavity = new Country(cavity);
		else if(!strcmp(val, "Date"))			cavity = new Date(cavity);
		// Seam point - add more tags.
		else if(!strcmp(val, "IncCounter"))		cavity = new IncCounter(cavity);
		else if(!strcmp(val, "PartNumber"))		cavity = new PartNumber(cavity);
		else if(!strcmp(val, "RecycleCode"))	cavity = new RecycleCode(cavity);

		else {
			cout << "Ignoring unknown tag " << val << ".\n";
		}

		remainingTokens = strchr(remainingTokens+1, ' ');
	}

	return cavity;
}

class Polymer {
protected:
	unsigned volume_m3;
public:
	Polymer(unsigned volume_m3=0) : volume_m3(volume_m3) {}
	virtual ~Polymer() { DTORF("~decorator::Polymer\n"); }
public:
	virtual unsigned mix() { return 0; }
	virtual string idNvol() { return ""; }
protected:
	 string volAsStr() {
		char vol[80];
		sprintf(vol, "(%d)", volume_m3);
		return vol;
	}
};
class Plastic : public Polymer {
public:
	Plastic(unsigned volume_m3=0) : Polymer(volume_m3) {}
	virtual ~Plastic() { DTORF("~Plastic "); }
};
class Additive : public Polymer {
protected:
	Polymer* delegate;
public:
	Additive(Polymer* delegate, unsigned volume_m3)
	  : Polymer(volume_m3), delegate(delegate) {}
	virtual ~Additive() { DTORF("~Polymer "); delete delegate; }
};
class Color : public Additive {
public:
	Color(Polymer* delegate, unsigned volume_m3=0)
	  : Additive(delegate, volume_m3) {}
	virtual ~Color() { DTORF("~Color "); }
public:
	unsigned mix() { return delegate->mix() + volume_m3; }
	string idNvol() { return delegate->idNvol() + " Color" + volAsStr(); }
};
class UVInhibiter : public Additive {
public:
	UVInhibiter(Polymer* delegate, unsigned volume_m3=0)
	  : Additive(delegate, volume_m3) {}
	virtual ~UVInhibiter() { DTORF("~UVInhibiter "); }
public:
	unsigned mix() { return delegate->mix() + volume_m3; }
	string idNvol() { return delegate->idNvol() + " UVInhibiter" + volAsStr(); }
};
class AntiBacterial : public Additive {
public:
	AntiBacterial(Polymer* delegate, unsigned volume_m3=0)
	  : Additive(delegate, volume_m3) {}
	virtual ~AntiBacterial() { DTORF("~AntiBacterial "); }
public:
	unsigned mix() { return delegate->mix() + volume_m3; }
	string idNvol() { return delegate->idNvol() + " AntiBacterial" + volAsStr(); }
};
class Hydrophilic : public Additive {
public:
	Hydrophilic(Polymer* delegate, unsigned volume_m3=0)
	  : Additive(delegate, volume_m3) {}
	virtual ~Hydrophilic() { DTORF("~Hydrophilic "); }
public:
	unsigned mix() { return delegate->mix() + volume_m3; }
	string idNvol() { return delegate->idNvol() + " Hydrophilic" + volAsStr(); }
};
// Seam point - add another option.
class MicroFibers : public Additive {
public:
	MicroFibers(Polymer* delegate, unsigned volume_m3=0)
	  : Additive(delegate, volume_m3) {}
	virtual ~MicroFibers() { DTORF("~MicroFibers "); }
public:
	unsigned mix() { return delegate->mix() + volume_m3; }
	string idNvol() { return delegate->idNvol() + " MicroFibers" + volAsStr(); }
};

Polymer* addAdditives(Polymer* additive, map<string,string>& order) {
	if(order.find("UVInhibiter") != order.end()) {
		additive = new UVInhibiter(additive, atoi(order["UVInhibiter"].c_str()));
	}
	if(order.find("AntiBacterial") != order.end()) {
		additive = new AntiBacterial(additive, atoi(order["AntiBacterial"].c_str()));
	}
	if(order.find("Hydrophilic") != order.end()) {
		additive = new Hydrophilic(additive, atoi(order["Hydrophilic"].c_str()));
	}
	// Seam point - add another option.
	if(order.find("MicroFibers") != order.end()) {
		additive = new MicroFibers(additive, atoi(order["MicroFibers"].c_str()));
	}

	return additive;
}

}

namespace factory_method {	// DP 5 - done except for inheriting from Observer.

class Packager {	// If the classes are varying...
public:
	Packager() {}
	virtual ~Packager() { DTORF("~factory_method::Packager\n"); }
public:
	virtual string wrap() { return "<packager>"; }
public:
	static Packager* makeObject(map<string,string>& map);
};
class Bulk : public Packager {
public:
	Bulk() {}
	~Bulk() { DTORF("~Bulk "); }
public:
	string wrap() { return "Bulk"; }
};
class ShrinkWrap : public Packager {
public:
	ShrinkWrap() {}
	~ShrinkWrap() { DTORF("~ShrinkWrap "); }
public:
	string wrap() { return "ShrinkWrap"; }
};
class HardPack : public Packager {
public:
	HardPack() {}
	~HardPack() { DTORF("~HardPack "); }
public:
	string wrap() { return "HardPack"; }
};
// Seam point - add another class.
class ZipLock : public Packager {
public:
	ZipLock() {}
	~ZipLock() { DTORF("~ZipLock "); }
public:
	string wrap() { return "ZipLock"; }
};

Packager* Packager::makeObject(map<string,string>& order) {
	if(		order["packager"] == "Bulk")		return new Bulk();
	else if(order["packager"] == "ShrinkWrap")	return new ShrinkWrap();
	else if(order["packager"] == "HardPack")	return new HardPack();
	// Seam point - add another class.
	else if(order["packager"] == "ZipLock")		return new ZipLock();

	else {
		legacy_classes::defaulting(order, "packager", "Bulk");
		return new Bulk();
	}
}

// Seam point - add another Observer.

class Stuffer {	// If the classes are varying...
public:
	Stuffer() {}
	virtual ~Stuffer() { DTORF("~factory_method::Stuffer\n"); }
public:
	virtual string fill() { return "<stuffer>"; }
public:
	static Stuffer* makeObject(map<string,string>& map);
};
class Air : public Stuffer {
public:
	Air() {}
	~Air() { DTORF("~Air "); }
public:
	string fill() { return "Air"; }
};
class Popcorn : public Stuffer {
public:
	Popcorn() {}
	~Popcorn() { DTORF("~Popcorn "); }
public:
	string fill() { return "styrene popcorn"; }
};
class Bubblewrap : public Stuffer {
public:
	Bubblewrap() {}
	~Bubblewrap() { DTORF("~Bubblewrap "); }
public:
	string fill() { return "bubble wrap"; }
};
// Seam point - add another class.
class Foam : public Stuffer {
public:
	Foam() {}
	~Foam() { DTORF("~Foam "); }
public:
	string fill() { return "expanding foam"; }
};

Stuffer* Stuffer::makeObject(map<string,string>& order) {
	if(		order["stuffer"] == "Air")	return new Air();
	else if(order["stuffer"] == "Popcorn")	return new Popcorn();
	else if(order["stuffer"] == "Bubblewrap")	return new Bubblewrap();
	// Seam point - add another class.
	else if(order["stuffer"] == "Foam")	return new Foam();

	else {
		legacy_classes::defaulting(order, "stuffer", "Air");
		return new Air();
	}
}

}

namespace template_method {	// DP 4.

using namespace adapter;
using namespace strategy;
using namespace factory_method;
using namespace decorator;
using namespace abstract_factory;

class ProcessOrder_TM {	// Template Method (order processing steps).
protected:
	string plasticDesc;
	unsigned colorVol;
private: // Heap objects.
	adapter::CleanMold*				cleaning;
	strategy::RuntimeEstimate*		runtimeEst;
	factory_method::Packager*		packager;
	factory_method::Stuffer*		cushion;	// Specs 2.
	decorator::TagCavity*			tagCavity;
	decorator::Additive*			additives;
	abstract_factory::InjectionLine*injectionLine;
	abstract_factory::IJM*			ijm;
	abstract_factory::Mold*			mold;
	abstract_factory::ConveyerBelt*	belt;
	abstract_factory::PackageBin*	bin;
public:
	ProcessOrder_TM(CleanMold* cleaning) :
		colorVol(0),
		// Heap objects to delete.
		cleaning(cleaning),
		runtimeEst(0),
		packager(0),
		cushion(0),	// Specs 2.
		tagCavity(0),
		additives(0),
		injectionLine(0),
		ijm(0),
		mold(0),
		belt(0),
		bin(0)
	{}
	virtual ~ProcessOrder_TM() {
		delete bin;
		delete belt;
		delete mold;
		delete ijm;
		delete injectionLine;
		delete additives;
		delete tagCavity;
		delete cushion;	// Specs 2.
		delete packager;
		delete runtimeEst;
		delete cleaning;
		DTORF("~ProcessOrder_TM\n");
	}
public:
	void run(map<string,string>& order) {	// Template Method (plastic).
		setupLine(order);		// AF (order size), Factory (packaging).
		getMold(order); 		// CofR (mold location), Bridge (shape, milling).
		insertTags(order);	 	// Decorator (tag list).
		loadBins(order);		// Becomes polymorphic on new (colored) plastics.
		loadAdditives(order);	// Decorator (additive list).
		runStats(order);		// Strategy (order size).
		cycleRecipe(order);		// TM polymorphic on plastic type.
		injectionCycle(order);	// Observer (bin full).
		cleanMold(order);		// Adapter (plastic, mold metal).
		// Seam point - add another constant step.
		ship(order);			// Factory (stuffing).
	}
protected:
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}

		injectionLine = InjectionLine::createInjectionLine(order);

		bin		= injectionLine->createPackageBin(order);			// Observer Subject.

		ijm		= injectionLine->createIJM(order);		// Observer.
		mold	= injectionLine->createMold(order);			// CofR, # of cavities.
		belt	= injectionLine->createConveyerBelt(order);	// Observer.

		packager = Packager::makeObject(order);	// FM & O.
		cushion = Stuffer::makeObject(order);	// FM & O - Specs 2.

		cout << "  Setup injection line for ";
		cout << order["size"] << " order";
		cout << " with " << packager->wrap() << " packager";
		// Seam line - add another observer machine (output bin stuffer).
		cout << " and " << cushion->fill() << " stuffer";
		cout << ":\n";

		cout << "    ";
		cout << ijm->setup() << " - ";
		cout << mold->setup() << " - ";
		cout << belt->setup() << " - ";
		cout << bin->setup() << ".\n";
	}
	void getMold(map<string,string>& order) { // CofR (mold location), Bridge (shape, milling platform).
		cout << "  Process order:\n";
		cout << "    Pull <shape> mold from <location>.\n";
	}
	void insertTags(map<string,string>& order) { // Decorator (tag list).
		tagCavity = new Blank();

		string list = "";
		if(order.find("tags") != order.end()) {
			list = order["tags"];
			tagCavity = TagCavity::addTags(tagCavity, list);
		}

		list = tagCavity->list();		// Strip trailing blank.
		int size = list.size();
		char tagString[size+1];
		strcpy(tagString, list.c_str());
		if(size) tagString[size-1] = '\0';

		cout << "    Insert tags [" << tagString << "]";
		cout << " of width "<< tagCavity->width_mm() << "/";
		cout << tagCavity->space_mm << " mm, blank tag is ";
		cout << tagCavity->computeBlankWidth(tagCavity->width_mm()) << " mm.\n";
	}
	// Seam point - convert a constant step into a polymorphic step.
	virtual void loadBins(map<string,string>& order) {	// Polymorphic on new (colored) plastics.
		unsigned shapeVol = 22;	// TODO: Replace with shape->volume_m3.
		if(order.find("color") != order.end())
			colorVol = 0.1*shapeVol;
		plasticDesc = order["plastic"];
		cout << "    Load plastic bin with " << plasticDesc;
		cout << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {	// Decorator (additive list).
		Polymer* plastic = new Plastic();

		Polymer* additives = addAdditives(plastic, order);

		string colorDesc = "";
		char str[80];
		if(colorVol) {
			sprintf(str, "(%d)", colorVol);
			colorDesc = string(" ") + order["color"] + str;
		}

		unsigned shapeVol = 35; // TODO: Get from shape->vol();
		unsigned cavities = 2;	// TODO: Get from mold->cavities();
		unsigned total = cavities*shapeVol;

		unsigned plasticVol = shapeVol - colorVol - additives->mix();

		cout << "      Recipe: " << plasticDesc << "(" << plasticVol << ")";
		cout << colorDesc << additives->idNvol() << " = 35.\n";

		cout << "      Volume: " << order["mold"] << "(" << shapeVol << ")";
		cout << " * " << cavities << " cavities";
		cout << " = " << total << " cc.\n";
	}
	void runStats(map<string,string>& order) { // Strategy (order size).
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/2;
		runtimeEst = RuntimeEstimate::selectEstimationAlgorithm(order);

		cout << "    Cycle <IJM> for " << order["plastic"] << " " << runSize << " times, ";
		cout << "estimated run time = " << (*runtimeEst)(order) << " hours.\n";
	}
	virtual void cycleRecipe(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "      Close - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void injectionCycle(map<string,string>& order) {	// Observer (bin full).
		cout << "      When <packageBin> full, paused <IJM/Conveyer/OptionalPackager> while stuffed with <stuffer> packing & swapped.\n";
	}
	void cleanMold(map<string,string>& order) {	// Adapter (plastic, mold metal).
		cleaning->clean(order["metal"]);
	}
	// Seam point - add another constant step.
	void ship(map<string,string>& order) {	// Factory (stuffing).
		cout << "    Ship to \"" << order["address"] << "\".\n";
	}
public:
	static ProcessOrder_TM* getOrderProcess(map<string,string>& order);
};
class ABS : public ProcessOrder_TM {	// If the plastics are varying...
public:
	ABS() : ProcessOrder_TM(new adapter::ABS()) {}
	~ABS() { DTORF("~ABS\n"); }
public:
	virtual void cycleRecipe(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "      Close - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject.\n";
	}
};
class Poly : public ProcessOrder_TM {
public:
	Poly() : ProcessOrder_TM(new adapter::Poly()) {}
	~Poly() { DTORF("~Poly\n"); }
public:
	virtual void cycleRecipe(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "      Close - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject.\n";
	}
};
class PET : public ProcessOrder_TM {
public:
	PET() : ProcessOrder_TM(new adapter::PET()) {}
	~PET() { DTORF("~PET\n"); }
public:
	virtual void cycleRecipe(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "      Close - heat to 404 - inject at 110 PSI - cool to 340 - separate - smooth eject.\n";
	}
};
// Seam point - add another polymorphic step (not in this version).
class Synthetic : public ProcessOrder_TM {	// If the plastics are varying...
public:
	Synthetic() : ProcessOrder_TM(new adapter::Synthetic()) {}
	~Synthetic() { DTORF("~Synthetic\n"); }
public:
	// Seam point - convert a constant step into a polymorphic step.
	virtual void loadBins(map<string,string>& order) {
		colorVol = 0;
		plasticDesc = order["color"] + string("-") + order["plastic"];
		cout << "    Load plastic bin with " << plasticDesc << " pellets.\n";
	}
	virtual void cycleRecipe(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "      Close - heat to 480 - inject at 150 PSI - cool to 390 - separate - shock eject.\n";
	}
};

ProcessOrder_TM* ProcessOrder_TM::getOrderProcess(map<string,string>& order) {
	if(		order["plastic"] == "ABS")				return new ABS;
	else if(order["plastic"] == "Polypropylene")	return new Poly;
	else if(order["plastic"] == "Polyethelene")		return new Poly;
	else if(order["plastic"] == "PET")				return new PET;
	// Seam point - add another plastic.
	else if(order["plastic"] == "Styrene")			return new Synthetic;
	else if(order["plastic"] == "Nylon66")			return new Synthetic;

	else {
		legacy_classes::defaulting(order, "plastic", "ABS");
		return new ABS;
	}
}

}

void process(map<string,string>& order) {
	using namespace legacy_classes;
	using namespace template_method;

	ProcessOrder_TM* processOrder = ProcessOrder_TM::getOrderProcess(order);

	processOrder->run(order);

	delete processOrder;
}

pair<string,string> parse(string line) {
	char key[83];
	char val[83] = {0};

	sscanf(line.c_str(), "%s", key);

	char* equalSign = strchr(line.c_str(), '=');
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
	cout << "<<< final solution >>>\n";

	FILE* orderFilePtr = fopen(orderFile.c_str(), "r");

	for(;;) {	// Get and process each order.
		map<string,string> order = getCompleteOrder(orderFilePtr);
		if(order.size() == 0)
			break;
		process(order);
	}
}

} // final_solution

#endif /* FINAL_FINAL_SOLUTION_H_ */

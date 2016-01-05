/*
 * finalGuided.h - Part 1: finalOriginal.h  Part 2: finalSolution.h
 *
 *  Created on: <MMDDCCYY>
 *      Author: <studentName>
 */

#ifndef FINAL_GUIDED_H_
#define FINAL_GUIDED_H_

namespace final_guided {

/* Instruction Steps:
 * 1. Have fully folded
 * 2. Unfold legacy_classes namespace
 * 3. Unfold adapter namespace
 * 4. Unfold template_method namespace
 */

const bool on = true;	// Useful macro to turn dtor instrumentation on/off.
#define DTORF(x) if(on) { cout << x; }

namespace legacy_classes {	// Can't change these.

/* Instruction Steps:
 * 1. Unfold clean classes
 * 2. Unfold their method(s)
 */

int cycleTime_sec(string metal, int volume_cc) {
	int heatCapacityAdjustment = 0;

	if(		metal == "Aluminum")	heatCapacityAdjustment = -2;
	else if(metal == "Steel")		heatCapacityAdjustment = +5;

	return 20 + heatCapacityAdjustment + 0.1*volume_cc;
}

const int setupAvg_min = 118;
const int teardownAvg_min = 54;
const float rejectRateLow_pcnt = 0.4;
const float rejectRateHigh_pcnt = 1.2;

class CleanABS {
public: ~CleanABS() { DTORF("~legacy_classes::CleanABS "); }
	void clean() {
		cout << "  Clean mold of ABS: soak in alcohol, rinse with water, dry.\n";
	}
};

class CleanPolys {
public: ~CleanPolys() { DTORF("~legacy_classes::CleanPolys "); }
	void prepForReuse() {
		cout << "  Clean mold of Poly: rinse with acetone, dry.\n";
	}
};

class CleanupPET {
public: ~CleanupPET() { DTORF("~legacy_classes::CleanupPET "); }
	void carbonize() { // Use only on stainless steel.
		cout << "  Clean Steel mold of PET: heat to 895 C.\n";
	}
	void purify() {	// Use only on aluminum.
		cout << "  Clean Aluminum mold of PET: rinse with soap and water, dry.\n";
	}
};

void defaulting(map<string,string>& order, const string& option, string def="simulation") {
	cout << "  <>Unknown " << option;
	cout << " |" << order[option] << "|";
	order[option] = def;
	cout << " defaulting to " << order[option] << ".\n";
}

}

namespace adapter {			// DP 2.

/* Instruction Steps:
 * 1. Unfold Base class
 *    1.1. Why pass metal to the clean method?
 * 2. Unfold ABS class
 *    2.1. Unfold clean method
 * 3. Unfold Poly class
 * 4. Unfold the Factory Method
 * 5. Students complete the classes
 * 6. Unfold the rest.
 */

class CleanMold {
public:	virtual ~CleanMold() { DTORF("~adapter::CleanMold\n"); }
public:
	virtual void clean(const string& metal) {
		cout << "  Clean mold of <>...\n";
	}
public:
	static CleanMold*getCleaning(map<string,string>& order);
};
class ABSClean : public CleanMold {
	legacy_classes::CleanABS plastic;
public: ~ABSClean() { DTORF("~ABSClean "); }
	void clean(const string& metal) {
		plastic.clean();
	}
};
class PolyClean : public CleanMold {
	legacy_classes::CleanPolys plastic;
public: ~PolyClean() { DTORF("~PolyClean "); }
	void clean(const string& metal) {
		plastic.prepForReuse();
	}
};
class PETClean : public CleanMold {
	legacy_classes::CleanupPET plastic;
public: ~PETClean() { DTORF("~PETClean "); }
	void clean(const string& metal) {
		if(metal == "Steel")
			plastic.carbonize();
		else
			plastic.purify();
	}
};
// Seam point - add another interface.

CleanMold* CleanMold::getCleaning(map<string,string>& order) {	// FM.
	if(order["plastic"] == "ABS")				return new ABSClean;
	if(order["plastic"] == "Polypropylene")		return new PolyClean;
	if(order["plastic"] == "Polyethelene")		return new PolyClean;
	if(order["plastic"] == "PET")				return new PETClean;
	// Seam point - add another plastic.

	return new CleanMold;
}

} // adapter

namespace strategy {		// DP 1.

class RuntimeEstimate {	// If the algorithms are varying...
protected:
	int orderSize;
	int cavities;
	int cycleTime; // Seconds.
public:
	RuntimeEstimate()
		: orderSize(0)
		, cavities(0)
		, cycleTime(0)
	{}
	virtual ~RuntimeEstimate() { DTORF("~strategy::RuntimeEstimate\n"); }
protected:
	void runTimeEst_hrs(map<string,string>& order) {
		orderSize = atoi(order["size"].c_str());
		cavities = atoi(order["cavities"].c_str());	// Added to order by TM.runtimeEstimate().
		if(cavities <= 0)	cavities = 1;
	}
public:
	virtual int operator()(map<string,string>& order) {
		return 999;
	}
	virtual string formula() {
		return "F(x,y,z)";
	}
	virtual string values() {
		stringstream value;
		value << 10000 << "*" << 60 << "/" << 3600 << ".\n";
		return value.str();
	}
	virtual string name() {
		return "algorithm";
	}
public:
	static RuntimeEstimate* selectEstimationAlgorithm(map<string,string>& order);
};
class BackOfTheEnvelope : public RuntimeEstimate {
public:	~BackOfTheEnvelope() { DTORF("~BackOfTheEnvelope "); }
public:
	int operator()(map<string,string>& order) {
		RuntimeEstimate::runTimeEst_hrs(order);
		cycleTime = 39;
		return (orderSize/cavities)*cycleTime/3600;
	}
	string formula() {
		return "(orderSize/cavities)*cycleTime/3600";
	}
	string values() {
		stringstream value;
		value << "(" << orderSize/cavities << ")*" << cycleTime << "/" << 3600;
		return value.str();
	}
	virtual string name() {
		return "BackOfTheEnvelope";
	}
};
class Calculation : public RuntimeEstimate {
	string	metal;
	int 	volume_cc;
public:
	Calculation() : volume_cc(0) {}
	~Calculation() { DTORF("~Calculation "); }
public:
	int operator()(map<string,string>& order) {
		RuntimeEstimate::runTimeEst_hrs(order);
		metal = order["metal"];	// Added to order by TM.getMold().
		volume_cc = atoi(order["volume"].c_str());	// Added to order by TM.?().
		cycleTime = legacy_classes::cycleTime_sec(metal, volume_cc);
		return (orderSize/cavities)*cycleTime/3600;
	}
	string formula() {
		return "(orderSize/cavities)*cycleTime_sec(metal, volume_cc)/3600";
	}
	string values() {
		stringstream value;
		value << "(" << orderSize/cavities << ")*"
			  << cycleTime << "(" << metal << "," << volume_cc << ")"
			  << "/" << 3600;
		return value.str();
	}
	virtual string name() {
		return "Calculation";
	}
};
class Historical : public RuntimeEstimate {
	string	metal;
	int 	volume_cc;
public:
	Historical() : volume_cc(0) {}
	~Historical() { DTORF("~Historical "); }
public:
	int operator()(map<string,string>& order) {
		using namespace legacy_classes;
		RuntimeEstimate::runTimeEst_hrs(order);
		metal = order["metal"];	// Added to order by TM.getMold().
		volume_cc = atoi(order["volume"].c_str());	// Added to order by TM.?().
		cycleTime = cycleTime_sec(metal, volume_cc);
		return (setupAvg_min + (orderSize/cavities)*cycleTime/60 + teardownAvg_min)/60;
	}
	string formula() {
		return "(setupAvg_min + (orderSize/cavities)*cycleTime_sec(metal, volume_cc)/60 + teardownAvg_min)/60";
	}
	string values() {
		using namespace legacy_classes;
		stringstream value;
		value << "(" << setupAvg_min << " + (" << orderSize/cavities << ")*"
			  << cycleTime << "(" << metal << "," << volume_cc << ")/" << 60
			  << " + " << teardownAvg_min << ")/60";
		return value.str();
	}
	virtual string name() {
		return "Historical";
	}
};
// Seam point - add another algorithm.

RuntimeEstimate* RuntimeEstimate::selectEstimationAlgorithm(map<string,string>& order) {
	int size = atoi(order["size"].c_str());

	if(size <=  10000)	return new BackOfTheEnvelope;
	if(size <=  30000)	return new Calculation;
	if(size <= 150000)	return new Historical;
	// Seam point - add another runtime estimation algorithm.

	cout << "    <>Using most sophisticated estimation algorithm";
	cout << " for orders greater than 150000.\n";
	return new Historical;
}

} // strategy

namespace observer {		// DP 5.

class BinSubject;
class BinObserver;

class BinSubject {
public:
	const string name;
private:
	list<BinObserver*>	obs;
public:
	BinSubject(const string name="needName") : name(name) {}
	~BinSubject();
public:
	void attach(BinObserver* observer) { obs.push_back(observer); }
	void detach(BinObserver* observer) { obs.remove(observer); }
	void pause();
};

class BinObserver {
	BinSubject* bin;
public:
	BinObserver(BinSubject* bin) : bin(bin) {
		bin->attach(this);
	}
	virtual ~BinObserver() {
		bin->detach(this);
		DTORF("~observer::BinObserver\n");
	}
public:
	virtual void update(BinSubject* bin=0) {}
};
// Seam point - add another listener (in other namespaces; the listeners are base classes).

BinSubject::~BinSubject() {
	DTORF("~observer::BinSubject\n");
	DTORF(" BinObservers left to process (should be zero) = ");
	DTORF(obs.size());
	DTORF(".\n");
}
void BinSubject::pause() {
	cout << "      Whenever " << name << " parts bin was full...\n";
	list<BinObserver*>::iterator it;
	for(it=obs.begin(); it!=obs.end(); ++it) {
		(*it)->update(this);
	}
}

} // observer

namespace factory_method {	// DP 4.

using namespace observer;

class Packager : public BinObserver {	// If the classes are varying...
public:
	Packager(BinSubject* bin) : BinObserver(bin) {}
	virtual ~Packager() { DTORF("~factory_method::Packager "); }
public:
	virtual string wrap() { return "<packager>"; }
public:
	virtual void update(BinSubject* bin) {}
public:
	static Packager* makeObject(map<string,string>& map, BinSubject* bin);
};
// class Bulk is the null class.
class ShrinkWrap : public Packager {
public:
	ShrinkWrap(BinSubject* bin) : Packager(bin) {}
	~ShrinkWrap() { DTORF("~ShrinkWrap "); }
public:
	string wrap() { return "ShrinkWrap"; }
public:
	void update(BinSubject* bin) {
		cout << "        Shrink wrap packager paused while ";
		cout << bin->name << " parts bin was swapped.\n";
	}
};
class HardPack : public Packager {
public:
	HardPack(BinSubject* bin) : Packager(bin) {}
	~HardPack() { DTORF("~HardPack "); }
public:
	string wrap() { return "HardPack"; }
public:
	void update(BinSubject* bin) {
		cout << "        Hard pack packager paused while ";
		cout << bin->name << " parts bin was swapped.\n";
	}
};
// Seam point - add another class.

Packager* Packager::makeObject(map<string,string>& order, BinSubject* bin) {
	if(order["packager"] == "Bulk")			return 0;
	if(order["packager"] == "ShrinkWrap")	return new ShrinkWrap(bin);
	if(order["packager"] == "HardPack")		return new HardPack(bin);
	// Seam point - add another class.

	legacy_classes::defaulting(order, "packager", "None");
	return 0;
}

} // factory_method

namespace abstract_factory1 {// DP 9.

class IJM {
public:
	IJM() {}
	virtual ~IJM() { DTORF("~IJM\n"); }
public:
	virtual string setup() { return "IJM_base"; }
};
class IJM_110 : public IJM {					// PilotOrder.
public:
	IJM_110() {}
	~IJM_110() { DTORF("~IJM_110 "); }
public:
	string setup() { return "IJM_110"; }
};
class IJM_120 : public IJM {					// SmallOrder.
public:
	IJM_120() {}
	~IJM_120() { DTORF("~IJM_120 "); }
public:
	string setup() { return "IJM_120"; }
};
class IJM_210 : public IJM {					// MediumOrder.
public:
	IJM_210() {}
	~IJM_210() { DTORF("~IJM_210 "); }
public:
	string setup() { return "IJM_210"; }
};
class IJM_220 : public IJM {					// LargeOrder.
public:
	IJM_220() {}
	~IJM_220() { DTORF("~IJM_220 "); }
public:
	string setup() { return "IJM_220"; }
};
// Seam point - add another type 1.

class Block {
public:
	const unsigned cavities;
public:
	Block(unsigned cavities) : cavities(cavities) {}
	virtual ~Block() { DTORF("~Block\n"); }
public:
	string setup() { return metal() + cavitiesAsString(); }
	virtual string metal() { return "metal"; }
	string cavitiesAsString() {
		char cav[] = "( )";
		cav[1] = cavities + '0';
		return string(cav);
	}
};
class Aluminum : public Block {
public:
	Aluminum(unsigned cavities) : Block(cavities) {}
	~Aluminum() { DTORF("~Aluminum "); }
public:
	string metal() { return "Aluminum"; }
};
class Steel : public Block {
public:
	Steel(unsigned cavities) : Block(cavities) {}
	~Steel() { DTORF("~Steel "); }
public:
	string metal() { return "Steel"; }
};
// Seam point - add another type 2.

class ConveyerBelt {
public:
	ConveyerBelt() {}
	virtual ~ConveyerBelt() { DTORF("~ConveyerBelt\n"); }
public:
	virtual string setup() { return "Base Conveyer belt"; }
};
class LinearBelt : public ConveyerBelt {
public:
	LinearBelt() {}
	~LinearBelt() { DTORF("~LinearBelt "); }
public:
	string setup() {return "Linear conveyer belt"; }
};
class YSplitBelt : public ConveyerBelt {
public:
	YSplitBelt() {}
	~YSplitBelt() { DTORF("~YSplitBelt "); }
public:
	string setup() {return "Y-Split conveyer belt"; }
};
// Seam point - add another type 3.

class PartsBin {
public:
	PartsBin() {}
	virtual ~PartsBin() { DTORF("~PartsBin\n"); }
public:
	virtual string setup() { return "PartsBin_base"; }
};
class CardboardBox : public PartsBin {
public:
	CardboardBox() {}
	~CardboardBox() { DTORF("~CardboardBox "); }
public:
	string setup() { return "CardboardBox"; }
};
class ShellBox : public PartsBin {
public:
	ShellBox() {}
	~ShellBox() { DTORF("~ShellBox "); }
public:
	string setup() { return "ShellBox"; }
};
class PalletBox : public PartsBin {
public:
	PalletBox() {}
	~PalletBox() { DTORF("~PalletBox "); }
public:
	string setup() { return "PalletBox"; }
};
// Seam point - add another type N.

class InjectionLine {	// If the families are varying...
public:
	InjectionLine() {}
	virtual ~InjectionLine() { DTORF("~abstract_factory::InjectionLine\n"); }
public:
	virtual IJM* createIJM(map<string,string>& order) {
		return new IJM;
	}
	virtual Block* createBlock(map<string,string>& order) {
		return new Block(0);
	}
	virtual ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new ConveyerBelt;
	}
	virtual PartsBin* createPartsBin(map<string,string>& order) {
		return new PartsBin;
	}
public:
	static InjectionLine* createInjectionLine(map<string,string>& order);
};
class PilotOrder : public InjectionLine {
public:	virtual ~PilotOrder() { DTORF("~PilotOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_110;
	}
	Block* createBlock(map<string,string>& order) {
		return new Aluminum(1);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new LinearBelt;
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new CardboardBox;
	}
};
class SmallOrder : public InjectionLine {
public:	virtual ~SmallOrder() { DTORF("~SmallOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_120;
	}
	Block* createBlock(map<string,string>& order) {
		return new Aluminum(2);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new YSplitBelt;
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new ShellBox;
	}
};
class MediumOrder : public InjectionLine {
public:	virtual ~MediumOrder() { DTORF("~MediumOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_210;
	}
	Block* createBlock(map<string,string>& order) {
		return new Steel(1);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new LinearBelt;
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new PalletBox;
	}
};
class LargeOrder : public InjectionLine {
public:	virtual ~LargeOrder() { DTORF("~LargeOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_220;
	}
	Block* createBlock(map<string,string>& order) {
		return new Steel(2);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new YSplitBelt;
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new PalletBox;
	}
};

// Seam point - add another family.

InjectionLine* InjectionLine::createInjectionLine(map<string,string>& order) {
	unsigned size = atoi(order["size"].c_str());

	if(size <=  10000)		return new PilotOrder;
	if(size <=  20000)		return new SmallOrder;
	if(size <=  50000)		return new MediumOrder;
	if(size <= 100000)		return new LargeOrder;
	// Seam point - add another order size.

	return new InjectionLine;
}

} // abstract_factory1
namespace abstract_factory2 {// Deprecated.

using namespace observer;

class IJM : public BinObserver {
public:
	IJM(BinSubject* bin) : BinObserver(bin) {}
	virtual ~IJM() { DTORF("~IJM "); }
public:
	void update(BinSubject* bin) {
		cout << "        " << setup() << " paused while ";
		cout << bin->name << " parts bin was swapped.\n";
	}
	virtual string setup() { return "IJM base"; }
};
class IJM_110 : public IJM {					// PilotOrder.
public:
	IJM_110(BinSubject* bin) : IJM(bin) {}
	~IJM_110() { DTORF("~IJM_110 "); }
public:
	string setup() { return "IJM_110"; }
};
class IJM_120 : public IJM {					// SmallOrder.
public:
	IJM_120(BinSubject* bin) : IJM(bin) {}
	~IJM_120() { DTORF("~IJM_120 "); }
public:
	string setup() { return "IJM_120"; }
};
class IJM_210 : public IJM {					// MediumOrder.
public:
	IJM_210(BinSubject* bin) : IJM(bin) {}
	~IJM_210() { DTORF("~IJM_210 "); }
public:
	string setup() { return "IJM_210"; }
};
class IJM_220 : public IJM {					// LargeOrder.
public:
	IJM_220(BinSubject* bin) : IJM(bin) {}
	~IJM_220() { DTORF("~IJM_220 "); }
public:
	string setup() { return "IJM_220"; }
};
// Seam point - add another Injection Molding machine.

class Mold {
public:
	const unsigned cavities;
public:
	Mold(unsigned cavities) : cavities(cavities) {}
	virtual ~Mold() { DTORF("~Mold\n"); }
public:
	string setup() { return metal() + cavitiesAsString(); }
	virtual string metal() { return "metal"; }
	string cavitiesAsString() {
		char cav[] = "( )";
		cav[1] = cavities + '0';
		return string(cav);
	}
};
class Aluminum : public Mold {
public:
	Aluminum(unsigned cavities) : Mold(cavities) {}
	~Aluminum() { DTORF("~Aluminum "); }
public:
	string metal() { return "Aluminum"; }
};
class Steel : public Mold {
public:
	Steel(unsigned cavities) : Mold(cavities) {}
	~Steel() { DTORF("~Steel "); }
public:
	string metal() { return "Steel"; }
};

class ConveyerBelt : public BinObserver {
public:
	ConveyerBelt(BinSubject* bin) : BinObserver(bin) {}
	virtual ~ConveyerBelt() { DTORF("~ConveyerBelt "); }
public:
	virtual string setup() { return "ConveyerBelt base"; }
public:
	void update(BinSubject* bin) {
		cout << "        " << setup() << " paused while ";
		cout << bin->name << " parts bin was swapped.\n";
	}
};
class LinearBelt : public ConveyerBelt {
public:
	LinearBelt(BinSubject* bin) : ConveyerBelt(bin) {}
	~LinearBelt() { DTORF("~LinearBelt "); }
public:
	string setup() {return "Linear conveyer belt"; }
};
class YSplitBelt : public ConveyerBelt {
public:
	YSplitBelt(BinSubject* bin) : ConveyerBelt(bin) {}
	~YSplitBelt() { DTORF("~YSplitBelt "); }
public:
	string setup() {return "Y-Split conveyer belt"; }
};
// Seam point - add another conveyer belt.

class PartsBin : public BinSubject {
public:
	PartsBin(const string name) : BinSubject(name) {}
	virtual ~PartsBin() { DTORF("~PartsBin "); }
public:
	virtual string setup() { return "PartsBin base"; }
};
class CardboardBox : public PartsBin {
public:
	CardboardBox() : PartsBin("CardboardBox") {}
	~CardboardBox() { DTORF("~CardboardBox "); }
public:
	string setup() { return "CardboardBox"; }
};
class ShellBox : public PartsBin {
public:
	ShellBox() : PartsBin("ShellBox") {}
	~ShellBox() { DTORF("~ShellBox "); }
public:
	string setup() { return "ShellBox"; }
};
class PalletBox : public PartsBin {
public:
	PalletBox() : PartsBin("PalletBox") {}
	~PalletBox() { DTORF("~PalletBox "); }
public:
	string setup() { return "PalletBox"; }
};
// Seam point - add another parts bin.

class InjectionLine {	// If the families are varying...
public:
	InjectionLine() {}
	virtual ~InjectionLine() { DTORF("~abstract_factory::InjectionLine\n"); }
public:
	virtual IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM(bin);
	}
	virtual Mold* createMold(map<string,string>& order) {
		return new Mold(0);
	}
	virtual ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new ConveyerBelt(bin);
	}
	virtual PartsBin* createPartsBin(map<string,string>& order) {
		return new PartsBin("needName");
	}
public:
	static InjectionLine* createInjectionLine(map<string,string>& order);
};
class PilotOrder : public InjectionLine {
public:	virtual ~PilotOrder() { DTORF("~PilotOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_110(bin);
	}
	Mold* createMold(map<string,string>& order) {
		return new Aluminum(1);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new LinearBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new CardboardBox();
	}
};
class SmallOrder : public InjectionLine {
public:	virtual ~SmallOrder() { DTORF("~SmallOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_120(bin);
	}
	Mold* createMold(map<string,string>& order) {
		return new Aluminum(2);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new YSplitBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new ShellBox();
	}
};
class MediumOrder : public InjectionLine {
public:	virtual ~MediumOrder() { DTORF("~MediumOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_210(bin);
	}
	Mold* createMold(map<string,string>& order) {
		return new Steel(1);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new LinearBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new PalletBox();
	}
};
class LargeOrder : public InjectionLine {
public:	virtual ~LargeOrder() { DTORF("~LargeOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_220(bin);
	}
	Mold* createMold(map<string,string>& order) {
		return new Steel(2);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new YSplitBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new PalletBox();
	}
};
// Seam point - add another family.

InjectionLine* InjectionLine::createInjectionLine(map<string,string>& order) {
	unsigned size = atoi(order["size"].c_str());

	if(size <=  10000)		return new PilotOrder;
	if(size <=  20000)		return new SmallOrder;
	if(size <=  50000)		return new MediumOrder;
	if(size <= 100000)		return new LargeOrder;
	// Seam point - add another order size.

	return new InjectionLine;
}

} // abstract_factory2
namespace abstract_factory3 {// DP 9.

using namespace observer;

class IJM : public BinObserver {
public:
	IJM(BinSubject* bin) : BinObserver(bin) {}
	virtual ~IJM() { DTORF("~IJM "); }
public:
	void update(BinSubject* bin) {
		cout << "        " << setup() << " paused while ";
		cout << bin->name << " parts bin was swapped.\n";
	}
	virtual string setup() { return "IJM base"; }
};
class IJM_110 : public IJM {					// PilotOrder.
public:
	IJM_110(BinSubject* bin) : IJM(bin) {}
	~IJM_110() { DTORF("~IJM_110 "); }
public:
	string setup() { return "IJM_110"; }
};
class IJM_120 : public IJM {					// SmallOrder.
public:
	IJM_120(BinSubject* bin) : IJM(bin) {}
	~IJM_120() { DTORF("~IJM_120 "); }
public:
	string setup() { return "IJM_120"; }
};
class IJM_210 : public IJM {					// MediumOrder.
public:
	IJM_210(BinSubject* bin) : IJM(bin) {}
	~IJM_210() { DTORF("~IJM_210 "); }
public:
	string setup() { return "IJM_210"; }
};
class IJM_220 : public IJM {					// LargeOrder.
public:
	IJM_220(BinSubject* bin) : IJM(bin) {}
	~IJM_220() { DTORF("~IJM_220 "); }
public:
	string setup() { return "IJM_220"; }
};
// Seam point - add another Injection Molding machine.

class Block {
public:
	const unsigned cavities;
public:
	Block(unsigned cavities) : cavities(cavities) {}
	virtual ~Block() { DTORF("~Block\n"); }
public:
	string setup() { return metal() + cavitiesAsString(); }
	virtual string metal() { return "metal"; }
	string cavitiesAsString() {
		char cav[] = "( )";
		cav[1] = cavities + '0';
		return string(cav);
	}
};
class Aluminum : public Block {
public:
	Aluminum(unsigned cavities) : Block(cavities) {}
	~Aluminum() { DTORF("~Aluminum "); }
public:
	string metal() { return "Aluminum"; }
};
class Steel : public Block {
public:
	Steel(unsigned cavities) : Block(cavities) {}
	~Steel() { DTORF("~Steel "); }
public:
	string metal() { return "Steel"; }
};

class ConveyerBelt : public BinObserver {
public:
	ConveyerBelt(BinSubject* bin) : BinObserver(bin) {}
	virtual ~ConveyerBelt() { DTORF("~ConveyerBelt "); }
public:
	virtual string setup() { return "ConveyerBelt base"; }
public:
	void update(BinSubject* bin) {
		cout << "        " << setup() << " paused while ";
		cout << bin->name << " parts bin was swapped.\n";
	}
};
class LinearBelt : public ConveyerBelt {
public:
	LinearBelt(BinSubject* bin) : ConveyerBelt(bin) {}
	~LinearBelt() { DTORF("~LinearBelt "); }
public:
	string setup() {return "Linear conveyer belt"; }
};
class YSplitBelt : public ConveyerBelt {
public:
	YSplitBelt(BinSubject* bin) : ConveyerBelt(bin) {}
	~YSplitBelt() { DTORF("~YSplitBelt "); }
public:
	string setup() {return "Y-Split conveyer belt"; }
};
// Seam point - add another conveyer belt.

class PartsBin : public BinSubject {
public:
	PartsBin(const string name) : BinSubject(name) {}
	virtual ~PartsBin() { DTORF("~PartsBin "); }
public:
	virtual string setup() { return "PartsBin base"; }
};
class CardboardBox : public PartsBin {
public:
	CardboardBox() : PartsBin("CardboardBox") {}
	~CardboardBox() { DTORF("~CardboardBox "); }
public:
	string setup() { return "CardboardBox"; }
};
class ShellBox : public PartsBin {
public:
	ShellBox() : PartsBin("ShellBox") {}
	~ShellBox() { DTORF("~ShellBox "); }
public:
	string setup() { return "ShellBox"; }
};
class PalletBox : public PartsBin {
public:
	PalletBox() : PartsBin("PalletBox") {}
	~PalletBox() { DTORF("~PalletBox "); }
public:
	string setup() { return "PalletBox"; }
};
// Seam point - add another parts bin.

class InjectionLine {	// If the families are varying...
public:
	InjectionLine() {}
	virtual ~InjectionLine() { DTORF("~abstract_factory::InjectionLine\n"); }
public:
	virtual IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM(bin);
	}
	virtual Block* createBlock(map<string,string>& order) {
		return new Block(0);
	}
	virtual ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new ConveyerBelt(bin);
	}
	virtual PartsBin* createPartsBin(map<string,string>& order) {
		return new PartsBin("needName");
	}
public:
	static InjectionLine* createInjectionLine(map<string,string>& order);
};
class PilotOrder : public InjectionLine {
public:	virtual ~PilotOrder() { DTORF("~PilotOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_110(bin);
	}
	Block* createBlock(map<string,string>& order) {
		return new Aluminum(1);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new LinearBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new CardboardBox();
	}
};
class SmallOrder : public InjectionLine {
public:	virtual ~SmallOrder() { DTORF("~SmallOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_120(bin);
	}
	Block* createBlock(map<string,string>& order) {
		return new Aluminum(2);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new YSplitBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new ShellBox();
	}
};
class MediumOrder : public InjectionLine {
public:	virtual ~MediumOrder() { DTORF("~MediumOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_210(bin);
	}
	Block* createBlock(map<string,string>& order) {
		return new Steel(1);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new LinearBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new PalletBox();
	}
};
class LargeOrder : public InjectionLine {
public:	virtual ~LargeOrder() { DTORF("~LargeOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_220(bin);
	}
	Block* createBlock(map<string,string>& order) {
		return new Steel(2);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new YSplitBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new PalletBox();
	}
};
// Seam point - add another family.

InjectionLine* InjectionLine::createInjectionLine(map<string,string>& order) {
	unsigned size = atoi(order["size"].c_str());

	if(size <=  10000)		return new PilotOrder;
	if(size <=  20000)		return new SmallOrder;
	if(size <=  50000)		return new MediumOrder;
	if(size <= 100000)		return new LargeOrder;
	// Seam point - add another order size.

	return new InjectionLine;
}

} // abstract_factory3

namespace bridge {			// DP 8.

class Platform { // Implementation class in Bridge pattern.
public: virtual ~Platform() { DTORF("~bridge::Platform\n"); }
public:
	virtual string name() { return "PlatformName"; }
	virtual string drill() { return "drill"; }
	virtual string cut() { return "cut"; }
	virtual string grind() { return "grind"; }
public:
	static Platform* getPlatform(map<string,string>& order);
};
class HighCarbon : public Platform {
public: virtual ~HighCarbon() { DTORF("~HighCarbon "); }
public:
	string name() { return "HighCarbon"; }
	string drill() { return "drill"; }
	string cut() { return "cut"; }
	string grind() { return "high speed grind"; }
public:
	static Platform* getPlatform(map<string,string>& order);
};
class Carbide : public Platform {
public: virtual ~Carbide() { DTORF("~Carbide "); }
public:
	string name() { return "Carbide"; }
	string drill() { return "high speed drill"; }
	string cut() { return "cross cut"; }
	string grind() { return "layer grind"; }
public:
	static Platform* getPlatform(map<string,string>& order);
};
class DiamondTipped : public Platform {
public: virtual ~DiamondTipped() { DTORF("~DiamondTipped "); }
public:
	string name() { return "DiamondTipped"; }
	string drill() { return "precision drill"; }
	string cut() { return "oil cooled cut"; }
	string grind() { return "cartoid grind"; }
public:
	static Platform* getPlatform(map<string,string>& order);
};
// Seam Point - add another implementation.

Platform* Platform::getPlatform(map<string,string>& order) {
	string metal = order["metal"];	// Added to order by TM.setupLine().
	string finish = order["finish"];

	if(metal == "Aluminum")						return new HighCarbon;
	if(metal == "Steel" && finish == "smooth")	return new HighCarbon;
	if(metal == "Steel" && finish == "rippled")	return new Carbide;
	if(metal == "Steel" && finish == "dimpled")	return new DiamondTipped;
	// Seam Point - add another implementation.

	cout << "    <>Unknown metal |" << metal << "| or finish |" << finish << "|.";
	order["metal"]  = "Steel";
	order["finish"] = "smooth";
	cout << " Defaulting to " << order["metal"] << " and " << order["finish"];
	cout << " with HighCarbon platform.\n";
	return new HighCarbon;
}

class Shape { // Abstraction class in Bridge pattern.
protected:
	Platform* platform;
public:
	const string name;
	const unsigned volume_cc;
public:
	Shape(Platform* platform, string name, unsigned volume_cc=0)
	  : platform(platform), name(name), volume_cc(volume_cc) {}
	virtual ~Shape() {
		if(platform) { DTORF("~bridge::Shape "); }
		else {		   DTORF("~bridge::Shape\n"); }
		delete platform;
	}
public:
	void mill(map<string,string>& order) {
		cout << "      using " << platform->name() << " tools (";
		cout << platform->drill() << ", ";
		cout << platform->cut() << ", and ";
		cout << platform->grind() << ")\n";
		cout << "      to mill " << order["metal"] << " block into ";
		string shape_s = order["cavities"]=="1" ? " shape " : " shapes ";
		cout << order["cavities"] << " " << name << shape_s;	// Added to order by TM.setupLine().
		cout << "with " << order["finish"] << " finish.\n";
	}
	virtual void steps() {
		cout << "steps: specify drill, cut and grind steps.\n";
	}
public:
	static Shape* getShape(map<string,string>& order);
};
class Duck : public Shape {
public:
	Duck(Platform* platform) : Shape(platform, "duck", 35) {};
	virtual ~Duck() { DTORF("~Duck "); }
public:
	virtual void steps() {
		cout << "steps: D2C4G1\n";
	}
};
class Car : public Shape {
public:
	Car(Platform* platform) : Shape(platform, "car", 40) {};
	virtual ~Car() { DTORF("~Car "); }
public:
	virtual void steps() {
		cout << "steps: D2C8D1G3\n";
	}
};
class Hero : public Shape {
public:
	Hero(Platform* platform) : Shape(platform, "hero", 50) {};
	virtual ~Hero() { DTORF("~Hero "); }
public:
	virtual void steps() {
		cout << "steps: D1C1D1G7\n";
	}
};
// Seam Point - add another abstraction.

Shape* Shape::getShape(map<string,string>& order) {
	Platform* platform = 0;
	if(order["moldLoc"] == "mill")
		platform = Platform::getPlatform(order);

	if(order["mold"] == "duck")		return new Duck(platform);
	if(order["mold"] == "car")		return new Car(platform);
	if(order["mold"] == "hero")		return new Hero(platform);
	// Seam Point - add another abstraction.

	legacy_classes::defaulting(order, "mold", "duck");
	return new Duck(platform);
}

} // bridge

namespace chain_of_resp {	// DP 7.

using namespace bridge;

class Mold {
protected:
	Mold* successor;
public:
	Mold(Mold* successor=0) : successor(successor) {}
	virtual ~Mold() { DTORF("~CofR::Mold "); }
public:
	virtual Shape* from(map<string,string>& order) {
		Shape* shape = Shape::getShape(order);
		cout << "    <>Can't find place |" << order["moldLoc"] << "|";
		cout << " to get |" << order["mold"] << "| mold from";
		cout << " with |" << order["finish"] << "| finish,";

		order["moldLoc"] = "inventory";
		if(order["mold"]	== "")	order["mold"]	 = "duck";
		if(order["finish"]	== "")	order["finish"]	 = "smooth";

		cout << " defaulting to ";
		cout << order["finish"] << " " << order["mold"];
		cout << " from " << order["moldLoc"] << ".\n";

		return shape;
	}
public:
	static Mold* acquireMold(map<string,string>& order);
};
class Inventory : public Mold {
public:
	Inventory(Mold* successor=0) : Mold(successor) {}
	virtual ~Inventory() { DTORF("~Inventory "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "inventory") {
			Shape* shape = Shape::getShape(order);
			cout << "    Pull " << order["mold"] << " mold from inventory.\n";
			return shape;
		}
		else if(successor != 0)
			return successor->from(order);
		else
			return Mold::from(order);	// Default.
	}
};
class SisterCompany : public Mold {
public:
	SisterCompany(Mold* successor=0) : Mold(successor) {}
	virtual ~SisterCompany() { DTORF("~SisterCompany "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "sisterCompany") {
			Shape* shape = Shape::getShape(order);
			cout << "    Borrow " << order["mold"] << " mold from sister company.\n";
			return shape;
		}
		else if(successor != 0)
			return successor->from(order);
		else
			return Mold::from(order);	// Default.
	}
};
// Seam points - insert another responder.
class Mill : public Mold {
public:
	Mill(Mold* successor=0) : Mold(successor) {}
	virtual ~Mill() { DTORF("~Mill "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "mill") {
			Shape* shape = Shape::getShape(order);
			cout << "    Create " << order["mold"] << " mold from mill ";
			cout << "with " << order["cavities"];	// Added to order by TM.setupLine().
			int cavities = atoi(order["cavities"].c_str());
			if(cavities == 1)
				cout << " cavity - ";
			else
				cout << " cavities - ";
			shape->steps();
			shape->mill(order);
			return shape;
		}
		else if(successor != 0)
			return successor->from(order);
		else
			return Mold::from(order);	// Default.
	}
};

Mold* Mold::acquireMold(map<string,string>& order) {
	return	new Inventory(
			new SisterCompany(
			// Seam point - mill likely location of last resort.
			new Mill(
			new Mold(
		))));
}


// Seam points - add another responder.

} // chain_of_resp

namespace decorator {		// DP 6.

// Seam point - add another option.

} // decorator

namespace composition {

using namespace bridge;
using namespace abstract_factory3;

class Mold {
public:
	Block* block;	// Depends on order size.
	Shape* shape;	// Depends on order shape.
	string finish;	// Depends on order finish.
public:
	Mold(map<string,string>& order, Block* block)
		: block(block)
		, shape(Shape::getShape(order))
		, finish(order["finish"])
	{}
	~Mold() {
//		delete block;	// Deleted by TM.
		delete shape;
		DTORF("~Mold\n");
	}
};

} // composition

namespace template_method {	// DP 3.

/* Instruction Steps: IN WORK.
 * 1. Unfold base class
 * 2. Unfold Factory Method
 * 3. Unfold process function below namespace
 * 4. Unfold run method
 * 5. Unfold setupLine method
 * 6. Unfold ctor/dtor
 * 7. Students complete the classes
 * 8. Unfold the rest.
 * 9. Note how cleanMold uses the adapter hierarchy
 * 10. Run code
 * 11. Diff with output file
 */

#define ProcessInherit ProcessOrder3	// Pedagogy: successively replace with 0,1,2,3...

class ProcessOrder0 { // Architecture - Template Method(4), Factory Method(3).
public:
	ProcessOrder0()
	{}
	virtual ~ProcessOrder0() {
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);
		loadAdditives(order);				// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
	}
protected: // Template Method methods.
	void setupLine(map<string,string>& order) {
		cout << "  Setup injection line for ";
		cout << "<size>" << " order";
		cout << " with " << "<packager>" << " packager:\n";

		cout << "    ";
		cout << "<IJM> - ";
		cout << "<metal>(<cavities>) - ";
		cout << "<belt> belt - ";
		cout << "<bin>.\n";
	}
	void getMold(map<string,string>& order) {
		cout << "  <Acquire> <mold> mold from <moldLoc>.\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with <plastic> and color bin with <color>.\n";
	}
	void loadAdditives(map<string,string>& order) {
		cout << "    Recipe: <plastic>(vol) <color>(vol) <additive(<vol>) list> = (vol) cc.\n";
		cout << "    Volume: <mold>(vol) * <cavities> cavities = (vol) cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		cout << "  Estimated run time (algorithm) = x hour(s).\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to <temp> - inject at <pressure> PSI";
		cout << " - cool to <temp> - separate - <manner of> eject.\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {
		cout << "    Whenever <partsBin> parts bin was full:\n";
		cout << "      List of upstream machines to pause...\n";
		cout << "      ...\n";
	}
	void cleanMold(map<string,string>& order) {
		cout << "  Clean mold of <plastic>: <cleaning steps>.\n";
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		cout << "  Cycle <IJM> for <plastic> <run> times.\n";
	}
};
class ProcessOrder1 { // Defaults and order specs.
public:
	ProcessOrder1()
	{}
	virtual ~ProcessOrder1() {
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		defaults(order);
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);
		loadAdditives(order);				// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
	}
protected: // Template Method methods.
	void defaults(map<string,string>& order) {
		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}
		if(order.find("packager") == order.end()) {
			legacy_classes::defaulting(order, "packager", "Bulk");
		}
		if(order.find("mold") == order.end()) {
			legacy_classes::defaulting(order, "mold", "duck");
		}
		if(order.find("moldLoc") == order.end()) {
			legacy_classes::defaulting(order, "moldLoc", "inventory");
		}
		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}
		if(order.find("metal") == order.end()) {
			legacy_classes::defaulting(order, "metal", "Steel");
		}
		if(order.find("finish") == order.end()) {
			legacy_classes::defaulting(order, "finish", "smooth");
		}
	}
	void setupLine(map<string,string>& order) {
		cout << "  Setup injection line for ";
		cout << order["size"] << " order";
		cout << " with " << order["packager"] << " packager:\n";

		cout << "    ";
		cout << "<IJM> - ";
		cout << "<metal>(<cavities>) - ";
		cout << "<belt> belt - ";
		cout << "<bin>.\n";
	}
	void getMold(map<string,string>& order) {
		cout << "  <Acquire> " << order["mold"] << " mold";
		cout << " from " << order["moldLoc"] << ".\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["plastic"];
		cout << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		cout << "    Recipe: " << order["plastic"] << "(vol) ";
		cout << order["color"] << "(vol) <additive(<vol>) list> = (vol) cc.\n";
		cout << "    Volume: " << order["mold"] << "(vol) * <cavities> cavities = (vol) cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		cout << "  Estimated run time (algorithm) = x hour(s).\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {
		cout << "    Whenever <partsBin> parts bin was full:\n";
		cout << "      List of upstream machines to pause...\n";
		cout << "      ...\n";
	}
	void cleanMold(map<string,string>& order) {
		cout << "  Clean mold of " << order["plastic"] << ": <cleaning steps>.\n";
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		cout << "  Cycle <IJM> for " << order["plastic"] << " <run> times.\n";
	}
};
class ProcessOrder2 { // Clean molds - Adapter(2).
	adapter::CleanMold*	cleaning;
public:
	ProcessOrder2()
		: cleaning(0)
	{}
	virtual ~ProcessOrder2() {
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		defaults(order);
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);
		loadAdditives(order);				// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
	}
protected: // Template Method methods.
	void defaults(map<string,string>& order) {
		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}
		if(order.find("packager") == order.end()) {
			legacy_classes::defaulting(order, "packager", "Bulk");
		}
		if(order.find("mold") == order.end()) {
			legacy_classes::defaulting(order, "mold", "duck");
		}
		if(order.find("moldLoc") == order.end()) {
			legacy_classes::defaulting(order, "moldLoc", "inventory");
		}
		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}
		if(order.find("metal") == order.end()) {
			legacy_classes::defaulting(order, "metal", "Steel");
		}
		if(order.find("finish") == order.end()) {
			legacy_classes::defaulting(order, "finish", "smooth");
		}
	}
	void setupLine(map<string,string>& order) {
		cout << "  Setup injection line for ";
		cout << order["size"] << " order";
		cout << " with " << order["packager"] << " packager:\n";

		cout << "    ";
		cout << "<IJM> - ";
		cout << "<metal>(<cavities>) - ";
		cout << "<belt> belt - ";
		cout << "<bin>.\n";
	}
	void getMold(map<string,string>& order) {
		cout << "  <Acquire> " << order["mold"] << " mold";
		cout << " from " << order["moldLoc"] << ".\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["plastic"];
		cout << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		cout << "    Recipe: " << order["plastic"] << "(vol) ";
		cout << order["color"] << "(vol) <additive(<vol>) list> = (vol) cc.\n";
		cout << "    Volume: " << order["mold"] << "(vol) * <cavities> cavities = (vol) cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		cout << "  Estimated run time (algorithm) = x hour(s).\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {
		cout << "    Whenever <partsBin> parts bin was full:\n";
		cout << "      List of upstream machines to pause...\n";
		cout << "      ...\n";
	}
	void cleanMold(map<string,string>& order) {
		cleaning = adapter::CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		cout << "  Cycle <IJM> for " << order["plastic"] << " <run> times.\n";
	}
};
class ProcessOrder3 { // Setup injection line - Abstract Factory(9).
	adapter::CleanMold*					cleaning;
	abstract_factory1::InjectionLine*	injectionLine;
	abstract_factory1::IJM*				ijm;
	abstract_factory1::Block*			block;
	abstract_factory1::ConveyerBelt*	belt;
	abstract_factory1::PartsBin*		bin;
public:
	ProcessOrder3()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
	{}
	virtual ~ProcessOrder3() {
		delete ijm;
		delete block;
		delete belt;
		delete bin;
		delete injectionLine;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		defaults(order);
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);
		loadAdditives(order);				// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
	}
protected: // Template Method methods.
	void defaults(map<string,string>& order) {
		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}
		if(order.find("packager") == order.end()) {
			legacy_classes::defaulting(order, "packager", "Bulk");
		}
		if(order.find("mold") == order.end()) {
			legacy_classes::defaulting(order, "mold", "duck");
		}
		if(order.find("moldLoc") == order.end()) {
			legacy_classes::defaulting(order, "moldLoc", "inventory");
		}
		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}
		if(order.find("finish") == order.end()) {
			legacy_classes::defaulting(order, "finish", "smooth");
		}
	}
	void setupLine(map<string,string>& order) {	// AF (order size).
		using namespace abstract_factory1;

		injectionLine = InjectionLine::createInjectionLine(order);
		ijm	  = injectionLine->createIJM(order);
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order);
		bin	  = injectionLine->createPartsBin(order);

		cout << "  Setup injection line for ";
		cout << order["size"] << " order";
		cout << " with " << order["packager"] << " packager:\n";

		cout << "    ";
		cout << ijm->setup() << " - ";
		cout << block->setup() << " - ";
		cout << belt->setup() << " - ";
		cout << bin->setup() << ".\n";

		stringstream metal;
		metal << block->metal();
		order["metal"] = metal.str();
	}
	void getMold(map<string,string>& order) {
		cout << "  <Acquire> " << order["mold"] << " mold";
		cout << " from " << order["moldLoc"] << ".\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["plastic"];
		cout << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		int cavities = block->cavities;

		cout << "    Recipe: " << order["plastic"] << "(vol) ";
		cout << order["color"] << "(vol) <additive(<vol>) list> = (vol) cc.\n";

		cout << "    Volume: " << order["mold"] << "(vol) * ";
		cout << cavities;
		if(cavities == 1)
			cout << " cavity ";
		else
			cout << " cavities ";
		cout << "= (vol) cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		cout << "  Estimated run time (algorithm) = x hour(s).\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {
		cout << "    Whenever <partsBin> parts bin was full:\n";
		cout << "      List of upstream machines to pause...\n";
		cout << "      ...\n";
	}
	void cleanMold(map<string,string>& order) {
		cleaning = adapter::CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		cout << "  Cycle <IJM> for " << order["plastic"] << " <run> times.\n";
	}
};
class ProcessOrder4 { // Packager and pausing upstream machines - Observer(5).
	adapter::CleanMold*					cleaning;
	abstract_factory3::InjectionLine*	injectionLine;
	abstract_factory3::IJM*				ijm;
	abstract_factory3::Block*			block;
	abstract_factory3::ConveyerBelt*	belt;
	abstract_factory3::PartsBin*		bin;	// Inherits from observer::BinSubject.
	factory_method::Packager*			packager;
public:
	ProcessOrder4()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, packager(0)
	{}
	virtual ~ProcessOrder4() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete bin;
		delete injectionLine;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		defaults(order);
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);
		loadAdditives(order);				// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
	}
protected: // Template Method methods.
	void defaults(map<string,string>& order) {
		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}
		if(order.find("packager") == order.end()) {
			legacy_classes::defaulting(order, "packager", "Bulk");
		}
		if(order.find("mold") == order.end()) {
			legacy_classes::defaulting(order, "mold", "duck");
		}
		if(order.find("moldLoc") == order.end()) {
			legacy_classes::defaulting(order, "moldLoc", "inventory");
		}
		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}
		if(order.find("finish") == order.end()) {
			legacy_classes::defaulting(order, "finish", "smooth");
		}
	}
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method;
		using namespace abstract_factory3;

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);// Observer.

		packager = Packager::makeObject(order,bin);		// FM & Observer.

		cout << "  Setup injection line for ";
		cout << order["size"] << " order";
		if(packager)
			cout << " with " << packager->wrap() << " packager";
		cout << ":\n";

		cout << "    ";
		cout << ijm->setup() << " - ";
		cout << block->setup() << " - ";
		cout << belt->setup() << " - ";
		cout << bin->setup() << ".\n";

		stringstream metal;
		metal << block->metal();
		order["metal"] = metal.str();
	}
	void getMold(map<string,string>& order) {
		cout << "  <Acquire> " << order["mold"] << " mold";
		cout << " from " << order["moldLoc"] << ".\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["plastic"];
		cout << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		int cavities = block->cavities;

		cout << "    Recipe: " << order["plastic"] << "(vol) ";
		cout << order["color"] << "(vol) <additive(<vol>) list> = (vol) cc.\n";

		cout << "    Volume: " << order["mold"] << "(vol) * ";
		cout << cavities;
		if(cavities == 1)
			cout << " cavity ";
		else
			cout << " cavities ";
		cout << "= (vol) cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		cout << "  Estimated run time (algorithm) = x hour(s).\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		cleaning = adapter::CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		cout << "  Cycle " << ijm->setup() << " for ";
		cout << order["plastic"] << " <run> times.\n";
	}
};
class ProcessOrder5 { // Mold Shape & Platform - Bridge(8).
	adapter::CleanMold*					cleaning;
	abstract_factory3::InjectionLine*	injectionLine;
	abstract_factory3::IJM*				ijm;
	abstract_factory3::Block*			block;
	abstract_factory3::ConveyerBelt*	belt;
	abstract_factory3::PartsBin*		bin;	// Inherits from observer::BinSubject.
	factory_method::Packager*			packager;
	bridge::Shape*						shape;
public:
	ProcessOrder5()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, packager(0)
		, shape(0)
	{}
	virtual ~ProcessOrder5() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete bin;
		delete injectionLine;
		delete shape;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		defaults(order);
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);
		loadAdditives(order);				// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
	}
protected: // Template Method methods.
	void defaults(map<string,string>& order) {
		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}
		if(order.find("packager") == order.end()) {
			legacy_classes::defaulting(order, "packager", "Bulk");
		}
		if(order.find("mold") == order.end()) {
			legacy_classes::defaulting(order, "mold", "duck");
		}
		if(order.find("moldLoc") == order.end()) {
			legacy_classes::defaulting(order, "moldLoc", "inventory");
		}
		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}
		if(order.find("finish") == order.end()) {
			legacy_classes::defaulting(order, "finish", "smooth");
		}
	}
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method;
		using namespace abstract_factory3;

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);// Observer.

		packager = Packager::makeObject(order,bin);		// FM & Observer.

		cout << "  Setup injection line for ";
		cout << order["size"] << " order";
		if(packager)
			cout << " with " << packager->wrap() << " packager";
		cout << ":\n";

		cout << "    ";
		cout << ijm->setup() << " - ";
		cout << block->setup() << " - ";
		cout << belt->setup() << " - ";
		cout << bin->setup() << ".\n";

		stringstream metal;
		metal << block->metal();
		order["metal"] = metal.str();
	}
	void getMold(map<string,string>& order) {
		using namespace bridge;

		shape = Shape::getShape(order);	// Volume_cc.
		char str[80];
		sprintf(str, "%d", shape->volume_cc);
		order["volume"] = str;

		cout << "  <Acquire> " << order["mold"] << " mold";
		cout << " from " << order["moldLoc"] << ".\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["plastic"];
		cout << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		int cavities = block->cavities;

		int plasticVol = shape->volume_cc;
		int colorVol = 0.10*plasticVol;
		int totalVol = plasticVol + colorVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") ";
		cout << order["color"] << "(" << colorVol << ") ";
		cout << "<additive(<vol>) list> = (" << totalVol << ") cc.\n";

		cout << "    Volume: " << order["mold"] << "(" << totalVol << ") * ";
		cout << cavities;
		if(cavities == 1)
			cout << " cavity ";
		else
			cout << " cavities ";
		cout << "= (vol) cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		cout << "  Estimated run time (algorithm) = x hour(s).\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		cleaning = adapter::CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		cout << "  Cycle " << ijm->setup() << " for ";
		cout << order["plastic"] << " <run> times.\n";
	}
};
class ProcessOrder6 { // Acquire mold - CofR(7).
	adapter::CleanMold*					cleaning;
	abstract_factory3::InjectionLine*	injectionLine;
	abstract_factory3::IJM*				ijm;
	abstract_factory3::Block*			block;
	abstract_factory3::ConveyerBelt*	belt;
	abstract_factory3::PartsBin*		bin;	// Inherits from observer::BinSubject.
	factory_method::Packager*			packager;
	bridge::Shape*						shape;
	chain_of_resp::Mold*				mold;
public:
	ProcessOrder6()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, packager(0)
		, shape(0)
		, mold(0)
	{}
	virtual ~ProcessOrder6() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete bin;
		delete injectionLine;
		delete shape;
		delete mold; cout << "\n";
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		defaults(order);
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);
		loadAdditives(order);				// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
	}
protected: // Template Method methods.
	void defaults(map<string,string>& order) {
		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}
		if(order.find("packager") == order.end()) {
			legacy_classes::defaulting(order, "packager", "Bulk");
		}
		if(order.find("mold") == order.end()) {
			legacy_classes::defaulting(order, "mold", "duck");
		}
		if(order.find("moldLoc") == order.end()) {
			legacy_classes::defaulting(order, "moldLoc", "inventory");
		}
		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}
		if(order.find("finish") == order.end()) {
			legacy_classes::defaulting(order, "finish", "smooth");
		}
	}
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method;
		using namespace abstract_factory3;

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);// Observer.

		packager = Packager::makeObject(order,bin);		// FM & Observer.

		cout << "  Setup injection line for ";
		cout << order["size"] << " order";
		if(packager)
			cout << " with " << packager->wrap() << " packager";
		cout << ":\n";

		cout << "    ";
		cout << ijm->setup() << " - ";
		cout << block->setup() << " - ";
		cout << belt->setup() << " - ";
		cout << bin->setup() << ".\n";

		stringstream metal;
		metal << block->metal();
		order["metal"] = metal.str();

		stringstream cavities;
		cavities << block->cavities;
		order["cavities"] = cavities.str();
	}
	void getMold(map<string,string>& order) {
		using namespace bridge;
		using namespace chain_of_resp;

		mold = Mold::acquireMold(order);

		shape = mold->from(order);	// Volume_cc.
		char str[80];
		sprintf(str, "%d", shape->volume_cc);
		order["volume"] = str;

		cout << "  <Acquire> " << order["mold"] << " mold";
		cout << " from " << order["moldLoc"] << ".\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["plastic"];
		cout << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		int cavities = block->cavities;

		int plasticVol = shape->volume_cc;
		int colorVol = 0.10*plasticVol;
		int totalVol = plasticVol + colorVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") ";
		cout << order["color"] << "(" << colorVol << ") ";
		cout << "<additive(<vol>) list> = (" << totalVol << ") cc.\n";

		cout << "    Volume: " << order["mold"] << "(" << totalVol << ") * ";
		cout << cavities;
		if(cavities == 1)
			cout << " cavity ";
		else
			cout << " cavities ";
		cout << "= (vol) cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		cout << "  Estimated run time (algorithm) = x hour(s).\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		cleaning = adapter::CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		cout << "  Cycle " << ijm->setup() << " for ";
		cout << order["plastic"] << " <run> times.\n";
	}
};
class ProcessOrder7 { // Tags - Decorator(6).

};
class ProcessOrder8 { // Additives - Decorator(6).

};
class ProcessOrder9 { // Estimated run time - Strategy(1).

};

class ProcessOrderB { // Runtime estimate - Strategy.
	strategy::RuntimeEstimate*			runtimeEst;
	adapter::CleanMold*					cleaning;
	factory_method::Packager*			packager;
	abstract_factory3::InjectionLine*	injectionLine;
	abstract_factory3::IJM*				ijm;
	abstract_factory3::Block*			block;
	abstract_factory3::ConveyerBelt*	belt;
	abstract_factory3::PartsBin*		bin;	// Inherits from observer::BinSubject.
public:
	ProcessOrderB()
		: runtimeEst(0)
		, cleaning(0)
		, packager(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
	{}
	virtual ~ProcessOrderB() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete bin;
		delete injectionLine;
		delete runtimeEst;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		defaults(order);
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);
		loadAdditives(order);				// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
	}
protected: // Template Method methods.
	void defaults(map<string,string>& order) {
		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}
		if(order.find("packager") == order.end()) {
			legacy_classes::defaulting(order, "packager", "Bulk");
		}
		if(order.find("mold") == order.end()) {
			legacy_classes::defaulting(order, "mold", "duck");
		}
		if(order.find("moldLoc") == order.end()) {
			legacy_classes::defaulting(order, "moldLoc", "inventory");
		}
		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}
		if(order.find("metal") == order.end()) {
			legacy_classes::defaulting(order, "metal", "Steel");
		}
		if(order.find("finish") == order.end()) {
			legacy_classes::defaulting(order, "finish", "smooth");
		}
	}
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method;
		using namespace abstract_factory3;

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);// Observer.

		packager = Packager::makeObject(order,bin);		// FM & Observer.

		cout << "  Setup injection line for ";
		cout << order["size"] << " order";
		if(packager)
			cout << " with " << packager->wrap() << " packager";
		cout << ":\n";

		cout << "    ";
		cout << ijm->setup() << " - ";
		cout << block->setup() << " - ";
		cout << belt->setup() << " - ";
		cout << bin->setup() << ".\n";
	}
	void getMold(map<string,string>& order) {
		order["metal"] = block->metal();

		cout << "  <Acquire> " << order["mold"] << " mold";
		cout << " from " << order["moldLoc"] << ".\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["plastic"];
		cout << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		cout << "    Recipe: " << order["plastic"] << "(vol) ";
		cout << order["color"] << "(vol) <additive(<vol>) list> = (vol) cc.\n";
		cout << "    Volume: " << order["mold"] << "(vol) * <cavities> cavities = (vol) cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		using namespace strategy;

		char n[10];
		sprintf(n, "%d", block->cavities);
		order["cavities"] = n;
		runtimeEst = RuntimeEstimate::selectEstimationAlgorithm(order);
		unsigned runtime = (*runtimeEst)(order);

		string hour_s = runtime==1 ? " hour" : " hours";
		cout << "  Estimated run time (" << runtimeEst->name()
			 << ") = " << runtime << hour_s << ".\n";
		cout << "    " << runtimeEst->formula() << ".\n";
		cout << "    " << runtimeEst->values() << ".\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		cleaning = adapter::CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/block->cavities;

		cout << "  Cycle " << ijm->setup() << " for ";
		cout << order["plastic"] << " " << runSize << " times.\n";
	}
};
class ProcessOrderC { // Shapes, and volume for runtime estimate.
	strategy::RuntimeEstimate*			runtimeEst;
	adapter::CleanMold*					cleaning;
	factory_method::Packager*			packager;
	abstract_factory3::InjectionLine*	injectionLine;
	abstract_factory3::IJM*				ijm;
	abstract_factory3::Block*			block;
	abstract_factory3::ConveyerBelt*	belt;
	abstract_factory3::PartsBin*		bin;	// Inherits from observer::BinSubject.
	composition::Mold*					mold;
public:
	ProcessOrderC()
		: runtimeEst(0)
		, cleaning(0)
		, packager(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, mold(0)
	{}
	virtual ~ProcessOrderC() {
		delete mold;
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete bin;
		delete injectionLine;
		delete runtimeEst;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		defaults(order);
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);
		loadAdditives(order);				// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
	}
protected: // Template Method methods.
	void defaults(map<string,string>& order) {
		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}
		if(order.find("packager") == order.end()) {
			legacy_classes::defaulting(order, "packager", "Bulk");
		}
		if(order.find("mold") == order.end()) {
			legacy_classes::defaulting(order, "mold", "duck");
		}
		if(order.find("moldLoc") == order.end()) {
			legacy_classes::defaulting(order, "moldLoc", "inventory");
		}
		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}
		if(order.find("metal") == order.end()) {
			legacy_classes::defaulting(order, "metal", "Steel");
		}
		if(order.find("finish") == order.end()) {
			legacy_classes::defaulting(order, "finish", "smooth");
		}
	}
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method;
		using namespace abstract_factory3;

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);// Observer.

		packager = Packager::makeObject(order,bin);		// FM & Observer.

		cout << "  Setup injection line for ";
		cout << order["size"] << " order";
		if(packager)
			cout << " with " << packager->wrap() << " packager";
		cout << ":\n";

		cout << "    ";
		cout << ijm->setup() << " - ";
		cout << block->setup() << " - ";
		cout << belt->setup() << " - ";
		cout << bin->setup() << ".\n";
	}
	void getMold(map<string,string>& order) {
		order["metal"] = block->metal();

		mold = new composition::Mold(order, block);

		cout << "  <Acquire> " << order["mold"] << " mold";
		cout << " from " << order["moldLoc"] << ".\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["plastic"];
		cout << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		cout << "    Recipe: " << order["plastic"] << "(vol) ";
		cout << order["color"] << "(vol) <additive(<vol>) list> = (vol) cc.\n";
		cout << "    Volume: " << order["mold"] << "(vol) * <cavities> cavities = (vol) cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		using namespace strategy;

		char val[10];
		sprintf(val, "%d", block->cavities);
		order["cavities"] = val;
		sprintf(val, "%d", mold->shape->volume_cc);
		order["volume"] = val;
		runtimeEst = RuntimeEstimate::selectEstimationAlgorithm(order);
		unsigned runtime = (*runtimeEst)(order);

		string hour_s = runtime==1 ? " hour" : " hours";
		cout << "  Estimated run time (" << runtimeEst->name()
			 << ") = " << runtime << hour_s << ".\n";
		cout << "    " << runtimeEst->formula() << ".\n";
		cout << "    " << runtimeEst->values() << ".\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		cleaning = adapter::CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/block->cavities;

		cout << "  Cycle " << ijm->setup() << " for ";
		cout << order["plastic"] << " " << runSize << " times.\n";
	}
};

class ABSOrder : public ProcessInherit {
public: ~ABSOrder() { DTORF("~ABSOrder\n"); }
public:
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to 440 - inject at 125 PSI"
			 << " - cool to 360 - separate - progressive eject.\n";
	}
};
class PropyleneOrder : public ProcessInherit {
public: ~PropyleneOrder() { DTORF("~PropyleneOrder\n"); }
public:
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to 350 - inject at 90 PSI"
			 << " - cool to 290 - separate - smooth eject.\n";
	}
};
class EtheleneOrder : public ProcessInherit {
public: ~EtheleneOrder() { DTORF("~EtheleneOrder\n"); }
public:
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to 360 - inject at 95 PSI"
			 << " - cool to 280 - separate - smooth eject.\n";
	}
};
class PETOrder : public ProcessInherit {
public: ~PETOrder() { DTORF("~PETOrder\n"); }
public:
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to 404 - inject at 110 PSI"
			 << " - cool to 340 - separate - smooth eject.\n";
	}
};
// Seam point - add another polymorphic step.
// Seam point - add another constant step.
// Seam point - convert a constant step into a polymorphic step.

ProcessInherit* getProcessOrder(map<string,string>& order) { // 3 - Factory Method.
	if(order["plastic"] == "ABS")				return new ABSOrder;
	if(order["plastic"] == "Polypropylene")		return new PropyleneOrder;
	if(order["plastic"] == "Polyethelene")		return new EtheleneOrder;
	if(order["plastic"] == "PET")				return new PETOrder;
	// Seam point - add another plastic.

	legacy_classes::defaulting(order, "plastic", "ABS");
	return new ABSOrder;
}

} // template_method

void process(map<string,string>& order) {
	// Fill in the namespaces above with your design pattern class hierarchies.
	// Call your order processing class from here <myProcess>->run(order);

	using namespace template_method;

	ProcessInherit* processOrder = getProcessOrder(order);

	processOrder->run(order);

	delete processOrder;

	cout << endl;

//	template_method::ProcessOrder processOrder;
//	processOrder.run(order);
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
	cout << "<<< final guided >>>\n";

	FILE* orderFilePtr = fopen(orderFile.c_str(), "r");

	for(;;) {	// Get and process each order.
		map<string,string> order = getCompleteOrder(orderFilePtr);
		if(order.size() == 0)
			break;
		process(order);
	}
}

} // final_guided

#endif /* FINAL_GUIDED_H_ */

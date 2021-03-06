/*
 * guided2.h - tutorial from finalPart1.h to finalPart2.h
 *
 *  Created on: 1/17/16
 *      Author: Allan Goff
 */

#ifndef FINAL_GUIDED2_H
#define FINAL_GUIDED2_H

namespace final_guided2 {

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

namespace adapter {				// DP 2.

class CleanMold {
public:	virtual ~CleanMold() { DTORF("~adapter::CleanMold\n"); }
public:
	virtual void clean(const string& metal) {
		cout << "  Clean mold of <plastic>: <cleaning steps>.\n";
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
class StyreneClean : public CleanMold {
public: ~StyreneClean() { DTORF("~StyreneClean "); }
	void clean(const string& metal) {
		cout << "  Clean mold of Styrene: spray with bleach, dry.\n";
	}
};
class Nylon66Clean : public CleanMold {
public: ~Nylon66Clean() { DTORF("~Nylon66Clean "); }
	void clean(const string& metal) {
		cout << "  Clean mold of Nylon66: ozone wash.\n";
	}
};

CleanMold* CleanMold::getCleaning(map<string,string>& order) {	// FM.
	if(order["plastic"] == "ABS")				return new ABSClean;
	if(order["plastic"] == "Polypropylene")		return new PolyClean;
	if(order["plastic"] == "Polyethelene")		return new PolyClean;
	if(order["plastic"] == "PET")				return new PETClean;
	// Seam point - add another plastic.
	if(order["plastic"] == "Styrene")			return new StyreneClean;
	if(order["plastic"] == "Nylon66")			return new Nylon66Clean;

	return new CleanMold;
}

} // adapter

namespace strategy_1 {			// DP 1.

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

	cout << "    <>Using most sophisticated estimation algorithm"
		 << " for orders greater than 150000.\n";
	return new Historical;
}

} // strategy_1
namespace strategy_2 {			// DP 1.

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
class Projection : public RuntimeEstimate {
	string	metal;
	int 	volume_cc;
	float	rejectRate_pcnt;
public:
	Projection() : volume_cc(0), rejectRate_pcnt(0) {}
	~Projection() { DTORF("~Projection "); }
public:
	int operator()(map<string,string>& order) {
		using namespace legacy_classes;
		RuntimeEstimate::runTimeEst_hrs(order);
		metal = order["metal"];	// Added to order by TM.getMold().
		volume_cc = atoi(order["volume"].c_str());	// Added to order by TM.?().
		cycleTime = cycleTime_sec(metal, volume_cc);
		bool pet = order["plastic"] == "PET";
		rejectRate_pcnt = pet ? rejectRateHigh_pcnt : rejectRateLow_pcnt;
		return (setupAvg_min + (1.0 + 0.01*rejectRate_pcnt)*(orderSize/cavities)*cycleTime/60 + teardownAvg_min)/60;
	}
	string formula() {
		return "(setupAvg_min + (1.0 + 0.01*rejectRate_pcnt)*(orderSize/cavities)*cycleTime_sec(metal, volume_cc)/60 + teardownAvg_min)/60";
	}
	string values() {
		using namespace legacy_classes;
		stringstream value;
		value << "(" << setupAvg_min
			  << " + (1.0 + 0.01*" << rejectRate_pcnt << ")*("
			  << orderSize/cavities << ")*"
			  << cycleTime << "(" << metal << "," << volume_cc << ")/" << 60
			  << " + " << teardownAvg_min << ")/60";
		return value.str();
	}
	virtual string name() {
		return "Projection";
	}
};

RuntimeEstimate* RuntimeEstimate::selectEstimationAlgorithm(map<string,string>& order) {
	int size = atoi(order["size"].c_str());

	if(size <=  10000)	return new BackOfTheEnvelope;
	if(size <=  30000)	return new Calculation;
	if(size <= 150000)	return new Historical;
	// Seam point - add another runtime estimation algorithm.
	if(size <= 650000)	return new Projection;

	cout << "    <>Using most sophisticated estimation algorithm"
		 << " for orders greater than 650000.\n";
	return new Projection;
}

} // strategy_2

namespace observer {			// DP 5.

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
// Seam point - add another listener (in other namespaces;
// the listeners are base classes in other design patterns).

BinSubject::~BinSubject() {
	DTORF("~observer::BinSubject\n");
	DTORF(" BinObservers left to process (should be zero) = ");
	DTORF(obs.size());
	DTORF(".\n");
}
void BinSubject::pause() {
	cout << "    Whenever " << name << " parts bin was full...\n";
	list<BinObserver*>::iterator it;
	for(it=obs.begin(); it!=obs.end(); ++it) {
		(*it)->update(this);
	}
}

} // observer

namespace factory_method_1 {	// DP 4.

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
		cout << "      Shrink wrap packager paused while "
			 << bin->name << " parts bin was swapped.\n";
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
		cout << "      Hard pack packager paused while "
			 << bin->name << " parts bin was swapped.\n";
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

} // factory_method_1
namespace factory_method_2 {	// DP 4.

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
		cout << "      Shrink wrap packager paused while "
			 << bin->name << " parts bin was swapped.\n";
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
		cout << "      Hard pack packager paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};
// Seam point - add another class.
class ZipLock : public Packager {
public:
	ZipLock(BinSubject* bin) : Packager(bin) {}
	~ZipLock() { DTORF("~ZipLock "); }
public:
	string wrap() { return "ZipLock"; }
public:
	void update(BinSubject* bin) {
		cout << "      ZipLock packager paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};

Packager* Packager::makeObject(map<string,string>& order, BinSubject* bin) {
	if(order["packager"] == "Bulk")			return 0;
	if(order["packager"] == "ShrinkWrap")	return new ShrinkWrap(bin);
	if(order["packager"] == "HardPack")		return new HardPack(bin);
	// Seam point - add another class.
	if(order["packager"] == "ZipLock")		return new ZipLock(bin);

	legacy_classes::defaulting(order, "packager", "None");
	return 0;
}

// Seam point - add another Observer.

class Stuffer : public BinObserver {	// If the classes are varying...
public:
	Stuffer(BinSubject* bin) : BinObserver(bin) {}
	virtual ~Stuffer() { DTORF("~factory_method::Stuffer "); }
public:
	virtual string fill() { return "<stuffer>"; }
	void update(BinSubject* bin) {}
public:
	static Stuffer* makeObject(map<string,string>& map, BinSubject* bin);
};
// class Air is the null class.
class Popcorn : public Stuffer {
public:
	Popcorn(BinSubject* bin) : Stuffer(bin) {}
	~Popcorn() { DTORF("~Popcorn "); }
public:
	string fill() { return "styrene popcorn"; }
	void update(BinSubject* bin) {
		cout << "      Popcorn stuffer paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};
class Bubblewrap : public Stuffer {
public:
	Bubblewrap(BinSubject* bin) : Stuffer(bin) {}
	~Bubblewrap() { DTORF("~Bubblewrap "); }
public:
	string fill() { return "bubble wrap"; }
	void update(BinSubject* bin) {
		cout << "      Bubblewrap stuffer paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};
class Foam : public Stuffer {
public:
	Foam(BinSubject* bin) : Stuffer(bin) {}
	~Foam() { DTORF("~Foam "); }
public:
	string fill() { return "expanding foam"; }
	void update(BinSubject* bin) {
		cout << "      Foam stuffer paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};
// Seam point - add another class.

Stuffer* Stuffer::makeObject(map<string,string>& order, BinSubject* bin) {
	if(order["stuffer"] == "Air")			return 0;	// Null, no Observer.
	if(order["stuffer"] == "Popcorn")		return new Popcorn(bin);
	if(order["stuffer"] == "Bubblewrap")	return new Bubblewrap(bin);
	if(order["stuffer"] == "Foam")			return new Foam(bin);
	// Seam point - add another class.

	legacy_classes::defaulting(order, "stuffer", "None");
	return 0;
}

} // factory_method_2

namespace abstract_factory_1 {	// DP 9.

using namespace observer;

class IJM : public BinObserver {
public:
	IJM(BinSubject* bin) : BinObserver(bin) {}
	virtual ~IJM() { DTORF("~IJM "); }
public:
	void update(BinSubject* bin) {
		cout << "      " << setup() << " paused while ";
		cout << bin->name << " parts bin was swapped.\n";
	}
	virtual string setup() { return "IJM base"; }
};
class IJM_110 : public IJM {		// PilotOrder.
public:
	IJM_110(BinSubject* bin) : IJM(bin) {}
	~IJM_110() { DTORF("~IJM_110 "); }
public:
	string setup() { return "IJM_110"; }
};
class IJM_120 : public IJM {		// SmallOrder.
public:
	IJM_120(BinSubject* bin) : IJM(bin) {}
	~IJM_120() { DTORF("~IJM_120 "); }
public:
	string setup() { return "IJM_120"; }
};
class IJM_210 : public IJM {		// MediumOrder.
public:
	IJM_210(BinSubject* bin) : IJM(bin) {}
	~IJM_210() { DTORF("~IJM_210 "); }
public:
	string setup() { return "IJM_210"; }
};
class IJM_220 : public IJM {		// LargeOrder.
public:
	IJM_220(BinSubject* bin) : IJM(bin) {}
	~IJM_220() { DTORF("~IJM_220 "); }
public:
	string setup() { return "IJM_220"; }
};
// Seam point - add another IJM type.

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
// Seam point - add another Block type.

class ConveyerBelt : public BinObserver {
public:
	ConveyerBelt(BinSubject* bin) : BinObserver(bin) {}
	virtual ~ConveyerBelt() { DTORF("~ConveyerBelt "); }
public:
	virtual string setup() { return "ConveyerBelt base"; }
public:
	void update(BinSubject* bin) {
		cout << "      " << setup() << " paused while ";
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
// Seam point - add another ConveyerBelt type.

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
// Seam point - add another PartsBin type.

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
public:	~PilotOrder() { DTORF("~PilotOrder "); }
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
public:	~SmallOrder() { DTORF("~SmallOrder "); }
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
public:	~MediumOrder() { DTORF("~MediumOrder "); }
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
public:	~LargeOrder() { DTORF("~LargeOrder "); }
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
// Seam point - add another InjectionLine family.

InjectionLine* InjectionLine::createInjectionLine(map<string,string>& order) {
	unsigned size = atoi(order["size"].c_str());

	if(size <=  10000)	return new PilotOrder;
	if(size <=  20000)	return new SmallOrder;
	if(size <=  50000)	return new MediumOrder;
	if(size <= 100000)	return new LargeOrder;
	// Seam point - add another order size.

	legacy_classes::defaulting(order, "size", "100000");
	return new LargeOrder;
}

} // abstract_factory_1
namespace abstract_factory_2 {	// DP 9.

using namespace observer;

class IJM : public BinObserver {
public:
	IJM(BinSubject* bin) : BinObserver(bin) {}
	virtual ~IJM() { DTORF("~IJM "); }
public:
	void update(BinSubject* bin) {
		cout << "      " << setup() << " paused while ";
		cout << bin->name << " parts bin was swapped.\n";
	}
	virtual string setup() { return "IJM base"; }
};
class IJM_110 : public IJM {		// PilotOrder.
public:
	IJM_110(BinSubject* bin) : IJM(bin) {}
	~IJM_110() { DTORF("~IJM_110 "); }
public:
	string setup() { return "IJM_110"; }
};
class IJM_120 : public IJM {		// SmallOrder.
public:
	IJM_120(BinSubject* bin) : IJM(bin) {}
	~IJM_120() { DTORF("~IJM_120 "); }
public:
	string setup() { return "IJM_120"; }
};
class IJM_210 : public IJM {		// MediumOrder.
public:
	IJM_210(BinSubject* bin) : IJM(bin) {}
	~IJM_210() { DTORF("~IJM_210 "); }
public:
	string setup() { return "IJM_210"; }
};
class IJM_220 : public IJM {		// LargeOrder.
public:
	IJM_220(BinSubject* bin) : IJM(bin) {}
	~IJM_220() { DTORF("~IJM_220 "); }
public:
	string setup() { return "IJM_220"; }
};
// Seam point - add another IJM type.
class IJM_140 : public IJM {		// FastOrder.
public:
	IJM_140(BinSubject* bin) : IJM(bin) {}
	~IJM_140() { DTORF("~IJM_140 "); }
public:
	string setup() { return "IJM_140"; }
};
class IJM_240 : public IJM {		// HugeOrder.
public:
	IJM_240(BinSubject* bin) : IJM(bin) {}
	~IJM_240() { DTORF("~IJM_240 "); }
public:
	string setup() { return "IJM_240"; }
};
class IJM_280 : public IJM {		// GiganticOrder.
public:
	IJM_280(BinSubject* bin) : IJM(bin) {}
	~IJM_280() { DTORF("~IJM_280 "); }
public:
	string setup() { return "IJM_280"; }
};

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
// Seam point - add another Block type.

class ConveyerBelt : public BinObserver {
public:
	ConveyerBelt(BinSubject* bin) : BinObserver(bin) {}
	virtual ~ConveyerBelt() { DTORF("~ConveyerBelt "); }
public:
	virtual string setup() { return "ConveyerBelt base"; }
public:
	void update(BinSubject* bin) {
		cout << "      " << setup() << " paused while ";
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
// Seam point - add another ConveyerBelt type.
class VLevelBelt : public ConveyerBelt {
public:
	VLevelBelt(BinSubject* bin) : ConveyerBelt(bin) {}
	~VLevelBelt() { DTORF("~VLevelBelt "); }
public:
	string setup() {return "V-Level conveyer belt"; }
};

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
// Seam point - add another PartsBin type.
class Crate : public PartsBin {
public:
	Crate() : PartsBin("Crate") {}
	~Crate() { DTORF("~Crate "); }
public:
	string setup() { return "Crate"; }
};

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
public:	~PilotOrder() { DTORF("~PilotOrder "); }
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
public:	~SmallOrder() { DTORF("~SmallOrder "); }
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
public:	~MediumOrder() { DTORF("~MediumOrder "); }
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
public:	~LargeOrder() { DTORF("~LargeOrder "); }
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
// Seam point - add another InjectionLine family.
class FastOrder : public InjectionLine {
public:	~FastOrder() { DTORF("~FastOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_140(bin);
	}
	Block* createBlock(map<string,string>& order) {
		return new Aluminum(4);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new VLevelBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new ShellBox();
	}
};
class HugeOrder : public InjectionLine {
public:	~HugeOrder() { DTORF("~HugeOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_240(bin);
	}
	Block* createBlock(map<string,string>& order) {
		return new Steel(4);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new VLevelBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new Crate();
	}
};
class GiganticOrder : public InjectionLine {
public:	~GiganticOrder() { DTORF("~GiganticOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_280(bin);
	}
	Block* createBlock(map<string,string>& order) {
		return new Steel(8);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new VLevelBelt(bin);
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new Crate();
	}
};

InjectionLine* InjectionLine::createInjectionLine(map<string,string>& order) {
	unsigned size = atoi(order["size"].c_str());

	if(size <=  10000)	return new PilotOrder;
	if(size <=  20000)	return new SmallOrder;
	if(size <=  40000)	return new FastOrder;
	if(size <=  50000)	return new MediumOrder;
	if(size <= 100000)	return new LargeOrder;
	// Seam point - add another order size.
	if(size <= 200000)	return new HugeOrder;
	if(size <= 400000)	return new GiganticOrder;

	legacy_classes::defaulting(order, "size", "400000");
	return new GiganticOrder;
}

} // abstract_factory_2

namespace bridge_1 {			// DP 8.

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

	if(metal == "Aluminum")	{		return new HighCarbon; }
	else if(metal == "Steel") {
		if(finish == "smooth")		return new HighCarbon;
		if(finish == "rippled")		return new Carbide;
		if(finish == "dimpled")		return new DiamondTipped;
		// Seam Point - add another finish.

		legacy_classes::defaulting(order, "finish", "smooth");
	}
	// Seam Point - add another metal.
	else {
		legacy_classes::defaulting(order, "metal", "Steel");
	}

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
		cout << "    using " << platform->name() << " tools (";
		cout << platform->drill() << ", ";
		cout << platform->cut() << ", and ";
		cout << platform->grind() << ")\n";
		cout << "    to mill " << order["metal"] << " block into ";
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
	~Duck() { DTORF("~Duck "); }
public:
	virtual void steps() {
		cout << "steps: D2C4G1\n";
	}
};
class Car : public Shape {
public:
	Car(Platform* platform) : Shape(platform, "car", 40) {};
	~Car() { DTORF("~Car "); }
public:
	virtual void steps() {
		cout << "steps: D2C8D1G3\n";
	}
};
class Hero : public Shape {
public:
	Hero(Platform* platform) : Shape(platform, "hero", 50) {};
	~Hero() { DTORF("~Hero "); }
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

} // bridge_1
namespace bridge_2 {			// DP 8.

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
class EDM : public Platform {
public: virtual ~EDM() { DTORF("~EDM "); }
public:
	string name() { return "EDM"; }
	string drill() { return "static punch"; }
	string cut() { return "static slice"; }
	string grind() { return "static etch"; }
public:
	static Platform* getPlatform(map<string,string>& order);
};

Platform* Platform::getPlatform(map<string,string>& order) {
	string metal = order["metal"];	// Added to order by TM.setupLine().
	string finish = order["finish"];

	if(metal == "Aluminum")	{		return new HighCarbon; }
	else if(metal == "Steel") {
		if(finish == "smooth")		return new HighCarbon;
		if(finish == "rippled")		return new Carbide;
		if(finish == "dimpled")		return new DiamondTipped;
		// Seam Point - add another finish.
		if(finish == "sanded")		return new EDM;
		if(finish == "satin")		return new EDM;

		legacy_classes::defaulting(order, "finish", "smooth");
	}
	// Seam Point - add another metal.
	else {
		legacy_classes::defaulting(order, "metal", "Steel");
	}

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
		cout << "    using " << platform->name() << " tools (";
		cout << platform->drill() << ", ";
		cout << platform->cut() << ", and ";
		cout << platform->grind() << ")\n";
		cout << "    to mill " << order["metal"] << " block into ";
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
	~Duck() { DTORF("~Duck "); }
public:
	virtual void steps() {
		cout << "steps: D2C4G1\n";
	}
};
class Car : public Shape {
public:
	Car(Platform* platform) : Shape(platform, "car", 40) {};
	~Car() { DTORF("~Car "); }
public:
	virtual void steps() {
		cout << "steps: D2C8D1G3\n";
	}
};
class Hero : public Shape {
public:
	Hero(Platform* platform) : Shape(platform, "hero", 50) {};
	~Hero() { DTORF("~Hero "); }
public:
	virtual void steps() {
		cout << "steps: D1C1D1G7\n";
	}
};
// Seam Point - add another abstraction.
class Dino : public Shape {
public:
	Dino(Platform* platform) : Shape(platform, "dino", 30) {};
	~Dino() { DTORF("~Dino "); }
public:
	virtual void steps() {
		cout << "steps: G5D2C1\n";
	}
};

Shape* Shape::getShape(map<string,string>& order) {
	Platform* platform = 0;
	if(order["moldLoc"] == "mill")
		platform = Platform::getPlatform(order);

	if(order["mold"] == "duck")		return new Duck(platform);
	if(order["mold"] == "car")		return new Car(platform);
	if(order["mold"] == "hero")		return new Hero(platform);
	// Seam Point - add another abstraction.
	if(order["mold"] == "dino")		return new Dino(platform);

	legacy_classes::defaulting(order, "mold", "duck");
	return new Duck(platform);
}

} // bridge_2

namespace chain_of_resp_1 {		// DP 7.

using namespace bridge_1;

class Mold {
protected:
	Mold* successor;
public:
	Mold(Mold* successor=0) : successor(successor) {}
	virtual ~Mold() { DTORF("~CofR::Mold "); }
public:
	virtual Shape* from(map<string,string>& order) {
		Shape* shape = Shape::getShape(order);

		legacy_classes::defaulting(order, "moldLoc", "inventory");

		cout << "  Pull " << order["mold"] << " mold "
			 << "from " << order["moldLoc"] << ".\n";

		return shape;
	}
public:
	static Mold* acquireMold(map<string,string>& order);
};
class Inventory : public Mold {
public:
	Inventory(Mold* successor=0) : Mold(successor) {}
	~Inventory() { DTORF("~Inventory "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "inventory") {
			Shape* shape = Shape::getShape(order);
			cout << "  Pull " << order["mold"] << " mold from inventory.\n";
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
	~SisterCompany() { DTORF("~SisterCompany "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "sisterCompany") {
			Shape* shape = Shape::getShape(order);
			cout << "  Borrow " << order["mold"] << " mold from sister company.\n";
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
	~Mill() { DTORF("~Mill "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "mill") {
			Shape* shape = Shape::getShape(order);
			cout << "  Create " << order["mold"] << " mold from mill ";
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

} // chain_of_resp_1
namespace chain_of_resp_2 {		// DP 7.

using namespace bridge_2;

class Mold {
protected:
	Mold* successor;
public:
	Mold(Mold* successor=0) : successor(successor) {}
	virtual ~Mold() { DTORF("~CofR::Mold "); }
public:
	virtual Shape* from(map<string,string>& order) {
		Shape* shape = Shape::getShape(order);

		legacy_classes::defaulting(order, "moldLoc", "inventory");

		cout << "  Pull " << order["mold"] << " mold "
			 << "from " << order["moldLoc"] << ".\n";

		return shape;
	}
public:
	static Mold* acquireMold(map<string,string>& order);
};
class Inventory : public Mold {
public:
	Inventory(Mold* successor=0) : Mold(successor) {}
	~Inventory() { DTORF("~Inventory "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "inventory") {
			Shape* shape = Shape::getShape(order);
			cout << "  Pull " << order["mold"] << " mold from inventory.\n";
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
	~SisterCompany() { DTORF("~SisterCompany "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "sisterCompany") {
			Shape* shape = Shape::getShape(order);
			cout << "  Borrow " << order["mold"] << " mold from sister company.\n";
			return shape;
		}
		else if(successor != 0)
			return successor->from(order);
		else
			return Mold::from(order);	// Default.
	}
};
// Seam points - insert another responder.
class Purchase : public Mold {
public:
	Purchase(Mold* successor=0) : Mold(successor) {}
	~Purchase() { DTORF("~Purchase "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "purchase") {
			Shape* shape = Shape::getShape(order);
			cout << "    Acquire " << order["mold"] << " mold via purchase.\n";
			return shape;
		}
		else if(successor != 0)
			return successor->from(order);
		else
			return Mold::from(order);	// Default.
	}
};
class Mill : public Mold {
public:
	Mill(Mold* successor=0) : Mold(successor) {}
	~Mill() { DTORF("~Mill "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "mill") {
			Shape* shape = Shape::getShape(order);
			cout << "  Create " << order["mold"] << " mold from mill ";
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
			new Purchase(
			new Mill(
			new Mold(
		)))));
}

// Seam points - add another responder.

} // chain_of_resp_2

namespace decorator_1 {			// DP 6.

class Cavity {	// If the options are varying...
public:
	const unsigned space_mm;
public:
	Cavity(unsigned width_mm=20)
	  : space_mm(width_mm)
	{}
	virtual ~Cavity() { DTORF("~dec::Cavity "); }
public:
	virtual unsigned width_mm() { return 0; }
	virtual string list() { return ""; }
public:
	unsigned computeBlankWidth(unsigned tags) {
		return(space_mm - tags);
	}
public:
	static stringstream unknownTags;
	static Cavity* addTags(Cavity* cavity, const string& list);
};
class Blank : public Cavity {
public:
	Blank(unsigned width_mm=20) : Cavity(width_mm) {}
	~Blank() { DTORF("~Blank "); }
public:
	virtual unsigned width_mm() { return 0; }
	string list() { return ""; }
};
class Tags : public Cavity {
protected:
	Cavity* delegate;
public:
	Tags(Cavity* delegate) : delegate(delegate) {}
	~Tags() { DTORF("~Tags "); delete delegate; }
};
class ModelNumber : public Tags {
public:
	ModelNumber(Cavity* delegate) : Tags(delegate) {}
	~ModelNumber() { DTORF("~ModelNumber "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "ModelNumber "; }
};
class Country : public Tags {
public:
	Country(Cavity* delegate) : Tags(delegate) {}
	~Country() { DTORF("~Country "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "Country "; }
};
class Date : public Tags {
public:
	Date(Cavity* delegate) : Tags(delegate) {}
	~Date() { DTORF("~Date "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "Date "; }
};
// Seam point - add another option.

stringstream Cavity::unknownTags;
Cavity* Cavity::addTags(Cavity* cavity, const string& list) {
	char val[83] = {0};
	const char*	remainingTokens = list.c_str();
	while(remainingTokens) {
		if(0 == sscanf(remainingTokens, "%s", val))
			break;

		if(		!strcmp(val, "ModelNumber"))	cavity = new ModelNumber(cavity);
		else if(!strcmp(val, "Country"))		cavity = new Country(cavity);
		else if(!strcmp(val, "Date"))			cavity = new Date(cavity);
		// Seam point - add more tags.

		else {
			unknownTags << "    Ignoring unknown tag " << val << ".\n";
		}

		remainingTokens = strchr(remainingTokens+1, ' ');
	}

	return cavity;
}

class Polymer {
protected:
	unsigned volume_cc;
public:
	Polymer(unsigned volume_cc=0) : volume_cc(volume_cc) {}
	virtual ~Polymer() { DTORF("~dec::Polymer "); }
public:
	virtual unsigned mix() { return 0; }
	virtual string idNvol() { return ""; }
protected:
	 string volAsStr() {
		char vol[80];
		sprintf(vol, "(%d)", volume_cc);
		return vol;
	}
};
class Plastic : public Polymer {
public:
	Plastic(unsigned volume_cc=0) : Polymer(volume_cc) {}
	~Plastic() { DTORF("~Plastic "); }
};
class Additive : public Polymer {
protected:
	Polymer* delegate;
public:
	Additive(Polymer* delegate, unsigned volume_cc)
	  : Polymer(volume_cc), delegate(delegate) {}
	~Additive() { DTORF("~Additive "); delete delegate; }
};
class UVInhibiter : public Additive {
public:
	UVInhibiter(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	~UVInhibiter() { DTORF("~UVInhibiter "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
	string idNvol() { return delegate->idNvol() + " + UVInhibiter" + volAsStr(); }
};
class AntiBacterial : public Additive {
public:
	AntiBacterial(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	~AntiBacterial() { DTORF("~AntiBacterial "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
	string idNvol() { return delegate->idNvol() + " + AntiBacterial" + volAsStr(); }
};
class Hydrophilic : public Additive {
public:
	Hydrophilic(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	~Hydrophilic() { DTORF("~Hydrophilic "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
	string idNvol() { return delegate->idNvol() + " + Hydrophilic" + volAsStr(); }
};
// Seam point - add another option.

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

	return additive;
}

} // decorator_1
namespace decorator_2 {			// DP 6.

class Cavity {	// If the options are varying...
public:
	const unsigned space_mm;
public:
	Cavity(unsigned width_mm=20)
	  : space_mm(width_mm)
	{}
	virtual ~Cavity() { DTORF("~dec::Cavity "); }
public:
	virtual unsigned width_mm() { return 0; }
	virtual string list() { return ""; }
public:
	unsigned computeBlankWidth(unsigned tags) {
		return(space_mm - tags);
	}
public:
	static stringstream unknownTags;
	static Cavity* addTags(Cavity* cavity, const string& list);
};
class Blank : public Cavity {
public:
	Blank(unsigned width_mm=20) : Cavity(width_mm) {}
	~Blank() { DTORF("~Blank "); }
public:
	virtual unsigned width_mm() { return 0; }
	string list() { return ""; }
};
class Tags : public Cavity {
protected:
	Cavity* delegate;
public:
	Tags(Cavity* delegate) : delegate(delegate) {}
	~Tags() { DTORF("~Tags "); delete delegate; }
};
class ModelNumber : public Tags {
public:
	ModelNumber(Cavity* delegate) : Tags(delegate) {}
	~ModelNumber() { DTORF("~ModelNumber "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "ModelNumber "; }
};
class Country : public Tags {
public:
	Country(Cavity* delegate) : Tags(delegate) {}
	~Country() { DTORF("~Country "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "Country "; }
};
class Date : public Tags {
public:
	Date(Cavity* delegate) : Tags(delegate) {}
	~Date() { DTORF("~Date "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "Date "; }
};
// Seam point - add another option.
class IncCounter : public Tags {
public:
	IncCounter(Cavity* delegate) : Tags(delegate) {}
	~IncCounter() { DTORF("~IncCounter "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 4; }
	string list() { return delegate->list() + "IncCounter "; }
};
class PartNumber : public Tags {
public:
	PartNumber(Cavity* delegate) : Tags(delegate) {}
	~PartNumber() { DTORF("~PartNumber "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "PartNumber "; }
};
class RecycleCode : public Tags {
public:
	RecycleCode(Cavity* delegate) : Tags(delegate) {}
	~RecycleCode() { DTORF("~RecycleCode "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 6; }
	string list() { return delegate->list() + "RecycleCode "; }
};

stringstream Cavity::unknownTags;
Cavity* Cavity::addTags(Cavity* cavity, const string& list) {
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
			unknownTags << "    Ignoring unknown tag " << val << ".\n";
		}

		remainingTokens = strchr(remainingTokens+1, ' ');
	}

	return cavity;
}

class Polymer {
protected:
	unsigned volume_cc;
public:
	Polymer(unsigned volume_cc=0) : volume_cc(volume_cc) {}
	virtual ~Polymer() { DTORF("~dec::Polymer "); }
public:
	virtual unsigned mix() { return 0; }
	virtual string idNvol() { return ""; }
protected:
	 string volAsStr() {
		char vol[80];
		sprintf(vol, "(%d)", volume_cc);
		return vol;
	}
};
class Plastic : public Polymer {
public:
	Plastic(unsigned volume_cc=0) : Polymer(volume_cc) {}
	~Plastic() { DTORF("~Plastic "); }
};
class Additive : public Polymer {
protected:
	Polymer* delegate;
public:
	Additive(Polymer* delegate, unsigned volume_cc)
	  : Polymer(volume_cc), delegate(delegate) {}
	~Additive() { DTORF("~Additive "); delete delegate; }
};
class UVInhibiter : public Additive {
public:
	UVInhibiter(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	~UVInhibiter() { DTORF("~UVInhibiter "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
	string idNvol() { return delegate->idNvol() + " + UVInhibiter" + volAsStr(); }
};
class AntiBacterial : public Additive {
public:
	AntiBacterial(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	~AntiBacterial() { DTORF("~AntiBacterial "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
	string idNvol() { return delegate->idNvol() + " + AntiBacterial" + volAsStr(); }
};
class Hydrophilic : public Additive {
public:
	Hydrophilic(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	~Hydrophilic() { DTORF("~Hydrophilic "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
	string idNvol() { return delegate->idNvol() + " + Hydrophilic" + volAsStr(); }
};
// Seam point - add another option.
class MicroFibers : public Additive {
public:
	MicroFibers(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	~MicroFibers() { DTORF("~MicroFibers "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
	string idNvol() { return delegate->idNvol() + " + MicroFibers" + volAsStr(); }
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

} // decorator_2

namespace template_method {		// DP 3.

/* Execution lines:
 * C:\Users\adgoffx\git\DP12>
 * Debug\DP12 final part1 src\final1week\In\orders_2.txt > src\Final1week\Out\part1on2_9.out
 * Debug\DP12 final part2 src\final1week\In\orders_2.txt > src\Final1week\Out\part2on2_9.out
 */

/* Instruction Steps:
 * 1. Collapse all, unfold template_method
 * 2. Open all four sessions in Beyond Compare
 * 3. P0: Synthetics & clean:
 *        Unfold getProcessOrder
 *        Unfold adapter namespace and FM
 *        Compare Final1week/PC/processOrder9.h Final1week/Solution/processOrder0.h
 *        Compare Final1week/Out/part1on2_0.out to part2on2_0.out
 * 4. P1: Ship to:
 *        Compare Final1week/Solution/processOrder1.h Final1week/Solution/processOrder0.h
 *        Compare Final1week/Out/part2on2_1.out to part2on2_0.out
 * 5. P2: Setup line:
 *        Unfold abstract_factory_2 namespace and FM
 *        NoChange Final1week/Solution/processOrder1.h Final1week/Solution/processOrder2.h
 *        Compare Final1week/Out/part2on2_1.out to part2on2_2.out
 *        Compare abstractFactory1.h abstractFactory2.h
 * 6. P3: Packagers & Stuffers:
 *        Unfold factory_method_2 namespace and FMs
 *        Compare Final1week/Solution/processOrder3.h Final1week/Solution/processOrder2.h
 *        Compare Final1week/Out/part2on2_3.out to part2on2_2.out
 *        Compare factoryMethod1.h factoryMethod2.h
 * 7. P4: Mold shapes & locations:
 *        Unfold bridge_2 namespace and FMs
 *        Unfold chain_of_resp_2 namespace and FM
 *        NoChange Final1week/Solution/processOrder3.h Final1week/Solution/processOrder4.h
 *        Compare Final1week/Out/part2on2_3.out to part2on2_4.out
 *        Compare bridge1.h bridge2.h
 *        Compare chainOfResp1.h chainOfResp2.h
 * 8. P5: Tags & Additives:
 *        Unfold decorator_2 namespace and FMs
 *        NoChange Final1week/Solution/processOrder5.h Final1week/Solution/processOrder4.h
 *        Compare Final1week/Out/part2on2_5.out to part2on2_4.out
 *        Compare decorator1.h decorator2.h
 * 9. P6: Runtime estimates:
 *        Unfold strategy_2 namespace and FM
 *        NoChange Final1week/Solution/processOrder5.h Final1week/Solution/processOrder6.h
 *        Compare Final1week/Out/part2on2_5.out to part2on2_6.out
 *        Compare strategy1.h strategy2.h
 * 10:    Part1 versus Part2:
 *        Compare finalPart1.h finalPart2.h
 */

#define P6	// Successively replace with 1,2,3,...,6.

#ifdef P0 // Synthetics & clean.
#define abstract_factory	abstract_factory_1
#define factory_method		factory_method_1
#define bridge				bridge_1
#define chain_of_resp		chain_of_resp_1
#define decorator			decorator_1
#define strategy			strategy_1
#include "Solution/processOrder0.h" // Synthetics & clean.
#endif
#ifdef P1 // Ship to.
#define abstract_factory	abstract_factory_1
#define factory_method		factory_method_1
#define bridge				bridge_1
#define chain_of_resp		chain_of_resp_1
#define decorator			decorator_1
#define strategy			strategy_1
#include "Solution/processOrder1.h" // Ship to.
#endif
#ifdef P2 // Setup line.
#define abstract_factory	abstract_factory_2
#define factory_method		factory_method_1
#define bridge				bridge_1
#define chain_of_resp		chain_of_resp_1
#define decorator			decorator_1
#define strategy			strategy_1
#include "Solution/processOrder2.h" // Setup line.
#endif
#ifdef P3 // Packagers & Stuffers.
#define abstract_factory	abstract_factory_2
#define factory_method		factory_method_2
#define bridge				bridge_1
#define chain_of_resp		chain_of_resp_1
#define decorator			decorator_1
#define strategy			strategy_1
#include "Solution/processOrder3.h" // Packagers & Stuffers.
#endif
#ifdef P4 // Mold shapes & locations.
#define abstract_factory	abstract_factory_2
#define factory_method		factory_method_2
#define bridge				bridge_2
#define chain_of_resp		chain_of_resp_2
#define decorator			decorator_1
#define strategy			strategy_1
#include "Solution/processOrder4.h" // Mold shapes & locations.
#endif
#ifdef P5 // Tags & Additives.
#define abstract_factory	abstract_factory_2
#define factory_method		factory_method_2
#define bridge				bridge_2
#define chain_of_resp		chain_of_resp_2
#define decorator			decorator_2
#define strategy			strategy_1
#include "Solution/processOrder5.h" // Tags & Additives.
#endif
#ifdef P6 // Runtime estimates.
#define abstract_factory	abstract_factory_2
#define factory_method		factory_method_2
#define bridge				bridge_2
#define chain_of_resp		chain_of_resp_2
#define decorator			decorator_2
#define strategy			strategy_2
#include "Solution/processOrder6.h" // Runtime estimates.
#endif

#ifdef O // Can (and should) delete all these except the last.
class ProcessOrderP0 { // Synthetics & clean.
protected:
	adapter::CleanMold*					cleaning;
	abstract_factory::InjectionLine*	injectionLine;
	abstract_factory::IJM*				ijm;
	abstract_factory::Block*			block;
	abstract_factory::ConveyerBelt*		belt;
	abstract_factory::PartsBin*			bin;	// Inherits from observer::BinSubject.
	factory_method::Packager*			packager;
	bridge::Shape*						shape;
	chain_of_resp::Mold*				mold;
	decorator::Cavity*					tags;
	decorator::Polymer*					additives;
	strategy::RuntimeEstimate*			runtimeEst;
public:
	ProcessOrderP0()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, packager(0)
		, shape(0)
		, mold(0)
		, tags(0)
		, additives(0)
		, runtimeEst(0)
	{}
	virtual ~ProcessOrderP0() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete bin;
		delete injectionLine;
		delete shape;
		delete mold; cout << "\n";
		delete tags; cout << "\n";
		delete additives; cout << "\n";
		delete runtimeEst;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);					// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
	}
protected: // Template Method methods.
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method;
		using namespace abstract_factory;

		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);	// Observer.

		packager = Packager::makeObject(order,bin);			// FM & Observer.

		cout << "  Setup injection line for "
			 << order["size"] << " order";
		if(packager)
			cout << " with " << packager->wrap() << " packager";
		cout << ":\n";

		cout << "    "
			 << ijm->setup() << " - "
			 << block->setup() << " - "
			 << belt->setup() << " - "
			 << bin->setup() << ".\n";

		order["metal"] = block->metal();

		stringstream cavities;
		cavities << block->cavities;
		order["cavities"] = cavities.str();
	}
	void getMold(map<string,string>& order) {
		using namespace bridge_1;
		using namespace chain_of_resp_1;

		mold = Mold::acquireMold(order);

		shape = mold->from(order);	// Volume_cc.
		stringstream volume;
		volume << shape->volume_cc;
		order["volume"] = volume.str();
	}
	void insertTags(map<string,string>& order) {
		using namespace decorator_1;

		tags = new Blank();

		string list = "";
		if(order.find("tags") != order.end()) {
			list = order["tags"];
			tags = Cavity::addTags(tags, list);
		}

		string tagString = tags->list();		// Strip trailing blank.
		int size = tagString.size();
		if(size) tagString.erase(tagString.size()-1, 1);

		cout << "  Insert tags [" << tagString << "]"
			 << " of width "<< tags->width_mm() << "/"
			 << tags->space_mm << " mm, blank tag is "
			 << tags->computeBlankWidth(tags->width_mm()) << " mm.\n";
		cout << Cavity::unknownTags.str();

		Cavity::unknownTags.str("");	// Clear.
	}
	virtual void loadBins(map<string,string>& order) {
		using namespace decorator_1;

		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}

		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with " << order["color"] << ".\n";

		int cavities = block->cavities;

		additives = addAdditives(new Plastic, order);

		int shapeVol = shape->volume_cc;
		int colorVol = 0.10*shapeVol;
		int plasticVol = shapeVol - colorVol - additives->mix();

		int totalVol = cavities*shapeVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") + "
			 << order["color"] << "(" << colorVol << ")"
			 << additives->idNvol() << " = " << shapeVol << " cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: " << order["mold"] << "(" << shapeVol << ") * "
			 << cavities << plural
			 << "= " << totalVol << " cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		using namespace strategy_1;

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
		cout << "    Close - heat to <temp> - inject at <pressure> PSI"
			 << " - cool to <temp> - separate - <manner of> eject.\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		using namespace adapter;

		cleaning = CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		int cavities = block->cavities;
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/cavities;

		cout << "  Cycle " << ijm->setup() << " for "
			 << order["plastic"] << " " << runSize << " times.\n";
	}
};
class ProcessOrderP1 { // Ship to.
protected:
	adapter::CleanMold*					cleaning;
	abstract_factory::InjectionLine*	injectionLine;
	abstract_factory::IJM*				ijm;
	abstract_factory::Block*			block;
	abstract_factory::ConveyerBelt*		belt;
	abstract_factory::PartsBin*			bin;	// Inherits from observer::BinSubject.
	factory_method::Packager*			packager;
	bridge::Shape*						shape;
	chain_of_resp::Mold*				mold;
	decorator::Cavity*					tags;
	decorator::Polymer*					additives;
	strategy::RuntimeEstimate*			runtimeEst;
public:
	ProcessOrderP1()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, packager(0)
		, shape(0)
		, mold(0)
		, tags(0)
		, additives(0)
		, runtimeEst(0)
	{}
	virtual ~ProcessOrderP1() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete bin;
		delete injectionLine;
		delete shape;
		delete mold; cout << "\n";
		delete tags; cout << "\n";
		delete additives; cout << "\n";
		delete runtimeEst;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);					// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
		ship(order);						// 4 - Template Method
	}
protected: // Template Method methods.
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method;
		using namespace abstract_factory;

		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);	// Observer.

		packager = Packager::makeObject(order,bin);			// FM & Observer.

		cout << "  Setup injection line for "
			 << order["size"] << " order";
		if(packager)
			cout << " with " << packager->wrap() << " packager";
		cout << ":\n";

		cout << "    "
			 << ijm->setup() << " - "
			 << block->setup() << " - "
			 << belt->setup() << " - "
			 << bin->setup() << ".\n";

		order["metal"] = block->metal();

		stringstream cavities;
		cavities << block->cavities;
		order["cavities"] = cavities.str();
	}
	void getMold(map<string,string>& order) {
		using namespace bridge_1;
		using namespace chain_of_resp_1;

		mold = Mold::acquireMold(order);

		shape = mold->from(order);	// Volume_cc.
		stringstream volume;
		volume << shape->volume_cc;
		order["volume"] = volume.str();
	}
	void insertTags(map<string,string>& order) {
		using namespace decorator_1;

		tags = new Blank();

		string list = "";
		if(order.find("tags") != order.end()) {
			list = order["tags"];
			tags = Cavity::addTags(tags, list);
		}

		string tagString = tags->list();		// Strip trailing blank.
		int size = tagString.size();
		if(size) tagString.erase(tagString.size()-1, 1);

		cout << "  Insert tags [" << tagString << "]"
			 << " of width "<< tags->width_mm() << "/"
			 << tags->space_mm << " mm, blank tag is "
			 << tags->computeBlankWidth(tags->width_mm()) << " mm.\n";
		cout << Cavity::unknownTags.str();

		Cavity::unknownTags.str("");	// Clear.
	}
	virtual void loadBins(map<string,string>& order) {
		using namespace decorator_1;

		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}

		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with " << order["color"] << ".\n";

		int cavities = block->cavities;

		additives = addAdditives(new Plastic, order);

		int shapeVol = shape->volume_cc;
		int colorVol = 0.10*shapeVol;
		int plasticVol = shapeVol - colorVol - additives->mix();

		int totalVol = cavities*shapeVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") + "
			 << order["color"] << "(" << colorVol << ")"
			 << additives->idNvol() << " = " << shapeVol << " cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: " << order["mold"] << "(" << shapeVol << ") * "
			 << cavities << plural
			 << "= " << totalVol << " cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		using namespace strategy_1;

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
		cout << "    Close - heat to <temp> - inject at <pressure> PSI"
			 << " - cool to <temp> - separate - <manner of> eject.\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		using namespace adapter;

		cleaning = CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
	void ship(map<string,string>& order) {
		if(order.find("address") != order.end()) {
			cout << "  Ship to \"" << order["address"] << "\".\n";
		}
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		int cavities = block->cavities;
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/cavities;

		cout << "  Cycle " << ijm->setup() << " for "
			 << order["plastic"] << " " << runSize << " times.\n";
	}
};
class ProcessOrderP2 { // Setup line.
protected:
	adapter::CleanMold*					cleaning;
	abstract_factory_2::InjectionLine*	injectionLine;
	abstract_factory_2::IJM*			ijm;
	abstract_factory_2::Block*			block;
	abstract_factory_2::ConveyerBelt*	belt;
	abstract_factory_2::PartsBin*		bin;	// Inherits from observer::BinSubject.
	factory_method_1::Packager*			packager;
	bridge_1::Shape*					shape;
	chain_of_resp_1::Mold*				mold;
	decorator_1::Cavity*				tags;
	decorator_1::Polymer*				additives;
	strategy_1::RuntimeEstimate*		runtimeEst;
public:
	ProcessOrderP2()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, packager(0)
		, shape(0)
		, mold(0)
		, tags(0)
		, additives(0)
		, runtimeEst(0)
	{}
	virtual ~ProcessOrderP2() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete bin;
		delete injectionLine;
		delete shape;
		delete mold; cout << "\n";
		delete tags; cout << "\n";
		delete additives; cout << "\n";
		delete runtimeEst;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);					// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
		ship(order);						// 4 - Template Method
	}
protected: // Template Method methods.
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method_1;
		using namespace abstract_factory_2;

		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);	// Observer.

		packager = Packager::makeObject(order,bin);			// FM & Observer.

		cout << "  Setup injection line for "
			 << order["size"] << " order";
		if(packager)
			cout << " with " << packager->wrap() << " packager";
		cout << ":\n";

		cout << "    "
			 << ijm->setup() << " - "
			 << block->setup() << " - "
			 << belt->setup() << " - "
			 << bin->setup() << ".\n";

		order["metal"] = block->metal();

		stringstream cavities;
		cavities << block->cavities;
		order["cavities"] = cavities.str();
	}
	void getMold(map<string,string>& order) {
		using namespace bridge_1;
		using namespace chain_of_resp_1;

		mold = Mold::acquireMold(order);

		shape = mold->from(order);	// Volume_cc.
		stringstream volume;
		volume << shape->volume_cc;
		order["volume"] = volume.str();
	}
	void insertTags(map<string,string>& order) {
		using namespace decorator_1;

		tags = new Blank();

		string list = "";
		if(order.find("tags") != order.end()) {
			list = order["tags"];
			tags = Cavity::addTags(tags, list);
		}

		string tagString = tags->list();		// Strip trailing blank.
		int size = tagString.size();
		if(size) tagString.erase(tagString.size()-1, 1);

		cout << "  Insert tags [" << tagString << "]"
			 << " of width "<< tags->width_mm() << "/"
			 << tags->space_mm << " mm, blank tag is "
			 << tags->computeBlankWidth(tags->width_mm()) << " mm.\n";
		cout << Cavity::unknownTags.str();

		Cavity::unknownTags.str("");	// Clear.
	}
	virtual void loadBins(map<string,string>& order) {
		using namespace decorator_1;

		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}

		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with " << order["color"] << ".\n";

		int cavities = block->cavities;

		additives = addAdditives(new Plastic, order);

		int shapeVol = shape->volume_cc;
		int colorVol = 0.10*shapeVol;
		int plasticVol = shapeVol - colorVol - additives->mix();

		int totalVol = cavities*shapeVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") + "
			 << order["color"] << "(" << colorVol << ")"
			 << additives->idNvol() << " = " << shapeVol << " cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: " << order["mold"] << "(" << shapeVol << ") * "
			 << cavities << plural
			 << "= " << totalVol << " cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		using namespace strategy_1;

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
		cout << "    Close - heat to <temp> - inject at <pressure> PSI"
			 << " - cool to <temp> - separate - <manner of> eject.\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		using namespace adapter;

		cleaning = CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
	void ship(map<string,string>& order) {
		if(order.find("address") != order.end()) {
			cout << "  Ship to \"" << order["address"] << "\".\n";
		}
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		int cavities = block->cavities;
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/cavities;

		cout << "  Cycle " << ijm->setup() << " for "
			 << order["plastic"] << " " << runSize << " times.\n";
	}
};
class ProcessOrderP3 { // Packagers & Stuffers.
protected:
	adapter::CleanMold*					cleaning;
	abstract_factory_2::InjectionLine*	injectionLine;
	abstract_factory_2::IJM*			ijm;
	abstract_factory_2::Block*			block;
	abstract_factory_2::ConveyerBelt*	belt;
	abstract_factory_2::PartsBin*		bin;	// Inherits from observer::BinSubject.
	factory_method_2::Packager*			packager;
	factory_method_2::Stuffer*			cushion;	// Specs 2.
	bridge_1::Shape*					shape;
	chain_of_resp_1::Mold*				mold;
	decorator_1::Cavity*				tags;
	decorator_1::Polymer*				additives;
	strategy_1::RuntimeEstimate*		runtimeEst;
public:
	ProcessOrderP3()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, packager(0)
		, cushion(0)	// Specs 2.
		, shape(0)
		, mold(0)
		, tags(0)
		, additives(0)
		, runtimeEst(0)
	{}
	virtual ~ProcessOrderP3() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete cushion;
		delete bin;
		delete injectionLine;
		delete shape;
		delete mold; cout << "\n";
		delete tags; cout << "\n";
		delete additives; cout << "\n";
		delete runtimeEst;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);					// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
		ship(order);						// 4 - Template Method
	}
protected: // Template Method methods.
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method_2;
		using namespace abstract_factory_2;

		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);	// Observer.

		packager = Packager::makeObject(order,bin);			// FM & Observer.
		cushion = Stuffer::makeObject(order, bin);			// FM & O - Specs 2.

		cout << "  Setup injection line for "
			 << order["size"] << " order";
		if(packager || cushion)
			cout << " with ";
		if(packager)
			cout << packager->wrap() << " packager";
		// Seam line - add another observer machine (output bin stuffer).
		if(packager && cushion)
			cout << " and ";
		if(cushion)
			cout << cushion->fill() << " stuffer";
		cout << ":\n";

		cout << "    "
			 << ijm->setup() << " - "
			 << block->setup() << " - "
			 << belt->setup() << " - "
			 << bin->setup() << ".\n";

		order["metal"] = block->metal();

		stringstream cavities;
		cavities << block->cavities;
		order["cavities"] = cavities.str();
	}
	void getMold(map<string,string>& order) {
		using namespace bridge_1;
		using namespace chain_of_resp_1;

		mold = Mold::acquireMold(order);

		shape = mold->from(order);	// Volume_cc.
		stringstream volume;
		volume << shape->volume_cc;
		order["volume"] = volume.str();
	}
	void insertTags(map<string,string>& order) {
		using namespace decorator_1;

		tags = new Blank();

		string list = "";
		if(order.find("tags") != order.end()) {
			list = order["tags"];
			tags = Cavity::addTags(tags, list);
		}

		string tagString = tags->list();		// Strip trailing blank.
		int size = tagString.size();
		if(size) tagString.erase(tagString.size()-1, 1);

		cout << "  Insert tags [" << tagString << "]"
			 << " of width "<< tags->width_mm() << "/"
			 << tags->space_mm << " mm, blank tag is "
			 << tags->computeBlankWidth(tags->width_mm()) << " mm.\n";
		cout << Cavity::unknownTags.str();

		Cavity::unknownTags.str("");	// Clear.
	}
	virtual void loadBins(map<string,string>& order) {
		using namespace decorator_1;

		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}

		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with " << order["color"] << ".\n";

		int cavities = block->cavities;

		additives = addAdditives(new Plastic, order);

		int shapeVol = shape->volume_cc;
		int colorVol = 0.10*shapeVol;
		int plasticVol = shapeVol - colorVol - additives->mix();

		int totalVol = cavities*shapeVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") + "
			 << order["color"] << "(" << colorVol << ")"
			 << additives->idNvol() << " = " << shapeVol << " cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: " << order["mold"] << "(" << shapeVol << ") * "
			 << cavities << plural
			 << "= " << totalVol << " cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		using namespace strategy_1;

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
		cout << "    Close - heat to <temp> - inject at <pressure> PSI"
			 << " - cool to <temp> - separate - <manner of> eject.\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		using namespace adapter;

		cleaning = CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
	void ship(map<string,string>& order) {
		if(order.find("address") != order.end()) {
			cout << "  Ship to \"" << order["address"] << "\".\n";
		}
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		int cavities = block->cavities;
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/cavities;

		cout << "  Cycle " << ijm->setup() << " for "
			 << order["plastic"] << " " << runSize << " times.\n";
	}
};
class ProcessOrderP4 { // Mold shapes & locations.
protected:
	adapter::CleanMold*					cleaning;
	abstract_factory_2::InjectionLine*	injectionLine;
	abstract_factory_2::IJM*			ijm;
	abstract_factory_2::Block*			block;
	abstract_factory_2::ConveyerBelt*	belt;
	abstract_factory_2::PartsBin*		bin;	// Inherits from observer::BinSubject.
	factory_method_2::Packager*			packager;
	factory_method_2::Stuffer*			cushion;	// Specs 2.
	bridge_2::Shape*					shape;
	chain_of_resp_2::Mold*				mold;
	decorator_1::Cavity*				tags;
	decorator_1::Polymer*				additives;
	strategy_1::RuntimeEstimate*		runtimeEst;
public:
	ProcessOrderP4()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, packager(0)
		, cushion(0)	// Specs 2.
		, shape(0)
		, mold(0)
		, tags(0)
		, additives(0)
		, runtimeEst(0)
	{}
	virtual ~ProcessOrderP4() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete cushion;
		delete bin;
		delete injectionLine;
		delete shape;
		delete mold; cout << "\n";
		delete tags; cout << "\n";
		delete additives; cout << "\n";
		delete runtimeEst;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);					// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
		ship(order);						// 4 - Template Method
	}
protected: // Template Method methods.
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method_2;
		using namespace abstract_factory_2;

		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);	// Observer.

		packager = Packager::makeObject(order,bin);			// FM & Observer.
		cushion = Stuffer::makeObject(order, bin);			// FM & O - Specs 2.

		cout << "  Setup injection line for "
			 << order["size"] << " order";
		if(packager || cushion)
			cout << " with ";
		if(packager)
			cout << packager->wrap() << " packager";
		// Seam line - add another observer machine (output bin stuffer).
		if(packager && cushion)
			cout << " and ";
		if(cushion)
			cout << cushion->fill() << " stuffer";
		cout << ":\n";

		cout << "    "
			 << ijm->setup() << " - "
			 << block->setup() << " - "
			 << belt->setup() << " - "
			 << bin->setup() << ".\n";

		order["metal"] = block->metal();

		stringstream cavities;
		cavities << block->cavities;
		order["cavities"] = cavities.str();
	}
	void getMold(map<string,string>& order) {
		using namespace bridge_2;
		using namespace chain_of_resp_2;

		mold = Mold::acquireMold(order);

		shape = mold->from(order);	// Volume_cc.
		stringstream volume;
		volume << shape->volume_cc;
		order["volume"] = volume.str();
	}
	void insertTags(map<string,string>& order) {
		using namespace decorator_1;

		tags = new Blank();

		string list = "";
		if(order.find("tags") != order.end()) {
			list = order["tags"];
			tags = Cavity::addTags(tags, list);
		}

		string tagString = tags->list();		// Strip trailing blank.
		int size = tagString.size();
		if(size) tagString.erase(tagString.size()-1, 1);

		cout << "  Insert tags [" << tagString << "]"
			 << " of width "<< tags->width_mm() << "/"
			 << tags->space_mm << " mm, blank tag is "
			 << tags->computeBlankWidth(tags->width_mm()) << " mm.\n";
		cout << Cavity::unknownTags.str();

		Cavity::unknownTags.str("");	// Clear.
	}
	virtual void loadBins(map<string,string>& order) {
		using namespace decorator_1;

		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}

		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with " << order["color"] << ".\n";

		int cavities = block->cavities;

		additives = addAdditives(new Plastic, order);

		int shapeVol = shape->volume_cc;
		int colorVol = 0.10*shapeVol;
		int plasticVol = shapeVol - colorVol - additives->mix();

		int totalVol = cavities*shapeVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") + "
			 << order["color"] << "(" << colorVol << ")"
			 << additives->idNvol() << " = " << shapeVol << " cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: " << order["mold"] << "(" << shapeVol << ") * "
			 << cavities << plural
			 << "= " << totalVol << " cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		using namespace strategy_1;

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
		cout << "    Close - heat to <temp> - inject at <pressure> PSI"
			 << " - cool to <temp> - separate - <manner of> eject.\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		using namespace adapter;

		cleaning = CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
	void ship(map<string,string>& order) {
		if(order.find("address") != order.end()) {
			cout << "  Ship to \"" << order["address"] << "\".\n";
		}
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		int cavities = block->cavities;
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/cavities;

		cout << "  Cycle " << ijm->setup() << " for "
			 << order["plastic"] << " " << runSize << " times.\n";
	}
};
class ProcessOrderP5 { // Tags & Additives.
protected:
	adapter::CleanMold*					cleaning;
	abstract_factory_2::InjectionLine*	injectionLine;
	abstract_factory_2::IJM*			ijm;
	abstract_factory_2::Block*			block;
	abstract_factory_2::ConveyerBelt*	belt;
	abstract_factory_2::PartsBin*		bin;	// Inherits from observer::BinSubject.
	factory_method_2::Packager*			packager;
	factory_method_2::Stuffer*			cushion;	// Specs 2.
	bridge_2::Shape*					shape;
	chain_of_resp_2::Mold*				mold;
	decorator_2::Cavity*				tags;
	decorator_2::Polymer*				additives;
	strategy_1::RuntimeEstimate*		runtimeEst;
public:
	ProcessOrderP5()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, packager(0)
		, cushion(0)	// Specs 2.
		, shape(0)
		, mold(0)
		, tags(0)
		, additives(0)
		, runtimeEst(0)
	{}
	virtual ~ProcessOrderP5() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete cushion;
		delete bin;
		delete injectionLine;
		delete shape;
		delete mold; cout << "\n";
		delete tags; cout << "\n";
		delete additives; cout << "\n";
		delete runtimeEst;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);					// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
		ship(order);						// 4 - Template Method
	}
protected: // Template Method methods.
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method_2;
		using namespace abstract_factory_2;

		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);	// Observer.

		packager = Packager::makeObject(order,bin);			// FM & Observer.
		cushion = Stuffer::makeObject(order, bin);			// FM & O - Specs 2.

		cout << "  Setup injection line for "
			 << order["size"] << " order";
		if(packager || cushion)
			cout << " with ";
		if(packager)
			cout << packager->wrap() << " packager";
		// Seam line - add another observer machine (output bin stuffer).
		if(packager && cushion)
			cout << " and ";
		if(cushion)
			cout << cushion->fill() << " stuffer";
		cout << ":\n";

		cout << "    "
			 << ijm->setup() << " - "
			 << block->setup() << " - "
			 << belt->setup() << " - "
			 << bin->setup() << ".\n";

		order["metal"] = block->metal();

		stringstream cavities;
		cavities << block->cavities;
		order["cavities"] = cavities.str();
	}
	void getMold(map<string,string>& order) {
		using namespace bridge_2;
		using namespace chain_of_resp_2;

		mold = Mold::acquireMold(order);

		shape = mold->from(order);	// Volume_cc.
		stringstream volume;
		volume << shape->volume_cc;
		order["volume"] = volume.str();
	}
	void insertTags(map<string,string>& order) {
		using namespace decorator_2;

		tags = new Blank();

		string list = "";
		if(order.find("tags") != order.end()) {
			list = order["tags"];
			tags = Cavity::addTags(tags, list);
		}

		string tagString = tags->list();		// Strip trailing blank.
		int size = tagString.size();
		if(size) tagString.erase(tagString.size()-1, 1);

		cout << "  Insert tags [" << tagString << "]"
			 << " of width "<< tags->width_mm() << "/"
			 << tags->space_mm << " mm, blank tag is "
			 << tags->computeBlankWidth(tags->width_mm()) << " mm.\n";
		cout << Cavity::unknownTags.str();

		Cavity::unknownTags.str("");	// Clear.
	}
	virtual void loadBins(map<string,string>& order) {
		using namespace decorator_2;

		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}

		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with " << order["color"] << ".\n";

		int cavities = block->cavities;

		additives = addAdditives(new Plastic, order);

		int shapeVol = shape->volume_cc;
		int colorVol = 0.10*shapeVol;
		int plasticVol = shapeVol - colorVol - additives->mix();

		int totalVol = cavities*shapeVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") + "
			 << order["color"] << "(" << colorVol << ")"
			 << additives->idNvol() << " = " << shapeVol << " cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: " << order["mold"] << "(" << shapeVol << ") * "
			 << cavities << plural
			 << "= " << totalVol << " cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		using namespace strategy_1;

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
		cout << "    Close - heat to <temp> - inject at <pressure> PSI"
			 << " - cool to <temp> - separate - <manner of> eject.\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		using namespace adapter;

		cleaning = CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
	void ship(map<string,string>& order) {
		if(order.find("address") != order.end()) {
			cout << "  Ship to \"" << order["address"] << "\".\n";
		}
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		int cavities = block->cavities;
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/cavities;

		cout << "  Cycle " << ijm->setup() << " for "
			 << order["plastic"] << " " << runSize << " times.\n";
	}
};
class ProcessOrderP6 { // Runtime estimates.
protected:
	adapter::CleanMold*					cleaning;
	abstract_factory_2::InjectionLine*	injectionLine;
	abstract_factory_2::IJM*			ijm;
	abstract_factory_2::Block*			block;
	abstract_factory_2::ConveyerBelt*	belt;
	abstract_factory_2::PartsBin*		bin;	// Inherits from observer::BinSubject.
	factory_method_2::Packager*			packager;
	factory_method_2::Stuffer*			cushion;	// Specs 2.
	bridge_2::Shape*					shape;
	chain_of_resp_2::Mold*				mold;
	decorator_2::Cavity*				tags;
	decorator_2::Polymer*				additives;
	strategy_2::RuntimeEstimate*		runtimeEst;
public:
	ProcessOrderP6()
		: cleaning(0)
		, injectionLine(0)
		, ijm(0)
		, block(0)
		, belt(0)
		, bin(0)
		, packager(0)
		, cushion(0)	// Specs 2.
		, shape(0)
		, mold(0)
		, tags(0)
		, additives(0)
		, runtimeEst(0)
	{}
	virtual ~ProcessOrderP6() {
		delete ijm;
		delete block;
		delete belt;
		delete packager;
		delete cushion;
		delete bin;
		delete injectionLine;
		delete shape;
		delete mold; cout << "\n";
		delete tags; cout << "\n";
		delete additives; cout << "\n";
		delete runtimeEst;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		cout << "Process order:\n";
		setupLine(order);					// 9 - Abstract Factory
		getMold(order);						// 7 - Chain of Responsibility, 8 - Bridge
		insertTags(order);					// 6 - Decorator
		loadBins(order);					// 6 - Decorator
		runtimeEstimate(order);				// 1 - Strategy
		injectionCycle(order);				// 4 - Template Method
		simulateFullPartsBin(order);		// 5 - Observer
		cleanMold(order);					// 2 - Adapter
		ship(order);						// 4 - Template Method
	}
protected: // Template Method methods.
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method_2;
		using namespace abstract_factory_2;

		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}

		injectionLine = InjectionLine::createInjectionLine(order);

		bin	  = injectionLine->createPartsBin(order);		// Observer Subject.

		ijm	  = injectionLine->createIJM(order,bin);			// Observer.
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order,bin);	// Observer.

		packager = Packager::makeObject(order,bin);			// FM & Observer.
		cushion = Stuffer::makeObject(order, bin);			// FM & O - Specs 2.

		cout << "  Setup injection line for "
			 << order["size"] << " order";
		if(packager || cushion)
			cout << " with ";
		if(packager)
			cout << packager->wrap() << " packager";
		// Seam line - add another observer machine (output bin stuffer).
		if(packager && cushion)
			cout << " and ";
		if(cushion)
			cout << cushion->fill() << " stuffer";
		cout << ":\n";

		cout << "    "
			 << ijm->setup() << " - "
			 << block->setup() << " - "
			 << belt->setup() << " - "
			 << bin->setup() << ".\n";

		order["metal"] = block->metal();

		stringstream cavities;
		cavities << block->cavities;
		order["cavities"] = cavities.str();
	}
	void getMold(map<string,string>& order) {
		using namespace bridge_2;
		using namespace chain_of_resp_2;

		mold = Mold::acquireMold(order);

		shape = mold->from(order);	// Volume_cc.
		stringstream volume;
		volume << shape->volume_cc;
		order["volume"] = volume.str();
	}
	void insertTags(map<string,string>& order) {
		using namespace decorator_2;

		tags = new Blank();

		string list = "";
		if(order.find("tags") != order.end()) {
			list = order["tags"];
			tags = Cavity::addTags(tags, list);
		}

		string tagString = tags->list();		// Strip trailing blank.
		int size = tagString.size();
		if(size) tagString.erase(tagString.size()-1, 1);

		cout << "  Insert tags [" << tagString << "]"
			 << " of width "<< tags->width_mm() << "/"
			 << tags->space_mm << " mm, blank tag is "
			 << tags->computeBlankWidth(tags->width_mm()) << " mm.\n";
		cout << Cavity::unknownTags.str();

		Cavity::unknownTags.str("");	// Clear.
	}
	virtual void loadBins(map<string,string>& order) {
		using namespace decorator_2;

		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}

		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with " << order["color"] << ".\n";

		int cavities = block->cavities;

		additives = addAdditives(new Plastic, order);

		int shapeVol = shape->volume_cc;
		int colorVol = 0.10*shapeVol;
		int plasticVol = shapeVol - colorVol - additives->mix();

		int totalVol = cavities*shapeVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") + "
			 << order["color"] << "(" << colorVol << ")"
			 << additives->idNvol() << " = " << shapeVol << " cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: " << order["mold"] << "(" << shapeVol << ") * "
			 << cavities << plural
			 << "= " << totalVol << " cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		using namespace strategy_2;

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
		cout << "    Close - heat to <temp> - inject at <pressure> PSI"
			 << " - cool to <temp> - separate - <manner of> eject.\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {
		using namespace adapter;

		cleaning = CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
	void ship(map<string,string>& order) {
		if(order.find("address") != order.end()) {
			cout << "  Ship to \"" << order["address"] << "\".\n";
		}
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		int cavities = block->cavities;
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/cavities;

		cout << "  Cycle " << ijm->setup() << " for "
			 << order["plastic"] << " " << runSize << " times.\n";
	}
};
#endif

class ABSOrder : public ProcessOrder {
public: ~ABSOrder() { DTORF("~ABSOrder\n"); }
public:
	void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to 440 - inject at 125 PSI"
			 << " - cool to 360 - separate - progressive eject.\n";
	}
};
class PropyleneOrder : public ProcessOrder {
public: ~PropyleneOrder() { DTORF("~PropyleneOrder\n"); }
public:
	void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to 350 - inject at 90 PSI"
			 << " - cool to 290 - separate - smooth eject.\n";
	}
};
class EtheleneOrder : public ProcessOrder {
public: ~EtheleneOrder() { DTORF("~EtheleneOrder\n"); }
public:
	void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to 360 - inject at 95 PSI"
			 << " - cool to 280 - separate - smooth eject.\n";
	}
};
class PETOrder : public ProcessOrder {
public: ~PETOrder() { DTORF("~PETOrder\n"); }
public:
	void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to 404 - inject at 110 PSI"
			 << " - cool to 340 - separate - smooth eject.\n";
	}
};
// Seam point - add another polymorphic step.
// Seam point - add another constant step.
// Seam point - convert a constant step into a polymorphic step.
class SyntheticOrder : public ProcessOrder {
public: ~SyntheticOrder() { DTORF("~SyntheticOrder\n"); }
public:
	void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to 480 - inject at 150 PSI"
			 << " - cool to 390 - separate - shock eject.\n";
	}
	void loadBins(map<string,string>& order) {
		using namespace decorator;

		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}

		cout << "  Load plastic bin with " << order["color"] << "-"
			 << order["plastic"] << ".\n";

		int cavities = block->cavities;

		additives = addAdditives(new Plastic, order);

		int shapeVol = shape->volume_cc;
		int plasticVol = shapeVol - additives->mix();

		int totalVol = cavities*shapeVol;

		cout << "    Recipe: " << order["color"] << "-"
			 << order["plastic"] << "(" << plasticVol << ")"
			 << additives->idNvol() << " = " << shapeVol << " cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: " << order["mold"] << "(" << shapeVol << ") * "
			 << cavities << plural
			 << "= " << totalVol << " cc.\n";
	}
};

ProcessOrder* getProcessOrder(map<string,string>& order) { // Factory Method(3).
	if(order["plastic"] == "ABS")				return new ABSOrder;
	if(order["plastic"] == "Polypropylene")		return new PropyleneOrder;
	if(order["plastic"] == "Polyethelene")		return new EtheleneOrder;
	if(order["plastic"] == "PET")				return new PETOrder;
	// Seam point - add another plastic.
	if(order["plastic"] == "Styrene")			return new SyntheticOrder;
	if(order["plastic"] == "Nylon66")			return new SyntheticOrder;

	legacy_classes::defaulting(order, "plastic", "ABS");
	return new ABSOrder;
}

} // template_method

void process(map<string,string>& order) {
	// Fill in the namespaces above with your design pattern class hierarchies.
	// Call your order processing class from here <myProcess>->run(order);

	using namespace template_method;

	ProcessOrder* processOrder = getProcessOrder(order);

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
	cout << "<<< final guided2 >>>\n";

	FILE* orderFilePtr = fopen(orderFile.c_str(), "r");

	for(;;) {	// Get and process each order.
		map<string,string> order = getCompleteOrder(orderFilePtr);
		if(order.size() == 0)
			break;
		process(order);
	}
}

} // final_guided2

#endif /* FINAL_GUIDED2_H */

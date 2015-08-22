/*
 * finalSolution.h
 *
 *  Created on: <MMDDCCYY>
 *      Author: <student name>
 */

#ifndef FINAL_FINAL_SOLUTION_H_
#define FINAL_FINAL_SOLUTION_H_

namespace final_solution {

/* Design Patterns Chart
 * x 1 - Strategy: runtime estimate (domain: order[size]) (depends: cavities, legacy, etc.)
 * x 2 - Adapter: cleaning tech (domain: order[plastic]) (depends: metal)
 * x 3 - Template Method: injectionLine & process (domain: order[plastic]) (depends: 1,2,4,5,6,7,8,9))
 * x 4 - Factory Methods: (Observer)
 * 			packaging (domain: order[packager])
 * 			stuffing (domain: order[stuffer])
 * x 5 - Observer: Subject(bins) Observers(IJM, conveyer belt, (domains: packager, stuffer)
 * x 6 - Decorator:
 * 			tags (domain: order[tags])
 * 			additives (domain: order[<additives>])
 * x 7 - ChainOfR: (domain: order[moldLoc] - inventory, borrow, mill, purchase)
 * x 8 - Bridge: mill
 * 			Platform: (domains: metal, order[finish])
 * 			Shape: (domain: order[mold])
 * x 9 - Abstract Factory: IJM, mold(metal,cavities), conveyer belt, bins (domain: order[size])
 */

/* Command line execution:
 * From C:\Users\adgoff\git\DP12>
 * Debug\DP12.exe final solution src\Final\In\orders_2.txt > src\Final\Out\solution2.out
 * [design|DtoO|original|OtoS|solution] [orders_1.txt|orders_2.txt]
 * [design1.out|original1.out|original2.out|solution2.out]
 * [design1Base.out|original1Base.out|solution2Base.out]
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
const float rejectRateLow_pcnt = 0.4;
const float rejectRateHigh_pcnt = 1.2;

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
public:
	static CleanMold* getCleaning(map<string,string>& order);
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

CleanMold* CleanMold::getCleaning(map<string,string>& order) {
	if(		order["plastic"] == "ABS")				return new ABS;
	else if(order["plastic"] == "Polypropylene")	return new Poly;
	else if(order["plastic"] == "Polyethelene")		return new Poly;
	else if(order["plastic"] == "PET")				return new PET;
	// Seam point - add another plastic.
	else if(order["plastic"] == "Styrene")			return new Synthetic;
	else if(order["plastic"] == "Nylon66")			return new Synthetic;

	else {
		cout << "*** Should never happen - CleanMold::getCleaning.\n";
		return new CleanMold;
	}
}

}

namespace strategy {		// DP 1 - done.

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
		cavities = atoi(order["cavities"].c_str());	// Added to order by TM.setupLine();
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
class Historical : public RuntimeEstimate {
public:
	Historical() {}
	~Historical() { DTORF("~Historical "); }
public:
	virtual int operator()(map<string,string>& order) {
		using namespace legacy_classes;
		RuntimeEstimate::runTimeEst_hrs(order);
		int volume_cc = atoi(order["volume"].c_str());
		int cycleTime = cycleTime_sec(order["metal"], volume_cc);
		return (setupAvg_min + (orderSize/cavities)*cycleTime/60 + teardownAvg_min)/60;
	}
};
// Seam point - add another algorithm.
class Projection : public RuntimeEstimate {
public:
	Projection() {}
	~Projection() { DTORF("~Projection "); }
public:
	virtual int operator()(map<string,string>& order) {
		using namespace legacy_classes;
		RuntimeEstimate::runTimeEst_hrs(order);
		int volume_cc = atoi(order["volume"].c_str());
		int cycleTime = cycleTime_sec(order["metal"], volume_cc);
		float rejectRate_pcnt = (order["plastic"] == "PET") ? rejectRateHigh_pcnt : rejectRateLow_pcnt;
		return (setupAvg_min
			 + (1 + 0.01*rejectRate_pcnt)*(orderSize/cavities)*cycleTime/60
			 + teardownAvg_min)/60;
	}
};

RuntimeEstimate* RuntimeEstimate::selectEstimationAlgorithm(map<string,string>& order) {
	int rteSizeTree = atoi(order["size"].c_str());

	if(		rteSizeTree <=  10000)	return new BackOfTheEnvelope;
	else if(rteSizeTree <=  25000)	return new Calculation;
	else if(rteSizeTree <=  50000)	return new Historical;
	// Seam point - add another runtime estimation algorithm.
	else if(rteSizeTree <= 100000)	return new Projection;

	else {
		cout << "    <>Using most sophisticated estimation algorithm";
		cout << " for orders greater than 100000.\n";
		return new Projection;
	}
}

}

namespace observer {		// DP 5 - done.

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
	cout << "      Whenever " << name << " package bin was full...\n";
	list<BinObserver*>::iterator it;
	for(it=obs.begin(); it!=obs.end(); ++it) {
		(*it)->update(this);
	}
}

}

namespace abstract_factory {// DP 9 - done.

using namespace observer;

// Seam point - add another type 1.
// Seam point - add another type 2.
// Seam point - add another type 3.
// Seam point - add another ...
// Seam point - add another type N.

class IJM : public BinObserver {
public:
	IJM(BinSubject* bin) : BinObserver(bin) {}
	virtual ~IJM() { DTORF("~IJM "); }
public:
	void update(BinSubject* bin) {
		cout << "        " << setup() << " paused while ";
		cout << bin->name << " package bin was swapped.\n";
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
// Seam point - add another Injection Molding machine.
class IJM_140 : public IJM {					// FastOrder.
public:
	IJM_140(BinSubject* bin) : IJM(bin) {}
	~IJM_140() { DTORF("~IJM_140 "); }
public:
	string setup() { return "IJM_140"; }
};
class IJM_220 : public IJM {					// LargeOrder.
public:
	IJM_220(BinSubject* bin) : IJM(bin) {}
	~IJM_220() { DTORF("~IJM_220 "); }
public:
	string setup() { return "IJM_220"; }
};
class IJM_240 : public IJM {					// HugeOrder.
public:
	IJM_240(BinSubject* bin) : IJM(bin) {}
	~IJM_240() { DTORF("~IJM_240 "); }
public:
	string setup() { return "IJM_240"; }
};

class Mold {
	unsigned m_cavities;
public:
	Mold(unsigned cavities=0) : m_cavities(cavities) {}
	virtual ~Mold() { DTORF("~Mold\n"); }
public:
	virtual string setup() { return "Mold base"; }
	virtual string metal() { return "unobtainium"; }
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
	~Aluminum() { DTORF("~Aluminum "); }
public:
	string setup() { return "Aluminum" + cavitiesAsString(); }
	virtual string metal() { return "aluminum"; }
};
class Steel : public Mold {
public:
	Steel(unsigned cavities=0) : Mold(cavities) {}
	~Steel() { DTORF("~Steel "); }
public:
	string setup() { return "Steel" + cavitiesAsString(); }
	virtual string metal() { return "steel"; }
};

class ConveyerBelt : public BinObserver {
public:
	ConveyerBelt(BinSubject* bin) : BinObserver(bin) {}
	virtual ~ConveyerBelt() { DTORF("~ConveyerBelt "); }
public:
public:
	void update(BinSubject* bin) {
		cout << "        " << setup() << " paused while ";
		cout << bin->name << " package bin was swapped.\n";
	}
	virtual string setup() { return "ConveyerBelt base"; }
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
class VLevelBelt : public ConveyerBelt {
public:
	VLevelBelt(BinSubject* bin) : ConveyerBelt(bin) {}
	~VLevelBelt() { DTORF("~VLevelBelt "); }
public:
	string setup() {return "V-Level conveyer belt"; }
};

class PackageBin : public BinSubject {
public:
	PackageBin(const string name) : BinSubject(name) {}
	virtual ~PackageBin() { DTORF("~PackageBin "); }
public:
	virtual string setup() { return "PackageBin base"; }
};
class CardboardBox : public PackageBin {
public:
	CardboardBox() : PackageBin("CardboardBox") {}
	~CardboardBox() { DTORF("~CardboardBox "); }
public:
	string setup() { return "CardboardBox"; }
};
class ShellBox : public PackageBin {
public:
	ShellBox() : PackageBin("ShellBox") {}
	~ShellBox() { DTORF("~ShellBox "); }
public:
	string setup() { return "ShellBox"; }
};
class PalletBox : public PackageBin {
public:
	PalletBox() : PackageBin("PalletBox") {}
	~PalletBox() { DTORF("~PalletBox "); }
public:
	string setup() { return "PalletBox"; }
};
// Seam point - add another package bin.
class Crate : public PackageBin {
public:
	Crate() : PackageBin("Crate") {}
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
	virtual Mold* createMold(map<string,string>& order) {
		return new Mold();
	}
	virtual ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new ConveyerBelt(bin);
	}
	virtual PackageBin* createPackageBin(map<string,string>& order) {
		return new PackageBin("needName");
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
	PackageBin* createPackageBin(map<string,string>& order) {
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
	PackageBin* createPackageBin(map<string,string>& order) {
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
	PackageBin* createPackageBin(map<string,string>& order) {
		return new PalletBox();
	}
};
// Seam point - add another family.
class FastOrder : public InjectionLine {
public:	virtual ~FastOrder() { DTORF("~FastOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_140(bin);
	}
	Mold* createMold(map<string,string>& order) {
		return new Aluminum(4);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new VLevelBelt(bin);
	}
	PackageBin* createPackageBin(map<string,string>& order) {
		return new ShellBox();
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
	PackageBin* createPackageBin(map<string,string>& order) {
		return new PalletBox();
	}
};
class HugeOrder : public InjectionLine {
public:	virtual ~HugeOrder() { DTORF("~HugeOrder "); }
public:
	IJM* createIJM(map<string,string>& order, BinSubject* bin) {
		return new IJM_240(bin);
	}
	Mold* createMold(map<string,string>& order) {
		return new Steel(4);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order, BinSubject* bin) {
		return new VLevelBelt(bin);
	}
	PackageBin* createPackageBin(map<string,string>& order) {
		return new Crate();
	}
};

InjectionLine* InjectionLine::createInjectionLine(map<string,string>& order) {
	unsigned mlSizeTree = atoi(order["size"].c_str());	// Mold Lifetime (ML).

	if(		mlSizeTree <= 10000)		return new PilotOrder;
	else if(mlSizeTree <= 20000)		return new SmallOrder;
	// Seam point - add fast order.
	else if(mlSizeTree <= 40000)		return new FastOrder;
	else if(mlSizeTree <= 50000)		return new MediumOrder;
	// Seam point - add larger orders.
	else if(mlSizeTree <= 100000)		return new LargeOrder;
	else if(mlSizeTree <= 200000)		return new HugeOrder;

	else {						// Defaulting to HugeOrder.
		cout << "  <>Size exceeds mold lifetime |" << mlSizeTree << "|";
		order["size"] = "200000";
		cout << " defaulting to HugeOrder of " << order["size"] << ".\n";
		return new HugeOrder;
	}
}

}

namespace bridge {			// DP 8 - done.

class Platform {
public: virtual ~Platform() { DTORF("~bridge::Platform\n"); }
public:
	virtual string name() { return "name"; }
	virtual string drill() { return "drill"; }
	virtual string cut() { return "cut"; }
	virtual string grind() { return "name"; }
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

	if(		metal == "aluminum")					return new HighCarbon;
	else if(metal == "steel" && finish == "smooth")		return new HighCarbon;
	else if(metal == "steel" && finish == "rippled")	return new Carbide;
	else if(metal == "steel" && finish == "dimpled")	return new DiamondTipped;
	// Seam Point - add another implementation.
	else if(metal == "steel" && finish == "sanded")	return new EDM;
	else if(metal == "steel" && finish == "satin")	return new EDM;

	else {
		cout << "    <>Unknown metal |" << metal << "| or finish |" << finish << "|.";
		order["metal"]  = "steel";
		order["finish"] = "smooth";
		cout << " Defaulting to " << order["metal"] << " and " << order["finish"];
		cout << " with HighCarbon platform.\n";
		return new HighCarbon;
	}
}

class Shape {
protected:
	Platform* platform;
public:
	const string name;
	const unsigned volume_cc;
public:
	Shape(Platform* platform, string name, unsigned volume_cc=0)
	  : platform(platform), name(name), volume_cc(volume_cc) {}
	virtual ~Shape() { DTORF("~bridge::Shape "); delete platform; }
public:
	void mill(map<string,string>& order) {
		cout << "      using " << platform->name() << " tools (";
		cout << platform->drill() << ", ";
		cout << platform->cut() << ", and ";
		cout << platform->grind() << ") ";
		cout << "to mill " << order["metal"] << " block into ";
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
		cout << "steps: D2C4G1.\n";
	}
};
class Car : public Shape {
public:
	Car(Platform* platform) : Shape(platform, "car", 40) {};
	virtual ~Car() { DTORF("~Car "); }
public:
	virtual void steps() {
		cout << "steps: D2C8D1G3.\n";
	}
};
class Hero : public Shape {
public:
	Hero(Platform* platform) : Shape(platform, "hero", 50) {};
	virtual ~Hero() { DTORF("~Hero "); }
public:
	virtual void steps() {
		cout << "steps: D1C1D1G7.\n";
	}
};
// Seam Point - add another abstraction.
class Dino : public Shape {
public:
	Dino(Platform* platform) : Shape(platform, "dino", 30) {};
	virtual ~Dino() { DTORF("~Dino "); }
public:
	virtual void steps() {
		cout << "steps: G5D2C1.\n";
	}
};

Shape* Shape::getShape(map<string,string>& order) {
	Platform* platform = Platform::getPlatform(order);

	if(		order["mold"] == "duck")		return new Duck(platform);
	else if(order["mold"] == "car")			return new Car(platform);
	else if(order["mold"] == "hero")		return new Hero(platform);
	// Seam Point - add another abstraction.
	else if(order["mold"] == "dino")		return new Dino(platform);

	else {
		cout << "    <>Unknown " << "mold" << " |" << order["mold"] << "|";
		order["mold"] = "duck";
		cout << " defaulting to '" << order["mold"] << "'.\n";
		return new Duck(platform);
	}
}

}

namespace chain_of_resp {	// DP 7 - done.

using namespace bridge;

class GetMold {
protected:
	GetMold* successor;
public:
	GetMold(GetMold* successor=0) : successor(successor) {}
	virtual ~GetMold() { DTORF("~CofR::GetMold "); }
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
	static GetMold* selectMold(map<string,string>& order);
};
class Inventory : public GetMold {
public:
	Inventory(GetMold* successor=0) : GetMold(successor) {}
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
			return GetMold::from(order);	// Default.
	}
};
class SisterCompany : public GetMold {
public:
	SisterCompany(GetMold* successor=0) : GetMold(successor) {}
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
			return GetMold::from(order);	// Default.
	}
};
// Seam points - insert another responder.
class Purchase : public GetMold {
public:
	Purchase(GetMold* successor=0) : GetMold(successor) {}
	virtual ~Purchase() { DTORF("~Purchase "); delete successor; }
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
			return GetMold::from(order);	// Default.
	}
};
class Mill : public GetMold {
public:
	Mill(GetMold* successor=0) : GetMold(successor) {}
	virtual ~Mill() { DTORF("~Mill "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "mill") {
			Shape* shape = Shape::getShape(order);
			cout << "    Create " << order["mold"] << " mold from mill ";
			cout << "with " << order["cavities"] << " cavities - ";	// Added to order by TM.setupLine().
			shape->steps();
			shape->mill(order);
			return shape;
		}
		else if(successor != 0)
			return successor->from(order);
		else
			return GetMold::from(order);	// Default.
	}
};

GetMold* GetMold::selectMold(map<string,string>& order) {
	return	new Inventory(
			// Seam point - add more mold sources.
			new SisterCompany(
			new Purchase(
			// Seam point - mill likely location of last resort.
			new Mill(
			// ...
			new GetMold(
		)))));
}

}

namespace decorator {		// DP 6 - done.

class Cavity {	// If the options are varying...
public:
	const unsigned space_mm;
private:
	unsigned blank_mm;
protected:
	unsigned tags_mm;
public:
	Cavity(unsigned width_mm=20)
	  : space_mm(width_mm), blank_mm(space_mm), tags_mm(0)
	{}
	virtual ~Cavity() { DTORF("~dec::Cavity "); }
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
	static Cavity* addTags(Cavity* cavity, const string& list);
};
class Blank : public Cavity {
public:
	Blank(unsigned width_mm=20) : Cavity(width_mm) {}
	virtual ~Blank() { DTORF("~Blank "); }
public:
	virtual unsigned width_mm() { return 0; }
	string list() { return ""; }
};
class Tags : public Cavity {
protected:
	Cavity* delegate;
public:
	Tags(Cavity* delegate) : delegate(delegate) {}
	virtual ~Tags() { DTORF("~Tags "); delete delegate; }
};
class ModelNumber : public Tags {
public:
	ModelNumber(Cavity* delegate) : Tags(delegate) {}
	virtual ~ModelNumber() { DTORF("~ModelNumber "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "ModelNumber "; }
};
class Country : public Tags {
public:
	Country(Cavity* delegate) : Tags(delegate) {}
	virtual ~Country() { DTORF("~Country "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "Country "; }
};
class Date : public Tags {
public:
	Date(Cavity* delegate) : Tags(delegate) {}
	virtual ~Date() { DTORF("~Date "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "Date "; }
};
// Seam point - add another option.
class IncCounter : public Tags {
public:
	IncCounter(Cavity* delegate) : Tags(delegate) {}
	virtual ~IncCounter() { DTORF("~IncCounter "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 4; }
	string list() { return delegate->list() + "IncCounter "; }
};
class PartNumber : public Tags {
public:
	PartNumber(Cavity* delegate) : Tags(delegate) {}
	virtual ~PartNumber() { DTORF("~PartNumber "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 2; }
	string list() { return delegate->list() + "PartNumber "; }
};
class RecycleCode : public Tags {
public:
	RecycleCode(Cavity* delegate) : Tags(delegate) {}
	virtual ~RecycleCode() { DTORF("~RecycleCode "); }
public:
	unsigned width_mm() { return delegate->width_mm() + 6; }
	string list() { return delegate->list() + "RecycleCode "; }
};

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
			cout << "Ignoring unknown tag " << val << ".\n";
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
	virtual ~Plastic() { DTORF("~Plastic "); }
};
class Additive : public Polymer {
protected:
	Polymer* delegate;
public:
	Additive(Polymer* delegate, unsigned volume_cc)
	  : Polymer(volume_cc), delegate(delegate) {}
	virtual ~Additive() { DTORF("~Additive "); delete delegate; }
};
class UVInhibiter : public Additive {
public:
	UVInhibiter(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	virtual ~UVInhibiter() { DTORF("~UVInhibiter "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
	string idNvol() { return delegate->idNvol() + " UVInhibiter" + volAsStr(); }
};
class AntiBacterial : public Additive {
public:
	AntiBacterial(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	virtual ~AntiBacterial() { DTORF("~AntiBacterial "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
	string idNvol() { return delegate->idNvol() + " AntiBacterial" + volAsStr(); }
};
class Hydrophilic : public Additive {
public:
	Hydrophilic(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	virtual ~Hydrophilic() { DTORF("~Hydrophilic "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
	string idNvol() { return delegate->idNvol() + " Hydrophilic" + volAsStr(); }
};
// Seam point - add another option.
class MicroFibers : public Additive {
public:
	MicroFibers(Polymer* delegate, unsigned volume_cc=0)
	  : Additive(delegate, volume_cc) {}
	virtual ~MicroFibers() { DTORF("~MicroFibers "); }
public:
	unsigned mix() { return delegate->mix() + volume_cc; }
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

namespace factory_method {	// DP 4 - done.

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
	void update(BinSubject* bin) {
		cout << "        Shrink wrap packager paused while ";
		cout << bin->name << " package bin was swapped.\n";
	}
};
class HardPack : public Packager {
public:
	HardPack(BinSubject* bin) : Packager(bin) {}
	~HardPack() { DTORF("~HardPack "); }
public:
	string wrap() { return "HardPack"; }
	void update(BinSubject* bin) {
		cout << "        Hard pack packager paused while ";
		cout << bin->name << " package bin was swapped.\n";
	}
};
// Seam point - add another class.
class ZipLock : public Packager {
public:
	ZipLock(BinSubject* bin) : Packager(bin) {}
	~ZipLock() { DTORF("~ZipLock "); }
public:
	string wrap() { return "ZipLock"; }
	void update(BinSubject* bin) {
		cout << "        Ziplock packager paused while ";
		cout << bin->name << " package bin was swapped.\n";
	}
};

Packager* Packager::makeObject(map<string,string>& order, BinSubject* bin) {
	if(		order["packager"] == "Bulk")		return 0;	// Null, no Observer.
	else if(order["packager"] == "ShrinkWrap")	return new ShrinkWrap(bin);
	else if(order["packager"] == "HardPack")	return new HardPack(bin);
	// Seam point - add another class.
	else if(order["packager"] == "ZipLock")		return new ZipLock(bin);

	else {
		legacy_classes::defaulting(order, "packager", "None");
		return 0;
	}
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
		cout << "        Popcorn stuffer triggered to fill ";
		cout << bin->name << " package bin.\n";
	}
};
class Bubblewrap : public Stuffer {
public:
	Bubblewrap(BinSubject* bin) : Stuffer(bin) {}
	~Bubblewrap() { DTORF("~Bubblewrap "); }
public:
	string fill() { return "bubble wrap"; }
	void update(BinSubject* bin) {
		cout << "        Bubble wrap stuffer triggered to fill ";
		cout << bin->name << " package bin.\n";
	}
};
// Seam point - add another class.
class Foam : public Stuffer {
public:
	Foam(BinSubject* bin) : Stuffer(bin) {}
	~Foam() { DTORF("~Foam "); }
public:
	string fill() { return "expanding foam"; }
	void update(BinSubject* bin) {
		cout << "        Foam stuffer triggered to fill ";
		cout << bin->name << " package bin.\n";
	}
};

Stuffer* Stuffer::makeObject(map<string,string>& order, BinSubject* bin) {
	if(		order["stuffer"] == "Air")			return 0;	// Null, no Observer.
	else if(order["stuffer"] == "Popcorn")		return new Popcorn(bin);
	else if(order["stuffer"] == "Bubblewrap")	return new Bubblewrap(bin);
	// Seam point - add another class.
	else if(order["stuffer"] == "Foam")	return new Foam(bin);

	else {
		legacy_classes::defaulting(order, "stuffer", "None");
		return 0;
	}
}

}

namespace template_method {	// DP 3 - done.

using namespace strategy;			// DP 1
using namespace adapter;			// DP 2
//              template_method;	// DP 3
using namespace factory_method;		// DP 4
//				observer;			// DP 5
using namespace decorator;			// DP 6
using namespace chain_of_resp;		// DP 7
using namespace bridge;				// DP 8
using namespace abstract_factory;	// DP 9

class ProcessOrder {	// Template Method (order processing steps).
protected:
	string plasticDesc;
	unsigned colorVol;
private: // Heap objects.
	strategy::RuntimeEstimate*		runtimeEst;
	adapter::CleanMold*				cleaning;
	factory_method::Packager*		packager;
	factory_method::Stuffer*		cushion;	// Specs 2.
	decorator::Cavity*				tags;
	decorator::Polymer*				additives;
	chain_of_resp::GetMold*			theMold;
	bridge::Shape*					shape;
	abstract_factory::InjectionLine*injectionLine;
	abstract_factory::IJM*			ijm;
	abstract_factory::Mold*			mold;
	abstract_factory::ConveyerBelt*	belt;
	abstract_factory::PackageBin*	bin;
public:
	ProcessOrder() :
		colorVol(0),
		// Heap objects to delete.
		runtimeEst(0),
		cleaning(0),
		packager(0),
		cushion(0),	// Specs 2.
		tags(0),
		additives(0),
		theMold(0),
		shape(0),
		injectionLine(0),
		ijm(0),
		mold(0),
		belt(0),
		bin(0)
	{}
	virtual ~ProcessOrder() {
		delete cushion;	// Specs 2.
		delete packager;
		delete belt;
		delete mold;
		delete ijm;
		delete bin;
		delete injectionLine;
		delete shape;
		delete theMold;		cout << endl;
		delete additives;	cout << endl;
		delete tags;		cout << endl;
		delete runtimeEst;
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {	// Template Method (plastic).
		setupLine(order);		// AF (order size), Factory (packaging).
		getMold(order); 		// CofR (mold location), Bridge (shape, milling).
		insertTags(order);	 	// Decorator (tag list).
		loadBins(order);		// Becomes polymorphic on new (colored) plastics.
		loadAdditives(order);	// Decorator (additive list).
		estimate(order);		// Strategy (order size).
		injectionCycle(order);		// TM polymorphic on plastic type.
		simulateFullBin(order);	// Observer (bin full).
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

		ijm		= injectionLine->createIJM(order, bin);			// Observer.
		mold	= injectionLine->createMold(order);				// CofR, # of cavities.
		belt	= injectionLine->createConveyerBelt(order, bin);// Observer.

		packager = Packager::makeObject(order, bin);	// FM & O.
		cushion = Stuffer::makeObject(order, bin);		// FM & O - Specs 2.

		order["metal"] = mold->metal();
		char str[80];
		sprintf(str, "%d", mold->cavities());
		order["cavities"] = str;

		cout << "  Setup injection line for ";
		cout << order["size"] << " order";
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

		cout << "    ";
		cout << ijm->setup() << " - ";
		cout << mold->setup() << " - ";
		cout << belt->setup() << " - ";
		cout << bin->setup() << ".\n";
	}
	void getMold(map<string,string>& order) { // CofR (mold location), Bridge (shape, milling platform).
		cout << "  Process order:\n";

		theMold = GetMold::selectMold(order);

		shape = theMold->from(order);	// Volume_cc.
		char str[80];
		sprintf(str, "%d", shape->volume_cc);
		order["volume"] = str;
	}
	void insertTags(map<string,string>& order) { // Decorator (tag list).
		tags = new Blank();

		string list = "";
		if(order.find("tags") != order.end()) {
			list = order["tags"];
			tags = Cavity::addTags(tags, list);
		}

		list = tags->list();		// Strip trailing blank.
		int size = list.size();
		char tagString[size+1];
		strcpy(tagString, list.c_str());
		if(size) tagString[size-1] = '\0';

		cout << "    Insert tags [" << tagString << "]";
		cout << " of width "<< tags->width_mm() << "/";
		cout << tags->space_mm << " mm, blank tag is ";
		cout << tags->computeBlankWidth(tags->width_mm()) << " mm.\n";
	}
	// Seam point - convert a constant step into a polymorphic step.
	virtual void loadBins(map<string,string>& order) {	// Polymorphic on new (colored) plastics.
		unsigned shapeVol = shape->volume_cc;
		if(order.find("color") != order.end())
			colorVol = 0.1*shapeVol;
		plasticDesc = order["plastic"];
		cout << "    Load plastic bin with " << plasticDesc;
		if(order.find("color") != order.end())
			cout << " and color bin with " << order["color"];
		cout << ".\n";
	}
	void loadAdditives(map<string,string>& order) {	// Decorator (additive list).
		string colorDesc = "";
		char str[80];
		if(colorVol) {
			sprintf(str, "(%d)", colorVol);
			colorDesc = string(" ") + order["color"] + str;
		}

		additives = addAdditives(new Plastic, order);

		unsigned shapeVol = shape->volume_cc;
		unsigned cavities = mold->cavities();
		string cavityies = cavities==1 ? " cavity" : " cavities";
		unsigned total = cavities*shapeVol;

		unsigned plasticVol = shapeVol - colorVol - additives->mix();

		cout << "      Recipe: " << plasticDesc << "(" << plasticVol << ")";
		cout << colorDesc << additives->idNvol() << " = " << shapeVol << " cc.\n";

		cout << "      Volume: " << order["mold"] << "(" << shapeVol << ")";
		cout << " * " << cavities << cavityies;
		cout << " = " << total << " cc.\n";
	}
	void estimate(map<string,string>& order) { // Strategy (order size).
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/mold->cavities();
		runtimeEst = RuntimeEstimate::selectEstimationAlgorithm(order);
		unsigned runtime = (*runtimeEst)(order);

		cout << "    Cycle " << ijm->setup() << " for " << order["plastic"];
		cout << " " << runSize << " times, ";
		string hour_s = runtime==1 ? " hour" : " hours";
		cout << "estimated run time = " << runtime << hour_s << ".\n";
	}
	virtual void injectionCycle(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "      Close - mix - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void simulateFullBin(map<string,string>& order) {	// Observer (bin full).
		bin->pause();
	}
	void cleanMold(map<string,string>& order) {	// Adapter (plastic, mold metal).
		cleaning = CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
	// Seam point - add another constant step.
	void ship(map<string,string>& order) {	// Factory (stuffing).
		cout << "    Ship to \"" << order["address"] << "\".\n";
	}
public:
	static ProcessOrder* getOrderProcess(map<string,string>& order);
};
class ABS : public ProcessOrder {	// If the plastics are varying...
public: ~ABS() { DTORF("~ABS\n"); }
public:
	virtual void injectionCycle(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "      Close - mix - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject.\n";
	}
};
class Poly : public ProcessOrder {
public: ~Poly() { DTORF("~Poly\n"); }
public:
	virtual void injectionCycle(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "      Close - mix - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject.\n";
	}
};
class PET : public ProcessOrder {
public: ~PET() { DTORF("~PET\n"); }
public:
	virtual void injectionCycle(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "      Close - mix - heat to 404 - inject at 110 PSI - cool to 340 - separate - smooth eject.\n";
	}
};
// Seam point - add another polymorphic step (not in this version).
class Synthetic : public ProcessOrder {	// If the plastics are varying...
public: ~Synthetic() { DTORF("~Synthetic\n"); }
public:
	// Seam point - convert a constant step into a polymorphic step.
	virtual void loadBins(map<string,string>& order) {
		colorVol = 0;
		plasticDesc = order["color"] + string("-") + order["plastic"];
		cout << "    Load plastic bin with " << plasticDesc << " pellets.\n";
	}
	virtual void injectionCycle(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "      Close - mix - heat to 480 - inject at 150 PSI - cool to 390 - separate - shock eject.\n";
	}
};

ProcessOrder* ProcessOrder::getOrderProcess(map<string,string>& order) {
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
	using namespace template_method;

	ProcessOrder* processOrder = ProcessOrder::getOrderProcess(order);

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

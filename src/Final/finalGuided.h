/*
 * finalGuided.h - Part 1: finalOriginal.h  Part 2: finalSolution.h
 *
 *  Created on: <MMDDCCYY>
 *      Author: <studentName>
 */

#ifndef FINAL_FINAL_GUIDED_H_
#define FINAL_FINAL_GUIDED_H_

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
		cout << "  Clean mold of ABS: soak in alcohol, rinse with water, dry.\n";
	}
};

class CleanPolys {
public: ~CleanPolys() { DTORF("~legacy_classes::CleanPolys "); }
	void prepForReuse() {
		cout << "  Clean mold of Poly: rinse with acetone, dry.\n";
	}
};

class PETCleanup {
public: ~PETCleanup() { DTORF("~legacy_classes::PETCleanup "); }
	void carbonize() { // Use only on stainless steel.
		cout << "  Clean steel mold of PET: heat to 895 C.\n";
	}
	void purify() {	// Use only on aluminum.
		cout << "  Clean aluminum mold of PET: rinse with soap and water, dry.\n";
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
class ABS : public CleanMold {
	legacy_classes::CleanABS plastic;
public: ~ABS() { DTORF("~ABS "); }
	void clean(const string& metal) {
		plastic.clean();
	}
};
class Poly : public CleanMold {
	legacy_classes::CleanPolys plastic;
public: ~Poly() { DTORF("~Poly "); }
	void clean(const string& metal) {
		plastic.prepForReuse();
	}
};
class PET : public CleanMold {
	legacy_classes::PETCleanup plastic;
public: ~PET() { DTORF("~PET "); }
	void clean(const string& metal) {
		if(metal == "steel")
			plastic.carbonize();
		else
			plastic.purify();
	}
};
// Seam point - add another interface.

CleanMold* CleanMold::getCleaning(map<string,string>& order) {	// FM.
	if(order["plastic"] == "ABS")				return new ABS;
	if(order["plastic"] == "Polypropylene")		return new Poly;
	if(order["plastic"] == "Polyethelene")		return new Poly;
	if(order["plastic"] == "PET")				return new PET;
	// Seam point - add another plastic.

	return new CleanMold;
}

} // adapter

namespace strategy {		// DP 1.

// Seam point - add another algorithm.

} // strategy

namespace observer {		// DP 5.

// Seam point - add another listener.

} // observer

namespace abstract_factory {// DP 9.

// Seam point - add another type 1.
// Seam point - add another type 2.
// Seam point - add another type 3.
// Seam point - add another ...
// Seam point - add another type N.
// Seam point - add another family.

} // abstract_factory

namespace bridge {			// DP 8.

// Seam Point - add another implementation.
// Seam Point - add another abstraction.

} // bridge

namespace chain_of_resp {	// DP 7.

// Seam points - add another responder.

} // chain_of_resp

namespace decorator {		// DP 6.

// Seam point - add another option.

} // decorator

namespace factory_method {	// DP 4.

// Seam point - add another class.

} // factory_method

namespace template_method {	// DP 3.

/* Instruction Steps:
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

class ProcessOrder {
	adapter::CleanMold*	cleaning;
public:
	ProcessOrder()
	: cleaning(0)
	{}
	virtual ~ProcessOrder() {
		delete cleaning;
		DTORF("~template_method::ProcessOrder\n");
	}
public:
	void run(map<string,string>& order) {
		setupLine(order);
		getMold(order);
		insertTags(order);
		loadPlastic(order);
		loadAddtives(order);
		runtimeEstimate(order);
		injectionCycle(order);
		simulateFullPartsBin(order);
		cleanMold(order);
	}
protected:
	void setupLine(map<string,string>& order) {
		cout << "  Setup injection line...\n";
	}
	void getMold(map<string,string>& order) {
		cout << "  Get mold...\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags...\n";
	}
	void loadPlastic(map<string,string>& order) {
		cout << "  Load plastic...\n";
	}
	void loadAddtives(map<string,string>& order) {
		cout << "  Load additives...\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		cout << "  Run time estimate...\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cout << "  Cycle...\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {
		cout << "  Simulate full parts bin...\n";
	}
	void cleanMold(map<string,string>& order) {
		cleaning = adapter::CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
public:
	static ProcessOrder* getProcessOrder(map<string,string>& order);
};
class ABSOrder : public ProcessOrder {
public: ~ABSOrder() { DTORF("~ABSOrder\n"); }
public:
	virtual void injectionCycle(map<string,string>& order) {
		cout << "  Cycle: close - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject.\n";
	}
};
class PolyOrder : public ProcessOrder {
public: ~PolyOrder() { DTORF("~PolyOrder\n"); }
public:
	virtual void injectionCycle(map<string,string>& order) {
		cout << "  Cycle: close - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject.\n";
	}
};
class PETOrder : public ProcessOrder {
public: ~PETOrder() { DTORF("~PETOrder\n"); }
public:
	virtual void injectionCycle(map<string,string>& order) {
		cout << "  Cycle: close - heat to 404 - inject at 110 PSI - cool to 340 - separate - smooth eject.\n";
	}
};
// Seam point - add another polymorphic step.
// Seam point - add another constant step.
// Seam point - convert a constant step into a polymorphic step.

ProcessOrder* ProcessOrder::getProcessOrder(map<string,string>& order) { // FM.
	if(order["plastic"] == "ABS")				return new ABSOrder;
	if(order["plastic"] == "Polypropylene")		return new PolyOrder;
	if(order["plastic"] == "Polyethelene")		return new PolyOrder;
	if(order["plastic"] == "PET")				return new PETOrder;
	// Seam point - add another plastic.

	return new ProcessOrder;
}

} // template_method

void process(map<string,string>& order) {
	// Fill in the namespaces above with your design pattern class hierarchies.
	// Call your order processing class from here <myProcess>->run(order);

	using namespace template_method;

	ProcessOrder* processOrder = ProcessOrder::getProcessOrder(order);

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

#endif /* FINAL_FINAL_GUIDED_H_ */

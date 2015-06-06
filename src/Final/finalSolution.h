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
public: ~CleanABS() { DTORF("~CleanABS "); }
	void clean() {
		cout << "    Clean ABS mold: soak in alcohol, rinse with water, dry.\n";
	}
};

class CleanPolys {
public: ~CleanPolys() { DTORF("~CleanPolys "); }
	void prepForReuse() {
		cout << "    Clean Poly mold: rinse with acetone, dry.\n";
	}
};

class PETCleanup {
public: ~PETCleanup() { DTORF("~PETCleanup "); }
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

int packingSize_cc(int partSize_cc, float packingFraction, float packagerFraction) {
	return (partSize_cc*(1 + packingFraction))*(1 + packagerFraction);
}
}

namespace adapter {			// DP 2.

// Seam point - add another interface.

}

namespace strategy {		// DP 1.

// 1) BackOfTheEnvelope(10k): runTime = orderSize*60/cavities;
// 2) Calculation(50k):  runTime = orderSize/cavities * cycletime(metal, mold->volume);
// 3) Historical(200k): runTime = setupAvg_min + orderSize/cavities*mold->cycletime + teardownAvg_min;
// Seam point -
// 4) Projection(1M): runTime = ijm->setupTime_mins()
//							  + (1+0.01*rejectRate_pcnt)*orderSize/cavities*mold->cycletime()
//						      + ijm->teardownTime_mins();

class Strategy {
protected:
	int orderSize;
	int cavities;
public:
	Strategy() : orderSize(0), cavities(0) {}
	virtual ~Strategy() { DTORF("~Strategy\n"); }
public:
	virtual int runTimeEst_hrs(map<string,string>& order) {
		orderSize = atoi(order["size"].c_str());
		cavities = atoi(order["cavities"].c_str());
		if(cavities <= 0)	cavities = 1;
		return 0;
	}
public:
	static Strategy* selectEstimationAlgorithm(map<string,string>& order);
};
class BackOfTheEnvelope : public Strategy {
public:
	BackOfTheEnvelope() {}
	~BackOfTheEnvelope() { DTORF("~BackOfTheEnvelope "); }
public:
	virtual int runTimeEst_hrs(map<string,string>& order) {
		Strategy::runTimeEst_hrs(order);
		return (orderSize/cavities)/60;
	}
};
class Calculation : public Strategy {
public:
	Calculation() {}
	~Calculation() { DTORF("~Calculation "); }
public:
	virtual int runTimeEst_hrs(map<string,string>& order) {
		int orderSize = atoi(order["size"].c_str());
		int cavities = atoi(order["cavities"].c_str());
		if(cavities <= 0)	cavities = 1;
		int volume_cc = atoi(order["volume"].c_str());
		int cycleTime = legacy_classes::cycleTime_sec(order["metal"], volume_cc);
		return (orderSize/cavities)*cycleTime/3600;
	}
};
class Historical : public Strategy {
public:
	Historical() {}
	~Historical() { DTORF("~Historical "); }
public:
	virtual int runTimeEst_hrs(map<string,string>& order) {
		using namespace legacy_classes;
		int orderSize = atoi(order["size"].c_str());
		int cavities = atoi(order["cavities"].c_str());
		if(cavities <= 0)	cavities = 1;
		int cycleTime = 30; // Get from mold->cycletime_sec();
		return (setupAvg_min + (orderSize/cavities)*cycleTime/60 + teardownAvg_min)/60;
	}
};
// Seam point - add another algorithm.

Strategy* Strategy::selectEstimationAlgorithm(map<string,string>& order) {
	int orderSize = atoi(order["size"].c_str());

	if(		orderSize <  10000)	return new BackOfTheEnvelope;
	else if(orderSize <  50000)	return new Calculation;
	else if(orderSize < 200000)	return new Historical;
	// Seam point - add another runtime estimation algorithm.

	else {
		legacy_classes::defaulting(order, "size", "100");
//		int orderSize = atoi(order["size"].c_str());
		return new Strategy;
	}
}

}

namespace observer {		// DP 7.

// Seam point - add another listener.

}

namespace abstract_factory {// DP 10.

// Seam point - add another type 1.
// Seam point - add another type 2.
// Seam point - add another type 3.
// Seam point - add another ...
// Seam point - add another type N.
// Seam point - add another family.

}

namespace bridge {			// DP 9.

// Seam Point - add another implementation.
// Seam Point - add another abstraction.

}

namespace chain_of_resp {	// DP 8.

// Seam points - add another responder.

}

namespace decorator {		// DP 6.

// Seam point - add another option.

}

namespace factory_method {	// DP 5.

// Seam point - add another class.

}

namespace template_method {	// DP 4.

class ProcessOrder_TM {	// Template Method (order processing steps).
	int runSize;
public:
	ProcessOrder_TM() : runSize(0) {}
	virtual ~ProcessOrder_TM() { DTORF("~ProcessOrder_TM\n"); }
private:
	int RunSize(map<string,string>& order) {
		int orderSize = atoi(order["size"].c_str());
		int runSize = orderSize/2;
		cout << "  Injection cycle: " << orderSize << " order";
		cout << " (" << runSize << " run)...\n";
		return runSize;
	}
public:
	void run(map<string,string>& order) {	// Template Method (plastic).
		cout << "ProcessOrder_TM.run()\n";
		setupLine(order);		// AF (order size), Factory (packaging).
		getMold(order); 		// CofR (mold location), Bridge (shape, milling).
		insertTags(order);	 	// Decorator (tag list).
		runTimeEst(order);		// Strategy (order size).
		loadBins(order);		// Becomes polymorphic on new (colored) plastics.
		loadAdditives(order);	// Decorator (additive list).
		runSize = RunSize(order);
		cycleRecipe(order);		// TM polymorphic on plastic type.
		injectionCycle(order);	// Observer (bin full).
		cleanMold(order);		// Adapter (plastic, mold metal).
		// Seam point - add step.
		ship(order);			// Factory (stuffing).
	}
public:
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).

	}
	void getMold(map<string,string>& order) { // CofR (mold location), Bridge (shape, milling platform).
		cout << "  Mold has 2 cavities.\n";
	}
	void insertTags(map<string,string>& order) { // Decorator (tag list).

	}
	void runTimeEst(map<string,string>& order) { // Strategy (order size).
		using namespace strategy;
		Strategy* strategy = Strategy::selectEstimationAlgorithm(order);
		cout << "  Estimated run time = " << strategy->runTimeEst_hrs(order)/60.0;
		cout << " days.\n";
	}
	virtual void loadBins(map<string,string>& order) {	// Polymorphic on new (colored) plastics.
		cout << "  Load plastic bin with " << order["plastic"] << ".\n";
		cout << "  Load color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {	// Decorator (additive list).

	}
	virtual void cycleRecipe(map<string,string>& order) {	// TM polymorphic on plastic type.
		cout << "    <plastic> - Close - heat to <temp> - inject at <pressure>";
		cout << " PSI - cool to <temp> - separate - <manner of> eject\n";
	}
	void injectionCycle(map<string,string>& order) {	// Observer (bin full).
		int effPartVol_cc = legacy_classes::packingSize_cc(50, .10, .25);
		int capacity = 300000/effPartVol_cc;
		cout << "    Effective part volume ~ " << effPartVol_cc;
		cout << " implies <package> bin capacity ~ " << capacity << ".\n";
		int count = 0;
		int total = 0;
		for(int i=0; i<runSize; i++) {
//			count += mold->cavities;
			count += 1;
//			bin->increment(mold->cavities);
//			if(bin->full()) {
			if(count > capacity) {
				cout << "    <package> bin full (" << count << " parts).\n";
//				bin->update();
				cout << "      IJM/Conveyer/Packager pausing while <package> bin is swapped.\n";
				cout << "      Fill <package> bin with <stuffer> packing.\n";
				total += count;
				count = 0;
			}
		}
		if(count > 0) {
			cout << "    <package> bin partial (" << count << " parts).\n";
			cout << "      Fill <package> bin with <stuffer> packing.\n";
		}
		total += count;
		cout << "  Total parts produced, boxed (and shipped) = " << total << ".\n";
	}
	void cleanMold(map<string,string>& order) {	// Adapter (plastic, mold metal).

	}
	// Seam point - add another step.
	void ship(map<string,string>& order) {	// Factory (stuffing).

	}
public:
	static ProcessOrder_TM* getOrderProcess(map<string,string>& order);
};
class Plastic : public ProcessOrder_TM {	// If the plastics are varying...
public:
	Plastic() {}
	~Plastic() { DTORF("~Plastic "); }
public:
	virtual void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["color"];
		cout << " " << order["plastic"] << ".\n";
	}
	virtual void cycleRecipe(map<string,string>& order) {}
};
// Seam point - add another plastic.
// Seam point - convert a constant step into a polymorphic step.

ProcessOrder_TM* ProcessOrder_TM::getOrderProcess(map<string,string>& order) {
	if(		order["plastic"] == "whatever")		return new ProcessOrder_TM;
	else if(order["plastic"] == "whatever")		return new ProcessOrder_TM;
	// Seam point - add another plastic.

	else {
		legacy_classes::defaulting(order, "plastic");
		return new ProcessOrder_TM;
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

pair<string, string> parse(string line) {
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

map<string, string> getCompleteOrder(FILE* orderFilePtr) {
	map<string, string> order;
	char line[80+3];
	pair<string, string>	keyValue;

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

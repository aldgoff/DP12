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

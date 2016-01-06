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
		if(order.find("finish") == order.end()) {
			legacy_classes::defaulting(order, "finish", "smooth");
		}
	}
	void setupLine(map<string,string>& order) {
		cout << "  Setup injection line for "
			 << order["size"] << " order"
			 << " with " << order["packager"] << " packager:\n";

		cout << "    "
			 << "<IJM> - "
			 << "<metal>(<cavities>) - "
			 << "<belt> belt - "
			 << "<bin>.\n";
	}
	void getMold(map<string,string>& order) {
		cout << "  <Acquire> " << order["mold"] << " mold"
			 << " from " << order["moldLoc"] << ".\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		cout << "    Recipe: " << order["plastic"] << "(vol) "
			 << order["color"] << "(vol) <additive(<vol>) list> = (vol) cc.\n";
		cout << "    Volume: " << order["mold"] << "(vol) * "
			 << "<cavities> cavities = (vol) cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		cout << "  Estimated run time (algorithm) = x hour(s).\n";
		cout << "    Formula:\n";
		cout << "    Values:\n";
	}
	virtual void injectionCycle(map<string,string>& order) {
		cycle(order);
		cout << "    Close - heat to <temp> - inject at <pressure> PSI"
			 << " - cool to <temp> - separate - <manner of> eject\n";
	}
	void simulateFullPartsBin(map<string,string>& order) {
		cout << "    Whenever <partsBin> parts bin was full:\n";
		cout << "      List of upstream machines to pause...\n";
		cout << "      ...\n";
	}
	void cleanMold(map<string,string>& order) {
		using namespace adapter;

		cleaning = CleanMold::getCleaning(order);
		cleaning->clean(order["metal"]);
	}
protected: // Helper methods.
	void cycle(map<string,string>& order) {
		cout << "  Cycle <IJM> for " << order["plastic"] << " <run> times.\n";
	}
};

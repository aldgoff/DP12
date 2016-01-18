class ProcessOrder { // Defaults and order specs.
public:
	ProcessOrder()
	{}
	virtual ~ProcessOrder() {
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
	void setupLine(map<string,string>& order) {
		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}

		cout << "  Setup injection line for "
			 << order["size"] << " order"
			 << " with " << "<packager>" << " packager:\n";

		cout << "    "
			 << "<IJM> - "
			 << "<metal>(<cavities>) - "
			 << "<belt> belt - "
			 << "<bin>.\n";
	}
	void getMold(map<string,string>& order) {
		cout << "  <Acquire> <mold> mold"
			 << " from <moldLoc>.\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with <color>.\n";
		cout << "    Recipe: " << order["plastic"] << "(vol) + "
				 << "<color>(vol) + <additive(<vol>) list> = (vol) cc.\n";
		cout << "    Volume: <mold>(vol) * "
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
			 << " - cool to <temp> - separate - <manner of> eject.\n";
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
		cout << "  Cycle <IJM> for "
			 << order["plastic"] << " <run> times.\n";
	}
};

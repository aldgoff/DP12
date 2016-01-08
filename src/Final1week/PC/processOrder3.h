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
	void setupLine(map<string,string>& order) {	// AF (order size).
		using namespace abstract_factory1;

		if(order.find("size") == order.end()) {
			cout << "  <>No size specified, defaulting to 100.\n";
			order["size"] = "100";
		}

		injectionLine = InjectionLine::createInjectionLine(order);
		ijm	  = injectionLine->createIJM(order);
		block = injectionLine->createBlock(order);
		belt  = injectionLine->createConveyerBelt(order);
		bin	  = injectionLine->createPartsBin(order);

		cout << "  Setup injection line for "
			 << order["size"] << " order"
			 << " with " << "<packager>" << " packager:\n";

		cout << "    "
			 << ijm->setup() << " - "
			 << block->setup() << " - "
			 << belt->setup() << " - "
			 << bin->setup() << ".\n";

		order["metal"] = block->metal();
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
	}
	void loadAdditives(map<string,string>& order) {
		int cavities = block->cavities;

		cout << "    Recipe: " << order["plastic"] << "(vol) "
				 << "<color>(vol) <additive(<vol>) list> = (vol) cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: <mold>(vol) * "
			 << cavities << plural
			 << "= (vol) cc.\n";
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

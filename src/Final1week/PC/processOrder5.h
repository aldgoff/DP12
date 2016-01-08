class ProcessOrder5 { // Mold Shape & Platform - Bridge(8).
	adapter::CleanMold*					cleaning;
	abstract_factory2::InjectionLine*	injectionLine;
	abstract_factory2::IJM*				ijm;
	abstract_factory2::Block*			block;
	abstract_factory2::ConveyerBelt*	belt;
	abstract_factory2::PartsBin*		bin;	// Inherits from observer::BinSubject.
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
	void setupLine(map<string,string>& order) {	// AF (order size), Factory (packaging).
		using namespace factory_method;
		using namespace abstract_factory2;

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
		using namespace bridge;

		shape = Shape::getShape(order);	// Volume_cc.
		stringstream volume;
		volume << shape->volume_cc;
		order["volume"] = volume.str();

		cout << "  <Acquire> " << order["mold"] << " mold"
			 << " from " << order["moldLoc"] << ".\n";
	}
	void insertTags(map<string,string>& order) {
		cout << "  Insert tags [<list>] of width <width>/20 mm, blank tag is <20-width> mm.\n";
	}
	void loadBins(map<string,string>& order) {
		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}

		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		int cavities = block->cavities;

		int shapeVol = shape->volume_cc;
		int colorVol = 0.10*shapeVol;
		int plasticVol = shapeVol - colorVol;

		int totalVol = cavities*shapeVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") "
			 << order["color"] << "(" << colorVol << ") "
			 << "<additive(<vol>) list> = (" << shapeVol << ") cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: " << order["mold"] << "(" << shapeVol << ") * "
			 << cavities << plural
			 << "= " << totalVol << " cc.\n";
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

class ProcessOrder9 { // Estimated run time - Strategy(1).
	adapter::CleanMold*					cleaning;
	abstract_factory2::InjectionLine*	injectionLine;
	abstract_factory2::IJM*				ijm;
	abstract_factory2::Block*			block;
	abstract_factory2::ConveyerBelt*	belt;
	abstract_factory2::PartsBin*		bin;	// Inherits from observer::BinSubject.
	factory_method::Packager*			packager;
	bridge::Shape*						shape;
	chain_of_resp::Mold*				mold;
	decorator::Cavity*					tags;
	decorator::Polymer*					additives;
	strategy::RuntimeEstimate*			runtimeEst;
public:
	ProcessOrder9()
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
	virtual ~ProcessOrder9() {
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
		using namespace chain_of_resp;

		mold = Mold::acquireMold(order);

		shape = mold->from(order);	// Volume_cc.
		stringstream volume;
		volume << shape->volume_cc;
		order["volume"] = volume.str();
	}
	void insertTags(map<string,string>& order) {
		using namespace decorator;

		tags = new Blank();

		string list = "";
		if(order.find("tags") != order.end()) {
			list = order["tags"];
			tags = Cavity::addTags(tags, list);
		}

		string tagString = tags->list();		// Strip trailing blank.
		int size = tagString.size();
		if(size) tagString.erase(tagString.size()-1, 1);
//		tagString.back() = '\0';	// Are these C++/11?
//		tagString.pop_back();

		cout << "  Insert tags [" << tagString << "]"
			 << " of width "<< tags->width_mm() << "/"
			 << tags->space_mm << " mm, blank tag is "
			 << tags->computeBlankWidth(tags->width_mm()) << " mm.\n";
		cout << Cavity::unknownTags.str();

		Cavity::unknownTags.str("");	// Clear.
	}
	void loadBins(map<string,string>& order) {
		if(order.find("color") == order.end()) {
			legacy_classes::defaulting(order, "color", "black");
		}

		cout << "  Load plastic bin with " << order["plastic"]
			 << " and color bin with " << order["color"] << ".\n";
	}
	void loadAdditives(map<string,string>& order) {
		using namespace decorator;

		int cavities = block->cavities;

		additives = addAdditives(new Plastic, order);

		int shapeVol = shape->volume_cc;
		int colorVol = 0.10*shapeVol;
		int plasticVol = shapeVol - colorVol - additives->mix();

		int totalVol = cavities*shapeVol;

		cout << "    Recipe: " << order["plastic"] << "(" << plasticVol << ") "
			 << order["color"] << "(" << colorVol << ")"
			 << additives->idNvol() << " = " << shapeVol << " cc.\n";

		string plural = (cavities == 1) ? " cavity ": " cavities ";
		cout << "    Volume: " << order["mold"] << "(" << shapeVol << ") * "
			 << cavities << plural
			 << "= " << totalVol << " cc.\n";
	}
	void runtimeEstimate(map<string,string>& order) {
		using namespace strategy;

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

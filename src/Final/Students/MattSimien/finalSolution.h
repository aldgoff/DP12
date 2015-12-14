#pragma once
/*
* finalDesign.h
*
*  Created on: <11292015>
*      Author: <Matthew Simien>
*/

#ifndef FINAL_FINAL_DESIGN_H_
#define FINAL_FINAL_DESIGN_H_

#include <algorithm>
#include <list>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <map>
using namespace std;

namespace final_design {

	const bool on = false;	// Useful macro to turn dtor instrumentation on/off.
#define DTORF(x) if(on) { cout << x; }

#define COUNT(x) (sizeof(x)/sizeof(*x))

	namespace legacy_classes {	// Can't change these.

		int cycleTime_sec(string metal, int volume_cc) {
			int heatCapacityAdjustment = 0;

			if (metal == "aluminum")	heatCapacityAdjustment = -2;
			else if (metal == "steel")		heatCapacityAdjustment = +5;

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

		void defaulting(map<string, string>& order, const string& option, string def = "simulation") {
			cout << "  <>Unknown " << option << " |";
			cout << order[option] << "|";
			order[option] = def;
			cout << " defaulting to '" << order[option] << "'.\n";
		}

	}
	void binFullPrint(string info, string standard) {
		cout << "        " << info << " " << standard;
	}

	enum Plastic { ABS, POLYPROPYLENE, POLYETHELENE, PET, STYRENE, NYLON66 };
	static map<string, Plastic> PlasticDictionary = {
		{ "abs",ABS },
		{ "polypropylene",POLYPROPYLENE },
		{ "polyethelene",POLYETHELENE },
		{ "pet",PET },
		{ "styrene", STYRENE},
		{ "nylon66", NYLON66}
	};
	static map<Plastic, string> PlasticDictionaryToString = {
		{ ABS, "ABS" },
		{ POLYPROPYLENE, "Polypropylene", },
		{ POLYETHELENE, "Polyethelene" },
		{ PET, "PET", },
		{STYRENE, "Styrene"},
		{NYLON66, "Nylon66"}
	};

	enum Packager { BULK, SHRINKWRAP, HARDPACK, ZIPLOCK};
	static map<string, Packager> PackagerDictionary = {
		{ "bulk",BULK },
		{ "shrinkwrap",SHRINKWRAP },
		{ "hardpack",HARDPACK },
		{ "ziplock",ZIPLOCK}
	};

	static map<Packager, string> PackagerDictionaryToString = {
		{ BULK, "Bulk" },
		{ SHRINKWRAP, "ShrinkWrap" },
		{ HARDPACK ,"HardPack" },
		{ ZIPLOCK, "ZipLock"}
	};

	enum Stuffer { AIR, POPCORN, BUBBLEWRAP, FOAM };
	static map<Stuffer, string> StufferDictionaryToString = {
		{ AIR, "air" },
		{ POPCORN, "styrene popcorn" },
		{ BUBBLEWRAP ,"bubble wrap" },
		{ FOAM, "expanding foam" }
	};
	static map<Stuffer, string> StufferDictionaryToStringBinFull = {
		{ AIR, "air" },
		{ POPCORN, "Popcorn" },
		{ BUBBLEWRAP ,"Bubblewrap" },
		{ FOAM, "Foam" }
	};
	static map<string, Stuffer> StufferDictionary = {
		{ "air", AIR},
		{ "popcorn" ,POPCORN},
		{ "bubblewrap", BUBBLEWRAP},
		{ "foam", FOAM}
	};

	enum Mold { DUCK, CAR, HERO, DINO };
	static map<string, Mold> MoldDictionary = {
		{ "duck",DUCK },
		{ "car",CAR },
		{ "hero",HERO },
		{ "dino", DINO}
	};
	static map<Mold, string> MoldDictionaryToString = {
		{ DUCK, "duck" },
		{ CAR, "car" },
		{ HERO, "hero" },
		{ DINO, "dino"}
	};

	enum Color { BLACK, BROWN, RED, ORANGE, YELLOW, GREEN, CLEAR, BLUE, VIOLET, GRAY, WHITE };
	static map<string, Color> ColorDictionary = {
		{ "black",BLACK },
		{ "brown",BROWN },
		{ "red",RED },
		{ "orange",ORANGE },
		{ "yellow",YELLOW },
		{ "green",GREEN },
		{ "clear",CLEAR },
		{ "blue",BLUE },
		{ "violet",VIOLET},
		{"gray",GRAY},
		{"white", WHITE}
	};

	static map<Color, string> ColorDictionaryToString = {
		{ BLACK, "black" },
		{ BROWN, "brown" },
		{ RED, "red" },
		{ ORANGE, "orange" },
		{ YELLOW, "yellow" },
		{ GREEN, "green" },
		{ CLEAR, "clear" },
		{ BLUE,"blue"},
		{ VIOLET,"violet" },
		{ GRAY,"gray" },
		{ WHITE, "white" }
	};

	enum MoldLoc { INVENTORY, SISTER_COMPANY, MILL, PURCHASE };
	static map<string, MoldLoc> MoldLocDictionary = {
		{ "inventory",INVENTORY },
		{ "sistercompany",SISTER_COMPANY },
		{ "mill",MILL },
		{ "purchase", PURCHASE}
	};

	enum Finish { SMOOTH, RIPPLED, DIMPLED, SANDED, SATIN };
	static map<string, Finish> FinishDictionary = {
		{ "smooth",SMOOTH },
		{ "rippled",RIPPLED },
		{ "dimpled",DIMPLED },
		{ "sanded",SANDED },
		{ "satin",SATIN },
	};

	enum Tags { BLANK, MODELNUMBER, COUNTRY, DATE, INCCOUNTER, PARTNUMBER, RECYCLECODE };
	enum PossibleIngredients { UVINHIBITER, ANTIBACTERIAL, HYDROPHILIC, MICROFIBERS };
	struct IngredientDetails {
		IngredientDetails(PossibleIngredients inputIngredient, int inputVolume) :ingredient(inputIngredient), volume(inputVolume) {}
		PossibleIngredients ingredient;
		int volume;
	};

	namespace adapter {			// DP 2.
		class CleanMoldProcess {
		public: virtual ~CleanMoldProcess() {
			DTORF("  ~adapter::CleanMoldProcess\n");
		}
				virtual void myWay(string moldMetal) {}

				static CleanMoldProcess* makeObject(Plastic type);
		};

		class ABSCleanInterface : public CleanMoldProcess {
			legacy_classes::CleanABS wrapped;
		public: ~ABSCleanInterface() {
			DTORF("  ~ABSCleanInterface");
		}
		public: void myWay(string moldMetal) { wrapped.clean(); }
		};
		class PolyCleanInterface : public CleanMoldProcess {
			legacy_classes::CleanPolys wrapped;
		public: ~PolyCleanInterface() {
			DTORF("  ~PolyCleanInterface");
		}
		public: void myWay(string moldMetal) { wrapped.prepForReuse(); }
		};
		class PETCleanInterface : public CleanMoldProcess {
			legacy_classes::PETCleanup wrapped;
		public: ~PETCleanInterface() {
			DTORF("  ~PETCleanInterface");
		}
		public: void myWay(string moldMetal) {
			if ("steel" == moldMetal) {
				wrapped.carbonize();
			}
			else {
				wrapped.purify();
			}
		}
		};
		class StyreneAndNylonCleanInterface : public CleanMoldProcess {
		public: ~StyreneAndNylonCleanInterface() {
			DTORF("  ~StyreneAndNylonCleanInterface");
		}
		public: void myWay(string moldMetal) { cout << "    Clean synthetic mold: ozone wash.\n"; }
		};

		// Seam point - add another interface.
		CleanMoldProcess*CleanMoldProcess::makeObject(Plastic type) {
			if (ABS == type) {
				return new ABSCleanInterface;
			}
			else if (PET == type) {
				return new PETCleanInterface;
			}
			else if (STYRENE == type || NYLON66 == type) {
				return new StyreneAndNylonCleanInterface;
			}
			else {
				return new PolyCleanInterface;
			}
		}
	}

	namespace strategy {		// DP 1.
		class RunTimeCalculation {
		public:
			RunTimeCalculation(Plastic inputPlastic) : plasticType(inputPlastic) {}
			Plastic plasticType;
			virtual ~RunTimeCalculation() {
				DTORF("  ~strategy::RunTimeCalculation\n");
			}
		public:
			virtual void compute(int orderSize, int cavities, int shapeVolume, string metal) {}
		};
		class BackOftheEnvelope : public RunTimeCalculation {
		public:
			BackOftheEnvelope(Plastic inputPlastic) : RunTimeCalculation(inputPlastic) {}
			virtual ~BackOftheEnvelope() {
				DTORF("  ~BackOftheEnvelope");
			}
		public:
			virtual void compute(int orderSize, int cavities, int shapeVolume, string metal) {
				int runtime = (orderSize / cavities) * .0166666666666667;
				string hourS = (runtime > 1) ? " hours" : " hour";
				cout << runtime << hourS;
			}
		};
		class Calculation : public RunTimeCalculation {
		public:
			Calculation(Plastic inputPlastic) : RunTimeCalculation(inputPlastic) {}
			virtual ~Calculation() {
				DTORF("  ~Calculation");
			}
		public:
			virtual void compute(int orderSize, int cavities, int shapeVolume, string metal) {
				int runtime = ((orderSize / cavities) * legacy_classes::cycleTime_sec(metal, shapeVolume)) / 3600;
				string hourS = (runtime > 1) ? " hours" : " hour";
				cout << runtime << hourS;

			}
		};
		class Historical : public RunTimeCalculation {
		public:
			Historical(Plastic inputPlastic) : RunTimeCalculation(inputPlastic) {}
			virtual ~Historical() {
				DTORF("  ~Historical");
			}
		public:
			virtual void compute(int orderSize, int cavities, int shapeVolume, string metal) {
				int cycletime = legacy_classes::cycleTime_sec(metal, shapeVolume);
				int runtime = (legacy_classes::setupAvg_min + ((orderSize / cavities) * cycletime / 60) + legacy_classes::teardownAvg_min) / 60;
				string hourS = (runtime > 1) ? " hours" : " hour";
				cout << runtime << hourS;
			}
		};
		class Projection : public RunTimeCalculation {
		public:
			Projection(Plastic inputPlastic) : RunTimeCalculation(inputPlastic) {}
			virtual ~Projection() {
				DTORF("  ~Projection");
			}
		public:
			virtual void compute(int orderSize, int cavities, int shapeVolume, string metal) {
				int cycletime = legacy_classes::cycleTime_sec(metal, shapeVolume);
			
				float rejectRate_pcnt = 0;
				if (PET == plasticType) {
					rejectRate_pcnt = legacy_classes::rejectRateHigh_pcnt;
				}
				else {
					rejectRate_pcnt = legacy_classes::rejectRateLow_pcnt;
				}

				int runtime = ((legacy_classes::setupAvg_min) + ((1 + 0.01*rejectRate_pcnt) *orderSize / cavities * cycletime / 60) + legacy_classes::teardownAvg_min) / 60;
				string hourS = (runtime > 1) ? " hours" : " hour";
				cout << runtime << hourS;
			}
		};
		// Seam point - add another algorithm.

	}

	namespace observer {		// DP 5.
		class BinObserver;
		class BinSubject {
			list<BinObserver*> observers;
			string standard;
		public:
			BinSubject(string inputStandard) :standard(inputStandard) {}
			~BinSubject() {
				DTORF("  ~observer::BinSubject\n");
			}

			void attach(BinObserver* obs) { observers.push_back(obs); }
			void detach(BinObserver* obs) { observers.remove(obs); }
			void BinSubject::notify();
		};

		class BinObserver {
		protected:
			BinSubject& subject;
			string standard;
		public:
			BinObserver(BinSubject& target) : subject(target) {
				subject.attach(this);
			}
			virtual ~BinObserver() {
				subject.detach(this);
				DTORF("  ~observer::BinObserver\n");
			}
		public:
			virtual void fullBin(string standard) {}

		};
		void BinSubject::notify() {
			for (list<BinObserver*>::iterator it = observers.begin(); it != observers.end(); it++) {
				(*it)->fullBin(standard);
			}
		}


		// Seam point - add another listener.

	}

	class ConveyerBelt : public observer::BinObserver {
	public: ConveyerBelt(observer::BinSubject& binSubj) : BinObserver(binSubj) {}
	public: virtual ~ConveyerBelt() {
		DTORF(" ~ConveyerBelt");
	}
	public:
		virtual void print() = 0;
		virtual void fullBin(string standard) {}
	};
	class Linear : public ConveyerBelt {
	public: Linear(observer::BinSubject& binSubj) : ConveyerBelt(binSubj) {}
	public: virtual ~Linear() {
		DTORF(" ~Linear");
	}
	public:
		virtual void print() { cout << "Linear"; }
		void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		string pauseName() { return "Linear conveyer belt"; }
	};
	class V_Level : public ConveyerBelt {
	public: V_Level(observer::BinSubject& binSubj) : ConveyerBelt(binSubj) {}
	public: virtual ~V_Level() {
		DTORF(" ~V_Level");
	}
	public:
		virtual void print() { cout << "V-Level"; }
		void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		string pauseName() { return "V-Level conveyer belt"; }
	};
	class Y_Split : public ConveyerBelt {
	public: Y_Split(observer::BinSubject& binSubj) : ConveyerBelt(binSubj) {}
	public: virtual ~Y_Split() {
		DTORF(" ~Y_Split");
	}
	public:
		virtual void print() { cout << "Y-Split"; }
		void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		string pauseName() { return "Y-Split conveyer belt"; }
	};

	class MoldMetal {
	public: MoldMetal(int cavityNum) : cavityCount(cavityNum) {}
	public: virtual ~MoldMetal() {
		DTORF(" ~MoldMetal");
	}
	public:
		virtual void print() = 0;
		virtual string getMoldMetalLC() = 0;
		virtual int getCavityCount() = 0;
		int cavityCount;
	};
	class Aluminum : public MoldMetal {
	public: Aluminum(int cavityNum) : MoldMetal(cavityNum) {}
	public: virtual ~Aluminum() {
		DTORF(" ~Aluminum");
	}
	public:
		void print() { cout << "Aluminum(" << cavityCount << ")"; }
		string getMoldMetalLC() { return "aluminum"; }
		int getCavityCount() { return cavityCount; }
	};
	class Steel : public MoldMetal {
	public: Steel(int cavityNum) : MoldMetal(cavityNum) {}
	public: virtual ~Steel() {
		DTORF(" ~Steel");
	}
	public:
		void print() { cout << "Steel(" << cavityCount << ")"; }
		string getMoldMetalLC() { return "steel"; }
		int getCavityCount() { return cavityCount; }
	};

	class InjectionMoldMachine : public observer::BinObserver {
	public: InjectionMoldMachine(observer::BinSubject& binSubj) : BinObserver(binSubj) {}
	public: virtual ~InjectionMoldMachine() {
		DTORF(" ~InjectionMoldMachine");
	}
	public:
		virtual void print() = 0;
		virtual void fullBin(string standard) {}
	};
	class IJM_110_m : public InjectionMoldMachine {
	public: IJM_110_m(observer::BinSubject& binSubj) : InjectionMoldMachine(binSubj) {}
	public: virtual ~IJM_110_m() {
		DTORF(" ~IJM_110_m");
	}
	public:
		void print() { cout << "IJM_110"; };
		void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		string pauseName() { return "IJM_110"; }
	};
	class IJM_140_m : public InjectionMoldMachine {
	public: IJM_140_m(observer::BinSubject& binSubj) : InjectionMoldMachine(binSubj) {}
	public: virtual ~IJM_140_m() {
		DTORF(" ~IJM_140_m");
	}
	public:
		void print() { cout << "IJM_140"; };
		void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		string pauseName() { return "IJM_140"; }
	};
	class IJM_120_m : public InjectionMoldMachine {
	public: IJM_120_m(observer::BinSubject& binSubj) : InjectionMoldMachine(binSubj) {}
	public: virtual ~IJM_120_m() {
		DTORF(" ~IJM_120_m");
	}
	public:
		void print() { cout << "IJM_120"; };
		void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		string pauseName() { return "IJM_120"; }
	};
	class IJM_210_m : public InjectionMoldMachine {
	public: IJM_210_m(observer::BinSubject& binSubj) : InjectionMoldMachine(binSubj) {}
	public: virtual ~IJM_210_m() {
		DTORF(" ~IJM_210_m");
	}
	public:
		void print() { cout << "IJM_210"; };
		void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		string pauseName() { return "IJM_210"; }
	};
	class IJM_220_m : public InjectionMoldMachine {
	public: IJM_220_m(observer::BinSubject& binSubj) : InjectionMoldMachine(binSubj) {}
	public: virtual ~IJM_220_m() {
		DTORF(" ~IJM_220_m");
	}
	public:
		void print() { cout << "IJM_220"; };
		void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		string pauseName() { return "IJM_220"; }
	};
	class IJM_240_m : public InjectionMoldMachine {
	public: IJM_240_m(observer::BinSubject& binSubj) : InjectionMoldMachine(binSubj) {}
	public: virtual ~IJM_240_m() {
		DTORF(" ~IJM_240_m");
	}
	public:
		void print() { cout << "IJM_240"; };
		void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		string pauseName() { return "IJM_240"; }
	};

	class PartsBin : public observer::BinSubject {
	public: virtual ~PartsBin() {
		DTORF(" ~PartsBin");
	}
	public:
		PartsBin(string explanation) :BinSubject(explanation) {}
		virtual void print() = 0;
	};
	class CardboardBox : public PartsBin {
	public: virtual ~CardboardBox() {
		DTORF(" ~CardboardBox");
	}
			string explanation = "paused while CardboardBox parts bin was swapped.\n";
	public:
		CardboardBox() :PartsBin("paused while CardboardBox parts bin was swapped.\n") {}
		void print() { cout << "CardboardBox"; }

	};
	class ShellBox : public PartsBin {
	public: virtual ~ShellBox() {
		DTORF(" ~ShellBox");
	}
			const string explanation = "paused while ShellBox parts bin was swapped.\n";
	public:
		ShellBox() :PartsBin("paused while ShellBox parts bin was swapped.\n") {}
		void print() { cout << "ShellBox"; }
	};
	class PalletBox : public PartsBin {
	public: virtual ~PalletBox() {
		DTORF(" ~PalletBox");
	}
			const string explanation = "paused while PalletBox parts bin was swapped.\n";
	public:
		PalletBox() :PartsBin("paused while PalletBox parts bin was swapped.\n") {}
		void print() { cout << "PalletBox"; }
	};
	class Crate : public PartsBin {
	public: virtual ~Crate() {
		DTORF(" ~Crate");
	}
			const string explanation = "paused while PalletBox parts bin was swapped.\n";
	public:
		Crate() :PartsBin("paused while Crate parts bin was swapped.\n") {}
		void print() { cout << "Crate"; }
	};
	namespace factory_method {	// DP 4.
		class Packaging : public observer::BinObserver {
		public: Packaging(observer::BinSubject& binSubj) : BinObserver(binSubj) {}
				virtual ~Packaging() {
					DTORF(" ~factory_method::Packaging\n");
				}
		public:
			virtual string pauseName() { return""; }
			virtual void fullBin(string standard) {}
		public:
			static Packaging* makeObject(observer::BinSubject& binSubj, Packager criteria);
		};
		class ShrinkWrap : public Packaging {
		public: ShrinkWrap(observer::BinSubject& binSubj) : Packaging(binSubj) {}
				virtual ~ShrinkWrap() {
					DTORF(" ~ShrinkWrap;");
				}
		public:
			string pauseName() { return"Shrink wrap packager"; }
			void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		};
		class ZipLock : public Packaging {
		public: ZipLock(observer::BinSubject& binSubj) : Packaging(binSubj) {}
				virtual ~ZipLock() {
					DTORF(" ~ZipLock;");
				}
		public:
			string pauseName() { return"Ziplock packager"; }
			void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		};
		class HardPack : public Packaging {
		public: HardPack(observer::BinSubject& binSubj) : Packaging(binSubj) {}
				virtual ~HardPack() {
					DTORF(" ~HardPack");
				}
		public:
			string pauseName() { return"Hard pack packager"; }
			virtual void fullBin(string standard) { binFullPrint(pauseName(), standard); }
		};

		// Seam point - add another class.

	}
	namespace abstract_factory {// DP 9.
		class InjectionLineOrder {
		public: virtual ~InjectionLineOrder() {
			DTORF(" ~abstract_factory::InjectionLineOrder\n");
		}
		public:
			virtual PartsBin* create_Bin() = 0;
			virtual InjectionMoldMachine* create_Injection_Mold_Machine(observer::BinSubject& alpha) = 0;
			virtual MoldMetal* create_Mold_Metal() = 0;
			virtual ConveyerBelt* create_Conveyor_Belt(observer::BinSubject& alpha) = 0;
			factory_method::Packaging* create_Packaging(observer::BinSubject& alpha, Packager type) {
				return factory_method::Packaging::makeObject(alpha, type);
			}

		public:
			static InjectionLineOrder*  makeObject(int count);
		};
		class PilotOrder : public InjectionLineOrder {
		public: virtual ~PilotOrder() {
			DTORF(" ~PilotOrder");
		}
		public:
			PartsBin* create_Bin() { return new CardboardBox; }
			InjectionMoldMachine* create_Injection_Mold_Machine(observer::BinSubject& alpha) { return new IJM_110_m(alpha); }
			MoldMetal* create_Mold_Metal() { return new Aluminum(1); }

			ConveyerBelt* create_Conveyor_Belt(observer::BinSubject& alpha) { return new Linear(alpha); }

		};
		class SmallOrder : public InjectionLineOrder {
		public: virtual ~SmallOrder() {
			DTORF(" ~SmallOrder");
		}
		public:
			PartsBin* create_Bin() { return new ShellBox; }
			InjectionMoldMachine* create_Injection_Mold_Machine(observer::BinSubject& alpha) { return new IJM_120_m(alpha); }
			MoldMetal* create_Mold_Metal() {
				return new Aluminum(2);
			}
			ConveyerBelt* create_Conveyor_Belt(observer::BinSubject& alpha) { return new Y_Split(alpha); }
		};
		class MediumOrder : public InjectionLineOrder {
		public: virtual ~MediumOrder() {
			DTORF(" ~MediumOrder");
		}
		public:
			PartsBin* create_Bin() { return new PalletBox; }
			InjectionMoldMachine* create_Injection_Mold_Machine(observer::BinSubject& alpha) { return new IJM_210_m(alpha); }
			MoldMetal* create_Mold_Metal() {
				return new Steel(1);
			}
			ConveyerBelt* create_Conveyor_Belt(observer::BinSubject& alpha) { return new Linear(alpha); }
		};
		class FastOrder : public InjectionLineOrder {
		public: virtual ~FastOrder() {
			DTORF(" ~FastOrder");
		}
		public:
			PartsBin* create_Bin() { return new ShellBox; }
			InjectionMoldMachine* create_Injection_Mold_Machine(observer::BinSubject& alpha) { return new IJM_140_m(alpha); }
			MoldMetal* create_Mold_Metal() {
				return new Aluminum(4);
			}
			ConveyerBelt* create_Conveyor_Belt(observer::BinSubject& alpha) { return new V_Level(alpha); }
		};

		class LargeOrder : public InjectionLineOrder {
		public: virtual ~LargeOrder() {
			DTORF(" ~LargeOrder");
		}
		public:
			PartsBin* create_Bin() { return new PalletBox; }
			InjectionMoldMachine* create_Injection_Mold_Machine(observer::BinSubject& alpha) { return new IJM_220_m(alpha); }
			MoldMetal* create_Mold_Metal() {
				return new Steel(2);
			}
			ConveyerBelt* create_Conveyor_Belt(observer::BinSubject& alpha) { return new Y_Split(alpha); }
		};

		class HugeOrder : public InjectionLineOrder {
		public: virtual ~HugeOrder() {
			DTORF(" ~HugeOrder");
		}
		public:
			PartsBin* create_Bin() { return new Crate; }
			InjectionMoldMachine* create_Injection_Mold_Machine(observer::BinSubject& alpha) { return new IJM_240_m(alpha); }
			MoldMetal* create_Mold_Metal() {
				return new Steel(4);
			}
			ConveyerBelt* create_Conveyor_Belt(observer::BinSubject& alpha) { return new V_Level(alpha); }
		};

		InjectionLineOrder*  InjectionLineOrder::makeObject(int count) {
			if (count <= 10000) {
				return new PilotOrder;
			}
			else if (count <= 20000) {
				return new SmallOrder;
			}
			else if (count <= 40000) {
				return new FastOrder;
			}
			else if (count <= 50000) {
				return new MediumOrder;
			}
			else if (count <= 100000) {
				return new LargeOrder;
			}
			else if (count <= 200000) {
				return new HugeOrder;
			}

		}
		// Seam point - add another type 1.
		// Seam point - add another type 2.
		// Seam point - add another type 3.
		// Seam point - add another ...
		// Seam point - add another type N.
		// Seam point - add another family.

	}

	namespace bridge {			// DP 8.
		class Shape {
		public: virtual ~Shape() {
			DTORF(" ~bridge::Shape\n");
		}
		public:
			virtual void printName() { cout << "oops this is no shape name"; }
			virtual void printMillSteps() { cout << "oops this is no mill step"; }
			static Shape* makeObject(Mold shape);
		};
		class Duck_s : public Shape {
		public: virtual ~Duck_s() {
			DTORF(" ~Duck_s");
		}
		public:
			virtual void printName() { cout << "duck"; }
			virtual void printMillSteps() { cout << "D3C2D8G4"; }
		};
		class Car_s : public Shape {
		public: virtual ~Car_s() {
			DTORF(" ~Car_s");
		}
		public:
			virtual void printName() { cout << "car"; }
			virtual void printMillSteps() { cout << "D2C8D1G3"; }
		};
		class Hero_s : public Shape {
		public: virtual ~Hero_s() {
			DTORF(" ~Hero_s");
		}
		public:
			virtual void printName() { cout << "hero"; }
			virtual void printMillSteps() { cout << "D1C1D1G7"; }
		};
		class Dino_s : public Shape {
		public: virtual ~Dino_s() {
			DTORF(" ~Dino_s");
		}
		public:
			virtual void printName() { cout << "dino"; }
			virtual void printMillSteps() { cout << "G5D2C1"; }
		};
		Shape* Shape::makeObject(Mold shape) {
			if (DUCK == shape) return new Duck_s;
			if (CAR == shape) return new Car_s;
			if (HERO == shape) return new Hero_s;
			if (DINO == shape) return new Dino_s;
		}
		class MillPlats {
		public:
			Shape* shape;
			MoldMetal* userMetal;
		public:
			MillPlats(Shape* inputShape, MoldMetal* inputMetal) : shape(inputShape), userMetal(inputMetal) {}
			virtual ~MillPlats() {
				if (shape) delete shape;
				DTORF(" ~bridge::MillPlats");
			}
		public:
			virtual void run() { shape->printName(); }
		public:
			static MillPlats* makeObject(Finish finish, Shape* shape, MoldMetal* inMetal);
		};
		class Smooth : public MillPlats {
		public:
			Smooth(Shape* inputShape, MoldMetal* inputMetal) : MillPlats(inputShape, inputMetal) {}
			virtual ~Smooth() {
				DTORF(" ~Smooth");
			}
		public:
			virtual void run() {
				cout << "    Create ";
				shape->printName();
				cout << " mold from mill with ";
				cout << userMetal->getCavityCount();
				cout << " cavities";
				cout << " - steps: ";
				shape->printMillSteps();
				cout << ".\n      using HighCarbon tools (drill, cut, and high speed grind) to mill ";
				cout << userMetal->getMoldMetalLC();
				cout << " block into ";
				cout << userMetal->getCavityCount();
				cout << " ";
				shape->printName();
				string printCorrectShape = (userMetal->getCavityCount() > 1) ? " shapes" : " shape";
				cout << printCorrectShape << " with ";
				cout << "smooth finish.\n";
			}
		};
		class Rippled : public MillPlats {
		public:
			Rippled(Shape* inputShape, MoldMetal* inputMetal) : MillPlats(inputShape, inputMetal) {}
			virtual ~Rippled() {
				DTORF(" ~Rippled");
			}
		public:
			virtual void run() {
				cout << "    Create ";
				shape->printName();
				cout << " mold from mill with ";
				cout << userMetal->getCavityCount();
				cout << " cavities";
				cout << " - steps: ";
				shape->printMillSteps();
				cout << ".\n      using Carbide tools (high speed drill, cross cut, and layer grind) to mill ";
				cout << userMetal->getMoldMetalLC();
				cout << " block into ";
				cout << userMetal->getCavityCount();
				cout << " ";
				shape->printName();
				string printCorrectShape = (userMetal->getCavityCount() > 1) ? " shapes" : " shape";
				cout << printCorrectShape << " with ";
				cout << "rippled finish.\n";

			}
		};
		class Dimpled : public MillPlats {
		public:
			Dimpled(Shape* inputShape, MoldMetal* inputMetal) : MillPlats(inputShape, inputMetal) {}
			virtual ~Dimpled() {
				DTORF(" ~Dimpled");
			}
		public:
			virtual void run() {
				cout << "    Create ";
				shape->printName();
				cout << " mold from mill with ";
				cout << userMetal->getCavityCount();
				cout << " cavities";
				cout << " - steps: ";
				shape->printMillSteps();
				cout << ".\n      using DiamondTipped tools (precision drill, oil cooled cut, and cartoid grind) to mill ";
				cout << userMetal->getMoldMetalLC();
				cout << " block into ";
				cout << userMetal->getCavityCount();
				cout << " ";
				shape->printName();
				string printCorrectShape = (userMetal->getCavityCount() > 1) ? " shapes" : " shape";
				cout << printCorrectShape << " with ";
				cout << "dimpled finish.\n";

			}
		};
		class Sanded : public MillPlats {
		public:
			Sanded(Shape* inputShape, MoldMetal* inputMetal) : MillPlats(inputShape, inputMetal) {}
			virtual ~Sanded() {
				DTORF(" ~Sanded");
			}
		public:
			virtual void run() {
				cout << "    Create ";
				shape->printName();
				cout << " mold from mill with ";
				cout << userMetal->getCavityCount();
				cout << " cavities";
				cout << " - steps: ";
				shape->printMillSteps();
				cout << ".\n      using EDM tools (static punch, static slice, and static etch) to mill ";
				cout << userMetal->getMoldMetalLC();
				cout << " block into ";
				cout << userMetal->getCavityCount();
				cout << " ";
				shape->printName();
				string printCorrectShape = (userMetal->getCavityCount() > 1) ? " shapes" : " shape";
				cout << printCorrectShape << " with ";
				cout << "sanded finish.\n";

			}
		};
		class Satin : public MillPlats {
		public:
			Satin(Shape* inputShape, MoldMetal* inputMetal) : MillPlats(inputShape, inputMetal) {}
			virtual ~Satin() {
				DTORF(" ~Satin");
			}
		public:
			virtual void run() {
				cout << "    Create ";
				shape->printName();
				cout << " mold from mill with ";
				cout << userMetal->getCavityCount();
				cout << " cavities";
				cout << " - steps: ";
				shape->printMillSteps();
				cout << ".\n      using EDM tools (static punch, static slice, and static etch) to mill ";
				cout << userMetal->getMoldMetalLC();
				cout << " block into ";
				cout << userMetal->getCavityCount();
				cout << " ";
				shape->printName();
				string printCorrectShape = (userMetal->getCavityCount() > 1) ? " shapes" : " shape";
				cout << printCorrectShape << " with ";
				cout << "satin finish.\n";

			}
		};
		MillPlats* MillPlats::makeObject(Finish finish, Shape* shape, MoldMetal* inMetal) {
			if (SMOOTH == finish) return new Smooth(shape, inMetal);
			if (RIPPLED == finish) return new Rippled(shape, inMetal);
			if (DIMPLED == finish) return new Dimpled(shape, inMetal);
			if (SANDED == finish) return new Sanded(shape, inMetal);
			if (SATIN == finish) return new Satin(shape, inMetal);
		}
		// Seam Point - add another implementation.
		// Seam Point - add another abstraction.

	}

	namespace chain_of_resp {	// DP 7.
		class MoldLocation {
		protected:
			MoldLocation* successor;
		public:
			MoldLocation() : successor(0) {}
			virtual ~MoldLocation() {
				DTORF(" ~chain_of_resp::MoldLocation");
				delete successor;
			}
		public:
			void setSuccessor(MoldLocation* next) { successor = next; }
		public:
			virtual void delegate(MoldLoc location, Mold moldChoice, Finish finChoice, MoldMetal* userMetal) {
				cout << "How did you get here?";
			}
		public:
			static MoldLocation* makeObject(MoldLoc location);
		};
		class Inventory : public MoldLocation {
		public:
			virtual ~Inventory() {
				DTORF(" ~Inventory");
			}
		public:
			virtual void delegate(MoldLoc location, Mold moldChoice, Finish finChoice, MoldMetal* userMetal) {
				if (INVENTORY == location) {
					cout << "    Pull ";
					cout << MoldDictionaryToString[moldChoice];
					cout << " mold from inventory.\n";
				}
				else {
					successor->delegate(location, moldChoice, finChoice, userMetal);
				}
			}
		};
		class SisterCompany : public MoldLocation {
		public:
			virtual ~SisterCompany() {
				DTORF(" ~SisterCompany");
			}
		public:
			virtual void delegate(MoldLoc location, Mold moldChoice, Finish finChoice, MoldMetal* userMetal) {
				if (SISTER_COMPANY == location) {
					cout << "    Borrow ";
					cout << MoldDictionaryToString[moldChoice];
					cout << " mold from sister company.\n";
				}
				else {
					successor->delegate(location, moldChoice, finChoice,  userMetal);
				}
			}
		};

		class Purchase : public MoldLocation {
		public:
			virtual ~Purchase() {
				DTORF(" ~Purchase");
			}
		public:
			virtual void delegate(MoldLoc location, Mold moldChoice, Finish finChoice, MoldMetal* userMetal) {
				if (PURCHASE == location) {
					cout << "    Acquire ";
					cout << MoldDictionaryToString[moldChoice];
					cout << " mold via purchase.\n";
				}
				else {
					successor->delegate(location, moldChoice, finChoice, userMetal);
				}
			}
		};
		class Mill : public MoldLocation {
		public:
			virtual ~Mill() {
				DTORF(" ~Mill");
			}
		public:
			virtual void delegate(MoldLoc location, Mold moldChoice, Finish finChoice, MoldMetal* userMetal) {
				if (MILL == location) {
					bridge::Shape* currentShape = bridge::Shape::makeObject(moldChoice);
					bridge::MillPlats* currentMillPlat = bridge::MillPlats::makeObject(finChoice, currentShape, userMetal);
					currentMillPlat->run();

					delete currentMillPlat;
				}
				else {
					successor->delegate(location, moldChoice, finChoice, userMetal);
				}
			}
		};

		// Seam points - add another responder.
		MoldLocation* MoldLocation::makeObject(MoldLoc location) {
			if (INVENTORY == location) return new Inventory;
			if (SISTER_COMPANY == location) return new SisterCompany;
			if (PURCHASE == location) return new Purchase;
			if (MILL == location) return new Mill;
		}
	}

	namespace decorator {		// DP 6.
		class Cavity {
		public: virtual ~Cavity() {
			DTORF(" ~decorator::Cavity");
		}
		public:
			virtual void behavior() = 0;
			virtual int mmSizeTotal() = 0;
		};
		class Blank : public Cavity {
		public: ~Blank() {
			DTORF(" ~Blank");
		}
		public:
			void behavior() {}
			virtual int mmSizeTotal() { return 0; }
		};
		class Tags_d : public Cavity {
		protected:
			Cavity* decorator;
			int tagMM = 0;
		public:
			Tags_d(Cavity* inputDecorator) : decorator(inputDecorator) {}
			~Tags_d() {
				delete decorator;
				DTORF(" ~Tags_d");
			}
			void behavior() {}
			virtual int mmSizeTotal() { return 0; }
		public:
			static Cavity* makeObject(Cavity* inputDecorator, Tags tagType);
		};
		class ModelNum : public Tags_d {
		public:
			int tagMM = 2;
			ModelNum(Cavity* decorator) : Tags_d(decorator) {}
			~ModelNum() {
				DTORF(" ~ModelNum");
			}
		public:
			void behavior() { decorator->behavior(); cout << " ModelNumber"; }
			int mmSizeTotal() { return tagMM + decorator->mmSizeTotal(); }
		};
		class Country : public Tags_d {
		public:
			int tagMM = 2;
			Country(Cavity* decorator) : Tags_d(decorator) {}
			~Country() {
				DTORF(" ~Country");
			}
		public:
			void behavior() { decorator->behavior(); cout << " Country"; }
			int mmSizeTotal() { return tagMM + decorator->mmSizeTotal(); }
		};
		class Date : public Tags_d {
		public:
			int tagMM = 2;
			Date(Cavity* decorator) : Tags_d(decorator) {}
			~Date() {
				DTORF(" ~Date");
			}
		public:
			void behavior() { decorator->behavior(); cout << " Date"; }
			int mmSizeTotal() { return tagMM + decorator->mmSizeTotal(); }
		};
		class IncCounter : public Tags_d {
		public:
			int tagMM = 4;
			IncCounter(Cavity* decorator) : Tags_d(decorator) {}
			~IncCounter() {
				DTORF(" ~IncCounter");
			}
		public:
			void behavior() { decorator->behavior(); cout << " IncCounter"; }
			int mmSizeTotal() { return tagMM + decorator->mmSizeTotal(); }
		};
		class PartNumber : public Tags_d {
		public:
			int tagMM = 2;
			PartNumber(Cavity* decorator) : Tags_d(decorator) {}
			~PartNumber() {
				DTORF(" ~PartNumber");
			}
		public:
			void behavior() { decorator->behavior(); cout << " PartNumber"; }
			int mmSizeTotal() { return tagMM + decorator->mmSizeTotal(); }
		};
		class RecycleCode : public Tags_d {
		public:
			int tagMM = 6;
			RecycleCode(Cavity* decorator) : Tags_d(decorator) {}
			~RecycleCode() {
				DTORF(" ~PartNumber");
			}
		public:
			void behavior() { decorator->behavior(); cout << " RecycleCode"; }
			int mmSizeTotal() { return tagMM + decorator->mmSizeTotal(); }
		};

		class Mixture {
		public: virtual ~Mixture() {
			DTORF(" ~decorator::Mixture");
		}
		public:
			virtual void behavior() = 0;
		};
		class Plastic : public Mixture {
		public: ~Plastic() {
			DTORF(" ~Plastic");
		}
		public:
			void behavior() {}
		};
		class Ingredient : public Mixture {
		protected:
			Mixture* decorator;
			int volume;
		public:
			Ingredient(Mixture* inputDecorator, int inputVolume) : decorator(inputDecorator), volume(inputVolume) {}
			~Ingredient() {
				delete decorator;
				DTORF(" ~Ingredient");
			}
			void behavior() {}
		public:
			static Mixture* makeObject(Mixture* inputDecorator, IngredientDetails ingredientType);
		};
		class UVInhibiter : public Ingredient {
		public:
			UVInhibiter(Mixture* decorator, int inputVolume) : Ingredient(decorator, inputVolume) {}
			~UVInhibiter() {
				DTORF(" ~UVInhibiter");
			}
		public:
			void behavior() { decorator->behavior(); cout << " UVInhibiter(" << volume << ")"; }
		};
		class AntiBacterial : public Ingredient {
		public:
			AntiBacterial(Mixture* decorator, int inputVolume) : Ingredient(decorator, inputVolume) {}
			~AntiBacterial() {
				DTORF(" ~AntiBacterial");
			}
		public:
			void behavior() { decorator->behavior(); cout << " AntiBacterial(" << volume << ")"; }
		};
		class Hydrophilic : public Ingredient {
		public:
			Hydrophilic(Mixture* decorator, int inputVolume) : Ingredient(decorator, inputVolume) {}
			~Hydrophilic() {
				DTORF(" ~Hydrophilic");
			}
		public:
			void behavior() { decorator->behavior(); cout << " Hydrophilic(" << volume << ")"; }
		};
		class MicroFibers : public Ingredient {
		public:
			MicroFibers(Mixture* decorator, int inputVolume) : Ingredient(decorator, inputVolume) {}
			~MicroFibers() {
				DTORF(" ~MicroFibers");
			}
		public:
			void behavior() { decorator->behavior(); cout << " MicroFibers(" << volume << ")"; }
		};

		Cavity* Tags_d::makeObject(Cavity* inputDecorator, Tags tagType) {
			if (MODELNUMBER == tagType) return new ModelNum(inputDecorator);
			if (COUNTRY == tagType) return new Country(inputDecorator);
			if (DATE == tagType) return new Date(inputDecorator);
			if (INCCOUNTER == tagType) return new IncCounter(inputDecorator);
			if (PARTNUMBER == tagType) return new PartNumber(inputDecorator);
			if (RECYCLECODE == tagType) return new RecycleCode(inputDecorator);
		}

		Mixture* Ingredient::makeObject(Mixture* inputDecorator, IngredientDetails ingredientType) {
			if (UVINHIBITER == ingredientType.ingredient) return new UVInhibiter(inputDecorator, ingredientType.volume);
			if (ANTIBACTERIAL == ingredientType.ingredient) return new AntiBacterial(inputDecorator, ingredientType.volume);
			if (HYDROPHILIC == ingredientType.ingredient) return new Hydrophilic(inputDecorator, ingredientType.volume);
			if (MICROFIBERS == ingredientType.ingredient) return new MicroFibers(inputDecorator, ingredientType.volume);
		}
		// Seam point - add another option.

	}


	factory_method::Packaging* factory_method::Packaging::makeObject(observer::BinSubject& binSubj, Packager criteria) {
		Packaging * returnPackage = nullptr;

		switch (criteria) {
		case SHRINKWRAP:
			returnPackage = new ShrinkWrap(binSubj);
			break;
		case HARDPACK:
			returnPackage = new HardPack(binSubj);
			break;
		case ZIPLOCK:
			returnPackage = new ZipLock(binSubj);
		default:
			break;
		}

		return returnPackage;

	}

	namespace template_method {	// DP 3.
		class RunSteps {
		public: virtual ~RunSteps() {
			DTORF(" ~template_method::RunSteps");
		}
				virtual void Close() { cout << "Close"; }
				virtual void Mix() { cout << "mix"; }
				virtual void Heat() = 0;
				virtual void Inject() = 0;
				virtual void Cool() = 0;
				virtual void Separate() { cout << "separate"; }
				virtual void Eject() = 0;
		};
		class ABS_Steps : public RunSteps {
		public: virtual ~ABS_Steps() {
			DTORF(" ~ABS_Steps");
		}

				void Heat() { cout << "heat to 440"; };
				void Inject() { cout << "inject at 125 PSI"; };
				void Cool() { cout << "cool to 360"; };
				void Eject() { cout << "progressive eject"; };
		};
		class Poly_Steps : public RunSteps {
		public: virtual ~Poly_Steps() {
			DTORF(" ~Poly_Steps");
		}

				void Heat() { cout << "heat to 350"; };
				void Inject() { cout << "inject at 90 PSI"; };
				void Cool() { cout << "cool to 290"; };
				void Eject() { cout << "smooth eject"; };
		};
		class PET_Steps : public RunSteps {
		public: virtual ~PET_Steps() {
			DTORF(" ~PET_Steps");
		}

				void Heat() { cout << "heat to 404"; };
				void Inject() { cout << "inject at 110 PSI"; };
				void Cool() { cout << "cool to 340"; };
				void Eject() { cout << "smooth eject"; };
		};
		class Styrene_Steps : public RunSteps {
		public: virtual ~Styrene_Steps() {
			DTORF(" ~Styrene_Steps");
		}


				void Heat() { cout << "heat to 480"; };
				void Inject() { cout << "inject at 150 PSI"; };
				void Cool() { cout << "cool to 390"; };
				void Eject() { cout << "shock eject"; };
		};
		class Nylon66_Steps : public RunSteps {
		public: virtual ~Nylon66_Steps() {
			DTORF(" ~Nylon66_Steps");
		}


				void Heat() { cout << "heat to 480"; };
				void Inject() { cout << "inject at 150 PSI"; };
				void Cool() { cout << "cool to 390"; };
				void Eject() { cout << "shock eject"; };
		};

		// Seam point - add another polymorphic step.
		// Seam point - add another constant step.
		// Seam point - convert a constant step into a polymorphic step.

	}

	//Packager packagerChoice;
	class ProcessOrder {
	public:

		ProcessOrder(map<string, string>& order) {
			map<string, string>::iterator it;

			//Find the plastic type in the order
			it = order.find("plastic");
			if (it != order.end()) {
				string temp = it->second;
				std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				plasticChoice = PlasticDictionary[temp];
			}
			else {
				plasticChoice = ABS;
				//cout << "  <>Unknown plastic |" << it->second << "| defaulting to 'ABS'.\n";
				cout << "  <>Unknown plastic || defaulting to 'ABS'.\n";
			}

			//Find the size in the order
			it = order.find("size");
			if (it != order.end()) {
				orderSize = atoi(it->second.c_str());
				if (orderSize > 200000) {
					cout << "  <>Size exceeds mold lifetime |" << orderSize << "| defaulting to HugeOrder of 200000.\n";
					orderSize = 200000;
				}
			}
			else {
				orderSize = 100;
				cout << "  <>No size specified, defaulting to 100.\n";
			}

			//Find the packaging type in the order
			it = order.find("packager");
			if (it != order.end()) {
				string temp = it->second;
				std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				packagerChoice = PackagerDictionary[temp];
			}
			else {
				packagerChoice = BULK;
				cout << "  <>Unknown packager || defaulting to 'None'.\n";
			}

			//Find the packaging type in the order
			it = order.find("stuffer");
			if (it != order.end()) {
				string temp = it->second;
				std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				stufferChoice = StufferDictionary[temp];
			}
			else {
				stufferChoice = AIR;
				cout << "  <>Unknown stuffer || defaulting to 'None'.\n";
			}

			//Find the color type in the order
			it = order.find("color");
			if (it != order.end()) {
				string temp = it->second;
				std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				colorChoice = ColorDictionary[temp];
			}
			else {
				colorChoice = CLEAR;
			}

			//Find the Finish type in the order
			it = order.find("finish");
			if (it != order.end()) {
				string temp = it->second;
				std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				finishChoice = FinishDictionary[temp];
			}
			else {
				finishChoice = SMOOTH;
			}

			//Find the order Number in the order
			it = order.find("orderNum");
			if (it != order.end()) {
				orderNumber = atoi(it->second.c_str());
			}

			//Find the comment in the order
			it = order.find("comment");
			if (it != order.end()) {
				comment = it->second;
			}
			else {
				comment = "";
			}

			//Find the Address to ship to in the order
			it = order.find("address");
			if (it != order.end()) {
				address = it->second;
			}
			else {
				address = "";
			}

			printInjectionLine();

			cout << "  Process order:\n";

			//Find the mold type in the order
			it = order.find("mold");
			if (it != order.end()) {
				string temp = it->second;
				std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				moldChoice = MoldDictionary[temp];
			}
			else {
				moldChoice = DUCK;
				cout << "    <>Unknown mold || defaulting to 'duck'.\n";
			}
			switch (moldChoice) {
			case DINO:
				shapeVolume = 30;
				break;
			case DUCK:
				shapeVolume = 35;
				break;
			case CAR:
				shapeVolume = 40;
				break;
			case HERO:
				shapeVolume = 50;
				break;
			default:
				cout << "something went wrong with picking shape and setting the volume of the shape";
				break;
			}

			//Find the moldLoc type in the order
			it = order.find("moldLoc");
			if (it != order.end()) {
				string temp = it->second;
				std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
				moldLocChoice = MoldLocDictionary[temp];
				getMold();
			}
			else {
				cout << "    <>Can't find place || to get |" << MoldDictionaryToString[moldChoice] << "| mold from with || finish, defaulting to smooth " << MoldDictionaryToString[moldChoice] << " from inventory.\n";
				moldLocChoice = INVENTORY;
				getMold(false);
			}

			//Find the Tags choices in the order
			it = order.find("tags");
			if (it != order.end()) {
				string temp = it->second;
				istringstream iss(temp);
				string s;

				while (getline(iss, s, ' ')) {
					//std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
					if (s.find("ModelNumber") != string::npos) {
						userTags.push_back(MODELNUMBER);
					}
					else if (s.find("Country") != string::npos) {
						userTags.push_back(COUNTRY);
					}
					else if (s.find("Date") != string::npos) {
						userTags.push_back(DATE);
					}
					else if (s.find("IncCounter") != string::npos) {
						userTags.push_back(INCCOUNTER);
					}
					else if (s.find("PartNumber") != string::npos) {
						userTags.push_back(PARTNUMBER);
					}
					else if (s.find("RecycleCode") != string::npos) {
						userTags.push_back(RECYCLECODE);
					}
					else {
						cout << "    Ignoring unknown tag " << s << ".\n";
					}
				}
			}
			getTags();

			//Find the UVInhibiter Number in the order
			it = order.find("UVInhibiter");
			if (it != order.end()) {
				uvInhibitorVolume = atoi(it->second.c_str());
				userIngredients.push_back(IngredientDetails(UVINHIBITER, uvInhibitorVolume));
			}
			else {
				uvInhibitorVolume = 0;
			}

			//Find the AntiBacterial Number in the order
			it = order.find("AntiBacterial");
			if (it != order.end()) {
				antiBacterialVolume = atoi(it->second.c_str());
				userIngredients.push_back(IngredientDetails(ANTIBACTERIAL, antiBacterialVolume));
			}
			else {
				antiBacterialVolume = 0;
			}

			//Find the Hydrophilic Number in the order
			it = order.find("Hydrophilic");
			if (it != order.end()) {
				hydrophilicVolume = atoi(it->second.c_str());
				userIngredients.push_back(IngredientDetails(HYDROPHILIC, hydrophilicVolume));
			}
			else {
				hydrophilicVolume = 0;
			}

			//Find the MicroFibers Number in the order
			it = order.find("MicroFibers");
			if (it != order.end()) {
				microFibersVolume = atoi(it->second.c_str());
				userIngredients.push_back(IngredientDetails(MICROFIBERS, microFibersVolume));
			}
			else {
				microFibersVolume = 0;
			}



			getMixtureAndVolume();
			getRunTimeCalculation();
			getIJMSteps();
			pretendPartsBinIsFull();
			cleanUsedMold();

			//Print Shipping Address
			cout << "    Ship to \"" << address << "\".\n";

			delete decoratorObject;
			delete mixtureObject;
			delete math;
			delete orders;
			delete cleaner;
			delete machine;
			delete belt;
			delete packaging;
			delete bin;
			delete metal;
			delete injectionLine;
			delete moldLocation;




		}


		void printInjectionLine() {
			injectionLine = abstract_factory::InjectionLineOrder::makeObject(orderSize);

			cout << "  Setup injection line for " << orderSize << " order";
			if (BULK != packagerChoice) {
				cout << " with " << PackagerDictionaryToString[packagerChoice] << " packager";
			}
			if (AIR != stufferChoice) {
				cout << " and " << StufferDictionaryToString[stufferChoice] << " stuffer";
			}
			cout << ":\n";


			bin = injectionLine->create_Bin();
			machine = injectionLine->create_Injection_Mold_Machine(*bin);
			metal = injectionLine->create_Mold_Metal();
			belt = injectionLine->create_Conveyor_Belt(*bin);
			packaging = injectionLine->create_Packaging(*bin, packagerChoice);
			cout << "    ";
			machine->print();
			cout << " - ";
			metal->print();
			cout << " - ";
			belt->print();
			cout << " conveyer belt - ";
			bin->print();
			cout << ".\n";
		}

		void getMold(bool runDelegate = true) {
			MoldLoc chain[] = { INVENTORY,SISTER_COMPANY, PURCHASE, MILL };
			moldLocation = chain_of_resp::MoldLocation::makeObject(chain[0]);

			chain_of_resp::MoldLocation* current = moldLocation;
			for (int i = 1; i < COUNT(chain); i++) {
				chain_of_resp::MoldLocation* next = chain_of_resp::MoldLocation::makeObject(chain[i]);
				current->setSuccessor(next);
				current = next;
			}

			current->setSuccessor(new chain_of_resp::MoldLocation);
			if (runDelegate) {
				moldLocation->delegate(moldLocChoice, moldChoice, finishChoice, metal);
			}
		}

		void getTags() {
			vector<Tags>::iterator it;
			decoratorObject = new decorator::Blank;

			int tagCount = 0;

			for (it = userTags.begin(); it < userTags.end(); it++) {
				decoratorObject = decorator::Tags_d::makeObject(decoratorObject, *it);
				tagCount++;
			}

			cout << "    Insert tags [";
			decoratorObject->behavior();
			cout << "] of width " << decoratorObject->mmSizeTotal() << "/20 mm, blank tag is " << 20 - decoratorObject->mmSizeTotal() << " mm.\n";

		}

		void getMixtureAndVolume() {
			vector<IngredientDetails>::iterator it;
			mixtureObject = new decorator::Plastic;
			int colorCount = 0;
			for (it = userIngredients.begin(); it < userIngredients.end(); it++) {
				mixtureObject = decorator::Ingredient::makeObject(mixtureObject, *it);
			}
			if (STYRENE == plasticChoice || NYLON66 == plasticChoice) {

				if (CLEAR != colorChoice) {
					cout << "    Load plastic bin with "<< ColorDictionaryToString[colorChoice] <<"-" << PlasticDictionaryToString[plasticChoice] << " pellets";
				}
				else {
					cout << "    Load plastic bin with " << PlasticDictionaryToString[plasticChoice] << " pellets";
				}
			} 
			else {
				cout << "    Load plastic bin with " << PlasticDictionaryToString[plasticChoice];
				if (CLEAR != colorChoice) {
					cout << " and color bin with " << ColorDictionaryToString[colorChoice];
					colorCount = shapeVolume * .10;
				}

			}
			cout << ".\n";
			int temp = shapeVolume - colorCount - uvInhibitorVolume - antiBacterialVolume - hydrophilicVolume - microFibersVolume;
			
			if (STYRENE == plasticChoice || NYLON66 == plasticChoice) {
				cout << "      Recipe: " << ColorDictionaryToString[colorChoice] << "-"<< PlasticDictionaryToString[plasticChoice] << "(" << temp << ")";
			}
			else {
				cout << "      Recipe: " << PlasticDictionaryToString[plasticChoice] << "(" << temp << ")";
				if (CLEAR != colorChoice || STYRENE == plasticChoice || NYLON66 == plasticChoice) {
					cout << " " << ColorDictionaryToString[colorChoice] << "(" << colorCount << ")";
				}
			}
			mixtureObject->behavior();
			cout << " = " << shapeVolume << " cc.\n";
			int totalVolume = shapeVolume * metal->getCavityCount();
			string cavity = (metal->getCavityCount() > 1) ? " cavities = " : " cavity = ";
			cout << "      Volume: " << MoldDictionaryToString[moldChoice] << "(" << shapeVolume << ") * " << metal->getCavityCount() << cavity << totalVolume << " cc.\n";
		}

		void getRunTimeCalculation() {
			math = nullptr;
			if (orderSize <= 10000) {
				math = new strategy::BackOftheEnvelope(plasticChoice);
			}
			else if (orderSize <= 20000) {
				math = new strategy::Calculation(plasticChoice);
			}
			else if (orderSize <= 50000) {
				math = new strategy::Historical(plasticChoice);
			}
			else if (orderSize <= 100000){
				math = new strategy::Projection(plasticChoice);
			}
			else {
				cout << "    <>Using most sophisticated estimation algorithm for orders greater than 100000.\n";
				math = new strategy::Projection(plasticChoice);
			}
			cout << "    Cycle "; machine->print(); cout << " for " << PlasticDictionaryToString[plasticChoice] << " " << orderSize / metal->getCavityCount() << " times, estimated run time = ";

			math->compute(orderSize, metal->getCavityCount(), shapeVolume, metal->getMoldMetalLC()); cout << ".\n";
		}

		void getIJMSteps() {
			orders = nullptr;
			if (PET == plasticChoice) {
				orders = new template_method::PET_Steps;
			}
			else if (ABS == plasticChoice) {
				orders = new template_method::ABS_Steps;
			}
			else if (STYRENE == plasticChoice) {
				orders = new template_method::Styrene_Steps;
			}
			else if (NYLON66 == plasticChoice) {
				orders = new template_method::Nylon66_Steps;
			}
			else {
				orders = new template_method::Poly_Steps;
			}

			cout << "      "; orders->Close(); cout << " - "; orders->Mix(); cout << " - "; orders->Heat(); cout << " - "; orders->Inject(); cout << " - "; orders->Cool(); cout << " - "; orders->Separate(); cout << " - "; orders->Eject(); cout << ".\n";
		}

		void pretendPartsBinIsFull() {
			cout << "      Whenever "; bin->print(); cout << " parts bin was full...\n";
			bin->notify();
			if (AIR != stufferChoice) {
				cout << "        " << StufferDictionaryToStringBinFull[stufferChoice] << " stuffer triggered to fill "; bin->print(); cout << " parts bin.\n";
			}
		}

		void cleanUsedMold() {
			cleaner = adapter::CleanMoldProcess::makeObject(plasticChoice);
			cleaner->myWay(metal->getMoldMetalLC());
		}
	private:
		Plastic plasticChoice;
		int orderSize;
		int orderNumber;
		Packager packagerChoice;
		Stuffer stufferChoice;
		Mold moldChoice;
		int shapeVolume;
		Color colorChoice;
		MoldLoc moldLocChoice;
		Finish finishChoice;
		int uvInhibitorVolume;
		int antiBacterialVolume;
		int hydrophilicVolume;
		int microFibersVolume;
		string comment;
		string address;
		vector<Tags> userTags;
		vector<IngredientDetails> userIngredients;

		abstract_factory::InjectionLineOrder* injectionLine;
		chain_of_resp::MoldLocation* moldLocation;
		PartsBin* bin;
		InjectionMoldMachine* machine;
		MoldMetal* metal;
		ConveyerBelt* belt;
		factory_method::Packaging* packaging;
		adapter::CleanMoldProcess* cleaner;
		template_method::RunSteps * orders;
		strategy::RunTimeCalculation* math;
		decorator::Mixture* mixtureObject;
		decorator::Cavity* decoratorObject;

	};

	void process(map<string, string>& order) {
		// Fill in the namespaces above with your design pattern class hierarchies.
		// Call your order processing class from here <myProcess>->run(order);
		ProcessOrder processOrder(order);
	}

	pair<string, string> parse(string line) {
		char key[83];
		char val[83] = { 0 };

		sscanf(line.c_str(), "%s", key);

		char* equalSign = const_cast<char*>(strchr(line.c_str(), '='));
		string value = "";
		if (equalSign) {	// tags = sam i am
			char*	nextToken = equalSign += 2;
			while (nextToken) {
				sscanf(nextToken, "%s", val);
				value += val;
				nextToken = strchr(nextToken + 1, ' ');
				value += " ";
			}
			value.erase(value.length() - 1, 1);
		}

		return make_pair(key, value);
	}

	map<string, string> getCompleteOrder(FILE* orderFilePtr) {
		map<string, string> order;
		char line[80 + 3];
		pair<string, string>	keyValue;

		while (fgets(line, 80, orderFilePtr)) {
			cout << line;
			if (strstr(line, "endOfOrder")) {
				return(order);
			}
			else {
				keyValue = parse(line);
				string key = keyValue.first;
				if (strchr(key.c_str(), ':'))	// Skip left justified order number.
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

		for (;;) {	// Get and process each order.
			map<string, string> order = getCompleteOrder(orderFilePtr);
			if (order.size() == 0)
				break;
			process(order);
		}
	}

} // final_design

#endif /* FINAL_FINAL_DESIGN_H_ */

/*
 * finalDesign.h
 *
 *  Created on: <Dec 4, 2015>
 *      Author: <Kuiyu Li: kuiyu.li@intel.com>
 */

#ifndef FINAL_FINAL_DESIGN_H_
#define FINAL_FINAL_DESIGN_H_

#include <string>
#include <iostream>
#include <map>
#include <list>

using namespace std;

namespace final_design {

const bool on = true;	// Useful macro to turn dtor instrumentation on/off.
#define DTORF(x) if(on) { cout << x; }

namespace legacy_classes {	// Can't change these.

int cycleTime_sec(string metal, int volume_cc) {
	int heatCapacityAdjustment = 0;

	if(		metal == "Aluminum" || metal == "aluminum")	heatCapacityAdjustment = -2;
	else if(metal == "Steel" || metal == "steel")		heatCapacityAdjustment = +5;

	return 20 + heatCapacityAdjustment + 0.1*volume_cc;
}

const int setupAvg_min = 118;
const int teardownAvg_min = 54;
const float rejectRateLow_pcnt = 0.4; // was 0.2
const float rejectRateHigh_pcnt = 1.2; // was 1.1

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

bool isColor(string name) { return (name == "black" || name == "brown" || name == "red" || name == "orange" || name == "yellow" || name == "green"); }
bool isAddictive(string name) { return (name == "AntiBacterial" || name == "Hydrophilic" || name == "UVInhibiter"); }

namespace adapter {			// DP 2.
	class CAdapterClean {
	public:
		virtual ~CAdapterClean() { DTORF("~adapter::clean "); }
		virtual void clean() { cout << "base clean" << endl; }
		static CAdapterClean* makeObject(string type);
	};

	class CCleanAbs : public CAdapterClean {
		legacy_classes::CleanABS legacy_clean_abs;
	public:
		~CCleanAbs() { DTORF("~CCleanAbs "); }
		void clean() { legacy_clean_abs.clean(); }
	};

	class CCleanPoly : public CAdapterClean	{
		legacy_classes::CleanPolys legacy_clean_poly;
	public:
		~CCleanPoly() { DTORF("~CCleanPoly "); }
		void clean() { legacy_clean_poly.prepForReuse(); }
	};

	class CCleanPetPoly : public CAdapterClean	{
		legacy_classes::PETCleanup legacy_clean_pet;
	public:
		~CCleanPetPoly() { DTORF("~CCleanPetPoly "); }
		void clean() { legacy_clean_pet.purify(); }
	};

	class CCleanPetSteel : public CAdapterClean	{
		legacy_classes::PETCleanup legacy_clean_pet;
	public:
		~CCleanPetSteel() { DTORF("~CCleanPetSteel "); }
		void clean() { legacy_clean_pet.carbonize(); }
	};

	CAdapterClean* CAdapterClean::makeObject(string type) {
		if (type == "ABS")           return new CCleanAbs;
		if (type == "Polypropylene") return new CCleanPoly;
		if (type == "Polyethelene")  return new CCleanPoly;
		if (type == "PET")           return new CCleanPetSteel;
		return new CCleanAbs;
	}
// Seam point - add another interface.
}

namespace strategy {		// DP 1.
	class RuntimeEstimate 	{
	public:
		virtual ~RuntimeEstimate() { cout << "~strategy::RuntimeEstimate " << endl; }
		virtual int runTime(int orderSize, int cavities, string metal, int volume) = 0;
		static RuntimeEstimate* makeObject(int order_size);
	};

	class BackOfTheEnvelope : public RuntimeEstimate 	{
	public:
		~BackOfTheEnvelope() { cout << "~BackOfTheEnvelope "; }
		int runTime(int orderSize, int cavities, string metal, int volume) 
		{ 
			return (float)orderSize/cavities*(1/60.0); 
		}
	};

	class Calculation : public RuntimeEstimate 	{
	public:
		~Calculation() { cout << "~Calculation "; }
		int runTime(int orderSize, int cavities, string metal, int volume) 	{ 
			return (float)orderSize/cavities*legacy_classes::cycleTime_sec(metal, volume)/3600.0; 
		}
	};

	class Historical : public RuntimeEstimate {
	public:
		~Historical() { cout << "~Historical "; }
		int runTime(int orderSize, int cavities, string metal, int volume) {
			int cycletime = legacy_classes::cycleTime_sec(metal, volume);
			return (float)(legacy_classes::setupAvg_min + ((float)orderSize/cavities)*cycletime/60.0 + legacy_classes::teardownAvg_min)/60.0;
		}
	};

	RuntimeEstimate* RuntimeEstimate::makeObject(int order_size) {
		if (order_size <= 10000) return new BackOfTheEnvelope();
		if (order_size <= 25000) return new Calculation();
		if (order_size <= 50000) return new Historical();
		cout << "    <>Using most sophisticated estimation algorithm for orders greater than 100000.\n";
		return new Historical;
	}
// Seam point - add another algorithm.
}

namespace observer {		// DP 5.
	class BinObserver;
	class BinSubject {
	public:
		~BinSubject() { DTORF("~observer::BinSubject "); };
		BinSubject(string name) : m_name(name) {}
		list<BinObserver*> obsList;
		void attach(BinObserver *obs);
		void detach(BinObserver *obs);
		void notify(string Str);
		string m_name;
	};

	class BinObserver {
	public:
		BinObserver(string name, BinSubject* subject) : m_name(name), m_subject(subject) { m_subject->attach(this); }
		~BinObserver() { m_subject->detach(this); DTORF("~observer::BinObserver "); }
		virtual void update() = 0;
		BinSubject* m_subject;
		string m_name;
	};

	void BinSubject::attach(BinObserver *obs) {	obsList.push_back(obs); }
	void BinSubject::detach(BinObserver *obs) { obsList.remove(obs); }
	void BinSubject::notify(string Str) {
		cout << Str;
		for (list<BinObserver*>::iterator it = obsList.begin(); it != obsList.end(); it++)
			(*it)->update();
	}

	class MachineObs : public BinObserver {
	public:
		MachineObs(string name, BinSubject* subject) : BinObserver(name, subject) {}
		~MachineObs() { DTORF("~MachineObs "); }
		void update() { cout << "        " << m_name << " paused while " << m_subject->m_name << " parts bin was swapped." << endl; }
	};

	class BeltObs : public BinObserver {
	public:
		BeltObs(string name, BinSubject* subject) : BinObserver(name, subject) {}
		~BeltObs() { DTORF("~BeltObs "); }
		void update() { cout << "        " << m_name << " paused while " << m_subject->m_name << " parts bin was swapped." << endl; }
	};

	class PackObs : public BinObserver {
	public:
		PackObs(string name, BinSubject* subject) : BinObserver(name, subject) {}
		~PackObs() { DTORF("~PackObs "); }
		void update() {
			if (m_name != "")
				cout << "        " << m_name << " paused while " << m_subject->m_name << " parts bin was swapped." << endl; 
		}
	};
// Seam point - add another listener.
}

namespace abstract_factory {// DP 9.
	class Machine {
	public:
		virtual ~Machine() { DTORF("~abstract_factory::Machine "); }
		virtual string metaltype() = 0;
		virtual string metaltypeLowercase() = 0;
		virtual int numcavity() = 0;
		virtual string belttype() = 0;
		virtual string binSubject() = 0;
		virtual string machinename() = 0;

		void setup_injection_line() {
			cout << "    " << machinename() << " - " << metaltype() << "(" << numcavity() <<")" << " - " << belttype() << " - " << binSubject() << ".\n";
		}
	};

	class Machine_IJM110 : public Machine {
	public:
		~Machine_IJM110() { DTORF("~Machine_IJM110" ); }
		string metaltype() { return string("Aluminum"); } ;
		string metaltypeLowercase() { return string("aluminum"); } ;
		int numcavity() { return 1; };
		string belttype() { return string("Linear conveyer belt"); };
		string binSubject() { return string("CardboardBox"); };
		string machinename() { return string("IJM_110"); };
	};

	class Machine_IJM120 : public Machine {
	public:
		~Machine_IJM120() { DTORF("~Machine_IJM120" ); }
		string metaltype() { return string("Aluminum"); } ;
		string metaltypeLowercase() { return string("aluminum"); } ;
		int numcavity() { return 2; };
		string belttype() { return string("Y-Split conveyer belt"); };
		string binSubject() { return string("ShellBox"); };
		string machinename() { return string("IJM_120"); };
	};

	class Machine_IJM210 : public Machine {
	public:
		~Machine_IJM210() { DTORF("~Machine_IJM210" ); }
		string metaltype() { return string("Steel"); } ;
		string metaltypeLowercase() { return string("steel"); } ;
		int numcavity() { return 1; };
		string belttype() { return string("Linear conveyer belt"); };
		string binSubject() { return string("PalletBox");} ;
		string machinename() { return string("IJM_210"); };
	};

	class InjectionLine {
	public:
		virtual ~InjectionLine() { DTORF("abstract_factory::InjectionLine "); }
		virtual Machine* create_machine() = 0;
		static InjectionLine* makeObject(int orderSize);
	};

	class AbstractFactory_IJM110 : public InjectionLine {
	public:
		~AbstractFactory_IJM110() {}
		Machine* create_machine() { return new Machine_IJM110(); };
	};

	class AbstractFactory_IJM120 : public InjectionLine {
	public:
		~AbstractFactory_IJM120() {}
		Machine* create_machine() { return new Machine_IJM120(); };
	};

	class AbstractFactory_IJM210 : public InjectionLine {
	public:
		~AbstractFactory_IJM210() {}
		Machine* create_machine() { return new Machine_IJM210(); };
	};

	InjectionLine* InjectionLine::makeObject(int orderSize) {
		if (orderSize <= 10000) return new AbstractFactory_IJM110;
		if (orderSize <= 20000) return new AbstractFactory_IJM120;
		if (orderSize <= 50000) return new AbstractFactory_IJM210;
		return new AbstractFactory_IJM110;
	}
// Seam point - add another type 1.
// Seam point - add another type 2.
// Seam point - add another type 3.
// Seam point - add another ...
// Seam point - add another type N.
// Seam point - add another family.
}

namespace bridge {			// DP 8.
	class PlatForm {
	public:
		~PlatForm() { DTORF("~bridge::PlatForm "); }
		void process(string mold, int cavity, string finish, string metal, string steps) {
			cout << " with " << cavity << " cavities - " << "steps: " << steps << ".\n";
			useTool();
			cout << "to mill " << metal << " block into " << cavity << " " << mold << " " << (cavity > 1 ? "shapes" : "shape") << " with " << finish << " finish";
		}
		virtual void useTool() = 0;
		static PlatForm* makeObject(string type);	
	};

	class HighCarbon : public PlatForm {
	public:
		~HighCarbon() { DTORF("~HighCarbon "); }
		void useTool() { 
			cout << "      using HighCarbon tools (drill, cut, and high speed grind) "; 
		}
	};

	class Carbide : public PlatForm {
	public:
		~Carbide() { DTORF("~Carbide "); }
		void useTool() { 
			cout << "      using Carbide tools (high speed drill, cross cut, and layer grind) "; 
		}
	};

	class Diamondtipped : public PlatForm {
	public:
		~Diamondtipped() { DTORF("~Diamondtipped "); }
		void useTool() { 
			cout << "      using DiamondTipped tools (precision drill, oil cooled cut, and cartoid grind) "; 
		}
	};

	PlatForm* PlatForm::makeObject(string type) {
		if (type == "smooth") return new HighCarbon;
		if (type == "rippled") return new Carbide;
		if (type == "dimpled") return new Diamondtipped;
		return new HighCarbon;
	}
	// Seam Point - add another implementation.
	
	class Shape {
	public:
		Shape(PlatForm* plat) : m_plat(plat) {}
		~Shape() { DTORF("~bridge::Shape "); if (m_plat) delete m_plat; }
		virtual void mill(int cavity, string finish, string metal, string steps) = 0;
		PlatForm* m_plat;
		static Shape* makeObject(string type, PlatForm* plat);
	};

	class DuckAbs : public Shape {
	public:
		DuckAbs(PlatForm* plat) : Shape(plat) {};
		void mill(int cavity, string finish, string metal, string steps) { 
			m_plat->process("duck", cavity, finish, metal, steps); 
		}
	};

	class CarAbs : public Shape {
	public:
		CarAbs(PlatForm* plat) : Shape(plat) {};
		void mill(int cavity, string finish, string metal, string steps) { 
			m_plat->process("car", cavity, finish, metal, steps); 
		}
	};

	class HeroAbs : public Shape {
	public:
		HeroAbs(PlatForm* plat) : Shape(plat) {};
		void mill(int cavity, string finish, string metal, string steps) { 
			m_plat->process("hero", cavity, finish, metal, steps); 
		}
	};

	Shape* Shape::makeObject(string Shapetype, PlatForm* plat) {
		if (Shapetype == "duck") return new DuckAbs(plat);
		if (Shapetype == "car")  return new CarAbs(plat);
		if (Shapetype == "hero") return new HeroAbs(plat);
		return new DuckAbs(plat);
	}
// Seam Point - add another abstraction.
}

namespace chain_of_resp {	// DP 7.
	class GetMold {
	protected:
		GetMold* m_succesor;
	public:
		GetMold() : m_succesor(0) {}
		~GetMold() { DTORF(" ~chain_of_resp::GetMold ");  delete m_succesor; }
		void setSuccessor(GetMold* next) { m_succesor = next; }
		virtual void delegate(string &location, string mold, string &finish) {
			cout << "    " << "<>Can't find place |" << location << "| to get |" << mold << "| mold from with |" << finish << "| finish, ";
			if (finish == "")  finish = "smooth";
			location = "inventory";
			cout << "defaulting to " << finish << " " << mold << " from " << location;
		}
		static GetMold* makeObject(string t);
	};

	class Inventory : public GetMold {
	public:
		void delegate(string &location, string mold, string &finish) {
			if (location == "inventory" || location == "Inventory")
				cout << "    Pull " << mold << " mold from " << location;
			else
				m_succesor->delegate(location, mold, finish);
		}
	};

	class SisterCompany : public GetMold {
	public:
		void delegate(string &location, string mold, string &finish) {
			if (location == "SisterCompany")
				cout << "    Borrow " << mold << " mold from " << location;
			else
				m_succesor->delegate(location, mold, finish);
		}
	};

	class Mill : public GetMold {
	public:
		void delegate(string &location, string mold, string &finish) {
			if (location == "Mill" || location == "mill")
				cout << "    Create " << mold << " mold from " << location;
			else
				m_succesor->delegate(location, mold, finish);
		}
	};

	GetMold* GetMold::makeObject(string t) {
		if (t == "inventory") return new Inventory();
		if (t == "SisterCompany") return new SisterCompany();
		if (t == "mill") return new Mill();
		return new Inventory();
	}

	GetMold* setUpChain() {
		const size_t chain_size = 3;
		string chain[chain_size] = { "inventory", "SisterCompany", "mill" };
		GetMold* responder = GetMold::makeObject(chain[0]);
		GetMold* current = responder;
		for (size_t i = 1; i < chain_size; i++) {
			GetMold* next = GetMold::makeObject(chain[i]);
			current->setSuccessor(next);
			current = next;
		}
		current->setSuccessor(new GetMold);
		return responder;
	}
// Seam points - add another responder.
}

namespace decorator {		// DP 6.
	class Recipe {
	public:
		~Recipe() { DTORF("~decorator::Recipe "); };
		virtual string desp() = 0;
		virtual int recipe() = 0;
	};

	class RecipeDecor : public Recipe {
	public:
		RecipeDecor() : m_volume(0) {}
		RecipeDecor(Recipe* decor, int volume = 0) : m_decor(decor), m_volume(volume) {};
		~RecipeDecor() { DTORF("~RecipeDecor "); delete m_decor; }
		string desp() { return ""; }
		int recipe() { return 0; }
		int m_volume;
		Recipe* m_decor;
		static Recipe* makeObject(Recipe* recp, string type, string name, int volume = 0);
	};

	class AbsDec : public RecipeDecor {
	public:
		AbsDec(Recipe* decor, int volume) : RecipeDecor(decor, volume) {}
		string desp() { return "ABS(" + to_string(m_volume) + ") "+ m_decor->desp(); }
		int recipe() { return m_decor->recipe() + m_volume; }
	};

	class PolypropyleneDec : public RecipeDecor {
	public:
		PolypropyleneDec(Recipe* decor, int volume) : RecipeDecor(decor, volume) {}
		string desp() { return "Polypropylene(" + to_string(m_volume) + ") " + m_decor->desp(); }
		int recipe() { return m_decor->recipe() + m_volume; }
	};

	class PolyetheleneDec : public RecipeDecor {
	public:
		PolyetheleneDec(Recipe* decor, int volume) : RecipeDecor(decor, volume) {}
		string desp() { return "Polyethelene(" + to_string(m_volume) + ") " + m_decor->desp(); }
		int recipe() { return m_decor->recipe() + m_volume; }
	};

	class PetDec : public RecipeDecor {
	public:
		PetDec(Recipe* decor, int volume) : RecipeDecor(decor, volume) {}
		string desp() { return "PET(" + to_string(m_volume) + ") " + m_decor->desp(); }
		int recipe() { return m_decor->recipe() + m_volume; }
	};

	class ColorDec : public RecipeDecor {
	public:
		ColorDec(Recipe* decor, int volume, string color = "") : RecipeDecor(decor, volume), m_color(color) {}
		string desp() { return m_color + "(" + std::to_string(m_volume) + ") " + m_decor->desp(); }
		int recipe() { return m_decor->recipe() + m_volume; }
		string m_color;
	};

	class UVDec : public RecipeDecor {
	public:
		UVDec(Recipe* decor, int volume) : RecipeDecor(decor, volume) {}
		string desp() { return "UVInhibiter(" + std::to_string(m_volume) + ") " + m_decor->desp(); }
		int recipe() { return m_decor->recipe() + m_volume; }
	};

	class AntiBactDec : public RecipeDecor {
	public:
		AntiBactDec(Recipe* decor, int volume) : RecipeDecor(decor, volume) {}
		string desp() { return "AntiBacterial(" + std::to_string(m_volume) + ") " + m_decor->desp(); }
		int recipe() { return m_decor->recipe() + m_volume; }
	};

	class HydrophilicDec : public RecipeDecor {
	public:
		HydrophilicDec(Recipe* decor, int volume) : RecipeDecor(decor, volume) {}
		string desp() { return "Hydrophilic(" + std::to_string(m_volume) + ") " + m_decor->desp(); }
		int recipe() { return m_decor->recipe() + m_volume; }
	};

	Recipe* RecipeDecor::makeObject(Recipe* recp, string type, string name, int volume) {
		if (type == "plastic" && name == "ABS") return new AbsDec(recp, volume);
		if (type == "plastic" && name == "Polyethelene") return new PolyetheleneDec(recp, volume);
		if (type == "plastic" && name == "Polypropylene") return new PolypropyleneDec(recp, volume);
		if (type == "plastic" && name == "PET") return new PetDec(recp, volume);
		if (type == "color"   && isColor(name)) return new ColorDec(recp, volume, name);
		if (type == "UVInhibiter") return new UVDec(recp, volume);
		if (type == "AntiBacterial") return new AntiBactDec(recp, volume);
		if (type == "Hydrophilic") return new HydrophilicDec(recp, volume);
		return recp;
	}
// Seam point - add another option.
}

namespace factory_method {	// DP 4.
	class Tag {
	public:
		virtual ~Tag() { /*DTORF("~factory_method::Tag ");*/ }
		virtual string name() = 0;
		virtual int width() = 0;
		static Tag* makeObject(string type);
	};

	class TagModelNumber : public Tag {
	public:
		~TagModelNumber() { /*DTORF("~TagModelNumber ");*/ }
		string name() { return "ModelNumber"; }
		int width() { return 2; }
	};

	class TagCountry : public Tag {
	public:
		~TagCountry() { /*DTORF("~TagCountry ");*/ }
		string name() { return "Country"; }
		int width() { return 2; }
	};

	class TagDate : public Tag {
	public:
		~TagDate() { /*DTORF("~TagDate ");*/ }
		string name() { return "Date"; }
		int width() { return 2; }
	};

	Tag* Tag::makeObject(string type) {
		if (type == "ModelNumber") return new TagModelNumber;
		if (type == "Country") return new TagCountry;
		if (type == "Date") return new TagDate;
		return new TagModelNumber;
	}

	class Mold {
	public:
		virtual ~Mold() { DTORF("~factory_method::Mold "); }
		virtual string steps() = 0;
		virtual string name() = 0;
		virtual int volume() = 0;
		static Mold* makeObject(string Shapetype);
	};

	class MoldDuck : public Mold {
	public:
		~MoldDuck() { DTORF("~Duck "); }
		string steps() { return "D3C2D2G4"; }
		string name() { return "Duck"; };
		int volume() { return 35; }
	};

	class MoldCar : public Mold {
	public:
		~MoldCar() { DTORF("~Car "); }
		string steps() { return "D2C8D1G3"; }
		string name() { return "Car"; };
		int volume() { return 40; }
	};

	class MoldHero : public Mold {
	public:
		~MoldHero() { DTORF("~Hero "); }
		string steps() { return "D1C1D1G7"; }
		string name() { return "Hero"; };
		int volume() { return 50; }
	};

	Mold* Mold::makeObject(string Shapetype) {
		if (Shapetype == "duck") return new MoldDuck();
		if (Shapetype == "car")  return new MoldCar();
		if (Shapetype == "hero") return new MoldHero();
		return new MoldDuck();
	}

	class Packager {
	public:
		virtual ~Packager() { DTORF("~factory_method::Packager "); }
		virtual void packing() { cout << ""; };
		virtual string name() { return ""; }
		static Packager* makeObject(string packtype);
	};

	class CPakBulk : public Packager {
	public:
		~CPakBulk() { DTORF("~CPakBulk "); }
		void packing() { cout << ""; }
		string name() { return ""; }
	};

	class CPakShrink : public Packager {
	public:
		~CPakShrink() { DTORF("~CPakShrink "); }
		void packing() { cout << " with ShrinkWrap packager"; }
		string name() { return "Shrink wrap packager"; }
	};

	class CPakHard : public Packager {
	public:
		~CPakHard() { DTORF("~CPakHard "); }
		void packing() { cout << " with HardPack packager"; }
		string name() { return "Hard pack packager"; }
	};

	Packager* Packager::makeObject(string packtype) {
		if (packtype == "Bulk") return new CPakBulk;
		if (packtype == "ShrinkWrap") return new CPakShrink;
		if (packtype == "HardPack") return new CPakHard;
		return new CPakBulk;
	}
// Seam point - add another class.
}

namespace template_method {	// DP 3.
	class Injection {
	public:
		~Injection() { DTORF("~template_method::Injection "); }
		void Close() { cout << "Close - "; }
		void Mix() { cout << "mix - "; }
		virtual void Heat() {};
		virtual void inject() {};
		virtual void cool() {};
		void separate() { cout << "separate - "; }
		virtual void eject() {}
		void run() { Close(); Mix(); Heat(); inject(); cool(); separate(); eject(); }
		static Injection* makeObject(string plastictype);
	};

	class InjectionABS : public Injection {
		~InjectionABS() { DTORF("~InjectionABS "); }
		void Heat() { cout << "heat to 440 - "; };
		void inject() { cout << "inject at 125 PSI - "; };
		void cool() { cout << "cool to 360 - "; };
		void eject() { cout << "progressive eject"; };
	};

	class InjectionPoly : public Injection {
		~InjectionPoly() { DTORF("~InjectionPoly "); }
		void Heat() { cout << "heat to 350 - "; };
		void inject() { cout << "inject at 90 PSI - "; };
		void cool() { cout << "cool to 290 - "; };
		void eject() { cout << "smooth eject"; };
	};

	class InjectionPET : public Injection {
		~InjectionPET() { DTORF("~InjectionPET "); }
		void Heat() { cout << "heat to 404 - "; };
		void inject() { cout << "inject at 110 PSI - "; };
		void cool() { cout << "cool to 340 - "; };
		void eject() { cout << "smooth eject"; };
	};

	Injection* Injection::makeObject(string plasticstype) {
		if (plasticstype == "ABS") return new InjectionABS;
		if (plasticstype == "Polypropylene") return new InjectionPoly;
		if (plasticstype == "Polyethelene") return new InjectionPoly;
		if (plasticstype == "PET") return new InjectionPET;
		return new InjectionABS;
	}

// 	class ProcessOrder_template {
// 	public:
// 		ProcessOrder_template(map<string,string>& _order) : order(_order) {}
// 		~ProcessOrder_template() { DTORF("~template_method::ProcessOrder_template "); }
// 		void process_this_order() {}
// 		static ProcessOrder_template* makeObject(map<string,string>& order);
// 		map<string,string>& order;
// 	};
// 	class ProcessOrder : public ProcessOrder_template {
// 	public:
// 		ProcessOrder(map<string,string>& order) : ProcessOrder_template(order) {}
// 		~ProcessOrder() { DTORF("~ProcessOrder "); }
// 	};
// 	ProcessOrder_template* ProcessOrder_template::makeObject(map<string,string>& order) {
// 		return new ProcessOrder(order);
// 	}

// Seam point - add another polymorphic step.
// Seam point - add another constant step.
// Seam point - convert a constant step into a polymorphic step.
}

int processTags(string tag, string &final_tag) {
	int final_tag_length = 0;
	std::string s = tag;
	std::string delimiter = " ";

	size_t pos = 0;
	std::string token;
	bool last_tag = false;

	while (!last_tag) {
		pos = s.find(delimiter);
		if (pos == std::string::npos)
			last_tag = true;
		if (last_tag)
			pos = s.length();
		if (pos == 0)
			break;
		token = s.substr(0, pos);
		if (token != "ModelNumber" && token != "Country" && token != "Date")
			cout << "    Ignoring unknown tag " << token << ".\n";
		else {
			factory_method::Tag* thisTag = factory_method::Tag::makeObject(token);
			final_tag += (token + (last_tag ? "":" "));
			final_tag_length += thisTag->width();
			delete thisTag;
		}
		if (last_tag)
			break;
		s.erase(0, pos + delimiter.length());
	}
	return final_tag_length;
}

void process(map<string,string>& order) {
	// Fill in the namespaces above with your design pattern class hierarchies.
	// Call your order processing class from here <myProcess>->run(order);

	if (order["plastic"] == "")  legacy_classes::defaulting(order, "plastic", "ABS");
	if (order["size"] == ""){
		cout << "  <>No size specified, defaulting to 100.\n";
		order["size"] = "100";
	}
	if (order["packager"] == "") legacy_classes::defaulting(order, "packager", "None");
	int order_size = std::stoi(order["size"]);
	if (order_size > 50000) {
		cout << "  " << "<>Size exceeds mold lifetime |100000| defaulting to MediumOrder of 50000.\n";
		order_size = 50000;
	}

	factory_method::Packager * PakingMethod = factory_method::Packager::makeObject(order["packager"]);
	cout << "  " << "Setup injection line for " << order_size << " order";
	PakingMethod->packing();
	cout << ":\n";

	// Abstract Factory: Setup Injection Line
	abstract_factory::InjectionLine* factory = abstract_factory::InjectionLine::makeObject(order_size);
	abstract_factory::Machine* machine = factory->create_machine();
	machine->setup_injection_line();

	// Process order
	cout << "  Process order:\n";
	if (order["mold"] == "") {
		cout << "  ";
		legacy_classes::defaulting(order, "mold", "duck");
	}
	factory_method::Mold* mold = factory_method::Mold::makeObject(order["mold"]);

	// Chain: pull/borrow/create mold from
	chain_of_resp::GetMold* chain = chain_of_resp::setUpChain();
	chain->delegate(order["moldLoc"], order["mold"], order["finish"]);

	// Bridge: create mold
	bridge::PlatForm* platform = bridge::PlatForm::makeObject(order["finish"]);
	bridge::Shape* shape = bridge::Shape::makeObject(order["mold"], platform);
	if (order["moldLoc"] == "mill")
		shape->mill(machine->numcavity(), order["finish"], machine->metaltypeLowercase(), mold->steps());
	cout << ".\n";

	// Insert tags
	const int total_tag_width = 20;
	string tag_str("");
	int final_tag_length = processTags(order["tags"], tag_str);
	cout << "    " << "Insert tags [" << tag_str << "] of width " << final_tag_length << "/" << total_tag_width << " mm, blank tag is " << total_tag_width - final_tag_length << " mm.\n";

	// Load plastic
	cout << "    " << "Load plastic bin with " << order["plastic"];
	if (order.find("color") != order.end()) 
		cout << " and color bin with " << order["color"];
	cout << ".\n";

	// Decor: Recipe
	decorator::Recipe * recipe = new decorator::RecipeDecor;
	int plasctic_volume = mold->volume();
	for (map<string, string>::iterator it = order.begin(); it != order.end(); it++) {
		if (isAddictive(it->first)) {
			int addictive_volume = std::stoi(it->second);
			recipe = decorator::RecipeDecor::makeObject(recipe, it->first, it->second, addictive_volume);
			plasctic_volume -= addictive_volume;
		}
		else if (it->first == "color" && isColor(it->second)) {
			int color_volume = 0.1*mold->volume();
			recipe = decorator::RecipeDecor::makeObject(recipe, it->first, it->second, color_volume);
			plasctic_volume -= color_volume;
		}
	}
	recipe = decorator::RecipeDecor::makeObject(recipe, "plastic", order["plastic"], plasctic_volume);
	cout << "      " << "Recipe: " << recipe->desp() << "= " << recipe->recipe() << " cc.\n";
	cout << "      " << "Volume: " << order["mold"] << "(" << mold->volume() <<  ") * " << machine->numcavity() << ((machine->numcavity() == 1) ? " cavity" : " cavities") << " = " << mold->volume() * machine->numcavity() << " cc.\n";

	// Strategy: runtime
	strategy::RuntimeEstimate* runtime = strategy::RuntimeEstimate::makeObject(order_size);
	int timeestimate = runtime->runTime(order_size, machine->numcavity(), machine->metaltype(), mold->volume());
	cout << "    Cycle " << machine->machinename() << " for " << order["plastic"] << " " << order_size / machine->numcavity() << " times, estimated run time = " << timeestimate << (timeestimate > 1 ? " hours":" hour") << ".\n";

	// Template: Cycle IJM: Inject: close - mix - ...
	template_method::Injection* inject = template_method::Injection::makeObject(order["plastic"]);
	cout << "      ";
	inject->run();
	cout << ".\n";

	// Observer: Whenever bin was full...
	observer::BinSubject binsubject(machine->binSubject());
	observer::MachineObs machineObs(machine->machinename(), &binsubject);
	observer::BeltObs    beltObs(machine->belttype(), &binsubject);
	observer::PackObs    pakObs(PakingMethod->name(), &binsubject);
	binsubject.notify("      Whenever " + machine->binSubject() + " parts bin was full...\n");
	binsubject.detach(&machineObs);
	binsubject.detach(&beltObs);
	binsubject.detach(&pakObs);

	// Adapter: clean
	adapter::CAdapterClean* myclean = adapter::CAdapterClean::makeObject(order["plastic"]);
	myclean->clean();

	// destructors
	delete PakingMethod;
	delete machine;
	delete factory;
	delete chain;
	delete shape;
	delete recipe;
	delete runtime;
	delete inject;
	delete myclean;
}

pair<string,string> parse(string line) {
	char key[83];
	char val[83] = {0};

	sscanf(line.c_str(), "%s", key);

	char* equalSign = (char*)strchr(line.c_str(), '=');
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
	cout << "<<< final design >>>\n";

	FILE* orderFilePtr = fopen(orderFile.c_str(), "r");

	for(;;) {	// Get and process each order.
		map<string,string> order = getCompleteOrder(orderFilePtr);
		if(order.size() == 0)
			break;
		process(order);
		cout << "\n";
	}
}

} // final_design

#endif /* FINAL_FINAL_DESIGN_H_ */

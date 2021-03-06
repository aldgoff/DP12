/*
 * finalDesign.h
 *
 *  Created on: <MMDDCCYY>
 *      Author: <student name>
 */

#ifndef FINAL_FINAL_DESIGN_H_
#define FINAL_FINAL_DESIGN_H_
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <vector>


using namespace std;

std::ofstream fos;
std::ofstream fos_empty;

namespace final_design {

const bool on = false;	// Useful macro to turn dtor instrumentation on/off.
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
const float rejectRateLow_pcnt = 0.2;
const float rejectRateHigh_pcnt = 1.1;

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

namespace adapter {			// DP 2.

// Seam point - add another interface.
	using namespace legacy_classes;
	
	class CleanPlastic_A_2
	{
	public:
		string sPlasticName;
		virtual void clean(){}
		virtual ~CleanPlastic_A_2(){fos_empty<<"CleanPlastic_A_2 "<<endl;}
		static CleanPlastic_A_2 *create_PlasticCleanObj(string &sPlasticName, string &sMetal);
	};

	class Synthetics_Clean: public CleanPlastic_A_2
	{
	public:
		Synthetics_Clean(){sPlasticName = "Synthetics";}
		~Synthetics_Clean(){fos_empty<<"~adapter::Synthetics_Clean "<<endl;}
		void clean()
		{
			fos<<"    Clean synthetic mold: ozone wash."<<endl;
		}
	};

	class PET_Clean: public CleanPlastic_A_2
	{
		PETCleanup oPETCleanup;
		string sMetal;
	public:
		PET_Clean(string &in_Metal) : sMetal(in_Metal){sPlasticName = "PET";};
		~PET_Clean(){fos_empty<<"~adapter::PET_Clean "<<endl;}
		void clean(){
			if(!sMetal.compare("aluminum"))
				oPETCleanup.purify();
			else if(!sMetal.compare("steel"))
				oPETCleanup.carbonize();
		}
	};

	class Poly_Clean: public CleanPlastic_A_2
	{
		CleanPolys oCleanPolys;
	public:
		Poly_Clean(){sPlasticName = "Poly";}
		~Poly_Clean(){fos_empty<<"~adapter::Poly_Clean "<<endl;}
		void clean()
		{
			oCleanPolys.prepForReuse();
		}
	};

	class ABS_Clean: public CleanPlastic_A_2
	{
		CleanABS oCleanABS;
	public:
		ABS_Clean(){sPlasticName = "ABS";}
		~ABS_Clean(){fos_empty<<"~adapter::ABS_Clean "<<endl;}
		void clean()
		{
			oCleanABS.clean();
		}
	};

	CleanPlastic_A_2 * CleanPlastic_A_2::create_PlasticCleanObj(string &sPlastic, string &sMetal)
	{
		if(sPlastic.compare("ABS") == 0)
		{
			return new adapter::ABS_Clean;
		}
		else if(sPlastic.compare("PET") == 0)
		{
			return new adapter::PET_Clean(sMetal);
		}
		else if(sPlastic.compare("Polypropylene") == 0 || sPlastic.compare("Polyethelene") == 0)
		{
			return new adapter::Poly_Clean;
		}
		else if(sPlastic.compare("Styrene") == 0 || sPlastic.compare("Nylon66") == 0)
		{
			return new adapter::Synthetics_Clean;
		}
		else
		{
			return new adapter::ABS_Clean;
		}
	}
}
namespace strategy {		// DP 1.

// Seam point - add another algorithm.
	class Strategy_1{
	public:
		virtual void Inject()
		{
			fos <<"      Close - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject."<<endl;
		}
		virtual ~Strategy_1(){fos_empty <<" ~Strategy_1"<<endl;};
		static Strategy_1 *Create_InjectionObj(string &sPlastic);
	};

	class ABS: public Strategy_1
	{
	public:
		~ABS(){fos_empty<<"~ABS "<<endl;}
		void Inject()
		{
			fos <<"      Close - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject."<<endl;
		}
	};

	class Poly: public Strategy_1
	{
	public:
		~Poly(){fos_empty<<"~Poly "<<endl;}
		void Inject()
		{
			fos <<"      Close - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject."<<endl;
		}
	};
	class PET: public Strategy_1
	{
	public:
		~PET(){fos_empty<<"~PET "<<endl;}
		void Inject()
		{
			fos <<"      Close - heat to 404 - inject at 110 PSI - cool to 340 - separate - smooth eject."<<endl;
		}
	};
	class Synthetics: public Strategy_1
	{
	public:
		~Synthetics(){fos_empty<<"~Synthetics "<<endl;}
		void Inject()
		{
			fos <<"      Close - heat to 480 - inject at 150 PSI - cool to 390 - separate - shock eject."<<endl;
		}
	};

	Strategy_1 * Strategy_1::Create_InjectionObj(string &sPlasticName)
	{
		if(!sPlasticName.compare("ABS"))
		{
			return new strategy::ABS;
		}
		else if(!sPlasticName.compare("Poly"))
		{
			return new strategy::Poly;
		}
		else if(!sPlasticName.compare("PET"))
		{
			return new strategy::PET;
		}
		else if(!sPlasticName.compare("Synthetics"))
		{
			return new strategy::Synthetics;
		}
		else
		{
			return new strategy::ABS;
		}
	}
}
namespace observer {		// DP 5.
	class Subject_7;
	class Observer_7
	{
	public:
		virtual ~Observer_7(){fos_empty<<"Observer_7"<<endl;}
		virtual void update(Subject_7 *pSubject){};
	};

	class Stuffer : public Observer_7
	{
	public:
		string sStufferName;
		virtual ~Stuffer(){fos_empty<<"~Stuffer "<<endl;}
		virtual void update(observer::Subject_7 *pSubject);
		static Stuffer *create_stuffer(string &sStufferName);
	};
	class Air : public Stuffer
	{
	public:
		Air()
		{
			sStufferName = "air";
		}
		virtual ~Air(){fos_empty<<"~Air "<<endl;}
		void update(observer::Subject_7 *pSubject)
		{
			Stuffer::update(pSubject);
		}
	};
	class Popcorn : public Stuffer
	{
	public:
		Popcorn(){sStufferName = "styrene popcorn";}
		virtual ~Popcorn(){fos_empty<<"~Popcorn "<<endl;}
		void update(observer::Subject_7 *pSubject)
		{
			Stuffer::update(pSubject);
		}
	};
	class BubbleWrap : public Stuffer
	{
	public:
		BubbleWrap(){sStufferName = "bubble wrap";}
		virtual ~BubbleWrap(){fos_empty<<"~BubbleWrap "<<endl;}
		void update(observer::Subject_7 *pSubject)
		{
			Stuffer::update(pSubject);
		}
	};
	class Foam : public Stuffer
	{
	public:
		Foam(){sStufferName = "expanding foam";}
		virtual ~Foam(){fos_empty<<"~Foam "<<endl;}
		void update(observer::Subject_7 *pSubject)
		{
			Stuffer::update(pSubject);
		}
	};
	Stuffer * Stuffer::create_stuffer(string &sStufferName)
	{
		if(sStufferName.compare("Popcorn") == 0)
		{
			return new observer::Popcorn;
		}
		else if (sStufferName.compare("BubbleWrap") == 0)
		{
			return new observer::BubbleWrap;
		}
		else if (sStufferName.compare("Foam") == 0)
		{
			return new observer::Foam;
		}
		else if (sStufferName.compare("Air") == 0)
		{
			return new observer::Air;
		}
		else
		{
			fos<<"  <>Unknown stuffer || defaulting to Air stuffer."<<endl;
			return new observer::Air;
		}
	}


	class Subject_7
	{
		list<Observer_7 *> observers;
	public: 
		string sPackageBinName;

	public:
		~Subject_7(){fos_empty<<"~Subject_7 Observers left to process (should be zero) = 0."<<endl;}

		void Attach(Observer_7 *obj)
		{
			observers.push_back(obj);
		};
		void Detach(Observer_7 *obj)
		{
			observers.remove(obj);
		};
		bool getState()
		{
			return true;
		};
		void notify()
		{
			fos<<"      "<<sPackageBinName.c_str()<<" package bin full..."<<endl;
			list<Observer_7 *>::iterator iter;
			for(iter = observers.begin(); iter != observers.end(); ++iter)
			{
				(*iter)->update(this);
			}
		}
	};

	void Stuffer::update(observer::Subject_7 *pSubject)
	{
		fos<<"        Stuffer filling "<<pSubject->sPackageBinName.c_str()<<" package bin with "<<sStufferName.c_str()<<" stuffing."<<endl;
	}
}
namespace abstract_factory {// DP 9.
	class observer::Subject_7;
	
	// Seam point - add another family.
	class IJM_AF : public observer::Observer_7
	{
	public:
		int CavityNum;
		virtual ~IJM_AF(){fos_empty<<"~IJM_AF "<<endl;}
		void update(observer::Subject_7 *pSubject)
		{
			fos<<"        IJM pausing while "<<pSubject->sPackageBinName.c_str()<<" package bin is swapped."<<endl;
		}
	};
	class IJM_110 : public IJM_AF
	{
	public:
		IJM_110(){
			CavityNum = 1; 
			fos<<"    IJM_110 - ";
		}
		~IJM_110(){fos_empty<<"~IJM_110 "<<endl;}
	};
	class IJM_120 : public IJM_AF
	{
	public:
		IJM_120(){
			CavityNum = 2; 
			fos<<"    IJM_120 - ";
		}
		~IJM_120(){fos_empty<<"~IJM_120 "<<endl;}
	};
	class IJM_210 : public IJM_AF
	{
	public:
		IJM_210(){
			CavityNum = 1;
			fos<<"    IJM_210 - ";
		}
		~IJM_210(){fos_empty<<"~IJM_210 "<<endl;}
	};
	class IJM_140 : public IJM_AF
	{
	public:
		IJM_140(){
			CavityNum = 4;
			fos<<"    IJM_140 - ";
		}
		~IJM_140(){fos_empty<<"~IJM_140 "<<endl;}
	};
	class IJM_220 : public IJM_AF
	{
	public:
		IJM_220(){
			CavityNum = 2;
			fos<<"    IJM_220 - ";
		}
		~IJM_220(){fos_empty<<"~IJM_220 "<<endl;}
	};
	class IJM_240 : public IJM_AF
	{
	public:
		IJM_240(){
			CavityNum = 4;
			fos<<"    IJM_240 - ";
		}
		~IJM_240(){fos_empty<<"~IJM_240 "<<endl;}
	};

	class Mold_AF
	{
	public:
		string sMetal;
		virtual ~Mold_AF(){fos_empty<<"~Mold_AF"<<endl;}
	};
	class Aluminum : public Mold_AF
	{
	public:
		Aluminum(){sMetal = "aluminum";}
		~Aluminum(){fos_empty<<"~Aluminum "<<endl;}
	};
	class Steel : public Mold_AF
	{
	public:
		Steel(){sMetal = "steel";}
		~Steel(){fos_empty<<"~Steel "<<endl;}
	};

	class ConveyerBelt_AF : public observer::Observer_7
	{
	public:
		virtual ~ConveyerBelt_AF(){fos_empty<<"~ConveyerBelt_AF "<<endl;}
		void update(observer::Subject_7 *pSubject)
		{
			fos<<"        Conveyer Belt pausing while "<<pSubject->sPackageBinName.c_str()<<" package bin is swapped."<<endl;
		}
	};
	class LinearBelt : public ConveyerBelt_AF
	{
	public:
		LinearBelt(){fos<<"Linear conveyer belt - ";}
		~LinearBelt(){fos_empty<<"~LinearBelt "<<endl;}
	};
	class YSplitBelt : public ConveyerBelt_AF
	{
	public:
		YSplitBelt(){fos<<"Y-Split conveyer belt - ";}
		~YSplitBelt(){fos_empty<<"~YSplitBelt "<<endl;}
	};
	class VlevelBelt : public ConveyerBelt_AF
	{
	public:
		VlevelBelt(){fos<<"V-Level conveyer belt - ";}
		~VlevelBelt(){fos_empty<<"~VlevelBelt "<<endl;}
	};

	class PackageBin_AF : public observer::Subject_7
	{
	public:
		virtual ~PackageBin_AF(){fos_empty<<"~PackageBin_AF "<<endl;}
	};
	class CardboardBox : public PackageBin_AF
	{
	public:
		CardboardBox() 
		{
			sPackageBinName = "CardboardBox";
			fos<<"CardboardBox"<<endl;
		}
		~CardboardBox(){fos_empty<<"~CardboardBox "<<endl;}
	};
	class PallotBox : public PackageBin_AF
	{
	public:
		PallotBox()
		{
			sPackageBinName = "PallotBox";
			fos<<"PalletBox"<<endl;
		}
		~PallotBox(){fos_empty<<"~PalletBox "<<endl;}
	};
	class Crate : public PackageBin_AF
	{
	public:
		Crate()
		{
			sPackageBinName = "Crate";
			fos<<"Crate"<<endl;
		}
		~Crate(){fos_empty<<"~Crate "<<endl;}
	};

	class Setup_AF_10
	{
	public:
		int m_orderSize;

		virtual ~Setup_AF_10(){fos_empty<<"~Setup_AF_10"<<endl;}

		virtual IJM_AF *createIJM()=0;
		virtual Mold_AF *createMold()=0;
		virtual ConveyerBelt_AF *createConveyerBelt() = 0;
		virtual PackageBin_AF *createPackbin()=0;

		static Setup_AF_10 * createOrder(int &orderSize);
	};

	class PilotOrder : public Setup_AF_10
	{
	public:
		PilotOrder(int orderSize){m_orderSize = orderSize;}
		~PilotOrder(){fos_empty<<"~PilotOrder"<<endl;}
		IJM_AF *createIJM(){return new IJM_110;}
		Mold_AF *createMold(){fos<<"Aluminum(1) - "; return new Aluminum;}
		ConveyerBelt_AF *createConveyerBelt(){return new LinearBelt;}
		PackageBin_AF *createPackbin(){return new CardboardBox;}
	};
	class SmallOrder : public Setup_AF_10
	{
	public:
		SmallOrder(int orderSize){m_orderSize = orderSize;}
		~SmallOrder(){fos_empty<<"~SmallOrder"<<endl;}
		IJM_AF *createIJM(){return new IJM_120;}
		Mold_AF *createMold(){fos<<"Aluminum(2) - "; return new Aluminum;}
		ConveyerBelt_AF *createConveyerBelt(){return new YSplitBelt;}
		PackageBin_AF *createPackbin(){return new CardboardBox;};
	};
	class FastOrder : public Setup_AF_10
	{
	public:
		FastOrder(int orderSize){m_orderSize = orderSize;}
		~FastOrder(){fos_empty<<"~FastOrder"<<endl;}
		IJM_AF *createIJM(){return new IJM_140;}
		Mold_AF *createMold(){fos<<"Aluminum(4) - "; return new Aluminum;}
		ConveyerBelt_AF *createConveyerBelt(){return new VlevelBelt;}
		PackageBin_AF *createPackbin(){return new PallotBox;}
	};
	class MediumOrder : public Setup_AF_10
	{
	public:
		MediumOrder(int orderSize){m_orderSize = orderSize;}
		~MediumOrder(){fos_empty<<"~MediumOrder"<<endl;}
		IJM_AF *createIJM(){return new IJM_210;}
		Mold_AF *createMold(){fos<<"Steel(1) - "; return new Steel;}
		ConveyerBelt_AF *createConveyerBelt(){return new LinearBelt;}
		PackageBin_AF *createPackbin(){return new PallotBox;}
	};
	class LargeOrder : public Setup_AF_10
	{
	public:
		LargeOrder(int orderSize){m_orderSize = orderSize;}
		~LargeOrder(){fos_empty<<"~LargeOrder"<<endl;}
		IJM_AF *createIJM(){return new IJM_220;}
		Mold_AF *createMold(){fos<<"Steel(2) - "; return new Steel;}
		ConveyerBelt_AF *createConveyerBelt(){return new YSplitBelt;}
		PackageBin_AF *createPackbin(){return new Crate;}
	};
	class HugeOrder : public Setup_AF_10
	{
	public:
		HugeOrder(int orderSize){m_orderSize = orderSize;}
		~HugeOrder(){fos_empty<<"~HugeOrder"<<endl;}
		IJM_AF *createIJM(){return new IJM_240;}
		Mold_AF *createMold(){fos<<"Steel(4) - "; return new Steel;}
		ConveyerBelt_AF *createConveyerBelt(){return new VlevelBelt;}
		PackageBin_AF *createPackbin(){return new Crate;}
	};

	Setup_AF_10 * Setup_AF_10::createOrder(int &orderSize)
	{
		if(orderSize == 0)
		{
			fos<<"  <>No size specified, defaulting to 100."<<endl;
			orderSize = 100;
			return new PilotOrder(100);
		}
		else if(orderSize <= 10000)
		{
			return new PilotOrder(orderSize);
		}
		else if(orderSize <= 20000)
		{
			return new SmallOrder(orderSize);
		}
		else if(orderSize <= 40000)
		{
			return new FastOrder(orderSize);
		}
		else if(orderSize <= 50000)
		{
			return new MediumOrder(orderSize);
		}
		else if(orderSize <= 100000)
		{
			return new LargeOrder(orderSize);
		}
		else if(orderSize <= 200000)
		{
			return new HugeOrder(orderSize);
		}
		else
		{
			fos<<"  <>size too large |"<<orderSize<<"| defaulting to HugeOrder of 200000."<<endl;
			orderSize = 200000;
			return new HugeOrder(orderSize);
		}
	}
}
namespace bridge {			// DP 8.

// Seam Point - add another implementation.
	class Platform_B_9
	{
	public:
		string sMetal;
		int NumCavity;
		virtual ~Platform_B_9(){fos_empty<<"~Platform_B_9"<<endl;}
		virtual void Mill_Shape(const string &in_finish, const string &sMold){}
		static Platform_B_9*create_Platform(const int in_CavityNum, string &sMetal, string &sFinish);
	};

	class HighCarbon : public Platform_B_9
	{
	public:
		HighCarbon(int in_cavity){sMetal = "aluminum"; NumCavity = in_cavity;} 
		virtual ~HighCarbon(){fos_empty<<"~HighCarbon "<<endl;}
		void Mill_Shape(const string &in_finish, const string &sMold)
		{
			fos<<"      using HighCarbon tools (drill, cut, and high speed grind) to mill "<<sMetal.c_str()<<" block into "<<NumCavity<<" "<<sMold<<" shapes with "<<in_finish.c_str()<<" finish."<<endl;
		}
	};

	class Carbide : public Platform_B_9
	{
	public:
		Carbide(int in_cavity){sMetal = "steel"; NumCavity = in_cavity;} 
		virtual ~Carbide(){fos_empty<<"~Carbide "<<endl;}
		void Mill_Shape(const string &in_finish, const string &sMold)
		{
			fos<<"      using Carbide tools (high speed drill, cross cut, and layer grind) to mill "<<sMetal.c_str()<<" block into "<<NumCavity<<" "<<sMold<<" shapes with "<<in_finish.c_str()<<" finish."<<endl;
		}
	};

	class DiamonTipped : public Platform_B_9
	{
	public:
		DiamonTipped(int in_cavity){sMetal = "steel"; NumCavity = in_cavity;} 
		virtual ~DiamonTipped(){fos_empty<<"~DiamonTipped "<<endl;}
		void Mill_Shape(const string &in_finish, const string &sMold)
		{
			fos<<"      using DiamondTipped tools (precision drill, oil cooled cut, and cartoid grind) to mill "<<sMetal.c_str()<<" block into "<<NumCavity<<" "<<sMold<<" shapes with "<<in_finish.c_str()<<" finish."<<endl;
		}
	};

	Platform_B_9 *Platform_B_9::create_Platform(const int in_CavityNum, string &sMetal, string &sFinish)
	{
		if(!sMetal.compare("aluminum"))
		{
			return new bridge::HighCarbon(in_CavityNum);
		}
		else if(!sMetal.compare("steel"))
		{
			if(!sFinish.compare("satin"))
			{
				return new bridge::DiamonTipped(in_CavityNum);
			}
			else
			{
				return new bridge::Carbide(in_CavityNum);
			}
		}
		else
		{
			return new bridge::Platform_B_9;
		}
	}

//////////////////////////////////////////////////////////////////////////
// Seam Point - add another abstraction.
//////////////////////////////////////////////////////////////////////////
	class Shape_B_9
	{
	public:
		Platform_B_9 *pTools;
		string sFinish;
		int Volume;
		string sMold;
		
	public:
		Shape_B_9() : pTools(0),Volume(0){}
		virtual ~Shape_B_9()
		{
			if(pTools) delete pTools;
			fos_empty<<"~Shape_B_9 ";
		}
		virtual void CreateShapewithTools()
		{
			if(pTools)
				pTools->Mill_Shape(sFinish, sMold);
		}
		static Shape_B_9 *create_ShapeObj(string &sMold, string &sFinish);
	};

	class Duck : public Shape_B_9
	{
	public:
		Duck(string &in_Finish)
		{
			sMold = "duck"; 
			sFinish = in_Finish;
			Volume = 35;
		}
		~Duck(){fos_empty<<"~Duck "<<endl;}
	};

	class Car : public Shape_B_9
	{
	public:
		Car(string &in_Finish)
		{
			sMold = "car"; 
			sFinish = in_Finish;
			Volume = 40;
		}
		~Car(){fos_empty<<"~Car "<<endl;}
	};
	class Hero : public Shape_B_9
	{
	public:
		Hero(string &in_Finish)
		{
			sMold = "hero"; 
			sFinish = in_Finish;
			Volume = 50;
		}
		~Hero(){fos_empty<<"~Hero "<<endl;}
	};

	class Dino : public Shape_B_9
	{
	public:
		Dino(string &in_Finish)
		{
			sMold = "dino"; 
			sFinish = in_Finish;
			Volume = 30;
		}
		~Dino(){fos_empty<<"~Dino "<<endl;}
	};

	Shape_B_9 *Shape_B_9::create_ShapeObj(string &sMold, string &sFinish)
	{
		if(!sMold.compare("duck"))
		{
			return new bridge::Duck(sFinish);
		}
		else if(!sMold.compare("car"))
		{
			return new bridge::Car(sFinish);
		}
		else if(!sMold.compare("hero"))
		{
			return new bridge::Hero(sFinish);
		}
		else if(!sMold.compare("dino"))
		{
			return new bridge::Dino(sFinish);
		}
		else
		{
			return new bridge::Duck(sFinish);
		}
	}
}
namespace chain_of_resp {	// DP 7.

// Seam points - add another responder.
	class GetMold_CofR_8
	{
	protected:
		GetMold_CofR_8 *m_pSuccessor;

	public:
		GetMold_CofR_8() : m_pSuccessor(NULL){}
		virtual ~GetMold_CofR_8(){fos_empty<<"~GetMold_CofR_8 "<<endl;}
		void setSuccessor(GetMold_CofR_8 *pSuccessor)
		{
			m_pSuccessor = pSuccessor;
		}
		virtual void GetMold(string &sMoldLoc, string &sMold, int CavictyNum)
		{
			fos<<"    <>Can't find place || to get || mold from, defaulting to duck from inventory."<<endl;
			sMoldLoc = "inventory";
		}
	};

	class Inventory : public GetMold_CofR_8
	{
	public:
		~Inventory(){fos_empty<<"~Inventory "<<endl;}
		void GetMold(string &sMoldLoc, string &sMold, int CavictyNum)
		{
			if(sMoldLoc.compare("inventory") == 0)
				fos<<"    Pull "<<sMold.c_str()<<" mold from inventory."<<endl;
			else if(m_pSuccessor)
				m_pSuccessor->GetMold(sMoldLoc, sMold, CavictyNum);
			else
				GetMold_CofR_8::GetMold(sMoldLoc, sMold, CavictyNum);
		}
	};
	class SisterCompany : public GetMold_CofR_8
	{
	public:
		~SisterCompany(){fos_empty<<"~SisterCompany "<<endl;}
		void GetMold(string &sMoldLoc, string &sMold, int CavictyNum)
		{
			if(sMoldLoc.compare("sisterCompany") == 0)
				fos<<"    Borrow "<<sMold.c_str()<<" mold from sister Company."<<endl;
			else if(m_pSuccessor)
				m_pSuccessor->GetMold(sMoldLoc, sMold, CavictyNum);
			else
				GetMold_CofR_8::GetMold(sMoldLoc, sMold, CavictyNum);
		}
	};
	class Purchase : public GetMold_CofR_8
	{
	public:
		~Purchase(){fos_empty<<"~Purchase "<<endl;}
		void GetMold(string &sMoldLoc, string &sMold, int CavictyNum)
		{
			if(sMoldLoc.compare("purchase") == 0)
				fos<<"    Acquire "<<sMold.c_str()<<" mold via purchase."<<endl;
			else if(m_pSuccessor)
				m_pSuccessor->GetMold(sMoldLoc, sMold, CavictyNum);
			else
				GetMold_CofR_8::GetMold(sMoldLoc, sMold, CavictyNum);
		}
	};
	class Mill : public GetMold_CofR_8
	{
	public:
		~Mill(){fos_empty<<"~Mill "<<endl;}
		void GetMold(string &sMoldLoc, string &sMold, int CavictyNum)
		{
			if(sMoldLoc.compare("mill") == 0)
			{
				fos<<"    Create "<<sMold.c_str()<<" mold from mill with "<<CavictyNum<<" cavities:"<<endl;
			}
			else
				GetMold_CofR_8::GetMold(sMoldLoc, sMold, CavictyNum);
		}
	};
}
namespace decorator {		// DP 6.
	class Plastic_D_6
	{
	public:
		int totalVol;
	public:
		Plastic_D_6() : totalVol(0){}
		virtual void PrintRecipe(){fos<<"      Recipe: ";}
		virtual int GetVolume(){return 0;}
		//virtual void PrintTotal(){fos<<"Total = "<<totalVol<<"."<<endl;}
		virtual ~Plastic_D_6(){fos_empty<<"~Plastic_D_6"<<endl;}
	};

	class AdditiveDecorator : public Plastic_D_6
	{
	public:
		Plastic_D_6 *pPlasticAdditive;
	public:
		AdditiveDecorator(){}
		AdditiveDecorator(Plastic_D_6 *in_PlasticAdditive) : pPlasticAdditive(in_PlasticAdditive){}
		virtual ~AdditiveDecorator(){fos_empty<<"~AdditiveDecorator "<<endl;}
	};
	class AntiBacterial : public AdditiveDecorator
	{
		int volume;
	public:
		AntiBacterial(){}
		AntiBacterial(Plastic_D_6 *in_PlasticAdditive, int in_volume) : volume(in_volume), AdditiveDecorator(in_PlasticAdditive)
		{
			//totalVol+= volume;
		}
		virtual ~AntiBacterial(){fos_empty<<"~AntiBacterial "<<endl;}
		void PrintRecipe()
		{
			pPlasticAdditive->PrintRecipe();
			fos<<"AntiBacterial("<<volume<<") ";
		}
		int GetVolume()
		{
			return pPlasticAdditive->GetVolume() + volume;
		}
	};
	class UVInhibiter : public AdditiveDecorator
	{
		int volume;
	public:
		UVInhibiter(){}
		UVInhibiter(Plastic_D_6 *in_PlasticAdditive, int in_volume) : volume(in_volume), AdditiveDecorator(in_PlasticAdditive)
		{
			//totalVol += volume;
		}
		virtual ~UVInhibiter(){fos_empty<<"~UVInhibiter "<<endl;}
		void PrintRecipe()
		{
			pPlasticAdditive->PrintRecipe();
			fos<<"UVInhibiter("<<volume<<") ";
		}
		int GetVolume()
		{
			return pPlasticAdditive->GetVolume() + volume;
		}
	};
	class MicroFiber : public AdditiveDecorator
	{
		int volume;
	public:
		MicroFiber(){}
		MicroFiber(Plastic_D_6 *in_PlasticAdditive, int in_volume) : volume(in_volume), AdditiveDecorator(in_PlasticAdditive)
		{
			//totalVol+= volume;
		}
		virtual ~MicroFiber(){fos_empty<<"~MicroFiber "<<endl;}
		void PrintRecipe()
		{
			pPlasticAdditive->PrintRecipe();
			fos<<"Microfibers("<<volume<<") ";
		}
		int GetVolume()
		{
			return pPlasticAdditive->GetVolume() + volume;
		}
	};
	class Color : public AdditiveDecorator
	{
		int volume;
		string sColor;
	public:
		Color(){}
		Color(Plastic_D_6 *in_PlasticAdditive, string in_sColor, int PlasticVolume) 
			: AdditiveDecorator(in_PlasticAdditive), sColor(in_sColor)
		{
			volume = PlasticVolume/10;
		}
		virtual ~Color(){fos_empty<<"~Color "<<endl;}
		void PrintRecipe()
		{
			pPlasticAdditive->PrintRecipe();
			fos<<sColor.c_str()<<"("<<volume<<") ";
		}
		int GetVolume()
		{
			return pPlasticAdditive->GetVolume() + volume;
		}
	};
	class ABS : public AdditiveDecorator
	{
		int volume;
	public:
		ABS(){}
		ABS(Plastic_D_6 *in_PlasticAdditive, int in_volume) 
			: volume(in_volume), AdditiveDecorator(in_PlasticAdditive)
		{
			//totalVol += volume;
		}
		virtual ~ABS(){fos_empty<<"~ABS "<<endl;}
		
		void PrintRecipe()
		{
			pPlasticAdditive->PrintRecipe();
			fos<<"ABS("<<volume<<") ";
		}
		int GetVolume()
		{
			return pPlasticAdditive->GetVolume() + volume;
		}
	};
	class Polypropylene : public AdditiveDecorator
	{
		int volume;
	public:
		Polypropylene(){}
		Polypropylene(Plastic_D_6 *in_PlasticAdditive, int in_volume) 
			: volume(in_volume), AdditiveDecorator(in_PlasticAdditive)
		{
			//totalVol += volume;
		}
		virtual ~Polypropylene(){fos_empty<<"~Polypropylene "<<endl;}

		void PrintRecipe()
		{
			pPlasticAdditive->PrintRecipe();
			fos<<"Polypropylene("<<volume<<") ";
		}
		int GetVolume()
		{
			return pPlasticAdditive->GetVolume() + volume;
		}
	};
	class PET : public AdditiveDecorator
	{
		int volume;
	public:
		PET(){}
		PET(Plastic_D_6 *in_PlasticAdditive, int in_volume) 
			: volume(in_volume), AdditiveDecorator(in_PlasticAdditive)
		{
			//totalVol += volume;
		}
		virtual ~PET(){fos_empty<<"~PET "<<endl;}
		void PrintRecipe()
		{
			pPlasticAdditive->PrintRecipe();
			fos<<"PET("<<volume<<") ";
		}
		int GetVolume()
		{
			return pPlasticAdditive->GetVolume() + volume;
		}
	};
	class Polyethelene : public AdditiveDecorator
	{
		int volume;
	public:
		Polyethelene(){}
		Polyethelene(Plastic_D_6 *in_PlasticAdditive, int in_volume) 
			: volume(in_volume), AdditiveDecorator(in_PlasticAdditive)
		{
			//totalVol += volume;
		}
		virtual ~Polyethelene(){fos_empty<<"~Polyethelene "<<endl;}
		void PrintRecipe()
		{
			pPlasticAdditive->PrintRecipe();
			fos<<"Polyethelene("<<volume<<") ";
		}
		int GetVolume()
		{
			return pPlasticAdditive->GetVolume() + volume;
		}
	};
	class Styrene : public AdditiveDecorator
	{
		int volume;
	public:
		Styrene(){}
		Styrene(Plastic_D_6 *in_PlasticAdditive, int in_volume) 
			: volume(in_volume), AdditiveDecorator(in_PlasticAdditive)
		{
			//totalVol += volume;
		}
		virtual ~Styrene(){fos_empty<<"~Styrene "<<endl;}
		void PrintRecipe()
		{
			pPlasticAdditive->PrintRecipe();
			fos<<"Styrene("<<volume<<") ";
		}
		int GetVolume()
		{
			return pPlasticAdditive->GetVolume() + volume;
		}
	};
	class Nilon66 : public AdditiveDecorator
	{
		int volume;
	public:
		Nilon66(){}
		Nilon66(Plastic_D_6 *in_PlasticAdditive, int in_volume) 
			: volume(in_volume), AdditiveDecorator(in_PlasticAdditive)
		{
			//totalVol += volume;
		}
		virtual ~Nilon66(){fos_empty<<"~Nilon66 "<<endl;}
		void PrintRecipe()
		{
			pPlasticAdditive->PrintRecipe();
			fos<<"Nylon66("<<volume<<") ";
		}
		int GetVolume()
		{
			return pPlasticAdditive->GetVolume() + volume;
		}
	};

	class Tags_D_6
	{
	public:
		int tagWidth;
		list<Tags_D_6 *> TagDecoratorList;
	public:
		virtual ~Tags_D_6()
		{
			fos_empty<<"~Tags_D_6"<<endl;
			for(std::list<Tags_D_6*>::iterator it = TagDecoratorList.begin(); it != TagDecoratorList.end(); ++it)
			{
				if(*it)
					delete (*it);
			}
			TagDecoratorList.clear();
		}
		virtual void insertTags(){}
		virtual int GetTagWidth(){return 0;}
	};

	class TagDecorator : public Tags_D_6
	{
	public:
		Tags_D_6 *m_pdecorator;
	public:
		TagDecorator(Tags_D_6 * in_tag) : m_pdecorator(in_tag){}
		virtual ~TagDecorator(){fos_empty<<"~TagDecorator "<<endl;}
	};
	class Blank : public TagDecorator
	{
	public:
		Blank(Tags_D_6 *in_tag) : TagDecorator(in_tag){}
		~Blank(){fos_empty<<"~Blank "<<endl;}
	};
	class ModelNumber : public TagDecorator
	{
		int tagWidth;
	public:
		ModelNumber(Tags_D_6 *in_tag) : TagDecorator(in_tag), tagWidth(2)
		{
		}
		~ModelNumber(){fos_empty<<"~ModelNumber "<<endl;}
		void insertTags()
		{
			fos<<"ModelNumber ";
		}
		int GetTagWidth(){return tagWidth;}
	};
	class Country : public TagDecorator
	{
		int tagWidth;
	public:
		Country(Tags_D_6 *in_tag) : TagDecorator(in_tag), tagWidth(2)
		{
		}
		~Country(){fos_empty<<"~Country "<<endl;}
		void insertTags()
		{
			fos<<"Country ";
		}
		int GetTagWidth(){return tagWidth;}
	};
	class Date : public TagDecorator
	{
		int tagWidth;
	public:
		Date(Tags_D_6 *in_tag) : TagDecorator(in_tag), tagWidth(2)
		{
		}
		~Date(){fos_empty<<"~Date "<<endl;}
		void insertTags()
		{
			fos<<"Date ";
		}
		int GetTagWidth(){return tagWidth;}
	};
	class IncrementCounter : public TagDecorator
	{
		int tagWidth;
	public:
		IncrementCounter(Tags_D_6 *in_tag) : TagDecorator(in_tag), tagWidth(4)
		{
		}
		~IncrementCounter(){fos_empty<<"~IncrementCounter "<<endl;}
		void insertTags()
		{
			fos<<"IncCounter ";
		}
		int GetTagWidth(){return tagWidth;}
	};
	class PartNumber : public TagDecorator
	{
		int tagWidth;
	public:
		PartNumber(Tags_D_6 *in_tag) : TagDecorator(in_tag), tagWidth(2)
		{
		}
		~PartNumber(){fos_empty<<"~PartNumber "<<endl;}
		void insertTags()
		{
			fos<<"PartNumber ";
		}
		int GetTagWidth(){return tagWidth;}
	};
	class RecycleCode : public TagDecorator
	{
		int tagWidth;
	public:
		RecycleCode(Tags_D_6 *in_tag) : TagDecorator(in_tag), tagWidth(6)
		{
		}
		~RecycleCode(){fos_empty<<"~RecycleCode "<<endl;}
		void insertTags()
		{
			fos<<"RecycleCode ";
		}
		int GetTagWidth(){return tagWidth;}
	};
}
namespace factory_method {	// DP 4.
	class observer::Subject_7;

	class Packager_FM_5 : public observer::Observer_7
	{
	public:
		virtual ~Packager_FM_5(){fos_empty<<"~Packager_FM_5 "<<endl;}
		static Packager_FM_5 * createPackager(string &sPackager);

		void update(observer::Subject_7 *pSubject)
		{
			fos<<"        Packager pausing while "<<pSubject->sPackageBinName.c_str()<<" package bin is swapped."<<endl;
		}
	};

	class Bulk : public Packager_FM_5
	{
	public:
		~Bulk(){fos_empty<<"~Bulk "<<endl;}
	};

	class ShrinkWrap : public Packager_FM_5
	{
	public:
		~ShrinkWrap(){fos_empty<<"~ShrinkWrap "<<endl;}
	};

	class HardPack : public Packager_FM_5
	{
	public:
		~HardPack(){fos_empty<<"~HardPack "<<endl;}
	};
	class ZipLock : public Packager_FM_5
	{
	public:
		~ZipLock(){fos_empty<<"~ZipLock "<<endl;}
	};

	Packager_FM_5 * Packager_FM_5::createPackager(string &sPackager)
	{
		if(sPackager.compare("Bulk") == 0)
		{
			return new Bulk;
		}
		else if(sPackager.compare("ShrinkWrap") == 0)
		{
			return new ShrinkWrap;
		}
		else if(sPackager.compare("HardPack") == 0)
		{
			return new HardPack;
		}
		else if(sPackager.compare("ZipLock") == 0)
		{
			return new ZipLock;
		}
		else
		{
			sPackager = "Bulk";
			fos<<"  <>Unknown packager || defaulting to Bulk packager."<<endl;
			return new Bulk;
		}
	}
}
namespace template_method {	// DP 3.

// Seam point - add another polymorphic step.
// Seam point - add another constant step.
// Seam point - convert a constant step into a polymorphic step.
	class ProcessOrder_TM_4
	{
	protected: 
		int CavityNum;
		int OrderSize;

		string sPackager;
		string sPlastic;
		string sMold;
		string sMoldLoc;
		string sFinish;
		string sColor;
		int UvInhibiterSize;
		int AntiBacterialSize;
		string sTags;
		std::vector<string>TagVector;
		
		string sMetal;
		int PlasticVolume;
		int MicroFibersSize;
		string sAddress;
		string sStuffer;

	public:
		adapter::CleanPlastic_A_2 *pPlastic;
		chain_of_resp::Inventory oInventory;
		chain_of_resp::SisterCompany oSisterCompany;
		chain_of_resp::Purchase oPurchase;
		chain_of_resp::Mill oMill;
		bridge::Shape_B_9 *pShape;
		bridge::Platform_B_9 *pMillTools;
		decorator::Tags_D_6 oTags;
		decorator::Plastic_D_6 oAdditives;
		strategy::Strategy_1 *pInjectProcess;

		abstract_factory::Setup_AF_10 *pOrder;
		factory_method::Packager_FM_5 *pPackager;
		abstract_factory::IJM_AF *pIJM;
		abstract_factory::Mold_AF *pMold;
		abstract_factory::ConveyerBelt_AF *pConveyerBelt;
		abstract_factory::PackageBin_AF *pPackageBin;
		observer::Stuffer *pStuffer;

	private:

		void ParseOrder(std::map<string, string> &order)
		{
			OrderSize = atoi(order["size"].c_str());
			sPackager = order["packager"];
			sPlastic = order["plastic"];
			sMold = order["mold"];
			sMoldLoc = order["moldLoc"];
			sFinish = order["finish"];
			sColor = order["color"];
			UvInhibiterSize = atoi(order["UVInhibiter"].c_str());
			AntiBacterialSize = atoi(order["AntiBacterial"].c_str());
			MicroFibersSize = atoi(order["MicroFibers"].c_str());
			sTags = order["tags"];
			sAddress = order["address"];
			sStuffer = order["stuffer"];

			if(!sTags.empty())
			{
				int pos = 0;
				int TagEndPos = 0;
				do
				{
					TagEndPos = sTags.find(' ', pos);

					string sOneTag = sTags.substr(pos, TagEndPos-pos);
					TagVector.push_back(sOneTag);
					pos = TagEndPos+1;
				}while(TagEndPos !=string::npos);
			}
		}

	public:
		ProcessOrder_TM_4() 
			: pPlastic(NULL),  pShape(NULL), pMillTools(NULL), pInjectProcess(NULL), 
				pOrder(NULL), pPackager(NULL), pIJM(NULL), pMold(NULL),
				pConveyerBelt(NULL),  pPackageBin(NULL), pStuffer(NULL),
				OrderSize(0), CavityNum(0), PlasticVolume(0)
		{
		}

		virtual ~ProcessOrder_TM_4()
		{
			if(pShape)
				delete pShape;
			if(pInjectProcess)
				delete pInjectProcess;
			if(pPlastic)
				delete pPlastic;
			if(pStuffer)
				delete pStuffer;
			if(pPackageBin)
				delete pPackageBin;
			if(pConveyerBelt)
				delete pConveyerBelt;
			if(pMold)
				delete pMold;
			if(pIJM)
				delete pIJM;
			if(pPackager)
				delete pPackager;
			if(pOrder)
				delete pOrder;
			fos_empty<<"~ProcessOrder_TM_4"<<endl;
		}


		void ProcessOrder(std::map<string, string>	&order)
		{
			ParseOrder(order);

			SetupInjectLine();
			fos<<"  Process order:"<<endl;

			GetMold(sMold, sMoldLoc);
			InsertTags();
			LoadBins();
			RunInjectCycle();
			CleanMold();
			ShipToAddress();
		}

	protected:
		virtual void SetupInjectLine()
		{
			if(sPlastic.compare("ABS") && sPlastic.compare("PET") && 
				sPlastic.compare("Polypropylene") && sPlastic.compare("Polyethelene") &&
				sPlastic.compare("Styrene") && sPlastic.compare("Nylon66"))
			{
				fos<<"  <>Unknown plastic || defaulting to ABS."<<endl;
				sPlastic = "ABS";
			}

			pOrder = abstract_factory::Setup_AF_10::createOrder(OrderSize);

			pPackager = factory_method::Packager_FM_5::createPackager(sPackager);

			observer::Stuffer *pStuffer = observer::Stuffer::create_stuffer(sStuffer);
			sStuffer = pStuffer->sStufferName;
			//CavityNum = pIJM->CavityNum;	
			CavityNum = 1;
			//fos<<"  Setup injection line for "<<OrderSize/CavityNum<<" run with "<<sPackager.c_str()<<" packager and "<<sStuffer.c_str()<<" stuffing:"<<endl;
			fos<<"  Setup injection line for "<<OrderSize/CavityNum<<" order:"<<endl;
			abstract_factory::IJM_AF *pIJM = pOrder->createIJM();
			abstract_factory::Mold_AF *pMold = pOrder->createMold();
			abstract_factory::ConveyerBelt_AF *pConveyerBelt = pOrder->createConveyerBelt();
			pPackageBin = pOrder->createPackbin();


			pPackageBin->Attach(pIJM);
			pPackageBin->Attach(pConveyerBelt);
			pPackageBin->Attach(pPackager);
			pPackageBin->Attach(pStuffer);
			
			sMetal = pMold->sMetal;
			pPlastic = adapter::CleanPlastic_A_2::create_PlasticCleanObj(sPlastic, sMetal);
		}
		virtual void GetMold(string &sMold, string &sMoldLoc)
		{
			oInventory.setSuccessor(&oSisterCompany);
			oSisterCompany.setSuccessor(&oPurchase);
			oPurchase.setSuccessor(&oMill);
			oInventory.GetMold(sMoldLoc, sMold, CavityNum);
			
			pShape = bridge::Shape_B_9::create_ShapeObj(sMold, sFinish);
			
			PlasticVolume = pShape->Volume;

			if(!sMoldLoc.compare("mill"))
			{
				pMillTools = bridge::Platform_B_9::create_Platform(CavityNum, sMetal, sFinish);

				pShape->pTools = pMillTools;
				pShape->CreateShapewithTools();
			}
		};

		virtual void InsertTags()
		{
			fos<<"    Insert tags [";
			int TagWidth = 0;
			oTags.TagDecoratorList.push_back(new decorator::Blank(&oTags));

			for(unsigned int i = 0; i<TagVector.size(); ++i)
			{
				string Tag = TagVector[i];
				if(!Tag.compare("ModelNumber"))
				{
					oTags.TagDecoratorList.push_back(new decorator::ModelNumber(&oTags));
					TagWidth += oTags.TagDecoratorList.back()->GetTagWidth();
					oTags.TagDecoratorList.back()->insertTags();
				}
				else if(!Tag.compare("Country"))
				{
					oTags.TagDecoratorList.push_back(new decorator::Country(&oTags));
					TagWidth += oTags.TagDecoratorList.back()->GetTagWidth();
					oTags.TagDecoratorList.back()->insertTags();
				}
				else if(!Tag.compare("Date"))
				{
					oTags.TagDecoratorList.push_back(new decorator::Date(&oTags));
					TagWidth += oTags.TagDecoratorList.back()->GetTagWidth();
					oTags.TagDecoratorList.back()->insertTags();
				}
				else if(!Tag.compare("IncCounter"))
				{
					oTags.TagDecoratorList.push_back(new decorator::IncrementCounter(&oTags));
					TagWidth += oTags.TagDecoratorList.back()->GetTagWidth();
					oTags.TagDecoratorList.back()->insertTags();
				}
				else if(!Tag.compare("PartNumber"))
				{
					oTags.TagDecoratorList.push_back(new decorator::PartNumber(&oTags));
					TagWidth += oTags.TagDecoratorList.back()->GetTagWidth();
					oTags.TagDecoratorList.back()->insertTags();
				}
				else if(!Tag.compare("RecycleCode"))
				{
					oTags.TagDecoratorList.push_back(new decorator::RecycleCode(&oTags));
					TagWidth += oTags.TagDecoratorList.back()->GetTagWidth();
					oTags.TagDecoratorList.back()->insertTags();
				}
			}
			fos<<"] of width "<<TagWidth<<"/20 mm."<<endl;;
		}
		virtual void LoadBins()
		{
			fos<<"    Load plastic, color, and additive bins."<<endl;
			int colorValue = 0;

			if(sColor.empty())
			{
				fos<<"      <>No color specified, defaulting to black."<<endl;
				sColor = "black";
			}
			
			decorator::Plastic_D_6 *pDecorator = &oAdditives;

			//can change to factory method below
			decorator::ABS *pABS = NULL;
			decorator::PET *pPET = NULL;
			decorator::Polyethelene *pPolyethelene = NULL;
			decorator::Polypropylene *pPolypropylene = NULL;
			decorator::Styrene *pStyrene = NULL;
			decorator::Nilon66 *pNilon66 = NULL;
			if(sPlastic.compare("ABS") == 0)
			{
				pABS = new decorator::ABS(pDecorator, PlasticVolume);
				pDecorator = pABS;
			}
			else if(sPlastic.compare("PET") == 0)
			{
				pPET = new decorator::PET(pDecorator, PlasticVolume);
				pDecorator = pPET;
			}
			else if(sPlastic.compare("Polypropylene") == 0)
			{
				pPolypropylene = new decorator::Polypropylene(pDecorator, PlasticVolume);
				pDecorator = pPolypropylene;
			}
			else if(sPlastic.compare("Styrene") == 0)
			{
				pStyrene = new decorator::Styrene(pDecorator, PlasticVolume);
				pDecorator = pStyrene;
			}
			else if(sPlastic.compare("Nylon66") == 0)
			{
				pNilon66 = new decorator::Nilon66(pDecorator, PlasticVolume);
				pDecorator = pNilon66;
			}
			else if(sPlastic.compare("Polyethelene") == 0)
			{
				pPolyethelene = new decorator::Polyethelene(pDecorator, PlasticVolume);
				pDecorator = pPolyethelene;
			}
			else
			{
				pABS = new decorator::ABS(pDecorator, PlasticVolume);
				pDecorator = pABS;
			}

			decorator::Color *pColorAdditive = new decorator::Color(pDecorator, sColor, PlasticVolume);
			pDecorator = pColorAdditive;
			
			decorator::UVInhibiter *pUVInhibiter = NULL;
			if (UvInhibiterSize>0)
			{
				pUVInhibiter = new decorator::UVInhibiter(pDecorator, UvInhibiterSize);
				pDecorator = pUVInhibiter;
			}
			decorator::AntiBacterial *pAntiBacterial = NULL;
			if(AntiBacterialSize > 0)
			{
				pAntiBacterial = new decorator::AntiBacterial(pDecorator, AntiBacterialSize);
				pDecorator = pAntiBacterial;
			}
			decorator::MicroFiber *pMicrofiber = NULL;
			if(MicroFibersSize > 0)
			{
				pMicrofiber = new decorator::MicroFiber(pDecorator, MicroFibersSize);
				pDecorator = pMicrofiber;
			}

			pDecorator->PrintRecipe();
			fos<<"Total = "<<pDecorator->GetVolume() * CavityNum<<"."<<endl;

			if(pMicrofiber)
				delete pMicrofiber;
			if(pAntiBacterial)
				delete pAntiBacterial;
			if(pUVInhibiter)
				delete pUVInhibiter;
			if(pColorAdditive)
				delete pColorAdditive;
			if(pABS)
				delete pABS;
			if(pNilon66)
				delete pNilon66;
			if(pStyrene)
				delete pStyrene;
			if(pPET)
				delete pPET;
			if(pPolyethelene)
				delete pPolyethelene;
			if(pPolypropylene)
				delete pPolypropylene;
		}
	
		virtual void RunInjectCycle()
		{
			string sPlasticName = pPlastic->sPlasticName.c_str();
			fos<<"    Cycle IJM for "<<sPlasticName<<" "<<OrderSize/CavityNum<<" times."<<endl;

			pInjectProcess = strategy::Strategy_1::Create_InjectionObj(sPlasticName);
			pInjectProcess->Inject();

			pPackageBin->notify();
		}

		virtual void CleanMold()
		{
			pPlastic->clean();
		}

		virtual void ShipToAddress()
		{
			fos<<"    Ship to \""<<sAddress.c_str()<<"\""<<endl;
		}
	};
}



void process(map<string,string>& order) {
	// Fill in the namespaces above with your design pattern class hierarchies.
	// Call your order processing class from here <myProcess>->run(order);
	template_method::ProcessOrder_TM_4 *pProcessOrder = new template_method::ProcessOrder_TM_4();
	
	pProcessOrder->ProcessOrder(order);
	
	if(pProcessOrder)
		delete pProcessOrder;

}

pair<string,string> parse(string line) {
	char key[83];
	char val[83] = {0};

	sscanf(line.c_str(), "%s", key);

	char * equalSign = (char *) strchr(line.c_str(), '=');
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
		fos<<line;
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
	fos.open("solution.txt", ios::out);
	cout << "<<< final design >>>\n";
	fos << "<<< final design >>>\n";

	FILE* orderFilePtr = fopen(orderFile.c_str(), "r");
	if(!orderFilePtr) return;

	for(;;) {	// Get and process each order.
		map<string,string> order = getCompleteOrder(orderFilePtr);
		if(order.size() == 0)
			break;
		process(order);
	}
	fclose(orderFilePtr);

	fos << "Aloha DP4.\n" << endl;
	fos.close();

}

} // final_design

#endif /* FINAL_FINAL_DESIGN_H_ */

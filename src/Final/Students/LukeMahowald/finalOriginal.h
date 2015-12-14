/*
 * finalDesign.h
 *
 *  Created on: <MMDDCCYY>
 *      Author: <student name>
 */

#ifndef FINAL_FINAL_DESIGN_H_
#define FINAL_FINAL_DESIGN_H_

#include <iostream>
#include <string>
using namespace std;

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
// Cleaning method adapts the different legacy codes to the same interface
    class CleaningMethod
    {
    public:
        virtual void Clean() = 0;
    };
    class ABSCleaningMethod : public CleaningMethod
    {
    public:
        void Clean() {
            legacy_classes::CleanABS abs;
            abs.clean();
        }
    };
    class PolysCleaningMethod : public CleaningMethod
    {
    public:
        void Clean() {
            legacy_classes::CleanPolys poly;
            poly.prepForReuse();
        }
    };
    class PetSteelCleaningMethod : public CleaningMethod
    {
    public:
        void Clean() {
            legacy_classes::PETCleanup pet;
            pet.carbonize();
        }
    };
    class PetAluminumCleaningMethod : public CleaningMethod
    {
    public:
        void Clean() {
            legacy_classes::PETCleanup pet;
            pet.purify();
        }
    };
// Seam point - add another interface.

    CleaningMethod* GetCleaningMethod(string plastic, string metal)
    {
        if (plastic == "ABS") {
            return new ABSCleaningMethod;
        } else if ((plastic == "Polypropylene") || (plastic == "Polyethelene")) {
            return new PolysCleaningMethod;
        } else if (plastic == "PET"){
            if (metal == "aluminum") {
                return new PetAluminumCleaningMethod;
            } else {
                return new PetSteelCleaningMethod;
            }
        }
        // Default
        cout << "Should not get this far!" << endl;
        return new ABSCleaningMethod;
    }
}

namespace strategy {		// DP 1.
// There are strategy patterns throughout.
// IJM, CleaningMethod

}

namespace observer {		// DP 5.

    class Listener
    {
    public:
        virtual string Pause() = 0;
    };

    // PartsBin is the observer, needs to tell IJM, conveyer, and packager to pause
    class PartsBin
    {
    protected:
        string name;
    public:
        list<Listener*> listeners;
        void Attach(Listener *listener) {
            listeners.push_back(listener);
        }
        void Pause() {
            cout << "      Whenever " << name << " parts bin was full..." << endl;
            for (auto each : listeners) {
                string listenerSays = each->Pause();
                if (listenerSays.length() > 0) {
                    cout << listenerSays << " while " << name << " parts bin was swapped." << endl;
                }
            }
        }
        
    };
    class CardboardBox : public PartsBin
    {
    public:
        CardboardBox()
        {
            name = "CardboardBox";
            cout << " - CardboardBox." << endl;
        }
    };
    class ShellBox :     public PartsBin
    {
    public:
        ShellBox()
        {
            name = "ShellBox";
            cout << " - ShellBox." << endl;
        }
    };
    class PalletBox :     public PartsBin
    {
    public:
        PalletBox()
        {
            name = "PalletBox";
            cout << " - PalletBox." << endl;
        }
    };
// Seam point - add another listener.

    class Packager : public Listener {
    public:
        virtual void Print() {}
    };
    class BulkPackager       : public Packager{
    public:
        string Pause() {return "";}
    };
    class ShrinkWrapPackager : public Packager{
    public:
        string Pause() {
            return "        Shrink wrap packager paused";
        }
        void Print() {cout << " with ShrinkWrap packager";}
    };
    class HardPackPackager   : public Packager{
    public:
        string Pause() {
            return "        Hard pack packager paused";
        }
        void Print() {cout << " with HardPack packager";}
    };

    Packager* GetPackager(string packager)
    {
        if (packager == "Bulk")       return new BulkPackager;
        if (packager == "ShrinkWrap") return new ShrinkWrapPackager;
        if (packager == "HardPack")   return new HardPackPackager;
        //cout << "  <>Unknown packager || defaulting to 'None'." << endl;
        cout << "ERROR! Should not be here!" << endl;
        exit(-1);
        return new BulkPackager;
    }
}

namespace abstract_factory {// DP 9.

    class IJM : public observer::Listener
    {
    public:
        virtual string PrintName() {return "DEFAULT";}

    };
    class IJM_110 : public IJM
    {
    public:
        string PrintName()
        {
            return "IJM_110";
        }
        IJM_110()
        {
            cout << "    " << PrintName();
        }
        string Pause() {
            return "        IJM_110 paused";
        }
    };
    class IJM_120 : public IJM
    {
    public:
        string PrintName()
        {
            return "IJM_120";
        }
        IJM_120()
        {
            cout << "    " << PrintName();
        }
        string Pause() {
            return "        IJM_120 paused";
        }
    };
    class IJM_210 : public IJM 
    {
    public:
        string PrintName()
        {
            return "IJM_210";
        }
        IJM_210()
        {
            cout << "    " << PrintName();
        }
        string Pause() {
            return "        IJM_210 paused";
        }
    };
    
    class Mold
    {
        
    public:
        // Public to be lazy...
        string mold;
        int    volume;
        string millSteps;
        string moldLoc;
        string finish;
        int    cavities;
        string metal;
        
        void SetMoldInfo(map<string,string>& order) {
            if (order["mold"] == "car") {
                mold = "car";
                volume = 40;
                millSteps = "D2C8D1G3";
            } else if (order["mold"] == "hero") {
                mold = "hero";
                volume = 50;
                millSteps = "D1C1D1G7";
            } else if (order["mold"] == "duck") {
                mold = "duck";
                volume = 35;
                millSteps = "D3C2D2G4";
            } else {
                cout << "    <>Unknown mold |" << order["mold"] << "| defaulting to 'duck'.\n";
                mold = "duck";
                volume = 35;
                millSteps = "D3C2D2G4";
            }
            moldLoc = order["moldLoc"];
            finish  = order["finish"];
        }
        void InsertTags() {};
    };
    class AluminumOneMold : public Mold
    {
    public:
        AluminumOneMold()
        {
            cout << " - Aluminum(1)";
            cavities = 1;
            metal = "aluminum";
        }
    };
    class AluminumTwoMold : public Mold
    {
    public:
        AluminumTwoMold()
        {
            cout << " - Aluminum(2)";
            cavities = 2;
            metal = "aluminum";
        }
    };
    class SteelOneMold : public Mold
    {
    public:
        SteelOneMold()
        {
            cout << " - Steel(1)";
            cavities = 1;
            metal = "steel";
        }
    };
    
    class Conveyer : public observer::Listener {};
    class LinearConveyer : public Conveyer
    {
    public:
        LinearConveyer()
        {
            cout << " - Linear conveyer belt";
        }
        string Pause() {
            return "        Linear conveyer belt paused";
        }
    };
    class SplitConveyer : public Conveyer
    {
    public:
        SplitConveyer()
        {
            cout << " - Y-Split conveyer belt";
        }
        string Pause() {
            return "        Y-Split conveyer belt paused";
        }
    };
    
// InjectionLineFactory is the abstract factory, and is used to get the "family" of machines based on the order size.
// Machinery, IJM, conveyer belt, box
    class InjectionLineFactory
    {
    public:
        int size;
        observer::Packager *linePackager;
        InjectionLineFactory(int orderSize, string packager) : size(orderSize)
        {
            linePackager = observer::GetPackager(packager);
            cout << "  Setup injection line for " << size << " order";
            linePackager->Print();
            cout << ":" << endl; // Packager needs to print here...
        }
        ~InjectionLineFactory() {
            delete linePackager;
        }
        virtual IJM*      GetIJM() = 0;
        virtual Mold*     GetMold() = 0;
        virtual Conveyer* GetConveyer() = 0;
        virtual observer::PartsBin* GetPartsBin() = 0;
        virtual int       RuntimeEstimate(int volume) = 0;
        observer::Packager* GetPackager(){return linePackager;}
    };
    // < 10000
    class SmallOrderLineFactory : public InjectionLineFactory
    {
    public:
        SmallOrderLineFactory(int orderSize, string packager) : InjectionLineFactory(orderSize, packager){}
        IJM*      GetIJM() {
            return new IJM_110();
        }
        Mold*     GetMold() {
            return new AluminumOneMold();
        }
        Conveyer* GetConveyer() {
            return new LinearConveyer();
        }
        observer::PartsBin* GetPartsBin() {
            return new observer::CardboardBox();
        }
        
        int RuntimeEstimate(int volume) {
            int cavities = 1;
            //cout << "size: " << size << endl;
            //cout << "cavities: " << cavities << endl;
            //cout << "size/cavities*(1/60): " << (float)size/cavities*(1.0/60) << endl;
            float runTime = size/cavities*(1.0/60);
            return runTime;
        }
    };
    // < 20000
    class MediumOrderLineFactory : public InjectionLineFactory
    {
    public:
        MediumOrderLineFactory(int orderSize, string packager) : InjectionLineFactory(orderSize, packager){}
        IJM*      GetIJM() {
            return new IJM_120();
        }
        Mold*     GetMold() {
            return new AluminumTwoMold();
        }
        Conveyer* GetConveyer() {
            return new SplitConveyer();
        }
        observer::PartsBin* GetPartsBin() {
            return new observer::ShellBox();
        }
        int RuntimeEstimate(int volume) {
            string metal = "aluminum";
            int cavities = 2;
            float runTime = size/cavities*legacy_classes::cycleTime_sec(metal, volume)/3600;
            return runTime;
        }
    };
    // < 50000
    class LargeOrderLineFactory : public InjectionLineFactory
    {
    public:
        LargeOrderLineFactory(int orderSize, string packager) : InjectionLineFactory(orderSize, packager){}
        IJM*      GetIJM() {
            return new IJM_210();
        }
        Mold*     GetMold() {
            return new SteelOneMold();
        }
        Conveyer* GetConveyer() {
            return new LinearConveyer();
        }
        observer::PartsBin* GetPartsBin() {
            return new observer::PalletBox();
        }
        
        int RuntimeEstimate(int volume) {
            string metal = "steel";
            int cavities = 1;
            //cout << "cycletime secs: " << legacy_classes::cycleTime_sec(metal, volume) << endl;
            float cycletime = legacy_classes::cycleTime_sec(metal, volume);
            float runTime   = (legacy_classes::setupAvg_min + (size/cavities*cycletime)/60 + legacy_classes::teardownAvg_min)/60.0;
            return runTime;
            //cout << "    <>Using most sophisticated estimation algorithm for orders greater than 100000.\n";
        }
    };
    // Seam point - add another type 1.
    // Seam point - add another type 2.
    // Seam point - add another type 3.
    // Seam point - add another ...
    // Seam point - add another type N.
    // Seam point - add another family.
    InjectionLineFactory* GetInjectionLineFactory(int orderSize, string packager)
    {
        if (orderSize <= 10000) return new SmallOrderLineFactory(orderSize, packager);
        if (orderSize <= 20000) return new MediumOrderLineFactory(orderSize, packager);
        if (orderSize <= 50000) return new LargeOrderLineFactory(orderSize, packager);

        cout << "  <>Size exceeds mold lifetime |" << orderSize << "| defaulting to MediumOrder of 50000.\n";
        orderSize = 50000;
        return new LargeOrderLineFactory(orderSize, packager);
    }
}

namespace bridge {			// DP 8.
// The bridge is the Packager.

// InjectionCycle is not the bridge! It could have been but doing so would not have offered much of an advantage.
    class InjectionCycle
    {
    public:
        virtual void Run() = 0;
    };
    class ABSCycle : public InjectionCycle
    {
    public:
        void Run() {cout << "    Close - mix - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject." << endl;}
    };
    class PolyCycle : public InjectionCycle
    {
    public:
        void Run() {cout << "    Close - mix - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject." << endl;}
    };
    class PETCycle : public InjectionCycle
    {
    public:
        void Run() {cout << "    Close - mix - heat to 404 - inject at 110 PSI - cool to 340 - separate - smooth eject." << endl;}
    };
    
    InjectionCycle* GetInjectionCycle(string plastic) {
        // THIS USES SIMILAR IF STRUCTURE AS GetCleaningMethod!
        if (plastic == "ABS") {
            return new ABSCycle;
        } else if ((plastic == "Polypropylene") || (plastic == "Polyethelene")) {
            return new PolyCycle;
        } else if (plastic == "PET"){
            return new PETCycle;
        } else {
            cout << "ERROR! Plastic type not recognized! BUG!" << endl;
            exit(-1);
        }
    };
    
// Seam Point - add another implementation.
// Seam Point - add another abstraction.

}

namespace chain_of_resp {	// DP 7.
// Place to get mold from, Inventory, Sister Company, Mill

    class MoldSource
    {
    public:
        string me;
        MoldSource *successor = nullptr;
        
        MoldSource(string myName) : me(myName) {}
        ~MoldSource(){
            delete successor;
        }
        
        virtual void ProvideMold(abstract_factory::Mold *mold) = 0;
        
        void GetMold(abstract_factory::Mold *mold) {
            //cout << "Entering GetMold, me = " << me << endl;
            if (mold->moldLoc == me) {
                ProvideMold(mold);
                return;
            }
            if (successor == nullptr) {
                cout << "    <>Can't find place |" << mold->moldLoc << "| to get |" << mold->mold << "| mold from with |" << mold->finish << "| finish, defaulting to smooth duck from inventory." << endl;
                mold->moldLoc = "Inventory";
                mold->finish = "Smooth";
                mold->mold = "duck";
                //cout << "MoldSource Error: Got to end of chain, this should not be the case!" << endl;
                //cout << "mold location: " << mold->moldLoc << endl;
                //exit(-1);
                return;
            }
            successor->GetMold(mold);
        }
    };
    class Inventory : public MoldSource
    {
    public:
        Inventory() : MoldSource("inventory") {}
        
        void ProvideMold(abstract_factory::Mold *mold) {
            cout << "    Pull " << mold->mold << " mold from inventory.\n";
        }
    };
    class SisterCompany : public MoldSource
    {
    public:
        SisterCompany() : MoldSource("sisterCompany") {}
        
        void ProvideMold(abstract_factory::Mold *mold) {
            cout << "    Borrow " << mold->mold << " mold from sister company.\n";
        }
    };
    class Mill : public MoldSource
    {
    public:
        Mill() : MoldSource("mill") {}
        
        void ProvideMold(abstract_factory::Mold *mold) {
            cout << "    Create " << mold->mold << " mold from mill with " << mold->cavities << " cavities - steps: " << mold->millSteps << ".\n";
            if ((mold->metal == "aluminum") || (mold->finish == "smooth")) {
                cout << "      using HighCarbon tools (drill, cut, and high speed grind)";
            } else if (mold->finish == "rippled") {
                cout << "      using Carbide tools (high speed drill, cross cut, and layer grind)";
            //} else if (mold.finish == "dimpled") {
            } else {
                // default
                cout << "      using DiamondTipped tools (precision drill, oil cooled cut, and cartoid grind)";
            }
            cout <<" to mill " << mold->metal << " block into " << mold->cavities << " " << mold->mold << ((mold->cavities == 1) ? " shape" : " shapes") << " with " << mold->finish << " finish.\n";
        }
    };
// Seam points - add another responder.

    MoldSource* SetupChain() {
        // Dirty, simple chain setup....
        MoldSource *chain = new Inventory();
        chain->successor = new SisterCompany();
        chain->successor->successor = new Mill();
        return chain;
    }
}

namespace decorator {		// DP 6.
// Color and additives
    class Material {
    public:
        virtual void behavior(int volume) = 0;
    };
    
    class Plastic : public Material {
        string plasticType;
    public:
        Plastic(string plastic) : plasticType(plastic) {}
        void behavior(int volume) {
            cout << plasticType << "(" << volume << ")";
        }
    };
    
    class Color : public Material {
        string colorName;
        Material *nextMaterial;
        int colorVolume;
    public:
        Color(Material *next, string color, int totalMoldVolume) : nextMaterial(next), colorName(color) {
            colorVolume = 0.1 * totalMoldVolume;
        }
        ~Color() {
            delete nextMaterial;
        }
        void behavior(int volume) {
            nextMaterial->behavior(volume - colorVolume);
            cout << " " << colorName << "(" << colorVolume << ")";
        }
    };
    
    class Additive : public Material {
        Material *nextMaterial;
        string additiveName;
        int additiveVolume;
    public:
        Additive(Material *next, string additive, string vol) : nextMaterial(next), additiveName(additive), additiveVolume(std::stoi(vol)) {}
        ~Additive() {
            delete nextMaterial;
        }
        void behavior(int volume) {
            nextMaterial->behavior(volume - additiveVolume);
            cout << " " << additiveName << "(" << additiveVolume << ")";
        }
    };
    
// Seam point - add another option.

}

namespace factory_method {	// DP 4.

// Factory methods used throughout...

}

namespace template_method {	// DP 3.

// Seam point - add another polymorphic step.
// Seam point - add another constant step.
// Seam point - convert a constant step into a polymorphic step.

}

int TagLength(char *tag) {
    string tagString(tag);
    //cout << "tag: " << tag << endl;
    //cout << "tagString: " << tagString << endl;
    //cout << "result: " << (tagString == "ModelNumber") << endl;
    if (tagString == "ModelNumber") return 2;
    if (tagString == "Country")     return 2;
    if (tagString == "Date")        return 2;
    return 0;
}

void process(map<string,string>& order) {
    // Error checking for plastic
    string plastic = order["plastic"];
    if (plastic.length() == 0) {
        cout << "  <>Unknown plastic |" << plastic << "| defaulting to 'ABS'." << endl;
        plastic = "ABS";
    }
    
    // Error checking for size
    string orderSizeStr = order["size"];
    int orderSize;
    if(orderSizeStr.length() == 0) {
        cout << "  <>No size specified, defaulting to 100.\n";
        orderSize = 100;
    } else {
        orderSize = std::stoi(orderSizeStr);
    }
    
    // Error checking for packager
    string packager = order["packager"];
    if ((packager != "Bulk")       &
        (packager != "ShrinkWrap") &
        (packager != "HardPack")   ) {
        cout << "  <>Unknown packager || defaulting to 'None'." << endl;
        packager = "Bulk";
    }
    
    //Setup the injection line
    //IJM – Mold – Conveyer Belt – Packager – Parts bin
    abstract_factory::InjectionLineFactory *InjectionLine = abstract_factory::GetInjectionLineFactory(orderSize, packager);
    abstract_factory::IJM*      ijm      = InjectionLine->GetIJM();
    abstract_factory::Mold*     mold     = InjectionLine->GetMold();
    abstract_factory::Conveyer* conveyer = InjectionLine->GetConveyer();
    observer::PartsBin*         partsBin = InjectionLine->GetPartsBin();
    observer::Packager*         packer   = InjectionLine->GetPackager();
    
    cout << "  Process order:" << endl;
    //Process order:
    //Get and install the mold
    mold->SetMoldInfo(order);
    //cout << "After SetMoldInfo" << endl;
    chain_of_resp::MoldSource *source = chain_of_resp::SetupChain();
    //cout << "After SetupChain" << endl;
    if (mold == nullptr) {
        cout << "Mold null, error" << endl;
        exit(-1);
    }
    source->GetMold(mold);
    
    //Insert tags into mold
    char *inputTags = const_cast<char*>(order["tags"].c_str());
    char *current = inputTags;
    string validTags = "";
    int tagWidth = 0;
    while(*current) {
        char aTag[60] = {0};
        sscanf(current, "%s", aTag);
        int length = 0;
        if ((length = TagLength(aTag))) {
            validTags += aTag;
            tagWidth += length;
        } else {
            cout << "    Ignoring unknown tag " << aTag << "." << endl;
        }
        current = strchr(current+1, ' ');
        if (current == nullptr) break;
        if (length) validTags += " ";
    }
    cout << "    Insert tags [" << validTags << "] of width " << tagWidth << "/20 mm, blank tag is " << (20 - tagWidth) << " mm.\n";

    
    //Load the bins
    // Start decorator with plastic type
    decorator::Material *material = new decorator::Plastic(plastic);
    cout << "    Load plastic bin with " << plastic;
    if (order["color"].length() != 0) {
        cout << " and color bin with " << order["color"];
        material = new decorator::Color(material, order["color"], mold->volume);
    }
    if (order["UVInhibiter"].length() != 0) {
        material = new decorator::Additive(material, "UVInhibiter", order["UVInhibiter"]);
    }
    if (order["AntiBacterial"].length() != 0) {
        material = new decorator::Additive(material, "AntiBacterial", order["AntiBacterial"]);
    }
    if (order["Hydrophilic"].length() != 0) {
        material = new decorator::Additive(material, "Hydrophilic", order["Hydrophilic"]);
    }
    cout << ".\n";
    cout << "      Recipe: ";
    material->behavior(mold->volume);
    cout << " = " << mold->volume << " cc.\n";
    // Hinge point, add another additive
    
    //cout << "      Recipe: <plastic>(<vol>) <color>(<vol>) <additive1>(<vol>)... = <shape vol> cc.\n";
    cout << "      Volume: " << mold->mold << "(" << mold->volume << ") * " << mold->cavities << ((mold->cavities == 1) ? " cavity = " : " cavities = ") << mold->volume * mold->cavities << " cc.\n";

    
    //Estimate the run time
    int runTime = InjectionLine->RuntimeEstimate(mold->volume);
    cout << "    Cycle " << ijm->PrintName() << " for " << plastic << " " << InjectionLine->size << " times, estimated run time = " << runTime << ((runTime == 1)? " hour" : " hours") << ".\n";

    //Run the injection cycle n times
    bridge::InjectionCycle *cycle = bridge::GetInjectionCycle(plastic);
    cycle->Run();
    
    //Simulate a full parts bin (just once)
    partsBin->Attach(ijm);
    partsBin->Attach(conveyer);
    partsBin->Attach(packer);
    partsBin->Pause();
    
    //Clean the mold
    adapter::CleaningMethod* cleaningMethod = adapter::GetCleaningMethod(plastic, mold->metal);
    cleaningMethod->Clean();
    
    delete material;
    delete source;
    delete cycle;
    delete InjectionLine;
    delete ijm;
    delete mold;
    delete conveyer;
    delete partsBin;
    delete cleaningMethod;
    cout << endl << endl << endl;
}

pair<string,string> parse(string line) {
	char key[83];
	char val[83] = {0};
//cout << "line:" << line << endl;
	sscanf(line.c_str(), "%s", key);
//cout << "key:" << key << endl;

	//char* equalSign = strchr(line.c_str(), '=');
	char* equalSign = const_cast<char*>(strchr(line.c_str(), '='));
	//char* equalSign = strchr(key, '=');
	string value = "";
	if(equalSign) {	// tags = sam i am
//cout << "equalSign:" << *equalSign << endl;
		char*	nextToken = equalSign += 2;
//cout << "nextToken:" << *nextToken << endl;
		while(nextToken) {
			sscanf(nextToken, "%s", val);
			value += val;
			nextToken = strchr(nextToken+1, ' ');
			value += " ";
		}
		value.erase(value.length()-1, 1);
	}
//cout << "(" << key << "," << value << ")" << endl;
	return make_pair(key, value);
}

map<string,string> getCompleteOrder(FILE* orderFilePtr) {
	map<string,string> order;
	char line[80+3];
	pair<string,string>	keyValue;
    //cout << "Starting loop" << endl;

	while(fgets(line, 80, orderFilePtr)) {
        //cout << "Got a line" << endl;
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
    //cout << "Order file: " << orderFile << endl;

	FILE* orderFilePtr = fopen(orderFile.c_str(), "r");
    //cout << "Opened file: " << orderFilePtr << endl;
    if (orderFilePtr == NULL) {
        cout << "File not found!" << endl;
        exit(-1);
    }
	for(;;) {	// Get and process each order.
        //cout << "Processing an order" << endl;
		map<string,string> order = getCompleteOrder(orderFilePtr);
        //cout << "Got order" << endl;
		if(order.size() == 0) {
			break;
            //cout << "Done with last order" << endl;
        }
		process(order);
        //return;
	}
}

} // final_design

#endif /* FINAL_FINAL_DESIGN_H_ */

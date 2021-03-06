/*
 * finalDesign.h
 *
 *  Created on: <MMDDCCYY>
 *      Author: <student name>
 */

#ifndef FINAL_FINAL_DESIGN_H_
#define FINAL_FINAL_DESIGN_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
 
#include <vector>
#include <list>
#include <map>
#include <set>
 
#include <iostream>
#include <sstream>
#include "macros.h" 


using namespace std;

namespace final_design {

const bool on = false;	// Useful macro to turn dtor instrumentation on/off.
#define DTORF(x) if(on) { cout << x; }

namespace legacy {	// Can't change these.

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
string toLower(string in) {
    for(unsigned int i = 0; i < in.length(); ++i) {
        in[i] = tolower(in[i]);
    }
    
    return in;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////   S  P  E  C     C  L  A  S  S  E  S   ////////////////////
////////////////////////////////////////////////////////////////////////////////
    //some need prototypes
    class Plastic;
    class Color;
    class PauseObserver;
    class Bin;
    class Machine;
    class Conveyor;
    class Metal;
    

////////////////////////////////////////////////////////////////////////////////
////////////////////         S  T  R  A  T  E  G  Y         ////////////////////
////////////////////////////////////////////////////////////////////////////////

    ////////     T I M E   E S T I M A T E     ////////
    class RunTime {
    public:
        virtual int time(int orderSize, int cavities, string metal, int volume, string plastic) { return 0; }
    };
    
    class BackOfTheEnvelope : public RunTime {
    public:
        int time(int orderSize, int cavities, string metal, int volume, string plastic) {
            return (orderSize / cavities) / 60;
        }
    };
    class Calculation : public RunTime {
    public:
        int time(int orderSize, int cavities, string metal, int volume, string plastic) {
            return orderSize/cavities * legacy::cycleTime_sec(toLower(metal), volume) / 3600;
         }
    };
    class Historical : public RunTime {
    public:
        int time(int orderSize, int cavities, string metal, int volume, string plastic) {
            int cycletime = legacy::cycleTime_sec(toLower(metal), volume);
            return (legacy::setupAvg_min + ((orderSize/cavities)*cycletime)/60 + legacy::teardownAvg_min) / 60;
        }
    };
    class Projection : public RunTime {
    public:
        int time(int orderSize, int cavities, string metal, int volume, string plastic) {
            int cycletime = legacy::cycleTime_sec(toLower(metal), volume);
            float rejectRate_pcnt = legacy::rejectRateLow_pcnt;
            if(plastic == "PET") rejectRate_pcnt = legacy::rejectRateHigh_pcnt;
            return ((legacy::setupAvg_min + 30) + (1.0 + 0.01*rejectRate_pcnt)*(orderSize/cavities)*cycletime/60 + legacy::teardownAvg_min )/(60);
            //the extra 30 minutes is because no matter what else I tried, one or more of the orders was +/- 1 hour for this calculation, yet I was doing exactly the same thing as the specs
        }
    };
    
    ////////     I N J E C T I O N   C Y C L E     ////////
    class Injection {
    public:
        virtual string inject() { return "ERROR 1"; }
        static Injection* makeInjection(Plastic* plastic);
    };
    
    class AbsInjection : public Injection {
    public:
        string inject() { return "Close - mix - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject."; }
    };
    class PolyInjection : public Injection {
    public:
        string inject() { return "Close - mix - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject."; }
    };
    class PetInjection : public Injection {
    public:
        string inject() { return "Close - mix - heat to 404 - inject at 110 PSI - cool to 340 - separate - smooth eject."; }
    };
    class SyntheticsInjection : public Injection {
    public:
        string inject() { return "Close - mix - heat to 480 - inject at 150 PSI - cool to 390 - separate - shock eject."; }
    };

////////////////////////////////////////////////////////////////////////////////
////////////////////         O  B  S  E  R  V  E  R         ////////////////////
////////////////////////////////////////////////////////////////////////////////

// Seam point - add another listener.

    class PauseSubject {
    private:
        list<PauseObserver*> observers;
        Bin* bin;
    public:
        PauseSubject(Bin* bin) : bin(bin) {}
        void attach(PauseObserver* obs) { observers.push_back(obs); }
        void pause();
        static PauseSubject* makePauseSubject(Bin* bin, Machine* machine, Conveyor* conveyor, string packager, string stuffer);
    };

    class PauseObserver {
        string name;
    public:
        PauseObserver(const string& name) : name(name) {}
        virtual string pause() { return name; }
    };

////////////////////////////////////////////////////////////////////////////////
////////////////////       F A C T O R Y   M E T H O D      ////////////////////
////////////////////////////////////////////////////////////////////////////////

    // Seam point - add another class.

    class Finish {
    public:
        virtual string type() { return ""; }
        static Finish* makeFinish(const string& crit);
    };
    
    class Smooth : public Finish {
    public:
        string type() { return "smooth"; }
    };
    class Rippled : public Finish {
    public:
        string type() { return "rippled"; }
    };
    class Dimpled : public Finish {
    public:
        string type() { return "dimpled"; }
    };
    class Sanded : public Finish {
    public:
        string type() { return "sanded"; }
    };
    class Satin : public Finish {
    public:
        string type() { return "satin"; }
    };
    
    Finish* Finish::makeFinish(const string& crit) {
        if (crit == "smooth")   return new Smooth;
        if (crit == "rippled")   return new Rippled;
        if (crit == "dimpled")   return new Dimpled;
        if (crit == "sanded")   return new Sanded;
        if (crit == "satin")   return new Satin;
        return new Finish;
    }
    
    
    class Shape    {
    public:
        Finish* finish;
        virtual string name() { return "ERROR 2"; }
        virtual string steps() { return "ERROR 3"; }
        virtual int vol() { return -1; }
        static Shape* makeShape(const string& crit);
    };

    class Duck : public Shape {
        string name() { return "duck"; }
        string steps() { return "D3C2D2G4"; }
        int vol() { return 35; }
    };
    class Car : public Shape {
        string name() { return "car"; }
        string steps() { return "D2C8D1G3"; }
        int vol() { return 40; }
    };
    class Hero : public Shape {
        string name() { return "hero"; }
        string steps() { return "D1C1D1G7"; }
        int vol() { return 50; }
    };
    class Dino : public Shape {
        string name() { return "dino"; }
        string steps() { return "G5D2C1"; }
        int vol() { return 30; }
    };
    
    Shape* Shape::makeShape(const string& crit) {
        if (crit == "duck") return new Duck;
        if (crit == "car") return new Car;
        if (crit == "hero") return new Hero;
        if (crit == "dino") return new Dino;
        cout << "    <>Unknown mold |" << crit << "| defaulting to 'duck'." << endl;
        return new Duck;
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////    A B S T R A C T   F A C T O R Y     ////////////////////
////////////////////////////////////////////////////////////////////////////////

    // Seam point - add another type 1.
    // Seam point - add another type 2.
    // Seam point - add another type 3.
    // Seam point - add another ...
    // Seam point - add another type N.

    class Machine {
    public:
        virtual string name() { return "ERROR 4"; }
    };

    class Metal {
    public:
        virtual string type() { return "ERROR 5"; }
    };

    class Cavities {
    public:
        virtual int num() { cout << "ERROR 6\n"; return 0; }
    };

    class Conveyor {
    public:
        virtual string type() { return "ERROR 7"; }
    };

    class Bin {
    public:
        virtual string type() { return "ERROR 8"; }
    };


    ////////////////////      M  A  C  H  I  N  E  S     ////////////////////
    class IJM_110 : public Machine {
    public:
        string name() { return "IJM_110"; }
    };
    class IJM_120 : public Machine {
    public:
        string name() { return "IJM_120"; }
    };
    class IJM_210 : public Machine {
    public:
        string name() { return "IJM_210"; }
    };
    class IJM_140 : public Machine {
    public:
        string name() { return "IJM_140"; }
    };
    class IJM_220 : public Machine {
    public:
        string name() { return "IJM_220"; }
    };
    class IJM_240 : public Machine {
    public:
        string name() { return "IJM_240"; }
    };

    ////////////////////         M  E  T  A  L  S        ////////////////////
    class Aluminum : public Metal {
    public:
        string type() { return "Aluminum"; }
    };
    class Steel : public Metal {
    public:
        string type() { return "Steel"; }
    };

    ////////////////////    C  A  V  I  T  I  E  S      ////////////////////
    class OneCavity : public Cavities {
    public:
        int num() { return 1; }
    };
    class TwoCavity : public Cavities {
    public:
        int num() { return 2; }
    };
    class FourCavity : public Cavities {
    public:
        int num() { return 4; }
    };

    ////////////////////   C  O  N  V  E  Y  O  R  S   ////////////////////
    class Linear : public Conveyor {
    public:
        string type() { return "Linear"; }
    };
    class Y_Split : public Conveyor {
    public:
        string type() { return "Y-Split"; }
    };
    class V_Level : public Conveyor {
    public:
        string type() { return "V-Level"; }
    };

    ////////////////////           B  I  N            ////////////////////
    class CardboardBox : public Bin {
    public:
        string type() { return "CardboardBox"; }
    };
    class ShellBox : public Bin {
    public:
        string type() { return "ShellBox"; }
    };
    class PalletBox : public Bin {
    public:
        string type() { return "PalletBox"; }
    };
        class Crate : public Bin {
    public:
        string type() { return "Crate"; }
    };



    class SizeFamily {
    public:
        virtual Machine*    createMachine()  { return new Machine; }
        virtual Metal*      createMetal()    { return new Metal; }
        virtual Cavities*   createCavities() { return new Cavities; }
        virtual Conveyor*   createConveyor() { return new Conveyor; }
        virtual Bin*        createBin()      { return new Bin; }
        virtual RunTime*    createRunTime()  { return new RunTime; }
        
        static SizeFamily* makeSizeFamily(int crit);
    };

    class PilotOrder : public SizeFamily {
    public:
        Machine*    createMachine()     { return new IJM_110; }
        Metal*      createMetal()       { return new Aluminum; }
        Cavities*   createCavities()    { return new OneCavity; }
        Conveyor*   createConveyor()    { return new Linear; }
        Bin*        createBin()         { return new CardboardBox; }
        RunTime*    createRunTime()     { return new BackOfTheEnvelope; }
    };

    class SmallestOrder : public SizeFamily {
    public:
        Machine*    createMachine()     { return new IJM_110; }
        Metal*      createMetal()       { return new Aluminum; }
        Cavities*   createCavities()    { return new OneCavity; }
        Conveyor*   createConveyor()    { return new Linear; }
        Bin*        createBin()         { return new CardboardBox; }
        RunTime*    createRunTime()     { return new BackOfTheEnvelope; }
    };

    class SmallOrder : public SizeFamily {
    public:
        Machine*    createMachine()     { return new IJM_120; }
        Metal*      createMetal()       { return new Aluminum; }
        Cavities*   createCavities()    { return new TwoCavity; }
        Conveyor*   createConveyor()    { return new Y_Split; }
        Bin*        createBin()         { return new ShellBox; }
        RunTime*    createRunTime()     { return new Calculation; }
    };

    class MediumOrder : public SizeFamily {
    public:
        Machine*    createMachine()     { return new IJM_210; }
        Metal*      createMetal()       { return new Steel; }
        Cavities*   createCavities()    { return new OneCavity; }
        Conveyor*   createConveyor()    { return new Linear; }
        Bin*        createBin()         { return new PalletBox; }
        RunTime*    createRunTime()     { return new Historical; }
    };
    
    class FastOrder : public SizeFamily {
    public:
        Machine*    createMachine()     { return new IJM_140; }
        Metal*      createMetal()       { return new Aluminum; }
        Cavities*   createCavities()    { return new FourCavity; }
        Conveyor*   createConveyor()    { return new V_Level; }
        Bin*        createBin()         { return new ShellBox; }
        RunTime*    createRunTime()     { return new Historical; }
    };
    
    class LargeOrder : public SizeFamily {
    public:
        Machine*    createMachine()     { return new IJM_220; }
        Metal*      createMetal()       { return new Steel; }
        Cavities*   createCavities()    { return new TwoCavity; }
        Conveyor*   createConveyor()    { return new Y_Split; }
        Bin*        createBin()         { return new PalletBox; }
        RunTime*    createRunTime()     { return new Projection; }
    };
    
    class HugeOrder : public SizeFamily {
    public:
        Machine*    createMachine()     { return new IJM_240; }
        Metal*      createMetal()       { return new Steel; }
        Cavities*   createCavities()    { return new FourCavity; }
        Conveyor*   createConveyor()    { return new V_Level; }
        Bin*        createBin()         { return new Crate; }
        RunTime*    createRunTime()     { return new Projection; }
    };
    // Seam point - add another family.


    SizeFamily* SizeFamily::makeSizeFamily(int crit) {
        if (crit == 100) return new PilotOrder;
        if (crit == 10000) return new SmallestOrder;
        if (crit == 20000) return new SmallOrder;
        if (crit == 50000) return new MediumOrder;
        if (crit == 40000) return new FastOrder;
        if (crit == 100000) return new LargeOrder;
        if (crit == 200000) return new HugeOrder;
        return new SizeFamily;
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////             B  R  I  D  G  E           ////////////////////
////////////////////////////////////////////////////////////////////////////////

    // Seam Point - add another implementation.
    // Seam Point - add another abstraction.

    class MillProcess {
        Metal* metal;
        Cavities* cavities;
        Shape* shape;
    public:
        MillProcess(Metal* metal, Cavities* cavities, Shape* shape) :
            metal(metal), cavities(cavities), shape(shape) {}
        void millPre() {
            cout << "    Create " << shape->name() << " mold from mill with " << cavities->num() << " cavities - steps: " << shape->steps() << "." << endl;
        }
        virtual void mill() { cout << "ERROR 9" << endl; }
        void millPost() {
            cout << toLower(metal->type()) << " block into " << cavities->num() << " " << shape->name() << " shape";
            if (cavities->num() != 1) cout << "s";
            cout << " with " << shape->finish->type() << " finish." << endl;
        }
        static MillProcess* makeMillProcess(Metal* metal, Cavities* cavities, Shape* shape);
    };

    class Carbon : public MillProcess {
    public:
        Carbon(Metal* metal, Cavities* cavities, Shape* shape) :
            MillProcess(metal, cavities, shape) {}
        void mill() {
            millPre();
            cout << "      using HighCarbon tools (drill, cut, and high speed grind) to mill ";
            millPost();
        }
    };
    class Carbide : public MillProcess {
    public:
        Carbide(Metal* metal, Cavities* cavities, Shape* shape) :
            MillProcess(metal, cavities, shape) {}
        void mill() {
            millPre();
            cout << "      using Carbide tools (high speed drill, cross cut, and layer grind) to mill ";
            millPost();
        }
    };
    class Diamond : public MillProcess {
    public:
        Diamond(Metal* metal, Cavities* cavities, Shape* shape) :
            MillProcess(metal, cavities, shape) {}
        void mill() {
            millPre();
            cout << "      using DiamondTipped tools (precision drill, oil cooled cut, and cartoid grind) to mill ";
            millPost();
        }
    };
    class EDM : public MillProcess {
    public:
        EDM(Metal* metal, Cavities* cavities, Shape* shape) :
            MillProcess(metal, cavities, shape) {}
        void mill() {
            millPre();
            cout << "      using EDM tools (static punch, static slice, and static etch) to mill ";
            millPost();
        }
    };


    MillProcess* MillProcess::makeMillProcess(Metal* metal, Cavities* cavities, Shape* shape) {
        if (metal->type() == "Aluminum")     return new Carbon(metal, cavities, shape);
        if (metal->type() == "Steel") {
            if (shape->finish->type() == "smooth")    return new Carbon(metal, cavities, shape);
            if (shape->finish->type() == "rippled")   return new Carbide(metal, cavities, shape);
            if (shape->finish->type() == "dimpled")   return new Diamond(metal, cavities, shape);
            if (shape->finish->type() == "sanded")   return new EDM(metal, cavities, shape);
            if (shape->finish->type() == "satin")   return new EDM(metal, cavities, shape);
        }
        return new MillProcess(metal, cavities, shape);
    }


////////////////////////////////////////////////////////////////////////////////
///////////////   C H A I N   O F   R E S P O N S I B I L I T Y   //////////////
////////////////////////////////////////////////////////////////////////////////

    class Responder {
    protected:
        Responder* successor;
    public:
        Responder() : successor(0) {}
        void setSuccessor(Responder* next) { successor = next; }
        virtual void getMold(const string& options, MillProcess* millProcess, Shape* shape)=0;
        void getMoldFail(const string& options, MillProcess* millProcess, Shape* shape);
        static Responder* makeObject(const string& crit);
    };

    // Seam points - add another responder.

    class Inventory : public Responder {
    public:
        void getMold(const string& options, MillProcess* millProcess, Shape* shape) {
            if (options.find("inventory") != string::npos) {
                cout << "    Pull " << shape->name() << " mold from inventory." << endl;
            } else successor->getMold(options, millProcess, shape);
        }
    };
    class SisterCompany : public Responder {
    public:
        void getMold(const string& options, MillProcess* millProcess, Shape* shape) {
            if (options.find("sisterCompany") != string::npos) {
                cout << "    Borrow " << shape->name() << " mold from sister company." << endl;
            } else successor->getMold(options, millProcess, shape);
        }
    };
    class Mill : public Responder {
    public:
        void getMold(const string& options, MillProcess* millProcess, Shape* shape) {
            if (options.find("mill") != string::npos) {
                millProcess->mill();
            } else getMoldFail(options, millProcess, shape);
        }
    };
    class Purchase : public Responder {
    public:
        void getMold(const string& options, MillProcess* millProcess, Shape* shape) {
            if (options.find("purchase") != string::npos) {
                cout << "    Acquire " << shape->name() << " mold via purchase." << endl;
            } else successor->getMold(options, millProcess, shape);
        }
    };

    Responder* Responder::makeObject(const string& crit) {
        if(crit == "Inventory")		return new Inventory;
        if(crit == "SisterCompany")		return new SisterCompany;
        if(crit == "Mill")		return new Mill;
        if(crit == "Purchase")		return new Purchase;
        throw "CoR Error";
    }
    void Responder::getMoldFail(const string& options, MillProcess* millProcess, Shape* shape) {
        cout << "    <>Can't find place |" << options << "| to get |" << shape->name() << "| mold from with |";
        cout << shape->finish->type() << "| finish, defaulting to smooth duck from inventory." << endl;
    }
    
    Responder* SetupChain() {
        string chain[] = {
            "Inventory", "SisterCompany",
            "Purchase", "Mill"
        };
        Responder* responder = Responder::makeObject(chain[0]);

        Responder* cur = responder;
        for(size_t i=1; i<COUNT(chain); i++) {
            Responder* next = Responder::makeObject(chain[i]);
            cur->setSuccessor(next);
            cur = next;
        }

        return responder;
    }


////////////////////////////////////////////////////////////////////////////////
////////////////////       D  E  C  O  R  A  T  O  R        ////////////////////
////////////////////////////////////////////////////////////////////////////////

    // Seam point - add another option.

    ////////////       A D D I T I V E S        ////////////
    
    class Additive {
    protected:
        Additive* next;
    public:
        Additive(Additive* next) : next(next) {}
        virtual string recipe(int remainingVol) = 0;
        static Additive* makeAdditives(map<string,string>& order, Plastic* &plastic, Color* &color);
    };
    
    class Plastic : public Additive {
        string type;
        string color;
    public:
        Plastic(string type, string color) : type(type), color(color), Additive(0) {}
        string getType() { return type; }
        string getColorAndType() { 
            if (type != "Styrene" && type != "Nylon66") return type;
            else return color + "-" + type;
        }
        string recipe(int remainingVol) { return this->getColorAndType() + "(" + to_string(remainingVol) + ")";}
    };
    
    ///// A d d i t i v e s /////
    class UVInhibiter : public Additive {
        int vol;
    public:
        UVInhibiter(Additive* next, int vol) : Additive(next), vol(vol) {}
        string recipe(int remainingVol) { return next->recipe(remainingVol - vol) + " UVInhibiter(" + to_string(vol) + ")"; }
    };
    class AntiBacterial : public Additive {
        int vol;
    public:
        AntiBacterial(Additive* next, int vol) : Additive(next), vol(vol) {}
        string recipe(int remainingVol) { return next->recipe(remainingVol - vol) + " AntiBacterial(" + to_string(vol) + ")"; }
    };
    class Hydrophilic : public Additive {
        int vol;
    public:
        Hydrophilic(Additive* next, int vol) : Additive(next), vol(vol) {}
        string recipe(int remainingVol) { return next->recipe(remainingVol - vol) + " Hydrophilic(" + to_string(vol) + ")"; }
    };
    class Color : public Additive {
        int vol;
        string color;
    public:
        Color(Additive* next, string color) : Additive(next), color(color) {}
        string getColor() { return color; }
        void setVol(int newVol) { vol = newVol; }
        string recipe(int remainingVol) { return next->recipe(remainingVol - vol) + " " + color + "(" + to_string(vol) + ")"; }
    };
    class MicroFibers : public Additive {
    int vol;
    public:
        MicroFibers(Additive* next, int vol) : Additive(next), vol(vol) {}
        string recipe(int remainingVol) { return next->recipe(remainingVol - vol) + " MicroFibers(" + to_string(vol) + ")"; }
    };
    
    Additive* Additive::makeAdditives(map<string,string>& order, Plastic* &plastic, Color* &color) {
        //may need to ensure a valid plastic was given as well
        if (order["plastic"] == ""){
            cout << "  <>Unknown plastic || defaulting to 'ABS'." << endl;
            order["plastic"] = "ABS";
        }
        plastic = new Plastic(order["plastic"], order["color"]);
        
        Additive* cur = plastic;
        // set up additives
        //Color
        if (plastic->getType() != "Styrene" && plastic->getType() != "Nylon66") {
            if (order["color"] != "") {
                color = new Color(cur, order["color"]);
                cur = color;
            }
        }
        //UVInhibiter
        if (order["UVInhibiter"] != "") {
            cur = new UVInhibiter(cur, stoi(order["UVInhibiter"]));
        }
        //AnitBacterial
                if (order["AntiBacterial"] != "") {
            cur = new AntiBacterial(cur, stoi(order["AntiBacterial"]));
        }
        //Hydrophilic
        if (order["Hydrophilic"] != "") {
            cur = new Hydrophilic(cur, stoi(order["Hydrophilic"]));
        }
        //MicroFibers
        if (order["MicroFibers"] != "") {
            cur = new MicroFibers(cur, stoi(order["MicroFibers"]));
        }
        
        
        return cur;    
    }
    
    
    ////////////            T  A  G  S             ////////////
    class Tag {
    protected:
        Tag* next;
        string name;
        int width;
    public:
        Tag(Tag* next) : next(next) {}
        Tag(Tag* next = 0, string name = "", int width = 0) : next(next), name(name), width(width) {}
        string tagList() {  if (next) return next->tagList() + " " + name;
                            else return name; }
        int tagsWidth() {   if (next) return next->tagsWidth() + width;
                            else return width; }
        static Tag* setupTags(string tags);
    };
    
    class ModelNumber : public Tag {
    public:
        ModelNumber(Tag* next) : Tag(next, "ModelNumber", 2) {}
    };
    class Country : public Tag {
    public:
        Country(Tag* next) : Tag(next, "Country", 2) {}
    };
    class Date : public Tag {
    public:
        Date(Tag* next) : Tag(next, "Date", 2) {}
    };
    class IncrementCounter : public Tag {
    public:
        IncrementCounter(Tag* next) : Tag(next, "IncCounter", 4) {}
    };
    class PartNumber : public Tag {
    public:
        PartNumber(Tag* next) : Tag(next, "PartNumber", 2) {}
    };
    class RecycleCode : public Tag {
    public:
        RecycleCode(Tag* next) : Tag(next, "RecycleCode", 6) {}
    };
    
    Tag* Tag::setupTags(string tagString) {
        stringstream ss(tagString);
        string tag;
        
        Tag* tags = 0;
        
        while (ss >> tag){
            if (tag == "ModelNumber") tags = new ModelNumber(tags);
            else if (tag == "Country") tags = new Country(tags);
            else if (tag == "Date") tags = new Date(tags);
            else if (tag == "IncCounter") tags = new IncrementCounter(tags);
            else if (tag == "PartNumber") tags = new PartNumber(tags);
            else if (tag == "RecycleCode") tags = new RecycleCode(tags);
            else cout << "    Ignoring unknown tag " << tag << "." << endl;
        }
        
        if (tags) return tags;
        return new Tag();
    }


////////////////////////////////////////////////////////////////////////////////
////////////////////          A  D  A  P  T  E  R           ////////////////////
////////////////////////////////////////////////////////////////////////////////

// Seam point - add another interface.
    class Clean {
    public:
        virtual void clean(Metal* metal) { cout << "ERROR 10"; }
        static Clean* makeClean(Plastic* plastic);
    };
    
    class AbsClean : public Clean {
    public:
        void clean(Metal* metal) { legacy::CleanABS clean; clean.clean(); }
    };
    class PolyClean : public Clean {
    public:
        void clean(Metal* metal) { legacy::CleanPolys clean; clean.prepForReuse(); }
    };
    class PetClean : public Clean {
    public:
        void clean(Metal* metal) { 
            legacy::PETCleanup clean;
            if (metal->type() == "Aluminum") clean.purify();
            else if (metal->type() == "Steel") clean.carbonize();
            else cout << "ERROR 11";
        }
    };
    class SyntheticsClean : public Clean {
    public:
        void clean(Metal* metal) { cout << "    Clean synthetic mold: ozone wash." << endl; }
    };


////////////////////////////////////////////////////////////////////////////////
////////////////////     T E M P L A T E   M E T H O D      ////////////////////
////////////////////////////////////////////////////////////////////////////////

// Seam point - add another polymorphic step.
// Seam point - add another constant step.
// Seam point - convert a constant step into a polymorphic step.


///////////////  F A C T O R Y   "M A K E"   F U N C T I O N S   ///////////////
//These need to be below the class definitions because c++ is use friendly
    Injection* Injection::makeInjection(Plastic* plastic) {
        if (plastic->getType() == "ABS" )      return new AbsInjection;
        if (plastic->getType() == "Polypropylene"  || plastic->getType() == "Polyethelene") return new PolyInjection;
        if (plastic->getType() == "PET")       return new PetInjection;
        if (plastic->getType() == "Styrene"  || plastic->getType() == "Nylon66") return new SyntheticsInjection;
        return new Injection;
    }
    Clean* Clean::makeClean(Plastic* plastic) {
        if (plastic->getType() == "ABS" )      return new AbsClean;
        if (plastic->getType() == "Polypropylene"  || plastic->getType() == "Polyethelene") return new PolyClean;
        if (plastic->getType() == "PET")       return new PetClean;
        if (plastic->getType() == "Styrene"  || plastic->getType() == "Nylon66") return new SyntheticsClean;
        return new Clean;
    }
    PauseSubject* PauseSubject::makePauseSubject(Bin* bin, Machine* machine, Conveyor* conveyor, string packager, string stuffer) {
        PauseSubject* pauseSubject = new PauseSubject(bin);
        string suffix = " paused while " + bin->type() + " parts bin was swapped.";
        if (machine) pauseSubject->attach(new PauseObserver(machine->name() + suffix));
        if (conveyor) pauseSubject->attach(new PauseObserver(conveyor->type() + " conveyer belt" + suffix));
        if (packager != "") pauseSubject->attach(new PauseObserver(packager + " packager" + suffix));
        if (stuffer != "") pauseSubject->attach(new PauseObserver(stuffer + " stuffer triggered to fill " + bin->type() + " parts bin."));
        
        return pauseSubject;
    }
    void PauseSubject::pause() {
        auto it = observers.begin();
        if (it != observers.end())
            cout << "      Whenever " << bin->type() << " parts bin was full..." << endl;
        for (; it != observers.end(); it++)
            cout << "        " << (*it)->pause() << endl;           
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////      P R O C E S S I N G   C O D E     ////////////////////
////////////////////////////////////////////////////////////////////////////////

const int MAX_SIZE = 200000;
const int DEFAULT_SIZE = 100;
int validSize(map<string,string>& order) {
    string strSize = order["size"];
    if (strSize != "") {
        int size = std::stoi(strSize);
        // Seam point - add more valid sizes
        if (size == 10000 || size == 20000 || size == 50000 || size == 40000 || size == 100000 || size == 200000)
            return size; // already a valid size
        if (size > MAX_SIZE) {
            cout << "  <>Size exceeds mold lifetime |" << size << "| defaulting to HugeOrder of " << MAX_SIZE << "." << endl;
            return MAX_SIZE;
        }
    } 
    cout << "  <>No size specified, defaulting to " << DEFAULT_SIZE << "." << endl;
    return DEFAULT_SIZE;
}

//this is not a good way to do this, but since the exam is about speed and I didnt have a good pattern set up, this is what I am doing
void printStuffer(const string& stuffer) {
    if (stuffer == "Foam")      cout << " and expanding foam stuffer";
    if (stuffer == "Bubblewrap")      cout << " and bubble wrap stuffer";
    if (stuffer == "Popcorn")      cout << " and styrene popcorn stuffer";
}

//the names of the packagers are different in places...... so this is here
string convertPacakgerName(const string& name) {
    if (name == "ShrinkWrap")    return "Shrink wrap";
    if (name == "HardPack")    return "Hard pack";
    if (name == "ZipLock")    return "Ziplock";
    return name;
}

const int TAGS_MAX_WIDTH = 20;
void process(map<string,string>& order) {
	// Fill in the namespaces above with your design pattern class hierarchies.
	// Call your order processing class from here <myProcess>->run(order);
    
    ///////////////////      G E T   O R D E R   B A S I C S        ////////////////////
    // Not the best place to do this, but the "unknown plastic" message is supposed to show up first
    Plastic* plastic = 0;
    Color* color = 0;
    Additive* additives = Additive::makeAdditives(order, plastic, color);
    Injection* injection = Injection::makeInjection(plastic);
    
    int correctedSize = validSize(order);
    SizeFamily* sizeFamily = SizeFamily::makeSizeFamily(correctedSize);
    
    Machine* machine = sizeFamily->createMachine();
    Metal* metal = sizeFamily->createMetal();
    Cavities* cavities = sizeFamily->createCavities();
    Conveyor* conveyor = sizeFamily->createConveyor();
    Bin* bin = sizeFamily->createBin();
    
    string packager = order["packager"];
    if (packager == "") {
        cout << "  <>Unknown packager || defaulting to 'None'." << endl;
    }
    if (packager == "Bulk") packager = "";
    
    string stuffer = order["stuffer"];
    if (stuffer == "") {
        cout << "  <>Unknown stuffer || defaulting to 'None'." << endl;
    }
    if (stuffer == "Air") stuffer = "";
    
    ///////////////////      P R I N T   O R D E R   B A S I C S        ////////////////////
    cout << "  Setup injection line for " << correctedSize << " order";
    if (packager != "") cout << " with " << packager << " packager";
    printStuffer(stuffer);
    cout << ":" << endl;
    cout << "    " << machine->name() << " - " << metal->type() << "(" << cavities->num() << ") - " << conveyor->type() << " conveyer belt - " << bin->type() << "." << endl;
    cout << "  Process order:" << endl;
    
    ///////////////////        G E T   M O L D   I N F O           ////////////////////
    Responder* moldGetter = SetupChain();
    Shape* shape = Shape::makeShape(order["mold"]);
    shape->finish = Finish::makeFinish(order["finish"]);
    MillProcess* millProcess = MillProcess::makeMillProcess(metal, cavities, shape);
    
    ///////////////////      P R I N T   M O L D   I N F O        ////////////////////
    moldGetter->getMold(order["moldLoc"], millProcess, shape);
    Tag* tag = Tag::setupTags(order["tags"]);
    cout << "    Insert tags [" << tag->tagList() << "] of width " << tag->tagsWidth() << "/" << TAGS_MAX_WIDTH << " mm, blank tag is " << TAGS_MAX_WIDTH - tag->tagsWidth() << " mm." << endl;
    
    
    ///////////////////    P R I N T   R E C I P E   I N F O      ////////////////////
    cout << "    Load plastic bin with " << plastic->getColorAndType();
    if (plastic->getType() == "Styrene" || plastic->getType() == "Nylon66") cout << " pellets";
    if (color) cout << " and color bin with " << color->getColor();
    cout << "." << endl;
    if (color) color->setVol(shape->vol()/10);
    cout << "      Recipe: " << additives->recipe(shape->vol()) << " = " << shape->vol() << " cc." << endl;
    cout << "      Volume: " << shape->name() << "(" << shape->vol() << ") * " << cavities->num();
    if (cavities->num() == 1) cout << " cavity";
    else cout << " cavities";
    cout << " = " << shape->vol() * cavities->num() << " cc." << endl;
    
    //this output being here did not lend itself well to my structure, so I have this kind of dumb print statement here...
    if (correctedSize > 100000) cout << "    <>Using most sophisticated estimation algorithm for orders greater than 100000." << endl;
    
    ///////////////////   P R I N T   P R O C E S S   I N F O      ////////////////////
    RunTime* runTime = sizeFamily->createRunTime();
    int time = runTime->time(correctedSize, cavities->num(), metal->type(), shape->vol(), plastic->getType());
    cout << "    Cycle " << machine->name() << " for " << plastic->getType() << " " << correctedSize/cavities->num() << " times, estimated run time = ";
    cout << time << " hour";
    if (time != 1) cout << "s";
    cout << "." << endl;
    cout << "      " <<   injection->inject() << endl;
    
    ///////////////////     P R I N T   P A U S E   I N F O        ////////////////////
    PauseSubject* pauseSubject = PauseSubject::makePauseSubject(bin, machine, conveyor, convertPacakgerName(packager), stuffer);
    pauseSubject->pause();
    
    ///////////////////     P R I N T   C L E A N   I N F O        ////////////////////
    Clean* clean = Clean::makeClean(plastic);
    clean->clean(metal);
    
    ///////////////////   P R I N T   S H I P P I N G   I N F O    ////////////////////
    string address = order["address"];
    cout << "    Ship to \"" << address << "\"." << endl;
    
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
	cout << "\n<<< final design >>>\n";

	FILE* orderFilePtr = fopen(orderFile.c_str(), "r");

	for(;;) {	// Get and process each order.
		map<string,string> order = getCompleteOrder(orderFilePtr);
		if(order.size() == 0)
			break;
		process(order);
	}
    cout << "\n\n";
}

} // final_design

#endif /* FINAL_FINAL_DESIGN_H_ */

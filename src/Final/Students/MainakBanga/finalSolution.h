/*
 * finalDesign.h
 *
 *  Created on: <MMDDCCYY>
 *      Author: <student name>
 */

#ifndef FINAL_FINAL_DESIGN_H_
#define FINAL_FINAL_DESIGN_H_

namespace final_design {

const bool on = true;	// Useful macro to turn dtor instrumentation on/off.
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

void defaulting(map<string,string>& order, const string& option, string def="simulation") {
	cout << "  <>Unknown " << option << " |";
	cout << order[option] << "|";
	order[option] = def;
	cout << " defaulting to '" << order[option] << "'.\n";
}

}

namespace adapter {			// DP 2.

/******************************************************************************/
/* Class Clean
/******************************************************************************/

class Clean {
    public :
        virtual ~Clean() {}
        virtual void cleanMolds(string &moldMetal) {}
        static Clean *makeObject(const string &plasticName);
};

class ABSClean : public Clean {
    private :
        legacy_classes::CleanABS *cleanMethod;

    public :
        ABSClean() { cleanMethod = new legacy_classes::CleanABS; };
        ~ABSClean() { delete cleanMethod; }
        void cleanMolds(string &moldMetal) { 
            cleanMethod->clean();
        }
};

class PolysClean : public Clean {
    private :
        legacy_classes::CleanPolys *cleanMethod;

    public :
        PolysClean() { cleanMethod = new legacy_classes::CleanPolys; };
        ~PolysClean() { delete cleanMethod; }
        void cleanMolds(string &moldMetal) {
           cleanMethod->prepForReuse();
        }
};

class PETClean : public Clean {
    private :
        legacy_classes::PETCleanup *cleanMethod;

    public :
        PETClean() { cleanMethod = new legacy_classes::PETCleanup; };
        ~PETClean() { delete cleanMethod; }
        void cleanMolds(string &moldMetal) { 
            if(moldMetal == "Aluminum") {
                cleanMethod->purify();
            } 
            if(moldMetal == "Steel") {
                cleanMethod->carbonize();
            }
        }
};

class SyntheticClean : public Clean {
    public :
        SyntheticClean() {}
        ~SyntheticClean() {}
        void cleanMolds(string &moldMetal) { cout << "    Clean synthetic mold: ozone wash.\n"; }
};

Clean *Clean::makeObject(const string &plasticName) {
    if(plasticName == "ABS") return new ABSClean;
    if(plasticName == "Polypropylene" or plasticName == "Polyethelene") return new PolysClean;
    if(plasticName == "PET") return new PETClean;
    else return new SyntheticClean;
}
// Seam point - add another interface.

}

namespace strategy {		// DP 1.

/******************************************************************************/
/* Class Plastic
/******************************************************************************/

class Plastic {
    protected : 
        string plasticName;
        //int plasticVol;
    public : 
        Plastic() {
            cout << "  <>Unknown plastic || defaulting to 'ABS'.\n";
            plasticName = "ABS";
          //  plasticVol = 35;
        }
        Plastic(const string &pName /*,int plasticVolume*/){ 
            plasticName = pName;
            //plasticVol = plasticVolume;
        }
        //int getPlasticVol() {
        //    return plasticVol;
        //}
        string getPlasticName() { return plasticName; }
        static Plastic *makeObject(const string &);
};


class ABS : public Plastic {
    public :
        ABS() : Plastic("ABS"/*, 30*/) {}
};

class Polypropylene : public Plastic {
    public :
        Polypropylene() : Plastic("Polypropylene" /*,31*/) {}
};

class Polyethelene : public Plastic {
    public :
        Polyethelene() : Plastic("Polyethelene" /*,45*/) {}
};

class PET : public Plastic {
    public :
        PET() : Plastic("PET" /*,45*/) {}
};

class Styrene : public Plastic {
    public :
        Styrene() : Plastic("Styrene") {}
};
 
class Nylon66 : public Plastic {
    public :
        Nylon66() : Plastic("Nylon66") {}
};

Plastic *Plastic::makeObject(const string &plasticName) {
    // Can be enumed
    if(plasticName == "ABS") return new ABS();
    if(plasticName == "Polypropylene") return new Polypropylene();
    if(plasticName == "Polyethelene") return new Polyethelene();
    if(plasticName == "PET") return new PET();
    if(plasticName == "Styrene") return new Styrene();
    if(plasticName == "Nylon66") return new Nylon66();
    return new Plastic();
}

/******************************************************************************/
/* Class OrderSize
/******************************************************************************/

class OrderSize {
    private :
        int size;
    public :
        OrderSize() {
            cout << "  <>No size specified, defaulting to 100.\n";
            size = 100;
        }
        OrderSize(string &orderSize) {
            size = stoi(orderSize);
            if(size > 200000) {
                cout << "  <>Size exceeds mold lifetime |" << size << "| defaulting to HugeOrder of 200000.\n";
                size = 200000;
            }
        }
        int getOrderSize(){ return size; }

        static OrderSize *makeObject(const string &orderSize);
};

OrderSize *OrderSize::makeObject(const string &orderSize) {
    if(orderSize == "unknown") return new OrderSize();
    else return new OrderSize(const_cast<string &>(orderSize));
};

/******************************************************************************/
/* Class Packager
/******************************************************************************/

class Packager {
    private :
        string pack;
    public :
        Packager() {
            cout << "  <>Unknown packager || defaulting to 'None'.\n";
            pack = "Bulk";
        }
        Packager(string &packType) {
            pack = packType;
        }
        string &getPackager() { return pack; }

        static Packager *makeObject(const string &);
};

Packager *Packager::makeObject(const string &packager) {
    if(packager == "unknown") return new Packager();
    else return new Packager(const_cast<string &>(packager));
}

/******************************************************************************/
/* Class IJM
/******************************************************************************/

class IJM {
    private :
        string IJMName;
    public :
        IJM() {}
        IJM(const string &name) : IJMName(name) {}

        string &getIJMName() { return IJMName; }

        static IJM *makeObject(int);
};

IJM *IJM::makeObject(int orderSize) {
    if(orderSize <= 10000) return new IJM("IJM_110");
    if(orderSize <= 20000) return new IJM("IJM_120");
    if(orderSize <= 40000) return new IJM("IJM_140");
    if(orderSize <= 50000) return new IJM("IJM_210");
    if(orderSize <= 100000) return new IJM("IJM_220");
    if(orderSize <= 200000) return new IJM("IJM_240");
    else throw "OOPS!";
};

/******************************************************************************/
/* Class Metal
/******************************************************************************/

class Metal {
    private :
        string metalName;
    public :
        Metal() {}
        Metal(const string &name) : metalName(name) {}

        string &getMetalName() { return metalName; }

        static Metal *makeObject(int);
};

Metal *Metal::makeObject(int orderSize) {
    if(orderSize <= 40000) return new Metal("Aluminum");
    if(orderSize <= 200000) return new Metal("Steel");
    else throw "OOPS!";
};

/******************************************************************************/
/* Class Cavities
/******************************************************************************/

class Cavities {
    private :
        int cavityCount;
    public :
        Cavities() {}
        Cavities(int count) : cavityCount(count) {}

        int getCavityCount() { return cavityCount; }

        static Cavities *makeObject(int);
};

Cavities *Cavities::makeObject(int orderSize) {
    if(orderSize <= 10000) return new Cavities(1);
    if(orderSize <= 20000) return new Cavities(2);
    if(orderSize <= 40000) return new Cavities(4);
    if(orderSize <= 50000) return new Cavities(1);
    if(orderSize <= 100000) return new Cavities(2);
    if(orderSize <= 200000) return new Cavities(4);
    else throw "OOPS!";
    
}

/******************************************************************************/
/* Class ConveyerBelt
/******************************************************************************/

class ConveyerBelt {
    private :
        string conveyerBeltType;
    public :
        ConveyerBelt() {}
        ConveyerBelt(const string &beltType) : conveyerBeltType(beltType) {}

        string &getConveyerBeltType() { return conveyerBeltType; }

        static ConveyerBelt *makeObject(int);
};

ConveyerBelt *ConveyerBelt::makeObject(int orderSize) {
    if(orderSize <= 10000) return new ConveyerBelt("Linear");
    if(orderSize <= 20000) return new ConveyerBelt("Y-Split");
    if(orderSize <= 40000) return new ConveyerBelt("V-Level");
    if(orderSize <= 50000) return new ConveyerBelt("Linear");
    if(orderSize <= 100000) return new ConveyerBelt("Y-Split");
    if(orderSize <= 200000) return new ConveyerBelt("V-Level");
    else throw "OOPS!";
    
}

/******************************************************************************/
/* Class PartsBin
/******************************************************************************/

class PartsBin {
    private :
        string partsBinType;
    public :
        PartsBin() {}
        PartsBin(const string &binType) : partsBinType(binType) {}

        string &getPartsBinType() { return partsBinType; }

        static PartsBin *makeObject(int);
};

PartsBin *PartsBin::makeObject(int orderSize) {
    if(orderSize <= 10000) return new PartsBin("CardboardBox");
    if(orderSize <= 40000) return new PartsBin("ShellBox");
    if(orderSize <= 100000) return new PartsBin("PalletBox");
    if(orderSize <= 200000) return new PartsBin("Crate");
    else return new PartsBin("largest");
    
}

/******************************************************************************/
/* class RunTime
/******************************************************************************/

class RunTime {
    public :
        RunTime() {}
        virtual int getRunTime(int orderSize, int shapeVol, int cavityCount, string &metalName, string plasticType) {}

        static RunTime*makeObject(int);
};

class BackOfTheEnvelope : public RunTime {
    public :
        BackOfTheEnvelope() {}
        int getRunTime(int orderSize, int shapeVol, int cavityCount, string &metalName, string plasticType) { return ((orderSize/cavityCount) * 1)/60; }
};

class Calculation : public RunTime {
    public :
        Calculation() {}
        int getRunTime(int orderSize, int shapeVol, int cavityCount, string &metalName, string &plasticType) { 
            if(metalName == "Aluminum") {
                return ((orderSize/cavityCount) * legacy_classes::cycleTime_sec("aluminum", shapeVol))/3600; 
            } else {
                return ((orderSize/cavityCount) * legacy_classes::cycleTime_sec("steel", shapeVol))/3600; 
            }
        }
};

class Historical : public RunTime {
    public :
        Historical() {}
        int getRunTime(int orderSize, int shapeVol, int cavityCount, string &metalName, string plasticType) { 
            int cycletime;
            if(metalName == "Aluminum") {
                cycletime = legacy_classes::cycleTime_sec("aluminum", shapeVol);
            } else {
                cycletime = legacy_classes::cycleTime_sec("steel", shapeVol);
            }
            return (legacy_classes::setupAvg_min + ((orderSize/cavityCount) * cycletime)/60 + legacy_classes::teardownAvg_min)/60;
        }
};

class Projection : public RunTime {
    public :
        Projection() {}
        int getRunTime(int orderSize, int shapeVol, int cavityCount, string &metalName, string plasticType) { 
            int cycletime;
            if(metalName == "Aluminum") {
                cycletime = legacy_classes::cycleTime_sec("aluminum", shapeVol);
            } else {
                cycletime = legacy_classes::cycleTime_sec("steel", shapeVol);
            }
            int rejectRatePercent;
            if(plasticType == "PET") {
                rejectRatePercent = legacy_classes::rejectRateHigh_pcnt;
            } else {
                rejectRatePercent = legacy_classes::rejectRateLow_pcnt;
            } 
            return (legacy_classes::setupAvg_min + ((1 + rejectRatePercent * 0.01)*(orderSize/cavityCount) * cycletime)/60 + legacy_classes::teardownAvg_min)/60;
        }
};

RunTime *RunTime::makeObject(int orderSize) {
    if(orderSize <= 10000) return new BackOfTheEnvelope;
    if(orderSize <= 25000) return new Calculation;
    if(orderSize <= 50000) return new Historical;
    if(orderSize <= 200000) return new Projection;
    else throw "OOPS !";
}

/******************************************************************************/
/* Class Tools
/******************************************************************************/

class Tools {
    public :
        Tools() {}
        static Tools *makeObject(const string &metalName, const string &finish);
        virtual void getTools() {}
};

class HighCarbonTools : public Tools {
    public :
        HighCarbonTools() {}
        void getTools() { cout << "      using HighCarbon tools (drill, cut, and high speed grind)"; }
};

class CarbideTools : public Tools {
    public :
        CarbideTools() {}
        void getTools() { cout << "      using Carbide tools (high speed drill, cross cut, and layer grind)"; }
};

class DiamondTippedTools : public Tools {
    public :
        DiamondTippedTools() {}
        void getTools() { cout << "      using DiamondTipped tools (precision drill, oil cooled cut, and cartoid grind)"; }
};

class EDMMillingTools : public Tools {
    public :
        EDMMillingTools() {}
        void getTools() { cout << "      using EDM tools (static punch, static slice, and static etch)"; }
};

Tools *Tools::makeObject(const string &metalName, const string &finish) {
    if((metalName == "Aluminum") or (metalName == "Steel" and finish == "smooth")) return new HighCarbonTools();
    if(finish == "rippled") return new CarbideTools();
    if(finish == "sanded" or finish == "satin") return new EDMMillingTools();
    else return new DiamondTippedTools();
}

/******************************************************************************/
/* Class Color
/******************************************************************************/

class Color {
    private :
        string colorName;
    public :
        Color(const string &colorType) {
            colorName = colorType;
        }
        string &getColorName() { return colorName; }

        static Color *makeObject(const string &colorName); 
};

Color *Color::makeObject(const string &colorName) {
    // Can be enumed
    if(colorName != "unknown") return new Color(colorName);
    else return new Color("clear");
}

/******************************************************************************/
/* Class Stuffer
/******************************************************************************/

class Stuffer {
    private :
       string stufferName;
    public :
        Stuffer() {
            stufferName = "Air";
            cout << "  <>Unknown stuffer || defaulting to 'None'.\n";
        }
        Stuffer(const string &stufferMaterial) : stufferName(stufferMaterial) {}
        string getStuffer() { return stufferName; }
        static Stuffer *makeObject(const string &);
};

class Air : public Stuffer {
    public :
        Air() : Stuffer("Air") {}
};
class Foam : public Stuffer {
    public :
        Foam() : Stuffer("expanding foam") {}
};

class Bubblewrap : public Stuffer {
    public :
        Bubblewrap() : Stuffer("bubble wrap") {}
};

class Popcorn : public Stuffer {
    public :
        Popcorn() : Stuffer("styrene popcorn") {}
};

Stuffer *Stuffer::makeObject(const string &stuffingMaterial) {
    if(stuffingMaterial == "Air") return new Air();
    if(stuffingMaterial == "Foam") return new Foam();
    if(stuffingMaterial == "Bubblewrap") return new Bubblewrap();
    if(stuffingMaterial == "Popcorn") return new Popcorn();
    else return new Stuffer();
} 

// Seam point - add another algorithm.

}

namespace observer {		// DP 5.

/******************************************************************************/
/* Class Additives
/******************************************************************************/

class Additive {
    protected :
        string additiveType;
        int additiveVol;
    public :
        Additive(const string &additiveName, int additiveVolume) : additiveType(additiveName), additiveVol(additiveVolume) {}
        void add() { cout << " " << additiveType << "(" << additiveVol << ")"; }
        void setAdditiveVol(int additiveVolume) { additiveVol = additiveVolume; }
        string &getAdditiveType() { return additiveType; }
        int getAdditiveVol() { return additiveVol; }
};

class Mixer {
        list<Additive *> additiveList;
    public :
        Mixer() {}

        void Attach(Additive *additive) { additiveList.push_back(additive); }
        void Detach(Additive *additive) { additiveList.remove(additive); }

        void Mix();
};

void Mixer::Mix() {
    list<Additive *>::iterator it = additiveList.begin();
    if((*it)->getAdditiveType() != "Styrene" and (*it)->getAdditiveType() != "Nylon66") {
        cout << "    Load plastic bin with " << (*it)->getAdditiveType();
        it++;
        if((*it)->getAdditiveType() != "clear") {
            cout << " and color bin with " << (*it)->getAdditiveType();
        }
        cout << ".\n";
        cout << "      Recipe:";
        for(it = additiveList.begin(); it != additiveList.end(); it++) {
            if((*it)->getAdditiveType() != "clear") {
                (*it)->add();
            }
        }
    } else {
        string PlasticName = (*it)->getAdditiveType();
        int PlasticVol = (*it)->getAdditiveVol();
        it++;
        string ColorName = (*it)->getAdditiveType();
        int ColorVol = (*it)->getAdditiveVol();
        it++;
        cout << "    Load plastic bin with " <<  ColorName << "-" << PlasticName << " pellets.\n";
        cout << "      Recipe: " << ColorName << "-" << PlasticName << "(" << ColorVol + PlasticVol << ")";
        for(; it != additiveList.end(); it++) {
            if((*it)->getAdditiveType() != "clear") {
                (*it)->add();
            }
        }
    }
}

// Seam point - add another listener.

}

namespace abstract_factory {// DP 9.

// Seam point - add another type 1.
// Seam point - add another type 2.
// Seam point - add another type 3.
// Seam point - add another ...
// Seam point - add another type N.
// Seam point - add another family.

}

namespace bridge {			// DP 8.

/******************************************************************************/
/* Class Mold
/******************************************************************************/

class Mold {
    private :
        string moldType;
        int    moldVolInCc;
        string moldMillSteps;
    public :
        Mold(const string &type, int vol, const string &steps) : moldType(type), moldVolInCc(vol), moldMillSteps(steps) {}
        Mold() {
            moldType = "duck";
            moldVolInCc = 35;
            moldMillSteps = "D3C2D2G4";
            cout << "    <>Unknown mold || defaulting to 'duck'.\n";
        }

        string getMoldType() { return moldType; }
        int    getMoldVolInCC() { return moldVolInCc; }
        string getMoldMillSteps() { return moldMillSteps; }

        static Mold *makeObject(const string &);
};

class DuckMold : public Mold {
    public :
        DuckMold() : Mold("duck", 35, "D3C2D2G4") {}
};

class CarMold : public Mold {
    public :
        CarMold() : Mold("car", 40, "D2C8D1G3") {}
};

class HeroMold : public Mold {
    public :
        HeroMold() : Mold("hero", 50, "D1C1D1G7") {}
};

class DinoMold : public Mold {
    public :
        DinoMold() : Mold("dino", 30, "G5D2C1") {}
};

Mold *Mold::makeObject(const string &moldType) {
    // Can be enumed
    if(moldType == "duck") return new DuckMold();
    if(moldType == "car") return new CarMold();
    if(moldType == "hero") return new HeroMold();
    if(moldType == "dino") return new DinoMold();
    return new Mold();
}

/******************************************************************************/
/* Class MoldLoc
/******************************************************************************/

class MoldLoc {
    protected :
        string location;
    public :
        Mold *mold;

        MoldLoc(Mold *moldreq) : mold(moldreq) {}
        virtual void display(int moldCavities) {
            cout << "    <>Can't find place || to get |" << mold->getMoldType() << "| mold from with || finish, defaulting to smooth duck from inventory.\n"; 
        }

        static MoldLoc *makeObject(const string &, Mold *);
        string &getLocation() { return location; }
        Mold *getMold() { return mold; }
};

class InventoryMoldLoc : public MoldLoc {
    public :
        InventoryMoldLoc(Mold *moldreq) : MoldLoc(moldreq) { location = "inventory"; }
        void display(int moldCavities) {
            cout << "    Pull " << mold->getMoldType() << " mold from inventory.\n";
        }
};

class SisterCompanyMoldLoc : public MoldLoc {
    public :
        SisterCompanyMoldLoc(Mold *moldreq) : MoldLoc(moldreq) { location = "sister comapny"; }
        void display(int moldCavities) {
            cout << "    Borrow " << mold->getMoldType() << " mold from sister company.\n";
        }
};

class PurchaseMoldLoc : public MoldLoc {
    public :
        PurchaseMoldLoc(Mold *moldreq) : MoldLoc(moldreq) { location = "purchase"; }
        void display(int moldCavities) {
            cout << "    Acquire " << mold->getMoldType() << " mold via purchase.\n";
        }
};

class MillMoldLoc : public MoldLoc {
    public :
        MillMoldLoc(Mold *moldreq) : MoldLoc(moldreq) { location = "mill"; }
        void display(int moldCavities) {
            if(moldCavities == 1) {
                cout << "    Create " << mold->getMoldType() << " mold from mill with 1 cavity - steps: " << mold->getMoldMillSteps() << ".\n";
            } else {
                cout << "    Create " << mold->getMoldType() << " mold from mill with " << moldCavities << " cavities - steps: " << mold->getMoldMillSteps() << ".\n";
            }
        }
};

MoldLoc *MoldLoc::makeObject(const string &location, Mold *moldreq) {
    if(location == "inventory") return new InventoryMoldLoc(moldreq);
    if(location == "sisterCompany") return new SisterCompanyMoldLoc(moldreq);
    if(location == "purchase") return new PurchaseMoldLoc(moldreq);
    if(location == "mill") return new MillMoldLoc(moldreq);
    return new MoldLoc(moldreq);
}
// Seam Point - add another implementation.
// Seam Point - add another abstraction.

}

namespace chain_of_resp {	// DP 7.

// Seam points - add another responder.

}

namespace decorator {		// DP 6.

/******************************************************************************/
/* Class Tag
/******************************************************************************/

class Tag {
    protected :
        string tagDesc;
    public :
        Tag(string name="Undefined tag") : tagDesc(name) {}
        virtual string getTagDesc() { return tagDesc; }
        virtual int getTagWidth() = 0;
        void displayTags();
};

class BlankTag : public Tag {
    public :
        BlankTag(const string &desc) : Tag(desc) {}
        string getTagDesc() { return tagDesc; }
        int getTagWidth() { return 0; }

        static BlankTag *makeObject();
};

BlankTag *BlankTag::makeObject() {
    return new BlankTag("");
}

class DecoratorTag : public Tag {
    protected :
        Tag *currTag;
    public :
        DecoratorTag(Tag *tags, const string &desc) : Tag(desc), currTag(tags) {}
        string getTagDesc()  { 
            if(currTag->getTagDesc() == "") {
                return tagDesc; 
            } else {
                return currTag->getTagDesc() + " " + tagDesc; 
            }
        }
        static Tag *makeObject(Tag *tag, const string &tagName);
};

class ModelNumberTag : public DecoratorTag {
    public :
        ModelNumberTag(Tag *tags) : DecoratorTag(tags, "ModelNumber") {}
        int getTagWidth() { return currTag->getTagWidth() + 2; }
};

class CountryTag : public DecoratorTag {
    public :
        CountryTag(Tag *tags) : DecoratorTag(tags, "Country") {}
        int getTagWidth() { return currTag->getTagWidth() + 2; }
};

class DateTag : public DecoratorTag {
    public :
        DateTag(Tag *tags) : DecoratorTag(tags, "Date") {}
        int getTagWidth() { return currTag->getTagWidth() + 2; }
};

class IncCounterTag : public DecoratorTag {
    public :
        IncCounterTag(Tag *tags) : DecoratorTag(tags, "IncCounter") {}
        int getTagWidth() { return currTag->getTagWidth() + 4; }
};

class PartNumberTag : public DecoratorTag {
    public :
        PartNumberTag(Tag *tags) : DecoratorTag(tags, "PartNumber") {}
        int getTagWidth() { return currTag->getTagWidth() + 2; }
};

class RecycleCodeTag : public DecoratorTag {
    public :
        RecycleCodeTag(Tag *tags) : DecoratorTag(tags, "RecycleCode") {}
        int getTagWidth() { return currTag->getTagWidth() + 6; }
};

Tag *DecoratorTag::makeObject(Tag *tags, const string &tagType) {
    if(tagType == "ModelNumber") {
        return new ModelNumberTag(tags);
    } else if(tagType == "Country") {
        return new CountryTag(tags);
    } else if(tagType == "Date") {
        return new DateTag(tags);
    } else if(tagType == "IncCounter") {
        return new IncCounterTag(tags);
    } else if(tagType == "PartNumber") {
        return new PartNumberTag(tags);
    } else if(tagType == "RecycleCode") {
        return new RecycleCodeTag(tags);
    } else if(tagType == "unknown") {
        return tags;
    } else {
        cout << "    Ignoring unknown tag " << tagType << ".\n";
        return tags;
    }
}

void Tag::displayTags() {
    cout << "    Insert tags [" << getTagDesc() << "] of width " << getTagWidth() << "/20 mm, blank tag is " << 20 - getTagWidth() << " mm.\n";
}
// Seam point - add another option.

}

namespace factory_method {	// DP 4.

// Seam point - add another class.

}

namespace template_method {	// DP 3.

/******************************************************************************/
/* Class Injection cycle
/******************************************************************************/

class InjectionCycle {
    public :
        void Close() { cout << "      Close"; }
        void Mix() { cout << " - mix"; }
        virtual void Heat() { cout << " - heat to 440"; }
        virtual void Inject() { cout << " - inject at 125 PSI"; }
        virtual void Cool() { cout << " - cool to 360"; }
        void Separate() { cout << " - separate"; }
        virtual void Eject() { cout << " - smooth eject.\n"; }
        void Run() {
            Close();
            Mix();
            Heat();
            Inject();
            Cool();
            Separate();
            Eject();
        }
        static InjectionCycle *makeObject(const string &plasticType);
};

class ABSInjectionCycle : public InjectionCycle {
    public :
        void Eject() { cout << " - progressive eject.\n"; }
};

class PolyInjectionCycle : public InjectionCycle {
    public :
        void Heat() { cout << " - heat to 350"; }
        void Inject() { cout << " - inject at 90 PSI"; }
        void Cool() { cout << " - cool to 290"; }
};

class PETInjectionCycle : public InjectionCycle {
    public :
        void Heat() { cout << " - heat to 404"; }
        void Inject() { cout << " - inject at 110 PSI"; }
        void Cool() { cout << " - cool to 340"; }
};

class StyreneInjectionCycle : public InjectionCycle {
        void Heat() { cout << " - heat to 480"; }
        void Inject() { cout << " - inject at 150 PSI"; }
        void Cool() { cout << " - cool to 390"; }
        void Eject() { cout << " - shock eject.\n"; }
};

class Nylon66InjectionCycle : public InjectionCycle {
        void Heat() { cout << " - heat to 480"; }
        void Inject() { cout << " - inject at 150 PSI"; }
        void Cool() { cout << " - cool to 390"; }
        void Eject() { cout << " - shock eject.\n"; }
};


InjectionCycle *InjectionCycle::makeObject(const string &plasticType) {
    if(plasticType == "ABS") return new ABSInjectionCycle;
    if(plasticType == "Polyethelene" or plasticType == "Polypropylene") return new PolyInjectionCycle;
    if(plasticType == "PET") return new PETInjectionCycle;
    if(plasticType == "Styrene") return new StyreneInjectionCycle;
    if(plasticType == "Nylon66") return new Nylon66InjectionCycle;
    else return new InjectionCycle;
}

// Seam point - add another polymorphic step.
// Seam point - add another constant step.
// Seam point - convert a constant step into a polymorphic step.

}

/******************************************************************************/

string Parse(map<string, string> &order, const string &attr) {
    map<string, string>::iterator it;

    it = order.find(attr);
    if(it != order.end()) {
        return order.find(attr)->second;
    } else {
        return "unknown";
    }
}

class OrderAttributes {
        string plastic;
        string size;
        string packager;
        string stuffer;
        string mold;
        string moldLoc;
        string tags;
        string color;
        string finish;
        string uvinhibiter;
        string antibacterial;
        string hydrophilic;
        string microfibers;
        string address;

    public :
        OrderAttributes(map<string, string> &order) {
            plastic = Parse(order, "plastic");
            size = Parse(order, "size");
            packager = Parse(order, "packager");
            stuffer = Parse(order, "stuffer");
            mold = Parse(order, "mold");
            moldLoc = Parse(order, "moldLoc");
            tags = Parse(order, "tags");
            color = Parse(order, "color");
            finish = Parse(order, "finish");
            uvinhibiter = Parse(order, "UVInhibiter");
            antibacterial = Parse(order, "AntiBacterial");
            hydrophilic = Parse(order, "Hydrophilic");
            microfibers = Parse(order, "MicroFibers");
            address = Parse(order, "address");
        }
        ~OrderAttributes() {}

        string &getPlastic() { return plastic; }
        string &getSize() { return size; }
        string &getPackager() { return packager; }
        string &getStuffer() { return stuffer; }
        string &getMold() { return mold; }
        string &getMoldLoc() { return moldLoc; }
        string &getTags() { return tags; }
        string &getColor() { return color; }
        string &getFinish() { return finish; }
        string &getUvinhibiter() { return uvinhibiter; }
        string &getAntibacterial() { return antibacterial; }
        string &getHydrophilic() { return hydrophilic; }
        string &getMicroFibers() { return microfibers; }
        string &getAddress() { return address; }
};

/******************************************************************************/
/* Injection Cycle Methods
/******************************************************************************/

void SetupInjectionLine(strategy::Plastic *plasticType,
                        strategy::OrderSize *orderSize,
                        strategy::Packager *packager,
                        strategy::Stuffer *stuffer,
                        strategy::IJM *ijm,
                        strategy::Metal *metal,
                        strategy::Cavities *cavities,
                        strategy::ConveyerBelt *conveyerBelt,
                        strategy::PartsBin *partsBin) {
    cout << "  Setup injection line for " << orderSize->getOrderSize() << " order";
    if(packager->getPackager() != "Bulk") {
        cout << " with " << packager->getPackager() << " packager";
    }
    if(stuffer->getStuffer() != "Air") {
        cout << " and " << stuffer->getStuffer() << " stuffer";
    }
    cout << ":\n";
    cout << "    " << ijm->getIJMName() << " - " << metal->getMetalName() << "(" << cavities->getCavityCount() << ") - " << conveyerBelt->getConveyerBeltType() << " conveyer belt - " << partsBin->getPartsBinType() << ".\n";
}

void GetAndInstallTheMold(bridge::MoldLoc *moldloc, strategy::Cavities *cavities, strategy::Tools *tools, strategy::Metal *metal, const string &finish) {
    moldloc->display(cavities->getCavityCount());
    if(moldloc->getLocation() == "mill") {
        tools->getTools();
        if(cavities->getCavityCount() == 1) {
            cout << " to " << moldloc->getLocation() << " " << metal->getMetalName() << " block into " << cavities->getCavityCount() << " " << (moldloc->getMold())->getMoldType() << " shape with " << finish << " finish.\n";
        } else {
            cout << " to " << moldloc->getLocation() << " " << metal->getMetalName() << " block into " << cavities->getCavityCount() << " " << (moldloc->getMold())->getMoldType() << " shapes with " << finish << " finish.\n";
        }
    }
}

void InsertTagsIntoMold(decorator::Tag *tags, const string &taglist) {
    istringstream iss(taglist);
    string tagName;
    while(iss >> tagName) {
        tags = decorator::DecoratorTag::makeObject(tags, tagName);
    }
    tags->displayTags();
}

void LoadBins(observer::Mixer *mixer,
              strategy::Plastic *plasticType,
              strategy::Color *colorType,
              bridge::Mold *moldReq,
              strategy::Cavities *cavities,
              OrderAttributes &orderAttr) {
    observer::Additive *additive[6]; // Hard coding becasue spec says 1 plastic, 1 color and 4 additives
    size_t additiveNumber = 0;
    
    int additiveVol = 0;


    additive[additiveNumber] = new observer::Additive(plasticType->getPlasticName(), 0);
    mixer->Attach(additive[additiveNumber]);
    additiveNumber++;

    additive[additiveNumber] = new observer::Additive(colorType->getColorName(), moldReq->getMoldVolInCC()/10);
    mixer->Attach(additive[additiveNumber]);
    additiveNumber++;

    if(colorType->getColorName() != "clear") {
        additiveVol += moldReq->getMoldVolInCC()/10;
    }

    if(orderAttr.getUvinhibiter() != "unknown") {
        additive[additiveNumber] = new observer::Additive("UVInhibiter", stoi(orderAttr.getUvinhibiter()));
        mixer->Attach(additive[additiveNumber]);
        additiveVol += stoi(orderAttr.getUvinhibiter());
        additiveNumber++;
    }
    if(orderAttr.getAntibacterial() != "unknown") {
        additive[additiveNumber] = new observer::Additive("AntiBacterial", stoi(orderAttr.getAntibacterial()));
        mixer->Attach(additive[additiveNumber]);
        additiveVol += stoi(orderAttr.getAntibacterial());
        additiveNumber++;
    }
    if(orderAttr.getHydrophilic() != "unknown") {
        additive[additiveNumber] = new observer::Additive("Hydrophilic", stoi(orderAttr.getHydrophilic()));
        mixer->Attach(additive[additiveNumber]);
        additiveVol += stoi(orderAttr.getHydrophilic());
        additiveNumber++;
    }
    if(orderAttr.getMicroFibers() != "unknown") {
        additive[additiveNumber] = new observer::Additive("MicroFibers", stoi(orderAttr.getMicroFibers()));
        mixer->Attach(additive[additiveNumber]);
        additiveVol += stoi(orderAttr.getMicroFibers());
    }
    int plasticVol = moldReq->getMoldVolInCC() - additiveVol;
    additive[0]->setAdditiveVol(plasticVol);

    mixer->Mix();
    cout << " = " << moldReq->getMoldVolInCC() << " cc.\n";
    if(cavities->getCavityCount() == 1) {
        cout << "      Volume: " << moldReq->getMoldType() << "(" << moldReq->getMoldVolInCC() << ") * " << "1 cavity = " << moldReq->getMoldVolInCC() * 1 << " cc.\n";
    } else {
        cout << "      Volume: " << moldReq->getMoldType() << "(" << moldReq->getMoldVolInCC() << ") * " << cavities->getCavityCount() << " cavities = " << moldReq->getMoldVolInCC() * cavities->getCavityCount() << " cc.\n";
    }
}

void ComputeRunTime(strategy::RunTime *runTime, 
                    strategy::OrderSize *orderSize, 
                    strategy::Cavities *cavities, 
                    strategy::Metal *metal, 
                    bridge::Mold *moldreq, 
                    strategy::IJM *ijm,
                    strategy::Plastic *plasticType) {
    int time = runTime->getRunTime(orderSize->getOrderSize(), moldreq->getMoldVolInCC(), cavities->getCavityCount(), metal->getMetalName(), plasticType->getPlasticName());
    if(orderSize->getOrderSize() > 100000) {
        cout << "    <>Using most sophisticated estimation algorithm for orders greater than 100000.\n";
    }
    if(time == 1) {
        cout << "    Cycle " << ijm->getIJMName() << " for " << plasticType->getPlasticName() << " " << orderSize->getOrderSize()/cavities->getCavityCount() << " times, estimated run time = " << time << " hour.\n";
    } else {
        cout << "    Cycle " << ijm->getIJMName() << " for " << plasticType->getPlasticName() << " " << orderSize->getOrderSize()/cavities->getCavityCount() << " times, estimated run time = " << time << " hours.\n";
    }
}

void SimulateFullBin(strategy::PartsBin *partsBin, strategy::IJM *ijm, strategy::ConveyerBelt *conveyerBelt, strategy::Packager *packager, strategy::Stuffer *stuffer) {
    cout << "      Whenever " << partsBin->getPartsBinType() << " parts bin was full...\n";
    cout << "        " << ijm->getIJMName() << " paused while " << partsBin->getPartsBinType() << " parts bin was swapped.\n";
    cout << "        " << conveyerBelt->getConveyerBeltType() << " conveyer belt paused while " <<  partsBin->getPartsBinType() << " parts bin was swapped.\n";
    if(packager->getPackager() != "Bulk") {
        cout << "        " << packager->getPackager() << " packager paused while " << partsBin->getPartsBinType() << " parts bin was swapped.\n";;
    }
    if(stuffer->getStuffer() == "styrene popcorn") {
        cout << "        Popcorn stuffer triggered to fill " << partsBin->getPartsBinType() << " parts bin.\n";
    }
    if(stuffer->getStuffer() == "bubble wrap") {
        cout << "        Bubblewrap stuffer triggered to fill " << partsBin->getPartsBinType() << " parts bin.\n";
    }
    if(stuffer->getStuffer() == "expanding foam") {
        cout << "        Foam stuffer triggered to fill " << partsBin->getPartsBinType() << " parts bin.\n";
    }
}

void RunInjectionCycle(template_method::InjectionCycle *injectionCycle) {
    injectionCycle->Run();
}

void CleanMolds(adapter::Clean *cleaning, strategy::Metal *metal) {
    cleaning->cleanMolds(metal->getMetalName());
}

/******************************************************************************/

void process(map<string,string>& order) {
	// Fill in the namespaces above with your design pattern class hierarchies.
	// Call your order processing class from here <myProcess>->run(order);
    OrderAttributes orderAttr(order);

    strategy::Plastic *plasticType = strategy::Plastic::makeObject(orderAttr.getPlastic());
    strategy::OrderSize *orderSize = strategy::OrderSize::makeObject(orderAttr.getSize());
    strategy::Packager *packager = strategy::Packager::makeObject(orderAttr.getPackager());
    strategy::Stuffer *stuffer = strategy::Stuffer::makeObject(orderAttr.getStuffer());
    strategy::IJM *ijm = strategy::IJM::makeObject(orderSize->getOrderSize());
    strategy::Metal *metal = strategy::Metal::makeObject(orderSize->getOrderSize());
    strategy::Cavities *cavities = strategy::Cavities::makeObject(orderSize->getOrderSize());
    strategy::ConveyerBelt *conveyerBelt = strategy::ConveyerBelt::makeObject(orderSize->getOrderSize());
    strategy::PartsBin *partsBin = strategy::PartsBin::makeObject(orderSize->getOrderSize());
    SetupInjectionLine(plasticType, orderSize, packager, stuffer, ijm, metal, cavities, conveyerBelt, partsBin);

    cout << "  Process order:\n";

    // Get mold
    bridge::Mold *moldReq = bridge::Mold::makeObject(orderAttr.getMold());
    bridge::MoldLoc *moldLocationReq = bridge::MoldLoc::makeObject(orderAttr.getMoldLoc(), moldReq);
    strategy::Tools *tools = strategy::Tools::makeObject(metal->getMetalName(), orderAttr.getFinish());
    GetAndInstallTheMold(moldLocationReq, cavities, tools, metal, orderAttr.getFinish());

    // Insert Tags into mold
    decorator::Tag *tags = decorator::BlankTag::makeObject();
    InsertTagsIntoMold(tags, orderAttr.getTags());

    // Load plastic, color die and additive bins
    strategy::Color *colorType = strategy::Color::makeObject(orderAttr.getColor());
    observer::Mixer *mixer = new observer::Mixer();
    LoadBins(mixer, plasticType, colorType, moldReq, cavities, orderAttr);

    // Compute estimated Run Time
    strategy::RunTime *runTime = strategy::RunTime::makeObject(orderSize->getOrderSize());
    ComputeRunTime(runTime, orderSize, cavities, metal, moldReq, ijm, plasticType);

    // Run the Injection Cycle till done
    template_method::InjectionCycle *injectionCycle = template_method::InjectionCycle::makeObject(plasticType->getPlasticName());
    RunInjectionCycle(injectionCycle);

    // Simulate Full Bin
    SimulateFullBin(partsBin, ijm, conveyerBelt, packager, stuffer);

    // Clean the mold
    adapter::Clean *cleaning = adapter::Clean::makeObject(plasticType->getPlasticName());
    CleanMolds(cleaning, metal);
 
    // Ship it Whereever Needed
    if(orderAttr.getAddress() == "unknown") {
        cout << "    Ship to \"\".\n";
    } else {
        cout << "    Ship to \"" << orderAttr.getAddress() << "\".\n";
    }
    cout << endl << endl << endl;
}

pair<string,string> parse(string line) {
	char key[83];
	char val[83] = {0};

	sscanf(line.c_str(), "%s", key);

	char* equalSign = strchr((char *)line.c_str(), '=');
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
	cout << "<<< final solution >>>\n";

	FILE* orderFilePtr = fopen(orderFile.c_str(), "r");

	for(;;) {	// Get and process each order.
		map<string,string> order = getCompleteOrder(orderFilePtr);
		if(order.size() == 0)
			break;
		process(order);
	}
}

} // final_design

#endif /* FINAL_FINAL_DESIGN_H_ */

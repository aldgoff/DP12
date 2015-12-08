/*
 * finalOriginal.h
 *
 *  Created on: Nov.26, 2015
 *      Author: kchen12
 */

#ifndef FINAL_FINAL_ORIGINAL_H_
#define FINAL_FINAL_ORIGINAL_H_

const bool on = true;	// Useful macro to turn dtor instrumentation on/off.
#define DTORF(x) if(on) { cout << x; }

namespace final_original {
    namespace legacy_classes {	// Can't change these.

        int cycleTime_sec(string metal, int volume_cc) {
            int heatCapacityAdjustment = 0;

            if (metal == "aluminum")	heatCapacityAdjustment = -2;
            else if (metal == "steel")		heatCapacityAdjustment = +5;

            return (int)(20 + heatCapacityAdjustment + 0.1*volume_cc);
        }

        const int setupAvg_min = 118;
        const int teardownAvg_min = 54;
        const float rejectRateLow_pcnt = 0.4F;
        const float rejectRateHigh_pcnt = 1.2F;

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

    using namespace legacy_classes;

    enum Shapes { DUCK, CAR, HERO };
    const string shapes[] = { "duck", "car", "hero" };
    const int volume_cc[] = { 35, 40, 50 };
    const string mill_steps[] = { "D3C2D2G4", "D2C8D1G3", "D1C1D1G7" };
    enum Tools { HIGH_CARBON, CARBIDE, DIAMONDTIPPED };
    const string tools[] = { "HighCarbon", "Carbide", "DiamondTipped" };

    enum Metals { ALUMINUM, STEEL };
    string metals[] = { "aluminum", "steel" };
    enum Cavities { CAVITY_1 = 1, CAVITY_2 };

    const int SMALL_SIZE = 10000;
    const int MEDIUM_SIZE = 20000;
    const int LARGE_SIZE = 50000;

    const int TAG_WIDTH = 20;

    namespace adapter {			// DP 2.
        class CleanMold {
        protected:
            Metals metal;
        public:
            CleanMold(Metals metal) : metal(metal) {}
            virtual ~CleanMold() { DTORF("~adapter::CleanMold\n"); }

        public:
            Metals getMetal() const { return metal; }
            virtual void clean() {};
        };

        class CleanupABS : public CleanMold {
            legacy_classes::CleanABS cleanABS;
        public:
            CleanupABS(Metals metal) : CleanMold(metal) {}
            virtual ~CleanupABS() { DTORF("~ABS "); }

        public:
            void clean() { cleanABS.clean(); }
        };

        class CleanupPolys : public CleanMold {
            legacy_classes::CleanPolys cleanPolys;
        public:
            virtual ~CleanupPolys() { DTORF("~Poly "); }

        public:
            CleanupPolys(Metals metal) : CleanMold(metal) {}
            void clean() { cleanPolys.prepForReuse(); }
        };

        class CleanupPET : public CleanMold {
            legacy_classes::PETCleanup cleanPET;
        public:
            CleanupPET(Metals metal) : CleanMold(metal) {}
            virtual ~CleanupPET() { DTORF("~PET "); }

        public:
            void clean() {
                if (getMetal() == STEEL) cleanPET.carbonize();
                if (getMetal() == ALUMINUM) cleanPET.purify();
            }
        };
    }

    // Run Time Estimation
    namespace strategy {		// DP 1.

        class RuntimeEstimate {
        public:
            RuntimeEstimate(int orderSize, int cavities, string metal, int volume_cc) :
                orderSize(orderSize), cavities(cavities), metal(metal), volume_cc(volume_cc) {}

            virtual ~RuntimeEstimate() { DTORF("~strategy::RuntimeEstimate\n"); }

        public:
            virtual int estimate(string metal, int volume_cc) = 0;

        protected:
            int orderSize;
            int cavities;
            string metal;
            int volume_cc;
        };

        class BackOfTheEnvelope : public RuntimeEstimate {
        public:
            BackOfTheEnvelope(int orderSize, int cavities, string metal, int volume_cc) :
                RuntimeEstimate(orderSize, cavities, metal, volume_cc){}

            virtual ~BackOfTheEnvelope() { DTORF("~BackOfTheEnvelope "); }

        public:
            int estimate(string metal, int volume_cc) { return orderSize / cavities / 60; }
        };

        class Calculation : public RuntimeEstimate {
        public:
        public:
            Calculation(int orderSize, int cavities, string metal, int volume_cc) :
                RuntimeEstimate(orderSize, cavities, metal, volume_cc) {}

            virtual ~Calculation() { DTORF("~Calculation "); }

        public:
            int estimate(string metal, int volume_cc) {
                return orderSize / cavities * cycleTime_sec(metal, volume_cc) / 3600;
            }
        };

        class Historical : public RuntimeEstimate {
        public:
            Historical(int orderSize, int cavities, string metal, int volume_cc) :
                RuntimeEstimate(orderSize, cavities, metal, volume_cc) {}

            virtual ~Historical() { DTORF("~Historical "); }

        public:
            int estimate(string metal, int volume_cc) {
                return (setupAvg_min + orderSize / cavities * cycleTime_sec(metal, volume_cc) / 60 + teardownAvg_min) / 60;
            }
        };
    }

    // Parts Bins, IJM, conveyer belt, packager
    namespace observer {		// DP 5.
        class BinObserver {
        public:
            virtual ~BinObserver() { DTORF("~observer::BinObserver\n"); }

        public:
            virtual void pause() = 0;
            static BinObserver* makeObject(const string& name);
        };

        class IJM : public BinObserver {
        public:
            ~IJM() { DTORF("~IJM "); }
        };

        class IJM_110 : public IJM {
        public:
            ~IJM_110() { cout << "~IJM_110 "; }

        public:
            void pause() { cout << "        IJM_110 paused "; }
        };

        class IJM_120 : public IJM {
        public:
            ~IJM_120() { cout << "~IJM_120 "; }

        public:
            void pause() { cout << "        IJM_120 paused "; }
        };

        class IJM_210 : public IJM {
        public:
            ~IJM_210() { cout << "~IJM_210 "; }

        public:
            void pause() { cout << "        IJM_210 paused "; }
        };

        class ConveyerBelt : public BinObserver {
        public:
            ~ConveyerBelt() { cout << "~ConveyerBelt "; }

        public:
            virtual void pause() {}
        };

        class LinearBelt : public ConveyerBelt {
        public:
            ~LinearBelt() { cout << "~LinearBelt "; }

        public:
            void pause() { cout << "        Linear conveyer belt paused "; }
        };

        class YSplitBelt : public ConveyerBelt {
        public:
            ~YSplitBelt() { cout << "~YSplitBelt "; }

        public:
            void pause() { cout << "        Y-Split conveyer belt paused "; }
        };

        class BinSubject {
        public:
            virtual ~BinSubject() { DTORF("~observer::BinSubject\n"); }

        public:
            virtual void update() = 0;
            static BinSubject* makeObject(const string& name);
        };

        class PartsBin : public BinSubject {
        public:
            virtual ~PartsBin() { DTORF("~PartsBin "); }
        };

        class CardboardBox : public PartsBin {
        public:
            ~CardboardBox() { cout << "~CardboardBox "; }

        public:
            void update() { cout << "while CardboardBox parts bin was swapped.\n"; }
        };

        class ShellBox : public PartsBin {
        public:
            ~ShellBox() { cout << "~ShellBox "; }

        public:
            void update() { cout << "while ShellBox parts bin was swapped.\n"; }
        };

        class PalletBox : public PartsBin {
        public:
            ~PalletBox() { cout << "~PalletBox "; }

        public:
            void update() { cout << "while PalletBox parts bin was swapped.\n"; }
        };

        class IJM_Bin_Belt_Listerner {
            const string name;
            list<BinObserver*> IJMObs;
            list<BinObserver*> BeltObs;
            list<BinSubject*> BinObs;

        public:
            IJM_Bin_Belt_Listerner(const string& name) : name(name) {}
            ~IJM_Bin_Belt_Listerner() {}

        public:
            void attach(BinObserver* obsIJM, BinObserver* obsBelt, BinSubject* obsBin) {
                IJMObs.push_back(obsIJM); 
                BeltObs.push_back(obsBelt);
                BinObs.push_back(obsBin);
            }
            void detach(BinObserver* obsIJM, BinObserver* obsBelt, BinSubject* obsBin) { 
                IJMObs.remove(obsIJM); 
                BeltObs.remove(obsBelt);
                BinObs.remove(obsBin); 
            }
            string getPackagerName() const { return name; }
            void update(bool binFull = true) {
                if (binFull) {
                    cout << "      Whenever " + name + " parts bin was full...\n";
                    list<BinObserver*>::iterator itIJM = IJMObs.begin();
                    for (; itIJM != IJMObs.end(); ++itIJM) { (*itIJM)->pause(); }
                    list<BinSubject*>::iterator itBin = BinObs.begin();
                    for (; itBin != BinObs.end(); ++itBin) { (*itBin)->update(); }
                    list<BinObserver*>::iterator itBelt = BeltObs.begin();
                    for (; itBelt != BeltObs.end(); ++itBelt) { (*itBelt)->pause(); }
                    itBin = BinObs.begin();
                    for (; itBin != BinObs.end(); ++itBin) { (*itBin)->update(); }
                }
            }
        };

        BinObserver* BinObserver::makeObject(const string& name) {
            if (name == "IJM_110") return new IJM_110;
            if (name == "IJM_120") return new IJM_120;
            if (name == "IJM_210") return new IJM_210;

            if (name == "LinearBelt") return new LinearBelt;
            if (name == "YSplitBelt") return new YSplitBelt;

            throw "Error!";
        }

        BinSubject* BinSubject::makeObject(const string& name) {
            if (name == "CardboardBox") return new CardboardBox;
            if (name == "ShellBox") return new ShellBox;
            if (name == "PalletBox") return new PalletBox;

            throw "Error!";
        }
    }

    namespace factory_method {	// DP 4.
        using namespace observer;

        class Packager : public BinObserver {
        public:
            virtual ~Packager() { DTORF("~factory_method::Packager "); }

        public:
            virtual void pause() {}
            static Packager* makeObject(const string& name);
        };

        class ShrinkWrap : public Packager {
        public:
            virtual ~ShrinkWrap() { DTORF("~ShrinkWrap "); }

        public:
            void pause() { cout << "        Shrink wrap packager paused "; }
        };

        class HardPack : public Packager {
        public:
            virtual ~HardPack() { DTORF("~HardPack "); }

        public:
            void pause() { cout << "        Hard pack packager paused "; }
        };

        Packager* Packager::makeObject(const string& name)
        {
            if (name == "ShrinkWrap") return new ShrinkWrap;
            if (name == "HardPack") return new HardPack;

            return NULL;
        }
    }

    // Mold Location
    namespace abstract_factory {// DP 9.
        using namespace observer;
        using namespace factory_method;

        class PlasticType {
        protected:
            string type;
        public:
            PlasticType(string type) : type(type) {}
            virtual ~PlasticType() {}

            string getType() const { return type; }
            virtual void plastic_process() = 0;
        };
        class ABS : public PlasticType {
        public:
            ABS() : PlasticType("ABS") {}
            virtual ~ABS() { DTORF("~ABS\n"); }
            void plastic_process() {
                cout << "      Close - mix - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject.\n";
            }
        };
        class Poly : public PlasticType {
        public:
            Poly(string polyType) : PlasticType(polyType) {}
            virtual ~Poly() { DTORF("~Poly\n"); }
            virtual void plastic_process() {
                cout << "      Close - mix - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject.\n";
            }
        };
        class Polypropylene : public Poly {
        public:
            Polypropylene() : Poly("Polypropylene") {}
            virtual ~Polypropylene() {}
        };
        class Polyethelene : public Poly {
        public:
            Polyethelene() : Poly("Polyethelene") {}
            virtual ~Polyethelene() {}
        };
        class PET : public PlasticType {
        public:
            PET() : PlasticType("PET") {}
            virtual ~PET() { DTORF("~PET\n"); }
            void plastic_process() {
                cout << "      Close - mix - heat to 404 - inject at 110 PSI - cool to 340 - separate - smooth eject.\n";
            }
        };

        class Mold {
        public:
            Mold(string shape, string metal, Cavities cavity) : shape(shape), metal(metal), cavity(cavity) {}
            virtual ~Mold() { DTORF("~Mold\n"); }

        protected:
            int size;
            string shape;
            string metal;
            Cavities cavity;
        };

        class Aluminum : public Mold {
        public:
            Aluminum(string shape, string metal, Cavities cavity) : Mold(shape, metal, cavity) {}
            virtual ~Aluminum() { DTORF("~Aluminum "); }
        };

        class Steel : public Mold {
        public:
            Steel(string shape, string metal, Cavities cavity) : Mold(shape, metal, cavity) {}
            virtual ~Steel() { DTORF("~Steel ") }
        };

        class InjectionLine {
        protected:
            int size;
            Metals metal;
            int volume_cc;
            Cavities cavity;
            IJM_Bin_Belt_Listerner* ijm_bin_belt;
            BinObserver* obsIJM;
            Mold* mold;
            BinObserver* obsBelt;
            BinSubject* obsBin;
            Packager* packager;
            PlasticType *plasticType;
            string machine;

        public:
            InjectionLine(int size) : size(size) {}
            virtual ~InjectionLine() { DTORF("~abstract_factory::InjectionLine\n"); }

        public:
            virtual void setupProductInfo(const string& type, const string& mold_shape) {
                if (type == "ABS") plasticType = new ABS;
                else if (type == "Polypropylene") plasticType = new Polypropylene;
                else if (type == "Polyethelene") plasticType = new Polyethelene;
                else if (type == "PET") plasticType = new PET;
                else plasticType = new ABS;

                if (mold_shape == "duck") volume_cc = 35;
                else if (mold_shape == "car") volume_cc = 40;
                else if (mold_shape == "hero") volume_cc = 50;
                else volume_cc = 35;
            }

            virtual void setupIJM_Belt_Bin() {}
            virtual void setupMold() {}
            virtual void setupPackager(const string& name ) {
                packager = Packager::makeObject(name);
            }
            virtual void setupPartsBin() {}

            virtual void removePlasticType() {
                delete plasticType;
            }
            virtual void tearDownIJM_Belt_Bin_Mold() {
                ijm_bin_belt->detach(obsIJM, obsBelt, obsBin);

                if(obsBelt) delete obsBelt;
                if (mold) delete mold;
                if (obsIJM) delete obsIJM;
                if (obsBin) delete obsBin;
            }
            virtual void removePackager() { if (packager) delete(packager); }

            virtual Cavities getCavity() const { return cavity; }
            virtual Metals getMetal() const { return metal; }
            virtual int getOrderSize() const { return size; }
            virtual int getVolumeCC() const { return volume_cc; }
            virtual string getMachine() const { return machine; }
            virtual PlasticType* getPlasticObject() const { return plasticType; }
            virtual void ObserveIJM_Bin_Belt() { ijm_bin_belt->update(); }
            virtual void ObserverPackager() {
                if (packager) {
                    packager->pause();
                    cout << "while " << ijm_bin_belt->getPackagerName() << " parts bin was swapped.\n";
                }
            }
            static InjectionLine* makeObject(int size);
        };

        class SmallOrder : public InjectionLine {
        public:
            SmallOrder(int size) : InjectionLine(size) {
                cout << "    IJM_110 - Aluminum(1) - Linear conveyer belt - CardboardBox.\n";
                cavity = CAVITY_1;
                metal = ALUMINUM;
                machine = "IJM_110";
            }
            ~SmallOrder() { DTORF(((size > 100) ? "~SmallOrder " : "~PilotOrder ")); }

        public:
            void setupIJM_Belt_Bin() {
                ijm_bin_belt = new IJM_Bin_Belt_Listerner("CardboardBox");
                obsIJM = BinObserver::makeObject("IJM_110");
                obsBin = BinSubject::makeObject("CardboardBox");
                obsBelt = BinObserver::makeObject("LinearBelt");
                ijm_bin_belt->attach(obsIJM, obsBelt, obsBin);
            }
            void setupMold() { mold = new Aluminum("IJM_110", "Aluminum", CAVITY_1); }
        };

        class MediumOrder : public InjectionLine {
        public:
            MediumOrder(int size) : InjectionLine(size) {
                cout << "    IJM_120 - Aluminum(2) - Y-Split conveyer belt - ShellBox.\n";
                cavity = CAVITY_2;
                metal = ALUMINUM;
                machine = "IJM_120";
            }
            ~MediumOrder() { DTORF("~MediumOrder "); }

        public:
            void setupIJM_Belt_Bin() {
                ijm_bin_belt = new IJM_Bin_Belt_Listerner("ShellBox");
                obsIJM = BinObserver::makeObject("IJM_120");
                obsBin = BinSubject::makeObject("ShellBox");
                obsBelt = BinObserver::makeObject("YSplitBelt");
                ijm_bin_belt->attach(obsIJM, obsBelt, obsBin);
            }
            void setupMold() { mold = new Aluminum("IJM_120", "Aluminum", CAVITY_2); }
        };

        class LargeOrder : public InjectionLine {
        public:
            LargeOrder(int size) : InjectionLine(size) {
                cout << "    IJM_210 - Steel(1) - Linear conveyer belt - PalletBox.\n";
                cavity = CAVITY_1;
                metal = STEEL;
                machine = "IJM_210";
            }
            ~LargeOrder() { DTORF("~LargeOrder "); }

        public:
            void setupIJM_Belt_Bin() {
                ijm_bin_belt = new IJM_Bin_Belt_Listerner("PalletBox");
                obsIJM = BinObserver::makeObject("IJM_210");
                obsBin = BinSubject::makeObject("PalletBox");
                obsBelt = BinObserver::makeObject("LinearBelt");
                ijm_bin_belt->attach(obsIJM, obsBelt, obsBin);
            }
            void setupMold() { mold = new Steel("IJM_210", "Steel", CAVITY_1); }
        };

        InjectionLine* InjectionLine::makeObject(int size) {
            if (size <= SMALL_SIZE) return new SmallOrder(size);
            else if (size <= MEDIUM_SIZE) return new MediumOrder(size);
            else if (size <= LARGE_SIZE) return new LargeOrder(size);

            return new LargeOrder(LARGE_SIZE);
        }
    }

    namespace bridge {			// DP 8.
        class Platform {
        public:
            virtual ~Platform() { DTORF("~bridge::Platform\n"); }

        public:
            virtual void output() = 0;
            static Platform* makeObject(Tools& platform);
        };

        class Shape {
        protected:
            string name;
        public:
            Platform* platform;

        public:
            Shape(Platform* platform) : platform(platform) { name = ""; }
            virtual ~Shape() {
                DTORF("~bridge::Shape ");
                if (platform) delete platform;
            }
            virtual void process(Metals metal, Cavities cavity, const string& finish) {
                platform->output();
                cout << metals[metal] << " block into " << cavity << " " << name << " " <<
                    ((cavity > 1) ? "shapes" : "shape") << " with " << finish << " finish.\n";
            }
            virtual string getName() { return name; }

        public:
            static Shape* makeObject(const string& shape, Platform* platform);
        };

        class HighCarbon : public Platform {
        public:
            virtual ~HighCarbon() { DTORF("~HighCarbon "); }

            void output() {
                cout << "      using HighCarbon tools (drill, cut, and high speed grind) to mill ";
            }
        };

        class Carbide : public Platform {
        public:
            virtual ~Carbide() { DTORF("~Carbide "); }
            void output() {
                cout << "      using Carbide tools (high speed drill, cross cut, and layer grind) to mill ";
            }
        };

        class DiamondTipped : public Platform {
        public:
            virtual ~DiamondTipped() { DTORF("~DiamondTipped "); }
            void output() {
                cout << "      using DiamondTipped tools (precision drill, oil cooled cut, and cartoid grind) to mill ";
            }
        };

        class Duck : public Shape {
        public:
            Duck(Platform* platform) : Shape(platform) { name = "duck"; }
            ~Duck() { DTORF("~Duck "); }
        };

        class Car : public Shape {
        public:
            Car(Platform* platform) : Shape(platform) { name = "car"; }
            ~Car() { DTORF("~Car "); }
        };

        class Hero : public Shape {
        public:
            Hero(Platform* platform) :  Shape(platform) { name = "hero"; }
            ~Hero() { DTORF("~Hero "); }
        };

        Platform* Platform::makeObject(Tools& platform) {
            if (platform == HIGH_CARBON) return new HighCarbon;
            if (platform == CARBIDE) return new Carbide;
            if (platform == DIAMONDTIPPED) return new DiamondTipped;

            return NULL;
        }

        Shape* Shape::makeObject(const string& shape, Platform* platform) {
            if (shape == "duck") return new Duck(platform);
            if (shape == "car") return new Car(platform);
            if (shape == "hero") return new Hero(platform);

            return NULL;
        }
    }

    namespace chain_of_resp {	// DP 7.
        class GetMold {
        protected:
            GetMold* mold;

        public:
            GetMold() : mold(0) {}
            virtual ~GetMold() { delete mold; DTORF("~CofR::GetMold "); }

        public:
            void setMold(GetMold* next) { mold = next; }

            virtual void findModLoc(const string& loc, Shapes shape, Cavities cavity) {};
            static GetMold* makeObject(const string& criteria);
        };

        class Inventory : public GetMold {
        public:
            ~Inventory() { DTORF("~Inventory "); }

        public:
            void findModLoc(const string& loc, Shapes shape, Cavities cavity) {
                if (loc == "inventory")
                    cout << "    Pull " << shapes[shape] << " mold from inventory.\n";
                else
                    mold->findModLoc(loc, shape, cavity);
            }
        };

        class SisterCompany : public GetMold {
        public:
            ~SisterCompany() { DTORF("~SisterCompany "); }

        public:
            void findModLoc(const string& loc, Shapes shape, Cavities cavity) {
                if (loc == "sister company")
                    cout << "    Borrow " << shapes[shape] << " mold from sister company.\n";
                else
                    mold->findModLoc(loc, shape, cavity);
            }
        };

        class Mill : public GetMold {
        public:
            ~Mill() { DTORF("~Mill "); }

            void findModLoc(const string& loc, Shapes shape, Cavities cavity) {
                if (loc == "mill")
                    cout << "    Create " << shapes[shape] << " mold from mill with " << cavity << " cavities - steps: " << mill_steps[shape] << ".\n";
            }
        };

        GetMold* GetMold::makeObject(const string& criteria) {
            if (criteria == "inventory") return new Inventory;
            if (criteria == "sister company") return new SisterCompany;
            if (criteria == "mill") return new Mill;

            return new Inventory;
        }

        GetMold* SetupChain()
        {
            string chain[] = { "inventory", "sister company", "mill" };

            GetMold* mold = GetMold::makeObject(chain[0]);

            GetMold* current = mold;
            for (size_t i = 1; i < sizeof(chain) / sizeof(chain[0]); ++i)
            {
                GetMold* next = GetMold::makeObject(chain[i]);
                current->setMold(next);
                current = next;
            }
            current->setMold(new GetMold);

            return mold;
        }
    }

    // Plastic, Color, Tag, Additives
    namespace decorator {		// DP 6.
        class Polymer {
        protected:
            string plastic_name;
            string color;

        public:
            Polymer() {}
            Polymer(string name, string color = "") : plastic_name(name), color(color) {};
            virtual ~Polymer() { DTORF("~dec::Polymer "); }

            virtual string getName() { return ""; }
            virtual string getColor() { return ""; }

            virtual string getAdditiveRecipe() { return ""; }
            virtual int getData() { return 0; }
        };

        class Plastic : public Polymer {
        public:
            Plastic(string name, string color) : Polymer(name, color) {};
            ~Plastic() { DTORF("~Plastic "); }

            string getName() { return plastic_name;  }
            string getColor() { return color; }
            virtual string getAdditiveRecipe() { return ""; }
            virtual int getData() { return 0; }
        };

        class Additive : public Polymer {
        protected:
            Polymer* polymer;
        public:
            Additive(Polymer *polymer) : polymer(polymer) {}
            ~Additive() { DTORF("~Additive "); delete polymer; }

        public:
            static Polymer* makeObject(Polymer* polymer, string additives, string data);
        };

        class UVInhibiter : public Additive {
            string uvdata;
        public:
            UVInhibiter(Polymer* polymer, string data) : Additive(polymer), uvdata(data) {}
            ~UVInhibiter() { DTORF("~UVInhibiter "); }

        public:
            string getName() { return polymer->getName(); }
            string getColor() { return polymer->getColor(); }
            string getAdditiveRecipe() { return polymer->getAdditiveRecipe() + "UVInhibiter(" + uvdata + ") "; }
            int getData() { return atoi(uvdata.c_str()) + polymer->getData(); }
        };

        class AntiBacterial : public Additive {
            string anti_data;
        public:
            AntiBacterial(Polymer* polymer, string data) : Additive(polymer), anti_data(data) {}
            ~AntiBacterial() { DTORF("~AntiBacterial "); }

        public:
            string getName() { return polymer->getName(); }
            string getColor() { return polymer->getColor(); }
            string getAdditiveRecipe() { return polymer->getAdditiveRecipe() + "AntiBacterial(" + anti_data + ") "; }
            int getData() { return atoi(anti_data.c_str()) + polymer->getData(); }
        };

        class Hydrophilic : public Additive {
            string hydro_data;
        public:
            Hydrophilic(Polymer* polymer, string data) : Additive(polymer), hydro_data(data) {}
            ~Hydrophilic() { DTORF("~Hydrophilic "); }

        public:
            string getName() { return polymer->getName(); }
            string getColor() { return polymer->getColor(); }
            string getAdditiveRecipe() { return polymer->getAdditiveRecipe() + "Hydrophilic(" + hydro_data + ") "; }
            int getData() { return atoi(hydro_data.c_str()) + polymer->getData(); }
        };

        class Cavity {
        public:
            virtual ~Cavity() { DTORF("~dec::Cavity "); }
            virtual string add_tag() = 0;
            virtual int tag_width() = 0;
        };

        class Blank : public Cavity {
        public:
            ~Blank() { DTORF("~Blank "); }
            string add_tag() { return ""; }
            int tag_width() { return 0; }
        };

        class Tags : public Cavity {
        protected:
            Cavity* cavity;
        public:
            Tags(Cavity *cavity) : cavity(cavity) { }
            ~Tags() { DTORF("~Tags "); delete cavity; }

        public:
            static Cavity* makeObject(Cavity* cavity, const string tag);
        };

        class ModelNumber : public Tags {
        public:
            ModelNumber(Cavity* cavity) : Tags(cavity) {}
            ~ModelNumber() { DTORF("~ModelNumber "); }

        public:
            string add_tag() { return cavity->add_tag() + "ModelNumber"; }
            int tag_width() { return cavity->tag_width() + 2; }
        };

        class Country : public Tags {
        public:
            Country(Cavity* cavity) : Tags(cavity) {}
            ~Country() { DTORF("~Country "); }

        public:
            string add_tag() { return cavity->add_tag() + " Country"; }
            int tag_width() { return cavity->tag_width() + 2; }
        };

        class Date : public Tags {
        public:
            Date(Cavity* cavity) : Tags(cavity) {}
            ~Date() { DTORF("~Date "); }

        public:
            string add_tag() { return cavity->add_tag() + " Date"; }
            int tag_width() { return cavity->tag_width() + 2; }
        };

        Polymer* Additive::makeObject(Polymer* polymer, string additives, string data) {
            if (additives == "UVInhibiter") return new UVInhibiter(polymer, data);
            if (additives == "AntiBacterial") return new AntiBacterial(polymer, data);
            if (additives == "Hydrophilic") return new Hydrophilic(polymer, data);

            return polymer;
        }

        Cavity* Tags::makeObject(Cavity* cavity, const string tag) {
            if (tag == "ModelNumber") return new ModelNumber(cavity);
            if (tag == "Country") return new Country(cavity);
            if (tag == "Date") return new Date(cavity);

            return cavity;
        }
    }

    using namespace chain_of_resp;
    using namespace adapter;
    using namespace strategy;
    using namespace abstract_factory;
    using namespace bridge;
    using namespace decorator;

    // Injection Process, cleanup
    namespace template_method {	// DP 3.
        class ProcessOrder {
        protected:
            GetMold* mold;
            CleanMold* clean;
            RuntimeEstimate* rt;
            Shape* shapeHandle;
            Cavity* tagHanlde;
            Polymer* polymerHandle;

        public:
            ProcessOrder() : mold(0) {}
            virtual ~ProcessOrder() { DTORF("~template_method::ProcessOrder\n"); }

        public:
            virtual void get_install_mold(map<string, string>& order, Cavities cavity) {
                if (order["mold"] != "duck" && order["mold"] != "car" && order["mold"] != "hero") {
                    cout << "  "; 
                    defaulting(order, "mold", "duck");
                    order["mold"] = "duck";
                }

                Shapes shape;
                if (order["mold"] == "duck") shape = DUCK;
                if (order["mold"] == "car") shape = CAR;
                if (order["mold"] == "hero") shape = HERO;

                mold = SetupChain();

                if (order["moldLoc"] != "inventory" && order["moldLoc"] != "sister company" && order["moldLoc"] != "mill") {
                    cout << "    <>Can't find place |" << order["moldLoc"] << "| to get |" << order["mold"] << "| mold from with |" << order["finish"] << "| finish, defaulting to smooth duck from inventory.\n";
                    order["moldLoc"] = "inventory";
                    order["finish"] = "smooth";
                }
                else {
                    mold->findModLoc(order["moldLoc"], shape, cavity);
                }
            }
            virtual void selectMillingPlatform(map<string, string>& order, InjectionLine* inLine) {
                Tools tool;
                if (order["moldLoc"] == "mill") {
                    Tools tool;
                    if (order["finish"] == "smooth" || inLine->getMetal() == ALUMINUM) tool = HIGH_CARBON;
                    else if (order["finish"] == "rippled") tool = CARBIDE;
                    else tool = DIAMONDTIPPED;

                    shapeHandle = Shape::makeObject(order["mold"], Platform::makeObject(tool));
                    shapeHandle->process(inLine->getMetal(), inLine->getCavity(), order["finish"]);
                }
                else {
                    tool = HIGH_CARBON;
                    shapeHandle = Shape::makeObject(order["mold"], Platform::makeObject(tool));
                }
            }
            virtual void ProcessTagInsert(string tags) {
                istringstream in(tags);
                string modNum, country, date;

                in >> modNum >> country >> date;

                tagHanlde = new Blank;

                if (modNum != "") {
                    if (modNum == "ModelNumber") tagHanlde = Tags::makeObject(tagHanlde, "ModelNumber");
                    else cout << "    Ignoring unknown tag " << modNum << ".\n";
                }

                if (country != "") {
                    if (country == "Country") tagHanlde = Tags::makeObject(tagHanlde, "Country");
                    else cout << "    Ignoring unknown tag " << country << ".\n";
                }

                if (date != "") {
                    if (date == "Date") tagHanlde = Tags::makeObject(tagHanlde, "Date");
                    else cout << "    Ignoring unknown tag " << date << ".\n";
                }

                string tag_name = tagHanlde->add_tag();
                int tag_length = tagHanlde->tag_width();
                cout << "    Insert tags [" << tag_name << "] of width " 
                    << tag_length << "/" << TAG_WIDTH << " mm, " << "blank tag is " << (TAG_WIDTH - tag_length) << " mm.\n";
            }
            virtual void ProcessMixing(map<string, string>& order, InjectionLine* inLine) {
                polymerHandle = new Plastic(order["plastic"], order["color"]);

                if (order["UVInhibiter"] != "") 
                    polymerHandle = Additive::makeObject(polymerHandle, "UVInhibiter", order["UVInhibiter"]);

                if (order["AntiBacterial"] != "") 
                    polymerHandle = Additive::makeObject(polymerHandle, "AntiBacterial", order["AntiBacterial"]);

                if (order["Hydrophilic"] != "") 
                    polymerHandle = Additive::makeObject(polymerHandle, "Hydrophilic", order["Hydrophilic"]);

                cout << "    Load plastic bin with " << polymerHandle->getName();
                if (order["color"] != "") cout << " and color bin with " << polymerHandle->getColor();
                cout << ".\n";

                int color_vol = (order["color"] != "") ? (inLine->getVolumeCC() / 10) : 0;

                int shapeVol = inLine->getVolumeCC() - color_vol - polymerHandle->getData();
                cout << "      Recipe: " << polymerHandle->getName() << "(" << shapeVol << ") ";
                if (color_vol > 0)
                    cout << polymerHandle->getColor() << "(" << color_vol << ") ";

                cout << polymerHandle->getAdditiveRecipe() << "= " << inLine->getVolumeCC() << " cc.\n";

                int cavity = inLine->getCavity();
                if (shapeHandle) {
                    cout << "      Volume: " << shapeHandle->getName() << "(" << inLine->getVolumeCC()
                        << ") * " << cavity << ((cavity > 1) ? " cavities = " : " cavity = ") 
                        << (inLine->getVolumeCC() * inLine->getCavity()) << " cc.\n";
                }
            }
            virtual void ProcessRunTime(InjectionLine* inLine) {
                int size = inLine->getOrderSize();
                Cavities cavity = inLine->getCavity();
                string metal = metals[inLine->getMetal()];
                int volume_cc = inLine->getVolumeCC();
                string type = (inLine->getPlasticObject())->getType();

                // estimate runtime - maybe put in a function
                if (size <= SMALL_SIZE) rt = new BackOfTheEnvelope(size, cavity, metal, volume_cc);
                else if (size <= 25000) rt = new Calculation(size, cavity, metal, volume_cc);
                else rt = new Historical(size, cavity, metal, volume_cc);

                int hrs = rt->estimate(metal, volume_cc);
                cout << "    Cycle " << inLine->getMachine() 
                    << " for " << type << " " << (size / cavity)
                    << " times, estimated run time = "  << hrs << ((hrs > 1) ? " hours.\n" : " hour.\n");

                // processing - no detail spec about the process - just simulation
                (inLine->getPlasticObject())->plastic_process();
            }
            virtual void PartsBinFull(InjectionLine* inLine) {
                inLine->ObserveIJM_Bin_Belt();
                inLine->ObserverPackager();
            }
            virtual void cleanupMolds(InjectionLine* inLine) {
                string plastic = (inLine->getPlasticObject())->getType();
                Metals metal = inLine->getMetal();

                if (plastic == "ABS")
                    clean = new CleanupABS(metal);
                else if (plastic == "Polypropylene" || plastic == "Polyethelene" || plastic == "Poly")
                    clean = new CleanupPolys(metal);
                else if (plastic == "PET")
                    clean = new CleanupPET(metal);
                else
                    clean = new CleanupABS(metal);

                clean->clean();
            }

            virtual void stopProcess() {
                if (shapeHandle) delete shapeHandle;
                if (mold) delete mold;
                cout << "\n";
                if (polymerHandle) delete polymerHandle;
                cout << "\n";
                if (tagHanlde) delete tagHanlde;
                cout << "\n";
                if (rt) delete rt;
                if (clean) delete clean;
            }
        };
    }

    using namespace template_method;

    class MyProcess {
        map<string, string>& order;
        InjectionLine* injectLine;
        ProcessOrder* process;

    public:
        MyProcess(map<string, string>& order) : order(order) {}
        virtual ~MyProcess() {}

    public:
        void run(map<string, string>& order);

        void checkOrders(map<string, string>& order) {
            // 1. Check plastics
            if (order["plastic"] != "ABS" &&
                order["plastic"] != "Polypropylene" &&
                order["plastic"] != "Polyethelene" &&
                order["plastic"] != "PET") {
                defaulting(order, "plastic", "ABS");
                order["plastic"] = "ABS";
            }

            // 2. Check order size
            int size = atoi(order["size"].c_str());
            if (size == 0) {
                cout << "  <>No size specified, defaulting to 100.\n";
                order["size"] = "100";
                size = 100;
            }
            else if (size > 50000) {
                cout << "  <>Size exceeds mold lifetime |" << size << "| defaulting to LargeOrder of 50000.\n";
                order["size"] = "50000";
                size = 50000;
            }

            // 3. Check Pakager
            if (order["packager"] != "Bulk" && order["packager"] != "ShrinkWrap" && order["packager"] != "HardPack") {
                defaulting(order, "packager", "None");
                order["packager"] = "None";
            }
        }

        void setupInjectionLine(map<string, string>& order) {
            int size = atoi(order["size"].c_str());

            if (order["packager"] == "None" || order["packager"] == "Bulk") cout << "  Setup injection line for " << size << " order:\n";
            else cout << "  Setup injection line for " << size << " order with " << order["packager"] << " packager:\n";

            injectLine = InjectionLine::makeObject(size);

            injectLine->setupProductInfo(order["plastic"], order["mold"]);
            injectLine->setupIJM_Belt_Bin();
            injectLine->setupMold();
            injectLine->setupPackager(order["packager"]);
            injectLine->setupPartsBin();
        }

        void tearDownInjectionLine() {
            injectLine->removePlasticType();
            injectLine->removePackager();
            injectLine->tearDownIJM_Belt_Bin_Mold();

            cout << " BinObservers left to process (should be zero) = 0.\n";

            delete injectLine;
        }

        void injectionLineRun(map<string, string>& order) {
            process = new ProcessOrder;
            process->get_install_mold(order, injectLine->getCavity());
            process->selectMillingPlatform(order, injectLine);
            process->ProcessTagInsert(order["tags"]);
            process->ProcessMixing(order, injectLine);
            process->ProcessRunTime(injectLine);
            process->PartsBinFull(injectLine);
            process->cleanupMolds(injectLine);
        }

        void stopSubProcess() {
            process->stopProcess();
            delete process;
        }
    };

    void MyProcess::run(map<string, string>& order) {
        // 1. check orders
        checkOrders(order);

        // 2. Setup Injection Line
        setupInjectionLine(order);

        cout << "  Process order:\n";

        injectionLineRun(order);

        tearDownInjectionLine();

        stopSubProcess();
    }

    void process(map<string, string>& order) {
        MyProcess* myProcess = new MyProcess(order);
        myProcess->run(order);
        delete myProcess;
    }

    pair<string, string> parse(string line) {
        char key[83];
        char val[83] = { 0 };

        sscanf(line.c_str(), "%s", key);

        char* equalSign = const_cast<char *>(strchr(line.c_str(), '='));
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
        cout << "<<< final original >>>\n";

        FILE* orderFilePtr = fopen(orderFile.c_str(), "r");

        for (;;) {	// Get and process each order.
            map<string, string> order = getCompleteOrder(orderFilePtr);
            if (order.size() == 0)
                break;
            process(order);
        }
    }
} // final_original

#endif /* FINAL_FINAL_ORIGINAL_H_ */

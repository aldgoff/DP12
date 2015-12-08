/*
* finalDesign.h
*
*  Created on: <MMDDCCYY>
*      Author: <student name>
*/

#ifndef FINAL_FINAL_DESIGN_H_
#define FINAL_FINAL_DESIGN_H_
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cassert>
using namespace std;

// #define _EMPTY_PROCESS_   // use it to get design1.out

namespace final_design {

   const bool on = false;	// Useful macro to turn dtor instrumentation on/off.
#define DTORF(x) if(on) { cout << x << " "; }

   //typedef map<string,string> Order;
   typedef unordered_map<string, string> Order;

   template < typename T > std::string to_string( const T& n )
   {
      std::ostringstream stm ;
      stm << n ;
      return stm.str() ;
   }

   class PrintBase{
   public:
      virtual const string Name()const=0;  
      void SetName(string name){ _name=name; }
      const string GetName()const{ return _name; }
   protected:
      string _name;
   };

   namespace observer {		// DP 5.

      // Seam point - add another listener.
      class BinObserver : public PrintBase{
      public:
         virtual void Update(string str)=0;
         virtual ~BinObserver(){ DTORF("~observer::BinObserver\n"); }
      };

      class IJM : public BinObserver{
      public:
         void Update(string str){ cout << "        " << Name() << str; } 
         virtual ~IJM(){ DTORF("~IJM"); }
         virtual const string Metal()const=0;
         virtual const int NumCavities()const=0;
      };
      class IJM_110 : public IJM{   
      public:
         ~IJM_110(){ DTORF("~IJM_110"); }
         const string Name()const{ return "IJM_110"; }
         const string Metal()const{ return "Aluminum"; }
         const int NumCavities()const{ return 1; } 
      };
      class IJM_140 : public IJM{   
      public:
         ~IJM_140(){ DTORF("~IJM_140"); }
         const string Name()const{ return "IJM_140"; }
         const string Metal()const{ return "Aluminum"; }
         const int NumCavities()const{ return 4; } 
      };
      class IJM_120 : public IJM{
      public:
         ~IJM_120(){ DTORF("~IJM_120"); }
         const string Name()const{ return "IJM_120"; }
         const string Metal()const{ return "Aluminum"; }
         const int NumCavities()const{ return 2; }
      };
      class IJM_210 : public IJM{
      public:
         ~IJM_210(){ DTORF("~IJM_210"); }
         const string Name()const{ return "IJM_210"; }
         const string Metal()const{ return "Steel"; }
         const int NumCavities()const{ return 1; }
      };
      class IJM_220 : public IJM{
      public:
         ~IJM_220(){ DTORF("~IJM_220"); }
         const string Name()const{ return "IJM_220"; }
         const string Metal()const{ return "Steel"; }
         const int NumCavities()const{ return 2; }
      };
      class IJM_240 : public IJM{
      public:
         ~IJM_240(){ DTORF("~IJM_240"); }
         const string Name()const{ return "IJM_240"; }
         const string Metal()const{ return "Steel"; }
         const int NumCavities()const{ return 4; }
      };

      class ConveyerBelt : public BinObserver{
      public:
         void Update(string str){ cout << "        " << Name() << str; } 
         virtual ~ConveyerBelt(){ DTORF("~ConveyerBelt"); }
      };
      class LinearBelt : public ConveyerBelt{
      public:
         ~LinearBelt(){ DTORF("~LinearBelt"); }
         const string Name()const{ return "Linear conveyer belt"; }
      };
      class YSplitConveyer : public ConveyerBelt{
      public:
         ~YSplitConveyer(){ DTORF("~YSplitBelt"); }
         const string Name()const{ return "Y-Split conveyer belt"; }
      };
      class VlevelConveyer : public ConveyerBelt{
      public:
         ~VlevelConveyer(){ DTORF("~VlevelConveyer"); }
         const string Name()const{ return "V-Level conveyer belt"; }
      };

      class Packager : public BinObserver{
      public:
         ~Packager(){ DTORF("~factory_method::Packager"); }
         static Packager* GetInstance(string packager);
         void Update(string str){ cout << "        " << PauseName() << " packager" << str; } 
         virtual const string PauseName()const=0;
      };
      class ShrinkWrap : public Packager{
      public:
         ~ShrinkWrap(){ DTORF("~ShrinkWrap"); }
         const string Name()const{ return "ShrinkWrap"; }
         virtual const string PauseName()const{ return "Shrink wrap"; }
      };
      class HardPack : public Packager{
      public:
         ~HardPack(){ DTORF("~HardPack"); }
         const string Name()const{ return "HardPack"; }
         virtual const string PauseName()const{ return "Hard pack"; }
      };
      class ZipLock : public Packager{
      public:
         ~ZipLock(){ DTORF("~ZipLock"); }
         const string Name()const{ return "ZipLock"; }
         virtual const string PauseName()const{ return "Zip lock"; }
      };
      Packager* Packager::GetInstance(string packager){
         if( packager==string("Bulk") ) return nullptr;
         else if( packager==string("ShrinkWrap") ) return new ShrinkWrap;
         else if( packager==string("HardPack") ) return new HardPack;
         else if( packager==string("ZipLock") ) return new ZipLock;
         else assert(false);
         return nullptr;
      }

      class Stuffer : public BinObserver{
      public:
         ~Stuffer(){ DTORF("~factory_method::Stuffer"); }
         static Stuffer* GetInstance(string stuffer);
         void Update(string str){ cout << "        " << Name() << " stuffer triggered to fill " << "Box" << " parts bin.\n"; } 
      };
      class Air : public Stuffer{
      public:
         ~Air(){ DTORF("~factory_method::Air"); }
         const string Name()const{ return "Air"; }
      };
      class Popcorn : public Stuffer{
      public:
         ~Popcorn(){ DTORF("~factory_method::Popcorn"); }
         const string Name()const{ return "Popcorn"; }
      };
      class Bubblewrap : public Stuffer{
      public:
         ~Bubblewrap(){ DTORF("~factory_method::Bubblewrap"); }
         const string Name()const{ return "Bubblewrap"; }
      };
      Stuffer* Stuffer::GetInstance(string stuffer){
         if( stuffer==string("Air") ) return new Air;
         else if( stuffer==string("Popcorn") ) return new Popcorn;
         else if( stuffer==string("Bubblewrap") ) return new Bubblewrap;
         else assert(false);
         return nullptr;
      }

      class BinSubject{
      public:
         virtual ~BinSubject(){ 
            DTORF("~observer::BinSubject\n");
            _observers.clear();
            //cout << " BinObservers left to process (should be zero) = " << to_string(_observers.size()) << "." << endl;
         };
         void Attach(BinObserver* binObserver){ _observers.push_back(binObserver); }
      protected:
         vector<BinObserver* > _observers;
      };
      class PartsBin : public BinSubject, public PrintBase{
      public:
         ~PartsBin(){ DTORF("~PartsBin"); };
         void NotifyFull(){ 
            cout << "      Whenever " << Name() << " parts bin was full...\n";
            string msg(" paused while " + Name() + " parts bin was swapped.\n");
            for(auto& o : _observers)
               o->Update(msg);
         }
      };
      class CardboardBox : public PartsBin{
      public:
         ~CardboardBox(){ DTORF("~CardboardBox"); }
         const string Name()const{ return "CardboardBox"; }
      };
      class ShellBox : public PartsBin{
      public:
         ~ShellBox(){ DTORF("~ShellBox"); }
         const string Name()const{ return "ShellBox"; }
      };
      class PalletBox : public PartsBin{
      public:
         ~PalletBox(){ DTORF("~PalletBox"); }
         const string Name()const{ return "PalletBox"; }
      };
      class Crate : public PartsBin{
      public:
         ~Crate(){ DTORF("~Crate"); }
         const string Name()const{ return "Crate"; }
      };
      
   }

   using namespace observer;
   namespace bridge {			// DP 8.

      // Seam Point - add another implementation.
      // ~Car ~bridge::Shape ~HighCarbon ~bridge::Platform
      class Shape : public PrintBase{
      public:
         static Shape* GetInstance(const string& shape);
         virtual ~Shape(){ DTORF("~bridge::Shape"); }
         virtual const int Volume()const=0;
         virtual const string MillSteps()const=0;
      };
      class Duck : public Shape{
      public:
         ~Duck(){ DTORF("~Duck"); }
         const int Volume()const{ return 35; }
         string const MillSteps()const{ return "D3C2D2G4"; }
         const string Name()const{ return "duck"; }
      };
      class Car : public Shape{
      public:
         ~Car(){ DTORF("~Car"); }
         const int Volume()const{ return 40; }
         const string MillSteps()const{ return "D2C8D1G3"; }
         const string Name()const{ return "car"; }
      };
      class Hero : public Shape{
      public:
         ~Hero(){ DTORF("~Hero"); }
         const int Volume()const{ return 50; }
         string const MillSteps()const{ return "D1C1D1G7"; }
         const string Name()const{ return "hero"; }
      };
      class Dino : public Shape{
      public:
         ~Dino(){ DTORF("~Dino"); }
         const int Volume()const{ return 30; }
         string const MillSteps()const{ return "G5D2C1"; }
         const string Name()const{ return "dino"; }
      };
      Shape* Shape::GetInstance(const string& shape){
         if(shape=="car") return new Car;
         else if(shape=="duck") return new Duck;
         else if (shape=="hero") return new Hero;
         else if (shape=="dino") return new Dino;
         else assert(false);
         return nullptr;
      }

      class Platform;
      class MoldProperties{
      public:
         MoldProperties():shape(nullptr), numCavities(1){}
         MoldProperties(Order& order, const IJM* ijm, const Shape* s, Platform* p):metal(ijm->Metal()), numCavities(ijm->NumCavities()), shape(s), platform(p){
            moldLoc = string( order["moldLoc"] );
            finish = string( order["finish"] );
         }
         const Shape *shape;
         const Platform *platform;
         string moldLoc;
         string metal;   
         int numCavities;
         string finish;
         friend ostream& operator<<(ostream& o, MoldProperties& rhs){
            o << "shape : "<<rhs.shape<<endl;
            o << "moldLoc : "<<rhs.moldLoc <<endl;
            o << "metal : "<<rhs.metal <<endl;
            o << "numCavities : "<<rhs.numCavities <<endl;
            o << "finish : "<<rhs.finish <<endl;
            return o;
         }
      };

      // Seam Point - add another abstraction.

      class Platform : public PrintBase{
      public:
         Platform(const Shape* s):_shape(s){}
         virtual ~Platform(){ 
            DTORF("~bridge::Platform\n"); 
            delete _shape; _shape=nullptr; 
         }
         static Platform* GetInstance(string& finish, const Shape* s);
      protected: 
         const Shape* _shape;
      };
      class HighCarbon : public Platform{
      public:
         HighCarbon(const Shape* s):Platform(s){}
         ~HighCarbon(){
            DTORF("~HighCarbon"); 
         }
         const string Name()const{ return "HighCarbon tools (drill, cut, and high speed grind)"; }
      };
      class Carbide : public Platform{
      public:
         Carbide(const Shape* s):Platform(s){}
         ~Carbide(){
            DTORF("~Carbide"); 
         }
         const string Name()const{ return "Carbide tools (high speed drill, cross cut, and layer grind)"; }
      };
      class DiamondTipped : public Platform{
      public:
         DiamondTipped(const Shape* s):Platform(s){}
         ~DiamondTipped(){
            DTORF("~DiamondTipped"); 
         }
         const string Name()const{ return "DiamondTipped tools (precision drill, oil cooled cut, and cartoid grind)"; }
      };
      class Sanded : public Platform{
      public:
         Sanded(const Shape* s):Platform(s){}
         ~Sanded(){
            DTORF("~Sanded"); 
         }
         const string Name()const{ return "EDM tools (static punch, static slice, and static etch)"; }
      };
      Platform* Platform::GetInstance(string& finish, const Shape* s){
         if( finish == "smooth" ) return new HighCarbon(s);
         else if( finish == "rippled" ) return new Carbide(s);
         else if( finish == "dimpled" ) return new DiamondTipped(s);
         else if( finish == "sanded" || finish == "satin" ) return new Sanded(s);
         else assert(false);
         return nullptr;
      }

   }

   using namespace bridge;
   namespace decorator {		// DP 6.

      // Seam point - add another option.
      class Plastic;

      class InjectionMaterial : public PrintBase{
      public:
         InjectionMaterial():_volume(0){}
         virtual ~InjectionMaterial(){ DTORF("~dec::Polymer"); }
         static InjectionMaterial* GetInstance(Order& order, Shape* shape);
         virtual int GetVolumeSum()=0;
         virtual string GetInjectionProcess()=0;
         void SetVolume(int v){ _volume=v; }
         int GetVolume(){ return _volume; }
         virtual string GetRecipeStr()=0;
         virtual Plastic* GetPlastic()=0;
      protected:
         string _RecipeStr(){ return Name() + "(" + to_string(GetVolume()) + ")"; }
         int _volume;
      };

      class Plastic : public InjectionMaterial{
      public:
         Plastic(){}
         ~Plastic(){ DTORF("~Plastic"); }
         static Plastic* GetInstance(const string& str);
         int GetVolumeSum(){ return GetVolume(); }
         string GetInjectionProcess(){ return ""; }
         string GetRecipeStr(){ return _RecipeStr(); }
         Plastic* GetPlastic(){ return this; }
      };
/*
            if(plaName=="ABS")
               cout << ;
            else if( plaName.find("Poly") == 0 )
               cout << ;
            else if(plaName=="PET")
               cout << ;
            else assert(false);
*/

      class ABS : public Plastic{
      public:
         ~ABS(){ DTORF("~ABS"); }
         const string Name()const{ return "ABS"; }
         string GetInjectionProcess(){ return "      Close - mix - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject."; }
      };      
                 
      class Polypropylene : public Plastic{
      public:
         ~Polypropylene(){ DTORF("~Polypropylene"); }
         string GetInjectionProcess(){ return "      Close - mix - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject."; }
         const string Name()const{ return "Polypropylene"; }
      };

      class Polyethelene : public Plastic{
      public:
         ~Polyethelene(){ DTORF("~Polyethelene"); }
         string GetInjectionProcess(){ return "      Close - mix - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject."; }
         const string Name()const{ return "Polyethelene"; }
      };

      class PET : public Plastic{
      public:
         ~PET(){ DTORF("~PET"); }
         string GetInjectionProcess(){ return "      Close - mix - heat to 404 - inject at 110 PSI - cool to 340 - separate - smooth eject."; }
         const string Name()const{ return "PET"; }
      };
      class Styrene : public Plastic{
      public:
         ~Styrene(){ DTORF("~Styrene"); }
         string GetInjectionProcess(){ return "      Close - mix - heat to 480 - inject at 150 PSI - cool to 390 - separate - shock eject"; }
         const string Name()const{ return "Styrene"; }
      };
      class Nylon66 : public Plastic{
      public:
         ~Nylon66(){ DTORF("~Nylon66"); }
         string GetInjectionProcess(){ return "      Close - mix - heat to 480 - inject at 150 PSI - cool to 390 - shock eject."; }
         const string Name()const{ return "Nylon66"; }
      };
          

      Plastic* Plastic::GetInstance(const string& str){
         if( str=="ABS" ) return new ABS;
         else if( str=="Polypropylene" ) return new Polypropylene;
         else if( str=="Polyethelene" ) return new Polyethelene;
         else if( str=="PET" ) return new PET;
         else if( str=="Styrene" ) return new Styrene;
         else if( str=="Nylon66" ) return new Nylon66;
         else assert(false);
         return nullptr;
      }

      class InjMatDecorator : public InjectionMaterial{
      public:
         InjMatDecorator(InjectionMaterial* injMat){
            _decorated = injMat;
         }
         ~InjMatDecorator(){ delete _decorated; _decorated=nullptr; }
         string GetRecipeStr(){ return _decorated->GetRecipeStr() + " " + _RecipeStr(); } // shape->Volume() - Additives.Volume()
         Plastic* GetPlastic(){ return _decorated->GetPlastic(); }
         string GetInjectionProcess(){ return ""; }
         int GetVolumeSum(){ return GetVolume() + _decorated->GetVolumeSum(); }
      protected:
         InjectionMaterial* _decorated;
      };

      class Additive : public InjMatDecorator{
      public:
         Additive(InjectionMaterial* injMat):InjMatDecorator(injMat){}
         ~Additive(){ DTORF("~Additive"); }
      };

      class UVInhibiter : public Additive{
      public:
         UVInhibiter(InjectionMaterial* injMat):Additive(injMat){}
         ~UVInhibiter(){ DTORF("~UVInhibiter"); }
         const string Name()const{ return "UVInhibiter"; }
      };

      class AntiBacterial : public Additive{
      public:
         AntiBacterial(InjectionMaterial* injMat):Additive(injMat){}
         ~AntiBacterial(){ DTORF("~AntiBacterial"); }
         const string Name()const{ return "AntiBacterial"; }
      };

      class Hydrophilic : public Additive{
      public:
         Hydrophilic(InjectionMaterial* injMat):Additive(injMat){}
         ~Hydrophilic(){ DTORF("~Hydrophilic"); }
         const string Name()const{ return "Hydrophilic"; }
      };

      class MicroFibers : public Additive{
      public:
         MicroFibers(InjectionMaterial* injMat):Additive(injMat){}
         ~MicroFibers(){ DTORF("~MicroFibers"); }
         const string Name()const{ return "MicroFibers"; }
      };

      class Color : public Additive{
      public:
         Color(InjectionMaterial* injMat):Additive(injMat){}
         ~Color(){ DTORF("~Color"); }
         const string Name()const{ return GetName(); }
      };

      // assume order has default plastic already set, if not set by order text
      InjectionMaterial* InjectionMaterial::GetInstance(Order& order, Shape* shape){
         InjectionMaterial* plastic;
         plastic = Plastic::GetInstance( order["plastic"] );

         InjectionMaterial* additive = plastic;
         for(auto& item : order){
            if( item.first=="color" ){ additive = new Color(additive); additive->SetName(item.second); additive->SetVolume( shape->Volume()/10 ); }
            else if( item.first=="UVInhibiter" )  { additive = new UVInhibiter(additive);  additive->SetVolume( stoi(item.second) ); }
            else if( item.first=="AntiBacterial" ){ additive = new AntiBacterial(additive);  additive->SetVolume( stoi(item.second) ); }
            else if( item.first=="Hydrophilic" )  { additive = new Hydrophilic(additive);  additive->SetVolume( stoi(item.second) ); } 
            else if( item.first=="MicroFibers" )  { additive = new MicroFibers(additive);  additive->SetVolume( stoi(item.second) ); } 
         }
         int plasticVolume = shape->Volume() - additive->GetVolumeSum();
         plastic->SetVolume( plasticVolume );

         return additive;
      }      

      // Usage: cavity = Cavity::GetInstance("ModelNumber Date") => cavity = Date.new( ModelNumber.new( Blank.new ) )  => 
      //        cavity.GetTagWidthSum()
      class Cavity{
      public:
         static Cavity* GetInstance(const string& str);  
         virtual ~Cavity(){ DTORF("~dec::Cavity"); }
         virtual int GetTagWidthSum(){ return 0; }
         virtual string GetNames(){ return ""; }
         friend ostream& operator<<(ostream& o, Cavity& rhs){
            int w = rhs.GetTagWidthSum();
            o << "    Insert tags [" << rhs.GetNames() << "] of width "<< w <<"/20 mm, blank tag is "<< (20 - w) <<" mm."<<endl;
            return o;
         }
      };
      class Blank : public Cavity{
      public:
         Blank(){}
         ~Blank(){ DTORF("~Blank"); }
         const string Name()const{ return ""; } 
         int GetTagWidthSum(){ return 0; }
      };
      class Tags : public Cavity, public PrintBase{
      public:
         Tags(Cavity* t):_decorated(t){}
         ~Tags(){           
            DTORF("~Tags");
            if(_decorated!=nullptr) 
               delete _decorated;
         }
         int GetTagWidthSum(){
            return Width() + _decorated->GetTagWidthSum();
         }
         string GetNames(){
            if(_decorated==nullptr) return Name();
            else{
               string space = _decorated->GetNames()=="" ? "" : " ";
               return  _decorated->GetNames() + space + Name();
            }
         }
         virtual int Width()=0;
      protected:
         Cavity *_decorated;
      };
      class ModelNumber : public Tags{
      public:
         ModelNumber(Cavity* t):Tags(t){}
         ~ModelNumber(){ DTORF("~ModelNumber"); }
         const string Name()const{ return "ModelNumber"; } 
         int Width(){ return 2; }
      };
      class Country : public Tags{
      public:
         Country(Cavity* t):Tags(t){}
         ~Country(){ DTORF("~Country"); }
         const string Name()const{ return "Country"; } 
         int Width(){ return 2; }
      };
      class Date : public Tags{
      public:
         Date(Cavity* t):Tags(t){}
         ~Date(){ DTORF("~Date"); }
         const string Name()const{ return "Date"; } 
         int Width(){ return 2; }
      };
//--
      class IncCounter : public Tags{
      public:
         IncCounter(Cavity* t):Tags(t){}
         ~IncCounter(){ DTORF("~IncCounter"); }
         const string Name()const{ return "IncCounter"; } 
         int Width(){ return 4; }
      };
      class PartNumber : public Tags{
      public:
         PartNumber(Cavity* t):Tags(t){}
         ~PartNumber(){ DTORF("~PartNumber"); }
         const string Name()const{ return "PartNumber"; } 
         int Width(){ return 2; }
      };
      class RecycleCode : public Tags{
      public:
         RecycleCode(Cavity* t):Tags(t){}
         ~RecycleCode(){ DTORF("~RecycleCode"); }
         const string Name()const{ return "RecycleCode"; } 
         int Width(){ return 6; }
      };
  

      vector<string> tokenize(string str){
         stringstream ss(str);
         istream_iterator<string> begin(ss);
         istream_iterator<string> end;
         vector<string> vstrings(begin, end);
         return vstrings;
      }

      // for each tag strings, create Tag instance, and create a chain
      Cavity* Cavity::GetInstance(const string& str){
         Cavity *cavity = new Blank;
         vector<string> tagStrs = tokenize(str);
         for(auto& tagStr : tagStrs){
            if(tagStr=="ModelNumber") cavity = new ModelNumber( cavity );
            else if(tagStr=="Country") cavity = new Country( cavity );
            else if(tagStr=="Date")  cavity = new Date( cavity );
            else if(tagStr=="IncCounter")  cavity = new IncCounter( cavity );
            else if(tagStr=="PartNumber")  cavity = new PartNumber( cavity );
            else if(tagStr=="RecycleCode")  cavity = new RecycleCode( cavity );
            else{
               cout << "    Ignoring unknown tag " << tagStr <<".\n";
            }
         }
         return cavity;
      }

   }

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
      public: 
         ~CleanABS() { DTORF("~legacy_classes::CleanABS "); }
         void clean() {
            cout << "    Clean ABS mold: soak in alcohol, rinse with water, dry.\n";
         }
      };

      class CleanPolys {
      public: 
         ~CleanPolys() { DTORF("~legacy_classes::CleanPolys "); }
         void prepForReuse() {
            cout << "    Clean Poly mold: rinse with acetone, dry.\n";
         }
      };

      class PETCleanup {
      public: 
         ~PETCleanup() { DTORF("~legacy_classes::PETCleanup "); }
         void carbonize() { // Use only on stainless steel.
            cout << "    Clean PET steel mold: heat to 895 C.\n";
         }
         void purify() {	// Use only on aluminum.
            cout << "    Clean PET aluminum mold: rinse with soap and water, dry.\n";
         }
      };

      void defaulting(Order& order, const string& option, string def="simulation") {
         cout << "  <>Unknown " << option << " |";
         cout << order[option] << "|";
         order[option] = def;
         cout << " defaulting to '" << order[option] << "'.\n";
      }
      void SetDefaultAndReport(Order& order, const char* mapKey, string defaultValue, basic_string<char> msg2print){
         string keyStr(mapKey);
         if( order.find(keyStr) == order.end() || order[keyStr]==""){
            if(msg2print.size()>1) cout << msg2print << endl;
            order[keyStr] = defaultValue;       
         }
      }

   }

   using namespace decorator;
   namespace adapter {			// DP 2.


      // Seam point - add another interface.
      using namespace legacy_classes;

      class MoldCleaner{
      public:
         virtual ~MoldCleaner(){ DTORF("~adapter::CleanMold\n"); }
         static MoldCleaner* GetInstance(const Plastic* plastic);
         virtual void Clean(const string& metal)=0;
      };                          
      class AbsCleaner : public MoldCleaner{
      public:
         ~AbsCleaner(){ DTORF("~ABS"); }
         void Clean(const string& metal){ cleanABS.clean(); }
      private:
         CleanABS cleanABS;
      };
      class PolyCleaner : public MoldCleaner{
      public:
         ~PolyCleaner(){ DTORF("~Poly"); }
         void Clean(const string& metal){ cleanPolys.prepForReuse(); }
      private:
         CleanPolys cleanPolys;
      };
      class PetCleaner : public MoldCleaner{
      public:
         ~PetCleaner(){ DTORF("~PET"); }
         void Clean(const string& metal){ 
            if(metal=="Aluminum") petCleanup.purify(); 
            else if(metal=="Steel") petCleanup.carbonize();
            else assert(false);
         }
      private:
         PETCleanup petCleanup;
      };
      class SyntheticsCleaner : public MoldCleaner{
      public:
         ~SyntheticsCleaner(){ DTORF("~Synthetics"); }
         void Clean(const string& metal){ 
            cout << "    Clean synthetic mold: ozone wash." << endl;
         }
      private:
         PETCleanup petCleanup;
      };

   }

   adapter::MoldCleaner* adapter::MoldCleaner::GetInstance(const Plastic* plastic){
      if( plastic->Name() == "ABS" ) return new AbsCleaner;
      else if( plastic->Name().find("Poly")==0 ) return new PolyCleaner;
      else if( plastic->Name() == "PET" ) return new PetCleaner;
      else if( plastic->Name() == "Styrene" ||  plastic->Name() == "Nylon66"  ) return new SyntheticsCleaner;
      else assert(false);
      return nullptr;
   }

   namespace strategy {		// DP 1.

      // Seam point - add another algorithm.
      class BackOfTheEnvelope;
      class Calculation;
      class Historical;

      class RunTimeEstimator{
      public:
         RunTimeEstimator(int orderSize):_orderSize(orderSize){}
         virtual ~RunTimeEstimator(){ DTORF("~strategy::RuntimeEstimate\n"); }
         virtual int GetEstimate(int numCavities, const string& metal, const Shape* shape)=0;
         static RunTimeEstimator* GetInstance(int orderSize);

      protected:
         int _orderSize;
      };

      class BackOfTheEnvelope : public RunTimeEstimator{
      public:
         BackOfTheEnvelope(int orderSize):RunTimeEstimator(orderSize){}
         ~BackOfTheEnvelope(){ DTORF("~BackOfTheEnvelope"); }
         int GetEstimate(int numCavities, const string& metal, const Shape* shape){
            return (int)( _orderSize/(numCavities*60.0f) );
         }
      }; 
      class Calculation : public RunTimeEstimator{
      public:
         Calculation(int orderSize):RunTimeEstimator(orderSize){}
         ~Calculation(){ DTORF("~Calculation"); }
         int GetEstimate(int numCavities, const string& metal, const Shape* shape){
            return _orderSize /numCavities * legacy_classes::cycleTime_sec( metal, shape->Volume() )/3600;  // orderSize/cavities*legacy::cycletime(metal, shape->volume)/3600;
         }
      }; 
      const int ToMin(int t){ return t/60; }
      class Historical : public RunTimeEstimator{
      public:
         Historical(int orderSize):RunTimeEstimator(orderSize){}
         ~Historical(){ DTORF("~Historical"); }
         int GetEstimate(int numCavities, const string& metal, const Shape* shape){
            int cycletimeInSec = legacy_classes::cycleTime_sec( metal, shape->Volume() );
            int runTimeinHour  = (legacy_classes::setupAvg_min + (_orderSize/numCavities*cycletimeInSec)/60 + legacy_classes::teardownAvg_min)/60;
            return runTimeinHour;
         }
      }; 

      class Projection : public RunTimeEstimator{
      public:
         Projection(int orderSize):RunTimeEstimator(orderSize){}
         ~Projection(){ DTORF("~Projection"); }
         int GetEstimate(int numCavities, const string& metal, const Shape* shape){
            int cycletimeInSec = legacy_classes::cycleTime_sec( metal, shape->Volume() );
            int rejectRate_pcnt = (metal=="PET") ? legacy_classes::rejectRateHigh_pcnt  : legacy_classes::rejectRateLow_pcnt;
            int runTimeinHour  = (legacy_classes::setupAvg_min
               + (1 + 0.01*rejectRate_pcnt)*_orderSize/numCavities*cycletimeInSec/60
               + legacy_classes::teardownAvg_min)/60;
            return runTimeinHour;
         }
      }; 

      RunTimeEstimator* RunTimeEstimator::GetInstance(int orderSize){
         if(orderSize<=10000) return new BackOfTheEnvelope(orderSize);
         else if(10000 < orderSize && orderSize<=25000) return new Calculation(orderSize);
         else if(25000 < orderSize && orderSize<=50000) return new Historical(orderSize);
         else if(50000 < orderSize && orderSize<=100000) return new Projection(orderSize);
         else{ 
             cout << "    <>Using most sophisticated estimation algorithm for orders greater than 100000.\n";
             return new Projection(orderSize);
         }
         return nullptr;
      }
   }

   class Mold{
   public:
      static Mold* GetInstance(const string& metal);
      virtual ~Mold(){ DTORF("~Mold\n"); }
      void Insert(Tags* tags){
         cout << tags;
      }
   };
   class Aluminum : public Mold{
   public:
      ~Aluminum(){ DTORF("~Aluminum"); }
   };
   class Steel : public Mold{
   public:
      ~Steel(){ DTORF("~Steel"); }
   };
   Mold* Mold::GetInstance(const string& metal){
      if( metal == "Aluminum") return new Aluminum;
      else if( metal == "Steel" ) return new Steel;
      else assert(false);
      return nullptr;
   }

   using namespace observer;
   namespace abstract_factory {// DP 9.

      // Seam point - add another type 1.
      // Seam point - add another type 2.
      // Seam point - add another type 3.
      // Seam point - add another ...
      // Seam point - add another type N.
      // Seam point - add another family.
      class InjectionLineFactory{
      public:
         virtual ~InjectionLineFactory(){ DTORF("~abstract_factory::InjectionLine\n"); }
         static InjectionLineFactory* GetInstance(int orderSize);
         virtual IJM* GetIJM()=0;
         virtual ConveyerBelt* GetConveyer()=0;
         virtual PartsBin* GetPartsBin()=0;
      };
      class InjLinePilotlOrderFactory : public InjectionLineFactory{
      public:
         ~InjLinePilotlOrderFactory(){ DTORF("~PilotOrder"); }
         IJM* GetIJM(){ return new IJM_110; } 
         ConveyerBelt* GetConveyer(){ return new LinearBelt; } 
         PartsBin* GetPartsBin(){ return new CardboardBox; }
      };
      class InjLineSmallOrderFactory : public InjectionLineFactory{
      public:
         ~InjLineSmallOrderFactory(){ DTORF("~SmallOrder"); }
         IJM* GetIJM(){ return new IJM_120; } 
         ConveyerBelt* GetConveyer(){ return new YSplitConveyer; } 
         PartsBin* GetPartsBin(){ return new ShellBox; }
      };
      class InjLineFastOrderFactory : public InjectionLineFactory{
      public:
         ~InjLineFastOrderFactory(){ DTORF("~FastOrder"); }
         IJM* GetIJM(){ return new IJM_140; } 
         ConveyerBelt* GetConveyer(){ return new VlevelConveyer; } 
         PartsBin* GetPartsBin(){ return new ShellBox; }
      };
      class InjLineMediumOrderFactory : public InjectionLineFactory{
      public:
         ~InjLineMediumOrderFactory(){ DTORF("~MediumOrder"); }
         IJM* GetIJM(){ return new IJM_210; } 
         ConveyerBelt* GetConveyer(){ return new LinearBelt; } 
         PartsBin* GetPartsBin(){ return new PalletBox; }
      };
      class InjLineLargeOrderFactory : public InjectionLineFactory{
      public:
         ~InjLineLargeOrderFactory(){ DTORF("~LargeOrder"); }
         IJM* GetIJM(){ return new IJM_220; } 
         ConveyerBelt* GetConveyer(){ return new YSplitConveyer; } 
         PartsBin* GetPartsBin(){ return new PalletBox; }
      };
      class InjLineHugeOrderFactory : public InjectionLineFactory{        // Steel with 4 cavity
      public:
         ~InjLineHugeOrderFactory(){ DTORF("~HugeOrder"); }
         IJM* GetIJM(){ return new IJM_240; } 
         ConveyerBelt* GetConveyer(){ return new VlevelConveyer; } 
         PartsBin* GetPartsBin(){ return new Crate; }
      };
      const int MaxOrderSize = 200000;

      InjectionLineFactory* InjectionLineFactory::GetInstance(int orderSize){
         if(orderSize <= 10000) return new InjLinePilotlOrderFactory;
         if(10000 < orderSize && orderSize <= 20000) return new InjLineSmallOrderFactory;
         if(20000 < orderSize && orderSize <= 40000 ) return new InjLineFastOrderFactory;
         if(40000 < orderSize && orderSize <= 50000) return new InjLineMediumOrderFactory;
         if(50000 < orderSize && orderSize <= 100000) return new InjLineLargeOrderFactory;
         if(100000 < orderSize && orderSize <= 200000) return new InjLineHugeOrderFactory;
         else assert(false);
         return nullptr;
      }

      class InjectionLine{
      public:
         InjectionLine(int orderSize, string packager, string stuffer){
            _orderSize = orderSize;
            _injLineFactory = InjectionLineFactory::GetInstance(orderSize);
            _ijm = _injLineFactory->GetIJM();
            _conveyer = _injLineFactory->GetConveyer();
            _partsBin = _injLineFactory->GetPartsBin();
            _partsBin->Attach(_ijm);
            _partsBin->Attach(_conveyer);
            _packager = Packager::GetInstance(packager);
            _stuffer = Stuffer::GetInstance(stuffer);
            if( _packager!=nullptr ) _partsBin->Attach(_packager);
            if( _stuffer!=nullptr ) _partsBin->Attach(_stuffer);
            _mold = Mold::GetInstance(_ijm->Metal());
         }
         ~InjectionLine(){
            delete _packager; 
            delete _stuffer; 
            delete _conveyer; 
            delete _mold;
            delete _ijm; 
            delete _partsBin; 
            delete _injLineFactory;
         }
         void Setup(){
            string packagerStr = _packager==nullptr ? "" :  " with " + _packager->Name() +" packager";
            cout << "  Setup injection line for "<< _orderSize << " order"<< packagerStr << ":" << endl;
            cout << "    " << _ijm->Name() << " - " << _ijm->Metal() << "(" << _ijm->NumCavities() << ")" << " - " << _conveyer->Name() << " - "<< _partsBin->Name() << ".\n";
         }
         void Run(){
            cout << " Cycle " << _ijm->Name() << "for " ; //ABS 100 times, estimated run time = 1 hour.
            //_ijm->Run();
            //_conveyer->Run();
            //_packager->Run();
            _partsBin->NotifyFull();
         }
         IJM* GetIJM(){ return _ijm; }
         PartsBin *GetPartsBin(){ return _partsBin; }

      private:
         int _orderSize;
         InjectionLineFactory* _injLineFactory;
         IJM* _ijm;
         ConveyerBelt* _conveyer;
         Packager *_packager;
         Stuffer *_stuffer;
         PartsBin *_partsBin;
         Mold *_mold;
      };

   }

   using namespace observer;
   using namespace bridge;

   namespace chain_of_resp {	// DP 7.

      // Seam points - add another responder.
      class MoldLocation : public PrintBase{
      public:
         MoldLocation(MoldProperties& mp):_mp(mp),_next(nullptr){}
         virtual ~MoldLocation(){ 
            if(_next!=nullptr) 
               delete _next;
            DTORF("~CofR::GetMold"); 
         }
         static MoldLocation* GetInstance(MoldProperties& mp);
         static MoldLocation* SetupChain(const MoldProperties& mpIn);
         // no logic needed to find a mold 
         const Shape* Get(string& moldLoc){
            if( moldLoc==this->Name() ){ 
               GetMold();
               return _mp.shape;
            }else if(_next != nullptr){
               return _next->Get(moldLoc); 
            }else assert(false);
            return nullptr;
         }
         virtual void GetMold()=0;
         void SetSuccessor(MoldLocation* ml){  _next = ml; }

      protected:
         MoldProperties _mp;
         MoldLocation *_next;
      };
      class Inventory : public MoldLocation{
      public:
         Inventory(MoldProperties& mp):MoldLocation(mp){}
         ~Inventory(){ DTORF("~Inventory"); }
         void GetMold(){
            cout << "    Pull " << _mp.shape->Name() <<" mold from inventory.\n";   
         }
         const string Name()const{ return "inventory"; }
      };
      class SisterCompany : public MoldLocation{
      public:
         SisterCompany(MoldProperties& mp):MoldLocation(mp){}
         ~SisterCompany(){ DTORF("~SisterCompany"); }
         void GetMold(){
            cout << "    Borrow " << _mp.shape->Name() <<" mold from sister company.\n";
         }
         const string Name()const{ return "sisterCompany"; }
      };

      class Purchase : public MoldLocation{
      public:
         Purchase(MoldProperties& mp):MoldLocation(mp){}
         ~Purchase(){ DTORF("~Purchase"); }
         void GetMold(){
            cout << "    Acquire " << _mp.shape->Name() <<" mold via purchase.\n";
         }
         const string Name()const{ return "purchase"; }
      };

      class Mill : public MoldLocation{
      public:
         Mill(MoldProperties& mp):MoldLocation(mp){}
         ~Mill(){ DTORF("~Mill"); }
         void GetMold(){
            cout << "    Create " << _mp.shape->Name() <<" mold from mill with " << _mp.numCavities << " cavities - steps: " << _mp.shape->MillSteps() <<".\n";
         }
         const string Name()const{ return "mill"; }
      };

      MoldLocation* MoldLocation::GetInstance(MoldProperties& mp){
         if(mp.moldLoc=="inventory") return new Inventory(mp);  
         else if(mp.moldLoc=="sisterCompany") return new SisterCompany(mp);  
         else if(mp.moldLoc=="purchase") return new Purchase(mp);  
         else if(mp.moldLoc=="mill") return new Mill(mp);  
         else cout << "<>Can't find place |" << mp.moldLoc << "| to get |" << mp.shape->Name() << "| mold from with |" << mp.finish << "| finish, defaulting to smooth duck from inventory." << endl;
         return nullptr;
      }
      const int NUM_MOLD_LOC = 4;
      MoldLocation* MoldLocation::SetupChain(const MoldProperties& mpIn){
         string chain[] = { "inventory", "sisterCompany", "purchase", "mill" }; // ToDo: scalability test, try deleting/adding 
         MoldProperties mp[NUM_MOLD_LOC];
         for(int i=0; i<NUM_MOLD_LOC; i++){
            mp[i] = mpIn;
            mp[i].moldLoc = chain[i];
         }
         MoldLocation* moldLoc = MoldLocation::GetInstance(mp[0]);
         MoldLocation* current = moldLoc;
         for(size_t i=1; i<NUM_MOLD_LOC; i++) {
            MoldLocation* next = MoldLocation::GetInstance(mp[i]);
            current->SetSuccessor(next);
            current = next;
         }
         return moldLoc;   
      }
   }

   namespace factory_method {	// DP 4.

      // Seam point - add another class.

   }


   namespace template_method {	// DP 3.

      using namespace abstract_factory;
      using namespace adapter;
      using namespace chain_of_resp;
      using namespace decorator;
      using namespace legacy_classes;
      using namespace strategy;

      class ProcessOrder{
      public:
         ProcessOrder(Order& order, InjectionLine* injLine):_order(order), _injectionLine(injLine){
         }
         virtual ~ProcessOrder(){
            delete _mold; _mold=nullptr;
            delete _platform; _platform=nullptr; 
            delete _moldLocation; _moldLocation=nullptr;
            cout<<"\n";
            delete _injMat; _injMat=nullptr;  
            cout<<"\n";
            delete _cavity; _cavity=nullptr;
            cout<<"\n";            
            delete _runTimeEstimator; _runTimeEstimator=nullptr;
            delete _cleaner; _cleaner=nullptr;
            DTORF("~template_method::ProcessOrder\n");
         }

         static ProcessOrder* ProcessOrder::GetInstance(Order& order, InjectionLine* injLine){
            // seam point
            return new ProcessOrder(order, injLine);
         }

         // Seam point - add another polymorphic step.
         virtual void Ship(){ cout << "    Ship to \"" << _order["address"] << "\".\n"; }

         virtual void GetMold(){
            SetDefaultAndReport(_order, "mold", string("duck"), "    <>Unknown mold |" + _order["mold"] + "| defaulting to 'duck'.");
            //legacy_classes::defaulting( _order, string("mold"), string("duck") );
            _shape = Shape::GetInstance( _order["mold"] );

#ifdef _DEBUG_         
            cout<<" ==>"<< endl;
            for(auto& i : _order) cout<<"\torder["<<i.first<<"] = "<<i.second<<endl;
            cout<<" ==>"<< endl;
#endif       
            bool validMoldLoc = is_valid( _order, string("moldLoc") );
            if( !validMoldLoc ){
               basic_string<char> moldLocStr = "    <>Can't find place |" + _order["moldLoc"] + "| to get |" + _order["mold"] + 
                  "| mold from with |" + _order["finish"] + "| finish, defaulting to smooth duck from inventory.";                                    
               SetDefaultAndReport(_order, "moldLoc", string("inventory"), moldLocStr);
            }
            SetDefaultAndReport(_order, "finish", string("smooth"), "");  // ToDo: What if moldLoc not set, but finish is set??

            _platform = Platform::GetInstance(_order["finish"], _shape); 

            MoldProperties mp(_order, _injectionLine->GetIJM(), _shape, _platform);

            _moldLocation = MoldLocation::SetupChain(mp);
            if( validMoldLoc ) _moldLocation->Get(mp.moldLoc);      

            if( validMoldLoc && _order["moldLoc"]=="mill" ){
               const string& metal = _injectionLine->GetIJM()->Metal();
               int numCavities = _injectionLine->GetIJM()->NumCavities();
               string pluralStr = numCavities > 1 ? "s" : "";
               cout << "      using " << _platform->Name() << " to mill " << to_lower(metal) << " block into " << numCavities << " " << _shape->Name() << " shape" << pluralStr << " with " << _order["finish"] << " finish.\n";
            }
         }

         virtual void InsertTags(){
            _cavity = Cavity::GetInstance( _order["tags"] );
            if(_cavity!=nullptr) cout << *_cavity;
         }

         const bool is_valid(Order& order, string& key){
            return ( order.find(key)==order.end() || order[key]=="" ) ? false : true; 
         }

         virtual void MixAndLoadInjectionMaterial(){
            _injMat = InjectionMaterial::GetInstance(_order, _shape);
            cout << "    Load plastic bin with " << _order["plastic"];
            if( is_valid(_order, string("color") ) ) cout << " and color bin with " << _order["color"];
            cout << ".\n";
            cout << "      Recipe: " << _injMat->GetRecipeStr() << " = " << _shape->Volume() << " cc." << endl; //_order["plastic"] << "(<vol>) " << _order["color"] << "(<vol>) <additive1>(<vol>)... = <shape vol> cc.\n";
            int numCavities = _injectionLine->GetIJM()->NumCavities();
            string pluralStr = numCavities > 1 ? "ies" : "y";
            assert(_shape!=nullptr);
            cout << "      Volume: " << _shape->Name() << "(" << _shape->Volume() << ") * " << numCavities << " cavit" << pluralStr <<" = " << numCavities * _shape->Volume() << " cc.\n";
         }

         string to_lower(const string& inStr){
            string out = inStr;
            std::transform(out.begin(), out.end(), out.begin(), ::tolower);
            return out;
         }

         virtual void RunMoldingCycle(){
            Plastic *plastic = _injMat->GetPlastic();
            int orderSize = stoi(_order["size"]);
            int runSize = orderSize / _injectionLine->GetIJM()->NumCavities();
            int numCavities = _injectionLine->GetIJM()->NumCavities();
            const string& metal = _injectionLine->GetIJM()->Metal();

            _runTimeEstimator = RunTimeEstimator::GetInstance(stoi(_order["size"]));
            if(_runTimeEstimator!=nullptr){
               int estimatedRunTime = _runTimeEstimator->GetEstimate(numCavities, to_lower(metal), _shape);
               string pluralStr = estimatedRunTime > 1 ? "s" : "";
               cout << "    Cycle " << _injectionLine->GetIJM()->Name() << " for " << plastic->Name() << " " << runSize << " times, estimated run time = " << estimatedRunTime << " hour" << pluralStr << ".\n";
            }

            string plaName = plastic->Name();
            cout << plastic->GetInjectionProcess() << endl;
/*
            if(plaName=="ABS")
               cout << "      Close - mix - heat to 440 - inject at 125 PSI - cool to 360 - separate - progressive eject.";
            else if( plaName.find("Poly") == 0 )
               cout << "      Close - mix - heat to 350 - inject at 90 PSI - cool to 290 - separate - smooth eject.";
            else if(plaName=="PET")
               cout << "      Close - mix - heat to 404 - inject at 110 PSI - cool to 340 - separate - smooth eject.";
            else assert(false);
*/
            cout << endl;

            _injectionLine->GetPartsBin()->NotifyFull();
         }

         virtual void CleanMold(){
            const string& metal = _injectionLine->GetIJM()->Metal();
            _cleaner = MoldCleaner::GetInstance( _injMat->GetPlastic() );
            _cleaner->Clean(metal);
         }
         //10.1 Get mold
         //   10.2 Insert tags into mold
         //   10.3 Load plastic, color die, and additive bins
         //   10.4 Simulate cycling the IJM for the specified plastic <run size> times (see specs 20-23)
         //   10.4.1 Just once, specify the injection cycle (depends on plastic type)
         //   10.4.2 Just once, simulate a full parts bin asking upline machines to pause
         //   10.5 Clean the mold (ignore molds that are used up, clean them all)
      protected: 
         Order _order;
         InjectionLine *_injectionLine;
         InjectionMaterial* _injMat;
         Mold *_mold;   
         Cavity *_cavity;
         Shape* _shape;
         RunTimeEstimator *_runTimeEstimator;
         Platform *_platform;
         MoldLocation *_moldLocation;
         MoldCleaner* _cleaner;
      };
      // Seam point - add another constant step.
      // Seam point - convert a constant step into a polymorphic step.

   }

   using namespace template_method;

   class InjectionMoldingShop{
   public:
      InjectionMoldingShop(Order& order){
         _order = order;
         SetDefaultValues(_order);
         int orderSize = atoi( string(_order["size"] ).c_str() );
         _injectionLine = new InjectionLine(orderSize, _order["packager"], _order["stuffer"]);
         _processOrder = ProcessOrder::GetInstance(_order, _injectionLine);
      }
      ~InjectionMoldingShop(){  
         delete _injectionLine;
         delete _processOrder;
      }
      void SetDefaultValues(Order& order){
         SetDefaultAndReport(order, "plastic", string("ABS"),   "  <>Unknown plastic |" + order["plastic"] + "| defaulting to 'ABS'.");
         SetDefaultAndReport(order, "size", string("100"),      "  <>No size specified, defaulting to 100.");
         int size = stoi(order["size"]);
         if( size > MaxOrderSize ){
            string maxOrder = to_string(MaxOrderSize);
            cout << "  <>Size exceeds mold lifetime |" << size << "| defaulting to HugeOrder of " << maxOrder <<".\n";
            order["size"] = maxOrder;
         }
         SetDefaultAndReport(order, "packager", string("Bulk"), "  <>Unknown packager |" + order["packager"] + "| defaulting to 'None'.");
         SetDefaultAndReport(order, "stuffer", string("Air"), "  <>Unknown stuffer |" + order["stuffer"] + "| defaulting to 'None'.");
      }

      void Run(){
         _injectionLine->Setup();

         cout << "  Process order:" << endl;
         _processOrder->GetMold();         
         _processOrder->InsertTags();
         _processOrder->MixAndLoadInjectionMaterial();
         _processOrder->RunMoldingCycle();
         _processOrder->CleanMold();
         _processOrder->Ship();
      }

   private:
      Order _order;
      InjectionLine *_injectionLine;
      ProcessOrder *_processOrder;
   };

   void process(Order& order) {
      // Fill in the namespaces above with your design pattern class hierarchies.
      // Call your order processing class from here <myProcess>->run(order);
#ifndef _EMPTY_PROCESS_
      InjectionMoldingShop injMoldShop(order);
      injMoldShop.Run();
#endif
   }

   pair<string,string> parse(string line) {
      char key[83];
      char val[83] = {0};

      sscanf(line.c_str(), "%s", key);

      char* equalSign = (char *)strchr(line.c_str(), '=');
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

   Order getCompleteOrder(FILE* orderFilePtr) {
      Order order;
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
      cout << "<<< final original >>>\n";

      FILE* orderFilePtr = fopen(orderFile.c_str(), "r");

      int counter = 0;
      for(;;) {	// Get and process each order.
         Order order = getCompleteOrder(orderFilePtr);
         if(order.size() == 0)
            break;
         process(order);        
         //if(++counter==2) break;
      }
      cout<<endl;
   }

} // final_design

#endif /* FINAL_FINAL_DESIGN_H_ */

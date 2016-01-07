namespace abstract_factory1 {// DP 9.

class IJM {
public:
	IJM() {}
	virtual ~IJM() { DTORF("~IJM\n"); }
public:
	virtual string setup() { return "IJM_base"; }
};
class IJM_110 : public IJM {					// PilotOrder.
public:
	IJM_110() {}
	~IJM_110() { DTORF("~IJM_110 "); }
public:
	string setup() { return "IJM_110"; }
};
class IJM_120 : public IJM {					// SmallOrder.
public:
	IJM_120() {}
	~IJM_120() { DTORF("~IJM_120 "); }
public:
	string setup() { return "IJM_120"; }
};
class IJM_210 : public IJM {					// MediumOrder.
public:
	IJM_210() {}
	~IJM_210() { DTORF("~IJM_210 "); }
public:
	string setup() { return "IJM_210"; }
};
class IJM_220 : public IJM {					// LargeOrder.
public:
	IJM_220() {}
	~IJM_220() { DTORF("~IJM_220 "); }
public:
	string setup() { return "IJM_220"; }
};
// Seam point - add another IJM type.

class Block {
public:
	const unsigned cavities;
public:
	Block(unsigned cavities) : cavities(cavities) {}
	virtual ~Block() { DTORF("~Block\n"); }
public:
	string setup() { return metal() + cavitiesAsString(); }
	virtual string metal() { return "metal"; }
	string cavitiesAsString() {
		char cav[] = "( )";
		cav[1] = cavities + '0';
		return string(cav);
	}
};
class Aluminum : public Block {
public:
	Aluminum(unsigned cavities) : Block(cavities) {}
	~Aluminum() { DTORF("~Aluminum "); }
public:
	string metal() { return "Aluminum"; }
};
class Steel : public Block {
public:
	Steel(unsigned cavities) : Block(cavities) {}
	~Steel() { DTORF("~Steel "); }
public:
	string metal() { return "Steel"; }
};
// Seam point - add another Block type.

class ConveyerBelt {
public:
	ConveyerBelt() {}
	virtual ~ConveyerBelt() { DTORF("~ConveyerBelt\n"); }
public:
	virtual string setup() { return "Base Conveyer belt"; }
};
class LinearBelt : public ConveyerBelt {
public:
	LinearBelt() {}
	~LinearBelt() { DTORF("~LinearBelt "); }
public:
	string setup() {return "Linear conveyer belt"; }
};
class YSplitBelt : public ConveyerBelt {
public:
	YSplitBelt() {}
	~YSplitBelt() { DTORF("~YSplitBelt "); }
public:
	string setup() {return "Y-Split conveyer belt"; }
};
// Seam point - add another ConveyerBelt type.

class PartsBin {
public:
	PartsBin() {}
	virtual ~PartsBin() { DTORF("~PartsBin\n"); }
public:
	virtual string setup() { return "PartsBin_base"; }
};
class CardboardBox : public PartsBin {
public:
	CardboardBox() {}
	~CardboardBox() { DTORF("~CardboardBox "); }
public:
	string setup() { return "CardboardBox"; }
};
class ShellBox : public PartsBin {
public:
	ShellBox() {}
	~ShellBox() { DTORF("~ShellBox "); }
public:
	string setup() { return "ShellBox"; }
};
class PalletBox : public PartsBin {
public:
	PalletBox() {}
	~PalletBox() { DTORF("~PalletBox "); }
public:
	string setup() { return "PalletBox"; }
};
// Seam point - add another PartsBin type.

class InjectionLine {	// If the families are varying...
public:
	InjectionLine() {}
	virtual ~InjectionLine() { DTORF("~abstract_factory::InjectionLine\n"); }
public:
	virtual IJM* createIJM(map<string,string>& order) {
		return new IJM;
	}
	virtual Block* createBlock(map<string,string>& order) {
		return new Block(0);
	}
	virtual ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new ConveyerBelt;
	}
	virtual PartsBin* createPartsBin(map<string,string>& order) {
		return new PartsBin;
	}
public:
	static InjectionLine* createInjectionLine(map<string,string>& order);
};
class PilotOrder : public InjectionLine {
public:	virtual ~PilotOrder() { DTORF("~PilotOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_110;
	}
	Block* createBlock(map<string,string>& order) {
		return new Aluminum(1);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new LinearBelt;
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new CardboardBox;
	}
};
class SmallOrder : public InjectionLine {
public:	virtual ~SmallOrder() { DTORF("~SmallOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_120;
	}
	Block* createBlock(map<string,string>& order) {
		return new Aluminum(2);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new YSplitBelt;
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new ShellBox;
	}
};
class MediumOrder : public InjectionLine {
public:	virtual ~MediumOrder() { DTORF("~MediumOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_210;
	}
	Block* createBlock(map<string,string>& order) {
		return new Steel(1);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new LinearBelt;
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new PalletBox;
	}
};
class LargeOrder : public InjectionLine {
public:	virtual ~LargeOrder() { DTORF("~LargeOrder "); }
public:
	IJM* createIJM(map<string,string>& order) {
		return new IJM_220;
	}
	Block* createBlock(map<string,string>& order) {
		return new Steel(2);
	}
	ConveyerBelt* createConveyerBelt(map<string,string>& order) {
		return new YSplitBelt;
	}
	PartsBin* createPartsBin(map<string,string>& order) {
		return new PalletBox;
	}
};
// Seam point - add another InjectionLine family.

InjectionLine* InjectionLine::createInjectionLine(map<string,string>& order) {
	unsigned size = atoi(order["size"].c_str());

	if(size <=  10000)	return new PilotOrder;
	if(size <=  20000)	return new SmallOrder;
	if(size <=  50000)	return new MediumOrder;
	if(size <= 100000)	return new LargeOrder;
	// Seam point - add another order size.

	return new InjectionLine;
}

} // abstract_factory1

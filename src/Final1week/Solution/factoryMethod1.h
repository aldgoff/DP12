namespace factory_method_1 {	// DP 4.

using namespace observer;

class Packager : public BinObserver {	// If the classes are varying...
public:
	Packager(BinSubject* bin) : BinObserver(bin) {}
	virtual ~Packager() { DTORF("~factory_method::Packager "); }
public:
	virtual string wrap() { return "<packager>"; }
public:
	virtual void update(BinSubject* bin) {}
public:
	static Packager* makeObject(map<string,string>& map, BinSubject* bin);
};
// class Bulk is the null class.
class ShrinkWrap : public Packager {
public:
	ShrinkWrap(BinSubject* bin) : Packager(bin) {}
	~ShrinkWrap() { DTORF("~ShrinkWrap "); }
public:
	string wrap() { return "ShrinkWrap"; }
public:
	void update(BinSubject* bin) {
		cout << "      Shrink wrap packager paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};
class HardPack : public Packager {
public:
	HardPack(BinSubject* bin) : Packager(bin) {}
	~HardPack() { DTORF("~HardPack "); }
public:
	string wrap() { return "HardPack"; }
public:
	void update(BinSubject* bin) {
		cout << "      Hard pack packager paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};
// Seam point - add another class.

Packager* Packager::makeObject(map<string,string>& order, BinSubject* bin) {
	if(order["packager"] == "Bulk")			return 0;
	if(order["packager"] == "ShrinkWrap")	return new ShrinkWrap(bin);
	if(order["packager"] == "HardPack")		return new HardPack(bin);
	// Seam point - add another class.

	legacy_classes::defaulting(order, "packager", "None");
	return 0;
}

} // factory_method_1

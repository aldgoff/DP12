namespace factory_method_2 {	// DP 4.

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
class ZipLock : public Packager {
public:
	ZipLock(BinSubject* bin) : Packager(bin) {}
	~ZipLock() { DTORF("~ZipLock "); }
public:
	string wrap() { return "ZipLock"; }
public:
	void update(BinSubject* bin) {
		cout << "      ZipLock packager paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};

Packager* Packager::makeObject(map<string,string>& order, BinSubject* bin) {
	if(order["packager"] == "Bulk")			return 0;
	if(order["packager"] == "ShrinkWrap")	return new ShrinkWrap(bin);
	if(order["packager"] == "HardPack")		return new HardPack(bin);
	// Seam point - add another class.
	if(order["packager"] == "ZipLock")		return new ZipLock(bin);

	legacy_classes::defaulting(order, "packager", "None");
	return 0;
}

// Seam point - add another Observer.

class Stuffer : public BinObserver {	// If the classes are varying...
public:
	Stuffer(BinSubject* bin) : BinObserver(bin) {}
	virtual ~Stuffer() { DTORF("~factory_method::Stuffer "); }
public:
	virtual string fill() { return "<stuffer>"; }
	void update(BinSubject* bin) {}
public:
	static Stuffer* makeObject(map<string,string>& map, BinSubject* bin);
};
// class Air is the null class.
class Popcorn : public Stuffer {
public:
	Popcorn(BinSubject* bin) : Stuffer(bin) {}
	~Popcorn() { DTORF("~Popcorn "); }
public:
	string fill() { return "styrene popcorn"; }
	void update(BinSubject* bin) {
		cout << "      Popcorn stuffer paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};
class Bubblewrap : public Stuffer {
public:
	Bubblewrap(BinSubject* bin) : Stuffer(bin) {}
	~Bubblewrap() { DTORF("~Bubblewrap "); }
public:
	string fill() { return "bubble wrap"; }
	void update(BinSubject* bin) {
		cout << "      Bubblewrap stuffer paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};
// Seam point - add another class.
class Foam : public Stuffer {
public:
	Foam(BinSubject* bin) : Stuffer(bin) {}
	~Foam() { DTORF("~Foam "); }
public:
	string fill() { return "expanding foam"; }
	void update(BinSubject* bin) {
		cout << "      Foam stuffer paused while "
			 << bin->name << " parts bin was swapped.\n";
	}
};

Stuffer* Stuffer::makeObject(map<string,string>& order, BinSubject* bin) {
	if(order["stuffer"] == "Air")			return 0;	// Null, no Observer.
	if(order["stuffer"] == "Popcorn")		return new Popcorn(bin);
	if(order["stuffer"] == "Bubblewrap")	return new Bubblewrap(bin);
	// Seam point - add another class.
	if(order["stuffer"] == "Foam")	return new Foam(bin);

	legacy_classes::defaulting(order, "stuffer", "None");
	return 0;
}

} // factory_method_2

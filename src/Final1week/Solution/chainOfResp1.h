namespace chain_of_resp_1 {		// DP 7.

using namespace bridge_1;

class Mold {
protected:
	Mold* successor;
public:
	Mold(Mold* successor=0) : successor(successor) {}
	virtual ~Mold() { DTORF("~CofR::Mold "); }
public:
	virtual Shape* from(map<string,string>& order) {
		Shape* shape = Shape::getShape(order);

		legacy_classes::defaulting(order, "moldLoc", "inventory");

		cout << "  Pull " << order["mold"] << " mold "
			 << "from " << order["moldLoc"] << ".\n";

		return shape;
	}
public:
	static Mold* acquireMold(map<string,string>& order);
};
class Inventory : public Mold {
public:
	Inventory(Mold* successor=0) : Mold(successor) {}
	~Inventory() { DTORF("~Inventory "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "inventory") {
			Shape* shape = Shape::getShape(order);
			cout << "  Pull " << order["mold"] << " mold from inventory.\n";
			return shape;
		}
		else if(successor != 0)
			return successor->from(order);
		else
			return Mold::from(order);	// Default.
	}
};
class SisterCompany : public Mold {
public:
	SisterCompany(Mold* successor=0) : Mold(successor) {}
	~SisterCompany() { DTORF("~SisterCompany "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "sisterCompany") {
			Shape* shape = Shape::getShape(order);
			cout << "  Borrow " << order["mold"] << " mold from sister company.\n";
			return shape;
		}
		else if(successor != 0)
			return successor->from(order);
		else
			return Mold::from(order);	// Default.
	}
};
// Seam points - insert another responder.
class Mill : public Mold {
public:
	Mill(Mold* successor=0) : Mold(successor) {}
	~Mill() { DTORF("~Mill "); delete successor; }
public:
	Shape* from(map<string,string>& order) {
		string place = order["moldLoc"];
		if(place == "mill") {
			Shape* shape = Shape::getShape(order);
			cout << "  Create " << order["mold"] << " mold from mill ";
			cout << "with " << order["cavities"];	// Added to order by TM.setupLine().
			int cavities = atoi(order["cavities"].c_str());
			if(cavities == 1)
				cout << " cavity - ";
			else
				cout << " cavities - ";
			shape->steps();
			shape->mill(order);
			return shape;
		}
		else if(successor != 0)
			return successor->from(order);
		else
			return Mold::from(order);	// Default.
	}
};

Mold* Mold::acquireMold(map<string,string>& order) {
	return	new Inventory(
			new SisterCompany(
			// Seam point - mill likely location of last resort.
			new Mill(
			new Mold(
		))));
}

// Seam points - add another responder.

} // chain_of_resp_1

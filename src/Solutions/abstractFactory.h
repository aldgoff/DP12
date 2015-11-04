/*
 * abstractFactory.h
 *
 *  Created on: Nov 3, 2015
 *      Author: aldgoff
 *
 * Varies: If the families are varying, use the Abstract Factory pattern.
 *
 * Desc: Provide an interface for creating families of related or
 * 		 dependent objects without specifying their concrete classes.
 *
 * Category: Creational
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Abstract_Factory
 *  	http://www.dofactory.com/net/abstract-factory-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter11
 *  	http://sourcemaking.com/design_patterns/abstract_factory
 */

#ifndef SOLUTIONS_ABSTRACTFACTORY_H_
#define SOLUTIONS_ABSTRACTFACTORY_H_

/* Consider a wild life simulation with different animals and continents.
 * The major complexity is the relationships between the animals.
 * Animals are grouped according to their continent, so the simulation
 * runs on a continent by continent basis.
 * Legacy namespace has two continents, each with one prey and one predator:
 *   1) North America
 *   	1.1 Bison
 *   	1.2 Wolf
 *   2) Africa
 *   	2.1 Zebra
 *   	2.2 Lion
 * Problem namespace adds another continent:
 *   3) South America
 *   	3.1 Llama
 *   	3.2 Puma
 * Prey graze, mate, and evade; predators eat, mate and hunt.
 * With a procedural approach, scaling is quadratic.
 * The effort to add a relationship scales as the number of continents.
 * The effort to add a continent scales as the number of relationships.
 * Use the Abstract Factory design pattern to reduce coupling,
 * enhance cohesion, and avoid quadratic scaling.
 */

namespace solution {

class Herbivore;
class Carnivore;

class Herbivore { // An interface class in the AF DP.
protected:
	string name;
public:
	Herbivore(const string& name="") : name(name) {}
	virtual ~Herbivore() {
		DTOR("~Herbivore ", Lecture);
	};
public:
	string Name() { return name; }
public:
	virtual void graze() {}
	virtual void mate(Herbivore* other) {}
	virtual void evade(Carnivore* other) {}
};
class Bison : public Herbivore {
public:
	Bison() : Herbivore("Bison") {}
	~Bison() {
		DTOR("~Bison ", Lecture);
	};
public:
	void graze() {
		cout << "  " << name << " eats grass.\n";
	}
	void mate(Herbivore* other) {
		cout << "  Mate with " << other->Name() << " herd.\n";
	}
	void evade(Carnivore* other);
};
class Wildebeest : public Herbivore {
public:
	Wildebeest() : Herbivore("Wildebeest") {}
	~Wildebeest() {
		DTOR("~Wildebeest ", Lecture);
	};
public:
	void graze() {
		cout << "  " << name << " eats thrash.\n";
	}
	void mate(Herbivore* other) {
		cout << "  Breed with " << other->Name() << ".\n";
	}
	void evade(Carnivore* other);
};
class Llama : public Herbivore {
public:
	Llama() : Herbivore("Llama") {}
	~Llama() {
		DTOR("~Llama ", Lecture);
	};
public:
	void graze() {
		cout << "  " << name << " eats grain.\n";
	}
	void mate(Herbivore* other) {
		cout << "  Breed with " << other->Name() << ".\n";
	}
	void evade(Carnivore* other);
};
// Seam point - add another Herbivore.

class Carnivore { // An interface class in the AF DP.
protected:
	string name;
public:
	Carnivore(const string& name="") : name(name) {}
	virtual ~Carnivore() {
		DTOR("~Carnivore ", Lecture);
	};
public:
	string Name() { return name; }
public:
	virtual void eat(Herbivore* other) {}
	virtual void mate(Carnivore* other) {}
	virtual void hunt() {}
};
class Wolf : public Carnivore {
public:
	Wolf() : Carnivore("Wolf") {}
	~Wolf() {
		DTOR("~Wolf ", Lecture);
	};
public:
	void eat(Herbivore* other) {
		cout << "  " << name << " eats a "
			 << other->Name() << ".\n";
	}
	void mate(Carnivore* other) {
		cout << "  Mate for life with another "
			 << other->Name() << ".\n";
	}
	void hunt() {
		cout << "  Hunt in packs.\n";
	}
};
class Lion : public Carnivore {
public:
	Lion() : Carnivore("Lion") {}
	~Lion() {
		DTOR("~Lion ", Lecture);
	};
public:
	void eat(Herbivore* other) {
		cout << "  " << name << " eats a "
			 << other->Name() << ".\n";
	}
	void mate(Carnivore* other) {
		cout << "  Mate with another "
			 << other->Name() << ".\n";
	}
	void hunt() {
		cout << "  Stalk prey.\n";
	}
};
class Puma : public Carnivore {
public:
	Puma() : Carnivore("Puma") {}
	~Puma() {
		DTOR("~Puma ", Lecture);
	};
public:
	void eat(Herbivore* other) {
		cout << "  " << name << " eats a "
			 << other->Name() << ".\n";
	}
	void mate(Carnivore* other) {
		cout << "  Mate with another "
			 << other->Name() << ".\n";
	}
	void hunt() {
		cout << "  Chase prey.\n";
	}
};
// Seam point - add another Carnivore.

void Bison::evade(Carnivore* other) {
	cout << "  Flee from pack of "
		 << other->Name() << "s.\n";
}
void Wildebeest::evade(Carnivore* other) {
	cout << "  Flee from pride of "
		 << other->Name() << "s.\n";
}
void Llama::evade(Carnivore* other) {
	cout << "  Flee from a "
		 << other->Name() << ".\n";
}

class Continent { // Abstract Factory design pattern.
public: virtual ~Continent() {
	DTOR("~Continent\n", Lecture);
};
public:
	virtual Herbivore* createHerbivore() {
		return new Herbivore;
	}
	virtual Carnivore* createCarnivore() {
		return new Carnivore;
	}
public:
	static Continent* create(const string& criteria);
};
class NorthAmerica : public Continent {
public: ~NorthAmerica() {
	DTOR("~NorthAmerica ", Lecture);
};
public:
	Herbivore* createHerbivore() {
		return new Bison;
	}
	Carnivore* createCarnivore() {
		return new Wolf;
	}
};
class Africa : public Continent {
public: ~Africa() {
	DTOR("~Africa ", Lecture);
};
public:
	Herbivore* createHerbivore() {
		return new Wildebeest;
	}
	Carnivore* createCarnivore() {
		return new Lion;
	}
};
class SouthAmerica : public Continent {
public: ~SouthAmerica() {
	DTOR("~SouthAmerica ", Lecture);
};
public:
	Herbivore* createHerbivore() {
		return new Llama;
	}
	Carnivore* createCarnivore() {
		return new Puma;
	}
};
// Seam point - add another Continent.

Continent* Continent::create(const string& criteria) {
	if(criteria == "NorthAmerica")	return new NorthAmerica;
	if(criteria == "Africa")		return new Africa;
	if(criteria == "SouthAmerica")	return new SouthAmerica;
	// Seam point - insert another criteria.
	return new Continent;
}

class Simulation { // Client code.
	Herbivore* herbivore;
	Carnivore* carnivore;
public:
	Simulation(Continent* continent)
		: herbivore(continent->createHerbivore())
		, carnivore(continent->createCarnivore())
	{}
	virtual ~Simulation() {
		DTOR("~Simulation ", Lecture);
		delete herbivore;
		delete carnivore;
	}
public:
	void run() {
		herbivore->graze();
		herbivore->mate(herbivore);
		herbivore->evade(carnivore);
		carnivore->eat(herbivore);
		carnivore->mate(carnivore);
		carnivore->hunt();
	}
};

void demo() { // Test variations.
	string criteria[] = { "NorthAmerica", "Africa",
						  "SouthAmerica" };
	for(size_t i=0; i<COUNT(criteria); i++) {
		Continent* factory = Continent::create(criteria[i]);
		Simulation* world = new Simulation(factory);
		delete factory;
		world->run();

		delete world;
		cout << endl << endl;
	}
}

} // solution

#endif /* SOLUTIONS_ABSTRACTFACTORY_H_ */

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

#ifndef PROBLEMS_ABSTRACTFACTORY_H_
#define PROBLEMS_ABSTRACTFACTORY_H_

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

namespace legacy {

enum Continent {
	NorthAmerica,
	Africa,
	// Seam point - insert another continent.
};
string ids[] = {
	"North America",
	"Africa",
	// Seam point - insert another continent.
};

// North American animals.
class Wolf;

class Bison {
	string 	name;
public:
	Bison() : name("Bison") {}
	~Bison() {
		DTOR("~Bison ", Lecture);
	}
public:
	string Name() { return name; };
	void graze() {
		cout << "  " << name << " eats grass.\n";
	}
	void mate(Bison* bison) {
		cout << "  Mate with " << bison->Name() << " herd.\n";
	}
	void evade(Wolf* wolf);
};
class Wolf {
	string 	name;
public:
	Wolf() : name("Wolf") {}
	~Wolf() {
		DTOR("~Wolf ", Lecture);
	}
public:
	string Name() { return name; };
	void eat(Bison* bison) {
		cout << "  " << name << " eats a "
			 << bison->Name() << ".\n";
	}
	void mate(Wolf* wolf) {
		cout << "  Mate for life with another "
			 << wolf->Name() << ".\n";
	}
	void hunt() {
		cout << "  Hunt in packs.\n";
	}
};

void Bison::evade(Wolf* wolf) {
	cout << "  Flee from pack of "
		 << wolf->Name() << "s.\n";
}

// African animals.
class Lion;

class Wildebeest {
	string 	name;
public:
	Wildebeest() : name("Wildebeest") {}
	~Wildebeest() {
		DTOR("~Wildebeest ", Lecture);
	}
public:
	string Name() { return name; };
	void graze() {
		cout << "  " << name << " eats thrash.\n";
	}
	void mate(Wildebeest* wildebeest) {
		cout << "  Breed with "
			 << wildebeest->Name() << ".\n";
	}
	void evade(Lion* lion);
};
class Lion {
	string 	name;
public:
	Lion() : name("Lion") {}
	~Lion() {
		DTOR("~Lion ", Lecture);
	}
public:
	string Name() { return name; };
	void eat(Wildebeest* wildebeest) {
		cout << "  " << name << " eats a "
			 << wildebeest->Name() << ".\n";
	}
	void mate(Lion* lion) {
		cout << "  Mate with another "
			 << lion->Name() << ".\n";
	}
	void hunt() {
		cout << "  Stalk prey.\n";
	}
};

void Wildebeest::evade(Lion* lion) {
	cout << "  Flee from pride of "
		 << lion->Name() << "s.\n";
}

// Seam point - add animals for another continent.

class AnimalWorld {
	Bison*		bison;		// North America animals.
	Wolf*		wolf;
	Wildebeest*	wildebeest;	// Africa animals.
	Lion*		lion;
	// Seam point - insert another continent.
public:
	AnimalWorld()
	  : bison(new Bison), wolf(new Wolf),
		wildebeest(new Wildebeest), lion(new Lion)
	// Seam point - insert another continent.
	{}
	~AnimalWorld() {
		delete bison;
		delete wolf;
		delete wildebeest;
		delete lion;
		// Seam point - insert another continent.
		DTOR("~AnimalWorld\n", Lecture);
	 }
public:
	void runFoodChain(Continent continent) {
		cout << " " << ids[continent] << " Continent:\n";
		switch(continent) {
		case NorthAmerica:
			bison->graze();
			bison->mate(bison);
			bison->evade(wolf);
			wolf->eat(bison);
			wolf->mate(wolf);
			wolf->hunt();
			break;
		case Africa:
			wildebeest->graze();
			wildebeest->mate(wildebeest);
			wildebeest->evade(lion);
			lion->eat(wildebeest);
			lion->mate(lion);
			lion->hunt();
			break;
		// Seam point - insert another continent.
		default:
			throw "OOPS!";
		}
	}
};

void demo() {
	AnimalWorld* simulation = new AnimalWorld;
	Continent continents[] = { NorthAmerica, Africa };
	for(size_t i=0; i<COUNT(continents); i++) {
		simulation->runFoodChain(continents[i]);
		cout << endl;
	}

	delete simulation;
	cout << endl;
}

} // legacy

namespace problem {

enum Continent {
	NorthAmerica,
	Africa,
	SouthAmerica,
	// Seam point - insert another continent.
};
string ids[] = {
	"North America",
	"Africa",
	"South America",
	// Seam point - insert another continent.
};

// North American animals.
class Wolf;

class Bison {
	string 	name;
public:
	Bison() : name("Bison") {}
	~Bison() {
		DTOR("~Bison ", Lecture);
	}
public:
	string Name() { return name; };
	void graze() {
		cout << "  " << name << " eats grass.\n";
	}
	void mate(Bison* bison) {
		cout << "  Mate with " << bison->Name() << " herd.\n";
	}
	void evade(Wolf* wolf);
};
class Wolf {
	string 	name;
public:
	Wolf() : name("Wolf") {}
	~Wolf() {
		DTOR("~Wolf ", Lecture);
	}
public:
	string Name() { return name; };
	void eat(Bison* bison) {
		cout << "  " << name << " eats a " << bison->Name() << ".\n";
	}
	void mate(Wolf* wolf) {
		cout << "  Mate for life with another " << wolf->Name() << ".\n";
	}
	void hunt() {
		cout << "  Hunt in packs.\n";
	}
};

void Bison::evade(Wolf* wolf) {
	cout << "  Flee from pack of "
		 << wolf->Name() << "s.\n";
}

// African animals.
class Lion;

class Wildebeest {
	string 	name;
public:
	Wildebeest() : name("Wildebeest") {}
	~Wildebeest() {
		DTOR("~Wildebeest ", Lecture);
	}
public:
	string Name() { return name; };
	void graze() {
		cout << "  " << name << " eats thrash.\n";
	}
	void mate(Wildebeest* wildebeest) {
		cout << "  Breed with "
			 << wildebeest->Name() << ".\n";
	}
	void evade(Lion* lion);
};
class Lion {
	string 	name;
public:
	Lion() : name("Lion") {}
	~Lion() {
		DTOR("~Lion ", Lecture);
	}
public:
	string Name() { return name; };
	void eat(Wildebeest* wildebeest) {
		cout << "  " << name << " eats a "
			 << wildebeest->Name() << ".\n";
	}
	void mate(Lion* lion) {
		cout << "  Mate with another "
			 << lion->Name() << ".\n";
	}
	void hunt() {
		cout << "  Stalk prey.\n";
	}
};

void Wildebeest::evade(Lion* lion) {
	cout << "  Flee from pride of "
		 << lion->Name() << "s.\n";
}

// South American animals.
class Puma;

class Llama {
	string 	name;
public:
	Llama() : name("Llama") {}
	~Llama() {
		DTOR("~Llama ", Lecture);
	}
public:
	string Name() { return name; };
	void graze() {
		cout << "  " << name << " eats grain.\n";
	}
	void mate(Llama* llama) {
		cout << "  Breed with "
			 << llama->Name() << ".\n";
	}
	void evade(Puma* puma);
};
class Puma {
	string 	name;
public:
	Puma() : name("Puma") {}
	~Puma() {
		DTOR("~Puma ", Lecture);
	}
public:
	string Name() { return name; };
	void eat(Llama* llama) {
		cout << "  " << name << " eats a "
			 << llama->Name() << ".\n";
	}
	void mate(Puma* puma) {
		cout << "  Mate with another "
			 << puma->Name() << ".\n";
	}
	void hunt() {
		cout << "  Chase prey.\n";
	}
};

void Llama::evade(Puma* puma) {
	cout << "  Flee from a "
		 << puma->Name() << ".\n";
}

// Seam point - add animals for another continent.

class AnimalWorld {
	Bison*		bison;		// North America animals.
	Wolf*		wolf;
	Wildebeest*	wildebeest;	// Africa animals.
	Lion*		lion;
	Llama*		llama;		// South America animals.
	Puma*		puma;
	// Seam point - insert another continent.
public:
	AnimalWorld()
	  : bison(new Bison), wolf(new Wolf),
		wildebeest(new Wildebeest), lion(new Lion),
		llama(new Llama), puma(new Puma)
	// Seam point - insert another continent.
	{}
	~AnimalWorld() {
		delete bison;
		delete wolf;
		delete wildebeest;
		delete lion;
		delete llama;
		delete puma;
		// Seam point - insert another continent.
		DTOR("~AnimalWorld\n", Lecture);
	 }
public:
	void runFoodChain(Continent continent) {
		cout << " " << ids[continent] << " Continent:\n";
		switch(continent) {
		case NorthAmerica:
			bison->graze();
			bison->mate(bison);
			bison->evade(wolf);
			wolf->eat(bison);
			wolf->mate(wolf);
			wolf->hunt();
			break;
		case Africa:
			wildebeest->graze();
			wildebeest->mate(wildebeest);
			wildebeest->evade(lion);
			lion->eat(wildebeest);
			lion->mate(lion);
			lion->hunt();
			break;
		case SouthAmerica:
			llama->graze();
			llama->mate(llama);
			llama->evade(puma);
			puma->eat(llama);
			puma->mate(puma);
			puma->hunt();
			break;
		// Seam point - insert another continent.
		default:
			throw "OOPS!";
		}
	}
};

void demo() {
	AnimalWorld* simulation = new AnimalWorld;
	Continent continents[] = { NorthAmerica, Africa,
							   SouthAmerica };
	for(size_t i=0; i<COUNT(continents); i++) {
		simulation->runFoodChain(continents[i]);
		cout << endl;
	}

	delete simulation;
	cout << endl;
}

} // problem

#endif /* PROBLEMS_ABSTRACTFACTORY_H_ */

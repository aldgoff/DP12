/*
 * chainOfResp.h
 *
 *  Created on: Sep 26, 2015
 *      Author: aldgoff
 *
 * Varies: If the responders are varying, use the Chain of Responsibility pattern.
 *
 * Desc:
 *
 * Category: whatever
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#ChainOfReponsibility
 *  	http://www.dofactory.com/net/chain-of-responsibility-design-pattern
 *
 *  	http://sourcemaking.com/design_patterns/chain_of_responsibility
 */

#ifndef SOLUTIONS_CHAINOFRESP_H_
#define SOLUTIONS_CHAINOFRESP_H_

/* Consider how to model decision making in a military chain of command.
 * Simple decisions get made by the junior officers,
 * harder ones by the senior officers.
 * Model the major being killed in action.
 */

namespace solution {

class Officer { // Chain of Responsibility design pattern.
protected:
	Officer* superior;
public:
	Officer() : superior(0) {}
	virtual ~Officer() { delete superior;
		DTOR("~Officer\n", Homework);
	}
public:
	void setSuccessor(Officer* next) { superior = next; }
public:
	virtual void makeDecision(int livesAtRisk)=0;
public:
	static Officer* makeObject(const string& criteria);
};
class Lieutenant : public Officer {
public:
	Lieutenant() { cout << "  +Lieutenant\n"; }
	~Lieutenant() {
		DTOR("~Lieutenant ", Homework);
	}
public:
	void makeDecision(int livesAtRisk) {
		if(livesAtRisk < 10)
			cout << "  Lieutenant charges " << livesAtRisk << ".\n";
		else superior->makeDecision(livesAtRisk);
	}
};
class Captain : public Officer {
public:
	Captain() { cout << "  +Captain\n"; }
	~Captain() {
		DTOR("~Captain ", Homework);
	}
public:
	void makeDecision(int livesAtRisk) {
		if(livesAtRisk < 20)
			cout << "  Captain retreats " << livesAtRisk << ".\n";
		else superior->makeDecision(livesAtRisk);
	}
};
class Major : public Officer {
public:
	Major() { cout << "  +Major\n"; }
	~Major() {
		DTOR("~Major ", Homework);
	}
public:
	void makeDecision(int livesAtRisk) {
		if(livesAtRisk < 100)
			cout << "  Major flanks " << livesAtRisk << ".\n";
		else superior->makeDecision(livesAtRisk);
	}
};
class Colonel : public Officer {
public:
	Colonel() { cout << "  +Colonel\n"; }
	~Colonel() {
		DTOR("~Colonel ", Homework);
	}
public:
	void makeDecision(int livesAtRisk) {
		if(livesAtRisk < 5000)
			cout << "  Colonel gathers intel " << livesAtRisk << ".\n";
		else superior->makeDecision(livesAtRisk);
	}
};
class General : public Officer {
public:
	General() { cout << "  +General\n"; }
	~General() {
		DTOR("~General ", Homework);
	}
public:
	void makeDecision(int livesAtRisk) {
		if(livesAtRisk < 100000)
			cout << "  General strategizes " << livesAtRisk << ".\n";
		else superior->makeDecision(livesAtRisk);
	}
};
class CommanderInChief : public Officer { // Buck stops here.
public:
	CommanderInChief() { cout << "  +CommanderInChief\n"; }
	~CommanderInChief() {
		DTOR("~CommanderInChief ", Homework);
	}
public:
	void makeDecision(int livesAtRisk) {
		cout << "  CommanderInChief negotiates " << livesAtRisk << ".\n";
	}
};
// Seam point - add another Approver.

Officer* Officer::makeObject(const string& criteria) {
	if(criteria == "Lieutenant")		return new Lieutenant;
	if(criteria == "Captain")			return new Captain;
	if(criteria == "Major")				return new Major;
	if(criteria == "Colonel")			return new Colonel;
	if(criteria == "General")			return new General;
	if(criteria == "CommanderInChief")	return new CommanderInChief;
	// Seam point - add another Officer.
	throw "OOPS!"; // Base, default, null, exception.
}
Officer* SetupChain() {
	string chain[] = {
		"Lieutenant", "Captain",
		//"Major",	// Killed in action.
		"Colonel",
		"General", "CommanderInChief"
	};
	Officer* officer = Officer::makeObject(chain[0]);

	Officer* current = officer;
	for(size_t i=1; i<COUNT(chain); i++) {
		Officer* next = Officer::makeObject(chain[i]);
		current->setSuccessor(next);
		current = next;
	}

	return officer;
}

void clientCode(Officer* chain, int livesAtRisk) {
	chain->makeDecision(livesAtRisk);
}

void demo() { // Test variations.
	Officer* approver = SetupChain();

	int data[] = { 5, 15, 80, 1155, 8144, 400000 };
	for(size_t i=0; i<COUNT(data); i++) {
		clientCode(approver, data[i]);
	}

	delete approver;
	cout << endl;
}

} // solution

#endif /* SOLUTIONS_CHAINOFRESP_H_ */

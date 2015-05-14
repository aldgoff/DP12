//============================================================================
// Name        : DP8.cpp
// Author      : Allan Goff
// Version     :
// Copyright   : Novatia Labs 2015
// Description : Lectures, homework, and final for a Design Patterns class.
//============================================================================

#include "DP.h"

int main(int argc, char* args[]) {
	cout << "Hello DP8.\n" << endl;

	observer::ObserverSubject* command = new observer::ObserverSubject();

	command->init();

	if(argc == 1) {				// Give user list of understood args.
		command->listDPs();
		cout << "...\n";
		command->listMatrices();
		cout << endl;
	}

	for(int i=1; i<argc; i++) {		// Execute commands, consume supporting args.
		bool last = (i+1 == argc);
		if(command->findDP(args[i])) {			// Design pattern commands.
			int consumedArgs = command->executeDP(&(args[i]), last);
			i += consumedArgs;
			}
		else if(command->findMatrix(args[i])) {	// Matrix commands.
			int consumedArgs = command->executeMatrix(&(args[i]), last);
			i += consumedArgs;
			}
		else {
			cout << "  <" << args[i] << "> command line arg NOT understood.\n";
		}
		cout << endl;
	}

	delete command;

	cout << "Aloha DP8.\n" << endl;
	return 0;
}

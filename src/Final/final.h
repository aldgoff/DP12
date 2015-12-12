/*
 * final.h
 *
 *  Created on: May 15, 2015
 *      Author: aldgoff
 */

#ifndef FINAL_FINAL_H_
#define FINAL_FINAL_H_

#include "../DP.h"

#include "../Final/finalDesign.h"
#include "../Final/finalGuided.h"
#include "../Final/finalDtoO.h"
#include "../Final/finalOriginal.h"
#include "../Final/finalOtoS.h"
#include "../Final/finalSolution.h"

namespace final {

class FinalObserver : public observer::DPObserver {
public:
	FinalObserver(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "final") {}
	virtual ~FinalObserver() { DTOR("~FinalObserver ", Architecture); }
public:
	virtual int execute(char* args[], bool last) {
		string opt = "";
		if(!last)
			opt = args[1];

		// final <command> src/Final/In/orders_1/2.txt
		if(		opt == "design")	final_design::demo(args[2]);
		else if(opt == "guided")	final_guided::demo(args[2]);
		else if(opt == "DtoO")		final_d_to_o::demo(args[2]);
		else if(opt == "original")	final_original::demo(args[2]);
		else if(opt == "OtoS")		final_o_to_s::demo(args[2]);
		else if(opt == "solution")	final_solution::demo(args[2]);
		else {
			cout << "<< final [design|guided|DtoO|original|OtoS|solution]";
			cout << " [src/Final/In/orders_1/2.txt] >>\n";
			return 0;
		}

		return 2;
	}
};

}

#endif /* FINAL_FINAL_H_ */

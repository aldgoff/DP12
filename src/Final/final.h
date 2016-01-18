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
#include "../Final/finalDtoO.h"
#include "../Final/finalOriginal.h"
#include "../Final/finalOtoS.h"
#include "../Final/finalSolution.h"

#include "../Final1week/finalPart1.h"
#include "../Final1week/guided2.h"
#include "../Final1week/finalPart2.h"
#include "../Final1week/guided1.h"

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
		// final <command> src/Final1week/In/orders_1/2.txt
		if(		opt == "design")	final_design::demo(args[2]);

		else if(opt == "DtoO")		final_d_to_o::demo(args[2]);	// 12 wk course.
		else if(opt == "original")	final_original::demo(args[2]);
		else if(opt == "OtoS")		final_o_to_s::demo(args[2]);
		else if(opt == "solution")	final_solution::demo(args[2]);

		else if(opt == "guided1")	final_guided1::demo(args[2]);	// 1 week course.
		else if(opt == "part1")		final_part1::demo(args[2]);
		else if(opt == "guided2")	final_guided2::demo(args[2]);
		else if(opt == "part2")		final_part2::demo(args[2]);

		else {
			cout << "<< final [design|DtoO|original|OtoS|solution]\n";
			cout << "   final [design|guided1|part1|guided2|part2]\n";
			cout << "   src/[Final|Final1week]/In/[orders_1.txt|orders_2.txt] >>\n";
			return 0;
		}

		return 2;
	}
};

}

#endif /* FINAL_FINAL_H_ */

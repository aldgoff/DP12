/*
 * DP.cpp
 *
 *  Created on: May 11, 2015
 *      Author: aldgoff
 */

#include "DP.h"

#include "Final/final.h"
#include "dptemplate.h"
#include "Lectures/strategy.h"
#include "Lectures/adapter.h"
#include "Lectures/state.h"
#include "Lectures/headfirst.h"
// Seam point - add another design pattern include (strategy.h, adapter.h, etc.).

namespace observer {

DPObserver::DPObserver(ObserverSubject* subject, int seqNo, const string& name)
	: subject(subject), seqNo(seqNo), name(name) {
	subject->attachDP(this);
}
DPObserver::~DPObserver() {
	DTOR("~DPObserver\n", Architecture);
	subject->detachDP(this);
}


MatrixObserver::MatrixObserver(ObserverSubject* subject, const string& name)
	: subject(subject), name(name) {
		subject->attachMatrix(this);
	}
MatrixObserver::~MatrixObserver() {
	DTOR("~MatrixObserver\n", Architecture);
	subject->detachMatrix(this);
}
int MatrixObserver::execute(char* args[], bool last) {
	list<DPObserver*>::iterator it;
	for(it=subject->obsDP.begin(); it!=subject->obsDP.end(); ++it) {
		matrix(*it);
	}
	return 0;
}


void ObserverSubject::init() {
	int i=0;
	new strategy::StrategyObserver(this, ++i);
	new adapter::AdapterObserver(this, ++i);
	new state::StateObserver(this, ++i);
	new headfirst::HeadfirstObserver(this, ++i);
	// Seam point - add another design pattern command.

	new final::FinalObserver(this, 0);

	new LectureObserver(this);
	new HomeworkObserver(this);
	// Seam point - add another matrix command.
}

}

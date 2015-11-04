/*
 * DP.cpp
 *
 *  Created on: May 11, 2015
 *      Author: aldgoff
 */

#include "DP.h"

#include "swpc.h"

#include "Final/final.h"
#include "dptemplate.h"
#include "Lectures/strategy.h"
#include "Lectures/adapter.h"
#include "Lectures/factoryMethod.h"
#include "Lectures/templateMethod.h"
#include "Lectures/decorator.h"
#include "Lectures/observer.h"
#include "Lectures/chainOfResp.h"
#include "Lectures/bridge.h"
#include "Lectures/abstractFactory.h"

#include "WIP/state.h"
#include "WIP/headfirst.h"

/* Lecture/Homework Chart
 * Design Pattern			Lecture					Homework
 * ---------------------------------------------------------------------
 * Strategy					Traversal/Parse/Swarm	Testing hardware
 * Adapter					Audio codecs			Shape draw
 * Factory Method			Vacation				Encrypted video
 * Template Method			Sterilize process		Coating process
 * Observer					New Customers			Cat in the Hat
 * Decorator				Windows					Cars
 * Chain of Responsibility	RefundApproval			ChainOfCommand
 * Bridge					Thread Scheduler		ShapeRenderPlatform
 * Abstract Factory			Fast Hierarchy			Herbivores/Carnivores
 */

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
	new strategy::Observer(this, ++i);
	new adapter::Observer(this, ++i);
	new factory_method::Observer(this, ++i);
	new template_method::Observer(this, ++i);
	new observer::Observer(this, ++i);
	new decorator::Observer(this, ++i);
	new chain_of_resp::Observer(this, ++i);
	new bridge::Observer(this, ++i);
	new abstract_factory::Observer(this, ++i);
	// Seam point - add another design pattern command.
	new state::StateObserver(this, ++i);
	new headfirst::HeadfirstObserver(this, ++i);

	new final::FinalObserver(this, 0);

	new swpc::Observer(this, 0);

	new LectureObserver(this);
	new HomeworkObserver(this);
	// Seam point - add another matrix command.
	new SkeletonObserver(this);
	new RecognitionObserver(this);
	new RefactoringObserver(this);
	new PracticalIssuesObserver(this);
}

}

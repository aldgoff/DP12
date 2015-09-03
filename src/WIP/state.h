/*
 * state.h
 *
 *  Created on: May 11, 2015
 *      Author: aldgoff
 *
 * Varies: If the algorithms are varying, use the State pattern.
 *
 * Desc: Define a family of interchangeable algorithms which can vary independently from the clients.
 *
 * Category: Behavioral
 *
 *  URLs:
 *  	http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#State
 *  	http://www.dofactory.com/net/state-design-pattern
 *  	http://www.netobjectives.com/resources/books/design-patterns-explained/review-questions#Chapter9
 *  	http://sourcemaking.com/design_patterns/state
 */

#ifndef LECTURES_STATE_H_
#define LECTURES_STATE_H_

#include "../DP.h"

namespace state {	// Command line: state [l|h] [ll|lp|ls] [hl|hp|hs]

namespace lecture {

namespace legacy {

/* Grid  State
 * Command   Initialized  Running  Paused  Halted  Done
 * Init      ---
 * Start     Running      ---
 * Pause                  Paused   ---
 * Resume                          Running
 * Stop                   Halted
 * Finish                                   Done
 */

void demo(int seqNo) {
	cout << seqNo << ") << state::lecture::legacy::demo() >>\n";
}

}

namespace problem {

/* There are two things that can vary here:
 * 1) The states
 * 2) The actions
 * Think graph theory; nodes and edges
 * Initially, we will only know about a few states and a few actions.
 * In the original state design pattern it is only easy to add states.
 * Here, I am going to try to make it easy to add actions.
 */

class State2;

class Context2 {
public:
	Context2(State2* initialState) : state(initialState) {}
	virtual ~Context2() { DTOR("~Context2 ", Architecture); }
public:
	State2*	state;
public:
	void start();
	void stop();
	// Seam point - add another action.
};

class State2 {
public:
	State2() {}
	virtual ~State2() { DTOR("~State2 ", Architecture); }
public:
	virtual void start(Context2* context) { cout << "State2.start()\n"; }
	virtual void stop(Context2* context) { cout << "State2.stop()\n"; }
	// Seam point - add another action.
public:
	static State2* decisionLogic(string criteria);
};
class Started : public State2 {
public:
	Started() {}
	virtual ~Started() { DTOR("~Started ", Architecture); }
public:
	void start(Context2* context) {
		cout << "  Starting; fresh context.\n";
	}
	void stop(Context2* context) {
		cout << "  Stopping; forgetting context.\n";
	}
	// Seam point - add another action.
};
class Stopped : public State2 {
public:
	Stopped() {}
	virtual ~Stopped() { DTOR("~Stopped ", Architecture); }
public:
	void start(Context2* context) {
		cout << "  Starting; fresh context.\n";
	}
	void stop(Context2* context) {
		cout << "  Stopping; forgetting context.\n";
	}
	// Seam point - add another action.
};
// Seam point - add another state.

State2* State2::decisionLogic(string newState) {	// Decouples derived states from each other.
	if(		newState == "Started")		return new Started;
	else if(newState == "Stopped")		return new Stopped;
	// Seam point - add another state.
	else {
		return new State2;
	}
}

//Context2::Context2(State2* initialState) : state(initialState) {}
//Context2::~Context2() {
//		delete state;
//		DTOR("~Context2\n", Architecture);
//	}
void Context2::start()	{ state->start(this); }
void Context2::stop()	{ state->stop(this); }
// Seam point - add another action.

void demo(int seqNo) {
	cout << seqNo << ") << state::lecture::problem::demo() >>\n";
}

}

namespace solution {

class State;

class Context {
public:
	State*	state;
public:
	Context(State* initalState);
	~Context();
public:
	void init();
	void start();
	void pause();
	void resume();
	void stop();
	void done();
};

class State {
public:
	State() {}
	virtual ~State() { DTOR("~State ", Architecture); }
public:
	virtual void status() { cout << "\nState::status()"; }
	virtual void init(Context* context) {
		cout << "  Initializing; ensure required resources exist.\n";
	}
	virtual void start(Context* context) {
		cout << "  Starting; fresh context.\n";
	}
	virtual void pause(Context* context) {
		cout << "  Pausing; save current context.\n";
	}
	virtual void resume(Context* context) {
		cout << "  Resuming from previous context.\n";
	}
	virtual void stop(Context* context) {
		cout << "  Stopping; forgetting context.\n";
	}
	virtual void done(Context* context) {
		cout << "  Done; prepare stats.\n";
	}
public:
	static State* decisionLogic(string criteria);
};
class Initialized : public State {
public:
	Initialized() {}
	virtual ~Initialized() { DTOR("~Initialized ", Architecture); }
public:
	virtual void status() { cout << "\nInitialized:\n\n"; }
	void init(Context* context) {
		cout << "  Already initialized.\n";
	}
	void start(Context* context) {
		cout << "  Starting; fresh context.\n";
		delete context->state;
		context->state = State::decisionLogic("running");
		context->state->status();
	}
	void pause(Context* context) {
		cout << "  Pausing; save current context.\n";
	}
	void resume(Context* context) {
		cout << "  Resuming: restore previous context.\n";
	}
	void stop(Context* context) {
		cout << "  Stopping; forgetting context.\n";
	}
	void done(Context* context) {
		cout << "  Done; prepare stats, release resources.\n";
	}
};
class Running : public State {
public:
	Running() {}
	virtual ~Running() { DTOR("~Running ", Architecture); }
public:
	virtual void status() { cout << "\nRunning:\n\n"; }
	void init(Context* context) {
		cout << "  Dumping context, stats & resources.\n";
		delete context->state;
		context->state = State::decisionLogic("initialized");
		context->state->status();
	}
	void start(Context* context) {
		cout << "  Already running.\n";
	}
	void pause(Context* context) {
		cout << "  Pausing; save current context.\n";
		delete context->state;
		context->state = State::decisionLogic("paused");
		context->state->status();
	}
	void resume(Context* context) {
		cout << "  Resuming from previous context.\n";
	}
	void stop(Context* context) {
		cout << "  Stopping; forgetting context.\n";
		delete context->state;
		context->state = State::decisionLogic("halted");
		context->state->status();
	}
	void done(Context* context) {
		cout << "  Done; prepare stats.\n";
	}
};
class Paused : public State {
public:
	Paused() {}
	virtual ~Paused() { DTOR("~Paused ", Architecture); }
public:
	virtual void status() { cout << "\nPaused:\n\n"; }
	void init(Context* context) {
		cout << "  Initializing; ensure required resources exist.\n";
	}
	void start(Context* context) {
		cout << "  Starting; fresh context.\n";
		delete context->state;
		context->state = State::decisionLogic("running");
		context->state->status();
	}
	void pause(Context* context) {
		cout << "  Already paused.\n";
	}
	void resume(Context* context) {
		cout << "  Resuming; restore previous context.\n";
		delete context->state;
		context->state = State::decisionLogic("running");
		context->state->status();
	}
	void stop(Context* context) {
		cout << "  Stopping paused process, dumping context.\n";
		delete context->state;
		context->state = State::decisionLogic("halted");
		context->state->status();
	}
	void done(Context* context) {
		cout << "  Done; prepare stats.\n";
	}
};
class Halted : public State {
public:
	Halted() {}
	virtual ~Halted() { DTOR("~Halted ", Architecture); }
public:
	virtual void status() { cout << "\nHalted:\n\n"; }
	void init(Context* context) {
		cout << "  Initializing; ensure required resources exist.\n";
	}
	void start(Context* context) {
		cout << "  Starting; fresh context.\n";
	}
	void pause(Context* context) {
		cout << "  Pausing; save current context.\n";
	}
	void resume(Context* context) {
		cout << "  Resuming w/o previous context.\n";
		delete context->state;
		context->state = State::decisionLogic("running");
		context->state->status();
	}
	void stop(Context* context) {
		cout << "  Already halted.\n";
	}
	void done(Context* context) {
		cout << "  Done; prepare stats, release resources.\n";
		delete context->state;
		context->state = State::decisionLogic("done");
		context->state->status();
	}
};
class Done : public State {
public:
	Done() {}
	virtual ~Done() { DTOR("~Done ", Architecture); }
public:
	virtual void status() { cout << "\nDone:\n\n"; }
	void init(Context* context) {
		cout << "  Initializing; allocate resources, reset stats.\n";
		delete context->state;
		context->state = State::decisionLogic("initialized");
		context->state->status();
	}
	void start(Context* context) {
		cout << "  Can't start a done process, re-initialize.\n";
	}
	void resume(Context* context) {
		cout << "  Can't resume a done process, re-initialize.\n";
	}
	void done(Context* context) {
		cout << "  Already done.\n";
	}
};
// Seam point - add another state.

State* State::decisionLogic(string newState) {	// Decouples derived states from each other.
	if(		newState == "initialized")	return new Initialized;
	else if(newState == "running")		return new Running;
	else if(newState == "paused")		return new Paused;
	else if(newState == "halted")		return new Halted;
	else if(newState == "done")			return new Done;
	// Seam point - add another state.
	else {
		return new State;
	}
}

Context::Context(State* initialState) : state(initialState) {}
Context::~Context() {
		delete state;
		DTOR("~Context\n", Architecture);
	}
void Context::init()	{ state->init(this); }
void Context::start()	{ state->start(this); }
void Context::pause()	{ state->pause(this); }
void Context::resume()	{ state->resume(this); }
void Context::stop()	{ state->stop(this); }
void Context::done()	{ state->done(this); }

void clientCode(Context* context) {
	context->init();	// Initialized.
	context->start();	// Running.
	context->pause();	// Paused.
	context->resume();	// Running.
	context->stop();	// Halted.
	context->done();	// Done.
}

void demo(int seqNo) {
	cout << seqNo << ") << state::lecture::solution::demo() >>\n";
	Context* contexts[] = {
		new Context(new Done),
//		new Context(new Running),
//		new Context(new Halted),
	};
	for(size_t i=0; i<sizeof(contexts)/sizeof(*contexts); i++) {
//		contexts[i]->state->status();
		clientCode(contexts[i]);
	}
	cout << endl;

	for(size_t i=0; i<sizeof(contexts)/sizeof(*contexts); i++) {
		delete contexts[i];
	}
}

}

} // lecture

namespace homework {

namespace legacy {

void demo(int seqNo) {
	cout << seqNo << ") << state::homework::legacy::demo() >>\n";
}

}

namespace problem {

void demo(int seqNo) {
	cout << seqNo << ") << state::homework::problem::demo() >>\n";
}

}

namespace solution {

void demo(int seqNo) {
	cout << seqNo << ") << state::homework::solution::demo() >>\n";
}

}

} // homework

class StateObserver : public observer::DPObserver {
public:
	StateObserver(observer::ObserverSubject* subject, int seqNo)
	: observer::DPObserver(subject, seqNo, "state") {}
	virtual ~StateObserver() { DTOR("~StateObserver ", Architecture); }
public:
	virtual void lectureLegacy() {
		cout << seqNo << ") << state::lecture::legacy >>\n";
		lecture::legacy::demo(seqNo);
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << state::lecture::problem >>\n";
		lecture::problem::demo(seqNo);
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << state::lecture::solution >>\n";
		lecture::solution::demo(seqNo);
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << state::homework::legacy >>\n";
		homework::legacy::demo(seqNo);
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << state::homework::problem >>\n";
		homework::problem::demo(seqNo);
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << state::homework::solution >>\n";
		homework::solution::demo(seqNo);
	}
};

} // state

#endif /* LECTURES_STATE_H_ */

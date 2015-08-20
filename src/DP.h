/*
 * DP.h
 *
 *  Created on: May 11, 2015
 *      Author: aldgoff
 */

#ifndef DP_H_
#define DP_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
using namespace std;

const unsigned flags = 0x02;	// Dtor instrumentation controlled by bit flags.
#define DTOR(x, flag) if(flag & flags) { cout << x; }
enum DtorFlags {
	Architecture	= 0x01,
	Lecture			= 0x02,
	Homework		= 0x0C,
	Problem			= 0x04,
	Solution		= 0x08,
	Final			= 0x10,
};

#define COUNT(x) (sizeof(x)/sizeof(*x))

/* Seam point index
 * path/file/namespace/class/method
 * --------------------------------
 * src/<designPattern>.h
 * src/DP.cpp/#include <designPattern>.h
 * src/DP.cpp/ObserverSubject/init()
 */
namespace observer {

class ObserverSubject;

class DPObserver {
	ObserverSubject* subject;
public:
	const int		seqNo;
	const string	name;
public:
	DPObserver( ObserverSubject* subject, int seqNo=0,
		const string& name="designPattern");
	virtual ~DPObserver();
public:
	void update(ObserverSubject* subject) {
		cout << name << "\n";
	}
public:
	virtual int execute(char* args[], bool last) {
		string opt = "";
		if(!last)
			opt = args[1];

		if(opt == "l")		lecture();
		else if(opt == "ll")	lectureLegacy();
		else if(opt == "lp")	lectureProblem();
		else if(opt == "ls")	lectureSolution();

		else if(opt == "h")	homework();
		else if(opt == "hl")	homeworkLegacy();
		else if(opt == "hp")	homeworkProblem();
		else if(opt == "hs")	homeworkSolution();

		else {
			lecture();
			homework();
			return 0;
		}

		return 1;
	}
	virtual void lecture() {
		lectureLegacy();
		lectureProblem();
		lectureSolution();
	}
	virtual void lectureLegacy() {
		cout << seqNo << ") << design_pattern::lecture::legacy >>\n";
	}
	virtual void lectureProblem() {
		cout << seqNo << ") << design_pattern::lecture::problem >>\n";
	}
	virtual void lectureSolution() {
		cout << seqNo << ") << design_pattern::lecture::solution >>\n";
	}
	virtual void homework() {
		homeworkLegacy();
		homeworkProblem();
		homeworkSolution();
	}
	virtual void homeworkLegacy() {
		cout << seqNo << ") << design_pattern::homework::legacy >>\n";
	}
	virtual void homeworkProblem() {
		cout << seqNo << ") << design_pattern::homework::problem >>\n";
	}
	virtual void homeworkSolution() {
		cout << seqNo << ") << design_pattern::homework::solution >>\n";
	}
	// Seam point - add another matrix entry point.
	virtual void skeleton() {
		cout << seqNo << ") << design_pattern::skeleton >>\n";
	}
	virtual void recognition() {
		cout << seqNo << ") << design_pattern::recognition >>\n";
	}
	virtual void refactoring() {
		cout << seqNo << ") << design_pattern::refactoring >>\n";
	}
	virtual void practicalIssues() {
		cout << seqNo << ") << design_pattern::practicalIssues >>\n";
	}
};
// Seam point - add another design pattern header (strategy.h, adapter.h, etc.).

class MatrixObserver {
protected:
	ObserverSubject* subject;
public:
	const string	name;
public:
	MatrixObserver(ObserverSubject* subject, const string& name="MatrixObserver");
	virtual ~MatrixObserver();
public:
	void update(ObserverSubject* subject) {
		cout << name << "\n";
	}
	int execute(char* args[], bool last);
public:
	virtual void matrix(DPObserver* command) {
		cout << "matrix\n";
	}
};
class LectureObserver : public MatrixObserver {
public:
	LectureObserver(ObserverSubject* subject)
		: MatrixObserver(subject, "lectures") {
	}
	virtual ~LectureObserver() {
		DTOR("~LectureObserver ", Architecture);
	}
public:
	virtual void matrix(DPObserver* command) {
		command->lecture();
	}
};
class HomeworkObserver : public MatrixObserver {
public:
	HomeworkObserver(ObserverSubject* subject)
		: MatrixObserver(subject, "homework") {
	}
	virtual ~HomeworkObserver() {
		DTOR("~HomeworkObserver ", Architecture);
	}
public:
	virtual void matrix(DPObserver* command) {
		command->homework();
	}
};
// Seam point - add another matrix command (lectures, homework, pairs, etc.).
class SkeletonObserver : public MatrixObserver {
public:
	SkeletonObserver(ObserverSubject* subject)
		: MatrixObserver(subject, "skeletons") {
	}
	virtual ~SkeletonObserver() {
		DTOR("~SkeletonObserver ", Architecture);
	}
public:
	void matrix(DPObserver* command) {
		command->skeleton();
	}
};
class RecognitionObserver : public MatrixObserver {
public:
	RecognitionObserver(ObserverSubject* subject)
		: MatrixObserver(subject, "recognition") {
	}
	virtual ~RecognitionObserver() {
		DTOR("~RecognitionObserver ", Architecture);
	}
public:
	void matrix(DPObserver* command) {
		command->recognition();
	}
};
class RefactoringObserver : public MatrixObserver {
public:
	RefactoringObserver(ObserverSubject* subject)
		: MatrixObserver(subject, "refactoring") {
	}
	virtual ~RefactoringObserver() {
		DTOR("~RefactoringObserver ", Architecture);
	}
public:
	void matrix(DPObserver* command) {
		command->refactoring();
	}
};
class PracticalIssuesObserver : public MatrixObserver {
public:
	PracticalIssuesObserver(ObserverSubject* subject)
		: MatrixObserver(subject, "practicalIssues") {
	}
	virtual ~PracticalIssuesObserver() {
		DTOR("~PracticalIssuesObserver ", Architecture);
	}
public:
	void matrix(DPObserver* command) {
		command->practicalIssues();
	}
};

class ObserverSubject {
public:
	list<DPObserver*>				obsDP;
private:
	list<DPObserver*>::iterator	it;
	list<MatrixObserver*>			obsMatrix;
	list<MatrixObserver*>::iterator	itm;
public:
	ObserverSubject() {}
	virtual ~ObserverSubject() {
		string observers = "observers left to process (should be zero) =";
		char str[180];

		while(obsDP.begin() != obsDP.end()) {
			delete *obsDP.begin();
		}
		sprintf(str, " Design Pattern %s %d.\n",observers.c_str(),obsDP.size());
		DTOR(str, Architecture);

		while(obsMatrix.begin() != obsMatrix.end()) {
			delete *obsMatrix.begin();
		}
		sprintf(str, " Design Pattern %s %d.\n",observers.c_str(),obsDP.size());
		DTOR(str, Architecture);

		DTOR("~ObserverSubject\n", Architecture);
		if(flags)
			cout << endl;
	}
public:
	void init();
public:
	void attachDP(DPObserver* obs) {
		obsDP.push_back(obs);
	}
	void detachDP(DPObserver* obs) {
		obsDP.remove(obs);
	}
	void listDPs() {
		list<DPObserver*>::iterator it;
		for(it=obsDP.begin(); it!=obsDP.end(); ++it) {
			(*it)->update(this);
		}
	}
	bool findDP(string arg) {
		for(it=obsDP.begin(); it!=obsDP.end(); ++it) {
			if((*it)->name == arg)
				return true;
		}
		return false;
	}
	int executeDP(char* args[], bool last) {
		return (*it)->execute(args, last);
	}
public:
	void attachMatrix(MatrixObserver* obs) {
		obsMatrix.push_back(obs);
	}
	void detachMatrix(MatrixObserver* obs) {
		obsMatrix.remove(obs);
	}
	void listMatrices() {
		list<MatrixObserver*>::iterator itm;
		for(itm=obsMatrix.begin(); itm!=obsMatrix.end(); ++itm) {
			(*itm)->update(this);
		}
	}
	bool findMatrix(string arg) {
		for(itm=obsMatrix.begin(); itm!=obsMatrix.end(); ++itm) {
			if((*itm)->name == arg)
				return true;
		}
		return false;
	}
	int executeMatrix(char* args[], bool last) {
		return (*itm)->execute(args, last);
	}
};

}

#endif /* DP_H_ */

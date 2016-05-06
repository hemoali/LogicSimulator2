#ifndef _CREATE_TRUTH_TABLE_H
#define _CREATE_TRUTH_TABLE_H
#include"Action.h"
class CreateTruthTable : public Action
{
public:
	CreateTruthTable(ApplicationManager*);
	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image *);

	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();

	~CreateTruthTable();
};

#endif
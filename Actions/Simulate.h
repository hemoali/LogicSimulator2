#ifndef _SIMULATE_H
#define _SIMULATE_H
#include "Action.h"
class Simulate :
	public Action
{
public:
	Simulate(ApplicationManager*pApp);
	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image *);

	void spreadTheWord(Component* comp);
	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();

	virtual ~Simulate();
};

#endif
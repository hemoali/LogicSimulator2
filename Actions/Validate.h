#ifndef _VALIDATE_H
#define _VALIDATE_H
#include "Action.h"
class Validate :
	public Action
{
	string msg;
public:
	Validate(ApplicationManager*pApp);
	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image *);

	bool validateOutputComponent(Component* comp);
	bool validateInputComponent(Component* comp);
	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();

	virtual ~Validate();
};

#endif
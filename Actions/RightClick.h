#pragma once
#include "Action.h"
#include "../Components/Gate.h"
//TODO:::
class RightClick : public Action
{
private:
	ActionType SelectedAction;
	int actionNum;
	Component* C;

public:

	RightClick(ApplicationManager*);
	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image* img = NULL);
	//Get the Selected Action
	virtual ActionType getSelectedAction();//TO BE DEleted
	//Execute action (code depends on action type)
	virtual void Execute();
	//To undo this action (code depends on action type)
	virtual void Undo();
	//To redo this action (code depends on action type)
	virtual void Redo();
	~RightClick();
};

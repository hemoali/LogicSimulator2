#ifndef _RIGHT_CLICK
#define _RIGHT_CLICK
#include "Action.h"
#include "../Components/Gate.h"
#include "../Components/Connection.h"
#include "../Actions/EditLabel.h"
#include "../Actions/Delete.h"
#include "../Actions/MultiDelete.h"
#include "../Components/LED.h"
#include "../Components/SWITCH.h"
#include "../Actions/ModifyConnection.h"
class RightClick : public Action
{
private:
	ActionType SelectedAction;
	int actionNum;
	Component* C;
	Action* theAction;

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
	//Return the ACtion
	virtual Action* getAction();
	~RightClick();
};
#endif
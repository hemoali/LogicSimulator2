#pragma once
#include "Action.h"
#include "../Components/Component.h"

class EditLabel :
	public Action
{
	Component *theComponenet;
	string theNewLabel;
	string theOriginalLabel;
public:
	EditLabel(ApplicationManager *pApp);

	EditLabel(ApplicationManager * pApp, Component * C);

	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image * = NULL) ;

	//Execute action (code depends on action type)
	virtual void Execute() ;

	//To undo this action (code depends on action type)
	virtual void Undo() ;

	//To redo this action (code depends on action type)
	virtual void Redo() ;

	~EditLabel();
};
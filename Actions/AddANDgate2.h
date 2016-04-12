#ifndef _ADD_AND_GATE_H
#define _ADD_AND_GATE_H

#include "action.h"
#include "..\Components\AND2.h"

class AddANDgate2 : public Action
{
private:
	string gateLabel;
	GraphicsInfo GInfo;
public:
	AddANDgate2(ApplicationManager *pApp);
	virtual ~AddANDgate2(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image* );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();


};

#endif
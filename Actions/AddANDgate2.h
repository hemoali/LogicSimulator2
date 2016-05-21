#ifndef AND2_Header
#define AND2_Header

#include "Action.h"
#include "..\Components\AND2.h"

class AddANDgate2 : public Action
{
public:
	AddANDgate2(ApplicationManager *pApp);
	virtual ~AddANDgate2(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image*, Component* );
	//Execute action (code depends on action type)
	virtual void Execute();
};

#endif
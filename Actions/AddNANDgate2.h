#ifndef NAND2_Header
#define NAND2_Header
#include"Action.h"
#include"..\Components\NAND2.h"
class AddNANDgate2:public Action
{
public:
	AddNANDgate2(ApplicationManager *pApp);
	virtual ~AddNANDgate2(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * , Component*);
	//Execute action (code depends on action type)
	virtual void Execute();
};

#endif
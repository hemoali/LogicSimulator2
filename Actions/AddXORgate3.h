#ifndef XOR3_Header
#define XOR3_Header
#include"..\Components\XOR3.h"
#include"Action.h"
class AddXORgate3:public Action
{
public:
	AddXORgate3(ApplicationManager *pApp);
	virtual ~AddXORgate3(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

};

#endif
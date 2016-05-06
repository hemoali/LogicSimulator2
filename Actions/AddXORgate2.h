#ifndef XOR2_Header
#define XOR2_Header
#include"..\Components\XOR2.h"
#include"Action.h"
class AddXORgate2:public Action
{
public:
	AddXORgate2(ApplicationManager *pApp);
	virtual ~AddXORgate2(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();


};

#endif
#ifndef BUFFER_Header
#define BUFFER_Header

#include"Action.h"
#include"..\Components\BUFFER.h"
class AddBUFFER:public Action
{
public:
	AddBUFFER(ApplicationManager *pApp);
	virtual ~AddBUFFER(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();
};

#endif
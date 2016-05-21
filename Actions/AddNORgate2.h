#ifndef NOR_Header
#define NOR_Header
#include"Action.h"
#include"..\Components\NOR2.h"
class AddNORgate2 :public Action
{
public:
	AddNORgate2(ApplicationManager *pApp);
	virtual ~AddNORgate2(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * , Component*);
	//Execute action (code depends on action type)
	virtual void Execute();
};
#endif
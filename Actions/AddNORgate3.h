#ifndef NOR3_Header
#define NOR3_Header
#include"Action.h"
#include"..\Components\NOR3.h"
class AddNORgate3:public Action
{
public:
	AddNORgate3(ApplicationManager *pApp);
	virtual ~AddNORgate3(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

#endif
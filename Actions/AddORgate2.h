#ifndef OR2_Header
#define OR2_Header
#include"..\Components\OR2.h"
#include"Action.h"
class AddORgate2:public Action
{
public:
	AddORgate2(ApplicationManager *pApp);
	virtual ~AddORgate2(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

#endif
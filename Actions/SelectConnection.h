#ifndef SELECT_Header
#define SELECT__Header
#include "Action.h"
class Select_Connection :
	public Action
{
public:
	Select_Connection(ApplicationManager *pApp);
	virtual ~Select_Connection();

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * , Component*);
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

#endif;
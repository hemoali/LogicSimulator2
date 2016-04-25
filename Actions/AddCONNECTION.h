#ifndef CONNECTION_Header
#define CONNECTION_Header

#include"Action.h"
#include"..\Components\Connection.h"
#include"..\Components\Gate.h"
#include"..\Components\LED.h"
#include"..\Components\SWITCH.h"
class AddConnection:public Action
{
private:
	int Cx1,Cy1,Cx2,Cy2;// start and end point for Connections

public:
	AddConnection(ApplicationManager *pApp);
	virtual ~AddConnection(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();


};

#endif
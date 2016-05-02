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
	bool Silent;

public:
	AddConnection(ApplicationManager *pApp);
	virtual ~AddConnection(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();
	//Adding a Silent Function
	void AddConnectionSilent(int c1, int c2, int c3, int c4,string s);

	virtual void Undo();
	virtual void Redo();


};

#endif
#ifndef XNOR2_Header
#define XNOR2_Header
#include"..\Components\XNOR2.h"
#include"..\ApplicationManager.h"
class AddXNORgate2:public Action
{
private:
	 string gateLabel;
	GraphicsInfo GInfo;
public:
	AddXNORgate2(ApplicationManager *pApp);
	virtual ~AddXNORgate2(void);
	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

#endif
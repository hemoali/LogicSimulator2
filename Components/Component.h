#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "..\Defs.h"
#include "..\GUI\Output.h"
#include <iostream>
using namespace std;
//Base class for classes Gate, Switch, and LED.
class Component
{
private:
	string m_Label;
	bool deleted;
	image* smallCleanImageBeforeAddingComp;
protected:
	GraphicsInfo m_GfxInfo;	//The parameters required to draw a component
	GraphicsInfo m_CenterInfo;
public:
	Component(const GraphicsInfo &r_GfxInfo);
	virtual void Operate() = 0;	//Calculates the output according to the inputs
	virtual void Draw(Output* pOut) = 0;	//for each component to Draw itself
	
	
	virtual int GetOutPinStatus()=0;	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n)=0;	//returns status of Inputpin # n if SWITCH, return -1

	virtual void setInputPinStatus(int n, STATUS s)=0;	//set status of Inputpin # n, to be used by connection class.
	void setLabel(string s);
	string getLabel();

	void setDelete(bool d);
	bool getDelete();

	void setNewLocation(GraphicsInfo GfxInfo);

	void setSmallCleanImageBeforeAddingComp(image* i);
	image* getSmallCleanImageBeforeAddingComp();
	Component();	
	
	//Destructor must be virtual
	virtual ~Component();
};

#endif

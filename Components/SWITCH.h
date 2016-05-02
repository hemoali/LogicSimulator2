#ifndef _SWITCH_H
#define _SWITCH_H

#include"Component.h"
#include"InputPin.h"
#include"OutputPin.h"
class SWITCH : public Component
{
public:
	SWITCH(const GraphicsInfo &r_GfxInfo, int r_FanOut);
	virtual void Operate();	//Calculates the output according to the inputs
	virtual void Draw(Output* pOut, bool highlight);	//for each component to Draw itself


	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n);	//returns status of Inputpin # n if SWITCH, return -1

	virtual void setInputPinStatus(int n, STATUS s); virtual void selectYourSelf(Output* pOut, color Color);

	void setOutputPinStatus(int n, STATUS s);
	virtual void save(int, ofstream &);
	virtual void load(string s, ApplicationManager*);
	virtual ActionType getComponentActionType();

	virtual ~SWITCH();
};

#endif
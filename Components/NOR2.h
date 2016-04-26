#ifndef _NOR2s_H
#define _NOR2s_H
#include"Gate.h"

class NOR2 :public Gate
{
public:
	NOR2(const GraphicsInfo &r_GfxInfo, int r_FanOut);
	virtual void Operate();	//Calculates the output of the NOR gate
	virtual void Draw(Output* pOut, bool highlight);	//Draws 2-input gate

	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n);	//returns status of Inputpin # n if SWITCH, return -1

	virtual void save(int, ofstream &);
	virtual void load(ApplicationManager*);

	virtual void setInputPinStatus(int n, STATUS s); virtual void selectYourSelf(Output* pOut, color Color);
	virtual ActionType getComponentActionType();

};
#endif
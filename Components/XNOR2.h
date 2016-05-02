#ifndef _XNOR2_H
#define _XNOR2_H

/*
Class XNOR2
-----------
represent the 2-input XNOR gate
*/

#include"Gate.h"

class XNOR2 :public Gate
{
public:
	XNOR2(const GraphicsInfo &r_GfxInfo, int r_FanOut);
	virtual void Operate();	//Calculates the output of the XNOR gate
	virtual void Draw(Output* pOut, bool highlight);	//Draws 2-input gate

	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n);	//returns status of Inputpin # n if SWITCH, return -1

	virtual void setInputPinStatus(int n, STATUS s); virtual void selectYourSelf(Output* pOut, color Color);
	virtual void save(int, ofstream &);
	virtual void load(string s, ApplicationManager*);
	virtual ActionType getComponentActionType();

};

#endif
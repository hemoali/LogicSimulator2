#ifndef _BUFFER_H
#define _BUFFER_H

/*
Class BUFFER
-----------
represent the BUFFER gate
*/

#include"Gate.h"

class BUFFER :public Gate
{
public:
	BUFFER(const GraphicsInfo &r_GfxInfo, int r_FanOut);
	virtual void Operate();	//Calculates the output of the BUFFER gate
	virtual void Draw(Output* pOut, bool highlight);	//Draws BUFFER gate

	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n);	//returns status of the Inputpin if SWITCH, return -1

	virtual void setInputPinStatus(int n, STATUS s); virtual void selectYourSelf(Output* pOut, color Color);	//set status of the Inputpin , to be used by connection class.
	
	virtual void save(int, ofstream &);
	virtual void load(string s, ApplicationManager*);
	virtual ActionType getComponentActionType();

};

#endif
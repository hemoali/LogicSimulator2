#ifndef _NOT_H
#define _NOT_H

/*
Class NOT
-----------
represent the NOT gate
*/

#include"Gate.h"

class NOT :public Gate
{
public:
	NOT(const GraphicsInfo &r_GfxInfo, int r_FanOut);
	virtual void Operate();	//Calculates the output of the NOT gate
	virtual void Draw(Output* pOut);	//Draws NOT gate

	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n);	//returns status of the Inputpin if SWITCH, return -1

	virtual void setInputPinStatus(int n, STATUS s);	//set status of the Inputpin , to be used by connection class.

};

#endif
#ifndef _BUFFER_H
#define _BUFFER_H

/*
Class BUFFER
-----------
represent the BUFFER gate
*/

#include"Components\Gate.h"

class BUFFER :public Gate
{
public:
	BUFFER(const GraphicsInfo &r_GfxInfo, int r_FanOut);
	virtual void Operate();	//Calculates the output of the BUFFER gate
	virtual void Draw(Output* pOut);	//Draws BUFFER gate

	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus();	//returns status of the Inputpin if SWITCH, return -1

	virtual void setInputPinStatus(STATUS s);	//set status of the Inputpin , to be used by connection class.

};

#endif
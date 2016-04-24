#ifndef _NOR2_H
#define _NOR2_H


#include"Gate.h"

class NOR3 :public Gate
{
public:
	NOR3(const GraphicsInfo &r_GfxInfo, int r_FanOut);
	virtual void Operate();	//Calculates the output of the NOR gate
	virtual void Draw(Output* pOut, bool highlight);	//Draws 3-input gate

	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n);	//returns status of Inputpin # n if SWITCH, return -1

	virtual void save(int, ofstream &);
	virtual void load(ApplicationManager*);

	virtual void setInputPinStatus(int n, STATUS s); virtual void selectYourSelf(Output* pOut, color Color);

};

#endif
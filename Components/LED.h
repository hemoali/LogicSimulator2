#ifndef _LED_H
#define _LED_H
#include"Component.h"
#include "InputPin.h"
#include "OutputPin.h"
class LED : public Component
{
public:
	LED(const GraphicsInfo &r_GfxInfo, int r_FanOut);
	virtual void Operate();	//Calculates the output according to the inputs
	virtual void Draw(Output* pOut);	//for each component to Draw itself


	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n);	//returns status of Inputpin # n if SWITCH, return -1

	virtual void setInputPinStatus(int n, STATUS s);	//set status of Inputpin # n, to be used by connection class.
	//Destructor must be virtual
	virtual ~LED();
};

#endif
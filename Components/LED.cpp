#include "LED.h"

LED::LED(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Component(r_GfxInfo, r_FanOut)
{
	m_InputPins = new InputPin[0];
	m_InputPins[0].setConnection(NULL);
	m_InputPins[0].setComponent(this);
}
void LED::Operate()
{
}

// Function Draw
// Draws LED
void LED::Draw(Output* pOut)
{
	//Call output class and pass LED drawing info to it.
	if (!getDelete()) pOut->DrawLed(m_CenterInfo, (getInputPin(0)->getStatus() == HIGH) ? true : false);

	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}
int LED::GetOutPinStatus()
{
	return -1; //LED 
}


//returns status of Inputpin #n
int LED::GetInputPinStatus(int n)
{
	return getInputPin(n = 0)->getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin to HIGH or LOW
void LED::setInputPinStatus(int n, STATUS s)
{
	getInputPin(n = 0)->setStatus(s);
}
LED::~LED(){

}
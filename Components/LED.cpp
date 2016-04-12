#include "LED.h"

LED::LED(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Component(r_GfxInfo)
{
	m_GfxInfo.x1 = r_GfxInfo.x1;
	m_GfxInfo.y1 = r_GfxInfo.y1;
	m_GfxInfo.x2 = r_GfxInfo.x2;
	m_GfxInfo.y2 = r_GfxInfo.y2;
	inkey = new InputPin;
	inkey->setComponent(this);
}


void LED::Operate()
{
}


// Function Draw
// Draws LED
void LED::Draw(Output* pOut)
{
	//Call output class and pass LED drawing info to it.
	pOut->DrawLed(m_CenterInfo, (inkey->getStatus() == HIGH) ? true : false);
}
int LED::GetOutPinStatus()
{
	return -1; //LED 
}


//returns status of Inputpin #n
int LED::GetInputPinStatus(int n)
{
	return inkey->getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin to HIGH or LOW
void LED::setInputPinStatus(int n, STATUS s)
{
	inkey->setStatus(s);
}
InputPin*LED::getinppin()
{
	return inkey;
}
LED::~LED(){

}
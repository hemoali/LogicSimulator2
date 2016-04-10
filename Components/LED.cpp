#include "LED.h"

LED::LED(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Component(r_GfxInfo)
{
	m_GfxInfo.x1 = r_GfxInfo.x1;
	m_GfxInfo.y1 = r_GfxInfo.y1;
	m_GfxInfo.x2 = r_GfxInfo.x2;
	m_GfxInfo.y2 = r_GfxInfo.y2;
}


void LED::Operate()
{
}


// Function Draw
// Draws LED
void LED::Draw(Output* pOut)
{
	//Call output class and pass LED drawing info to it.
	pOut->DrawLed(m_GfxInfo,(inkey[0].getStatus()==HIGH)?true:false);
}

//returns status of outputpin
int LED::GetOutPinStatus()
{
	return -1; //LED 
}


//returns status of Inputpin #n
int LED::GetInputPinStatus(int n)
{
	return inkey[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin to HIGH or LOW
void LED::setInputPinStatus(int n, STATUS s)
{
	outkey[n - 1].setStatus(s);
	inkey[n-1].setStatus(s);
}
#include "SWICH.h"

SWICH::SWICH(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Component(r_GfxInfo)
{
	m_GfxInfo.x1 = r_GfxInfo.x1;
	m_GfxInfo.y1 = r_GfxInfo.y1;
	m_GfxInfo.x2 = r_GfxInfo.x2;
	m_GfxInfo.y2 = r_GfxInfo.y2;
}


void SWICH::Operate()
{
}


// Function Draw
// Draws SWICH
void SWICH::Draw(Output* pOut)
{
	//Call output class and pass SWICH drawing info to it.
	pOut->DrawSwtich(m_GfxInfo, (outkey[0].getStatus() == HIGH) ? true : false);
}

//returns status of outputpin
int SWICH::GetOutPinStatus()
{
	return outkey[0].getStatus();
}


//returns status of Inputpin #n
int SWICH::GetInputPinStatus(int n)
{
	return -1; //SWICH
}

//Set status of an input pin to HIGH or LOW
void SWICH::setInputPinStatus(int n, STATUS s)
{
	inkey[n - 1].setStatus(s);
	outkey[n-1].setStatus(s);
}
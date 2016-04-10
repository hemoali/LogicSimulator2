#include "BUFFER.h"

BUFFER::BUFFER(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(1, r_FanOut)
{
	m_GfxInfo.x1 = r_GfxInfo.x1;
	m_GfxInfo.y1 = r_GfxInfo.y1;
	m_GfxInfo.x2 = r_GfxInfo.x2;
	m_GfxInfo.y2 = r_GfxInfo.y2;
}


void BUFFER::Operate()
{
	//caclulate the output status as the ORing of the two input pins;
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == HIGH) ? HIGH : LOW);
}


// Function Draw
// Draws BUFFER gate
void BUFFER::Draw(Output* pOut)
{
	//Call output class and pass gate drawing info to it.
	pOut->DrawNot_Buffer(m_GfxInfo, true);
}

//returns status of outputpin
int BUFFER::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}

//returns status of the Inputpin 
int BUFFER::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();
}

//Set status of the input pin to HIGH or LOW
void BUFFER::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[0].setStatus(s);
}
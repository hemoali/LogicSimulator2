#include "AND2.h"

AND2::AND2(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(2, r_FanOut, r_GfxInfo)
{
}


void AND2::Operate()
{
	//caclulate the output status as the ANDing of the two input pins
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == LOW || m_InputPins[1].getStatus() == LOW) ? LOW : HIGH);
}


// Function Draw
// Draws 2-input AND gate
void AND2::Draw(Output* pOut)
{
	if (!getDelete()) pOut->DrawAnd_Nand(m_CenterInfo, 2, false);
	else {		
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int AND2::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int AND2::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin to HIGH or LOW
void AND2::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}
void AND2::selectYourSelf(Output* pOut, color Color) {

}
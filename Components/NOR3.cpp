#include "NOR3.h"

NOR3::NOR3(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(3, r_FanOut,r_GfxInfo)
{

}


void NOR3::Operate()
{
	//caclulate the output status as the NORing of the three input pins;
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == HIGH || m_InputPins[1].getStatus() == HIGH || m_InputPins[2].getStatus() == HIGH) ? HIGH : LOW);
	m_OutputPin.setStatus((m_OutputPin.getStatus() == LOW) ? HIGH : LOW);
}


// Function Draw
// Draws 3-input NOR gate
void NOR3::Draw(Output* pOut)
{
	//Call output class and pass gate drawing info to it.
	if (!getDelete()) pOut->DrawOr_Nor(m_CenterInfo, 3, true);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int NOR3::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int NOR3::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin ot HIGH or LOW
void NOR3::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}
void NOR3::selectYourSelf(Output* pOut, color Color) {

}
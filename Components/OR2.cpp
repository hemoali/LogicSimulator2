#include "OR2.h"

OR2::OR2(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(2, r_FanOut)
{
	m_GfxInfo.x1 = r_GfxInfo.x1;
	m_GfxInfo.y1 = r_GfxInfo.y1;
	m_GfxInfo.x2 = r_GfxInfo.x2;
	m_GfxInfo.y2 = r_GfxInfo.y2;
	m_CenterInfo.x1 = m_GfxInfo.x1 + UI.GATE_Width / 2;
	m_CenterInfo.y1 = m_GfxInfo.y1 + UI.GATE_Height / 2;
}


void OR2::Operate()
{
	//caclulate the output status as the ORing of the two input pins;
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == HIGH || m_InputPins[1].getStatus() == HIGH) ? HIGH : LOW);
}


// Function Draw
// Draws 2-input OR gate
void OR2::Draw(Output* pOut)
{
	//Call output class and pass gate drawing info to it.
	
	if (!getDelete())pOut->DrawOr_Nor(m_CenterInfo, 2, false);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int OR2::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int OR2::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin ot HIGH or LOW
void OR2::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}

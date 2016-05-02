#include "XOR2.h"
#include"..\ApplicationManager.h"
#include<fstream>

XOR2::XOR2(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(2, r_FanOut,r_GfxInfo)
{
}


void XOR2::Operate()
{
	//caclulate the output status as the XORing of the two input pins;
	m_OutputPin.setStatus((m_InputPins[0].getStatus() != m_InputPins[1].getStatus()) ? HIGH : LOW);
}


// Function Draw
// Draws 2-input XOR gate
void XOR2::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass gate drawing info to it.
	
	if (!getDelete())pOut->DrawXor_Xnor(m_CenterInfo, 2, false, highlight);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int XOR2::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int XOR2::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin ot HIGH or LOW
void XOR2::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}

void XOR2::selectYourSelf(Output* pOut, color Color) {

}

void XOR2::save(int id, ofstream & file)
{

	file << "XOR2  " << "  " << m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << " L" << this->getLabel() << endl;
}

void XOR2::load(string s, ApplicationManager*pM)
{
	pM->AddComponent(this);
	this->setLabel(s);
}
ActionType XOR2::getComponentActionType() {
	return ADD_XOR_GATE_2;
}
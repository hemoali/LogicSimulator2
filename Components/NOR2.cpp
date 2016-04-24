#include "NOR2.h"
#include"..\ApplicationManager.h"
#include<fstream>

NOR2::NOR2(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(2, r_FanOut,r_GfxInfo)
{
	
}


void NOR2::Operate()
{
	//caclulate the output status as the NORing of the two input pins;
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == HIGH || m_InputPins[1].getStatus() == HIGH) ? HIGH : LOW);
	m_OutputPin.setStatus((m_OutputPin.getStatus() == LOW) ? HIGH : LOW);
}


// Function Draw
// Draws 2-input NOR gate
void NOR2::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass gate drawing info to it.
	if (!getDelete())pOut->DrawOr_Nor(m_CenterInfo, 2, true, highlight);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int NOR2::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int NOR2::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin ot HIGH or LOW
void NOR2::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}
void NOR2::selectYourSelf(Output* pOut, color Color) {

}

void NOR2::save(int id, ofstream & file)
{

	file << "NOR2  "<<this->getID()<<"  "/* << this->getLabel() << "  " */<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << endl;
}

void NOR2::load(ApplicationManager*pM)
{
	pM->AddComponent(this);
}
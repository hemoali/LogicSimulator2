#include "OR2.h"
#include"..\ApplicationManager.h"
#include<fstream>

OR2::OR2(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(2, r_FanOut,r_GfxInfo)
{

}


void OR2::Operate()
{
	//caclulate the output status as the ORing of the two input pins;
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == HIGH || m_InputPins[1].getStatus() == HIGH) ? HIGH : LOW);
}


// Function Draw
// Draws 2-input OR gate
void OR2::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass gate drawing info to it.
	
	if (!getDelete())pOut->DrawOr_Nor(m_CenterInfo, 2, false, highlight);
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

void OR2::selectYourSelf(Output* pOut, color Color) {

}

void OR2::save(int id, ofstream & file)
{

	file << "OR2  "<<this->getID()<<"  "/* << this->getLabel() << "  " */<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << endl;
}

void OR2::load(ApplicationManager*pM)
{
	pM->AddComponent(this);
}
#include "XOR3.h"
#include"..\ApplicationManager.h"
#include<fstream>

XOR3::XOR3(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(3, r_FanOut,r_GfxInfo)
{
}


void XOR3::Operate()
{
	//caclulate the output status as the ORing of the two input pins;
	int cOnes = 0;
	for (int i = 0; i < 3; i++)
	{
		if (m_InputPins[i].getStatus() == HIGH)
			cOnes++;
	}
	m_OutputPin.setStatus((cOnes%2!=0) ? HIGH : LOW);
}


// Function Draw
// Draws 3-input XOR gate
void XOR3::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass gate drawing info to it.
	
	if (!getDelete()) pOut->DrawXor_Xnor(m_CenterInfo, 3, false, highlight);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int XOR3::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int XOR3::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin ot HIGH or LOW
void XOR3::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}
void XOR3::selectYourSelf(Output* pOut, color Color) {

}

void XOR3::save(int id, ofstream & file)
{
	file << "XOR3  "<<this->getID()<<"  "/* << this->getLabel() << "  " */<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << endl;
}

void XOR3::load(ApplicationManager*pM)
{
	pM->AddComponent(this);
}
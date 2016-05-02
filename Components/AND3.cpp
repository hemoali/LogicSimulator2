#include"AND3.h"
#include"..\ApplicationManager.h"
#include<fstream>

AND3::AND3(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(3, r_FanOut, r_GfxInfo)
{
	
}


void AND3::Operate()
{
	//caclulate the output status as the ANDing of the two input pins
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == LOW || m_InputPins[1].getStatus() == LOW || m_InputPins[2].getStatus() == LOW) ? LOW : HIGH);
}


// Function Draw
// Draws 3-input AND gate
void AND3::Draw(Output* pOut, bool highlight)
{
	if (!getDelete()) pOut->DrawAnd_Nand(m_CenterInfo,3, false, highlight);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int AND3::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int AND3::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin to HIGH or LOW
void AND3::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}
void AND3::selectYourSelf(Output* pOut, color Color) {
	
}

void AND3::save(int id, ofstream & file)
{

	file << "AND3  "<<"  "<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 <<  " L" << this->getLabel() << endl;
}

void AND3::load(string s, ApplicationManager*pM)
{
	pM->AddComponent(this);
	this->setLabel(s);
}
ActionType AND3::getComponentActionType() {
	return ADD_AND_GATE_3;
}
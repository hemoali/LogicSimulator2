#include "NOT.h"
#include"..\ApplicationManager.h"
#include<fstream>

NOT::NOT(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(1, r_FanOut, r_GfxInfo)
{
	
}


void NOT::Operate()
{
	//caclulate the output status as the ORing of the two input pins;
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == LOW) ? HIGH : LOW);
}


// Function Draw
// Draws NOT gate
void NOT::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass gate drawing info to it.
	
	if (!getDelete()) pOut->DrawNot_Buffer(m_CenterInfo, false, highlight);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int NOT::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of the Inputpin 
int NOT::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();
}

//Set status of the input pin to HIGH or LOW
void NOT::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}
void NOT::selectYourSelf(Output* pOut, color Color) {

}

void NOT::save(int id, ofstream & file)
{

	file << "NOT  "<<"  "<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << " L" << this->getLabel() << endl;
}

void NOT::load(string s, ApplicationManager*pM)
{
	pM->AddComponent(this);
	this->setLabel(s);
}
ActionType NOT::getComponentActionType() {
	return ADD_INV;
}
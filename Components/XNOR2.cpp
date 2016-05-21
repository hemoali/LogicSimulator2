#include "XNOR2.h"
#include"..\ApplicationManager.h"
#include<fstream>

XNOR2::XNOR2(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(2, r_FanOut,r_GfxInfo)
{
}


void XNOR2::Operate()
{
	//caclulate the output status as the XNORing of the two input pins;
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == m_InputPins[1].getStatus()) ? HIGH : LOW);
}


// Function Draw
// Draws 2-input XNOR gate
void XNOR2::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass gate drawing info to it.
	
	if (!getDelete()) pOut->DrawXor_Xnor(m_CenterInfo, 2, true, highlight, false, this);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int XNOR2::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int XNOR2::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin ot HIGH or LOW
void XNOR2::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}
void XNOR2::selectYourSelf(Output* pOut, color Color) {

}

void XNOR2::save(int id, ofstream & file)
{

	file << "XNOR2 "<< "  "<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << " L" << this->getLabel() << endl;
}

void XNOR2::load(ApplicationManager* pM, ifstream& in)
{
	string compLabel;
	in >> compLabel;
	if (compLabel.size() <= 1) {
		//Means that the label is empty as we have put an extra L 
		//char at the begining of te saved label to know whetherit has a name or not
		// in order to avoid misreading the input file
		compLabel = "";
	}
	else {
		compLabel = compLabel.substr(1, compLabel.size());
	}
	pM->AddComponent(this);
	this->setLabel(compLabel);
}
ActionType XNOR2::getComponentActionType() {
	return ADD_XNOR_GATE_2;
}
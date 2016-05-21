#include "NAND2.h"
#include"..\ApplicationManager.h"
#include<fstream>

NAND2::NAND2(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(2, r_FanOut,r_GfxInfo)
{
	
}


void NAND2::Operate()
{
	//caclulate the output status as the NANDing of the two input pins
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == LOW || m_InputPins[1].getStatus() == LOW) ? LOW : HIGH);
	m_OutputPin.setStatus((m_OutputPin.getStatus() == LOW) ? HIGH : LOW);
}


// Function Draw
// Draws 2-input NAND gate
void NAND2::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass gate drawing info to it.
	if (!getDelete()) pOut->DrawAnd_Nand(m_CenterInfo, 2, true, highlight, false, this);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int NAND2::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int NAND2::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin to HIGH or LOW
void NAND2::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}
void NAND2::selectYourSelf(Output* pOut, color Color) {

}

void NAND2::save(int id, ofstream & file)
{

	file << "NAND2  "<<"  "<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << " L" << this->getLabel() << endl;
}

void NAND2::load(ApplicationManager* pM, ifstream& in)
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
ActionType NAND2::getComponentActionType() {
	return ADD_NAND_GATE_2;
}
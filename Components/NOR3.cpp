#include "NOR3.h"
#include"..\ApplicationManager.h"
#include<fstream>

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
void NOR3::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass gate drawing info to it.
	if (!getDelete()) pOut->DrawOr_Nor(m_CenterInfo, 3, true, highlight, false, this);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1, true);
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
	return m_InputPins[n ].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin ot HIGH or LOW
void NOR3::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n ].setStatus(s);
}
void NOR3::selectYourSelf(Output* pOut, color Color) {

}

void NOR3::save(int id, ofstream & file)
{

	file << "NOR3  "<<"  "<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << " L" << this->getLabel() << endl;
}

void NOR3::load(ApplicationManager* pM, ifstream& in)
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

ActionType NOR3::getComponentActionType() {
	return ADD_NOR_GATE_3;
}
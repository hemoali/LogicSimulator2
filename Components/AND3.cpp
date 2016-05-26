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
	if (!getDelete()) pOut->DrawAnd_Nand(m_CenterInfo,3, false, highlight, false, this);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1, true);
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
	return m_InputPins[n ].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin to HIGH or LOW
void AND3::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n ].setStatus(s);
}
void AND3::selectYourSelf(Output* pOut, color Color) {
	
}

void AND3::save(int id, ofstream & file)
{
	setID(id);
	file << "AND3  "<< "  " << id <<"  "<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 <<  " L" << this->getLabel() << endl;
}

void AND3::load(ApplicationManager*pM, ifstream& in)
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
ActionType AND3::getComponentActionType() {
	return ADD_AND_GATE_3;
}
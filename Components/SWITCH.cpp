#include "SWITCH.h"
#include"..\ApplicationManager.h"
#include<fstream>

SWITCH::SWITCH(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Component(r_GfxInfo, r_FanOut)
{
	m_OutputPin.setComponent(this);
	setNumOfInputs(0);
	setOutputPinStatus(LOW);
}


void SWITCH::Operate()
{
}


// Function Draw
// Draws SWITCH
void SWITCH::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass SWITCH drawing info to it.
	if (!getDelete())pOut->DrawSwtich(m_CenterInfo, (getOutputPin()->getStatus() == HIGH) ? true : false, highlight, false, this);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int SWITCH::GetOutPinStatus()
{
	return getOutputPin()->getStatus();
}


//returns status of Inputpin #n
int SWITCH::GetInputPinStatus(int n)
{
	return-1; //SWITCH
}

//Set status of an input pin to HIGH or LOW
void SWITCH::setOutputPinStatus(STATUS s)
{
	getOutputPin()->setStatus(s);
}
void SWITCH::setInputPinStatus(int n, STATUS s)
{
	return;
}
SWITCH::~SWITCH(){

}
void SWITCH::selectYourSelf(Output* pOut, color Color) {

}

void SWITCH::save(int id, ofstream & file)
{

	file << "SWITCH  "<< " "<< m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << " L" << this->getLabel() << endl;
}

void SWITCH::load(ApplicationManager* pM, ifstream& in)
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
ActionType SWITCH::getComponentActionType() {
	return ADD_Switch;
}
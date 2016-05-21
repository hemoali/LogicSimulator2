#include "LED.h"
#include"..\ApplicationManager.h"
#include<fstream>

LED::LED(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Component(r_GfxInfo, r_FanOut)
{
	setNumOfInputs(1);
	m_InputPins = new InputPin[1];
	m_InputPins[0].setConnection(NULL);
	m_InputPins[0].setComponent(this);
}
void LED::Operate()
{
}

// Function Draw
// Draws LED
void LED::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass LED drawing info to it.
	if (!getDelete()) pOut->DrawLed(m_CenterInfo, (getInputPin(0)->getStatus() == HIGH) ? true : false, highlight, false, this);

	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1, true);
	}
}
int LED::GetOutPinStatus()
{
	return -1; //LED 
}


//returns status of Inputpin #n
int LED::GetInputPinStatus(int n)
{
	return getInputPin(n = 0)->getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin to HIGH or LOW
void LED::setInputPinStatus(int n, STATUS s)
{
	getInputPin(n = 0)->setStatus(s);
}
LED::~LED(){

}
void LED::selectYourSelf(Output* pOut, color Color) {

}

void LED::save(int id, ofstream & file)
{
	file << "LED  "<< "  " << m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << " L" << this->getLabel() << endl;
}

void LED::load(ApplicationManager* pM, ifstream& in)
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

ActionType LED::getComponentActionType() {
	return ADD_LED;
}
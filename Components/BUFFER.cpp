#include "BUFFER.h"
#include"..\ApplicationManager.h"
#include<fstream>

BUFFER::BUFFER(const GraphicsInfo &r_GfxInfo, int r_FanOut) :Gate(1, r_FanOut, r_GfxInfo)
{
	m_GfxInfo.x1 = r_GfxInfo.x1;
	m_GfxInfo.y1 = r_GfxInfo.y1;
	m_GfxInfo.x2 = r_GfxInfo.x2;
	m_GfxInfo.y2 = r_GfxInfo.y2; 
	m_CenterInfo.x1 = m_GfxInfo.x1 + UI.GATE_Width / 2;
	m_CenterInfo.y1 = m_GfxInfo.y1 + UI.GATE_Height / 2;
}


void BUFFER::Operate()
{
	//caclulate the output status as the ORing of the two input pins;
	m_OutputPin.setStatus((m_InputPins[0].getStatus() == HIGH) ? HIGH : LOW);
}


// Function Draw
// Draws BUFFER gate
void BUFFER::Draw(Output* pOut, bool highlight)
{
	//Call output class and pass gate drawing info to it.
	if (!getDelete()) 	pOut->DrawNot_Buffer(m_CenterInfo, true, highlight);
	else {
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1, m_CenterInfo.y1);
	}
}

//returns status of outputpin
int BUFFER::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}

//returns status of the Inputpin 
int BUFFER::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();
}

//Set status of the input pin to HIGH or LOW
void BUFFER::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[0].setStatus(s);
}
void BUFFER::selectYourSelf(Output* pOut, color Color) {

}

void BUFFER::save(int id, ofstream & file)
{

	file << "BUFFER  " << id << "  " << /*this->getLabel() << "  " <<*/ m_GfxInfo.x1 << "  " << m_GfxInfo.y1 << endl;
}

void BUFFER::load(ApplicationManager*pM)
{
	pM->AddComponent(this);
}
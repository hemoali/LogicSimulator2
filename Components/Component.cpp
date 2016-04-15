#include "Component.h"

Component::Component(const GraphicsInfo &r_GfxInfo, int r_FanOut) :m_OutputPin(r_FanOut)
{
	m_GfxInfo.x1 = r_GfxInfo.x1;
	m_GfxInfo.x2 = r_GfxInfo.x1;
	m_GfxInfo.y1 = r_GfxInfo.y1;
	m_GfxInfo.y2 = r_GfxInfo.y1;
	m_CenterInfo.x1 = m_GfxInfo.x1 + UI.GATE_Width / 2;
	m_CenterInfo.y1 = m_GfxInfo.y1 + UI.GATE_Height / 2;

	deleted = false;
	smallCleanImageBeforeAddingComp = NULL;
}
void Component::setLabel(string s){
	m_Label = s;
}
string Component::getLabel(){ return m_Label; }

void Component::setDelete(bool d){
	deleted = d;
}
bool Component::getDelete(){ return deleted; }

void Component::setSmallCleanImageBeforeAddingComp(image* i){
	smallCleanImageBeforeAddingComp = i;
}
image* Component::getSmallCleanImageBeforeAddingComp(){
	return smallCleanImageBeforeAddingComp;
}
void Component::setNewCenterLocation(GraphicsInfo GfxInfo){
	m_CenterInfo.x1 = GfxInfo.x1;
	m_CenterInfo.y1 = GfxInfo.y1;

	m_GfxInfo.x1 = m_CenterInfo.x1 - UI.GATE_Width / 2;
	m_GfxInfo.x2 = m_CenterInfo.x1 + UI.GATE_Width / 2;
	m_GfxInfo.y1 = m_CenterInfo.y1 - UI.GATE_Height / 2;
	m_GfxInfo.y2 = m_CenterInfo.y1 + UI.GATE_Height / 2;
}
GraphicsInfo Component::getCenterLocation(){
	return m_CenterInfo;
}
int Component::getNumOfInputs()const
{
	return m_Inputs;
}
OutputPin*Component::getOutputPin()
{
	OutputPin*ptr = &m_OutputPin;
	return ptr;
}

InputPin*Component::getInputPin(int idx)
{
	InputPin*ptr = &m_InputPins[idx];
	return ptr;
}

void Component::setNumOfInputs(int n)
{
	m_Inputs = n;
}
Component::~Component()
{}


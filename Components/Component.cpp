#include "Component.h"
int Component::lastID = 0;
Component::Component(const GraphicsInfo &r_GfxInfo, int r_FanOut) :m_OutputPin(r_FanOut), ID(++lastID)
{

	m_GfxInfo.x1 = r_GfxInfo.x1;
	m_GfxInfo.x2 = r_GfxInfo.x2;
	m_GfxInfo.y1 = r_GfxInfo.y1;
	m_GfxInfo.y2 = r_GfxInfo.y2;
	m_CenterInfo.x1 = m_GfxInfo.x1 + UI.GATE_Width / 2;
	m_CenterInfo.y1 = m_GfxInfo.y1 + UI.GATE_Height / 2;

	deleted = false;
	smallCleanImageBeforeAddingComp = NULL;
}
Component::Component(const GraphicsInfo & r_GfxInfo) :m_OutputPin(0), ID(++lastID)
{
	m_GfxInfo.x1 = r_GfxInfo.x1;
	m_GfxInfo.x2 = r_GfxInfo.x2;
	m_GfxInfo.y1 = r_GfxInfo.y1;
	m_GfxInfo.y2 = r_GfxInfo.y2;
	deleted = false;
}
void Component::setLabel(string s){
	m_Label = s;
}
string Component::getLabel(){ return m_Label; }

void Component::setDelete(bool d){
	deleted = d;
}
bool Component::getDelete(){ return deleted; }

void Component::setID(int id)
{
	this->ID = id;
}

int Component::getID()
{
	return this->ID;
}
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
GraphicsInfo Component::getCornersLocation() {
	return m_GfxInfo;
}
void Component::setCornersLocation(GraphicsInfo GfxInfo) {
	m_GfxInfo.x1 = GfxInfo.x1;
	m_GfxInfo.y1 = GfxInfo.y1;
	m_GfxInfo.x2 = GfxInfo.x2;
	m_GfxInfo.y2 = GfxInfo.y2;
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

void Component::getAllInputConnections(vector<Connection*> &allInputConnections)
{
	allInputConnections.clear();
	for (size_t i = 0; i < m_Inputs; i++)
	{
		if (m_InputPins[i].getConnection()!=NULL && !((Component*)m_InputPins[i].getConnection())->getDelete())
		{
			allInputConnections.push_back(m_InputPins[i].getConnection());
		}
	}
}
void Component::getAllOutputConnections(vector<Connection*>& allOutputConnections)
{
	allOutputConnections.clear();
	for (size_t i = 0; i < m_OutputPin.connectedConnectionsCount(); i++)
	{
		if (((Component*) m_OutputPin.getConnection(i))->getDelete() || m_OutputPin.getConnection(i) == NULL)
		{
			continue;
		}
		allOutputConnections.push_back(m_OutputPin.getConnection(i));
	}
}
void Component::setNumOfInputs(int n)
{
	m_Inputs = n;
}
Component::~Component()
{}


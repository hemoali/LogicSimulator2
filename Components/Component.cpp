#include "Component.h"

Component::Component(const GraphicsInfo &r_GfxInfo)
{
	m_GfxInfo = r_GfxInfo;
}
void Component::setLabel(string s){
	m_Label = s;
}
string Component::getLabel(){ return m_Label; }

void Component::setDelete(bool d){
	deleted = d;
}
bool Component::getDelete(){ return deleted; }
Component::Component()
{
	deleted = false;
}

void Component::setSmallCleanImageBeforeAddingComp(image* i){
	smallCleanImageBeforeAddingComp = i;
}
image* Component::getSmallCleanImageBeforeAddingComp(){
	return smallCleanImageBeforeAddingComp;
}

void Component::setNewLocation(GraphicsInfo GfxInfo){
	m_CenterInfo.x1 = GfxInfo.x1;
	m_CenterInfo.y1 = GfxInfo.y1;

	m_GfxInfo.x1 = m_CenterInfo.x1 - UI.GATE_Width / 2;
	m_GfxInfo.x2 = m_CenterInfo.x1 + UI.GATE_Width / 2;
	m_GfxInfo.y1 = m_CenterInfo.y1 - UI.GATE_Height / 2;
	m_GfxInfo.y2 = m_CenterInfo.y1 + UI.GATE_Height / 2;
}

Component::~Component()
{}


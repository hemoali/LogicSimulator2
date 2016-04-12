#include "Component.h"

Component::Component(const GraphicsInfo &r_GfxInfo)
{
	m_GfxInfo = r_GfxInfo;	
	m_CenterInfo = r_GfxInfo;
	m_CenterInfo.x1 += UI.GATE_Width / 2;
	m_CenterInfo.y1 += UI.GATE_Height / 2;
}
void Component::setLabel(string s){
	m_Label = s;
}

Component::Component()
{}

Component::~Component()
{}


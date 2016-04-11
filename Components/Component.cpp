#include "Component.h"

Component::Component(const GraphicsInfo &r_GfxInfo)
{
	m_GfxInfo = r_GfxInfo;	
}
void Component::setLabel(string s){
	m_Label = s;
}

Component::Component()
{}

Component::~Component()
{}


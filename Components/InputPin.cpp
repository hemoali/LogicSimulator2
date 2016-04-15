#include "InputPin.h"

InputPin::InputPin()
{}

void InputPin::setComponent(Component *pCmp)
{
	this->pComp = pCmp;
}

Component* InputPin::getComponent()
{
	return pComp;
}
void InputPin::setPosition(int p){ this->position = p; }
int InputPin::getPosition(){ return position; }

void InputPin::setConnection(Connection* pConn){ inConnection = pConn; }
Connection* InputPin::getConnection(){
	return inConnection;
}
#include "InputPin.h"
#include "Connection.h"
#include "Component.h"
InputPin::InputPin() 
{
	setConnection(NULL);
}

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

STATUS InputPin::getStatus()
{
	return Pin::getStatus();
}

void InputPin::setConnection(Connection* pConn){ inConnection = pConn; }
Connection* InputPin::getConnection(){
	return inConnection;
}

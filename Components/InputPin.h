#ifndef _INPUTPIN_H
#define _INPUTPIN_H
#include "Pin.h"
class Component;
class Connection;
class InputPin: public Pin	//inherited from class Pin
{
	Component* pComp; //Component at which this pin is associated
	Connection* inConnection;
	int position;
public:
	InputPin();
	void setComponent(Component* pCmp);	//sets the component of this input pin
	Component* getComponent();	//returns the component of this input pin
	void setConnection(Connection* pConn);
	Connection* getConnection();
	void setPosition(int p);
	int getPosition();

	STATUS getStatus();
};

#endif
#include "Connection.h"
#include"..\ApplicationManager.h"
#include<fstream>

Connection::Connection(const GraphicsInfo &r_GfxInfo, OutputPin *pSrcPin, InputPin *pDstPin) :Component(r_GfxInfo)

{
	SrcPin = pSrcPin;
	DstPin = pDstPin;
	isDrawn = false;

}
void Connection::setSourcePin(OutputPin *pSrcPin)
{
	SrcPin = pSrcPin;
}

OutputPin* Connection::getSourcePin()
{
	return SrcPin;
}


void Connection::setDestPin(InputPin *pDstPin)
{
	DstPin = pDstPin;
}

InputPin* Connection::getDestPin()
{
	return DstPin;
}


void Connection::Operate()
{
	//Status of connection destination pin = status of connection source pin
	DstPin->setStatus((STATUS)SrcPin->getStatus());
}

void Connection::Draw(Output* pOut, bool highlight)
{
	if (!isDrawn)
	{
		pOut->DrawConnection(m_GfxInfo, DstPin->getPosition(), DstPin->getComponent()->getCenterLocation(), getCellsBeforeAddingConnection(), false);
		isDrawn = true;
	}
}
void Connection::setIsDrawn(bool isDrawn) {
	this->isDrawn = isDrawn;
}
bool Connection::getIsDrawn() {
	return this->isDrawn;
}

void Connection::deleteConnection(Output* pOut) {
	setDelete(true);
	setIsDrawn(false);
	DstPin->setConnection(NULL);
	SrcPin->removeConnectedConnection(this);
}

int Connection::GetOutPinStatus()	//returns status of outputpin if LED, return -1
{
	return DstPin->getStatus();
}


int Connection::GetInputPinStatus(int n)	//returns status of Inputpin # n if SWITCH, return -1
{
	return SrcPin->getStatus();	//n is ignored as connection has only one input pin (src pin)	
}

void Connection::setInputPinStatus(int n, STATUS s)
{
	SrcPin->setStatus(s);
}

void Connection::setCellsBeforeAddingConnection(vector<Cell> cellsBeforeAddingConnection)
{
	this->cellsBeforeAddingConnection.clear();
	for (size_t i = 0; i < cellsBeforeAddingConnection.size(); i++)
	{
		this->cellsBeforeAddingConnection.push_back({ cellsBeforeAddingConnection[i].x ,cellsBeforeAddingConnection[i].y,cellsBeforeAddingConnection[i].cellType });
	}
}

vector<Cell>& Connection::getCellsBeforeAddingConnection()
{
	return cellsBeforeAddingConnection;
}
void Connection::selectYourSelf(Output* pOut, color Color) {
	pOut->changeConnectionColor(this, Color);
}

void Connection::save(int id, ofstream & file)
{
	Component *theInputPinComponent;
	Component *theOutputPinComponent;
	Connection *theConnection = this;
	theInputPinComponent = theConnection->getDestPin()->getComponent();
	int inputPosistion = theConnection->getDestPin()->getPosition();
	theOutputPinComponent = theConnection->getSourcePin()->getComponent();
	int c1, c2, c3, c4;
	c1 = c2 = c3 = c4 = 0;
	switch (inputPosistion) {
	case 0:
		c3 = theInputPinComponent->getCenterLocation().x1 - 10;
		c4 = theInputPinComponent->getCenterLocation().y1 - 13;
		break;
	case 2:
		c3 = theInputPinComponent->getCenterLocation().x1 - 10;
		c4 = theInputPinComponent->getCenterLocation().y1 + 13;
		break;
	case 1:
		c3 = theInputPinComponent->getCenterLocation().x1 - 10;
		c4 = theInputPinComponent->getCenterLocation().y1;
		break;
	}
	c1 = theOutputPinComponent->getCenterLocation().x1 + 9;
	c2 = theOutputPinComponent->getCenterLocation().y1;
	file << "  " << c1 << "  " << c2 << "  " << c3 << "  " << c4 << " L" << this->getLabel() <<  endl;
}

void Connection::load(string s, ApplicationManager*pM)
{
	pM->AddComponent(this);
	this->setLabel(s);
}
ActionType Connection::getComponentActionType() {
	return ADD_CONNECTION;
}
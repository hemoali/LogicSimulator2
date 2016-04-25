#include "Connection.h"
#include"..\ApplicationManager.h"
#include<fstream>

Connection::Connection(const GraphicsInfo &r_GfxInfo, OutputPin *pSrcPin, InputPin *pDstPin) :Component(r_GfxInfo)

{
	SrcPin = pSrcPin;
	DstPin = pDstPin;

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
	pOut->DrawConnection(m_GfxInfo, DstPin->getPosition(), DstPin->getComponent()->getCenterLocation(), getCellsBeforeAddingConnection(), false);
}


void Connection::deleteConnection(Output* pOut) {
	setDelete(true);
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
	for (size_t i = 0; i < cellsBeforeAddingConnection.size(); i++)
	{
		this->cellsBeforeAddingConnection.push_back({cellsBeforeAddingConnection[i].x ,cellsBeforeAddingConnection[i].y,cellsBeforeAddingConnection[i].cellType });
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
	file << this->getSourcePin()->getComponent()->getID()<< "  " << this->getDestPin()->getComponent()->getID()<<"  " << this->getDestPin()->getPosition() << "\n";
}

void Connection::load(ApplicationManager*pM)
{
	pM->AddComponent(this);
}
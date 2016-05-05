#include "ModifyConnection.h"
#include"..\Components\Component.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"

ModifyConnection::ModifyConnection(ApplicationManager*pApp) : Action(pApp)
{

}
ModifyConnection::ModifyConnection(ApplicationManager*pApp, Connection* c) : Action(pApp), theConnection(c)
{

}
bool ModifyConnection::ReadActionParameters(image *I)
{
	return false;
}
void ModifyConnection::Execute()
{
	
}
void ModifyConnection::Undo()
{

}
void ModifyConnection::Redo()
{

}
ModifyConnection::~ModifyConnection()
{
}

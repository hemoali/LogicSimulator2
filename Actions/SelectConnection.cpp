#include "SelectConnection.h"
#include "..\ApplicationManager.h"
#include "..\GUI\Input.h"
#include "..\Components\Connection.h"
#include<iostream>
using namespace std;

Select_Connection::Select_Connection(ApplicationManager *pApp) :Action(pApp)
{
}

bool Select_Connection::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	return true;
}

void Select_Connection::Execute()
{
	Input* pIn = pManager->GetInput();
	Output* pOut = pManager->GetOutput();

	pIn->getToBeSelectedConnection()->selectYourSelf(pOut, UI.SelectColor);
	pIn->setToBeSelectedConnection(NULL);
}

void Select_Connection::Undo()
{}

void Select_Connection::Redo()
{}


Select_Connection::~Select_Connection()
{
}

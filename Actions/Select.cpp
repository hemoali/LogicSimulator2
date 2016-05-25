#include "Select.h"
#include "..\ApplicationManager.h"
#include<iostream>
using namespace std;

Select::Select(ApplicationManager *pApp) :Action(pApp)
{
}

bool Select::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();
	int x, y;
	return true;
}

void Select::Execute()
{
	//Get Center point of the Gate
}

void Select::Undo()
{}

void Select::Redo()
{}


Select::~Select()
{
}

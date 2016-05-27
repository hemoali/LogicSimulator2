#include "Load.h"
#include<string>
#include"..\Components\Component.h"
#include"..\Components\AND2.h"
#include"..\Components\AND3.h"
#include"..\Components\BUFFER.h"
#include"..\Components\LED.h"
#include"..\Components\NAND2.h"
#include"..\Components\NOR2.h"
#include"..\Components\NOR3.h"
#include"..\Components\NOT.h"
#include"..\Components\OR2.h"
#include"..\Components\SWITCH.h"
#include"..\Components\XNOR2.h"
#include"..\Components\XOR2.h"
#include"..\Components\XOR3.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"
#include "AddConnection.h"
#include "New.h"
#include <iostream>
using namespace std;

Load::Load(ApplicationManager*pApp) : Action(pApp)
{
	path = "";
}
bool Load::ReadActionParameters(image *, Component* c)
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	int selected = pOut->printPopUpMessage("Do you want to load? All unsaved progress will be lost.", 'L');
	if (selected == 1) {
		bool loadSuccess = false;
		loadSuccess = pOut->loadFile();
		path = pIn->getLoadPath();
		return true;
	}
	return false;
}
void Load::Execute()
{
	Clear newAction(pManager);
	Output *pOut = pManager->GetOutput();
	if (ReadActionParameters(NULL, NULL) && path != "") {
		// To clear the screen

		file.open(path);
		pManager->LoadComponents(file);
		file.close();
		// clear undo/redo stacks
		while (!Utils::undoActions.empty()) Utils::undoActions.pop();
		while (!Utils::redoActions.empty()) Utils::redoActions.pop();
	}
}
void Load::Undo()
{

}
void Load::Redo()
{

}
Load::~Load()
{
}

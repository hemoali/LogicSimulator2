#include "Paste.h"
#include"Delete.h"
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
#include <iostream>
using namespace std;
Paste::Paste(ApplicationManager* pApp) :Action(pApp)
{

}

Paste::Paste(ApplicationManager* pApp, Component* C, bool pushToUndo) : Action(pApp)
{
	theComponent = C;
	this->pushToUndo = pushToUndo;
}

bool Paste::ReadActionParameters(image *, Component* c)
{
	return true;
}

void Paste::Execute()
{
	Output* pOut = pManager->GetOutput();

	//create the new object
	if (dynamic_cast<AND2*>(pManager->PastedComponent)) {
		pastedcomponent = new AND2(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<AND3*>(pManager->PastedComponent)) {
		pastedcomponent = new AND3(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<BUFFER*>(pManager->PastedComponent)) {
		pastedcomponent = new BUFFER(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<LED*>(pManager->PastedComponent)) {
		pastedcomponent = new LED(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<NAND2*>(pManager->PastedComponent)) {
		pastedcomponent = new NAND2(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<OR2*>(pManager->PastedComponent)) {
		pastedcomponent = new OR2(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<NOR3*>(pManager->PastedComponent)) {
		pastedcomponent = new NOR3(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<NOR2*>(pManager->PastedComponent)) {
		pastedcomponent = new NOR2(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<NOT*>(pManager->PastedComponent)) {
		pastedcomponent = new NOT(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<SWITCH*>(pManager->PastedComponent)) {
		pastedcomponent = new SWITCH(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<XNOR2*>(pManager->PastedComponent)) {
		pastedcomponent = new XNOR2(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<XOR2*>(pManager->PastedComponent)) {
		pastedcomponent = new XOR2(pManager->pastepoint, FANOUT);
	}
	else if (dynamic_cast<XOR3*>(pManager->PastedComponent)) {
		pastedcomponent = new XOR3(pManager->pastepoint, FANOUT);
	}

	//set parametees
	GraphicsInfo Gffx;
	image* smallImageBeforeAddingComponent = new image;
	Gffx.x1 = pManager->pastepoint.x1 - UI.GATE_Width / 2;
	Gffx.x2 = pManager->pastepoint.x1 + UI.GATE_Width / 2;
	Gffx.y1 = pManager->pastepoint.y1 - UI.GATE_Height / 2;
	Gffx.y2 = pManager->pastepoint.y1 + UI.GATE_Height / 2;
	if (pastedcomponent != NULL && Utils::CheckPoint({ pManager->pastepoint.x1 ,  pManager->pastepoint.y1 }, pOut, false, true))
	{
		//push to needed vectors
		pManager->AddComponent(pastedcomponent);
		pastedcomponent->setLabel(pManager->PastedComponent->getLabel());
		pastedcomponent->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);
		pOut->storeimagefordeletegate(smallImageBeforeAddingComponent, { pManager->pastepoint.x1 ,  pManager->pastepoint.y1 });
		pastedcomponent->setNewCenterLocation(pManager->pastepoint);
		pastedcomponent->Draw(pOut); // draw
		
		pA = pastedcomponent;
		Utils::undoActions.push(this);
	}
	else pOut->PrintStatusBox("Please select point within workspace and avoid overlaping!");
}
void Paste::Redo()
{
	pA->setDelete(false);
	pA->Draw(pManager->GetOutput(), false);
}
void Paste::Undo()
{
	pA->setDelete(true);
	pA->Draw(pManager->GetOutput(), false);
}
Paste::~Paste()
{

}

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
	if (dynamic_cast<AND2*>(pManager->getPastedComponent())) {
		pastedcomponent = new AND2(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<AND3*>(pManager->getPastedComponent())) {
		pastedcomponent = new AND3(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<BUFFER*>(pManager->getPastedComponent())) {
		pastedcomponent = new BUFFER(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<LED*>(pManager->getPastedComponent())) {
		pastedcomponent = new LED(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<NAND2*>(pManager->getPastedComponent())) {
		pastedcomponent = new NAND2(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<OR2*>(pManager->getPastedComponent())) {
		pastedcomponent = new OR2(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<NOR3*>(pManager->getPastedComponent())) {
		pastedcomponent = new NOR3(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<NOR2*>(pManager->getPastedComponent())) {
		pastedcomponent = new NOR2(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<NOT*>(pManager->getPastedComponent())) {
		pastedcomponent = new NOT(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<SWITCH*>(pManager->getPastedComponent())) {
		pastedcomponent = new SWITCH(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<XNOR2*>(pManager->getPastedComponent())) {
		pastedcomponent = new XNOR2(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<XOR2*>(pManager->getPastedComponent())) {
		pastedcomponent = new XOR2(pManager->getPastePoint(), FANOUT);
	}
	else if (dynamic_cast<XOR3*>(pManager->getPastedComponent())) {
		pastedcomponent = new XOR3(pManager->getPastePoint(), FANOUT);
	}

	//set parametees
	GraphicsInfo Gffx;
	image* smallImageBeforeAddingComponent = new image;
	Gffx.x1 = pManager->getPastePoint().x1 - UI.GATE_Width / 2;
	Gffx.x2 = pManager->getPastePoint().x1 + UI.GATE_Width / 2;
	Gffx.y1 = pManager->getPastePoint().y1 - UI.GATE_Height / 2;
	Gffx.y2 = pManager->getPastePoint().y1 + UI.GATE_Height / 2;
	if (pastedcomponent != NULL && Utils::CheckPoint({ pManager->getPastePoint().x1 ,  pManager->getPastePoint().y1 }, pOut, false, true))
	{
		//push to needed vectors
		pManager->AddComponent(pastedcomponent);
		pastedcomponent->setLabel(pManager->getPastedComponent()->getLabel());
		pastedcomponent->setSmallCleanImageBeforeAddingComp(smallImageBeforeAddingComponent);
		pOut->storeimagefordeletegate(smallImageBeforeAddingComponent, { pManager->getPastePoint().x1 ,  pManager->getPastePoint().y1 });
		pastedcomponent->setNewCenterLocation(pManager->getPastePoint());
		pastedcomponent->Draw(pOut); // draw
		
		pA = pastedcomponent;
		Utils::undoActions.push(this);
	}
	else {
		//The Gate has no chance to live, It's dooomed
		delete pastedcomponent;
		pOut->PrintStatusBox("Please select point within workspace and avoid overlaping!");
	}
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

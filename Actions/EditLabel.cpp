#include "EditLabel.h"



EditLabel::EditLabel(ApplicationManager *pApp):Action(pApp)
{
	theComponenet = NULL;
}

EditLabel::EditLabel(ApplicationManager *pApp,Component* C) : Action(pApp)
{
	theComponenet = C;
}


bool EditLabel::ReadActionParameters(image *)
{
	Output* pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	//To be Modified Later with Menus 
	theNewLabel = pIn->EditComponenetLabel(pOut);
	if (theComponenet == NULL) return false; //Idon't know whether this condtion is gonna be executed or not 
	return true;
}

void EditLabel::Execute()
{
	Output* pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	if (this->ReadActionParameters()) {
		theOriginalLabel = theComponenet->getLabel();
		theComponenet->setLabel(theNewLabel);
		pOut->PrintMsg("Label changed Successfully");
		Utils::undoActions.push(this);
	}
	else {
		pOut->PrintMsg("Error editing the label!");
	}
}

void EditLabel::Undo()
{
	theComponenet->setLabel(theOriginalLabel);
}


void EditLabel::Redo()
{
	theComponenet->setLabel(theNewLabel);
}

EditLabel::~EditLabel()
{
}

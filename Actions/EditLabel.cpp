#include "EditLabel.h"



EditLabel::EditLabel(ApplicationManager *pApp):Action(pApp)
{
	theComponenet = NULL;
}

EditLabel::EditLabel(ApplicationManager *pApp,Component* C) : Action(pApp)
{
	theComponenet = C;
}

bool EditLabel::ReadActionParameters(image *, Component* c)
{
	Output* pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	// Get new label
	theNewLabel = pIn->getStringBox(theComponenet->getLabel());
	if (theComponenet == NULL) return false;
	return true;
}

void EditLabel::Execute()
{
	Output* pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	if (this->ReadActionParameters(NULL, NULL)) {
		theOriginalLabel = theComponenet->getLabel();
		theComponenet->setLabel(theNewLabel); // Assign the new label
		if( theNewLabel!= theOriginalLabel)
		pOut->PrintStatusBox("Label changed Successfully");
		Utils::undoActions.push(this);
	}
	else {
		pOut->PrintStatusBox("Error editing the label!");
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

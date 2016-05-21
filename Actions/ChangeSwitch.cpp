#include "ChangeSwitch.h"
#include"Simulate.h"
ChangeSwitch::ChangeSwitch(ApplicationManager* pApp) :Action(pApp)
{

}

ChangeSwitch::ChangeSwitch(ApplicationManager* pApp, SWITCH* C) : Action(pApp)
{
	theComponent = C;
}

bool ChangeSwitch::ReadActionParameters(image *, Component* c)
{
	if (theComponent == NULL)
		return false;
	return true;
}

void ChangeSwitch::Execute()
{
	Output* pOut = pManager->GetOutput();
	if (this->ReadActionParameters(NULL, NULL)) {
		oldStatus = theComponent->GetOutPinStatus();
		newStatus = !oldStatus;
		theComponent->setOutputPinStatus(static_cast<STATUS>(newStatus));
		theComponent->Draw(pOut, false);
		Utils::simulationUndoActions.push(this);		
	}	
}
void ChangeSwitch::Undo()
{
	Output* pOut = pManager->GetOutput();
	theComponent->setOutputPinStatus(static_cast<STATUS>(oldStatus));
	theComponent->Draw(pOut, false);
	Simulate simAction(pManager, false);
	simAction.Execute();
}

void ChangeSwitch::Redo()
{
	Output *pOut = pManager->GetOutput();
	theComponent->setOutputPinStatus(static_cast<STATUS>(newStatus));
	theComponent->Draw(pOut, false);
	Simulate simAction(pManager, false);
	simAction.Execute();
}

ChangeSwitch::~ChangeSwitch()
{
}

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
		oldStatus = theComponent->GetOutPinStatus(); // old status for undo
		newStatus = !oldStatus; // change the status
		theComponent->setOutputPinStatus(static_cast<STATUS>(newStatus)); // set new status
		theComponent->Draw(pOut, false); // redraw the switch
		Utils::simulationUndoActions.push(this); //push to undo simualtion actions
	}	
}
void ChangeSwitch::Undo()
{
	// back to old status and simulate
	Output* pOut = pManager->GetOutput();
	theComponent->setOutputPinStatus(static_cast<STATUS>(oldStatus));
	theComponent->Draw(pOut, false);
	Simulate simAction(pManager, false);
	simAction.Execute();
}

void ChangeSwitch::Redo()
{
	// back to new status and simulate
	Output *pOut = pManager->GetOutput();
	theComponent->setOutputPinStatus(static_cast<STATUS>(newStatus));
	theComponent->Draw(pOut, false);
	Simulate simAction(pManager, false);
	simAction.Execute();
}

ChangeSwitch::~ChangeSwitch()
{
}

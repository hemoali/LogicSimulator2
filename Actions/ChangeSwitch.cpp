#include "ChangeSwitch.h"
#include"Simulate.h"
ChangeSwitch::ChangeSwitch(ApplicationManager* pApp) :Action(pApp)
{

}

ChangeSwitch::ChangeSwitch(ApplicationManager* pApp, SWITCH* C) : Action(pApp)
{
	theComponent = C;
}

bool ChangeSwitch::ReadActionParameters(image *)
{
	if (theComponent == NULL)
		return false;
	return true;
}

void ChangeSwitch::Execute()
{
	Output* pOut = pManager->GetOutput();
	if (this->ReadActionParameters()) {
		oldStatus = theComponent->GetOutPinStatus();
		newStatus = !oldStatus;
		theComponent->setOutputPinStatus(static_cast<STATUS>(newStatus));
		theComponent->Draw(pOut, false);
		Utils::simulationUndoActions.push(this);
		
	}
	else {
		pOut->PrintStatusBox("It's not deleted.. Error");
	}
}
void ChangeSwitch::Undo()
{
	Output* pOut = pManager->GetOutput();
	theComponent->setOutputPinStatus(static_cast<STATUS>(oldStatus));
	theComponent->Draw(pOut, false);
	Action* pAct = new Simulate(pManager, false);
	pAct->Execute();
}

void ChangeSwitch::Redo()
{
	Output *pOut = pManager->GetOutput();
	theComponent->setOutputPinStatus(static_cast<STATUS>(newStatus));
	theComponent->Draw(pOut, false);
	Action* pAct = new Simulate(pManager, false);
	pAct->Execute();
}

ChangeSwitch::~ChangeSwitch()
{
}

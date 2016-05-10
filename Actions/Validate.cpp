#include "Validate.h"
#include"..\ApplicationManager.h"
#include"..\Components\Component.h"
#include"..\Components\Connection.h"
#include"..\Components\LED.h"
#include"..\Components\SWITCH.h"
#include "Simulate.h"
Validate::Validate(ApplicationManager*pApp) : Action(pApp)
{
	msg = "";
}

bool Validate::ReadActionParameters(image *I)
{
	return false;
}
bool Validate::validateOutputComponent(Component* comp) {
	if (comp != NULL && dynamic_cast<LED*>(comp))
	{
		return true;
	}
	if (comp == NULL || comp->getOutputPin()->connectedConnectionsCount() == 0) {
		msg = "Error: Floating pins";
		return false;
	}
	bool t[3] = { true, true, true };
	for (size_t j = 0; j < comp->getOutputPin()->connectedConnectionsCount(); j++)
	{
		t[j] = validateOutputComponent(comp->getOutputPin()->getConnection(j)->getDestPin()->getComponent());
	}
	return t[0] && t[1] && t[2];
}
bool Validate::validateInputComponent(Component* comp) {
	if (comp != NULL && dynamic_cast<SWITCH*>(comp))
	{
		return true;
	}
	if (comp == NULL || ((comp->getNumOfInputs() == 3 && (comp->getInputPin(0)->getConnection() == NULL || comp->getInputPin(1)->getConnection() == NULL || comp->getInputPin(2)->getConnection() == NULL))
		|| (comp->getNumOfInputs() == 2 && (comp->getInputPin(0)->getConnection() == NULL || comp->getInputPin(1)->getConnection() == NULL))
		|| (comp->getNumOfInputs() == 1 && (comp->getInputPin(0)->getConnection() == NULL)))) {
		msg = "Error: Floating pins";
		return false;
	}
	bool t[3] = { true, true, true };
	if (comp->getNumOfInputs() == 3)
	{
		t[0] = comp->getInputPin(0)->getConnection()->getSourcePin()->getComponent();
		t[1] = comp->getInputPin(0)->getConnection()->getSourcePin()->getComponent();
		t[2] = comp->getInputPin(0)->getConnection()->getSourcePin()->getComponent();
	}
	else if (comp->getNumOfInputs() == 2)
	{
		t[0] = comp->getInputPin(0)->getConnection()->getSourcePin()->getComponent();
		t[1] = comp->getInputPin(0)->getConnection()->getSourcePin()->getComponent();
	}
	else if (comp->getNumOfInputs() == 1)
	{
		t[0] = comp->getInputPin(0)->getConnection()->getSourcePin()->getComponent();
	}
	return t[0] && t[1] && t[2];
}
void Validate::Execute()
{
	msg = "";
	bool isValid = true;
	int cnt = 0;
	for (size_t i = 0; i < pManager->allComponentsCorners.size() && isValid; i++)
	{
		Component* comp = pManager->getComponent(i);

		if (dynamic_cast<Connection*> (comp))continue;

		if (comp->getDelete())continue;
		cnt++;
		if (!dynamic_cast<LED*> (comp))
		{
			isValid = validateOutputComponent(comp);
		}
		if (isValid)
		{
			if (!dynamic_cast<SWITCH*> (comp))
			{
				isValid = validateInputComponent(comp);
			}
			if (!isValid)
			{
				if (msg == "")
				{
					msg = "First level isn't all switches";
				}
			}
		}
		else {
			if (msg == "")
			{
				msg = "Last level isn't all LEDs";
			}
		}
	}
	if (isValid && cnt >0)
	{
		pManager->GetInput()->switchMode(SIMULATION, pManager);
		Action* act = new Simulate(pManager);
		act->Execute();
	}
	pManager->GetOutput()->printPopUpMessage(msg);
	Sleep(600);
}
void Validate::Undo()
{

}
void Validate::Redo()
{

}
Validate::~Validate()
{

}

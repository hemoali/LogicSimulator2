#include "Validate.h"
#include"..\ApplicationManager.h"
#include"..\Components\Component.h"
#include"..\Components\Connection.h"
#include"..\Components\LED.h"
#include"..\Components\SWITCH.h"
#include "Simulate.h"
#include <string>
using namespace std;
Validate::Validate(ApplicationManager*pApp) : Action(pApp)
{
	msg = "";
	isValid = true;
	cnt = 0;
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
		if (msg.find("Floating") == string::npos) {
			msg += ((msg.length() == 0) ? "" : "\n"); msg += " • Error: Floating pins";
		}
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
		if (msg.find("Floating") == string::npos) {
			msg += ((msg.length() == 0) ? "" : "\n"); msg += " • Error: Floating pins";
		}
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

	for (size_t i = 0; i < Utils::allComponentsCorners.size() && isValid && isValid2; i++)
	{
		Component* comp = pManager->getComponent(i);

		if (dynamic_cast<Connection*> (comp))continue;

		if (comp->getDelete())continue;
		cnt++;
		if (!dynamic_cast<LED*> (comp))
		{
			isValid = validateOutputComponent(comp);
			if (!isValid && msg.find("LEDs") == string::npos)
			{
				msg += ((msg.length() == 0) ? "" : "\n"); msg += " • Last level isn't all LEDs";
			}
		}

		if (!dynamic_cast<SWITCH*> (comp))
		{
			isValid2 = validateInputComponent(comp);
		}
		if (!isValid2 && msg.find("switches") == string::npos)
		{
			msg += ((msg.length() == 0) ? "" : "\n"); msg += " • First level isn't all switches";
		}

	}
	if (isValid && isValid2 && cnt > 0)
	{
		pManager->GetOutput()->switchMode(SIMULATION);
		UI.AppMode = SIMULATION;
	}
	pManager->GetOutput()->printPopUpMessage(msg);
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

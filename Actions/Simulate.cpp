#include "Simulate.h"
#include"..\ApplicationManager.h"
#include "..\Components\LED.h"
#include "..\Components\SWITCH.h"
#include "..\Components\Connection.h"
#include "..\Components\Component.h"
Simulate::Simulate(ApplicationManager*pApp) : Action(pApp)
{
}

bool Simulate::ReadActionParameters(image *I)
{
	return false;
}
void Simulate::Execute()
{
	for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<SWITCH*>(comp)) || (dynamic_cast<Connection*>(comp) || comp->getDelete())) continue;
		for (size_t i = 0; i < comp->getNumOfInputs(); i++)
		{
			comp->setInputPinStatus(i, UNDEFINED);
		}
	}
	int totalComponentsCount = 0, operatedItemsCount = 0;
	vector<Component*> toBeOperatedComponents;
	// Send switches signals to all next components
	for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<Connection*>(comp) || comp->getDelete())) continue;
		if (dynamic_cast<SWITCH*>(comp))
		{
			for (size_t i = 0; i < comp->getOutputPin()->connectedConnectionsCount(); i++)
			{
				comp->getOutputPin()->getConnection(i)->getDestPin()->setStatus(static_cast<STATUS>(comp->GetOutPinStatus()));
			}
		}
		else if (!dynamic_cast<LED*>(comp)) { toBeOperatedComponents.push_back(comp); totalComponentsCount++; }
	}
	//Go to all LEDS and calcaulate the pins values
	while (operatedItemsCount < totalComponentsCount)
	{
		for (size_t i = 0; i < toBeOperatedComponents.size(); i++)
		{
			Component* comp = toBeOperatedComponents[i];
			bool isAllPinsHaveStatus = true;
			for (size_t j = 0; j < comp->getNumOfInputs(); j++)
			{
				if (comp->getInputPin(j)->getStatus() == UNDEFINED)
				{
					isAllPinsHaveStatus = false;
				}
			}
			if (!isAllPinsHaveStatus)
			{
				continue;
			}
			else {
				comp->Operate();
				for (size_t k = 0; k < comp->getOutputPin()->connectedConnectionsCount(); k++)
				{
					comp->getOutputPin()->getConnection(k)->getDestPin()->setStatus(static_cast<STATUS>(comp->GetOutPinStatus()));
				}
				operatedItemsCount++;
			}
		}

	}
	//Light up leds
	for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<Connection*>(comp) || comp->getDelete())) continue;
		if (dynamic_cast<LED*>(comp)) { comp->Draw(pManager->GetOutput(), false); }
	}
	//light up connections
	for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if (comp->getDelete()) continue;
		if (dynamic_cast<Connection*>(comp))
		{
			if (((Connection*)comp)->getSourcePin()->getStatus() == HIGH)
			{
				((Connection*)comp)->selectYourSelf(pManager->GetOutput(), UI.OneValueConnection);
			}
			else {
				((Connection*)comp)->selectYourSelf(pManager->GetOutput(), UI.DrawColor);
			}
		}
	}
}
void Simulate::Undo()
{

}
void Simulate::Redo()
{

}
Simulate::~Simulate()
{

}

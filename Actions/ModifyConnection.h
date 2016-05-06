#pragma once
#include "Action.h"
class InputPin;
class OutputPin;
class ModifyConnection :
	public Action
{
	int Cx1, Cy1, Cx2, Cy2;
	Connection *theConnection; // The Selected Connection

	Component* outputComponent = NULL;
	Component*inputComponent = NULL;
	Component*oldOutputComponent = NULL;
	Component*oldInputComponent = NULL;
	InputPin* newInputPin = NULL;
	OutputPin* newOutputPin = NULL;
	InputPin* oldInputPin = NULL;
	OutputPin* oldOutputPin = NULL;

	vector <Cell> oldCells, newCells;
	int inputPin, oldInputPinPosition;

	GraphicsInfo GInfo, oldGInfo;
public:
	ModifyConnection(ApplicationManager* pApp);

	ModifyConnection(ApplicationManager* pApp, Connection* C);

	~ModifyConnection();

	bool validateOutputComponent(Component* comp, Component* dstComp);
	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image * = NULL);

	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();
};


#include "ApplicationManager.h"
#include "Actions\AddANDgate2.h"
#include"Actions\AddANDgate3.h"
#include"Actions\AddBUFFER.h"
#include"Actions\AddLED.h"
#include"Actions\AddNANDgate2.h"
#include"Actions\AddNORgate2.h"
#include"Actions\AddNORgate3.h"
#include"Actions\AddNOTgate.h"
#include"Actions\AddORgate2.h"
#include"Actions\AddSWITCH.h"
#include"Actions\AddXNORgate2.h"
#include"Actions\AddXORgate2.h"
#include"Actions\AddXORgate3.h"
#include"Actions\AddConnection.h"
#include "Actions\Select.h"
#include "Actions\Move.h"
#include "Actions\MultiMove.h"
#include "Actions\RightClick.h"
#include"Actions\Save.h"
#include"Actions\Load.h"
#include "Actions\MultiSelect.h"
#include "Actions\Validate.h"
#include "Actions\Simulate.h"
#include"Actions\CreateTruthTable.h"
#include"Actions\ChangeSwitch.h"
#include"Actions\New.h"
#include "Utils.h"
#include "Actions\Exit.h"

ApplicationManager::ApplicationManager()
{
	CompCount = 0;
	Exitchoice = 0;
	for (int i = 0; i < MaxCompCount; i++)
		CompList[i] = NULL;

	//Creates the Input / Output Objects & Initialize the GUI
	for (int i = 0; i < CompCount; i++) {
		if (dynamic_cast<Connection*> (CompList[i]) && !CompList[i]->getDelete())
		{
			Utils::allConnections.push_back((Connection*)CompList[i]);
		}
	}
	OutputInterface = new Output(&Utils::allConnections);
	InputInterface = OutputInterface->CreateInput();
}
////////////////////////////////////////////////////////////////////
void ApplicationManager::AddComponent(Component* pComp)
{
	CompList[CompCount++] = pComp;
}
////////////////////////////////////////////////////////////////////

ActionType ApplicationManager::GetUserAction()
{
	if (Exitchoice == 1)
	{
		return QUIT;
	}

	Utils::allConnections.clear();
	for (int i = 0; i < CompCount; i++) {
		if (dynamic_cast<Connection*> (CompList[i]) && !CompList[i]->getDelete())
		{
			Utils::allConnections.push_back((Connection*)CompList[i]);
		}
	}
	//Call input to get what action is reuired from the user
	return InputInterface->GetUserAction();
}
////////////////////////////////////////////////////////////////////
void ApplicationManager::ExecuteAction(ActionType ActType)
{
	//GetOutput()->clearHoveringImage(Utils::imgh, Utils::J, Utils::K, Utils::widthh);
	Action* pAct = NULL;
	switch (ActType)
	{
	case ADD_Buff: {
		pAct = new AddBUFFER(this);
		Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_INV: {
		pAct = new AddNOTgate(this);
		Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_AND_GATE_2: {
		pAct = new AddANDgate2(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_OR_GATE_2: {
		pAct = new AddORgate2(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_NAND_GATE_2: {
		pAct = new AddNANDgate2(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_NOR_GATE_2: {
		pAct = new AddNORgate2(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_XOR_GATE_2: {
		pAct = new AddXORgate2(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_XNOR_GATE_2: {
		pAct = new AddXNORgate2(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_AND_GATE_3: {
		pAct = new AddANDgate3(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_NOR_GATE_3: {
		pAct = new AddNORgate3(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_XOR_GATE_3: {
		pAct = new AddXORgate3(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_Switch: {
		pAct = new AddSWITCH(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_LED: {
		pAct = new AddLED(this); Utils::theActions.push_back(pAct);
		break;
	}
	case ADD_CONNECTION: {
		pAct = new AddConnection(this); Utils::theActions.push_back(pAct);
		break;
	}
	case MOVE: {
		pAct = new Move(this); Utils::theActions.push_back(pAct);
		break;
	}
	case MULTI_MOVE: {
		pAct = new MultiMove(this); Utils::theActions.push_back(pAct);
		break;
	}
	case MULTI_SELECT: {
		pAct = new MultiSelect(this); Utils::theActions.push_back(pAct);
		break;
	}
	case Change_Switch: {
		ChangeSwitch* act = new ChangeSwitch(this, GetInput()->toBeChangedSwitch);
		Utils::theActions.push_back(act);
		act->Execute();
		Simulate simulateAction(this, false);
		simulateAction.Execute();
		
		break;
	}
	case Create_TruthTable:
	{
		OutputInterface->PrintStatusBox("Creating truth table ....");
		CreateTruthTable CreateTruthTAction(this);
		CreateTruthTAction.Execute();
		Utils::theActions.push_back(pAct);
		break;
	}
	case RIGHT_CLICKSELECT: {
		RightClick rightClickAction(this);
		rightClickAction.Execute();
		pAct = rightClickAction.getAction();
		Utils::theActions.push_back(pAct);
		break;
	}
	case SAVE: {
		Save saveAction(this);
		saveAction.Execute();
		break;
	}
	case LOAD: {
		Load loadAction(this);
		loadAction.Execute();
		break;
	}
	case ValidateAction: {
		Validate validateAction(this);
		validateAction.Execute();
		break;
	}
	case SimulateAction: {
		Simulate simulateAction(this, true);
		simulateAction.Execute();
		break;
	}
	case SimulateActionWithoutValidation:
	{
		Simulate simulateAction(this, false);
		simulateAction.Execute();
		break;
	}
	case UNDOACTION:
	{
		if (UI.AppMode == DESIGN && Utils::undoActions.size() > 0)
		{
			Action* act = Utils::undoActions.top();
			act->Undo();
			Utils::undoActions.pop();
			Utils::redoActions.push(act);
		}
		else if (UI.AppMode == SIMULATION && Utils::simulationUndoActions.size() > 0) {
			Action* act = Utils::simulationUndoActions.top();
			act->Undo();
			Utils::simulationUndoActions.pop();
			Utils::simulationRedoActions.push(act);
		}
		break;
	}
	case REDOACTION:
	{
		if (UI.AppMode == DESIGN && Utils::redoActions.size() > 0) {
			Action* act = Utils::redoActions.top();
			act->Redo();
			Utils::redoActions.pop();
			Utils::undoActions.push(act);
		}
		else if (UI.AppMode == SIMULATION && Utils::simulationRedoActions.size() > 0) {
			Action* act = Utils::simulationRedoActions.top();
			act->Redo();
			Utils::simulationRedoActions.pop();
			Utils::simulationUndoActions.push(act);
		}
		break;
	}
	case NEW: {
		Clear clearAction(this);
		clearAction.Execute();
		break;
	}
	case DSN_MODE:
		for (size_t i = 0; i < Utils::allComponentsCorners.size(); i++)
		{
			Component* comp = getComponent(i);
			if (comp->getDelete()) continue;
			if (!(dynamic_cast<SWITCH*>(comp)) && !(dynamic_cast<Connection*>(comp)))
			{
				for (size_t i = 0; i < comp->getNumOfInputs(); i++)
				{
					comp->setInputPinStatus(i, UNDEFINED);
				}
				comp->Draw(GetOutput(), false);
			}
			else if (dynamic_cast<SWITCH*>(comp)) {
				((SWITCH*)comp)->setOutputPinStatus(LOW);
				comp->Draw(GetOutput(), false);
			}
			else if (dynamic_cast<Connection*>(comp)) {
				((Connection*)comp)->selectYourSelf(GetOutput(), UI.DrawColor);
			}
		}
		UI.AppMode = DESIGN;
		GetOutput()->switchMode(DESIGN);
		break;
	case SIM_MODE:
		GetOutput()->switchMode(SIMULATION);
		UI.AppMode = SIMULATION;
		break;
	case EXIT:
	{
		//Create a static Exit Actions it doesn't have to be automatic
		Exit exitAction(this);
		exitAction.Execute();
		break;
	}
	}
	if (pAct)
	{
		pAct->Execute();
	}
}
////////////////////////////////////////////////////////////////////
Input* ApplicationManager::GetInput()
{
	return InputInterface;
}
////////////////////////////////////////////////////////////////////
Output* ApplicationManager::GetOutput()
{
	return OutputInterface;
}

////////////////////////////////////////////////////////////////////
int ApplicationManager::getCompCount()
{
	return CompCount;
}
void ApplicationManager::setCompCount(int n)
{
	CompCount = n;
}
void ApplicationManager::setPastePoint(GraphicsInfo s)
{
	pastepoint = s;
}
GraphicsInfo ApplicationManager::getPastePoint()
{
	return pastepoint;
}
void ApplicationManager::setPastedComponent(Component * C)
{
	PastedComponent = C;
}
Component * ApplicationManager::getPastedComponent()
{
	return PastedComponent;
}
void ApplicationManager::setActionType(ActionType s)
{
	cutorcopy = s;
}
ActionType ApplicationManager::getActionType()
{
	return cutorcopy;
}
////////////////////////////////////////////////////////////////////
Component * ApplicationManager::getComponent(int idx)
{
	return CompList[idx];
}
void ApplicationManager::componentLoading(ifstream & in, string compType, GraphicsInfo point)
{
	image* img = new image;
	Component* pA;
	if (compType == "AND2")
	{
		AND2*ptr = new AND2(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "AND3")
	{
		AND3*ptr = new AND3(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "BUFFER")
	{
		BUFFER*ptr = new BUFFER(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "LED")
	{
		LED*ptr = new LED(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "NAND2")
	{
		NAND2*ptr = new NAND2(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "NOR2")
	{
		NOR2*ptr = new NOR2(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "NOR3")
	{
		NOR3*ptr = new NOR3(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "NOT")
	{
		NOT*ptr = new NOT(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "OR2")
	{
		OR2*ptr = new OR2(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "SWITCH")
	{
		SWITCH*ptr = new SWITCH(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "XNOR2")
	{
		XNOR2*ptr = new XNOR2(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "XOR2")
	{
		XOR2*ptr = new XOR2(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	else if (compType == "XOR3")
	{
		XOR3*ptr = new XOR3(point, 3);
		ptr->load(this, in);
		pA = ptr;
	}
	//Filling the needed arrays of the Grid
	GraphicsInfo GInfotmp = point;
	Utils::allComponentsCorners.push_back(point);
	GetOutput()->storeImage(img, pA->getCenterLocation().x1 - UI.GRID_SIZE - 5, pA->getCenterLocation().y1 - UI.GRID_SIZE - 5, 2 * UI.GRID_SIZE + 3, UI.GATE_Height + 3);
	pA->setSmallCleanImageBeforeAddingComp(img);
	//Draw the Loaded Gate 
	pA->Draw(GetOutput());
}
void ApplicationManager::setExitChoice(int x)
{
	Exitchoice = x;
}
ApplicationManager::~ApplicationManager()
{
	Output* pOut = GetOutput();
	ofstream file;
	Action *temp;
	for (int i = 0; i < Utils::theActions.size(); i++)
		delete Utils::theActions[i];
	file.open("Check.txt");
	file << "Using Check.txt file to debug deallocations or any furthur Checking.\n" << "................................................................." <<
		"\nThis Function is Called in the Application Manager Destructor\n" << "................................................................." << endl << endl ;
	file << "Actions constructed  " << Action::ID << endl;
	file << "Actions Destructed  " << Action::IDD << endl;
	file << "\n\n"<< "................................................................." << endl << endl;
	file << "Components constructed  " << Component::CreatedComponents << endl;
	file << "Components Destructed  " << Component::DestructedComponents << endl;
	file.close();
	pOut->printCheck();
	delete OutputInterface;
	delete InputInterface;
}
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
ApplicationManager::ApplicationManager()
{
	CompCount = 0;

	for (int i = 0; i < MaxCompCount; i++)
		CompList[i] = NULL;

	//Creates the Input / Output Objects & Initialize the GUI
	OutputInterface = new Output(this);
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
	//Call input to get what action is reuired from the user
	return InputInterface->GetUserAction(this);
}
////////////////////////////////////////////////////////////////////

void ApplicationManager::ExecuteAction(ActionType ActType)
{
	bool drawGate = true, drawConnection = false;

	Action* pAct = NULL;
	string msg = "Press ESCAPE to stop";
	string error_msg = "Please select point within workspace avoiding overlaping!";
	GraphicsInfo Gfx_info;
	Gfx_info.x1 = Gfx_info.y1 = -1;
	switch (ActType)
	{
	case ADD_Buff:
		pAct = new AddBUFFER(this);
		break;
	case ADD_INV:
		pAct = new AddNOTgate(this);
		break;
	case ADD_AND_GATE_2:
		pAct = new AddANDgate2(this);
		break;
	case ADD_OR_GATE_2:
		pAct = new AddORgate2(this);
		break;
	case ADD_NAND_GATE_2:
		pAct = new AddNANDgate2(this);
		break;
	case ADD_NOR_GATE_2:
		pAct = new AddNORgate2(this);
		break;
	case ADD_XOR_GATE_2:
		pAct = new AddXORgate2(this);
		break;
	case ADD_XNOR_GATE_2:
		pAct = new AddXNORgate2(this);
		break;
	case ADD_AND_GATE_3:
		pAct = new AddANDgate3(this);
		break;
	case ADD_NOR_GATE_3:
		pAct = new AddNORgate3(this);
		break;
	case ADD_XOR_GATE_3:
		pAct = new AddXORgate3(this);
		break;
	case ADD_Switch:
		pAct = new AddSWITCH(this);
		break;
	case ADD_LED:
		pAct = new AddLED(this);
		break;
	case ADD_CONNECTION:
		pAct = new AddConnection(this);
		break;
	case MOVE:
		pAct = new Move(this);
		break;
	case MULTI_MOVE:
		pAct = new MultiMove(this);
		break;
	case RIGHT_CLICKSELECT: {
		pAct = new RightClick(this);
		pAct->Execute();
		RightClick* tmp = (RightClick*)(pAct);
		pAct = tmp->getAction();
		delete tmp;
		break;
	}
	case SAVE:
		pAct = new Save(this);
		break;
	case MULTI_SELECT:
		pAct = new MultiSelect(this);
		break;
	case LOAD:
		pAct = new Load(this);
		break;
	case ValidateAction:
		pAct = new Validate(this);
		break;
	case SimulateAction:
		pAct = new Simulate(this);
		break;
	case UNDOACTION: {
		if (UI.AppMode == DESIGN && undoActions.size() > 0)
		{
			Action* act = undoActions.top();
			act->Undo();
			undoActions.pop();
			redoActions.push(act);
		}
		else if (UI.AppMode == SIMULATION && simulationUndoActions.size() > 0) {
			Action* act = simulationUndoActions.top();
			act->Undo();
			simulationUndoActions.pop();
			simulationRedoActions.push(act);
		}
		break;
	}case REDOACTION: {
		if (UI.AppMode == DESIGN && redoActions.size() > 0) {
			Action* act = redoActions.top();
			act->Redo();
			redoActions.pop();
			undoActions.push(act);
		}
		else if (UI.AppMode == SIMULATION && simulationRedoActions.size() > 0) {
			Action* act = simulationRedoActions.top();
			act->Redo();
			simulationRedoActions.pop();
			simulationUndoActions.push(act);
		}
		break;
	}case EXIT:
		// Exit action here
		break;
	}
	if (pAct)
	{
		pAct->Execute();
		//delete pAct;
		//pAct = NULL;
	}
}
////////////////////////////////////////////////////////////////////

void ApplicationManager::UpdateInterface()
{
	for (int i = 0; i < CompCount; i++)
		CompList[i]->Draw(OutputInterface);

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

void ApplicationManager::getAllConnections(vector<Connection*>& allConnections) {
	allConnections.clear();
	for (int i = 0; i < CompCount; i++) {
		if (dynamic_cast<Connection*> (CompList[i]) && !CompList[i]->getDelete())
		{
			allConnections.push_back((Connection*)CompList[i]);
		}
	}
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
////////////////////////////////////////////////////////////////////
Component * ApplicationManager::getComponent(int idx)
{
	return CompList[idx];
}
ApplicationManager::~ApplicationManager()
{
	for (int i = 0; i < CompCount; i++)
		delete CompList[i];
	delete OutputInterface;
	delete InputInterface;

}
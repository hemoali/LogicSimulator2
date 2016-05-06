#include "CreateTruthTable.h"
#include "Simulate.h"
#include"..\ApplicationManager.h"
#include "..\Components\LED.h"
#include "..\Components\SWITCH.h"
#include "..\Components\Connection.h"
#include "..\Components\Component.h"
#include"Action.h"
#include<vector>
#include <iostream>
#include <windows.h>

using namespace std;

CreateTruthTable::CreateTruthTable(ApplicationManager*pApp) : Action(pApp)
{
}

bool CreateTruthTable::ReadActionParameters(image *I)
{
	return false;
}

void CreateTruthTable::Execute()
{
	Output* pOut = pManager->GetOutput();
	int NumOfInputs = 0, NumOfOutputs = 0;
	for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<SWITCH*>(comp)))
			NumOfInputs++;
		else if ((dynamic_cast<LED*>(comp)))
			NumOfOutputs++;
	}

	//genrating all Compination of the inputs
	vector<vector<int>>AllCompination(1<<NumOfInputs);
	for (int i = 0; i < AllCompination.size(); i++)
	{
		AllCompination[i].resize(NumOfInputs);
		int cur = i;
		for (int j = NumOfInputs-1; j >=0 ; j--)
		{
			AllCompination[i][j] = cur % 2;
			cur /= 2;
		}
	}


	for (size_t i = 0; i < AllCompination.size(); i++)
	{
		int k = 0;
		for (size_t j = 0; j < pManager->allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<SWITCH*>(comp)))
			{
				comp->getOutputPin()->setStatus((STATUS)AllCompination[i][k]);
				comp->Draw(pOut,(STATUS)AllCompination[i][k]);
				k++;
			}
		}
		Action* pAct = new Simulate(pManager);
		pAct->Execute();
		Sleep(500);
		delete pAct;
		for (int j = 0; j < NumOfInputs; j++)
		{
			if (NumOfInputs <= 5)
			{
				cout << AllCompination[i][j] << "  ";
			}
			else
			{
				// To Do : create the truth table in a text file
			}
		}
		for (size_t j = 0; j < pManager->allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<LED*>(comp)))
			{
				if (NumOfInputs <= 5)
				{
					cout << comp->GetInputPinStatus(0) << "  ";
				}
				else
				{
					// To Do : create the truth table in a text file
				}
			}
		}
			cout << endl;
	}
	pOut->PrintMsg("The truth table has been created sucessfully");
}

void CreateTruthTable::Undo()
{

}
void CreateTruthTable::Redo()
{

}

CreateTruthTable::~CreateTruthTable()
{
}

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
#include<fstream>
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
	ofstream file;
	file.open("TruthTable.txt");
	int NumOfInputs = 0, NumOfOutputs = 0;
	for (size_t i = 0; i < Utils::allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<SWITCH*>(comp)))
		{
			if (comp->getDelete())
				continue;
			NumOfInputs++;
			if (comp->getLabel() == "")
				file << "Input " << NumOfInputs << "  ";
			else
			{
				file << comp->getLabel() << "  ";
			}
		}
	}
	for (size_t i = 0; i < Utils::allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<LED*>(comp)))
		{
			if (comp->getDelete())
				continue;
			NumOfOutputs++;
			if (comp->getLabel() == "")
				file << "Output " << NumOfOutputs << "  ";
			else
				file << comp->getLabel() << "  ";
		}

	}
	file << endl;
	

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
		for (size_t j = 0; j < Utils::allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<SWITCH*>(comp)))
			{
				if (comp->getDelete())
					continue;
				comp->getOutputPin()->setStatus((STATUS)AllCompination[i][k]);
				comp->Draw(pOut,(STATUS)AllCompination[i][k]);
				k++;
			}
		}
		Action* pAct = new Simulate(pManager, false);
		pAct->Execute();
		//Sleep(50);
		delete pAct;
		k = 0;
		NumOfInputs = 0;
		NumOfOutputs = 0;
		for (size_t j = 0; j < Utils::allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<SWITCH*>(comp)))
			{
				if (comp->getDelete())
					continue;
				NumOfInputs++;
				if (comp->getLabel() == "")
				{
					for (int K = 0; K < (6 + to_string(NumOfInputs).size())/2; K++)
					{
						file << " ";
					}
					file << AllCompination[i][k] << "  ";
					int siz = ((6 + to_string(NumOfInputs).size()) % 2 != 0) ? ((6 + to_string(NumOfInputs).size()) / 2) : ((6 + to_string(NumOfInputs).size()) / 2 - 1);
					for (int K = 0; K < siz; K++)
					{
						file << " ";
					}
				}
				else
				{
					for (int K = 0; K < comp->getLabel().size() / 2; K++)
					{
						file << " ";
					}
					file << AllCompination[i][k] << "  ";
					int siz = (comp->getLabel().size() % 2 != 0) ? (comp->getLabel().size() / 2) : (comp->getLabel().size() / 2 - 1);
					for (int K = 0; K < siz; K++)
					{
						file << " ";
					}
				}
				k++;
			}
		}
		for (size_t j = 0; j < Utils::allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<LED*>(comp)))
			{
				if (comp->getDelete())
					continue;
				NumOfOutputs++;
				if (comp->getLabel() == "")
				{
					for (int K = 0; K < (7 + to_string(NumOfOutputs).size())/2; K++)
					{
						file << " ";
					}
					file << comp->GetInputPinStatus(0) << "  ";
					int siz = ((7 + to_string(NumOfOutputs).size()) % 2 != 0) ? ((7 + to_string(NumOfOutputs).size()) / 2) : ((7 + to_string(NumOfOutputs).size()) / 2 - 1);
					for (int K = 0; K < siz; K++)
					{
						file << " ";
					}
				}
				else
				{
					for (int K = 0; K < comp->getLabel().size() / 2; K++)
					{
						file << " ";
					}
					file << comp->GetInputPinStatus(0) << "  ";
					int siz = (comp->getLabel().size() % 2 != 0) ? (comp->getLabel().size() / 2) : (comp->getLabel().size() / 2 - 1);
					for (int K = 0; K < siz; K++)
					{
						file << " ";
					}
				}
			}
		}
		file << endl;
	}
	pOut->PrintStatusBox("The truth table has been created sucessfully");
	file.close();
	pOut->PrintTruthTable();
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

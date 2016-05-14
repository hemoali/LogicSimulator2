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
	for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<SWITCH*>(comp)))
		{
			NumOfInputs++;
			if (comp->getLabel() == "")
				file << "Input " << NumOfInputs << "  ";
			else
			{
				file << comp->getLabel() << "  ";
			}
		}
		else if ((dynamic_cast<LED*>(comp)))
		{
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
		Sleep(50);
		delete pAct;
		k = 0;
		for (size_t j = 0; j < pManager->allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<SWITCH*>(comp)))
			{
				if (comp->getLabel() == "")
					file << AllCompination[i][k] << "        ";
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
		for (size_t j = 0; j < pManager->allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<LED*>(comp)))
			{
				if(comp->getLabel()=="")
			      file << comp->GetInputPinStatus(0) <<"         ";
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

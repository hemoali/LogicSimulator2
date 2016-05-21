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
	bool tooLarge = false;
	int NumOfInputs = 0, NumOfOutputs = 0;
	//Calculating the Number of Inputs (the number of Switches)
	for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<SWITCH*>(comp)))
		{
			if (comp->getDelete())
				continue;
			NumOfInputs++;
		}
		else if ((dynamic_cast<LED*>(comp)))
		{
			if (comp->getDelete())
				continue;
			NumOfOutputs++;
		}
	}
	//and later print their Names
	if (NumOfInputs > 4) tooLarge = true;
	string row = "";
	string *table = new string[NumOfInputs*NumOfInputs + 1];
	int RowNum = 0;
	NumOfInputs = NumOfOutputs = 0;
	ofstream file;
	if (tooLarge)
		file.open("TruthTable.txt");
	//printing Labeles  of all switches 
	for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<SWITCH*>(comp)))
		{
			if (comp->getDelete())
				continue;
			NumOfInputs++;
			if (comp->getLabel() == "")
				if (tooLarge)
					file << "Input " << NumOfInputs << "||";
				else {
					row += "Input ";
					row += to_string(NumOfInputs);
					row += "  ";
				}
			else
			{
				if (tooLarge)
					file << comp->getLabel() << "||";
				else {
					row += comp->getLabel();
					row += "  ";
				}
			}
		}
	}
	//printing Lables  of all LEDs
	for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<LED*>(comp)))
		{
			if (comp->getDelete())
				continue;
			NumOfOutputs++;
			if (comp->getLabel() == "")
				if (tooLarge)
					file << "Output " << NumOfOutputs << "||";
				else {
					row += "Output ";
					row += to_string(NumOfOutputs);
					row += "  ";
				}
			else
			{
				if (tooLarge)
					file << comp->getLabel() << "||";
				else {
					row += comp->getLabel();
					row += "  ";
				}
			}
		}

	}
	if (tooLarge)
		file << endl;
	else {
		table[RowNum++] = row;
		row = "";
	}

	//genrating all Compination of the inputs
	vector<vector<int>>AllCompination(1 << NumOfInputs);
	for (int i = 0; i < AllCompination.size(); i++)
	{
		AllCompination[i].resize(NumOfInputs);
		int cur = i;
		for (int j = NumOfInputs - 1; j >= 0; j--)
		{
			AllCompination[i][j] = cur % 2;
			cur /= 2;
		}
	}

	//iterate All compination of inputs and Excute this inputs 
	//printing all compination and its outputs
	for (size_t i = 0; i < AllCompination.size(); i++)
	{
		int k = 0;
		for (size_t j = 0; j < pManager->allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<SWITCH*>(comp)))
			{
				if (comp->getDelete())
					continue;
				comp->getOutputPin()->setStatus((STATUS)AllCompination[i][k]);
				comp->Draw(pOut, (STATUS)AllCompination[i][k]);
				k++;
			}
		}
		Action* pAct = new Simulate(pManager, false);
		pAct->Execute();
		delete pAct;
		k = 0;
		NumOfInputs = 0;
		NumOfOutputs = 0;
		//printing current compination input
		for (size_t j = 0; j < pManager->allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<SWITCH*>(comp)))
			{
				if (comp->getDelete())
					continue;
				NumOfInputs++;
				if (comp->getLabel() == "")
				{
					for (int K = 0; K < (6 + to_string(NumOfInputs).size()) / 2; K++)
					{
						if (tooLarge)
							file << " ";
						else
							row += " ";
					}
					if (tooLarge)
						file << AllCompination[i][k] << "  ";
					else {
						row += to_string(AllCompination[i][k]);
						row += "  ";
					}
					int siz = ((6 + to_string(NumOfInputs).size()) % 2 != 0) ? ((6 + to_string(NumOfInputs).size()) / 2) : ((6 + to_string(NumOfInputs).size()) / 2 - 1);
					for (int K = 0; K < siz; K++)
					{
						if (tooLarge)
						{
							if (abs(K - siz) <= 2)
								file << "|";
							else
								file << " ";
						}
						else
							row += " ";
					}
				}
				else
				{
					for (int K = 0; K < comp->getLabel().size() / 2; K++)
					{
						if (tooLarge)
							file << " ";
						else
							row += " ";
					}

					if (tooLarge)
						file << AllCompination[i][k] << ((comp->getLabel().size() == 4) ? " " : "  ");
					else {
						row += to_string(AllCompination[i][k]);
						row += ((comp->getLabel().size() == 4) ? " " : "  ");
					}
					int siz = (comp->getLabel().size() % 2 != 0) ? (comp->getLabel().size() / 2) : (comp->getLabel().size() / 2 - 1);
					if (comp->getLabel().size() == 4)
					{
						if (tooLarge)
							file << "||";
						else
							row += "  ";
					}
					else
					{
						for (int K = 0; K < siz; K++)
						{
							if (tooLarge)
							{
								if (abs(K - siz) <= 2)
									file << "|";
								else
									file << " ";
							}
							else
								row += " ";
						}
					}
				}
				k++;
			}
		}
		//printing outputs of the current compination
		for (size_t j = 0; j < pManager->allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<LED*>(comp)))
			{
				if (comp->getDelete())
					continue;
				NumOfOutputs++;
				if (comp->getLabel() == "")
				{
					for (int K = 0; K < (7 + to_string(NumOfOutputs).size()) / 2; K++)
					{
						if (tooLarge)
							file << " ";
						else
							row += " ";
					}
					if (tooLarge)
						file << comp->GetInputPinStatus(0) << "  ";
					else {
						row += to_string(comp->GetInputPinStatus(0));
						row += "  ";
					}
					int siz = ((7 + to_string(NumOfOutputs).size()) % 2 != 0) ? ((7 + to_string(NumOfOutputs).size()) / 2) : ((7 + to_string(NumOfOutputs).size()) / 2 - 1);
					for (int K = 0; K < siz; K++)
					{
						if (tooLarge)
						{
							if (abs(K - siz) <= 2)
								file << "|";
							else
								file << " ";
						}
						else
							row += " ";
					}
				}
				else
				{
					for (int K = 0; K < comp->getLabel().size() / 2; K++)
					{
						if (tooLarge)
							file << " ";
						else
							row += " ";
					}
					if (tooLarge)
						file << comp->GetInputPinStatus(0) << ((comp->getLabel().size() == 4) ? " " : "  ");
					else {
						row += to_string(comp->GetInputPinStatus(0));
						row += ((comp->getLabel().size() == 4) ? " " : "  ");
					}
					int siz = (comp->getLabel().size() % 2 != 0) ? (comp->getLabel().size() / 2) : (comp->getLabel().size() / 2 - 1);
					if (comp->getLabel().size() == 4)
					{
						if (tooLarge)
							file << "||";
						else
							row += "  ";
					}
					else
					{
						for (int K = 0; K < siz; K++)
						{
							if (tooLarge)
								if (abs(K - siz) <= 2)
									file << "|";
								else
									file << " ";
							else
								row += " ";
						}
					}
				}
			}
		}
		if (tooLarge)
			file << endl;
		else
		{
			table[RowNum++] = row;
			row = "";
		}
	}
	if (tooLarge)
		pOut->PrintStatusBox("The truth table has been created sucessfully");
	if (tooLarge)
		file.close();
	if (tooLarge)
		pOut->PrintTruthTable();
	else {
		int X, Y, w, h;
		//The before saved Image
		image *img = pOut->DrawTruthTable(table, NumOfInputs, NumOfOutputs, X, Y, w, h);
		Input *pIn = pManager->GetInput(); //Pointer to Input Class
		int x, y;
		bool terminate = false;
		pOut->PrintStatusBox("to exit click at any point out of the rectangle");
		//New member function to wait for Selection Point.
		pIn->WaitSelectionPoint(x, y);
		//Check if the new Point is in the Menu Bar or Not
		while (!terminate) {
			if (x > X && x < w && y > Y && y < h) {
				//The user still in the truth table rectangle
			}
			else {
				//The user want to dismiss the truth table
				terminate = true;
				//So delete the image & restore previous state
				pOut->drawAfterTruthTable(img, X, Y, w, h);
			}
		}
	}
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
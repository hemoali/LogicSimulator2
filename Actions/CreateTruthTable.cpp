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

bool CreateTruthTable::ReadActionParameters(image *, Component* c)
{
	return false;
}

void CreateTruthTable::Execute()
{
	Output* pOut = pManager->GetOutput();
	bool tooLarge = false;
	int NumOfInputs = 0, NumOfOutputs = 0;
	//Calculating the Number of Inputs (the number of Switches)
	for (size_t i = 0; i < Utils::allComponentsCorners.size(); i++)
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
	string *table = new string[(1 << NumOfInputs) + 1];
	vector<vector<string> > table2((1 << NumOfInputs) + 1);
	int RowNum = 0;
	NumOfInputs = NumOfOutputs = 0;
	ofstream file;
	if (tooLarge)
		file.open("TruthTable.txt");
	for (size_t i = 0; i < Utils::allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<SWITCH*>(comp)))
		{
			if (comp->getDelete())
				continue;
			NumOfInputs++;
			if (comp->getLabel() == "")
			{
				if (tooLarge)
				{
					file << "Input " << NumOfInputs << "||";
				}
				else
				{
					row += "Input ";
					row += to_string(NumOfInputs);
					row += "  ";
					table2[RowNum].push_back("Input " + to_string(NumOfInputs));
				}
			}
			else
			{
				if (tooLarge)
				{
					file << comp->getLabel() << "||";
				}
				else
				{
					row += comp->getLabel();
					row += "  ";
					table2[RowNum].push_back(comp->getLabel());

				}
			}
		}
	}
	//printing Lables  of all LEDs
	for (size_t i = 0; i < Utils::allComponentsCorners.size(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<LED*>(comp)))
		{
			if (comp->getDelete())
				continue;
			NumOfOutputs++;
			if (comp->getLabel() == "")
			{
				if (tooLarge)
				{
					file << "Output " << NumOfOutputs << "||";
				}
				else
				{
					row += "Output ";
					row += to_string(NumOfOutputs);
					row += "  ";
					table2[RowNum].push_back("Output " + to_string(NumOfOutputs));
				}
			}
			else
			{
				if (tooLarge)
				{
					file << comp->getLabel() << "||";
				}
				else
				{
					row += comp->getLabel();
					row += "  ";
					table2[RowNum].push_back(comp->getLabel());
				}
			}
		}

	}
	if (tooLarge)
	{
		file << endl;
	}
	else
	{
		table[RowNum++] = row;
		row = "";
	}

	//genrating all Combinations of the inputs
	vector<vector<int>>AllCombinations(1 << NumOfInputs);
	for (int i = 0; i < AllCombinations.size(); i++)
	{
		AllCombinations[i].resize(NumOfInputs);
		int cur = i;
		for (int j = NumOfInputs - 1; j >= 0; j--)
		{
			AllCombinations[i][j] = cur % 2;
			cur /= 2;
		}
	}

	//iterate All combinations of inputs and Excute these inputs 
	//printing all combinations and their outputs
	for (size_t i = 0; i < AllCombinations.size(); i++)
	{
		int k = 0;
		for (size_t j = 0; j < Utils::allComponentsCorners.size(); j++)
		{
			Component* comp = pManager->getComponent(j);
			if ((dynamic_cast<SWITCH*>(comp)))
			{
				if (comp->getDelete())
					continue;
				comp->getOutputPin()->setStatus((STATUS)AllCombinations[i][k]);
				comp->Draw(pOut, (STATUS)AllCombinations[i][k]);
				k++;
			}
		}
		Simulate simAction(pManager, false);
		simAction.Execute();
		k = 0;
		NumOfInputs = 0;
		NumOfOutputs = 0;
		//printing current combination input
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
					for (int K = 0; K < (6 + to_string(NumOfInputs).size()) / 2; K++)
					{
						if (tooLarge)
							file << " ";
						else
							row += " ";
					}
					if (tooLarge)
						file << AllCombinations[i][k] << "  ";
					else {
						row += to_string(AllCombinations[i][k]);
						row += "  ";
						table2[RowNum].push_back(to_string(AllCombinations[i][k]));
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
						file << AllCombinations[i][k] << ((comp->getLabel().size() == 4) ? " " : "  ");
					else {
						row += to_string(AllCombinations[i][k]);
						row += ((comp->getLabel().size() == 4) ? " " : "  ");
						table2[RowNum].push_back(to_string(AllCombinations[i][k]));
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
		//printing outputs of the current combination
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
						table2[RowNum].push_back(to_string(comp->GetInputPinStatus(0)));
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
						table2[RowNum].push_back(to_string(comp->GetInputPinStatus(0)));
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
		image *img = pOut->DrawTruthTable(table2, NumOfInputs, NumOfOutputs, X, Y, w, h);
		if (img != NULL) {
			Input *pIn = pManager->GetInput(); //Pointer to Input Class
			int x, y;
			bool terminate = false;
			pOut->PrintStatusBox("To exit, click at any point out of the rectangle");
			//New member function to wait for Selection Point.
			pIn->WaitSelectionPoint(x, y);
			//Check if the new Point is in the Menu Bar or Not
			while (!terminate) {
				if (x > X && x < w && y > Y && y < h) {
					//The user still in the truth table rectangle
					pIn->WaitSelectionPoint(x, y);
				}
				else {
					//The user want to dismiss the truth table
					terminate = true;
					//So delete the image & restore previous state
					pOut->drawAfterTruthTable(img, X, Y, w, h);
				}
			}
		}
		else {
			int res = pOut->printPopUpMessage("", 'T');
			if (res == 1) {
				file.open("TruthTable.txt");
				for (int i = 0; i < (1 << NumOfInputs) + 1; i++) {
					file << table[i] << endl;
				}
				file.close();
				pOut->PrintTruthTable();
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

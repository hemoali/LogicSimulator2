#include "Load.h"
#include<string>
#include"..\Components\Component.h"
#include"..\Components\AND2.h"
#include"..\Components\AND3.h"
#include"..\Components\BUFFER.h"
#include"..\Components\LED.h"
#include"..\Components\NAND2.h"
#include"..\Components\NOR2.h"
#include"..\Components\NOR3.h"
#include"..\Components\NOT.h"
#include"..\Components\OR2.h"
#include"..\Components\SWITCH.h"
#include"..\Components\XNOR2.h"
#include"..\Components\XOR2.h"
#include"..\Components\XOR3.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"

Load::Load(ApplicationManager*pApp) : Action(pApp)
{

}
bool Load::ReadActionParameters(image *I)
{
	return false;
}
void Load::Execute()
{
	file.open("save.txt");
	int compCount, ID;
	string compName, compLabel;
	GraphicsInfo point;
	file >> compCount;
	for (int i = 0; i < compCount; i++)
	{
		file >> compName >> ID >> /*compLabel >>*/ point.x1 >> point.y1;
		if (compName == "AND2")
		{
			AND2*ptr = new AND2(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "AND3")
		{
			AND3*ptr = new AND3(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "BUFFER")
		{
			BUFFER*ptr = new BUFFER(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "LED")
		{
			LED*ptr = new LED(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "NAND2")
		{
			NAND2*ptr = new NAND2(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "NOR2")
		{
			NOR2*ptr = new NOR2(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "NOR3")
		{
			NOR3*ptr = new NOR3(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "NOT")
		{
			NOT*ptr = new NOT(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "OR2")
		{
			OR2*ptr = new OR2(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "SWITCH")
		{
			SWITCH*ptr = new SWITCH(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "XNOR2")
		{
			XNOR2*ptr = new XNOR2(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "XOR2")
		{
			XOR2*ptr = new XOR2(point, 5);
			ptr->load(pManager);
		}
		else if (compName == "XOR3")
		{
			XOR3*ptr = new XOR3(point, 5);
			ptr->load(pManager);
		}
	}
	file.close();
}
void Load::Undo()
{

}
void Load::Redo()
{

}
Load::~Load()
{
}

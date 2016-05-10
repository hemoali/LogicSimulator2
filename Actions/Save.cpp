#include "Save.h"
#include"..\Components\Component.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"

Save::Save(ApplicationManager*pApp) : Action(pApp)
{

}
bool Save::ReadActionParameters(image *I)
{
	return false;
}
void Save::Execute()
{
	vector<pair <int, int >> thePoints;
	Output *pOut = pManager->GetOutput();
	file.open("save");
	int compCount = 0;
	int ConnectionCount = 0;
	for (int i = 0; i < pManager->getCompCount(); i++)
	{
		if (pManager->getComponent(i)->getDelete())
		{
			continue;
		}
		if (dynamic_cast<Connection*>(pManager->getComponent(i)))
			ConnectionCount++;
		else compCount++;
	}
	file << compCount << "\n";
	for (int i = 0; i < pManager->getCompCount(); i++)
	{
		if (pManager->getComponent(i)->getDelete())
		{
			continue;
		}
		if (dynamic_cast<Connection*>(pManager->getComponent(i)))
			continue;
		pManager->getComponent(i)->save(i + 1, file);
	}
	file << "Connections\n";
	file << ConnectionCount << '\n';
	for (int i = 0; i < pManager->getCompCount(); i++)
	{
		if (pManager->getComponent(i)->getDelete())
		{
			continue;
		}
		if (dynamic_cast<Connection*>(pManager->getComponent(i)))
		{
			pManager->getComponent(i)->save(i + 1, file);
		}
	}
	file.close();
	pManager->GetOutput()->PrintStatusBox("Design saved successfully");
}
void Save::Undo()
{

}
void Save::Redo()
{

}
Save::~Save()
{
}

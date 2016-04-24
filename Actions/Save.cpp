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
	file.open("save.txt");
	int compCount = 0;
	for (int i = 0; i < pManager->getCompCount(); i++)
	{
		if (dynamic_cast<Connection*>(pManager->getComponent(i)))
			continue;
		compCount++;
	}
	file << compCount << "\n";
	for (int i = 0; i < pManager->getCompCount(); i++)
	{
		if (dynamic_cast<Connection*>(pManager->getComponent(i)))
			continue;
		pManager->getComponent(i)->save(i + 1, file);
	}
	file << "Connections\n";
	for (int i = 0; i < pManager->getCompCount(); i++)
	{
		if (dynamic_cast<Connection*>(pManager->getComponent(i)))
		{
			pManager->getComponent(i)->save(i + 1, file);
		}
	}
	file << "-1";
	file.close();
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

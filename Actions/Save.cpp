#include "Save.h"
#include"..\Components\Component.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"

Save::Save(ApplicationManager*pApp) : Action(pApp)
{
	path = "";
	ok = false;
}
bool Save::ReadActionParameters(image * , Component* c)
{
	Output *pOut = pManager->GetOutput();
	Input *pIn = pManager->GetInput();
	if (pOut->saveFile()) {
		path = pIn->getSavePath();
		return true;
	}

	return false;
}
void Save::Execute()
{
	if (ReadActionParameters(NULL, NULL)) {
		ok = true;
		vector<pair <int, int >> thePoints;
		Output *pOut = pManager->GetOutput();
		file.open(path);
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
			pManager->componentSaving(i, file);
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
				pManager->componentSaving(i, file);
			}
		}
		file << -1 << endl;
		file.close();
		pManager->GetOutput()->PrintStatusBox("Design saved successfully");
	}
	else
		ok = false;
}
void Save::Undo()
{

}
void Save::Redo()
{

}
bool Save::isSuccessful()
{
	return ok;
}
Save::~Save()
{
}

#include "Save.h"
#include"..\Components\Component.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"

Save::Save(ApplicationManager*pApp) : Action(pApp)
{
	path = "";
	ok = false;
}
bool Save::ReadActionParameters(image *, Component* c)
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
		pManager->SaveComponents(file);
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

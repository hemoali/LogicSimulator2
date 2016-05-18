#include "Exit.h"





Exit::Exit(ApplicationManager * pApp):Action(pApp)
{
	choice = -1;
}

Exit::~Exit()
{
}

bool Exit::ReadActionParameters(image *)
{

	choice = pManager->GetOutput()->printPopUpMessage("", 'Q');
	return true;
}

void Exit::Execute()
{
	// Exit action here
	if (ReadActionParameters()) {
		if (choice == IDNO) {
			pManager->setExitChoice(1);
			Clear *pAct = new Clear(pManager);
			pAct->setLoading(true);
			pAct->Execute();
		}
		else if (choice == IDYES)
		{
			Action *pAct = new Save(pManager);
			pAct->Execute();
			if (((Save*)pAct)->isSuccessful()) {
				pManager->setExitChoice(1);
				Clear *pAct = new Clear(pManager);
				pAct->setLoading(true);
				pAct->Execute();
			}
			else
				pManager->setExitChoice(0);
			delete pAct;
			pAct = NULL;
		}
		else
			pManager->setExitChoice(0);

	}
}

void Exit::Undo()
{
}

void Exit::Redo()
{
}

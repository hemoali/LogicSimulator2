#ifndef MOVE_Header
#define MOVE_Header
#include "Action.h"
class Move :
	public Action
{
	GraphicsInfo oldGraphicsInfo, newCoor;
	int compIdx;
	image* oldSmallCleanImage;
	image* newSmallImageForGate;

public:
	Move(ApplicationManager *pApp);
	virtual ~Move();

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

#endif
#ifndef MULTIMOVE_Header
#define MULTIMOVE_Header
#include "Action.h"
class MultiMove :
	public Action
{
	vector<GraphicsInfo> oldGraphicsInfo, newCoor; // Centers
	vector<int> compIdx;
	vector<image*> oldSmallCleanImage, newSmallImageForGate;
	vector<Component*> selectedComponents;
public:
	MultiMove(ApplicationManager *pApp);
	virtual ~MultiMove();

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image *);
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

#endif
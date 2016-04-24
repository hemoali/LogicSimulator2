#ifndef _SAVE_H
#define _SAVE_H


#include"Action.h"
#include<fstream>
class Save : public Action
{
protected:
	ofstream file;
public:
	Save(ApplicationManager*pApp);

	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image *);


	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();
	~Save();
};

#endif
#ifndef _LOAD_H
#define _LOAD_H

#include"Action.h"
#include<fstream>
class Load : public Action
{

protected:
	ifstream file;
public:
	Load(ApplicationManager*pApp);


	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image *);


	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();
	~Load();
};

#endif
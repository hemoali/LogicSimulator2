#ifndef _SAVE_H
#define _SAVE_H


#include"Action.h"
#include<fstream>
class Save : public Action
{
	string path;
	bool ok;
protected:
	ofstream file;
public:
	Save(ApplicationManager*pApp);

	//Reads parameters required for action to execute
	virtual bool Save::ReadActionParameters(image *, Component* c);


	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();
	bool isSuccessful();
	~Save();
};

#endif
#ifndef _NEW_H
#define _NEW_H


#include"Action.h"
#include<fstream>
class Clear : public Action
{
	bool Loading;
protected:
	ofstream file;
public:
	Clear(ApplicationManager*pApp);

	//Reads parameters required for action to execute
	virtual bool ReadActionParameters(image * = NULL);


	//Execute action (code depends on action type)
	virtual void Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();
	void setLoading(bool T);
	~Clear();
};

#endif
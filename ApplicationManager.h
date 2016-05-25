#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

#include "GUI\Output.h"
#include "GUI\Input.h"
#include <vector>
#include <stack>
#include "Utils.h"
using namespace std;
class Component;
class Connection;
class Action;
//Main class that manages everything in the application.
class ApplicationManager
{

	enum { MaxCompCount = 200 };	//Max no of Components	

private:
	int CompCount;		//Actual number of Components
	Component* CompList[MaxCompCount];	//List of all Components (Array of pointers)

	Output* OutputInterface; //pointer to the Output Clase Interface
	Input* InputInterface; //pointer to the Input Clase Interface
public:
	int Exitchoice;
	Component*PastedComponent;
	GraphicsInfo pastepoint;
	ActionType cutorcopy;

	ApplicationManager(); //constructor
	void UpdateInterface() {};
	//Reads the required action from the user and returns the corresponding action type
	ActionType GetUserAction();

	//Creates an action and executes it
	void ExecuteAction(ActionType);

	//Gets a pointer to Input / Output Object
	Output* GetOutput();
	Input* GetInput();

	//Adds a new component to the list of components
	void AddComponent(Component* pComp);

	Component* getComponent(int);
	Component* getComponentByCoordinates(int x, int y, bool ignoreConnections, bool ignoreDelete, int&i);
	int getComponentIndex(Component* c);
	//Compcount Setter And Getter
	int getCompCount();
	void setCompCount(int n);

	//Loading Function
	void componentLoading(ifstream& in, string compType, GraphicsInfo point);
	//Exitting Indicator
	void setExitChoice(int x);
	//destructor
	~ApplicationManager();
};

#endif
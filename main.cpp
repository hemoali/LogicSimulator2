#include <iostream>
#include <sstream>
#include <cmath>
#include "ApplicationManager.h"
//Hiding the Console
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main()
{
	ActionType ActType;
	//Create an object of ApplicationManager
	ApplicationManager AppManager;
	int exitChoice = 0;
	do
	{		
		//Read user action
		ActType = AppManager.GetUserAction();
		//Exexute the action
		AppManager.ExecuteAction(ActType);
		exitChoice = AppManager.Exitchoice;
	} while(exitChoice != 1);
	
	return 0;
}
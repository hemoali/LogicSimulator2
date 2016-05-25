#include <iostream>
#include <sstream>
#include <cmath>
#include "ApplicationManager.h"
//Hiding the Console
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
using namespace std;
int main()
{
	ActionType ActType;
	//Create an object of ApplicationManager
	ApplicationManager AppManager;
	do
	{		
		//Read user action
		ActType = AppManager.GetUserAction();
		//Exexute the action
		AppManager.ExecuteAction(ActType);

		//AppManager.UpdateInterface();
	} while(ActType != QUIT);
	
	return 0;
}
#pragma once

#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "UI_Info.h"

class Output;
class Input		//The application manager should have a pointer to this class
{
private:
	window *pWind;	//Pointer to the Graphics Window

public:
	Input(window*);
	void GetPointClicked(int &, int &, bool drawImage = false, bool drawConnection = false);	//Get coordinate where user clicks
	buttonstate Input::GetButtonStatus(const button btMouse, int &iX, int &iY) const;
	string GetSrting(Output*, string);		//Returns a string entered by the user
	ActionType GetUserAction(ApplicationManager* pManager) const; //Reads the user click and maps it to an action

	~Input();
};

#ifndef _INPUT_H
#define _INPUT_H

#include "UI_Info.h"
#include <vector>
#include <utility>
class ApplicationManager;
class Output;
class Component;
class Input		//The application manager should have a pointer to this class
{
private:
	window *pWind;	//Pointer to the Graphics Window
	bool isSelectMode, isSelectionContainConnections;
	vector<pair<int, Component*> > selectedComponents;
	int startXPointForConnections, startYPointForConnections;
public:
	Input(window*);
	void GetPointClicked(int &, int &, bool drawImage = false, bool drawConnection = false);	//Get coordinate where user clicks
	void getSelectionPoint(int& , int&);
	void getMouseCoordinates(int & x, int & y);
	buttonstate Input::GetButtonStatus(const button btMouse, int &iX, int &iY) const;
	ActionType GetUserAction(ApplicationManager* pManager); //Reads the user click and maps it to an action
	void WaitSelectionPoint(int &X, int &Y);
	
	void setSelectMode(bool b);
	bool getSelectMode();
	void Input::setisSelectionContainConnections(bool b);
	bool Input::getisSelectionContainConnections();

	void setSelectedComponents(vector<pair<int, Component*> >);
	vector<pair<int,Component*> >& getSelectedComponents();

	void getConnectionStartPoint(int&x, int&y) {
		x = startXPointForConnections;
		y = startYPointForConnections;
	}
	
	void CorrectPointClickedSilent(int&, int&, bool drawImage = false, bool drawConnection = false);

	void getExactConnectionLocation(int&x, int&y);

	void clearSelectedComponents();

	void switchMode(MODE appMode, ApplicationManager* pManager);
	//the new getString
	string getStringBox(string s = "");
	//returns the Save/Load File Path 
	string getSavePath();
	string getLoadPath();

	~Input();
};
#endif
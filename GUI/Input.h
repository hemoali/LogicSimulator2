#ifndef _INPUT_H
#define _INPUT_H

#include "UI_Info.h"
#include <vector>
#include <utility>
#include "..\Utils.h"
class ApplicationManager;
class Output;
class Component;
class SWITCH;
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
	buttonstate GetButtonStatus(const button btMouse, int &iX, int &iY) const;
	ActionType GetUserAction(); //Reads the user click and maps it to an action
	void WaitSelectionPoint(int &X, int &Y);
	void clearHoveringImage(image * img, int J, int K, int widthh);
	void setSelectMode(bool b);
	bool getSelectMode();
	void setisSelectionContainConnections(bool b);
	bool getisSelectionContainConnections();
	void DrawAfterMenu(image * img, int x, int y, int type, int w);
	image* printHovering(int & x, int & y, string s, int & w, Component * C);
	void setSelectedComponents(vector<pair<int, Component*> >);
	vector<pair<int,Component*> >& getSelectedComponents();
	void correctHoverStartpoint(int & x, int & y, int width);
	void getConnectionStartPoint(int&x, int&y) {
		x = startXPointForConnections;
		y = startYPointForConnections;
	}
	Component* getArrOfComponents(int y, int x) { return Utils::getArrayOfComponents(y, x); }
	image * StoreBeforeMenu(int x, int y, int type, int w);
	void CorrectPointClickedSilent(int&, int&, bool drawImage = false, bool drawConnection = false);

	void getExactConnectionLocation(int&x, int&y);

	void clearSelectedComponents();
	//the new getString
	string getStringBox(string s = "");
	//returns the Save/Load File Path 
	string getSavePath();
	string getLoadPath();
	SWITCH* toBeChangedSwitch;
	~Input();
};
#endif
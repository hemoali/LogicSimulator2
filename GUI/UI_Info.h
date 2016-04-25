#ifndef UI_INFO_H
#define UI_INFO_H

#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\Defs.h"

//User Interface information file.
//This file contains info that is needed by Input and Output classes to
//handle the user interface

__declspec(selectany) //This line to prevent "redefinition error"

struct UI_Info	//User Interface Info.
{
	MODE AppMode;		//Application Mode (design or simulation)
	
	static const int	width = 1200, height = 700,	//Window width and height
		wx = 50, wy = 15,			//Window starting coordinates
		StatusBarHeight = 50,	//Status Bar Height
		ToolBarHeight = 40,		//Tool Bar Height (distance from top of window to bottom line of toolbar)
		ToolItemWidth = 60,		//Width of each item in toolbar menu
		RightClickMenuLength = 180, //Length of Right CLl Menu
		RightClickMenuHeight = 160, //Width of Right CLl Menu
		RightClickCMenuH = 60;

	color DrawColor;		//Drawing color
	color SelectColor;		//Highlighting color for gates and connections
	color ConnColor;		//Connector color
	color HumpColor;		//Connector hump color
	color SelectedHumpColor;//Color of the hump when connector is selected
	
	color OneValueConnection;//For Simulation 1 value
	color OneValueHumpColor;//For hump wheb simulation 1 value
	
	color MsgColor;			//Messages color
	color BkGrndColor;		//Back ground color
	color ErrorColor;


	//This should be calculated accurately because it will be used later to create connections between gates
	//For now, we will assume that rect width = 50 and height = 50
	static const int
		GRID_SIZE = 16,
		GATE_Width = GRID_SIZE * 3,
		GATE_Height = GRID_SIZE * 3,
		GRID_WIDTH = width / GRID_SIZE - 1,
		GRID_HEIGHT = height / GRID_SIZE - 1;

}UI;	//create a single global object UI

#endif
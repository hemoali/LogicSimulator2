#pragma once
#include "..\Defs.h"
#include "Input.h"
#include<vector>

struct bfs_node
{
	int x;
	int y;
	bfs_node* parent;
};
class Output	//The application manager should have a pointer to this class
{
private:
	window* pWind;	//Pointer to the Graphics Window
	mutable vector < pair <int, int> >vec;
	static CellType Output::usedPixels[44][74];

public:
	Output(); // Performs the Window Initialization
	Input* CreateInput() const; //creates a pointer to the Input object
	void ChangeTitle(string Title) const;

	void CreateDesignToolBar() const;	//Tool bar of the design mode
	void CreateSimulationToolBar() const;//Tool bar of the simulation mode
	void CreateStatusBar() const;	//Create Status bar

	void ClearStatusBar() const;		//Clears the status bar
	void ClearDrawingArea() const;	//Clears the drawing area

	window* CreateWind(int wd, int h, int x, int y) const; //Creates user interface window

	bool DrawString(string s, GraphicsInfo Gfx_info) const;
	void DrawGrid() const;
	
	// Draw  
	bool SetDragImage(ActionType ActType, GraphicsInfo& GfxInfo);
	void DrawNot_Buffer(GraphicsInfo, bool = false, bool = false) const;
	void DrawAnd_Nand(GraphicsInfo, int = 2, bool = false, bool = false) const;
	void DrawOr_Nor(GraphicsInfo, int = 2, bool = false, bool = false) const;
	void DrawXor_Xnor(GraphicsInfo, int = 2, bool = false, bool = false) const;
	void DrawLed(GraphicsInfo, bool = false, bool = false) const;
	void DrawSwtich(GraphicsInfo, bool = false, bool = false) const;

	bool DrawConnection(GraphicsInfo r_GfxInfo, bool selected = false) const;
	bfs_node* bfs(bfs_node* bf, int requX, int requY, vector<bfs_node*> allNodes) const;
	
	void PrintMsg(string msg) const;	//Print a message on Status bar


	~Output();
};

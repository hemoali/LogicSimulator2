#ifndef _OUTPUT_H
#define _OUTPUT_H
#include "Input.h"
#include<vector>
class Component;
class Connection;
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
	mutable vector < pair <int, int> >allComponentsCorners;
	static CellType usedPixels[44][74];
	static Component* arrayOfComponents[44][74];
	ApplicationManager* pManager;
public:
	Output(ApplicationManager* pManager); // Performs the Window Initialization
	Input* CreateInput() const; //creates a pointer to the Input object
	void ChangeTitle(string Title) const;

	void CreateStatusBar() const;	//Create Status bar
	void CreateLeftToolBar() const;	//Tool bar of the design mode
	void CreateTopToolBar() const;	//Tool bar of the design mode

	void ClearStatusBar() const;		//Clears the status bar
	void ClearDrawingArea() const;	//Clears the drawing area

	window* CreateWind(int wd, int h, int x, int y) const; //Creates user interface window

	bool DrawString(string s, GraphicsInfo Gfx_info) const;
	void DrawGrid() const;
	
	// Draw  
	bool SetDragImage(ActionType ActType, GraphicsInfo& GfxInfo, image*, bool moving=false, Component* comp = NULL);
	bool SetMultiDragImage(int x, int y, Component* mainMovingComponent, vector< pair<int, Component*> > allSelectedComponents);
	void DrawNot_Buffer(GraphicsInfo, bool = false, bool = false, bool = false) const;
	void DrawAnd_Nand(GraphicsInfo, int = 2, bool = false, bool = false, bool = false) const;
	void DrawOr_Nor(GraphicsInfo, int = 2, bool = false, bool = false, bool = false) const;
	void DrawXor_Xnor(GraphicsInfo, int = 2, bool = false, bool = false, bool = false) const;
	void DrawLed(GraphicsInfo, bool = false, bool = false, bool = false) const;
	void DrawSwtich(GraphicsInfo, bool = false, bool = false, bool = false) const;
	void DrawCleanImage(image*, int x,int y);
	bool DrawConnection(GraphicsInfo r_GfxInfo, int inputPin, GraphicsInfo compCenterLocationbool, vector<Cell>&, bool selected = false) const;
	void DrawRClickMenu_CorrectPoints(int& x, int& y, int type, bool = true);
	image* StoreBeforeMenu(int, int);
	void storeDrawingAreaImage(image*& img); 
	void drawStoredDrawingAreaImage(image*& img);
	void drawRectangle(int x1, int y1, int x2, int y2);

	void DrawAfterMenu(image* img,int x, int y);
	bfs_node* bfs(bfs_node* bf, int requX, int requY, vector<bfs_node*> allNodes) const;
	
	void clearConnections(vector<Connection*> & allConnections, int originalX, int originalY, bool isInput, bool setDeleted = false);
	void changeConnectionColor(Connection * connection, color Color = BLUE);
	void PrintMsg(string msg, color Color = UI.MsgColor) const;	//Print a message on Status bar

	void setUsedPixel(int i, int j, CellType);
	CellType getUsedPixel(int i, int j);

	void setArrayOfComponents(int i, int j, Component*);
	Component* getArrayOfComponents(int i, int j);
	
	void clearConnectionsFromGrid(vector<Connection*> allOutputConnections, vector<Connection*> allInputConnections);

	void printMatrix(string msg);
	~Output();
};
#endif
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
class Output
{
private:
	window* pWind;	//Pointer to the Graphics Window
	mutable vector < pair <int, int> >allComponentsCorners;
	static CellType usedPixels[44][74];
	vector<Connection*>* allConnectionsPointer;
	friend void DRAWAFTERMENUE(Output *pOut, HWND D);
public:
	Output(vector<Connection*>* allConns); // Performs the Window Initialization
	Input* CreateInput() const; //creates a pointer to the Input object
	void ChangeTitle(string Title) const;

	void CreateLeftToolBar() const;	//Tool bar of the design mode
	void CreateTopToolBar() const;	//Tool bar of the design mode

	void ClearDrawingArea() const;	//Clears the drawing area

	window* CreateWind(int wd, int h, int x, int y) const; //Creates user interface window

	bool DrawString(string s, GraphicsInfo Gfx_info) const;
	void DrawGrid() const;
	// Draw  
	bool SetDragImage(ActionType ActType, GraphicsInfo& GfxInfo, image*, bool moving, Component* comp);
	bool SetMultiDragImage(int x, int y, Component* mainMovingComponent, vector< pair<int, Component*> > allSelectedComponents);
	void DrawNot_Buffer(GraphicsInfo, bool = false, bool = false, bool = false, Component* = NULL);
	void DrawAnd_Nand(GraphicsInfo, int = 2, bool = false, bool = false, bool = false, Component* = NULL);
	void DrawOr_Nor(GraphicsInfo, int = 2, bool = false, bool = false, bool = false, Component* = NULL);
	void DrawXor_Xnor(GraphicsInfo, int = 2, bool = false, bool = false, bool = false, Component* = NULL);
	void DrawLed(GraphicsInfo, bool = false, bool = false, bool = false, Component* = NULL);
	void DrawSwtich(GraphicsInfo, bool = false, bool = false, bool = false, Component* = NULL);
	void DrawCleanImage(image*, int x, int y, bool emptyArray);
	bool DrawConnection(GraphicsInfo r_GfxInfo, int inputPin, GraphicsInfo compCenterLocationbool, vector<Cell>&, bool selected , Component* c);

	void fillArrays(Component* comp);

	void storeDrawingAreaImage(image*& img);
	void drawStoredDrawingAreaImage(image*& img);
	void drawRectangle(int x1, int y1, int x2, int y2);

	image* StoreBeforeMenu(int, int, int,int w = 0);
	void DrawAfterMenu(image* img, int x, int y, int type,int w = 0);
	void DrawRClickMenu_CorrectPoints(int& x, int& y, int type, bool = true);

	void switchMode(MODE appMode);
	void DrawWarningMenues(char type,int x= 0, int y=0);
	image* StoreBeforeWarning();
	void DrawAfterWarning(image* theWarningImage);
	void PrintStatusBox(string s,color = GREEN);

	int printPopUpMessage(string s, char type = 'V'); 

	image* DrawTruthTable(string table[], int inputsNum, int outputsNum, int& X, int& Y, int& w, int& h);
	void drawAfterTruthTable(image* img, int X, int Y, int w, int h);
	void PrintTruthTable();

	void storeImage(image* img, int x1, int y1, int x2, int y2);

	bfs_node* bfs(bfs_node* bf, int requX, int requY, vector<bfs_node*> allNodes) const;
	void clearConnections(vector<Connection*> & allConnections, int originalX, int originalY, bool isInput, bool setDeleted = false);
	void changeConnectionColor(Connection * connection, color Color = BLUE);

	void setUsedPixel(int i, int j, CellType);
	CellType getUsedPixel(int i, int j);

	void clearConnectionsFromGrid(vector<Connection*> allOutputConnections, vector<Connection*> allInputConnections);

	bool saveFile();
	bool loadFile();

	void printMatrix(string msg);
	void printCheck();
	//Resetting Interface
	void resetInterfaceArrays();

	void Output::storeimagefordeletegate(image* imageptr, GraphicsInfo Gfx)
	{
		pWind->StoreImage(imageptr, Gfx.x1 - UI.GRID_SIZE - 5, Gfx.y1 - UI.GRID_SIZE - 5, 2 * UI.GRID_SIZE + 4, UI.GATE_Height + 3);
	}

	~Output();
};
#endif
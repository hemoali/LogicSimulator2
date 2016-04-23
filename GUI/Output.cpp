#include "Output.h"
#include "..\Utils.h"
#include <vector>
#include <queue>
#include <cstring>
#include <iostream>
#include "..\Components\Gate.h"
using namespace std;
CellType Output::usedPixels[44][74];
int arrayOfIntersections[44][74];
int arrayOfCorners[44][74];
int connectionsCountAtPixel[44][74];

Output::Output(ApplicationManager* pManager)
{
	//Initialize user interface parameters

	UI.AppMode = DESIGN;	//Design Mode is the startup mode

							//Initilaize interface colors
	UI.DrawColor = BLACK;
	UI.SelectColor = BLUE;
	UI.ConnColor = RED;
	UI.MsgColor = BLUE;
	UI.BkGrndColor = WHITE;

	//Create the drawing window
	pWind = CreateWind(UI.width, UI.height, UI.wx, UI.wy);
	ChangeTitle("Programming Techniques Project");

	DrawGrid();
	CreateDesignToolBar();	//Create the desgin toolbar
	CreateStatusBar();		//Create Status bar

	memset(usedPixels, 0, sizeof usedPixels);
	memset(arrayOfIntersections, -1, sizeof arrayOfIntersections);
	memset(arrayOfCorners, 0, sizeof arrayOfCorners);
	memset(connectionsCountAtPixel, 0, sizeof connectionsCountAtPixel);
	memset(allPixels, NULL, sizeof allPixels);

	this->pManager = pManager;
}

Input* Output::CreateInput() const
{
	//Create an Input Object & assign it to the Same Window
	Input* pIn = new Input(pWind);
	return pIn;
}


//======================================================================================//
//								Interface Functions										//
//======================================================================================//

window* Output::CreateWind(int wd, int h, int x, int y) const
{
	return new window(wd, h, x, y);
}
//////////////////////////////////////////////////////////////////////////////////
void Output::ChangeTitle(string Title) const
{
	pWind->ChangeTitle(Title);
}
//////////////////////////////////////////////////////////////////////////////////
void Output::CreateStatusBar() const
{

	pWind->SetPen(RED, 3);
	pWind->DrawLine(0, UI.height - UI.StatusBarHeight, UI.width, UI.height - UI.StatusBarHeight);
	pWind->SetPen(WHITE, 3);
	pWind->DrawRectangle(0, UI.height - UI.StatusBarHeight + 2, UI.width, UI.height);

}
//////////////////////////////////////////////////////////////////////////////////
void Output::PrintMsg(string msg) const
{
	ClearStatusBar();	//Clear Status bar to print message on it
						// Set the Message offset from the Status Bar
	int MsgX = 25;
	int MsgY = UI.StatusBarHeight - 10;

	// Print the Message
	pWind->SetFont(20, BOLD | ITALICIZED, BY_NAME, "Arial");
	pWind->SetPen(UI.MsgColor);
	pWind->DrawString(MsgX, UI.height - MsgY, msg);
}
//////////////////////////////////////////////////////////////////////////////////
void Output::ClearStatusBar()const
{
	// Set the Message offset from the Status Bar
	int MsgX = 25;
	int MsgY = UI.StatusBarHeight - 10;

	//Overwrite using bachground color to erase the message
	pWind->SetPen(UI.BkGrndColor);
	pWind->SetBrush(UI.BkGrndColor);
	pWind->DrawRectangle(MsgX, UI.height - MsgY, UI.width, UI.height);
}
void Output::DrawGrid()const {
	pWind->SetPen(BLACK, 1);
	for (size_t i = 0; i < UI.height; i += UI.GRID_SIZE) // Hori
	{
		for (size_t j = 0; j < UI.width; j += UI.GRID_SIZE) // Vert
		{
			pWind->DrawPixel(j, i);
		}
	}

}
//////////////////////////////////////////////////////////////////////////////////////////
//Clears the drawing (degin) area
void Output::ClearDrawingArea() const
{
	pWind->SetPen(RED, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, 0, UI.width, UI.height);
	DrawGrid();
	CreateDesignToolBar();	//Create the desgin toolbar
	CreateStatusBar();

}
//////////////////////////////////////////////////////////////////////////////////////////
//Draws the menu (toolbar) in the Design mode
void Output::CreateDesignToolBar() const
{
	pWind->DrawRectangle(0, 0, UI.width, UI.ToolBarHeight + 10);
	UI.AppMode = DESIGN;	//Design Mode

							//You can draw the tool bar icons in any way you want.

							//First prepare List of images for each menu item
	string MenuItemImages[DITEMSCOUNT];
	MenuItemImages[D2AND] = "images\\Menu\\AND2.jpg";
	MenuItemImages[D3AND] = "images\\Menu\\AND3.jpg";
	MenuItemImages[D2OR] = "images\\Menu\\OR2.jpg";
	MenuItemImages[D2XOR] = "images\\Menu\\XOR2.jpg";
	MenuItemImages[D3XOR] = "images\\Menu\\XOR3.jpg";
	MenuItemImages[DBUFFER] = "images\\Menu\\BUFFER.jpg";
	MenuItemImages[DNOT] = "images\\Menu\\NOT.jpg";
	MenuItemImages[D2NAND] = "images\\Menu\\NAND2.jpg";
	MenuItemImages[D2NOR] = "images\\Menu\\NOR2.jpg";
	MenuItemImages[D3NOR] = "images\\Menu\\NOR3.jpg";
	MenuItemImages[D2XNOR] = "images\\Menu\\XNOR2.jpg";
	MenuItemImages[DLED] = "images\\Menu\\LED_ON.jpg";
	MenuItemImages[DSWITCH] = "images\\Menu\\SWITCH_ON_TOOLBAR.jpg";
	MenuItemImages[DCONNECTION] = "images\\Menu\\CONNECTION.jpg";

	MenuItemImages[DSIMULATION] = "images\\Menu\\SIMULATION.jpg";
	MenuItemImages[DSAVE] = "images\\Menu\\SAVE.jpg";
	MenuItemImages[DLOAD] = "images\\Menu\\LOAD.jpg";
	MenuItemImages[DEXIT] = "images\\Menu\\EXIT.jpg";

	//TODO: Prepare image for each menu item and add it to the list

	//Draw menu item one image at a time
	for (int i = 0; i < DITEMSCOUNT; i++)
	{
		pWind->DrawImage(MenuItemImages[i], i*UI.ToolItemWidth + ((i == 0) ? 5 : (i + 1) * 5), 5, UI.ToolItemWidth, UI.ToolBarHeight);
	}

	//Draw a line under the toolbar
	pWind->SetPen(RED, 3);
	pWind->DrawLine(0, UI.ToolBarHeight + 10, UI.width, UI.ToolBarHeight + 10);

}
//////////////////////////////////////////////////////////////////////////////////////////
//Draws the menu (toolbar) in the simulation mode
void Output::CreateSimulationToolBar() const
{
	//pWind->DrawRectangle(0, UI.ToolBarHeight, UI.width, UI.height - UI.StatusBarHeight);
	pWind->DrawRectangle(0, 0, UI.width, UI.ToolBarHeight + 10);
	UI.AppMode = SIMULATION;	//Simulation Mode
	string MenuItemImages[SITEMSCOUNT];
	/*
	SVALIDATE,	//Simulate menu item
	SSIMULATE,
	STT,	//Truth table menu item
	SDESIGN,
	SSAVE,
	SLOAD,
	SEXIT,
	SITEMSCOUNT*/
	MenuItemImages[SVALIDATE] = "images\\Menu\\VALIDATE.jpg";
	MenuItemImages[SSIMULATE] = "images\\Menu\\SIMULATE.jpg";
	MenuItemImages[STT] = "images\\Menu\\TT.jpg";
	MenuItemImages[SDESIGN] = "images\\Menu\\DESIGN.jpg";
	MenuItemImages[SSAVE] = "images\\Menu\\SAVE.jpg";
	MenuItemImages[SLOAD] = "images\\Menu\\LOAD.jpg";
	MenuItemImages[SEXIT] = "images\\Menu\\EXIT.jpg";
	//TODO: Write code to draw the simualtion toolbar (similar to that of design toolbar drawing)

	for (int i = 0; i < SITEMSCOUNT; i++)
		pWind->DrawImage(MenuItemImages[i], i*UI.ToolItemWidth + ((i == 0) ? 5 : (i + 1) * 5), 6, UI.ToolItemWidth, UI.ToolBarHeight);

	//Draw a line under the toolbar
	pWind->SetPen(RED, 3);
	pWind->DrawLine(0, UI.ToolBarHeight + 10, UI.width, UI.ToolBarHeight + 10);

}

bool Output::DrawString(string s, GraphicsInfo Gfx_info) const {
	pWind->DrawString(Gfx_info.x1, Gfx_info.y1, s);
	return true;
}

//======================================================================================//
//								Components Drawing Functions							//
//======================================================================================//


bfs_node* Output::bfs(bfs_node* bf, int requX, int requY, vector<bfs_node*> allNodes) const {
	if (bf == NULL)
	{
		return NULL;
	}
	int vis[46][74];
	memset(vis, -1, sizeof vis);
	queue<bfs_node*> q;
	bfs_node* tmp;
	vis[bf->y][bf->x] = 0;
	bf->parent = NULL;
	q.push(bf);
	while (!q.empty())
	{
		tmp = q.front();
		q.pop();
		if (tmp->x == requX && tmp->y == requY) {
			return tmp;
		}
		if (Utils::CheckPointForConnections((tmp->x + 1)* UI.GRID_SIZE, tmp->y * UI.GRID_SIZE, usedPixels) && tmp->x + 1 <= 74 && vis[tmp->y][tmp->x + 1] < 0 && Output::usedPixels[tmp->y][tmp->x + 1] != INTERSECTION && (Output::usedPixels[tmp->y][tmp->x + 1] == EMPTY || ((Output::usedPixels[tmp->y][tmp->x] == EMPTY || Output::usedPixels[tmp->y][tmp->x] == PIN || Output::usedPixels[tmp->y][tmp->x] == VERTICAL) && tmp->y == requY && tmp->x + 1 == requX && Output::usedPixels[tmp->y][tmp->x + 1] == PIN) || (Output::usedPixels[tmp->y][tmp->x + 1] == VERTICAL && Output::usedPixels[tmp->y][tmp->x + 1] != END_CONNECTION)))
		{
			vis[tmp->y][tmp->x + 1] = vis[tmp->y][tmp->x] + 1;
			bfs_node* newNode = new bfs_node;
			newNode->x = tmp->x + 1;
			newNode->y = tmp->y;
			newNode->parent = tmp;
			q.push(newNode);
			allNodes.push_back(newNode);
			if (Output::usedPixels[tmp->y][tmp->x + 1] == VERTICAL)
			{
				arrayOfIntersections[tmp->y][tmp->x + 1] = 0;
			}
		}
		if (Utils::CheckPointForConnections((tmp->x)* UI.GRID_SIZE, (tmp->y + 1) * UI.GRID_SIZE, usedPixels) && tmp->y + 1 <= 44 && vis[tmp->y + 1][tmp->x] < 0 && Output::usedPixels[tmp->y + 1][tmp->x] != INTERSECTION && (Output::usedPixels[tmp->y + 1][tmp->x] == EMPTY || ((Output::usedPixels[tmp->y][tmp->x] == EMPTY || Output::usedPixels[tmp->y][tmp->x] == HORIZONTAL) && tmp->y + 1 == requY && tmp->x == requX &&  Output::usedPixels[tmp->y + 1][tmp->x] == PIN) || (abs((tmp->y + 1) - bf->y) == 1 && tmp->x == bf->x &&  Output::usedPixels[tmp->y + 1][tmp->x] == PIN) || (Output::usedPixels[tmp->y + 1][tmp->x] == HORIZONTAL && Output::usedPixels[tmp->y + 1][tmp->x] != END_CONNECTION)))
		{

			vis[tmp->y + 1][tmp->x] = vis[tmp->y][tmp->x] + 1;
			bfs_node* newNode = new bfs_node;
			newNode->x = tmp->x;
			newNode->y = tmp->y + 1;
			newNode->parent = tmp;
			q.push(newNode);
			allNodes.push_back(newNode);
			if (Output::usedPixels[tmp->y + 1][tmp->x] == HORIZONTAL)
			{
				arrayOfIntersections[tmp->y + 1][tmp->x] = 1;
			}
		}
		if (Utils::CheckPointForConnections((tmp->x - 1)* UI.GRID_SIZE, tmp->y * UI.GRID_SIZE, usedPixels) && tmp->x - 1 >= 0 && vis[tmp->y][tmp->x - 1] < 0 && Output::usedPixels[tmp->y][tmp->x - 1] != INTERSECTION && (Output::usedPixels[tmp->y][tmp->x - 1] == EMPTY || (Output::usedPixels[tmp->y][tmp->x - 1] == VERTICAL && Output::usedPixels[tmp->y][tmp->x - 1] != END_CONNECTION)))
		{
			vis[tmp->y][tmp->x - 1] = vis[tmp->y][tmp->x] + 1;
			bfs_node* newNode = new bfs_node;
			newNode->x = tmp->x - 1;
			newNode->y = tmp->y;
			newNode->parent = tmp;
			q.push(newNode);
			allNodes.push_back(newNode);
			if (Output::usedPixels[tmp->y][tmp->x - 1] == VERTICAL)
			{
				arrayOfIntersections[tmp->y][tmp->x - 1] = 0;
			}
		}

		if (Utils::CheckPointForConnections((tmp->x)* UI.GRID_SIZE, (tmp->y - 1) * UI.GRID_SIZE, usedPixels) && tmp->y - 1 >= 0 && vis[tmp->y - 1][tmp->x] < 0 && Output::usedPixels[tmp->y - 1][tmp->x] != INTERSECTION && (Output::usedPixels[tmp->y - 1][tmp->x] == EMPTY || ((Output::usedPixels[tmp->y][tmp->x] == EMPTY || Output::usedPixels[tmp->y][tmp->x] == HORIZONTAL) && tmp->y - 1 == requY && tmp->x == requX &&  Output::usedPixels[tmp->y - 1][tmp->x] == PIN) || (abs((tmp->y - 1) - bf->y) == 1 && tmp->x == bf->x &&  Output::usedPixels[tmp->y - 1][tmp->x] == PIN) || (Output::usedPixels[tmp->y - 1][tmp->x] == HORIZONTAL && Output::usedPixels[tmp->y - 1][tmp->x] != END_CONNECTION)))
		{
			vis[tmp->y - 1][tmp->x] = vis[tmp->y][tmp->x] + 1;
			bfs_node* newNode = new bfs_node;
			newNode->x = tmp->x;
			newNode->y = tmp->y - 1;
			newNode->parent = tmp;
			q.push(newNode);
			allNodes.push_back(newNode);
			if (Output::usedPixels[tmp->y - 1][tmp->x] == HORIZONTAL)
			{
				arrayOfIntersections[tmp->y - 1][tmp->x] = 1;
			}
		}
	}
	return NULL;
}
bool Output::DrawConnection(GraphicsInfo GfxInfo, int inputPin, GraphicsInfo compCenterLocation, vector<Cell>& cellsBeforeAddingConnection, bool selected) const
{
	vector<bfs_node*> allNodes;
	bfs_node* current = new bfs_node;

	current->x = (GfxInfo.x1 - (GfxInfo.x1 % UI.GRID_SIZE)) / UI.GRID_SIZE;
	current->y = GfxInfo.y1 / UI.GRID_SIZE;

	int remindX2 = GfxInfo.x2 % UI.GRID_SIZE;

	GfxInfo.x2 = compCenterLocation.x1 - UI.GATE_Width / 2 - 2;

	int destX = GfxInfo.x2 / UI.GRID_SIZE + ((remindX2 > UI.GRID_SIZE / 2) ? 1 : 0);
	int destY;

	if (inputPin == 0)
	{
		destY = compCenterLocation.y1 / UI.GRID_SIZE - 1;
	}
	else if (inputPin == 1) {
		destY = compCenterLocation.y1 / UI.GRID_SIZE;
	}
	else {
		destY = compCenterLocation.y1 / UI.GRID_SIZE + 1;
	}
	if (current == NULL)
	{
		pWind->FlushMouseQueue();
		return false;
	}


	bfs_node* target = bfs(current, destX, destY, allNodes);

	if (target == NULL)
	{

		pWind->FlushMouseQueue();

		return false;
	}
	if (!selected)pWind->SetPen(color(23, 79, 181), 2);	else pWind->SetPen(BLUE, 2);

	if (inputPin == 0)
	{
		pWind->DrawLine(destX * UI.GRID_SIZE, destY* UI.GRID_SIZE, compCenterLocation.x1 - UI.GATE_Width / 2 + 3, compCenterLocation.y1 - 13);
	}
	else if (inputPin == 1) {
		pWind->DrawLine(destX * UI.GRID_SIZE - 1, compCenterLocation.y1, compCenterLocation.x1 - UI.GATE_Width / 2 + 3, compCenterLocation.y1);
	}
	else {
		pWind->DrawLine(destX * UI.GRID_SIZE, destY* UI.GRID_SIZE, compCenterLocation.x1 - UI.GATE_Width / 2 + 3, compCenterLocation.y1 + 13);
	}

	bfs_node* parent = target->parent;
	bool draw = true, skip_next = false, PreviousIsIntersection = false;
	int i = 0;
	while (parent != NULL) {
		// Fill cells vector
		if (i == 0)
		{
			cellsBeforeAddingConnection.push_back({ target->x , target->y, PIN });
			cellsBeforeAddingConnection.push_back({ parent->x , parent->y, usedPixels[parent->y][parent->x] });
			usedPixels[target->y][target->x] = PIN;
		}
		else {
			if (parent->parent != NULL)
			{
				cellsBeforeAddingConnection.push_back({ parent->x , parent->y, usedPixels[parent->y][parent->x] });
			}
			else {
				cellsBeforeAddingConnection.push_back({ parent->x , parent->y, PIN });
			}
		}
		connectionsCountAtPixel[target->y][target->x]++;
		if (draw && !skip_next) {
			if (target->x == parent->x && arrayOfIntersections[parent->y][parent->x] == 1)
			{
				usedPixels[parent->y][parent->x] = INTERSECTION;
				if (parent->y < target->y) {
					if (!PreviousIsIntersection)
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 10);
					else {
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 6, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 10);
					}
					pWind->SetPen(color(14, 36, 75), 2);
					pWind->DrawBezier(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 10, target->x * UI.GRID_SIZE + 6, target->y*UI.GRID_SIZE - 14, target->x * UI.GRID_SIZE + 8, target->y*UI.GRID_SIZE - 18, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 22);
					if (!selected)pWind->SetPen(color(23, 79, 181), 2);	else pWind->SetPen(BLUE, 2);
					if (parent->parent != NULL && arrayOfIntersections[parent->parent->y][parent->parent->x] != 1)
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 22, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 32);
				}
				else {
					if (!PreviousIsIntersection) {
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 10);
					}
					else {
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 6, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 10);
					}
					pWind->SetPen(color(14, 36, 75), 2);
					pWind->DrawBezier(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 10, target->x * UI.GRID_SIZE + 6, target->y*UI.GRID_SIZE + 14, target->x * UI.GRID_SIZE + 8, target->y*UI.GRID_SIZE + 18, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 22);
					if (!selected)pWind->SetPen(color(23, 79, 181), 2);	else pWind->SetPen(BLUE, 2);
					if (parent->parent != NULL && arrayOfIntersections[parent->parent->y][parent->parent->x] != 1)
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 22, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 32);
				}
				PreviousIsIntersection = true;
				if (parent->parent != NULL && arrayOfIntersections[parent->parent->y][parent->parent->x] != 1)
				{
					skip_next = true;
					PreviousIsIntersection = false;

				}
			}
			else
				if (target->y == parent->y && arrayOfIntersections[parent->y][parent->x] == 0) {
					usedPixels[parent->y][parent->x] = INTERSECTION;
					if (parent->x < target->x) {
						if (!PreviousIsIntersection)
							pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 10, target->y*UI.GRID_SIZE);
						else {
							pWind->DrawLine(target->x * UI.GRID_SIZE - 6, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 10, target->y*UI.GRID_SIZE);
						}
						pWind->SetPen(color(14, 36, 75), 2);
						pWind->DrawBezier(target->x * UI.GRID_SIZE - 10, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 14, target->y*UI.GRID_SIZE - 6, target->x * UI.GRID_SIZE - 18, target->y*UI.GRID_SIZE - 8, target->x * UI.GRID_SIZE - 22, target->y*UI.GRID_SIZE);
						if (!selected)pWind->SetPen(color(23, 79, 181), 2);	else pWind->SetPen(BLUE, 2);
						if (parent->parent != NULL && arrayOfIntersections[parent->parent->y][parent->parent->x] != 0)
							pWind->DrawLine(target->x * UI.GRID_SIZE - 22, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 32, target->y*UI.GRID_SIZE);
					}
					else {
						if (!PreviousIsIntersection)
							pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE + 10, target->y*UI.GRID_SIZE);
						else {
							pWind->DrawLine(target->x * UI.GRID_SIZE + 6, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE + 10, target->y*UI.GRID_SIZE);
						}
						pWind->SetPen(color(14, 36, 75), 2);
						pWind->DrawBezier(target->x * UI.GRID_SIZE + 10, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE + 14, target->y*UI.GRID_SIZE - 6, target->x * UI.GRID_SIZE + 18, target->y*UI.GRID_SIZE - 8, target->x * UI.GRID_SIZE + 22, target->y*UI.GRID_SIZE);
						if (!selected)pWind->SetPen(color(23, 79, 181), 2);	else pWind->SetPen(BLUE, 2);
						if (parent->parent != NULL && arrayOfIntersections[parent->parent->y][parent->parent->x] != 0)
							pWind->DrawLine(target->x * UI.GRID_SIZE + 22, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE + 32, target->y*UI.GRID_SIZE);
					}
					PreviousIsIntersection = true;

					if (parent->parent != NULL && arrayOfIntersections[parent->parent->y][parent->parent->x] != 0)
					{
						skip_next = true;
						PreviousIsIntersection = false;
					}
				}
				else {
					pWind->DrawLine(target->x*UI.GRID_SIZE, target->y*UI.GRID_SIZE, parent->x*UI.GRID_SIZE, parent->y*UI.GRID_SIZE);

					PreviousIsIntersection = false;

				}
				if (i != 0)
				{
					if (target->x == parent->x)
					{
						if (arrayOfCorners[target->y][target->x] != 1 && usedPixels[target->y][target->x] != INTERSECTION)
							usedPixels[target->y][target->x] = VERTICAL;

					}
					else if (target->y == parent->y) {
						if (arrayOfCorners[target->y][target->x] != 1 && usedPixels[target->y][target->x] != INTERSECTION)
							usedPixels[target->y][target->x] = HORIZONTAL;
					}
				}
				if (parent->parent != NULL && ((target->x == parent->x && parent->parent->x != parent->x) || (target->y == parent->y && parent->parent->y != parent->y)))
				{
					usedPixels[parent->y][parent->x] = INTERSECTION;
					arrayOfCorners[parent->y][parent->x] = 1;
				}
		}
		else {
			if (parent->parent != NULL && ((target->x == parent->x && parent->parent->x != parent->x) || (target->y == parent->y && parent->parent->y != parent->y)))
			{
				usedPixels[parent->y][parent->x] = INTERSECTION;
				arrayOfCorners[parent->y][parent->x] = 1;
			}
			draw = true;
			skip_next = false;
		}
		parent = parent->parent;
		target = target->parent;
		i++;
	}
	for (size_t i = 0; i < allNodes.size(); i++)
	{
		bfs_node* tmp = allNodes[i];
		delete tmp;
		tmp = NULL;
	}
	pWind->FlushMouseQueue();
	/* cout << endl;
		for (size_t i = 0; i < cellsBeforeAddingConnection.size(); i++)
		{
			cout << cellsBeforeAddingConnection[i].x << " " << cellsBeforeAddingConnection[i].y << "  " << cellsBeforeAddingConnection[i].cellType << endl;
		}*/
	return true;
}

void Output::DrawRClickMenu_CorrectPoints(int& x, int& y, int type,bool draw)
{
	string imageURL;
	if(draw)
	switch (type) {
	case 1: //The Gate SubMenu
		imageURL = "images\\Menu\\RightClickMenu.jpg";
		break;
	default:
		break;
	}
	//If Point (x,y) in the Right or Middle of the Drawing Area
	if (x + UI.RightClickMenuLength < UI.width ) {
		 if (y + UI.RightClickMenuWidth > UI.height - UI.StatusBarHeight) {
			 // y must be corrected
			 y =  y - UI.RightClickMenuWidth;
			 if(draw)
			 pWind->DrawImage(imageURL, x, y, UI.RightClickMenuLength, UI.RightClickMenuWidth);
		}
		 else{
			 //The Point(x,y) is ready for drawing Menu 
			 if(draw)
			 pWind->DrawImage(imageURL, x, y, UI.RightClickMenuLength, UI.RightClickMenuWidth);
		 }
	}
	//If Point (x,y) in the Left of the Drawing Area
	if (x + UI.RightClickMenuLength > UI.width ) {
		// x must be corrected
		x = UI.width - UI.RightClickMenuWidth - 40;
		if (y + UI.RightClickMenuWidth > UI.height - UI.StatusBarHeight) {
			// y must be corrected
			y = y - UI.RightClickMenuWidth;
			if(draw)
			pWind->DrawImage(imageURL, x, y, UI.RightClickMenuLength, UI.RightClickMenuWidth);
		}
		else {
			//The Point(x,y) is ready for drawing Menu 
			if(draw)
			pWind->DrawImage(imageURL, x, y, UI.RightClickMenuLength, UI.RightClickMenuWidth);
		}
	}
}

image * Output::StoreBeforeMenu(int x, int y)
{
	image * ptr = new image;
	pWind->StoreImage(ptr, x, y, UI.RightClickMenuLength, UI.RightClickMenuWidth);
	return ptr;
}

void Output::DrawAfterMenu(image * img, int x, int y)
{
	pWind->DrawImage(img, x, y, UI.RightClickMenuLength, UI.RightClickMenuWidth);
}



void Output::DrawCleanImage(image* img, int x, int y)
{
	pWind->DrawImage(img, x - UI.GRID_SIZE - 5, y - UI.GRID_SIZE - 5, 2 * UI.GRID_SIZE + 4, UI.GATE_Height + 3);
}
void Output::changeConnectionColor(Connection * connection, color Color) {
	bool b1 = false, b2 = false, PreviousIsIntersection = false, PreviousIsIntersection2 = false, isCell2XGreaterThanCellX = false, isCell2YGreaterThanCellY = false;
	int i = 0;
	int Vertical0Horizontal1Nothing2 = 2;

	for (size_t j = 0; j < connection->getCellsBeforeAddingConnection().size(); j++)
	{
		Cell cell = connection->getCellsBeforeAddingConnection()[j];

		if (j < connection->getCellsBeforeAddingConnection().size() - 1)
		{
			Cell cell2 = connection->getCellsBeforeAddingConnection()[j + 1];
			pWind->SetPen(Color, 2);
			pWind->SetBrush(Color);

			// if above another connection: color the hump
			if (usedPixels[cell2.y][cell2.x] == INTERSECTION && cell2.cellType == VERTICAL && Vertical0Horizontal1Nothing2 == 2) {
				if (cell2.x < cell.x) {
					b1 = true;
					if (!PreviousIsIntersection)
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE);
					else {
						PreviousIsIntersection = false;
						pWind->DrawLine(cell.x * UI.GRID_SIZE - 6, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE);
					}
					pWind->DrawBezier(cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE - 14, cell.y*UI.GRID_SIZE - 6, cell.x * UI.GRID_SIZE - 18, cell.y*UI.GRID_SIZE - 8, cell.x * UI.GRID_SIZE - 22, cell.y*UI.GRID_SIZE);
					if (j + 2 < connection->getCellsBeforeAddingConnection().size() && arrayOfIntersections[connection->getCellsBeforeAddingConnection()[j + 2].y][connection->getCellsBeforeAddingConnection()[j + 2].x] != 0)
						pWind->DrawLine(cell.x * UI.GRID_SIZE - 22, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE - 32, cell.y*UI.GRID_SIZE);
				}
				else {
					b1 = false;
					if (!PreviousIsIntersection)
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE);
					else {
						PreviousIsIntersection = false;
						pWind->DrawLine(cell.x * UI.GRID_SIZE + 6, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE);
					}
					pWind->DrawBezier(cell.x * UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE + 14, cell.y*UI.GRID_SIZE - 6, cell.x * UI.GRID_SIZE + 18, cell.y*UI.GRID_SIZE - 8, cell.x * UI.GRID_SIZE + 22, cell.y*UI.GRID_SIZE);
					if (j + 2 < connection->getCellsBeforeAddingConnection().size() && arrayOfIntersections[connection->getCellsBeforeAddingConnection()[j + 2].y][connection->getCellsBeforeAddingConnection()[j + 2].x] != 0)
						pWind->DrawLine(cell.x * UI.GRID_SIZE + 22, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE + 32, cell.y*UI.GRID_SIZE);
				}
				PreviousIsIntersection = true;
				Vertical0Horizontal1Nothing2 = -1;
			}
			else if (usedPixels[cell2.y][cell2.x] == INTERSECTION && cell2.cellType == HORIZONTAL && Vertical0Horizontal1Nothing2 == 2) {
				if (cell2.y < cell.y) {
					b2 = true;
					if (!PreviousIsIntersection2)
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 10);
					else {
						PreviousIsIntersection2 = false;
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 6, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 10);
					}
					pWind->DrawBezier(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 10, cell.x * UI.GRID_SIZE + 6, cell.y*UI.GRID_SIZE - 14, cell.x * UI.GRID_SIZE + 8, cell.y*UI.GRID_SIZE - 18, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 22);
					if (j + 2 < connection->getCellsBeforeAddingConnection().size() && arrayOfIntersections[connection->getCellsBeforeAddingConnection()[j + 2].y][connection->getCellsBeforeAddingConnection()[j + 2].x] != 0)
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 22, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 32);
				}
				else {
					b2 = false;
					if (!PreviousIsIntersection2)
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 10);
					else {
						PreviousIsIntersection2 = false;
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 6, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 10);
					}
					pWind->DrawBezier(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 10, cell.x * UI.GRID_SIZE + 6, cell.y*UI.GRID_SIZE + 14, cell.x * UI.GRID_SIZE + 8, cell.y*UI.GRID_SIZE + 18, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 22);
					if (j + 2 < connection->getCellsBeforeAddingConnection().size() && arrayOfIntersections[connection->getCellsBeforeAddingConnection()[j + 2].y][connection->getCellsBeforeAddingConnection()[j + 2].x] != 0)
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 22, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 32);
				}
				PreviousIsIntersection2 = true;
				Vertical0Horizontal1Nothing2 = -1;

			}
			else if (usedPixels[cell2.y][cell2.x] == INTERSECTION && cell2.cellType == EMPTY && arrayOfCorners[cell2.y][cell2.x] == 0) {

				if (j - 1 >= 0 && connection->getCellsBeforeAddingConnection()[j - 1].y == cell.y)
				{
					Vertical0Horizontal1Nothing2 = 1;
				}
				else if (cell2.y == cell.y)
				{
					Vertical0Horizontal1Nothing2 = 1;
				}
				else {
					Vertical0Horizontal1Nothing2 = 0;
				}

				if (Vertical0Horizontal1Nothing2 == 1)
				{
					if (cell2.x > cell.x) {
						isCell2XGreaterThanCellX = true;
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE + 4, cell2.y*UI.GRID_SIZE);
					}
					else {
						isCell2XGreaterThanCellX = false;
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE + 6, cell2.y*UI.GRID_SIZE);
					}
				}
				else if (Vertical0Horizontal1Nothing2 == 0) {
					if (cell2.y < cell.y) {
						isCell2YGreaterThanCellY = false;
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE - 3);
					}
					else {
						isCell2YGreaterThanCellY = true;
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE - 6);
					}
				}
			}
			else {
				if (Vertical0Horizontal1Nothing2 == 2)
				{
					if (PreviousIsIntersection) {
						pWind->DrawLine(cell.x * UI.GRID_SIZE - ((b1) ? 6 : -6), cell.y*UI.GRID_SIZE - ((PreviousIsIntersection2) ? 6 : 0), cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);

					}
					else if (PreviousIsIntersection2) {
						pWind->DrawLine(cell.x * UI.GRID_SIZE , cell.y*UI.GRID_SIZE - ((b2) ? 6 : -6), cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);

					}
					else {
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);

					}
				}
				else if (Vertical0Horizontal1Nothing2 == 1) {
					if (isCell2XGreaterThanCellX) {
						pWind->DrawLine(cell.x * UI.GRID_SIZE + 6, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);
					}
					else {
						pWind->DrawLine(cell.x * UI.GRID_SIZE + 4, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);
					}
				}
				else if (Vertical0Horizontal1Nothing2 == 0) {
					if (!isCell2YGreaterThanCellY) {
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 6, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);
					}
					else {
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 3, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);
					}
				}

				int originalY = connection->getDestPin()->getComponent()->getCenterLocation().y1;
				int	originalX = connection->getDestPin()->getComponent()->getCenterLocation().x1;
				if (connection->getDestPin()->getPosition() == 0 && cell.cellType == PIN && j != connection->getCellsBeforeAddingConnection().size() - 2)
				{
					pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y* UI.GRID_SIZE, originalX - UI.GATE_Width / 2 + 3, originalY - 13);
				}
				else if (connection->getDestPin()->getPosition() == 1 && cell.cellType == PIN && j != connection->getCellsBeforeAddingConnection().size() - 2) {
					pWind->DrawLine(cell.x * UI.GRID_SIZE - 3, originalY, originalX - UI.GATE_Width / 2 + 3, originalY);
				}
				else if (connection->getDestPin()->getPosition() == 2 && cell.cellType == PIN&& j != connection->getCellsBeforeAddingConnection().size() - 2) {
					pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y* UI.GRID_SIZE, originalX - UI.GATE_Width / 2 + 3, originalY + 13);
				}
				PreviousIsIntersection = PreviousIsIntersection2 = false;
				Vertical0Horizontal1Nothing2 = 2;
			}

		}
	}

}
void Output::clearConnections(vector<Connection*>& allConnections, int originalX, int originalY, bool isInput) {
	for (size_t i = 0; i < allConnections.size(); i++)
	{
		for (size_t j = 0; j < allConnections[i]->getCellsBeforeAddingConnection().size(); j++)
		{
			Cell& cell = allConnections[i]->getCellsBeforeAddingConnection()[j];
			int Vertical0Horizontal1Nothing2 = 2;

			if (j < allConnections[i]->getCellsBeforeAddingConnection().size() - 1)
			{
				Vertical0Horizontal1Nothing2 = 2;
				Cell& cell2 = allConnections[i]->getCellsBeforeAddingConnection()[j + 1];

				// this Connection under other connection
				if (usedPixels[cell.y][cell.x] == INTERSECTION && cell.cellType == EMPTY && arrayOfCorners[cell.y][cell.x] == 0) {

					if (j - 1 >= 0 && allConnections[i]->getCellsBeforeAddingConnection()[j - 1].y == cell.y)
					{
						Vertical0Horizontal1Nothing2 = 1;
					}
					else if (cell2.y == cell.y)
					{
						Vertical0Horizontal1Nothing2 = 1;
					}
					else {
						Vertical0Horizontal1Nothing2 = 0;
					}

					if (Vertical0Horizontal1Nothing2 == 1)
					{
						pWind->SetPen(WHITE, 2);
						pWind->SetBrush(WHITE);
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x*UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);
						pWind->DrawRectangle(cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE - 10, cell.x*UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE + 10, FILLED);
						pWind->SetPen(color(23, 79, 181), 2);
						pWind->DrawLine(cell.x* UI.GRID_SIZE, cell.y* UI.GRID_SIZE - 11, cell.x* UI.GRID_SIZE, cell.y* UI.GRID_SIZE + 10);
					}
					else if (Vertical0Horizontal1Nothing2 == 0) {
						pWind->SetPen(WHITE, 2);
						pWind->SetBrush(WHITE);
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x*UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);
						pWind->DrawRectangle(cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE - 10, cell.x*UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE + 10, FILLED);
						pWind->SetPen(color(23, 79, 181), 2);
						pWind->DrawLine(cell.x* UI.GRID_SIZE - 15, cell.y* UI.GRID_SIZE, cell.x* UI.GRID_SIZE + 10, cell.y* UI.GRID_SIZE);
					}
				}

				// Any connection
				pWind->SetPen(WHITE, 2);
				if (Vertical0Horizontal1Nothing2 == 2)
				{
					pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);
					pWind->SetBrush(WHITE);
					pWind->SetPen(BLACK, 1);
					pWind->DrawPixel(cell.x*UI.GRID_SIZE, cell.y*UI.GRID_SIZE);
				}

				//Connection above connection
				if (usedPixels[cell.y][cell.x] == INTERSECTION && cell.cellType == VERTICAL && Vertical0Horizontal1Nothing2 == 2) {
					pWind->SetPen(WHITE, 2);
					pWind->SetBrush(WHITE);
					pWind->DrawRectangle(cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE - 10, cell.x*UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE + 10, FILLED);
					pWind->SetPen(BLACK, 1);
					pWind->DrawPixel(cell.x*UI.GRID_SIZE, cell.y*UI.GRID_SIZE);
					pWind->SetPen(color(23, 79, 181), 2);
					pWind->DrawLine(cell.x* UI.GRID_SIZE, cell.y* UI.GRID_SIZE - 10, cell.x* UI.GRID_SIZE, cell.y* UI.GRID_SIZE + 10);
					Vertical0Horizontal1Nothing2 = -1;
				}
				if (usedPixels[cell.y][cell.x] == INTERSECTION && cell.cellType == HORIZONTAL && Vertical0Horizontal1Nothing2 == 2) {
					pWind->SetPen(WHITE, 2);
					pWind->SetBrush(WHITE);
					pWind->DrawRectangle(cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE - 10, cell.x*UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE + 10, FILLED);
					pWind->SetPen(BLACK, 1);
					pWind->DrawPixel(cell.x*UI.GRID_SIZE, cell.y*UI.GRID_SIZE);
					pWind->SetPen(color(23, 79, 181), 2);
					pWind->DrawLine(cell.x* UI.GRID_SIZE - 11, cell.y* UI.GRID_SIZE, cell.x* UI.GRID_SIZE + 10, cell.y* UI.GRID_SIZE);
					Vertical0Horizontal1Nothing2 = -1;
				}

				//Clear other connections intersections
				if (Vertical0Horizontal1Nothing2 == 0 || Vertical0Horizontal1Nothing2 == 1)
				{
					vector<Connection*> allConnections2;
					pManager->getAllConnections(allConnections2);

					for (size_t k = 0; k < allConnections2.size(); k++)
					{
						for (size_t l = 0; l < allConnections2[k]->getCellsBeforeAddingConnection().size(); l++)
						{
							if (allConnections2[k] != allConnections[i] && allConnections2[k]->getCellsBeforeAddingConnection()[l].x == cell.x && allConnections2[k]->getCellsBeforeAddingConnection()[l].y == cell.y)
							{
								allConnections2[k]->getCellsBeforeAddingConnection()[l].cellType = EMPTY;
							}
						}
					}
				}
				if (isInput)
				{
					//clear input pin small lines
					pWind->SetPen(WHITE, 2);
					pWind->SetBrush(WHITE);
					if (allConnections[i]->getDestPin()->getPosition() == 0 && cell.cellType == PIN && j != allConnections[i]->getCellsBeforeAddingConnection().size() - 2)
					{
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y* UI.GRID_SIZE, originalX - UI.GATE_Width / 2 + 3, originalY - 13);
					}
					else if (allConnections[i]->getDestPin()->getPosition() == 1 && cell.cellType == PIN && j != allConnections[i]->getCellsBeforeAddingConnection().size() - 2) {
						pWind->DrawLine(cell.x * UI.GRID_SIZE - 3, originalY, originalX - UI.GATE_Width / 2 + 3, originalY);
					}
					else if (allConnections[i]->getDestPin()->getPosition() == 2 && cell.cellType == PIN&& j != allConnections[i]->getCellsBeforeAddingConnection().size() - 2) {
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y* UI.GRID_SIZE, originalX - UI.GATE_Width / 2 + 3, originalY + 13);
					}
					pWind->SetPen(BLACK, 1);
					pWind->DrawPixel(cell.x*UI.GRID_SIZE, cell.y*UI.GRID_SIZE);

				}
				else {
					pWind->SetPen(WHITE, 2);
					pWind->SetBrush(WHITE);
					originalY = allConnections[i]->getDestPin()->getComponent()->getCenterLocation().y1;
					originalX = allConnections[i]->getDestPin()->getComponent()->getCenterLocation().x1;
					if (allConnections[i]->getDestPin()->getPosition() == 0 && cell.cellType == PIN && j != allConnections[i]->getCellsBeforeAddingConnection().size() - 2)
					{
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y* UI.GRID_SIZE, originalX - UI.GATE_Width / 2 + 3, originalY - 13);
					}
					else if (allConnections[i]->getDestPin()->getPosition() == 1 && cell.cellType == PIN && j != allConnections[i]->getCellsBeforeAddingConnection().size() - 2) {
						pWind->DrawLine(cell.x * UI.GRID_SIZE - 3, originalY, originalX - UI.GATE_Width / 2 + 3, originalY);
					}
					else if (allConnections[i]->getDestPin()->getPosition() == 2 && cell.cellType == PIN&& j != allConnections[i]->getCellsBeforeAddingConnection().size() - 2) {
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y* UI.GRID_SIZE, originalX - UI.GATE_Width / 2 + 3, originalY + 13);
					}
					pWind->SetPen(BLACK, 1);
					pWind->DrawPixel(cell.x*UI.GRID_SIZE, cell.y*UI.GRID_SIZE);

				}
			}
			// Clear Grid
			arrayOfIntersections[cell.y][cell.x] = -1;
			arrayOfCorners[cell.y][cell.x] = 0;
			if (cell.cellType != PIN)
			{
				usedPixels[cell.y][cell.x] = cell.cellType;
				if (connectionsCountAtPixel[cell.y][cell.x] == 2)
				{
					if (Vertical0Horizontal1Nothing2 == 0)
					{
						usedPixels[cell.y][cell.x] = HORIZONTAL;
					}
					else if (Vertical0Horizontal1Nothing2 == 1) {
						usedPixels[cell.y][cell.x] = VERTICAL;
					}

				}
				connectionsCountAtPixel[cell.y][cell.x] -= ((connectionsCountAtPixel[cell.y][cell.x] > 0) ? 1 : 0);
			}
			else if (j != allConnections[i]->getCellsBeforeAddingConnection().size() - 1) {
				usedPixels[cell.y][cell.x] = EMPTY;
			}
			if (j == allConnections[i]->getCellsBeforeAddingConnection().size() - 1) {
				pWind->SetPen(BLACK, 1);
				pWind->DrawPixel(cell.x*UI.GRID_SIZE, cell.y*UI.GRID_SIZE);
			}
		}
		// clear connection
		allConnections[i]->getCellsBeforeAddingConnection().clear();
	}

}
bool Output::SetDragImage(ActionType ActType, GraphicsInfo& GfxInfo, image* smallCleanImageBeforeAddingGate, bool moving, Component* comp) {
	int originalX, originalY;
	if (moving)
	{
		originalX = comp->getCenterLocation().x1;
		originalY = comp->getCenterLocation().y1;
	}
	int iXOld = 0;
	int iYOld = 0;

	pWind->GetMouseCoord(iXOld, iXOld);

	char cKeyData;
	int RectULX = iXOld - UI.GATE_Width / 2;
	int RectULY = iYOld - UI.GATE_Height / 2;
	if (!moving)
	{
		PrintMsg("Please select point within workspace and avoid overlaping!, press ESCAPE to stop");
	}
	else {
		PrintMsg("Please select point within workspace and avoid overlaping!");
	}

	bool draw = true, isOriginalDrawn = false;
	vector<Connection*> allInputConnections, allOutputConnections;
	int noOfTotalConnections = 0;
	int drawnConnectionsCount = 0;

	image* storedImg = new image;
	image* storedDrawingImg = new image;
	if (moving)
	{
		//Clear connections from UI
		comp->getAllInputConnections(allInputConnections);
		comp->getAllOutputConnections(allOutputConnections);
		noOfTotalConnections = allInputConnections.size() + allOutputConnections.size();

		clearConnections(allInputConnections, originalX, originalY, true);
		clearConnections(allOutputConnections, originalX, originalY, false);
		//printMatrix("After clearing connections");
	}

	pWind->StoreImage(storedImg, 0, 0, pWind->GetWidth(), pWind->GetHeight());
	pWind->StoreImage(storedDrawingImg, 0, UI.ToolBarHeight, pWind->GetWidth(), pWind->GetHeight() - UI.StatusBarHeight);

	pWind->SetPen(BLUE, 2);
	while (true)
	{
		int x, y;
		pWind->GetMouseCoord(x, y);
		bool wrong = false;
		Utils::correctPointClicked(x, y, true, false);


		if (moving && (x != iXOld || y != iYOld))
		{
			//Clear connections from grid
			for (size_t i = 0; i < allOutputConnections.size(); i++)
			{
				for (size_t j = 0; j < allOutputConnections[i]->getCellsBeforeAddingConnection().size(); j++)
				{
					Cell cell = allOutputConnections[i]->getCellsBeforeAddingConnection()[j];
					int Vertical0Horizontal1Nothing2 = 2;

					// Clear intersected connections
					if (j < allOutputConnections[i]->getCellsBeforeAddingConnection().size() - 1)
					{
						Vertical0Horizontal1Nothing2 = 2;
						Cell& cell2 = allOutputConnections[i]->getCellsBeforeAddingConnection()[j + 1];

						// Connection under connection
						if (usedPixels[cell.y][cell.x] == INTERSECTION && cell.cellType == EMPTY && arrayOfCorners[cell.y][cell.x] == 0) {

							if (j - 1 >= 0 && allOutputConnections[i]->getCellsBeforeAddingConnection()[j - 1].y == cell.y)
							{
								Vertical0Horizontal1Nothing2 = 1;
							}
							else if (cell2.y == cell.y)
							{
								Vertical0Horizontal1Nothing2 = 1;
							}
							else {
								Vertical0Horizontal1Nothing2 = 0;
							}

						}
						//Clear other connections intersections
						if (Vertical0Horizontal1Nothing2 == 0 || Vertical0Horizontal1Nothing2 == 1)
						{
							vector<Connection*> allConnections;
							pManager->getAllConnections(allConnections);

							for (size_t k = 0; k < allConnections.size(); k++)
							{
								for (size_t l = 0; l < allConnections[k]->getCellsBeforeAddingConnection().size(); l++)
								{
									if (allConnections[k] != allOutputConnections[i] && allConnections[k]->getCellsBeforeAddingConnection()[l].x == cell.x && allConnections[k]->getCellsBeforeAddingConnection()[l].y == cell.y)
									{
										allConnections[k]->getCellsBeforeAddingConnection()[l].cellType = EMPTY;
									}
								}
							}
						}
					}
					// Clear Grid
					arrayOfIntersections[cell.y][cell.x] = -1;
					arrayOfCorners[cell.y][cell.x] = 0;
					if (cell.cellType != PIN)
					{
						usedPixels[cell.y][cell.x] = cell.cellType;
						connectionsCountAtPixel[cell.y][cell.x] -= ((connectionsCountAtPixel[cell.y][cell.x] > 0) ? 1 : 0);
					}
					else {
						usedPixels[cell.y][cell.x] = EMPTY;
					}
				}
				allOutputConnections[i]->getCellsBeforeAddingConnection().clear();
			}
			for (size_t i = 0; i < allInputConnections.size(); i++)
			{
				for (size_t j = 0; j < allInputConnections[i]->getCellsBeforeAddingConnection().size(); j++)
				{
					Cell cell = allInputConnections[i]->getCellsBeforeAddingConnection()[j];
					int Vertical0Horizontal1Nothing2 = 2;

					// Clear intersected connections
					if (j < allInputConnections[i]->getCellsBeforeAddingConnection().size() - 1)
					{
						Vertical0Horizontal1Nothing2 = 2;
						Cell& cell2 = allInputConnections[i]->getCellsBeforeAddingConnection()[j + 1];

						// Connection under connection
						if (usedPixels[cell.y][cell.x] == INTERSECTION && cell.cellType == EMPTY && arrayOfCorners[cell.y][cell.x] == 0) {

							if (j - 1 >= 0 && allInputConnections[i]->getCellsBeforeAddingConnection()[j - 1].y == cell.y)
							{
								Vertical0Horizontal1Nothing2 = 1;
							}
							else if (cell2.y == cell.y)
							{
								Vertical0Horizontal1Nothing2 = 1;
							}
							else {
								Vertical0Horizontal1Nothing2 = 0;
							}

						}
						//Clear other connections intersections
						if (Vertical0Horizontal1Nothing2 == 0 || Vertical0Horizontal1Nothing2 == 1)
						{
							vector<Connection*> allConnections;
							pManager->getAllConnections(allConnections);

							for (size_t k = 0; k < allConnections.size(); k++)
							{
								for (size_t l = 0; l < allConnections[k]->getCellsBeforeAddingConnection().size(); l++)
								{
									if (allConnections[k] != allInputConnections[i] && allConnections[k]->getCellsBeforeAddingConnection()[l].x == cell.x && allConnections[k]->getCellsBeforeAddingConnection()[l].y == cell.y)
									{
										allConnections[k]->getCellsBeforeAddingConnection()[l].cellType = EMPTY;
									}
								}
							}
						}
					}
					arrayOfIntersections[cell.y][cell.x] = -1;
					arrayOfCorners[cell.y][cell.x] = 0;
					if (cell.cellType != PIN || (allInputConnections[i]->getCellsBeforeAddingConnection()[allInputConnections[i]->getCellsBeforeAddingConnection().size() - 1].cellType == PIN && allInputConnections[i]->getCellsBeforeAddingConnection()[allInputConnections[i]->getCellsBeforeAddingConnection().size() - 2].cellType == PIN && (j == allInputConnections[i]->getCellsBeforeAddingConnection().size() - 2)))
					{
						usedPixels[cell.y][cell.x] = cell.cellType;
						connectionsCountAtPixel[cell.y][cell.x] -= ((connectionsCountAtPixel[cell.y][cell.x] > 0) ? 1 : 0);

					}
				}
				allInputConnections[i]->getCellsBeforeAddingConnection().clear();
			}
			drawnConnectionsCount = 0;
			//printMatrix("After clearing connections");
		}
		if (!Utils::CheckPoint({ x,y }, usedPixels, moving, false)) {
			wrong = true;
		}
		else {
			wrong = false;
		}
		if (Utils::CheckPoint(x, y, usedPixels)) {
			if (x != iXOld || y != iYOld) {
				pWind->DrawImage(storedDrawingImg, 0, UI.ToolBarHeight, pWind->GetWidth(), pWind->GetHeight() - UI.StatusBarHeight);

				if (x != iXOld) {
					RectULX = RectULX + (x - iXOld);
					iXOld = x;
				}
				if (y != iYOld) {
					RectULY = RectULY + (y - iYOld);
					iYOld = y;
				}
				GraphicsInfo Gfx;
				Gfx.x1 = RectULX + UI.GATE_Width / 2;
				Gfx.y1 = RectULY + UI.GATE_Height / 2;
				pWind->StoreImage(smallCleanImageBeforeAddingGate, Gfx.x1 - UI.GRID_SIZE - 5, Gfx.y1 - UI.GRID_SIZE - 5, 2 * UI.GRID_SIZE + 4, UI.GATE_Height + 3);

				switch (ActType) {
				case ADD_Buff: {
					DrawNot_Buffer(Gfx, true, false, wrong);
					break;
				}
				case ADD_INV:
				{

					DrawNot_Buffer(Gfx, false, false, wrong);

					break;
				}
				case ADD_AND_GATE_2:
				{

					DrawAnd_Nand(Gfx, 2, false, false, wrong);
					break;
				}
				case ADD_AND_GATE_3:
				{
					DrawAnd_Nand(Gfx, 3, false, false, wrong);
					break;
				}

				case ADD_NAND_GATE_2:
				{

					DrawAnd_Nand(Gfx, 2, true, false, wrong);
					break;
				}
				case ADD_OR_GATE_2:
				{
					DrawOr_Nor(Gfx, 2, false, false, wrong);
					break;
				}

				case ADD_NOR_GATE_2:
				{
					DrawOr_Nor(Gfx, 2, true, false, wrong);

					break;
				}

				case ADD_XOR_GATE_2:
				{

					DrawXor_Xnor(Gfx, 2, false, false, wrong);
					break;
				}
				case ADD_XOR_GATE_3:
				{
					DrawXor_Xnor(Gfx, 3, false, false, wrong);
					break;
				}

				case ADD_XNOR_GATE_2:
				{

					DrawXor_Xnor(Gfx, 2, true, false, wrong);
					break;
				}
				case ADD_NOR_GATE_3:
				{

					DrawOr_Nor(Gfx, 3, true, false, wrong);
					break;
				}
				case ADD_Switch:
				{
					DrawSwtich(Gfx, true, false, wrong);
					break;
				}
				case ADD_LED:
				{
					DrawLed(Gfx, false, false, wrong);
					break;
				}
				case ADD_Buff_H: {
					DrawNot_Buffer(Gfx, true, true, wrong);
					break;
				}
				case ADD_INV_H:
				{

					DrawNot_Buffer(Gfx, false, true, wrong);

					break;
				}
				case ADD_AND_GATE_2_H:
				{

					DrawAnd_Nand(Gfx, 2, false, true, wrong);
					break;
				}
				case ADD_AND_GATE_3_H:
				{
					DrawAnd_Nand(Gfx, 3, false, true, wrong);
					break;
				}

				case ADD_NAND_GATE_2_H:
				{

					DrawAnd_Nand(Gfx, 2, true, true, wrong);
					break;
				}
				case ADD_OR_GATE_2_H:
				{
					DrawOr_Nor(Gfx, 2, false, true, wrong);
					break;
				}

				case ADD_NOR_GATE_2_H:
				{
					DrawOr_Nor(Gfx, 2, true, true, wrong);

					break;
				}

				case ADD_XOR_GATE_2_H:
				{

					DrawXor_Xnor(Gfx, 2, false, true, wrong);
					break;
				}
				case ADD_XOR_GATE_3_H:
				{
					DrawXor_Xnor(Gfx, 3, false, true, wrong);
					break;
				}

				case ADD_XNOR_GATE_2_H:
				{

					DrawXor_Xnor(Gfx, 2, true, true, wrong);
					break;
				}
				case ADD_NOR_GATE_3_H:
				{

					DrawOr_Nor(Gfx, 3, true, true, wrong);
					break;
				}
				case ADD_Switch_H:
				{
					DrawSwtich(Gfx, true, true, wrong);
					break;
				}
				case ADD_LED_H:
				{
					DrawLed(Gfx, false, true, wrong);
					break;
				}
				}
				//move connections with gate
				if (moving && !wrong)
				{
					int xbegin = (Gfx.x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (Gfx.x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (Gfx.y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (Gfx.y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
					for (int i = ybegin + 1; i <= yend; i++)
					{
						for (int j = xbegin; j <= xend; j++)
						{
							if (xbegin == j || xend == j)
							{
								setUsedPixel(i, j, PIN);
								continue;
							}
							setUsedPixel(i, j, GATE);
						}
					}

					//Reconnect
					for (size_t i = 0; i < allInputConnections.size(); i++)
					{
						GraphicsInfo currentGfx = allInputConnections[i]->getCornersLocation();
						currentGfx.x2 = currentGfx.x2 + (Gfx.x1 - originalX);
						currentGfx.y2 = currentGfx.y2 + (Gfx.y1 - originalY);
						allInputConnections[i]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
						if (DrawConnection(currentGfx, allInputConnections[i]->getDestPin()->getPosition(), { Gfx.x1, Gfx.y1,0,0 }, allInputConnections[i]->getCellsBeforeAddingConnection()))drawnConnectionsCount++;
					}

					for (size_t i = 0; i < allOutputConnections.size(); i++)
					{
						GraphicsInfo currentGfx = allOutputConnections[i]->getCornersLocation();
						currentGfx.x1 = currentGfx.x1 + (Gfx.x1 - originalX);
						currentGfx.y1 = currentGfx.y1 + (Gfx.y1 - originalY);
						allOutputConnections[i]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
						Component* dstComp = allOutputConnections[i]->getDestPin()->getComponent();
						if (DrawConnection(currentGfx, allOutputConnections[i]->getDestPin()->getPosition(), { dstComp->getCenterLocation().x1, dstComp->getCenterLocation().y1,0,0 }, allOutputConnections[i]->getCellsBeforeAddingConnection()))drawnConnectionsCount++;
					}

					originalX = Gfx.x1;
					originalY = Gfx.y1;
					//Clear gate/led/switch
					for (int i = ybegin + 1; i <= yend; i++)
					{
						for (int j = xbegin; j <= xend; j++)
						{
							if (usedPixels[i][j] != INTERSECTION)
							{
								setUsedPixel(i, j, EMPTY);
							}
						}
					}
				}
				pWind->UpdateBuffer();
			}

		}

		if (!moving && pWind->GetKeyPress(cKeyData) == ESCAPE) {
			pWind->DrawImage(storedImg, 0, 0, pWind->GetWidth(), pWind->GetHeight());
			draw = false;
			break;
		}
		else if (!wrong && (pWind->GetMouseClick(GfxInfo.x1, GfxInfo.y1) == LEFT_CLICK || (moving && (pWind->GetButtonState(LEFT_BUTTON, GfxInfo.x1, GfxInfo.y1) == BUTTON_UP)))) {
			if ((moving && (noOfTotalConnections == drawnConnectionsCount)) || !moving)
			{
				pWind->FlushMouseQueue();
				GfxInfo.x1 = x;
				GfxInfo.y1 = y;
				Utils::correctPointClicked(GfxInfo.x1, GfxInfo.y1, true, false);
				if (Utils::CheckPoint(GfxInfo, usedPixels, moving)) {
					draw = true;
					break;
				}
				else {
					draw = false;
				}
			}
		}
	}
	//printMatrix("Final");
	pWind->FlushMouseQueue();
	PrintMsg("");
	delete storedDrawingImg;
	delete storedImg;
	return draw;
}
void Output::printMatrix(string msg) {
	cout << msg << endl;
	for (size_t i = 0; i < 44; i++)
	{
		for (size_t j = 0; j < 74; j++)
		{
			cout << usedPixels[i][j] << " ";
		}
		cout << endl;
	}
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//You Send a Centre Point (cx,cy) ,this means when you call Draw image Function , x and y sent should be cx-24, cy-24
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Output::DrawAnd_Nand(GraphicsInfo g, int in, bool isNand, bool highlighted, bool notValid) const {

	// Points
	int cx = g.x1, cy = g.y1; //Centre Point
	int in1x, in1y, in2x, in2y, outx, outy; // the 2 Inputs & Output
	int p1x, p2x, p1y, p2y; //the Helping Points

	in1x = in2x = p1x = p2x = cx - 14 + (2 * ciDefBrushSize);
	in1y = cy - 13; in2y = cy + 13;
	outy = cy;
	p1y = cy - 20; p2y = cy + 21;

	if (highlighted) pWind->SetPen(BLUE); else if (notValid) pWind->SetPen(RED);
	else pWind->SetPen(BROWN);

	if (isNand) {
		outx = cx + 14 - (2 * ciDefBrushSize);
		p1x = p2x = p1x - 5;
		//Darwing lines
		pWind->DrawLine(p1x, p1y, p1x + 8, p1y, FRAME);
		pWind->DrawLine(p2x, p2y, p2x + 8, p2y, FRAME);
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x - 6, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x - 6, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 3 * ciDefBrushSize - 1, outy, outx + 7 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		pWind->DrawLine(p1x, p1y - 1, p2x, p2y, FRAME);
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x, in3y = cy;
			pWind->DrawLine(in3x - 6 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
		}
		//Drawing Arc
		pWind->DrawBezier(p1x + 8, p1y, p1x + 10 + 3 + 4, p1y + 4, p1x + 10 + 9 + 4, p1y + 12, outx, outy, FRAME);
		pWind->DrawBezier(p2x + 8, p2y, p2x + 10 + 3 + 4, p2y - 4, p2x + 10 + 9 + 4, p2y - 12, outx, outy, FRAME);

		//Drawing Buuble
		pWind->DrawCircle(outx - ciDefBrushSize + 2 * ciDefBrushSize, outy, 2 * ciDefBrushSize, FRAME);

	}
	else {
		outx = cx + 15 - (2 * ciDefBrushSize);
		//Darwing lines
		pWind->DrawLine(p1x, p1y, p1x + 8, p1y, FRAME);
		pWind->DrawLine(p2x, p2y, p2x + 8, p2y, FRAME);
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 3, outy, outx + 6 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		pWind->DrawLine(p1x, p1y - 1, p2x, p2y, FRAME);
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x, in3y = cy;
			pWind->DrawLine(in3x - 6 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
		}
		//Drawing Arc
		//pWind->DrawBezier(p1x + 8, p1y, p1x + 8 + 5 , outy + 8, p1x + 8 + 8 , outy + 3, outx, outy, FRAME);
		pWind->DrawBezier(p1x + 8, p1y, p1x + 10 + 3 + 4, p1y + 4, p1x + 10 + 9 + 4, p1y + 12, outx + 4, outy, FRAME);
		pWind->DrawBezier(p2x + 8, p2y, p2x + 10 + 3 + 4, p2y - 4, p2x + 10 + 9 + 4, p2y - 12, outx + 4, outy, FRAME);
		//pWind->DrawPixel(, );
	}
}

void Output::DrawNot_Buffer(GraphicsInfo g, bool isBuffer, bool highlighted, bool notValid) const {
	int cx = g.x1, cy = g.y1; //Centre Point
	int p1x, p1y, p2x, p2y, inx, iny, outx, outy; //Vertices of Triangle and input/output Points

	p1x = p2x = inx = cx - 15 + (2 * ciDefBrushSize);
	p1y = cy + 21; p2y = cy - 21;
	iny = outy = cy;

	if (highlighted) pWind->SetPen(BLUE); else if (notValid) pWind->SetPen(RED);
	else if (notValid) pWind->SetPen(RED);
	else pWind->SetPen(BROWN);

	if (isBuffer) {
		//BUFFER Gate
		outx = cx + 15 - (2 * ciDefBrushSize);
		//Drawing lines 
		pWind->DrawLine(inx - 6 - (2 * ciDefBrushSize), iny, inx, iny, FRAME); //The Input-Line
		pWind->DrawLine(outx, outy, outx + 6 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
																				   //Drawing Trianlge
		pWind->DrawTriangle(p1x, p1y, p2x, p2y, outx, outy, FRAME);
	}
	else {
		// NOT GATE
		outx = cx + 14 - (2 * ciDefBrushSize);
		//Drawing lines 
		pWind->DrawLine(inx - 6 - (2 * ciDefBrushSize), iny, inx, iny, FRAME); //The Input-Line
		pWind->DrawLine(outx + 4 * ciDefBrushSize - 1, outy, outx + 7 + (2 * ciDefBrushSize), outy, FRAME);//The Output-Line

																										   //Drawing Trianlge
		pWind->DrawTriangle(p1x, p1y, p2x, p2y, outx, outy, FRAME);
		//Darwing Bubble
		pWind->DrawCircle(outx + 2 * ciDefBrushSize, outy, 2 * ciDefBrushSize, FRAME);
	}
}

void Output::DrawOr_Nor(GraphicsInfo g, int in, bool isNor, bool highlighted, bool notValid) const {
	int cx = g.x1, cy = g.y1; //Centre Point
	int p1x, p1y, p2x, p2y, hx1, hx2, hy1, hy2, kx, ky, ky2; //Helping Points
	int in1x, in1y, in2x, in2y, outx, outy; // the 2 Inputs & Output

	in1x = in2x = cx - 15 + (2 * ciDefBrushSize);
	in1y = cy - 13;
	in2y = cy + 13; outy = cy;

	p1x = p2x = cx - 21;
	p2y = hy2 = cy + 21;
	p1y = hy1 = cy - 21;
	hx1 = hx2 = cx;

	if (highlighted) pWind->SetPen(BLUE); else if (notValid) pWind->SetPen(RED);
	else pWind->SetPen(BROWN);

	if (isNor) {
		//NOR GATE
		outy = cy; outx = cx + 14 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;

		//Draw lines
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 4 * ciDefBrushSize - 1, outy, outx + 7 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x + 2 * (ciDefBrushSize == 1 ? 2 : 1), in3y = cy;
			pWind->DrawLine(in3x - 8 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
		}
		//Draw Buuble
		pWind->DrawCircle(outx + 2 * ciDefBrushSize, outy, 2 * ciDefBrushSize, FRAME);
		//Draw Bezier
		pWind->DrawBezier(p1x, p1y, hx1, hy1, kx, ky, outx, outy, FRAME);
		pWind->DrawBezier(p2x, p2y, hx2, hy2, kx, ky2, outx, outy, FRAME);
		pWind->DrawBezier(p1x, p1y, in1x + 8, in1y, in2x + 8, in2y, p2x, p2y, FRAME);

	}

	else {
		//OR GATE
		outx = cx + 15 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;
		//Draw lines
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx, outy, outx + 6 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x + 2 * (ciDefBrushSize == 1 ? 2 : 1), in3y = cy;
			pWind->DrawLine(in3x - 8 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
		}
		//Draw Bezier
		pWind->DrawBezier(p1x, p1y, hx1, hy1, kx, ky, outx, outy, FRAME);
		pWind->DrawBezier(p2x, p2y, hx2, hy2, kx, ky2, outx, outy, FRAME);
		pWind->DrawBezier(p1x, p1y, in1x + 8, in1y, in2x + 8, in2y, p2x, p2y, FRAME);
	}



}

void Output::DrawXor_Xnor(GraphicsInfo g, int in, bool isXNor, bool highlighted, bool notValid) const
{
	int cx = g.x1, cy = g.y1; //Centre Points
	int in1x, in1y, in2x, in2y, outx, outy; // the 2 Inputs And Output
	int hx1, hx2, hy1, hy2, kx, ky, ky2, p1x, p1y, p2x, p2y; //Helping Points

	in1x = in2x = cx - 15 + (2 * ciDefBrushSize);
	in1y = cy - 13;
	in2y = cy + 13;

	p1x = p2x = cx - 22;
	p2y = hy2 = cy + 21;
	p1y = hy1 = cy - 21;
	hx1 = hx2 = cx;
	int xi = 10; //X-Increment

	if (highlighted) pWind->SetPen(BLUE); else if (notValid) pWind->SetPen(RED);
	else pWind->SetPen(BROWN);

	if (isXNor) {
		//XNOR GATE
		outy = cy; outx = cx + 14 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;
		//Draw lines
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 4 * ciDefBrushSize - 1, outy, outx - 1 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x, in3y = cy;
			pWind->DrawLine(in3x - 6 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
		}
		//Drawing Bezier
		pWind->DrawBezier(p1x + xi, p1y, hx1, hy1, kx, ky, outx, outy, FRAME);
		pWind->DrawBezier(p1x + xi, p1y, in1x + xi, in1y, in2x + xi, in2y, p2x + xi, p2y, FRAME);
		pWind->DrawBezier(p2x + xi, p2y, hx2, hy2, kx, ky2, outx, outy, FRAME);
		//draw the xor Bezier with Delta x slightly different than the previous to avoid collision
		pWind->DrawBezier(p1x + (2 * ciDefBrushSize), p1y, in1x + (2 * ciDefBrushSize), in1y, in2x + (2 * ciDefBrushSize), in2y, p2x + (2 * ciDefBrushSize), p2y, FRAME);
		//Drawing Bubble
		pWind->DrawCircle(outx + 2 * ciDefBrushSize, outy, 2 * ciDefBrushSize, FRAME);

	}
	else {
		//XOR GATE
		outy = cy; outx = cx + 15 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;
		//Draw lines
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx, outy, outx + (2 * ciDefBrushSize) - 1, outy, FRAME); //The Output-Line
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x, in3y = cy;
			pWind->DrawLine(in3x - 6 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
		}
		//Draw Bezier
		pWind->DrawBezier(p1x + xi, p1y, hx1, hy1, kx, ky, outx, outy, FRAME);
		pWind->DrawBezier(p1x + xi, p1y, in1x + xi, in1y, in2x + xi, in2y, p2x + xi, p2y, FRAME);
		pWind->DrawBezier(p2x + xi, p2y, hx2, hy2, kx, ky2, outx, outy, FRAME);
		//draw the xor Bezier with delta x slightly different than the previous to avoid collision
		pWind->DrawBezier(p1x + (2 * ciDefBrushSize), p1y, in1x + (2 * ciDefBrushSize), in1y, in2x + (2 * ciDefBrushSize), in2y, p2x + (2 * ciDefBrushSize), p2y, FRAME);
	}

}
void Output::DrawLed(GraphicsInfo g, bool isON, bool highlighted, bool notValid) const
{
	if (highlighted) pWind->SetPen(BLUE); else if (notValid) pWind->SetPen(RED);
	else pWind->SetPen(BROWN);
	int cx = g.x1, cy = g.y1, radius = 12; //Centre Points
	if (!isON)
	{
		pWind->SetBrush(GRAY);
		pWind->DrawCircle(cx, cy, radius, FILLED);
	}
	else
	{
		pWind->SetBrush(DARKGREEN);
		pWind->DrawCircle(cx, cy, radius, FILLED);
	}
	int inx = cx - radius - 10, iny = cy; // one input 
	pWind->DrawLine(inx, iny, cx - radius, cy, FRAME);//input line (left)
													  ///////////////------->>> lines of the fram <<<-------///////////////
	pWind->DrawLine(cx + radius, cy, cx + radius + 4, cy, FRAME); //right
	pWind->DrawLine(cx, cy + radius, cx, cy + radius + 4, FRAME); //lower
	pWind->DrawLine(cx, cy - radius, cx, cy - radius - 4, FRAME); //upper
																  //right up
	pWind->DrawLine(cx + radius*(1 / sqrt(2)), cy - radius*(1 / sqrt(2)), cx + (radius + 4)*(1 / sqrt(2)), cy - (radius + 4)*(1 / sqrt(2)), FRAME);
	//right down
	pWind->DrawLine(cx + radius*(1 / sqrt(2)), cy + radius*(1 / sqrt(2)), cx + (radius + 4)*(1 / sqrt(2)), cy + (radius + 4)*(1 / sqrt(2)), FRAME);
	//left up
	pWind->DrawLine(cx - radius*(1 / sqrt(2)), cy - radius*(1 / sqrt(2)), cx - (radius + 4)*(1 / sqrt(2)), cy - (radius + 4)*(1 / sqrt(2)), FRAME);
	//left down
	pWind->DrawLine(cx - radius*(1 / sqrt(2)), cy + radius*(1 / sqrt(2)), cx - (radius + 4)*(1 / sqrt(2)), cy + (radius + 4)*(1 / sqrt(2)), FRAME);

}
void Output::DrawSwtich(GraphicsInfo g, bool isON, bool highlighted, bool notValid) const
{
	if (highlighted) pWind->SetPen(BLUE); else if (notValid) pWind->SetPen(RED);
	else pWind->SetPen(BROWN);
	int cx = g.x1, cy = g.y1; //Centre Points
							  //the rectangle
	if (isON)
	{
		pWind->DrawImage("images\\Menu\\SWITCH_ON.jpg", cx - 16, cy - 16, 32, 32);
	}
	else
	{
		pWind->DrawImage("images\\Menu\\SWITCH_OFF.jpg", cx - 16, cy - 16, 32, 32);
	}
	//the output line
	pWind->DrawLine(cx + 12, cy, cx + 22, cy);




}
void Output::setUsedPixel(int i, int j, CellType c) {
	usedPixels[i][j] = c;
}
void Output::setAllPixels(int i, int j, Component* comp) {
	allPixels[i][j] = comp;
}

Component * Output::getAllPixels(int i, int j)
{
	return allPixels[i][j];
}

Output::~Output()
{
	delete pWind;
}

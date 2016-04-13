#include "Output.h"
#include "..\Utils.h"
#include <vector>
#include <queue>
#include <cstring>
using namespace std;
CellType Output::usedPixels[44][74];
int arrayOfIntersections[44][74];
Output::Output()
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
void Output::DrawGrid()const{
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

bool Output::DrawString(string s, GraphicsInfo Gfx_info) const{
	pWind->DrawString(Gfx_info.x1, Gfx_info.y1, s);
	return true;
}

//======================================================================================//
//								Components Drawing Functions							//
//======================================================================================//


bfs_node* Output::bfs(bfs_node* bf, int requX, int requY, vector<bfs_node*> allNodes) const{

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
		if (tmp->x == requX && tmp->y == requY){
			return tmp;
		}
		if (Utils::CheckPointForConnections((tmp->x + 1)* UI.GRID_SIZE, tmp->y * UI.GRID_SIZE, usedPixels) && tmp->x + 1 <= 74 && vis[tmp->y][tmp->x + 1] < 0 && Output::usedPixels[tmp->y][tmp->x + 1] != INTERSECTION && (Output::usedPixels[tmp->y][tmp->x + 1] == EMPTY || Output::usedPixels[tmp->y][tmp->x + 1] == PIN || (Output::usedPixels[tmp->y][tmp->x + 1] == VERTICAL && Output::usedPixels[tmp->y][tmp->x + 1] != END_CONNECTION)))
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
		if (Utils::CheckPointForConnections((tmp->x)* UI.GRID_SIZE, (tmp->y + 1) * UI.GRID_SIZE, usedPixels) && tmp->y + 1 <= 44 && vis[tmp->y + 1][tmp->x] < 0 && Output::usedPixels[tmp->y + 1][tmp->x] != INTERSECTION && (Output::usedPixels[tmp->y + 1][tmp->x] == EMPTY || (Output::usedPixels[tmp->y + 1][tmp->x] == HORIZONTAL && Output::usedPixels[tmp->y + 1][tmp->x] != END_CONNECTION)))
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

		if (Utils::CheckPointForConnections((tmp->x)* UI.GRID_SIZE, (tmp->y - 1) * UI.GRID_SIZE, usedPixels) && tmp->y - 1 >= 0 && vis[tmp->y - 1][tmp->x] < 0 && Output::usedPixels[tmp->y - 1][tmp->x] != INTERSECTION && (Output::usedPixels[tmp->y - 1][tmp->x] == EMPTY || (Output::usedPixels[tmp->y - 1][tmp->x] == HORIZONTAL && Output::usedPixels[tmp->y - 1][tmp->x] != END_CONNECTION)))
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
bool Output::DrawConnection(GraphicsInfo GfxInfo, int inputPin, GraphicsInfo compCenterLocation, bool selected) const
{
	vector<bfs_node*> allNodes;
	bfs_node* current = new bfs_node;
	GraphicsInfo* outPut = Utils::getOutputDirections(GfxInfo, usedPixels, 3);
	bool availableConnection = false;
	bool freeOutputCells[] = { 0, 0, 0 };
	for (size_t i = 0; i < 3; i++)
	{
		if (usedPixels[outPut[i].y1 / UI.GRID_SIZE][outPut[i].x1 / UI.GRID_SIZE] == EMPTY){
			current->x = outPut[i].x1 / UI.GRID_SIZE;
			current->y = outPut[i].y1 / UI.GRID_SIZE;
			availableConnection = true;
			freeOutputCells[i] = 1;
		}
	}
	for (size_t i = 0; i < 3; i++)
	{
		if (inputPin == i && freeOutputCells[i])
		{
			current->x = outPut[i].x1 / UI.GRID_SIZE;
			current->y = outPut[i].y1 / UI.GRID_SIZE;
			break;
		}
	}
	
	int remindX2 = GfxInfo.x2 % UI.GRID_SIZE;
	int remindY2 = GfxInfo.y2 % UI.GRID_SIZE;

	GfxInfo.x2 = compCenterLocation.x1 - UI.GATE_Width / 2 - 2;

	int destX = GfxInfo.x2 / UI.GRID_SIZE + ((remindX2 > UI.GRID_SIZE / 2) ? 1 : 0);
	int destY;
	if (inputPin == 0)
	{
		destY = compCenterLocation.y1 / UI.GRID_SIZE - 1;
	}
	else if (inputPin == 1){
		destY = compCenterLocation.y1 / UI.GRID_SIZE;
	}
	else{
		destY = compCenterLocation.y1 / UI.GRID_SIZE + 1;
	}

	bfs_node* target = bfs(current, destX, destY, allNodes);

	if (!availableConnection || target == NULL)
	{
		pWind->FlushMouseQueue();

		return false;
	}
	if (!selected)pWind->SetPen(color(23, 79, 181), 2);	else pWind->SetPen(BLUE, 2);

	if (inputPin == 0)
	{
		pWind->DrawLine(destX * UI.GRID_SIZE, destY* UI.GRID_SIZE, compCenterLocation.x1 - UI.GATE_Width / 2 + 3, compCenterLocation.y1 - 13);
	}
	else if (inputPin == 1){
		pWind->DrawLine(destX * UI.GRID_SIZE - 1, compCenterLocation.y1, compCenterLocation.x1 - UI.GATE_Width / 2 + 3, compCenterLocation.y1);
	}
	else{
		pWind->DrawLine(destX * UI.GRID_SIZE, destY* UI.GRID_SIZE, compCenterLocation.x1 - UI.GATE_Width / 2 + 3, compCenterLocation.y1 + 13);
	}

	

	//Draw small lines

	pWind->DrawLine(current->x * UI.GRID_SIZE, current->y * UI.GRID_SIZE, GfxInfo.x1, GfxInfo.y1);

	bfs_node* parent = target->parent;
	bool draw = true, skip_next = false, PreviousIsIntersection = false;
	int i = 0;
	while (parent != NULL){

		if (draw && !skip_next){
			if (target->x == parent->x && arrayOfIntersections[parent->y][parent->x] == 1)
			{
				usedPixels[parent->y][parent->x] = INTERSECTION;
				if (parent->y < target->y){
					if (!PreviousIsIntersection)
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 10);
					else{
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 6, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 10);
					}
					pWind->SetPen(color(14, 36, 75), 2);
					pWind->DrawBezier(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 10, target->x * UI.GRID_SIZE + 6, target->y*UI.GRID_SIZE - 14, target->x * UI.GRID_SIZE + 8, target->y*UI.GRID_SIZE - 18, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 22);
					if (!selected)pWind->SetPen(color(23, 79, 181), 2);	else pWind->SetPen(BLUE, 2);
					if (parent->parent != NULL && arrayOfIntersections[parent->parent->y][parent->parent->x] != 1)
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 22, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 32);
				}
				else{
					if (!PreviousIsIntersection){
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 10);
					}
					else{
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
			if (target->y == parent->y && arrayOfIntersections[parent->y][parent->x] == 0){
				usedPixels[parent->y][parent->x] = INTERSECTION;
				if (parent->x < target->x){
					if (!PreviousIsIntersection)
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 10, target->y*UI.GRID_SIZE);
					else{
						pWind->DrawLine(target->x * UI.GRID_SIZE - 6, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 10, target->y*UI.GRID_SIZE);
					}
					pWind->SetPen(color(14, 36, 75), 2);
					pWind->DrawBezier(target->x * UI.GRID_SIZE - 10, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 14, target->y*UI.GRID_SIZE - 6, target->x * UI.GRID_SIZE - 18, target->y*UI.GRID_SIZE - 8, target->x * UI.GRID_SIZE - 22, target->y*UI.GRID_SIZE);
					if (!selected)pWind->SetPen(color(23, 79, 181), 2);	else pWind->SetPen(BLUE, 2);
					if (parent->parent != NULL && arrayOfIntersections[parent->parent->y][parent->parent->x] != 0)
						pWind->DrawLine(target->x * UI.GRID_SIZE - 22, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 32, target->y*UI.GRID_SIZE);
				}
				else{
					if (!PreviousIsIntersection)
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE + 10, target->y*UI.GRID_SIZE);
					else{
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
			else{
				pWind->DrawLine(target->x*UI.GRID_SIZE, target->y*UI.GRID_SIZE, parent->x*UI.GRID_SIZE, parent->y*UI.GRID_SIZE);

				PreviousIsIntersection = false;

			}
			if (target->x == parent->x)
			{
				if (target->y > parent->y)
				{
					for (size_t i = target->y; i >= parent->y; i--)
					{
						if (usedPixels[i][target->x] != INTERSECTION)
						{
							usedPixels[i][target->x] = VERTICAL;
						}
					}
				}
				else{
					for (size_t i = target->y; i <= parent->y; i++)
					{
						if (usedPixels[i][target->x] != INTERSECTION)
						{
							usedPixels[i][target->x] = VERTICAL;
						}
					}
				}

			}
			else if (target->y == parent->y){
				if (target->x > parent->x)
				{
					for (int i = target->x; i >= parent->x; i--)
					{
						if (usedPixels[target->y][i] != INTERSECTION)
						{
							usedPixels[target->y][i] = HORIZONTAL;
						}
					}
				}
				else{
					for (int i = target->x; i <= parent->x; i++)
					{
						if (usedPixels[target->y][i] != INTERSECTION)
						{
							usedPixels[target->y][i] = HORIZONTAL;
						}
					}
				}
			}

			if (parent->parent != NULL && ((target->x == parent->x && parent->parent->x != parent->x) || (target->y == parent->y && parent->parent->y != parent->y)))
			{
				usedPixels[parent->y][parent->x] = INTERSECTION;
			}
			if (parent->parent == NULL)
			{
				usedPixels[parent->y][parent->x] = END_CONNECTION;
			}
			if (i == 0)
			{
				usedPixels[target->y][target->x] = END_CONNECTION;
			}
		}
		else{
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
	return true;
}
void Output::DrawCleanImage(image* img, int x, int y)
{
	pWind->DrawImage(img, x - UI.GATE_Width / 2 + 1, y - UI.GRID_HEIGHT / 2, UI.GATE_Width - 1, UI.GATE_Height - 5);
}
bool Output::SetDragImage(ActionType ActType, GraphicsInfo& GfxInfo, image* smallCleanImageBeforeAddingGate, bool moving){
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
	else{
		PrintMsg("Please select point within workspace and avoid overlaping!");
	}

	bool draw = true, isOriginalDrawn = false;

	image* storedImg = new image;
	image* storedDrawingImg = new image;
	pWind->StoreImage(storedImg, 0, 0, pWind->GetWidth(), pWind->GetHeight());
	pWind->StoreImage(storedDrawingImg, 0, UI.ToolBarHeight, pWind->GetWidth(), pWind->GetHeight() - UI.StatusBarHeight);

	pWind->SetPen(BLUE, 2);
	while (true)
	{
		int x, y;
		pWind->GetMouseCoord(x, y);
		bool wrong = false;
		Utils::correctPointClicked(x, y, true, false);
		GraphicsInfo tmpGraphicsInfo; tmpGraphicsInfo.x1 = x; tmpGraphicsInfo.y1 = y;
		if (!Utils::CheckPoint(tmpGraphicsInfo, usedPixels, false)){
			wrong = true;
		}
		else{
			wrong = false;
		}
		if (Utils::CheckPoint(x, y, usedPixels)){
			if (x != iXOld || y != iYOld){
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

				pWind->StoreImage(smallCleanImageBeforeAddingGate, Gfx.x1 - UI.GATE_Width / 2 + 1, Gfx.y1 - UI.GATE_Height / 2 + 1, UI.GATE_Width - 1, UI.GATE_Height - 1);

				switch (ActType){
				case ADD_Buff:{
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
				case ADD_Buff_H:{
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
				pWind->UpdateBuffer();
			}
		}
		if (!moving && pWind->GetKeyPress(cKeyData) == ESCAPE){
			pWind->DrawImage(storedImg, 0, 0, pWind->GetWidth(), pWind->GetHeight());
			draw = false;
			break;
		}
		else if (pWind->GetMouseClick(GfxInfo.x1, GfxInfo.y1) == LEFT_CLICK || (moving && (pWind->GetButtonState(LEFT_BUTTON, GfxInfo.x1, GfxInfo.y1) == BUTTON_UP))){
			pWind->FlushMouseQueue();
			Utils::correctPointClicked(GfxInfo.x1, GfxInfo.y1, true, false);
			if (Utils::CheckPoint(GfxInfo, usedPixels)){
				draw = true;
				break;
			}
			else{
				draw = false;
			}
		}
	}
	/*for (size_t i = 0; i < 44; i++)
	{
		for (size_t j = 0; j < 74; j++)
		{
			cout << usedPixels[i][j] << " ";
		}
		cout << endl;
	}*/
	pWind->FlushMouseQueue();
	PrintMsg("");
	delete storedDrawingImg;
	delete storedImg;
	return draw;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//You Send a Centre Point (cx,cy) ,this means when you call Draw image Function , x and y sent should be cx-24, cy-24
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Output::DrawAnd_Nand(GraphicsInfo g, int in, bool isNand, bool highlighted, bool notValid) const{

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

	if (isNand){
		outx = cx + 14 - (2 * ciDefBrushSize);
		p1x = p2x = p1x - 5;
		//Darwing lines
		pWind->DrawLine(p1x, p1y, p1x + 8, p1y, FRAME);
		pWind->DrawLine(p2x, p2y, p2x + 8, p2y, FRAME);
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x - 6, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x - 6, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 3 * ciDefBrushSize - 1, outy, outx + 7 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		pWind->DrawLine(p1x, p1y - 1, p2x, p2y, FRAME);
		if (in == 3){ //Checking for 3 input Gate 
			int in3x = in1x, in3y = cy;
			pWind->DrawLine(in3x - 6 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
		}
		//Drawing Arc
		pWind->DrawBezier(p1x + 8, p1y, p1x + 10 + 3 + 4, p1y + 4, p1x + 10 + 9 + 4, p1y + 12, outx, outy, FRAME);
		pWind->DrawBezier(p2x + 8, p2y, p2x + 10 + 3 + 4, p2y - 4, p2x + 10 + 9 + 4, p2y - 12, outx, outy, FRAME);

		//Drawing Buuble
		pWind->DrawCircle(outx - ciDefBrushSize + 2 * ciDefBrushSize, outy, 2 * ciDefBrushSize, FRAME);

	}
	else{
		outx = cx + 15 - (2 * ciDefBrushSize);
		//Darwing lines
		pWind->DrawLine(p1x, p1y, p1x + 8, p1y, FRAME);
		pWind->DrawLine(p2x, p2y, p2x + 8, p2y, FRAME);
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 3, outy, outx + 6 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		pWind->DrawLine(p1x, p1y - 1, p2x, p2y, FRAME);
		if (in == 3){ //Checking for 3 input Gate 
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

void Output::DrawNot_Buffer(GraphicsInfo g, bool isBuffer, bool highlighted, bool notValid) const{
	int cx = g.x1, cy = g.y1; //Centre Point
	int p1x, p1y, p2x, p2y, inx, iny, outx, outy; //Vertices of Triangle and input/output Points

	p1x = p2x = inx = cx - 15 + (2 * ciDefBrushSize);
	p1y = cy + 21; p2y = cy - 21;
	iny = outy = cy;

	if (highlighted) pWind->SetPen(BLUE); else if (notValid) pWind->SetPen(RED);
	else if (notValid) pWind->SetPen(RED);
	else pWind->SetPen(BROWN);

	if (isBuffer){
		//BUFFER Gate
		outx = cx + 15 - (2 * ciDefBrushSize);
		//Drawing lines 
		pWind->DrawLine(inx - 6 - (2 * ciDefBrushSize), iny, inx, iny, FRAME); //The Input-Line
		pWind->DrawLine(outx, outy, outx + 6 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		//Drawing Trianlge
		pWind->DrawTriangle(p1x, p1y, p2x, p2y, outx, outy, FRAME);
	}
	else{
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

void Output::DrawOr_Nor(GraphicsInfo g, int in, bool isNor, bool highlighted, bool notValid) const{
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

	if (isNor){
		//NOR GATE
		outy = cy; outx = cx + 14 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;

		//Draw lines
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 4 * ciDefBrushSize - 1, outy, outx + 7 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		if (in == 3){ //Checking for 3 input Gate 
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

	else{
		//OR GATE
		outx = cx + 15 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;
		//Draw lines
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx, outy, outx + 6 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		if (in == 3){ //Checking for 3 input Gate 
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

	if (isXNor){
		//XNOR GATE
		outy = cy; outx = cx + 14 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;
		//Draw lines
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 4 * ciDefBrushSize - 1, outy, outx + 7 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		if (in == 3){ //Checking for 3 input Gate 
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
	else{
		//XOR GATE
		outy = cy; outx = cx + 15 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;
		//Draw lines
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx, outy, outx + 6 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		if (in == 3){ //Checking for 3 input Gate 
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
void Output::setUsedPixel(int i, int j, CellType c){
	usedPixels[i][j] = c;
}
Output::~Output()
{
	delete pWind;
}

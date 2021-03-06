#include "Output.h"
#include "..\Utils.h"
#include <vector>
#include <queue>
#include <cstring>
#include <iostream>
#include <algorithm>
#include "..\Components\Connection.h"
#include "..\Components\Gate.h"
using namespace std;
CellType Output::usedPixels[44][74];
int arrayOfIntersections[44][74]; // array of connections intersections
int arrayOfCorners[44][74]; // array of connections corners
int connectionsCountAtPixel[44][74]; // array of connections count at every pixel (2 if there's intersection, 1 if there's not)

									 //the Timer doen't allow passing a memberfunction pointer to it unless it's a static memberFunction
void DRAWAFTERMENUE(Output* pOut, HWND D) {
	pOut->pWind->setActive();
	CloseWindow(D);
	SendMessage(D, WM_CLOSE, 0, 0);
}
Output::Output(vector<Connection*>* allConns)
{
	//Initialize user interface parameters

	UI.AppMode = DESIGN;	//Design Mode is the startup mode

							//Initilaize interface colors
	UI.DrawColor = BROWN;
	UI.SelectColor = BLUE;
	UI.ConnColor = BROWN;
	UI.HumpColor = BLACK;
	UI.SelectedHumpColor = DARKBLUE;// color(23, 79, 181);

	UI.OneValueConnection = color(0, 200, 0);
	UI.OneValueHumpColor = color(0, 175, 0);

	UI.MsgColor = BLUE;
	UI.ErrorColor = RED;
	UI.BkGrndColor = WHITE;

	//Create the drawing window
	pWind = CreateWind(UI.width, UI.height, UI.wx, UI.wy);
	ChangeTitle("Programming Techniques Project");

	DrawGrid();
	CreateLeftToolBar();
	CreateTopToolBar();

	//Reseting all arrays
	memset(usedPixels, 0, sizeof usedPixels);
	memset(arrayOfIntersections, -1, sizeof arrayOfIntersections);
	memset(arrayOfCorners, 0, sizeof arrayOfCorners);
	memset(connectionsCountAtPixel, 0, sizeof connectionsCountAtPixel);
	memset(Utils::arrayOfComponents, NULL, sizeof Utils::arrayOfComponents);

	this->allConnectionsPointer = allConns;
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
//Clears the drawing (degin) area:: Not needed
void Output::ClearDrawingArea() const
{
	//pWind->SetPen(RED, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, 0, UI.width, UI.height);
	DrawGrid();
	CreateLeftToolBar();
	CreateTopToolBar();
}
//////////////////////////////////////////////////////////////////////////////////////////
//Draw the left bar
void Output::CreateLeftToolBar() const
{
	UI.AppMode = DESIGN;
	pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
}
void Output::CreateTopToolBar() const
{
	pWind->DrawImage("images\\Menu\\top_bar_normal.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight);

}
//////////////////////////////////////////////////////////////////////////////////////////
bool Output::DrawString(string s, GraphicsInfo Gfx_info) const {
	pWind->DrawString(Gfx_info.x1, Gfx_info.y1, s);
	return true;
}

bfs_node* Output::bfs(bfs_node* bf, int requX, int requY, vector<bfs_node*> allNodes) const {
	if (bf == NULL || bf->y>44 || bf->x > 74)
	{
		return NULL;
	}
	int vis[46][74];
	memset(vis, -1, sizeof vis); // reset the visited array
	queue<bfs_node*> q;
	bfs_node* tmp;
	vis[bf->y][bf->x] = 0;
	bf->parent = NULL;
	q.push(bf);
	while (!q.empty())
	{
		tmp = q.front();
		q.pop();
		if (tmp->x == requX && tmp->y == requY) { // check if reached required
			return tmp;
		}
		if (Utils::CheckPointInBorders((tmp->x + 1)* UI.GRID_SIZE, tmp->y * UI.GRID_SIZE) && tmp->x + 1 <= 74 && vis[tmp->y][tmp->x + 1] < 0 && Output::usedPixels[tmp->y][tmp->x + 1] != INTERSECTION && (Output::usedPixels[tmp->y][tmp->x + 1] == EMPTY || ((Output::usedPixels[tmp->y][tmp->x] == EMPTY || Output::usedPixels[tmp->y][tmp->x] == PIN || Output::usedPixels[tmp->y][tmp->x] == VERTICAL) && ((tmp->y == requY && tmp->x + 1 == requX - 1 && Output::usedPixels[tmp->y][tmp->x + 1] == VERTICAL && Output::usedPixels[tmp->y][tmp->x + 2] == PIN) || (tmp->y == requY && tmp->x + 1 == requX && Output::usedPixels[tmp->y][tmp->x + 1] == PIN))) || (Output::usedPixels[tmp->y][tmp->x + 1] == VERTICAL && Output::usedPixels[tmp->y][tmp->x + 1] != END_CONNECTION)))
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
		if (Utils::CheckPointInBorders((tmp->x)* UI.GRID_SIZE, (tmp->y + 1) * UI.GRID_SIZE) && tmp->y + 1 <= 44 && vis[tmp->y + 1][tmp->x] < 0 && Output::usedPixels[tmp->y + 1][tmp->x] != INTERSECTION && (Output::usedPixels[tmp->y + 1][tmp->x] == EMPTY || ((Output::usedPixels[tmp->y][tmp->x] == EMPTY || Output::usedPixels[tmp->y][tmp->x] == HORIZONTAL) && tmp->y + 1 == requY && tmp->x == requX &&  Output::usedPixels[tmp->y + 1][tmp->x] == PIN) || (abs((tmp->y + 1) - bf->y) == 1 && tmp->x == bf->x &&  Output::usedPixels[tmp->y + 1][tmp->x] == PIN) || (Output::usedPixels[tmp->y + 1][tmp->x] == HORIZONTAL && Output::usedPixels[tmp->y + 1][tmp->x] != END_CONNECTION)))
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
		if (Utils::CheckPointInBorders((tmp->x - 1)* UI.GRID_SIZE, tmp->y * UI.GRID_SIZE) && tmp->x - 1 >= 0 && vis[tmp->y][tmp->x - 1] < 0 && Output::usedPixels[tmp->y][tmp->x - 1] != INTERSECTION && (Output::usedPixels[tmp->y][tmp->x - 1] == EMPTY || (Output::usedPixels[tmp->y][tmp->x - 1] == VERTICAL && Output::usedPixels[tmp->y][tmp->x - 1] != END_CONNECTION)))
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

		if (Utils::CheckPointInBorders((tmp->x)* UI.GRID_SIZE, (tmp->y - 1) * UI.GRID_SIZE) && tmp->y - 1 >= 0 && vis[tmp->y - 1][tmp->x] < 0 && Output::usedPixels[tmp->y - 1][tmp->x] != INTERSECTION && (Output::usedPixels[tmp->y - 1][tmp->x] == EMPTY || ((Output::usedPixels[tmp->y][tmp->x] == EMPTY || Output::usedPixels[tmp->y][tmp->x] == HORIZONTAL) && tmp->y - 1 == requY && tmp->x == requX &&  Output::usedPixels[tmp->y - 1][tmp->x] == PIN) || (abs((tmp->y - 1) - bf->y) == 1 && tmp->x == bf->x &&  Output::usedPixels[tmp->y - 1][tmp->x] == PIN) || (Output::usedPixels[tmp->y - 1][tmp->x] == HORIZONTAL && Output::usedPixels[tmp->y - 1][tmp->x] != END_CONNECTION)))
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

bool Output::DrawConnection(GraphicsInfo GfxInfo, int inputPin, GraphicsInfo compCenterLocation, vector<Cell>& cellsBeforeAddingConnection, bool selected, Component* pA)
{
	cellsBeforeAddingConnection.clear(); // vector to hold status before adding the connection
	vector<bfs_node*> allNodes; // store all nodes in order to deallocate them
	bfs_node* current = new bfs_node;

	//get start point
	current->x = (GfxInfo.x1 - (GfxInfo.x1 % UI.GRID_SIZE)) / UI.GRID_SIZE;
	current->y = GfxInfo.y1 / UI.GRID_SIZE;

	int remindX2 = GfxInfo.x2 % UI.GRID_SIZE;

	GfxInfo.x2 = compCenterLocation.x1 - UI.GATE_Width / 2 - 2;

	//get destination points depending on inpit pin
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
	//Find the path
	bfs_node* target = bfs(current, destX, destY, allNodes);

	if (target == NULL) // check if there exists path or not
	{

		pWind->FlushMouseQueue();

		return false;
	}
	// set colors
	if (!selected)pWind->SetPen(UI.ConnColor, 2);	else pWind->SetPen(UI.SelectColor, 2);

	//Draw small lines which connects the end of the connection to the pin
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
	bool draw = true, skip_next = false, PreviousIsIntersection = false; // boolean to check if previous point was intersection
	int i = 0;
	while (parent != NULL) { // start drawing the path
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
		connectionsCountAtPixel[target->y][target->x]++; // increase connection count at this pixel
		if (draw && !skip_next) {
			if (target->x == parent->x && arrayOfIntersections[parent->y][parent->x] == 1) // draw vertical line - curve - vertical line (in case of vertical intersection)
			{
				usedPixels[parent->y][parent->x] = INTERSECTION;
				if (parent->y < target->y) {
					// Check if previous intersection or not to know where to start drawing 
					if (!PreviousIsIntersection)
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 10);
					else {
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 6, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 10);
					}
					if (!selected)pWind->SetPen(UI.HumpColor, 2);	else pWind->SetPen(UI.SelectedHumpColor, 2);
					pWind->DrawBezier(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 10, target->x * UI.GRID_SIZE + 6, target->y*UI.GRID_SIZE - 14, target->x * UI.GRID_SIZE + 8, target->y*UI.GRID_SIZE - 18, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 22);
					if (!selected)pWind->SetPen(UI.ConnColor, 2);	else pWind->SetPen(BLUE, 2);
					if (parent->parent != NULL && arrayOfIntersections[parent->parent->y][parent->parent->x] != 1)
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 22, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE - 32);
				}
				else { // same as the previous case but when going in the opposite direction
					if (!PreviousIsIntersection) {
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 10);
					}
					else {
						pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 6, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 10);
					}
					if (!selected)pWind->SetPen(UI.HumpColor, 2);	else pWind->SetPen(UI.SelectedHumpColor, 2);
					pWind->DrawBezier(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 10, target->x * UI.GRID_SIZE + 6, target->y*UI.GRID_SIZE + 14, target->x * UI.GRID_SIZE + 8, target->y*UI.GRID_SIZE + 18, target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE + 22);
					if (!selected)pWind->SetPen(UI.ConnColor, 2);	else pWind->SetPen(BLUE, 2);
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
				if (target->y == parent->y && arrayOfIntersections[parent->y][parent->x] == 0) { // Horiz. line with intersection
					usedPixels[parent->y][parent->x] = INTERSECTION;
					if (parent->x < target->x) {
						if (!PreviousIsIntersection)
							pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 10, target->y*UI.GRID_SIZE);
						else {
							pWind->DrawLine(target->x * UI.GRID_SIZE - 6, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 10, target->y*UI.GRID_SIZE);
						}
						if (!selected)pWind->SetPen(UI.HumpColor, 2);	else pWind->SetPen(UI.SelectedHumpColor, 2);
						pWind->DrawBezier(target->x * UI.GRID_SIZE - 10, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 14, target->y*UI.GRID_SIZE - 6, target->x * UI.GRID_SIZE - 18, target->y*UI.GRID_SIZE - 8, target->x * UI.GRID_SIZE - 22, target->y*UI.GRID_SIZE);
						if (!selected)pWind->SetPen(UI.ConnColor, 2);	else pWind->SetPen(BLUE, 2);
						if (parent->parent != NULL && arrayOfIntersections[parent->parent->y][parent->parent->x] != 0)
							pWind->DrawLine(target->x * UI.GRID_SIZE - 22, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE - 32, target->y*UI.GRID_SIZE);
					}
					else { // Same as above but in the opposite direction
						if (!PreviousIsIntersection)
							pWind->DrawLine(target->x * UI.GRID_SIZE, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE + 10, target->y*UI.GRID_SIZE);
						else {
							pWind->DrawLine(target->x * UI.GRID_SIZE + 6, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE + 10, target->y*UI.GRID_SIZE);
						}
						if (!selected)pWind->SetPen(UI.HumpColor, 2);	else pWind->SetPen(UI.SelectedHumpColor, 2);
						pWind->DrawBezier(target->x * UI.GRID_SIZE + 10, target->y*UI.GRID_SIZE, target->x * UI.GRID_SIZE + 14, target->y*UI.GRID_SIZE - 6, target->x * UI.GRID_SIZE + 18, target->y*UI.GRID_SIZE - 8, target->x * UI.GRID_SIZE + 22, target->y*UI.GRID_SIZE);
						if (!selected)pWind->SetPen(UI.ConnColor, 2);	else pWind->SetPen(BLUE, 2);
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
				if (i != 0) // fill the array
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
				//Fill array of corners
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

	// Fill arrays
	if (pA != NULL)
	{
		for (size_t i = 0; i < cellsBeforeAddingConnection.size(); i++)
		{
			Utils::setArrayOfComponents(cellsBeforeAddingConnection[i].y, cellsBeforeAddingConnection[i].x, pA);
		}
	}
	// deallocate all nodes to free memory
	for (size_t i = 0; i < allNodes.size(); i++)
	{
		bfs_node* tmp = allNodes[i];
		delete tmp;
		tmp = NULL;
	}
	pWind->FlushMouseQueue();

	return true;
}
// Draw right click menu
void Output::DrawRClickMenu_CorrectPoints(int& x, int& y, int type, bool draw)
{
	string imageURL;
	if (draw)
		switch (type) {
		case 1: //The Gate SubMenu
			imageURL = "images\\Menu\\RightClickMenu.jpg";
			break;
		case 2: //The connection menu
			imageURL = "images\\Menu\\RightClickMenuConnection.jpg";
			break;
		case 3: //The Space Menu
			imageURL = "images\\Menu\\RightClickMenuSpace.jpg";
			break;
		case 5: //The MultiDelete Menu
			imageURL = "images\\Menu\\RightClickMultiDelete.jpg";
			break;
		default:
			break;
		}
	int Height;
	switch (type)
	{
	case 1:
		Height = UI.RightClickMenuHeight;
		break;
	case 2:
		Height = UI.RightClickCMenuH + UI.RightClickCMenuH / 2;
		break;
	case 3:
		Height = UI.RightClickCMenuH / 2;
		break;
	case 5:
		Height = UI.RightClickCMenuH / 2;
		break;
	}
	//If Point (x,y) in the Right or Middle of the Drawing Area
	if (x + UI.RightClickMenuLength < UI.width) {
		if (y + Height > UI.height - UI.StatusBarHeight) {
			// y must be corrected
			y = y - Height;
			if (draw)
				pWind->DrawImage(imageURL, x, y, UI.RightClickMenuLength, Height);
		}
		else {
			//The Point(x,y) is ready for drawing Menu 
			if (draw)
				pWind->DrawImage(imageURL, x, y, UI.RightClickMenuLength, Height);
		}
	}
	//If Point (x,y) in the Right of the Drawing Area
	if (x + UI.RightClickMenuLength > UI.width) {
		// x must be corrected
		x = UI.width - UI.RightClickMenuLength - 40;
		if (y + Height > UI.height - UI.StatusBarHeight) {
			// y must be corrected
			y = y - Height;
			if (draw)
				pWind->DrawImage(imageURL, x, y, UI.RightClickMenuLength, Height);
		}
		else {
			//The Point(x,y) is ready for drawing Menu 
			if (draw)
				pWind->DrawImage(imageURL, x, y, UI.RightClickMenuLength, Height);
		}
	}
}

// Draw clean image in the place of a gate in order to delete it from the array and UI 
void Output::DrawCleanImage(image* img, int x, int y, bool emptyArray)
{
	if (emptyArray)
	{
		GraphicsInfo GInfotmp;
		GInfotmp.x1 = x - UI.GATE_Width / 2;
		GInfotmp.x2 = x + UI.GATE_Width / 2;
		GInfotmp.y1 = y - UI.GATE_Height / 2;
		GInfotmp.y2 = y + UI.GATE_Height / 2;
		for (int i = GInfotmp.y1 / UI.GRID_SIZE + 1; i <= GInfotmp.y2 / UI.GRID_SIZE; i++) {
			for (int j = GInfotmp.x1 / UI.GRID_SIZE; j <= GInfotmp.x2 / UI.GRID_SIZE; j++) {
				Utils::setArrayOfComponents(i, j, NULL);
				setUsedPixel(i, j, EMPTY);
			}
		}
	}
	pWind->DrawImage(img, x - UI.GRID_SIZE - 5, y - UI.GRID_SIZE - 5, 2 * UI.GRID_SIZE + 3, UI.GATE_Height + 3);
	pWind->SetPen(WHITE);
	pWind->SetBrush(WHITE);
	pWind->DrawPixel(x - UI.GRID_SIZE - 7 + (2 * UI.GRID_SIZE + 5) + 1, y - UI.GRID_SIZE - 9 + (UI.GATE_Height + 3) / 2);
	pWind->DrawPixel(x - UI.GRID_SIZE - 7 + (2 * UI.GRID_SIZE + 5) + 1, y - UI.GRID_SIZE - 10 + (UI.GATE_Height + 3) / 2);
	pWind->DrawPixel(x - UI.GRID_SIZE - 7 + (2 * UI.GRID_SIZE + 4) + 1, y - UI.GRID_SIZE - 9 + (UI.GATE_Height + 3) / 2);
	pWind->DrawPixel(x - UI.GRID_SIZE - 7 + (2 * UI.GRID_SIZE + 4) + 1, y - UI.GRID_SIZE - 10 + (UI.GATE_Height + 3) / 2);
}
void Output::storeDrawingAreaImage(image*& img) {
	pWind->StoreImage(img, UI.LeftToolBarWidth, 0, pWind->GetWidth() - UI.LeftToolBarWidth, pWind->GetHeight() - UI.StatusBarHeight);
}
void Output::drawStoredDrawingAreaImage(image*& img) {
	pWind->DrawImage(img, UI.LeftToolBarWidth, 0, pWind->GetWidth() - UI.LeftToolBarWidth, pWind->GetHeight() - UI.StatusBarHeight);
}

void Output::drawRectangle(int x1, int y1, int x2, int y2) {
	pWind->DrawRectangle(x1, y1, x2, y2, FRAME);
}

//Switch app mode
void Output::switchMode(MODE appMode)
{
	if (appMode == DESIGN)
	{
		pWind->DrawImage("images\\Menu\\top_bar_normal.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight);
		pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);

	}
	else {
		pWind->DrawImage("images\\Menu\\top_bar_simulate.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight);
		pWind->DrawImage("images\\Menu\\left_bar_simulation_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);

	}
}
//Draw menus
void Output::DrawWarningMenues(char type, int x, int y)
{
	int xbegin, xend, ybegin, yend;
	xbegin = UI.width / 2 - UI.WarningMenuWidth / 2;
	ybegin = UI.height / 2 - UI.WarningMenuHeight / 2;
	string imageURL;
	switch (type) {
	case 'L':
		imageURL = "images\\Menu\\LoadConfirmationMenu.jpg";
		pWind->DrawImage(imageURL, xbegin, ybegin, UI.WarningMenuWidth, UI.WarningMenuHeight);
		break;
	case 'S':
	{
		imageURL = "images\\Menu\\statusBar.jpg";
		pWind->DrawImage(imageURL, x, y, UI.StatusBoxWidth, UI.StatusBoxHeight);
		break;
	}
	}
}

image* Output::StoreBeforeWarning()
{
	int xbegin, xend, ybegin, yend;
	xbegin = UI.width / 2 - UI.WarningMenuWidth / 2;
	ybegin = UI.height / 2 - UI.WarningMenuHeight / 2;
	image * ptr = new image;
	pWind->StoreImage(ptr, xbegin, ybegin, UI.WarningMenuWidth, UI.WarningMenuHeight);
	return ptr;
}

void Output::DrawAfterWarning(image * theWarningImage)
{
	int xbegin = 0, ybegin = 0;
	xbegin = UI.width / 2 - UI.WarningMenuWidth / 2;
	ybegin = UI.height / 2 - UI.WarningMenuHeight / 2;
	pWind->DrawImage(theWarningImage, xbegin, ybegin, UI.WarningMenuWidth, UI.WarningMenuHeight);
}
image * Output::StoreBeforeMenu(int x, int y, int type, int w)
{
	int Height;
	switch (type)
	{
	case 1:
		Height = UI.RightClickMenuHeight;
		break;
	case 2:
		Height = UI.RightClickCMenuH + UI.RightClickCMenuH / 2;
		break;
	case 3:
		Height = UI.RightClickCMenuH / 2;
		break;
	case 5:
		Height = UI.RightClickCMenuH / 2;
		break;
	case 7:
		//HoveringBar
		Height = UI.HoverHeight;
		break;
	}
	image * ptr = new image;
	if (type == 7) {
		//It's an hover part
		pWind->StoreImage(ptr, x, y, w, Height);
	}
	else if (type == 9) {
		//It's the status Bar
		pWind->StoreImage(ptr, x, y, UI.StatusBoxWidth, UI.StatusBoxHeight);
	}
	else pWind->StoreImage(ptr, x, y, UI.RightClickMenuLength, Height);
	return ptr;
}
void Output::DrawAfterMenu(image * img, int x, int y, int type, int w)
{
	int Height;
	switch (type)
	{
	case 1:
		Height = UI.RightClickMenuHeight;
		break;
	case 2:
		Height = UI.RightClickCMenuH + UI.RightClickCMenuH / 2;
		break;
	case 3:
		Height = UI.RightClickCMenuH / 2;
		break;
	case 5:
		Height = UI.RightClickCMenuH / 2;
		break;
	case 7:
		//HoveringBar
		Height = UI.HoverHeight;
		break;
	case 9:
		//It's the status Bar
		Height = UI.StatusBarHeight;
		break;
	}
	if (type == 7) {
		//It's an hover part
		pWind->DrawImage(img, x, y, w, Height);
	}
	else if (type == 9) {
		//It's the status Bar
		pWind->DrawImage(img, x, y, UI.StatusBoxWidth, UI.StatusBoxHeight);
	}
	else pWind->DrawImage(img, x, y, UI.RightClickMenuLength, Height);
	if (type != 7)
		delete img;
}
//Draw status box
void Output::PrintStatusBox(string s, color Z)
{
	//Print a StatusBox with timer in the time stack
	image * img;
	//the upperleft coordinates of the StatusBox
	int x, y;
	x = UI.width - UI.StatusBoxWidth;
	y = UI.height - UI.StatusBoxHeight;
	//Drawing the Text
	HWND theWin = pWind->DrawStringStatusBox(s, x, y);
	//Timer
	later theTimer(2000, true, &DRAWAFTERMENUE, this, theWin);
}
//Pop up error/confirmation msg box
int Output::printPopUpMessage(string s, char type)
{
	char *msg = (char*)(s.c_str());
	return (pWind->printMessageBox(msg, type));
}
//Draw truth table as pop up
image * Output::DrawTruthTable(vector<vector<string> > table, int inputsNum, int outputsNum, int & X, int & Y, int & w, int & h)
{
	int startx, starty;
	int maxInputOutputNameWidth, maxInputOutputNameHeight;
	pWind->SetFont(20, BOLD | UNDERLINED, BY_NAME, "Consolas");
	pWind->getStringWidth("InputNumber12", maxInputOutputNameWidth, maxInputOutputNameHeight); // get max size
	w = (inputsNum + outputsNum) * maxInputOutputNameWidth + (inputsNum + outputsNum + 1) * 20 + 4 + 20; // calc width
	h = ((1 << inputsNum) + 1) * (maxInputOutputNameHeight + 15) + 50;//calc height

	if (w > UI.width - 40) // if wide return NULL to print in file
		return NULL;

	//Setting the upper left point
	X = startx = UI.width / 2 - w / 2;
	Y = starty = UI.height / 2 - h / 2;

	//Screen shot of the table area startx, starty, width, height
	//Dont forget to deallocate it when finishing
	image *img = new image;
	pWind->StoreImage(img, startx, starty, w, h);

	//Draw Background with shadow
	pWind->SetPen(color(176, 178, 181), 0);
	pWind->SetBrush(color(176, 178, 181));
	pWind->DrawRectangle(startx, starty, startx + w, starty + h, FILLED);
	pWind->SetBrush(WHITE);
	pWind->SetPen(WHITE, 0);
	pWind->DrawRectangle(startx + 1, starty + 1, startx + 1 + w - 3, starty + 1 + h - 3, FILLED);

	//Print header
	pWind->SetFont(20, BOLD | UNDERLINED, BY_NAME, "Consolas");
	pWind->SetPen(color(0, 72, 130));
	for (size_t i = 0; i < table[0].size(); i++)
	{
		int x = startx + 10 + maxInputOutputNameWidth * i + maxInputOutputNameWidth * 0.5 + (i + 1) + 20 * (i + 1) - 5;
		int sWidth, sHeight;
		pWind->getStringWidth(table[0][i], sWidth, sHeight); // get header width to align in center of column

															 //Draw header 
		pWind->DrawString(x - sWidth / 2 + 2, starty + 20, table[0][i]);
	}
	// Draw combinations
	pWind->SetFont(18, BOLD, BY_NAME, "Consolas");
	pWind->SetPen(BLACK);
	for (size_t i = 1; i < table.size(); i++)
	{
		if (i % 2 == 0) // Draw gray row
		{
			pWind->SetBrush(color(236, 236, 236));
			pWind->SetPen(color(236, 236, 236), 0);
			pWind->DrawRectangle(startx + 10, starty + 40 + maxInputOutputNameHeight * (i)+15 * (i)-3, startx + 10 + w - 22, starty + 40 + maxInputOutputNameHeight * (i)+15 * (i)+maxInputOutputNameHeight + 10, FILLED);
		}
		//Draw combination
		pWind->SetFont(18, BOLD, BY_NAME, "Consolas");
		pWind->SetPen(BLACK);
		for (size_t j = 0; j < table[i].size(); j++)
		{
			int x = startx + 10 + maxInputOutputNameWidth * j + maxInputOutputNameWidth * 0.5 + (j + 1) + 20 * (j + 1) - 5;
			pWind->DrawString(x, starty + 40 + maxInputOutputNameHeight * (i)+15 * (i), table[i][j]);
		}
	}
	return img;
}
//Restore old state before drawing truth table
void Output::drawAfterTruthTable(image * img, int X, int Y, int w, int h)
{
	if (img != NULL) {
		pWind->DrawImage(img, X, Y, w, h);
		delete img;
	}
}

void Output::PrintTruthTable()
{
	pWind->OpenTruthTable();
}

void Output::storeImage(image * img, int x1, int y1, int x2, int y2)
{
	pWind->StoreImage(img, x1, y1, x2, y2);
}
// Change connection color
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

			// Draw small pins connections
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
			// if above another connection: color the hump
			if (usedPixels[cell2.y][cell2.x] == INTERSECTION && cell2.cellType == VERTICAL) {
				if (cell2.x < cell.x) {
					b1 = true;
					if (!PreviousIsIntersection)
					{
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE);
					}
					else {
						PreviousIsIntersection = false;
						pWind->DrawLine(cell.x * UI.GRID_SIZE - 6, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE);
					}
					if (Color == UI.SelectColor) pWind->SetPen(UI.SelectedHumpColor, 2); else if (Color == UI.ConnColor) pWind->SetPen(UI.HumpColor, 2);
					if (Color == UI.OneValueConnection) { pWind->SetPen(UI.OneValueHumpColor, 2); }
					pWind->DrawBezier(cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE - 14, cell.y*UI.GRID_SIZE - 6, cell.x * UI.GRID_SIZE - 18, cell.y*UI.GRID_SIZE - 8, cell.x * UI.GRID_SIZE - 22, cell.y*UI.GRID_SIZE);
					pWind->SetPen(Color, 2);
					if (j + 2 < connection->getCellsBeforeAddingConnection().size() && arrayOfIntersections[connection->getCellsBeforeAddingConnection()[j + 2].y][connection->getCellsBeforeAddingConnection()[j + 2].x] != 0)
					{
						pWind->DrawLine(cell.x * UI.GRID_SIZE - 22, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE - 32, cell.y*UI.GRID_SIZE);
					}
				}
				else {
					b1 = false;
					if (!PreviousIsIntersection)
					{
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE);
					}
					else {
						PreviousIsIntersection = false;
						pWind->DrawLine(cell.x * UI.GRID_SIZE + 6, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE);
					}
					if (Color == UI.SelectColor) pWind->SetPen(UI.SelectedHumpColor, 2); else if (Color == UI.ConnColor) pWind->SetPen(UI.HumpColor, 2);
					if (Color == UI.OneValueConnection) { pWind->SetPen(UI.OneValueHumpColor, 2); }
					pWind->DrawBezier(cell.x * UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE + 14, cell.y*UI.GRID_SIZE - 6, cell.x * UI.GRID_SIZE + 18, cell.y*UI.GRID_SIZE - 8, cell.x * UI.GRID_SIZE + 22, cell.y*UI.GRID_SIZE);
					pWind->SetPen(Color, 2);
					if (j + 2 < connection->getCellsBeforeAddingConnection().size() && arrayOfIntersections[connection->getCellsBeforeAddingConnection()[j + 2].y][connection->getCellsBeforeAddingConnection()[j + 2].x] != 0)
					{
						pWind->DrawLine(cell.x * UI.GRID_SIZE + 22, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE + 32, cell.y*UI.GRID_SIZE);
					}
				}
				PreviousIsIntersection = true;
				Vertical0Horizontal1Nothing2 = -1;
			}
			// if this connection above another horiz one
			else if (usedPixels[cell2.y][cell2.x] == INTERSECTION && cell2.cellType == HORIZONTAL) {
				if (cell2.y < cell.y) {
					b2 = true;
					if (!PreviousIsIntersection2) {
						if (arrayOfIntersections[cell.y][cell.x] != 0)
						{
							pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 10);
						}
						else {
							pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 10);
						}
					}
					else {
						PreviousIsIntersection2 = false;
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 6, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 10);
					}
					if (Color == UI.SelectColor) pWind->SetPen(UI.SelectedHumpColor, 2); else if (Color == UI.ConnColor) pWind->SetPen(UI.HumpColor, 2);
					if (Color == UI.OneValueConnection) { pWind->SetPen(UI.OneValueHumpColor, 2); }
					pWind->DrawBezier(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 10, cell.x * UI.GRID_SIZE + 6, cell.y*UI.GRID_SIZE - 14, cell.x * UI.GRID_SIZE + 8, cell.y*UI.GRID_SIZE - 18, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 22);
					pWind->SetPen(Color, 2);
					if (j + 2 < connection->getCellsBeforeAddingConnection().size() && arrayOfIntersections[connection->getCellsBeforeAddingConnection()[j + 2].y][connection->getCellsBeforeAddingConnection()[j + 2].x] != 1)
					{
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 22, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 32);
					}
				}
				else {
					b2 = false;
					if (!PreviousIsIntersection2)
					{
						if (arrayOfIntersections[cell.y][cell.x] != 0)
						{
							pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 10);
						}
						else {
							pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 3, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 10);
						}
					}
					else {
						PreviousIsIntersection2 = false;
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 6, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 10);
					}
					if (Color == UI.SelectColor) pWind->SetPen(UI.SelectedHumpColor, 2); else if (Color == UI.ConnColor) pWind->SetPen(UI.HumpColor, 2);
					if (Color == UI.OneValueConnection) { pWind->SetPen(UI.OneValueHumpColor, 2); }
					pWind->DrawBezier(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 10, cell.x * UI.GRID_SIZE + 6, cell.y*UI.GRID_SIZE + 14, cell.x * UI.GRID_SIZE + 8, cell.y*UI.GRID_SIZE + 18, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 22);
					pWind->SetPen(Color, 2);
					if (j + 2 < connection->getCellsBeforeAddingConnection().size() && arrayOfIntersections[connection->getCellsBeforeAddingConnection()[j + 2].y][connection->getCellsBeforeAddingConnection()[j + 2].x] != 1)
					{
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 22, cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 32);
					}
				}
				PreviousIsIntersection2 = true;
				Vertical0Horizontal1Nothing2 = -1;

			}
			// if connection above this connection
			else if (usedPixels[cell2.y][cell2.x] == INTERSECTION && cell2.cellType == EMPTY && arrayOfCorners[cell2.y][cell2.x] == 0) {
				//check orientation and draw depending on it
				if (cell2.y == cell.y)
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
						if (!PreviousIsIntersection)
						{
							pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE + 4, cell2.y*UI.GRID_SIZE);
						}
						else {
							pWind->DrawLine(cell.x * UI.GRID_SIZE + 6, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE + 4, cell2.y*UI.GRID_SIZE);
						}
					}
					else {
						isCell2XGreaterThanCellX = false;
						if (!PreviousIsIntersection)
						{
							pWind->DrawLine(cell.x * UI.GRID_SIZE + ((arrayOfCorners[cell.y][cell.x] == 0 && (i != 0 || j != 0)) ? 4 : 0), cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE + 6, cell2.y*UI.GRID_SIZE);
						}
						else {
							pWind->DrawLine(cell.x * UI.GRID_SIZE - 6, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE + 6, cell2.y*UI.GRID_SIZE);
						}
					}
				}
				else if (Vertical0Horizontal1Nothing2 == 0) {
					if (cell2.y < cell.y) {
						isCell2YGreaterThanCellY = false;
						if (!PreviousIsIntersection2)
						{
							pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE - 3);
						}
						else {
							pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - 6, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE - 3);
						}
					}
					else {
						isCell2YGreaterThanCellY = true;
						if (!PreviousIsIntersection2)
						{
							pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE - 6);

						}
						else {
							pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE + 6, cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE - 6);
						}

					}
				}
			}
			else { // normal erase
				if (Vertical0Horizontal1Nothing2 == 2)
				{
					if (PreviousIsIntersection) {
						pWind->DrawLine(cell.x * UI.GRID_SIZE - ((b1) ? 6 : -6), cell.y*UI.GRID_SIZE - ((PreviousIsIntersection2) ? 6 : 0), cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);

					}
					else if (PreviousIsIntersection2) {
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE - ((b2) ? 6 : -6), cell2.x * UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);

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


				PreviousIsIntersection = PreviousIsIntersection2 = false;
				Vertical0Horizontal1Nothing2 = 2;
			}

		}
	}

}
// Clear the connections
void Output::clearConnections(vector<Connection*>& allConnections, int originalX, int originalY, bool isInput, bool setDeleted) {

	for (size_t i = 0; i < allConnections.size(); i++)
	{
		if (allConnections[i]->getDelete()) // if already deleted stop
		{
			continue;
		}
		for (size_t j = 0; j < allConnections[i]->getCellsBeforeAddingConnection().size(); j++)
		{
			// Clear from grid
			if (Utils::getArrayOfComponents(allConnections[i]->getCellsBeforeAddingConnection()[j].y, allConnections[i]->getCellsBeforeAddingConnection()[j].x) == allConnections[i])
			{
				Utils::setArrayOfComponents(allConnections[i]->getCellsBeforeAddingConnection()[j].y, allConnections[i]->getCellsBeforeAddingConnection()[j].x, NULL);
			}
			//start clearing UI
			Cell& cell = allConnections[i]->getCellsBeforeAddingConnection()[j];
			int Vertical0Horizontal1Nothing2 = 2;

			if (j < allConnections[i]->getCellsBeforeAddingConnection().size() - 1)
			{
				Vertical0Horizontal1Nothing2 = 2;
				Cell& cell2 = allConnections[i]->getCellsBeforeAddingConnection()[j + 1];

				// this Connection under other connection
				if (usedPixels[cell.y][cell.x] == INTERSECTION && cell.cellType == EMPTY && arrayOfCorners[cell.y][cell.x] == 0) {
					if (cell2.y == cell.y)
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
						pWind->SetPen(UI.ConnColor, 2);
						pWind->DrawLine(cell.x* UI.GRID_SIZE, cell.y* UI.GRID_SIZE - 11, cell.x* UI.GRID_SIZE, cell.y* UI.GRID_SIZE + 10);
					}
					else if (Vertical0Horizontal1Nothing2 == 0) {
						pWind->SetPen(WHITE, 2);
						pWind->SetBrush(WHITE);
						pWind->DrawLine(cell.x * UI.GRID_SIZE, cell.y*UI.GRID_SIZE, cell2.x*UI.GRID_SIZE, cell2.y*UI.GRID_SIZE);
						pWind->DrawRectangle(cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE - 10, cell.x*UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE + 10, FILLED);
						pWind->SetPen(UI.ConnColor, 2);
						pWind->DrawLine(cell.x* UI.GRID_SIZE - ((arrayOfIntersections[cell.y][cell.x - 1] != -1) ? 11 : 15), cell.y* UI.GRID_SIZE, cell.x* UI.GRID_SIZE + 10, cell.y* UI.GRID_SIZE);
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
					pWind->SetPen(UI.ConnColor, 2);
					pWind->DrawLine(cell.x* UI.GRID_SIZE, cell.y* UI.GRID_SIZE - 10, cell.x* UI.GRID_SIZE, cell.y* UI.GRID_SIZE + 10);
					Vertical0Horizontal1Nothing2 = -1;
				}
				if (usedPixels[cell.y][cell.x] == INTERSECTION && cell.cellType == HORIZONTAL && Vertical0Horizontal1Nothing2 == 2) {
					pWind->SetPen(WHITE, 2);
					pWind->SetBrush(WHITE);
					pWind->DrawRectangle(cell.x * UI.GRID_SIZE - 10, cell.y*UI.GRID_SIZE - 10, cell.x*UI.GRID_SIZE + 10, cell.y*UI.GRID_SIZE + 10, FILLED);
					pWind->SetPen(BLACK, 1);
					pWind->DrawPixel(cell.x*UI.GRID_SIZE, cell.y*UI.GRID_SIZE);
					pWind->SetPen(UI.ConnColor, 2);
					pWind->DrawLine(cell.x* UI.GRID_SIZE - 11, cell.y* UI.GRID_SIZE, cell.x* UI.GRID_SIZE + 10, cell.y* UI.GRID_SIZE);
					Vertical0Horizontal1Nothing2 = -1;
				}

				//Clear other connections intersections
				if (Vertical0Horizontal1Nothing2 == 0 || Vertical0Horizontal1Nothing2 == 1)
				{
					vector<Connection*> allConnections2 = *allConnectionsPointer;

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
				//clear input pin small lines
				if (!isInput) {
					originalY = allConnections[i]->getDestPin()->getComponent()->getCenterLocation().y1;
					originalX = allConnections[i]->getDestPin()->getComponent()->getCenterLocation().x1;
				}
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
			else if (j != allConnections[i]->getCellsBeforeAddingConnection().size() - 1 && j != 0) {
				usedPixels[cell.y][cell.x] = EMPTY;
			}
			if (j == allConnections[i]->getCellsBeforeAddingConnection().size() - 1) {
				pWind->SetPen(BLACK, 1);
				pWind->DrawPixel(cell.x*UI.GRID_SIZE, cell.y*UI.GRID_SIZE);
				if (setDeleted)
				{
					usedPixels[cell.y][cell.x] = PIN;
				}
			}
			else if (setDeleted && j == allConnections[i]->getCellsBeforeAddingConnection().size() - 2) {
				if (cell.cellType == PIN)
				{
					usedPixels[cell.y][cell.x] = PIN;
				}
			}

		}
		// clear connection
		allConnections[i]->getCellsBeforeAddingConnection().clear();
		if (setDeleted)
		{
			allConnections[i]->deleteConnection(this);
		}

	}

}

void Output::clearConnectionsFromGrid(vector<Connection*> allOutputConnections, vector<Connection*> allInputConnections) {
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

					if (cell2.y == cell.y)
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
					vector<Connection*> allConnections = *allConnectionsPointer;

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
			else if (j != allOutputConnections[i]->getCellsBeforeAddingConnection().size() - 1 && j != 0) {
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

					if (cell2.y == cell.y)
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
					vector<Connection*> allConnections = *allConnectionsPointer;

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

}


bool Output::SetDragImage(ActionType ActType, GraphicsInfo& GfxInfo, image* smallCleanImageBeforeAddingGate, bool moving, Component* comp) {
	int originalX, originalY;
	bool alreadyHighlighted = false;
	if (moving) // if moving get component center
	{
		originalX = comp->getCenterLocation().x1;
		originalY = comp->getCenterLocation().y1;
	}
	int iXOld = 0;
	int iYOld = 0;

	pWind->GetMouseCoord(iXOld, iYOld); // get current mouse coordinates

										//get gate upper corner
	char cKeyData;
	int RectULX = iXOld - UI.GATE_Width / 2;
	int RectULY = iYOld - UI.GATE_Height / 2;
	if (!moving)
	{
		PrintStatusBox("Please select point within workspace and avoid overlaping!, press ESCAPE to stop");
	}
	else {
		PrintStatusBox("Please select point within workspace and avoid overlaping!");
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
	}
	//Store screenshots without the gate and connection in order to display them while moving 
	pWind->StoreImage(storedImg, 0, 0, pWind->GetWidth(), pWind->GetHeight());
	pWind->StoreImage(storedDrawingImg, UI.LeftToolBarWidth, 0, pWind->GetWidth() - UI.LeftToolBarWidth, pWind->GetHeight() - UI.StatusBarHeight);

	pWind->SetPen(BLUE, 2);
	while (true)
	{
		//get and correct the mouse point
		int x, y;
		pWind->GetMouseCoord(x, y);
		bool wrong = false;
		Utils::correctPointClicked(x, y, true, false);

		if (moving && (x != iXOld || y != iYOld))
		{
			//Clear connections from grid
			clearConnectionsFromGrid(allOutputConnections, allInputConnections);
			drawnConnectionsCount = 0;
		}
		if (!Utils::CheckPoint({ x,y }, this, comp, moving, false)) {
			wrong = true;
		}
		else {
			wrong = false;
		}
		// if correct points 
		if (Utils::CheckPoint(x, y, usedPixels)) {
			// if not the same old position
			if ((x != iXOld || y != iYOld) || (!alreadyHighlighted && moving && x == iXOld && y == iYOld)) {
				alreadyHighlighted = true;
				pWind->DrawImage(storedDrawingImg, UI.LeftToolBarWidth, 0, pWind->GetWidth() - UI.LeftToolBarWidth, pWind->GetHeight() - UI.StatusBarHeight);

				//update variables
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
				//Store small image for the component before drawing the component in the new place 
				pWind->StoreImage(smallCleanImageBeforeAddingGate, Gfx.x1 - UI.GRID_SIZE - 5, Gfx.y1 - UI.GRID_SIZE - 5, 2 * UI.GRID_SIZE + 3, UI.GATE_Height + 3);
				switch (ActType) {
				case ADD_Buff: {
					DrawNot_Buffer(Gfx, true, moving, wrong);
					break;
				}
				case ADD_INV:
				{

					DrawNot_Buffer(Gfx, false, moving, wrong);

					break;
				}
				case ADD_AND_GATE_2:
				{

					DrawAnd_Nand(Gfx, 2, false, moving, wrong);
					break;
				}
				case ADD_AND_GATE_3:
				{
					DrawAnd_Nand(Gfx, 3, false, moving, wrong);
					break;
				}

				case ADD_NAND_GATE_2:
				{

					DrawAnd_Nand(Gfx, 2, true, moving, wrong);
					break;
				}
				case ADD_OR_GATE_2:
				{
					DrawOr_Nor(Gfx, 2, false, moving, wrong);
					break;
				}

				case ADD_NOR_GATE_2:
				{
					DrawOr_Nor(Gfx, 2, true, moving, wrong);

					break;
				}

				case ADD_XOR_GATE_2:
				{

					DrawXor_Xnor(Gfx, 2, false, moving, wrong);
					break;
				}
				case ADD_XOR_GATE_3:
				{
					DrawXor_Xnor(Gfx, 3, false, moving, wrong);
					break;
				}

				case ADD_XNOR_GATE_2:
				{

					DrawXor_Xnor(Gfx, 2, true, moving, wrong);
					break;
				}
				case ADD_NOR_GATE_3:
				{

					DrawOr_Nor(Gfx, 3, true, moving, wrong);
					break;
				}
				case ADD_Switch:
				{
					DrawSwtich(Gfx, false, moving, wrong);
					break;
				}
				case ADD_LED:
				{
					DrawLed(Gfx, false, moving, wrong);
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

					//Reconnect conenctions 
					for (size_t i = 0; i < allInputConnections.size(); i++)
					{
						GraphicsInfo currentGfx = allInputConnections[i]->getCornersLocation();
						currentGfx.x2 = currentGfx.x2 + (Gfx.x1 - originalX);
						currentGfx.y2 = currentGfx.y2 + (Gfx.y1 - originalY);
						allInputConnections[i]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
						if (DrawConnection(currentGfx, allInputConnections[i]->getDestPin()->getPosition(), { Gfx.x1, Gfx.y1,0,0 }, allInputConnections[i]->getCellsBeforeAddingConnection(), moving, NULL))drawnConnectionsCount++;
					}
					for (size_t i = 0; i < allOutputConnections.size(); i++)
					{
						GraphicsInfo currentGfx = allOutputConnections[i]->getCornersLocation();
						currentGfx.x1 = currentGfx.x1 + (Gfx.x1 - originalX);
						currentGfx.y1 = currentGfx.y1 + (Gfx.y1 - originalY);
						allOutputConnections[i]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
						Component* dstComp = allOutputConnections[i]->getDestPin()->getComponent();
						if (DrawConnection(currentGfx, allOutputConnections[i]->getDestPin()->getPosition(), { dstComp->getCenterLocation().x1, dstComp->getCenterLocation().y1,0,0 }, allOutputConnections[i]->getCellsBeforeAddingConnection(), moving, NULL))drawnConnectionsCount++;
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

		int tX, tY;
		if (!moving && pWind->GetKeyPress(cKeyData) == ESCAPE) { // if clicked escape stop
			pWind->DrawImage(storedImg, 0, 0, pWind->GetWidth(), pWind->GetHeight());
			draw = false;
			break;
		}
		else if (!wrong && Utils::CheckPoint({ x,y }, this, comp, moving, false) && (pWind->GetMouseClick(tX, tY) == LEFT_CLICK) || ((pWind->GetButtonState(LEFT_BUTTON, tX, tY) == BUTTON_UP))) {
			//if mouse clicked check and if coorect point fil the arrays and break
			if ((moving && (noOfTotalConnections == drawnConnectionsCount)) || !moving)
			{
				Utils::correctPointClicked(x, y, true, false);
				if (Utils::CheckPoint({ x,y }, this, comp, moving, false))
				{
					GfxInfo.x1 = x;
					GfxInfo.y1 = y;
					if (Utils::CheckPoint(GfxInfo, this, comp, moving)) {
						if (moving) // Reset connections on grid of pointers
						{
							for (size_t i = 0; i < allInputConnections.size(); i++)
							{
								for (size_t j = 0; j < allInputConnections[i]->getCellsBeforeAddingConnection().size(); j++)
								{
									Utils::setArrayOfComponents(allInputConnections[i]->getCellsBeforeAddingConnection()[j].y, allInputConnections[i]->getCellsBeforeAddingConnection()[j].x, allInputConnections[i]);
								}
							}
							for (size_t i = 0; i < allOutputConnections.size(); i++)
							{
								for (size_t j = 0; j < allOutputConnections[i]->getCellsBeforeAddingConnection().size(); j++)
								{
									Utils::setArrayOfComponents(allOutputConnections[i]->getCellsBeforeAddingConnection()[j].y, allOutputConnections[i]->getCellsBeforeAddingConnection()[j].x, allOutputConnections[i]);
								}
							}
						}
						draw = true;
						break;
					}
					else {
						draw = false;
					}
				}
			}
		}
		pWind->DownButtons(true); // get buttons down
		pWind->FlushMouseQueue();
	}
	pWind->FlushMouseQueue();
	pWind->DownButtons(false);
	delete storedDrawingImg;
	delete storedImg;
	return draw;
}
//Same as what happenes in setDragImage, the only difference is looping throught all selected ones
bool Output::SetMultiDragImage(int currentX, int currentY, Component* mainMovingComponent, vector< pair<int, Component*> > allSelectedComponents) {
	if (mainMovingComponent == NULL)
	{
		return false;
	}
	//PrintMsg("Please select point within workspace and avoid overlaping!, press ESCAPE to stop");
	PrintStatusBox("Please select point within workspace and avoid overlaping!, press ESCAPE to stop");
	int successfullyDrawnComponentsCount = 0;
	image* storedImg = new image;
	image* storedDrawingImg = new image;
	vector <pair <int, int> > xyOld, RectULXY, originalXY;
	vector<bool> isComponentDrawn;
	vector<image*> allSmallCleanImages;
	vector<bool> alreadyHighlighted;
	Utils::correctPointClicked(currentX, currentY, true, false);
	bool isMovingSucceded = false;
	for (size_t m = 0; m < allSelectedComponents.size(); m++)
	{
		Component* comp = allSelectedComponents[m].second; if (dynamic_cast<Connection*>(allSelectedComponents[m].second)) continue;
		int iXOld = currentX + (allSelectedComponents[m].second->getCenterLocation().x1 - mainMovingComponent->getCenterLocation().x1);
		int iYOld = currentY + (allSelectedComponents[m].second->getCenterLocation().y1 - mainMovingComponent->getCenterLocation().y1);
		int RectULX = iXOld - UI.GATE_Width / 2;
		int RectULY = iYOld - UI.GATE_Height / 2;
		int originalX = allSelectedComponents[m].second->getCenterLocation().x1;
		int originalY = allSelectedComponents[m].second->getCenterLocation().y1;

		xyOld.push_back(make_pair(iXOld, iYOld));
		RectULXY.push_back(make_pair(RectULX, RectULY));
		allSmallCleanImages.push_back(new image);
		isComponentDrawn.push_back(false);
		originalXY.push_back(make_pair(originalX, originalY));
		alreadyHighlighted.push_back(false);

		vector<Connection*> allInputConnections, allOutputConnections;
		comp->getAllInputConnections(allInputConnections);
		comp->getAllOutputConnections(allOutputConnections);

		clearConnections(allInputConnections, originalX, originalY, true);
		clearConnections(allOutputConnections, originalX, originalY, false);
	}
	for (size_t m = 0; m < allSelectedComponents.size(); m++)
	{
		allSelectedComponents[m].second->setDelete(true);
		allSelectedComponents[m].second->Draw(this);
	}
	pWind->StoreImage(storedImg, 0, 0, pWind->GetWidth(), pWind->GetHeight());
	pWind->StoreImage(storedDrawingImg, UI.LeftToolBarWidth, 0, pWind->GetWidth() - UI.LeftToolBarWidth, pWind->GetHeight() - UI.StatusBarHeight);

	pWind->SetPen(UI.SelectColor, 2);
	int oldox = 0, oldoy = 0;
	while (true)
	{
		int ox, oy;
		pWind->GetMouseCoord(ox, oy);
		Utils::correctPointClicked(ox, oy, true, false);
		bool moved = false;
		if (!Utils::CheckPoint(ox, oy, usedPixels))
		{

			pWind->FlushMouseQueue();
			pWind->DownButtons(true);
			continue;
		}
		if (Utils::CheckPoint(ox, oy, usedPixels) && (ox != oldox || oy != oldoy)) {
			pWind->DrawImage(storedDrawingImg, UI.LeftToolBarWidth, 0, pWind->GetWidth() - UI.LeftToolBarWidth, pWind->GetHeight() - UI.StatusBarHeight);
		}
		for (size_t m = 0; m < allSelectedComponents.size(); m++)
		{

			if (dynamic_cast<Connection*>(allSelectedComponents[m].second)) continue;
			int drawnConnectionsCount = 0;
			int noOfTotalConnections = 0;
			int x = ox + (allSelectedComponents[m].second->getCenterLocation().x1 - mainMovingComponent->getCenterLocation().x1);
			int y = oy + (allSelectedComponents[m].second->getCenterLocation().y1 - mainMovingComponent->getCenterLocation().y1);
			bool wrong = false;
			Utils::correctPointClicked(x, y, true, false);
			if ((x != xyOld[m].first || y != xyOld[m].second) || (!alreadyHighlighted[m] && x == xyOld[m].first && y == xyOld[m].second))
			{

				moved = true;
				alreadyHighlighted[m] = true;
				vector<Connection*> allInputConnections, allOutputConnections;

				allSelectedComponents[m].second->getAllInputConnections(allInputConnections);
				allSelectedComponents[m].second->getAllOutputConnections(allOutputConnections);
				noOfTotalConnections = allInputConnections.size() + allOutputConnections.size();

				clearConnectionsFromGrid(allOutputConnections, allInputConnections);
				if (!Utils::CheckPoint({ x,y }, this, allSelectedComponents[m].second, true, false)) {
					wrong = true;
				}
				else {
					wrong = false;
				}

				if (Utils::CheckPoint(x, y, usedPixels)) {
					if (x != xyOld[m].first) {
						RectULXY[m].first = RectULXY[m].first + (x - xyOld[m].first);
						xyOld[m].first = x;
					}
					if (y != xyOld[m].second) {
						RectULXY[m].second = RectULXY[m].second + (y - xyOld[m].second);
						xyOld[m].second = y;
					}

					GraphicsInfo Gfx;
					Gfx.x1 = RectULXY[m].first + UI.GATE_Width / 2;
					Gfx.y1 = RectULXY[m].second + UI.GATE_Height / 2;
					pWind->StoreImage(allSmallCleanImages[m], Gfx.x1 - UI.GRID_SIZE - 5, Gfx.y1 - UI.GRID_SIZE - 5, 2 * UI.GRID_SIZE + 3, UI.GATE_Height + 3);

					switch (allSelectedComponents[m].second->getComponentActionType()) {
					case ADD_Buff: {
						DrawNot_Buffer(Gfx, true, true, wrong);
						break;
					}
					case ADD_INV:
					{

						DrawNot_Buffer(Gfx, false, true, wrong);

						break;
					}
					case ADD_AND_GATE_2:
					{

						DrawAnd_Nand(Gfx, 2, false, true, wrong);
						break;
					}
					case ADD_AND_GATE_3:
					{
						DrawAnd_Nand(Gfx, 3, false, true, wrong);
						break;
					}

					case ADD_NAND_GATE_2:
					{

						DrawAnd_Nand(Gfx, 2, true, true, wrong);
						break;
					}
					case ADD_OR_GATE_2:
					{
						DrawOr_Nor(Gfx, 2, false, true, wrong);
						break;
					}

					case ADD_NOR_GATE_2:
					{
						DrawOr_Nor(Gfx, 2, true, true, wrong);

						break;
					}

					case ADD_XOR_GATE_2:
					{

						DrawXor_Xnor(Gfx, 2, false, true, wrong);
						break;
					}
					case ADD_XOR_GATE_3:
					{
						DrawXor_Xnor(Gfx, 3, false, true, wrong);
						break;
					}

					case ADD_XNOR_GATE_2:
					{

						DrawXor_Xnor(Gfx, 2, true, true, wrong);
						break;
					}
					case ADD_NOR_GATE_3:
					{

						DrawOr_Nor(Gfx, 3, true, true, wrong);
						break;
					}
					case ADD_Switch:
					{
						DrawSwtich(Gfx, false, true, wrong);
						break;
					}
					case ADD_LED:
					{
						DrawLed(Gfx, false, true, wrong);
						break;
					}
					}

				}
				if (!wrong && Utils::CheckPoint({ x,y }, this, allSelectedComponents[m].second, true, false)) {
					isComponentDrawn[m] = true;
				}
				else {
					isComponentDrawn[m] = false;
				}
			}
			else { moved = false; }
			pWind->UpdateBuffer();
		}
		bool toContinue = true;

		for (size_t m = 0; m < isComponentDrawn.size(); m++)
		{
			if (dynamic_cast<Connection*>(allSelectedComponents[m].second)) continue;
			if (isComponentDrawn[m] == false)
			{
				toContinue = false;
			}
		}
		if (toContinue && moved)
		{
			//Change corners

			for (size_t m = 0; m < allSelectedComponents.size(); m++)
			{
				if (dynamic_cast<Connection*>(allSelectedComponents[m].second)) continue;
				GraphicsInfo Gfx;
				Gfx.x1 = RectULXY[m].first + UI.GATE_Width / 2;
				Gfx.y1 = RectULXY[m].second + UI.GATE_Height / 2;

				vector<Connection*> allInputConnections, allOutputConnections;

				allSelectedComponents[m].second->getAllInputConnections(allInputConnections);
				allSelectedComponents[m].second->getAllOutputConnections(allOutputConnections);

				for (size_t i = 0; i < allInputConnections.size(); i++)
				{
					GraphicsInfo currentGfx = allInputConnections[i]->getCornersLocation();
					currentGfx.x2 = currentGfx.x2 + (Gfx.x1 - originalXY[m].first);
					currentGfx.y2 = currentGfx.y2 + (Gfx.y1 - originalXY[m].second);
					allInputConnections[i]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
				}

				for (size_t i = 0; i < allOutputConnections.size(); i++)
				{
					Component* dstComp = allOutputConnections[i]->getDestPin()->getComponent();
					GraphicsInfo currentGfx = allOutputConnections[i]->getCornersLocation();
					currentGfx.x1 = currentGfx.x1 + (Gfx.x1 - originalXY[m].first);
					currentGfx.y1 = currentGfx.y1 + (Gfx.y1 - originalXY[m].second);
					allOutputConnections[i]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
				}

			}

			for (size_t m = 0; m < allSelectedComponents.size(); m++)
			{
				if (dynamic_cast<Connection*>(allSelectedComponents[m].second)) continue;
				GraphicsInfo Gfx;
				Gfx.x1 = RectULXY[m].first + UI.GATE_Width / 2;
				Gfx.y1 = RectULXY[m].second + UI.GATE_Height / 2;

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
			}
			////Reconnect

			vector <Connection*> allDrawnConnections;
			for (size_t m = 0; m < allSelectedComponents.size(); m++)
			{
				if (dynamic_cast<Connection*>(allSelectedComponents[m].second)) continue;
				GraphicsInfo Gfx;
				Gfx.x1 = RectULXY[m].first + UI.GATE_Width / 2;
				Gfx.y1 = RectULXY[m].second + UI.GATE_Height / 2;

				vector<Connection*> allInputConnections, allOutputConnections;

				allSelectedComponents[m].second->getAllInputConnections(allInputConnections);
				allSelectedComponents[m].second->getAllOutputConnections(allOutputConnections);
				int drawnConnectionsCount = 0;
				int noOfTotalConnections = allInputConnections.size() + allOutputConnections.size();

				for (size_t i = 0; i < allInputConnections.size(); i++)
				{
					if (std::find(allDrawnConnections.begin(), allDrawnConnections.end(), allInputConnections[i]) != allDrawnConnections.end())
					{
						drawnConnectionsCount++;
						continue;
					}
					if (DrawConnection(allInputConnections[i]->getCornersLocation(), allInputConnections[i]->getDestPin()->getPosition(), { Gfx.x1, Gfx.y1,0,0 }, allInputConnections[i]->getCellsBeforeAddingConnection(), true, NULL)) {
						drawnConnectionsCount++;
						allDrawnConnections.push_back(allInputConnections[i]);
					}
				}
				for (size_t i = 0; i < allOutputConnections.size(); i++)
				{
					if (std::find(allDrawnConnections.begin(), allDrawnConnections.end(), allOutputConnections[i]) != allDrawnConnections.end())
					{
						drawnConnectionsCount++;
						continue;
					}
					bool drawn = false;
					Component* dstComp = allOutputConnections[i]->getDestPin()->getComponent();
					for (size_t z = 0; z < allSmallCleanImages.size(); z++)
					{
						if (allSelectedComponents[z].second == dstComp)
						{
							if (DrawConnection(allOutputConnections[i]->getCornersLocation(), allOutputConnections[i]->getDestPin()->getPosition(), { (RectULXY[z].first + UI.GATE_Width / 2),(RectULXY[z].second + UI.GATE_Height / 2),0,0 }, allOutputConnections[i]->getCellsBeforeAddingConnection(), true, NULL))
							{
								drawnConnectionsCount++;
								allDrawnConnections.push_back(allOutputConnections[i]);
							}
							drawn = true;
							break;
						}
					}
					if (!drawn)
					{
						if (DrawConnection(allOutputConnections[i]->getCornersLocation(), allOutputConnections[i]->getDestPin()->getPosition(), { dstComp->getCenterLocation().x1, dstComp->getCenterLocation().y1,0,0 }, allOutputConnections[i]->getCellsBeforeAddingConnection(), true, NULL))
						{
							drawnConnectionsCount++;
							allDrawnConnections.push_back(allOutputConnections[i]);
						}
					}
				}

				originalXY[m].first = Gfx.x1;
				originalXY[m].second = Gfx.y1;

				if (noOfTotalConnections == drawnConnectionsCount)
				{
					isComponentDrawn[m] = true;
				}
				else {
					isComponentDrawn[m] = false;
				}

			}

			//Remove gates
			for (size_t m = 0; m < allSelectedComponents.size(); m++)
			{
				if (dynamic_cast<Connection*>(allSelectedComponents[m].second)) continue;
				GraphicsInfo Gfx;
				Gfx.x1 = RectULXY[m].first + UI.GATE_Width / 2;
				Gfx.y1 = RectULXY[m].second + UI.GATE_Height / 2;

				int xbegin = (Gfx.x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (Gfx.x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (Gfx.y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (Gfx.y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
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

		}
		if (toContinue) {
			int tx, ty;
			if (Utils::CheckPoint({ ox,oy }, this, NULL, true, false) && (pWind->GetMouseClick(tx, ty) == LEFT_CLICK || pWind->GetButtonState(LEFT_BUTTON, tx, ty) == BUTTON_UP)) {
				{

					for (size_t m = 0; m < allSelectedComponents.size(); m++)
					{
						if (dynamic_cast<Connection*>(allSelectedComponents[m].second)) continue;
						if (Utils::CheckPoint({ originalXY[m].first,originalXY[m].second }, this, allSelectedComponents[m].second, true)) {

							allSelectedComponents[m].second->setDelete(false);
							allSelectedComponents[m].second->setNewCenterLocation({ originalXY[m].first, originalXY[m].second });
							allSelectedComponents[m].second->setSmallCleanImageBeforeAddingComp(allSmallCleanImages[m]);
							allSelectedComponents[m].second->Draw(this, false);
						}
					}

					for (size_t m = 0; m < allSelectedComponents.size(); m++)
					{
						if (dynamic_cast<Connection*>(allSelectedComponents[m].second)) continue;
						vector<Connection*> allInputConnections, allOutputConnections;

						allSelectedComponents[m].second->getAllInputConnections(allInputConnections);
						allSelectedComponents[m].second->getAllOutputConnections(allOutputConnections);

						for (size_t i = 0; i < allInputConnections.size(); i++)
						{
							for (size_t j = 0; j < allInputConnections[i]->getCellsBeforeAddingConnection().size(); j++)
							{
								Utils::setArrayOfComponents(allInputConnections[i]->getCellsBeforeAddingConnection()[j].y, allInputConnections[i]->getCellsBeforeAddingConnection()[j].x, allInputConnections[i]);
							}
							allInputConnections[i]->selectYourSelf(this, UI.DrawColor);
						}
						for (size_t i = 0; i < allOutputConnections.size(); i++)
						{
							for (size_t j = 0; j < allOutputConnections[i]->getCellsBeforeAddingConnection().size(); j++)
							{
								Utils::setArrayOfComponents(allOutputConnections[i]->getCellsBeforeAddingConnection()[j].y, allOutputConnections[i]->getCellsBeforeAddingConnection()[j].x, allOutputConnections[i]);
							}
							allOutputConnections[i]->selectYourSelf(this, UI.DrawColor);
						}
					}
					isMovingSucceded = true;
					break;
					pWind->FlushMouseQueue();
				}
			}
		}

		pWind->FlushMouseQueue();
		pWind->DownButtons(true);
		oldox = ox;
		oldoy = oy;
	}

	pWind->FlushMouseQueue();
	pWind->DownButtons(false);
	//PrintMsg("");
	delete storedDrawingImg;
	delete storedImg;
	return isMovingSucceded;
}

bool Output::saveFile()
{
	return pWind->saveAs();
}

bool Output::loadFile()
{
	return pWind->loadAs();
}

void Output::printMatrix(string msg) {
	/*cout << msg << endl;
	for (size_t i = 0; i < 44; i++)
	{
	for (size_t j = 0; j < 74; j++)
	{
	cout << usedPixels[i][j] << " ";
	}
	cout << endl;
	}*/
}
void Output::printCheck()
{
	//pWind->printChecking();
}
// Fill arrays
void Output::fillArrays(Component* pA) {
	if (pA == NULL || pA->getDelete() || dynamic_cast<Connection*>(pA))
	{
		return;
	}
	int xbegin = (pA->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE;
	int xend = (pA->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE;
	int ybegin = (pA->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE;
	int yend = (pA->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
	for (int i = ybegin + 1; i <= yend; i++)
	{
		for (int j = xbegin; j <= xend; j++)
		{
			Utils::setArrayOfComponents(i, j, pA);
			if (xbegin == j || xend == j)
			{
				if (getUsedPixel(i, j) != INTERSECTION)
				{
					setUsedPixel(i, j, PIN);
				}
				continue;
			}
			setUsedPixel(i, j, GATE);
		}
	}
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//You Send a Centre Point (cx,cy) ,this means when you call Draw image Function , x and y sent should be cx-24, cy-24
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Output::DrawAnd_Nand(GraphicsInfo g, int in, bool isNand, bool highlighted, bool notValid, Component* pA) {
	fillArrays(pA);
	// Points
	int cx = g.x1, cy = g.y1; //Centre Point
	int in1x, in1y, in2x, in2y, outx, outy; // the 2 Inputs & Output
	int p1x, p2x, p1y, p2y; //the Helping Points

	in1x = in2x = p1x = p2x = cx - 14 + (2 * ciDefBrushSize);
	in1y = cy - 13; in2y = cy + 13;
	outy = cy;
	p1y = cy - 20; p2y = cy + 21;

	pWind->SetPen(UI.DrawColor);
	if (highlighted) pWind->SetPen(UI.SelectColor);
	if (notValid) pWind->SetPen(UI.ErrorColor);

	if (isNand) {
		outx = cx + 10 - (2 * ciDefBrushSize);
		p1x = p2x = p1x - 5;
		//Darwing lines
		pWind->DrawLine(p1x, p1y, p1x + 9, p1y, FRAME);
		pWind->DrawLine(p2x, p2y, p2x + 9, p2y, FRAME);
		pWind->DrawLine(in1x - 6 - (2 * ciDefBrushSize), in1y, in1x - 6, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 6 - (2 * ciDefBrushSize), in2y, in2x - 6, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 3 * ciDefBrushSize - 1, outy, outx + 5 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		pWind->DrawLine(p1x, p1y - 1, p2x, p2y, FRAME);
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x, in3y = cy;
			pWind->DrawLine(in3x - 6 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
		}
		//Drawing Arc
		pWind->DrawBezier(p1x + 9, p1y, p1x + 6 + 3 + 4, p1y + 4 - 3, p1x + 6 + 9 + 4, p1y + 12, outx, outy, FRAME);
		pWind->DrawBezier(p2x + 9, p2y, p2x + 6 + 3 + 4, p2y - 4 + 3, p2x + 6 + 9 + 4, p2y - 12, outx, outy, FRAME);

		//Drawing Buuble
		pWind->DrawCircle(outx - ciDefBrushSize + 2 * ciDefBrushSize, outy, 2 * ciDefBrushSize, FRAME);

	}
	else {
		outx = cx + 10 - (2 * ciDefBrushSize);
		p1x = p2x = p1x - 5;
		//Darwing lines
		pWind->DrawLine(p1x, p1y, p1x + 12, p1y, FRAME);
		pWind->DrawLine(p2x, p2y, p2x + 12, p2y, FRAME);
		pWind->DrawLine(in1x - 7 - (2 * ciDefBrushSize), in1y, in1x - 5, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 7 - (2 * ciDefBrushSize), in2y, in2x - 5, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 3, outy, outx + 5 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		pWind->DrawLine(p1x, p1y - 1, p2x, p2y, FRAME);
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x, in3y = cy;
			pWind->DrawLine(in3x - 7 - (2 * ciDefBrushSize), in3y, in3x - 5, in3y, FRAME); //The Input3-Line
		}
		//Drawing Arc
		//pWind->DrawBezier(p1x + 8, p1y, p1x + 8 + 5 , outy + 8, p1x + 8 + 8 , outy + 3, outx, outy, FRAME);
		pWind->DrawBezier(p1x + 12, p1y, p1x + 14 + 3 + 4, p1y + 4 - 3, p1x + 10 + 9 + 4, p1y + 12, outx + 4, outy, FRAME);
		pWind->DrawBezier(p2x + 12, p2y, p2x + 14 + 3 + 4, p2y - 4 + 3, p2x + 10 + 9 + 4, p2y - 12, outx + 4, outy, FRAME);
		//pWind->DrawPixel(, );
	}
}

void Output::DrawNot_Buffer(GraphicsInfo g, bool isBuffer, bool highlighted, bool notValid, Component* pA) {
	fillArrays(pA);
	int cx = g.x1, cy = g.y1; //Centre Point
	int p1x, p1y, p2x, p2y, inx, iny, outx, outy; //Vertices of Triangle and input/output Points

	p1x = p2x = inx = cx - 18 + (2 * ciDefBrushSize);
	p1y = cy + 21; p2y = cy - 21;
	iny = outy = cy;

	pWind->SetPen(UI.DrawColor);
	if (highlighted) pWind->SetPen(UI.SelectColor);
	if (notValid) pWind->SetPen(UI.ErrorColor);

	if (isBuffer) {
		//BUFFER Gate
		outx = cx + 9 - (2 * ciDefBrushSize);
		//Drawing lines 
		pWind->DrawLine(inx - 3 - (2 * ciDefBrushSize), iny, inx, iny, FRAME); //The Input-Line
		pWind->DrawLine(outx, outy, outx + 6 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
																				   //Drawing Trianlge
		pWind->DrawTriangle(p1x, p1y, p2x, p2y, outx, outy, FRAME);
	}
	else {
		// NOT GATE
		outx = cx + 9 - (2 * ciDefBrushSize);
		//Drawing lines 
		pWind->DrawLine(inx - 3 - (2 * ciDefBrushSize), iny, inx, iny, FRAME); //The Input-Line
		pWind->DrawLine(outx + 4 * ciDefBrushSize - 1, outy, outx + 6 + (2 * ciDefBrushSize), outy, FRAME);//The Output-Line

																										   //Drawing Trianlge
		pWind->DrawTriangle(p1x, p1y, p2x, p2y, outx, outy, FRAME);
		//Darwing Bubble
		pWind->DrawCircle(outx + 2 * ciDefBrushSize, outy, 2 * ciDefBrushSize, FRAME);
	}
}

void Output::DrawOr_Nor(GraphicsInfo g, int in, bool isNor, bool highlighted, bool notValid, Component* pA) {
	fillArrays(pA);
	int cx = g.x1, cy = g.y1; //Centre Point
	int p1x, p1y, p2x, p2y, hx1, hx2, hy1, hy2, kx, ky, ky2; //Helping Points
	int in1x, in1y, in2x, in2y, outx, outy; // the 2 Inputs & Output

	in1x = in2x = cx - 16 + (2 * ciDefBrushSize);
	in1y = cy - 13;
	in2y = cy + 13; outy = cy;

	p1x = p2x = cx - 21;
	p2y = hy2 = cy + 21;
	p1y = hy1 = cy - 21;
	hx1 = hx2 = cx;

	pWind->SetPen(UI.DrawColor);
	if (highlighted) pWind->SetPen(UI.SelectColor);
	if (notValid) pWind->SetPen(UI.ErrorColor);
	if (isNor) {
		//NOR GATE
		outy = cy; outx = cx + 10 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2 - 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;

		//Draw lines
		pWind->DrawLine(in1x - 5 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 5 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 4 * ciDefBrushSize - 1, outy, outx + 5 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x + 2 * (ciDefBrushSize == 1 ? 2 : 1), in3y = cy;
			pWind->DrawLine(in3x - 6 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
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
		outx = cx + 14 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;
		//Draw lines
		pWind->DrawLine(in1x - 5 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 5 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx, outy, outx + (2 * ciDefBrushSize) + 1, outy, FRAME); //The Output-Line
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x + 2 * (ciDefBrushSize == 1 ? 2 : 1), in3y = cy;
			pWind->DrawLine(in3x - 6 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
		}
		//Draw Bezier
		pWind->DrawBezier(p1x, p1y, hx1, hy1, kx, ky, outx, outy, FRAME);
		pWind->DrawBezier(p2x, p2y, hx2, hy2, kx, ky2, outx, outy, FRAME);
		pWind->DrawBezier(p1x, p1y, in1x + 8, in1y, in2x + 8, in2y, p2x, p2y, FRAME);
	}



}

void Output::DrawXor_Xnor(GraphicsInfo g, int in, bool isXNor, bool highlighted, bool notValid, Component* pA)
{
	fillArrays(pA);
	int cx = g.x1, cy = g.y1; //Centre Points
	int in1x, in1y, in2x, in2y, outx, outy; // the 2 Inputs And Output
	int hx1, hx2, hy1, hy2, kx, ky, ky2, p1x, p1y, p2x, p2y; //Helping Points

	in1x = in2x = cx - 17 + (2 * ciDefBrushSize);
	in1y = cy - 13;
	in2y = cy + 13;

	p1x = p2x = cx - 24;
	p2y = hy2 = cy + 21;
	p1y = hy1 = cy - 21;
	hx1 = hx2 = cx - 2;
	int xi = 8; //X-Increment

	pWind->SetPen(UI.DrawColor);
	if (highlighted) pWind->SetPen(UI.SelectColor);
	if (notValid) pWind->SetPen(UI.ErrorColor);
	if (isXNor) {
		//XNOR GATE
		outy = cy; outx = cx + 11 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2 - 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;
		//Draw lines
		pWind->DrawLine(in1x - 3 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 3 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx + 4 * ciDefBrushSize - 3, outy, outx + 4 + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
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
		pWind->DrawCircle(outx + 1 * ciDefBrushSize, outy, 2 * ciDefBrushSize, FRAME);

	}
	else {
		//XOR GATE
		outy = cy; outx = cx + 15 - (2 * ciDefBrushSize);
		kx = (outx + hx1) / 2;
		ky = (outy + hy1) / 2;
		ky2 = (outy + hy2) / 2;
		//Draw lines
		pWind->DrawLine(in1x - 4 - (2 * ciDefBrushSize), in1y, in1x, in1y, FRAME); //The Input1-Line
		pWind->DrawLine(in2x - 4 - (2 * ciDefBrushSize), in2y, in2x, in2y, FRAME); //The Input2-Line
		pWind->DrawLine(outx, outy, outx + (2 * ciDefBrushSize), outy, FRAME); //The Output-Line
		if (in == 3) { //Checking for 3 input Gate 
			int in3x = in1x, in3y = cy;
			pWind->DrawLine(in3x - 4 - (2 * ciDefBrushSize), in3y, in3x, in3y, FRAME); //The Input3-Line
		}
		//Draw Bezier
		pWind->DrawBezier(p1x + xi, p1y, hx1, hy1, kx, ky, outx, outy, FRAME);
		pWind->DrawBezier(p1x + xi, p1y, in1x + xi, in1y, in2x + xi, in2y, p2x + xi, p2y, FRAME);
		pWind->DrawBezier(p2x + xi, p2y, hx2, hy2, kx, ky2, outx, outy, FRAME);
		//draw the xor Bezier with delta x slightly different than the previous to avoid collision
		pWind->DrawBezier(p1x + (2 * ciDefBrushSize), p1y, in1x + (2 * ciDefBrushSize), in1y, in2x + (2 * ciDefBrushSize), in2y, p2x + (2 * ciDefBrushSize), p2y, FRAME);
	}

}

void Output::DrawLed(GraphicsInfo g, bool isON, bool highlighted, bool notValid, Component* pA)
{
	fillArrays(pA);
	pWind->SetPen(UI.DrawColor);
	if (highlighted) pWind->SetPen(UI.SelectColor);
	if (notValid) pWind->SetPen(UI.ErrorColor);
	int cx = g.x1 - 3, cy = g.y1, radius = 10; //Centre Points
	if (!isON)
	{
		pWind->SetBrush(GRAY);
		pWind->DrawCircle(cx, cy, radius, FILLED);
	}
	else
	{
		pWind->SetBrush(color(161, 192, 88));
		pWind->DrawCircle(cx, cy, radius, FILLED);
	}
	int inx = cx - radius - 10, iny = cy; // one input 
	pWind->DrawLine(inx + 3, iny, cx - radius, cy, FRAME);//input line (left)
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

void Output::DrawSwtich(GraphicsInfo g, bool isON, bool highlighted, bool notValid, Component* pA)
{
	fillArrays(pA);
	pWind->SetPen(UI.DrawColor);
	if (highlighted) pWind->SetPen(UI.SelectColor);
	if (notValid) pWind->SetPen(UI.ErrorColor);
	int cx = g.x1 - 6, cy = g.y1; //Centre Points
								  //the rectangle
	if (isON)
	{
		pWind->DrawImage("images\\Menu\\SWITCH_ON.jpg", cx - 13, cy - 16, 32, 32);
	}
	else
	{
		pWind->DrawImage("images\\Menu\\SWITCH_OFF.jpg", cx - 13, cy - 16, 32, 32);
	}
	//the output line
	pWind->DrawLine(cx + 12 + 4, cy, cx + 21, cy);




}

void Output::setUsedPixel(int i, int j, CellType c) {
	usedPixels[i][j] = c;
}

CellType Output::getUsedPixel(int i, int j)
{
	return usedPixels[i][j];
}

void Output::resetInterfaceArrays()
{
	memset(usedPixels, 0, sizeof usedPixels);
	memset(arrayOfIntersections, -1, sizeof arrayOfIntersections);
	memset(arrayOfCorners, 0, sizeof arrayOfCorners);
	memset(connectionsCountAtPixel, 0, sizeof connectionsCountAtPixel);
	memset(Utils::arrayOfComponents, NULL, sizeof Utils::arrayOfComponents);
}

Output::~Output()
{
	delete pWind;
}

#include "Utils.h"
#include "GUI\UI_Info.h"
#include <iostream>
#include "Components\Component.h"
#include "Components\Connection.h"

Component* Utils::arrayOfComponents[44][74];
int Utils::J = 0; int Utils::K = 0; int Utils::widthh = 0;
image* Utils::imgh = NULL;
stack <Action*> Utils::undoActions, Utils::redoActions, Utils::simulationUndoActions, Utils::simulationRedoActions;
vector<Action*> Utils::theActions;
vector<GraphicsInfo> Utils::allComponentsCorners;
vector<Connection*> Utils::allConnections;

Utils::Utils()
{
}
void Utils::setArrayOfComponents(int i, int j, Component * c)
{
	Utils::arrayOfComponents[i][j] = c;
}

Component * Utils::getArrayOfComponents(int i, int j)
{
	if (i > 44 || j > 77) return NULL; //IBRAHIM IF I DIDN'T ADD THIS LINE THE PROGRAM WILL TERMINATE FOR NO KNOWN REASON 
									   //Giving that the error is that j is always  4096 
	return Utils::arrayOfComponents[i][j];
}

GraphicsInfo* Utils::getOutputDirections(GraphicsInfo GInfo, CellType usedPixels[44][74], int noOfOutputs) {
	GraphicsInfo* arrayOfDirections = new GraphicsInfo[noOfOutputs];
	GInfo.x1 += UI.GRID_SIZE - (GInfo.x1 % UI.GRID_SIZE);

	GraphicsInfo GInfo1, GInfo2, GInfo3;
	GInfo1.x1 = GInfo.x1;
	GInfo1.y1 = GInfo.y1;

	GInfo2.x1 = GInfo.x1;
	GInfo2.y1 = GInfo.y1 - UI.GRID_SIZE;

	GInfo3.x1 = GInfo.x1;
	GInfo3.y1 = GInfo.y1 + UI.GRID_SIZE;

	arrayOfDirections[1] = GInfo1;
	arrayOfDirections[0] = GInfo2;
	arrayOfDirections[2] = GInfo3;
	return arrayOfDirections;
}
void  Utils::correctPointClicked(int &x, int &y, bool DrawGate, bool DrawConnection) {
	if (DrawGate) {

		int xRemind = x % UI.GRID_SIZE;
		if (xRemind <= UI.GRID_SIZE / 2)
		{
			x = x - xRemind;
		}
		else {
			x = x + (UI.GRID_SIZE - xRemind);
		}
		int yRemind = y % UI.GRID_SIZE;
		if (yRemind <= UI.GRID_SIZE / 2)
		{
			y = y - yRemind;
		}
		else {
			y = y + (UI.GRID_SIZE - yRemind);
		}
	}
	else if (DrawConnection) {
		int yRemind = y % UI.GRID_SIZE;
		if (yRemind <= UI.GRID_SIZE / 2)
		{
			y = y - yRemind;
		}
		else {
			y = y + (UI.GRID_SIZE - yRemind);
		}
	}
}
bool Utils::CheckPoint(GraphicsInfo r_GfxInfo, Output* pOut, Component* comp,  bool isMoving, bool fillArray) {
	int xbegin = (r_GfxInfo.x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (r_GfxInfo.x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (r_GfxInfo.y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (r_GfxInfo.y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
	if (xbegin -1 <= 2 || xend + 1 > 73 || ybegin < 3 || yend + 1 > 43)
	{
		return 0;
	}
	for (int i = ybegin; i <=  yend + 1; i++)
	{
		for (int j = xbegin - 1; j <= xend + 1; j++)
		{
			if (pOut->getUsedPixel(i,j) == GATE || (pOut->getUsedPixel(i,j) == HORIZONTAL || pOut->getUsedPixel(i,j) == VERTICAL)) {
				if ((i == yend + 1 || i == ybegin) && (pOut->getUsedPixel(i,j) == HORIZONTAL || pOut->getUsedPixel(i,j) == END_CONNECTION)) {}
				else if (isMoving && (i == ybegin || i == yend + 1 || j == xbegin - 1 || j == xend+1) && pOut->getUsedPixel(i,j) != GATE) {}
				else {
					return 0;
				}
			}
		}
	}
	if ((r_GfxInfo.x1 - UI.GATE_Width / 2.0) <= UI.LeftToolBarWidth + 20 || (r_GfxInfo.y1 - UI.GATE_Height / 2.0) <=  UI.ToolBarHeight+8 || (r_GfxInfo.x1 + UI.GATE_Width / 2.0) >= UI.width - 16 || (r_GfxInfo.y1 + UI.GATE_Height / 2.0) >= UI.height - UI.StatusBarHeight)return 0;

	if (fillArray) {
		for (int i = ybegin + 1; i <= yend; i++)
		{
			for (int j = xbegin; j <= xend; j++)
			{
				if (xbegin == j || xend == j)
				{
					if (pOut->getUsedPixel(i,j) != INTERSECTION)
					{
						pOut->setUsedPixel(i,j, PIN);
					}
					continue;
				}
				pOut->setUsedPixel(i, j, GATE);
				setArrayOfComponents(i, j, comp);
			}
		}
	}
	return 1;
}
bool Utils::CheckPoint(int x, int y, CellType usedPixels[44][74]) {
	if ((x - UI.GATE_Width / 2.0) <= UI.LeftToolBarWidth + 20 || (y - UI.GATE_Height / 2.0) <= (UI.ToolBarHeight+20) || (x + UI.GATE_Width / 2.0) >= UI.width - 16 || (y + UI.GATE_Height / 2.0) >= (UI.height - UI.StatusBarHeight)) {
		return 0;
	}
	return 1;
}
bool Utils::CheckPointInBorders(int x, int y) {
	if (x< UI.LeftToolBarWidth + 20 || y <= (UI.ToolBarHeight + 20) || (x)>UI.width - 20 || (y) >= (UI.height - UI.StatusBarHeight - 20)) {
		return 0;
	}
	return 1;
}
Utils::~Utils()
{
}

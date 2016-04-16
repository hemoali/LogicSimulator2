#include "Utils.h"
#include "GUI\UI_Info.h"
#include <iostream>
Utils::Utils()
{
}
GraphicsInfo* Utils::getOutputDirections(GraphicsInfo GInfo, CellType usedPixels[44][74], int noOfOutputs){
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
void  Utils::correctPointClicked(int &x, int &y, bool DrawGate, bool DrawConnection){
	if (DrawGate){

		int xRemind = x % UI.GRID_SIZE;
		if (xRemind <= UI.GRID_SIZE / 2)
		{
			x = x - xRemind;
		}
		else{
			x = x + (UI.GRID_SIZE - xRemind);
		}
		int yRemind = y % UI.GRID_SIZE;
		if (yRemind <= UI.GRID_SIZE / 2)
		{
			y = y - yRemind;
		}
		else{
			y = y + (UI.GRID_SIZE - yRemind);
		}
	}
	else if (DrawConnection){
		int yRemind = y % UI.GRID_SIZE;
		if (yRemind <= UI.GRID_SIZE / 2)
		{
			y = y - yRemind;
		}
		else{
			y = y + (UI.GRID_SIZE - yRemind);
		}
	}
}
bool Utils::CheckPoint(GraphicsInfo r_GfxInfo, CellType usedPixels[44][74], bool fillArray){
	int xbegin = (r_GfxInfo.x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE, xend = (r_GfxInfo.x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE, ybegin = (r_GfxInfo.y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE, yend = (r_GfxInfo.y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
	for (int i = ybegin; i <= yend+1; i++)
	{
		for (int j = xbegin ; j <= xend; j++)
		{
			if (usedPixels[i][j] == GATE || (usedPixels[i][j] == HORIZONTAL || usedPixels[i][j] == VERTICAL)){
				if ((i == yend || i == ybegin) && (usedPixels[i][j] == HORIZONTAL || usedPixels[i][j] == END_CONNECTION)){}
				//else if ((j == xend || j == xbegin) && (usedPixels[i][j] == VERTICAL || usedPixels[i][j] == END_CONNECTION)){}
				else {
					return 0;
				}
			}
		}
	}
	if ((r_GfxInfo.x1 - UI.GATE_Width / 2.0) <= 0 || (r_GfxInfo.y1 - UI.GATE_Height / 2.0) <= UI.ToolBarHeight + 8 || (r_GfxInfo.x1 + UI.GATE_Width / 2.0) >= UI.width - 16 || (r_GfxInfo.y1 + UI.GATE_Height / 2.0) >= UI.height - UI.StatusBarHeight)return 0;

	if (fillArray){
		for (int i = ybegin+1; i <= yend; i++)
		{
			for (int j = xbegin; j <= xend; j++)
			{
				if (xbegin == j || xend == j)
				{
					usedPixels[i][j] = PIN;
					continue;
				}
				usedPixels[i][j] = GATE;
			}
		}
	}
	return 1;
}
bool Utils::CheckPoint(int x, int y, CellType usedPixels[44][74]) {
	if ((x - UI.GATE_Width / 2.0) <= 0 || (y - UI.GATE_Height / 2.0) <= (UI.ToolBarHeight + 20) || (x + UI.GATE_Width / 2.0) >= UI.width - 16 || (y + UI.GATE_Height / 2.0) >= (UI.height - UI.StatusBarHeight)){
		return 0;
	}
	return 1;
}
bool Utils::CheckPointForConnections(int x, int y, CellType usedPixels[44][74]) {
	if (x< 20 || y <= (UI.ToolBarHeight + 20) || (x)>UI.width - 20 || (y) >= (UI.height - UI.StatusBarHeight - 20)){
		return 0;
	}
	return 1;
}
Utils::~Utils()
{
}

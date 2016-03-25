#include "Utils.h"
#include "GUI\UI_Info.h"

Utils::Utils()
{
}
void  Utils::correctPointClicked(int &x, int &y, bool DrawGate, bool DrawConnection){
	if (DrawGate){

		int xRemind = x % UI.GRID_SIZE;
		if (xRemind <= UI.GRID_SIZE/2)
		{
			x = x - xRemind;
			x += 8;
		}
		else{
			x = x + (UI.GRID_SIZE - xRemind);
			x -= 8;
		}
		int yRemind = y % UI.GRID_SIZE;
		if (yRemind <= UI.GRID_SIZE/2)
		{
			y = y - yRemind;
			y += 8;
		}
		else{
			y = y + (UI.GRID_SIZE - yRemind);
			y -= 8;
		}

	}
	else if (DrawConnection){
		int yRemind = y % UI.GRID_SIZE;
		if (yRemind <= UI.GRID_SIZE/2)
		{
			y = y - yRemind;
		}
		else{
			y = y + (UI.GRID_SIZE - yRemind);
		}
	}
}
Utils::~Utils()
{
}

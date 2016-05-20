#include "Action.h"
#include "..\Components\Component.h"
int Action::ID = 0;
int Action::IDD = 0;
void Action::Undo()
{
	pA->setDelete(true);
	pA->Draw(pManager->GetOutput(), false);
	GraphicsInfo GInfotmp = pA->getCornersLocation();
	for (int i = GInfotmp.y1 / UI.GRID_SIZE + 1; i <= GInfotmp.y2 / UI.GRID_SIZE; i++) {
		for (int j = GInfotmp.x1 / UI.GRID_SIZE; j <= GInfotmp.x2 / UI.GRID_SIZE; j++) {
			Utils::setArrayOfComponents(i, j, NULL);
			pManager->GetOutput()->setUsedPixel(i, j, EMPTY);

		}
	}
}

void Action::Redo()
{
	pA->setDelete(false);
	pA->Draw(pManager->GetOutput(), false);
	GraphicsInfo GInfotmp = pA->getCornersLocation();
	for (int i = GInfotmp.y1 / UI.GRID_SIZE + 1; i <= GInfotmp.y2 / UI.GRID_SIZE; i++) {
		for (int j = GInfotmp.x1 / UI.GRID_SIZE; j <= GInfotmp.x2 / UI.GRID_SIZE; j++) {
			Utils::setArrayOfComponents(i, j, pA);
		}
	}
	int xbegin = (pA->getCenterLocation().x1 - UI.GATE_Width / 2.0) / UI.GRID_SIZE;
	int xend = (pA->getCenterLocation().x1 + UI.GATE_Width / 2.0) / UI.GRID_SIZE;
	int ybegin = (pA->getCenterLocation().y1 - UI.GATE_Height / 2.0) / UI.GRID_SIZE;
	int yend = (pA->getCenterLocation().y1 + UI.GATE_Height / 2.0) / UI.GRID_SIZE;
	for (int i = ybegin + 1; i <= yend; i++)
	{
		for (int j = xbegin; j <= xend; j++)
		{
			if (xbegin == j || xend == j)
			{
				if (pManager->GetOutput()->getUsedPixel(i, j) != INTERSECTION)
				{
					pManager->GetOutput()->setUsedPixel(i, j, PIN);
				}
				continue;
			}
			pManager->GetOutput()->setUsedPixel(i, j, GATE);
		}
	}
}
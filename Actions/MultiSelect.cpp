#include "MultiSelect.h"
#include "..\Utils.h"
MultiSelect::MultiSelect(ApplicationManager* pManager) :Action(pManager) {

}
bool MultiSelect::ReadActionParameters(image * smallImageBeforeAddingComponent)
{
	return true;
}

void MultiSelect::Execute()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	int x, y, initX, initY;
	pIn->getMouseCoordinates(initX, initY);
	image* originalImage = new image;
	pOut->storeDrawingAreaImage(originalImage);
	pOut->updateBuffer();
	int xOld = 0, yOld = 0;

	vector<Component*> allSelectedComponents;
	while (pIn->GetButtonStatus(LEFT_BUTTON, x, y) == BUTTON_DOWN) {
		if ((x != xOld || y != yOld) && Utils::CheckPointInBorders(x, y))
		{
			pOut->drawStoredDrawingAreaImage(originalImage);
			pOut->drawRectangle(x, y, initX, initY);
			xOld = x;
			yOld = y;
			//Get components
			allSelectedComponents.clear();
			for (size_t i = 0; i < pManager->allComponentsCorners.size(); i++)
			{
				int gateCenterX = pManager->allComponentsCorners[i].x1 + UI.GATE_Width / 2;
				int gateCenterY = pManager->allComponentsCorners[i].y1 + UI.GATE_Height / 2;
				if (
					(x > initX && gateCenterX < x && gateCenterX > initX && initY < y && gateCenterY < y && gateCenterY > initY) ||
					(x<initX && gateCenterX > x && gateCenterX < initX && initY < y && gateCenterY < y && gateCenterY > initY) ||
					(x<initX && gateCenterX > x && gateCenterX < initX && initY > y && gateCenterY > y && gateCenterY < initY) ||
					(x > initX && gateCenterX < x && gateCenterX > initX && initY > y && gateCenterY > y && gateCenterY < initY)
					) {
					if (!dynamic_cast<Connection*>(pManager->getComponent(i)))
					{
						allSelectedComponents.push_back(pManager->getComponent(i));
						pManager->getComponent(i)->Draw(pManager->GetOutput(), true);
						//cout << pManager->getComponent(i)->getLabel()<<endl;
						vector<Connection*> allInConnections, allOutConnections;
						pManager->getComponent(i)->getAllInputConnections(allInConnections);
						pManager->getComponent(i)->getAllOutputConnections(allOutConnections);
						for (size_t i = 0; i < allInConnections.size(); i++)
						{
							allInConnections[i]->selectYourSelf(pManager->GetOutput(), UI.SelectColor);
						}
						for (size_t i = 0; i < allOutConnections.size(); i++)
						{
							allOutConnections[i]->selectYourSelf(pManager->GetOutput(), UI.SelectColor);
						}
					}
				}
			}

		}
	}
	pOut->drawStoredDrawingAreaImage(originalImage);
}
void MultiSelect::Undo()
{}

void MultiSelect::Redo()
{}
MultiSelect::~MultiSelect()
{}

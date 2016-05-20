#ifndef _UTILS_H
#define _UTILS_H
#include "Defs.H"
#include "Components\Component.h"
#include "Components\Connection.h"
#include "ApplicationManager.h"
#include <chrono>
#include <ctime>
#include <thread>
class Action;
class Utils
{
public:
	Utils();
	static void correctPointClicked(int &, int &, bool DrawGate = false, bool DrawConnection = false);
	static bool CheckPoint(GraphicsInfo,Output* pOut, bool isMoving = false, bool fillArray = true);
	static bool CheckPoint(int x, int y, CellType[44][74]);
	static bool CheckPointInBorders(int x, int y);
	static GraphicsInfo* Utils::getOutputDirections(GraphicsInfo GInfo, CellType usedPixels[44][74], int noOfOutputs = 4);
	static stack <Action*> undoActions, redoActions, simulationUndoActions, simulationRedoActions;
	static Component* arrayOfComponents[44][74];
	static image* imgh;
	static int J, K, widthh ;
	static void setArrayOfComponents(int i, int j, Component*);
	static Component* getArrayOfComponents(int i, int j);
	static vector<GraphicsInfo> allComponentsCorners;
	static vector<Connection*> allConnections;
	static vector<Action*> theActions;
	~Utils();
};

class later
{
public:
	template <class callable, class... arguments>
	later(int after, bool async, callable&& f, arguments&&... args)
	{
		std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

		if (async)
		{
			std::thread([after, task]() {
				std::this_thread::sleep_for(std::chrono::milliseconds(after));
				task();
			}).detach();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(after));
			task();
		}
	}

};
#endif
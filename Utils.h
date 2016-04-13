#pragma once
#include "Defs.H"
class Utils
{
public:
	Utils();
	static void correctPointClicked(int &, int &, bool DrawGate = false, bool DrawConnection = false);
	static bool CheckPoint(GraphicsInfo, CellType[44][74], bool = true);
	static bool CheckPoint(int x, int y, CellType[44][74]);
	static bool CheckPointForConnections(int x, int y, CellType[44][74]);
	static GraphicsInfo* Utils::getOutputDirections(GraphicsInfo GInfo, CellType usedPixels[44][74], int noOfOutputs = 4);

	~Utils();
};


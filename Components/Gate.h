#ifndef _GATE_H
#define _GATE_H

/*
  Class Gate
  -----------
  - Base class for all types of gates
  - Each gate has n inputs pins and one output pin
*/

#include "Component.h"

class Gate:public Component
{

public:
	Gate(int r_Inputs, int r_FanOut, GraphicsInfo r_GfxInfo);
	~Gate();
};

#endif

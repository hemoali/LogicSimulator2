#include "Gate.h"

//Gate Constructor
//Parameters:
//r_Inputs: no. of gate's input pins
//r_FanOut: Fan out of the gate's output pin
Gate::Gate(int r_Inputs, int r_FanOut) :m_OutputPin(r_FanOut)
{
	//Allocate number of input pins (equals r_Inputs)
	m_InputPins = new InputPin[r_Inputs];
	setnumofinputs(r_Inputs);	//set no. of inputs of that gate
	//Associate all input pins to this gate
	for (int i = 0; i < getnumofinputs(); i++)
		m_InputPins[i].setComponent(this);

}
OutputPin*Gate::getoutpin()
{
	OutputPin*ptr = &m_OutputPin;
	return ptr;
}

InputPin*Gate::getinppin(int idx)
{
	InputPin*ptr = &m_InputPins[idx];
	return ptr;
}
#include "Pin.h"
#include"InputPin.h"
Pin::Pin()
{
	m_Status = UNDEFINED;
}

void Pin::setStatus(STATUS r_Status)
{
	m_Status = r_Status;
}

STATUS Pin::getStatus()
{
	return m_Status;
}
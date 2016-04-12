#pragma once
#include"Components\Component.h"
class GetComponent
{
public:
	Component * const  ptr;
	GetComponent(Component*const);

	~GetComponent();
};

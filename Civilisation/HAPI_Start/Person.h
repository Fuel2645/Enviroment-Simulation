#pragma once
#include "Entity.h"
#include "Food.h"

class Person : public Entity
{
public:
	//Functions
	bool Create(std::string EName,Vector2 PV, Type GType, float EatingTime)override;
	void Update()  override;

};


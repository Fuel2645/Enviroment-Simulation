#pragma once
#include "Entity.h"

class Food : public Entity
{
public:
	//Functions
	bool Create(std::string EName, Vector2 PV, Type GType, float EatingTime) override;
	bool Render(Graphics* GraphicsSystem) override;
	void Update() override;			  
	void FoodGrowth();
};


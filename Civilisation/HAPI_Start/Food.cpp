#include "Food.h"
#include "Graphics.h"

bool Food::Create(std::string EName, Vector2 PV, Type GType, float EatingTime) //initalisation, populating with given data 
{
	srand(time(NULL));
	m_Vpos.x = PV.x;
	m_Vpos.y = PV.y; 
	m_Name = EName;
	m_Type = GType;
	m_HP = 1;
	return true;;
}


bool Food::Render(Graphics* GraphicsSystem) //rendering in the graphics system with its specific name 
{
	GraphicsSystem->DrawSprite(m_Name, m_Vpos);
	return true;
}

void Food::Update() //death check 
{
	if (m_HP < 0)
	{
		isDead = true;
	}

}

void Food::FoodGrowth()//needs to respawn 
{
	m_Vpos = Vector2(rand() % 1024, rand() % 768); //ranodm place on the screen
	m_HP = 1;
	isDead = false;
}



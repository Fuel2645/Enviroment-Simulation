#include "Entity.h"
#include "Graphics.h"

bool Entity::Create(std::string EName, Vector2 PV, Type GTYpe, float EatingTime) //initialisation 
{
	m_Vpos = PV;
	m_Name = EName;
	return false;
}
void Entity::MoveTo(Vector2 CV) //setting values 
{
	MoveToPos = CV;
}
bool Entity::Render(Graphics* GraphicsSystem) //Drawing of specific pixel at the entities position
{
	GraphicsSystem->DrawSprite(m_Name, m_Vpos);

	return true;
}


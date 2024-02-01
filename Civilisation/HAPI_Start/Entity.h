#pragma once
#include <HAPI_lib.h>
#include "Vector2.h"
#include <ctime>

using namespace HAPISPACE;

enum State
{
	idle,
	moving,
	eating,
	dead,
	finding_food
};

enum Type
{
	Herbivore,
	Grass,
	Omnivore,
	Carnivore,
	Meat,
};


class Graphics;

class Entity
{
public:
	//Functions

	//Virtual
	virtual void Update() {};
	virtual bool Create(std::string EName, Vector2 PV, Type GTYpe, float EatingTime);
	virtual bool Render(Graphics* GraphicsSystem);

	//Getters
	const Type GetType() const { return m_Type; }
	const State GetState() const { return m_State; }
	const int GetFoodValue() const { return m_FoodValue; }
	const std::string GetName() const { return m_Name; }
	const float GetHP() const { return m_HP; }
	const bool ISDead() const { return isDead; }
	const float GetEatingTime() const { return EatingTime; }
	Vector2 GetVector() const { return m_Vpos; }

	//Setters
	void SetState(State GivenState) { m_State = GivenState; }
	void TakeDMG(float DamageToTake) { m_HP -= DamageToTake; }
	void MoveTo(Vector2 CV);


	//Variables
	HAPI_TColour m_Colour{ 0,0,0 };
	clock_t EatingTimer{ clock() };
protected:
	//Variables
	float EatingTime{ 0 };
	float m_HP{ 0 };
	int m_FoodValue = rand() % 20 + 1;
	bool isDead{ false };
	Vector2 MoveToPos;
	Vector2 m_Vpos{ 1024.f,600.f };
	Type m_Type{ Grass };
	std::string m_Name;
	State m_State{ idle };

};


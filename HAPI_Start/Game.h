#pragma once
#include <HAPI_lib.h>
#include <vector>
#include "Graphics.h"
#include <ctime>

//Pre declerations 
class Entity;

class Food;

class Game
{
private:
	//Variables
	Graphics GraphicsSystem;
	std::vector<Food*> m_Foods;
	std::vector<Entity*> m_Entities;
	Vector2 PV{ 100.f,100.f };
	clock_t FrameRateClock{ clock() };
	clock_t FoodRespawnClock{ clock() };
	clock_t BirthClock{ clock() };
	clock_t SpawnClock{ clock() };
	const float kFrameRate = 0.006 * CLOCKS_PER_SEC;
	const float kBirthRate = 1 * CLOCKS_PER_SEC;
	const float kSpawnDelay = 0.1 * CLOCKS_PER_SEC;
	const float kFoodRespawnRate = 3 * CLOCKS_PER_SEC;
	const int width{ 1024 };
	const int height{ 768 };
	//Funcitons
	void GraphicsFunction();
	void Physics();
	void CollisionDetection();
	Vector2 ClosestFood(Vector2 CurPos, Type m_type);
public:
	//Functions 
	~Game();
	bool MapLoad();
	bool Initialise();
	bool WorldLoop();

};


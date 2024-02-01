#include "Game.h"
#include "Entity.h"
#include "Person.h"
#include "Food.h"


void Game::GraphicsFunction() //Drawing Function 
{
	GraphicsSystem.ScreenClear();
	for (int x = 0; x< m_Entities.size(); x++)
	{
		if (!m_Entities[x]->ISDead()) //checking to see if its alive and worth drawing 
		{
			m_Entities[x]->Render(&GraphicsSystem); //draws teh pixel att the entities locaiton 
		}
		
	}
}

Vector2 Game::ClosestFood(Vector2 CurPos, Type g_Type) // Uses the given entity type to find the closest food depending on what it is 
{
	float closestDist{ 9999999};
	Vector2 ClosestPos{ Vector2(9999.f,9999.f)}; //values impossible to be higher than 
	float NewDist{ 100000.0f };
	for (int x = 0; x < m_Entities.size(); x++)
	{
		if (!m_Entities[x]->ISDead()) //death check 
		{
			if (g_Type == Omnivore)
			{
				if (m_Entities[x]->GetType() == Grass || m_Entities[x]->GetType() == Meat) //omnivore eats both meat pixels and grass, not other entities though
				{
					if (!m_Entities[x]->ISDead())
					{
						//Pythagorus therom to find hypotonuse without using square root to impvoe efficncy 
						NewDist = ((m_Entities[x]->GetVector().x - CurPos.x) * (m_Entities[x]->GetVector().x - CurPos.x) + (m_Entities[x]->GetVector().y - CurPos.y) * (m_Entities[x]->GetVector().y - CurPos.y)); 
						if (NewDist < closestDist)
						{
							closestDist = NewDist;
							ClosestPos = m_Entities[x]->GetVector();
						}
					}
				}
			}
			else if (g_Type == Carnivore)
			{
				if (m_Entities[x]->GetType() != Grass && m_Entities[x]->GetType() != g_Type) //eats everything but grass and other carniovores due to an issue explained in the report 
				{
					if (!m_Entities[x]->ISDead())
					{


						NewDist = ((m_Entities[x]->GetVector().x - CurPos.x) * (m_Entities[x]->GetVector().x - CurPos.x) + (m_Entities[x]->GetVector().y - CurPos.y) * (m_Entities[x]->GetVector().y - CurPos.y));
						if (NewDist < closestDist)
						{
							closestDist = NewDist;
							ClosestPos = m_Entities[x]->GetVector();
						}
					}
				}
			}
			else if (g_Type == Herbivore)
			{
				if (m_Entities[x]->GetType() == Grass) //Herbiovre grass lover 
				{
					if (!m_Entities[x]->ISDead())
					{


						NewDist = ((m_Entities[x]->GetVector().x - CurPos.x) * (m_Entities[x]->GetVector().x - CurPos.x) + (m_Entities[x]->GetVector().y - CurPos.y) * (m_Entities[x]->GetVector().y - CurPos.y));
						if (NewDist < closestDist)
						{
							closestDist = NewDist;
							ClosestPos = m_Entities[x]->GetVector();
						}
					}
				}
			}

		}
	}


	return ClosestPos; //returns value where the entity should go
}

void Game::Physics() //Main Physcis of the sim
{
	
	
	if (clock() - FrameRateClock > kFrameRate) //restricted to 150 fps using 1/150 * the macro CLOCKS_PER_SEC
	{
		if (clock() - FoodRespawnClock > kFoodRespawnRate) //same idea here but every 3 seconds to ewnsure the map doesnt get rammed with grass 
		{
			FoodRespawnClock = clock();
			int ammounttoSpawn = rand() % m_Foods.size() / 2 + 1; //random ammount each time 
			int ammountCheck{ 0 };
			for (int x = 0; x < m_Foods.size(); x++)
			{
				if (ammountCheck < ammounttoSpawn)
				{
					if (rand() % 10 <= 1)
					{
						if (m_Foods[x]->ISDead())
						{
							m_Foods[x]->FoodGrowth();
						}

					}
				}
			}
		}


		CollisionDetection();
		FrameRateClock = clock();

		for (int x = 0; x < m_Entities.size(); x++) //State machine for each entity 
		{
			switch (m_Entities[x]->GetState())
			{
			case idle:
				if (m_Entities[x]->GetType() != Meat && m_Entities[x]->GetType() != Grass)
				{
					m_Entities[x]->MoveTo(ClosestFood(m_Entities[x]->GetVector(), m_Entities[x]->GetType()));

				}
				m_Entities[x]->SetState(moving);
				break;
			case eating:
				if (clock() - m_Entities[x]->EatingTimer > m_Entities[x]->GetEatingTime()) //eating delay to improve realism of the sim changes depending on the type of entity 
				{
					if (m_Entities[x]->GetType() == Carnivore)
					{
						m_Entities[x]->MoveTo(Vector2((-200 + rand() % 200), (-200 + rand() % 200)) + m_Entities[x]->GetVector());
						m_Entities[x]->SetState(moving);
					}
					m_Entities[x]->SetState(idle);
				}
				break;
			case dead:
				break;
			case moving:
				m_Entities[x]->Update();
				break;
			default:
				break;
			}
			if (clock() - BirthClock > kBirthRate) //Entityies have an ability to give birth every second 
			{
				BirthClock = clock();
				if (m_Entities[x]->GetHP() >= 1000) //if their HP is over so much as HP is affected by how much food is eaten 
				{
					m_Entities[x]->TakeDMG(900);
					Person* newPerson = new Person;
					newPerson->Create(m_Entities[x]->GetName(), Vector2(m_Entities[x]->GetVector().x + (-20 + rand() % 20), m_Entities[x]->GetVector().y + (-20 + rand() % 20)), m_Entities[x]->GetType(), m_Entities[x]->GetEatingTime());
					m_Entities.push_back(newPerson);
				}
			}
			if (m_Entities[x]->GetHP() < 0 && m_Entities[x]->GetType() != Grass && m_Entities[x]->GetType() != Meat) //if an entity dies its respawned as a meat to resemble a caracass in real life 
			{
				Food* newFood = new Food;
				newFood->Create(std::string("Meat"), m_Entities[x]->GetVector(), Meat,0);
				delete m_Entities[x];
				m_Entities[x] = newFood;
			}
		}
		
	}
	if (clock() - SpawnClock > kSpawnDelay) //small clock used to ensure only 1 entity is spawned at a given time 
	{
		SpawnClock = clock();
		if (HAPI.GetKeyboardData().scanCode[HK_SPACE])
		{
			Person* newPerson = new Person;
			newPerson->Create("Herb", Vector2(rand()%1000, rand()%600), Herbivore, 0.5);
			
			m_Entities.push_back(newPerson);
			

		}
		if (HAPI.GetKeyboardData().scanCode[HK_ALT])
		{
			Person* newPerson = new Person;
			newPerson->Create("Omni", Vector2(rand() % 1000, rand() % 600), Omnivore, 1);
			m_Entities.push_back(newPerson);
		}
		if (HAPI.GetKeyboardData().scanCode[HK_TAB])
		{
			Person* newPerson = new Person;
			newPerson->Create("Carn", Vector2(rand() % 1000, rand() % 600), Carnivore, 2);
			m_Entities.push_back(newPerson);
		}
	}
		
	
}

void Game::CollisionDetection()
{
	for (int curEnt = 0; curEnt < m_Entities.size(); curEnt++)
	{
		if (m_Entities[curEnt]->GetType() != Grass && !m_Entities[curEnt]->ISDead()) //no need to detect collsions on both grass and dead things 
		{
			for (int nextEnt = 0; nextEnt < m_Entities.size(); nextEnt++)
			{
				if(m_Entities[nextEnt] != m_Entities[curEnt] && m_Entities[curEnt]->GetType() != m_Entities[nextEnt]->GetType() && !m_Entities[nextEnt]->ISDead())
				{ 
					if ((m_Entities[curEnt]->GetVector().x > m_Entities[nextEnt]->GetVector().x - 20) && (m_Entities[curEnt]->GetVector().x < m_Entities[nextEnt]->GetVector().x + 20))
					{
						if ((m_Entities[curEnt]->GetVector().y > m_Entities[nextEnt]->GetVector().y - 20) && (m_Entities[curEnt]->GetVector().y < m_Entities[nextEnt]->GetVector().y + 20)) //ensuring its within range of hte pixel 20 is used due to issues with the movement of the entities 
						{
							switch (m_Entities[curEnt]->GetType()) //checking to see what type it is to then detect for the correct food 
							{
							case Herbivore:
								if (m_Entities[nextEnt]->GetType() == Grass)
								{
									m_Entities[nextEnt]->TakeDMG(10);
									m_Entities[curEnt]->SetState(eating);
									m_Entities[curEnt]->TakeDMG(-m_Entities[nextEnt]->GetFoodValue());
									m_Entities[curEnt]->EatingTimer = clock();
								}
								break;
							case Omnivore:
								if (m_Entities[nextEnt]->GetType() == Meat || m_Entities[nextEnt]->GetType() == Grass)
								{
									m_Entities[nextEnt]->TakeDMG(10);
									m_Entities[curEnt]->SetState(eating);
									m_Entities[curEnt]->TakeDMG(-m_Entities[nextEnt]->GetFoodValue());
									m_Entities[curEnt]->EatingTimer = clock();
								}
								break;
							case Carnivore:
								if (m_Entities[nextEnt]->GetType() != Grass && m_Entities[curEnt]->GetType() != m_Entities[nextEnt]->GetType())
								{
									m_Entities[nextEnt]->TakeDMG(10);
									m_Entities[curEnt]->SetState(eating);
									m_Entities[curEnt]->TakeDMG(-m_Entities[nextEnt]->GetFoodValue());
									m_Entities[curEnt]->EatingTimer = clock();
								}
								break;
							default:
								break;
							}

						}
					}

				}
			}
		}
	}




}

Game::~Game() //Destructor 
{
	for (int x = 0; x < m_Entities.size(); x++)
	{
		delete m_Entities[x];
	}

	for (int x = 0; x < m_Foods.size(); x++)
	{
		delete m_Foods[x];
	}

}

bool Game::MapLoad() //populating graphcis system with the correct colours for the specific entity type
{
	GraphicsSystem.CreateSprite(std::string("Omni"), HAPI_TColour(255, 0, 255));
	GraphicsSystem.CreateSprite(std::string("Herb"), HAPI_TColour(0, 255, 255));
	GraphicsSystem.CreateSprite(std::string("Carn"), HAPI_TColour(255, 255, 0));
	GraphicsSystem.CreateSprite(std::string("Grass"), HAPI_TColour(0, 255, 0));
	GraphicsSystem.CreateSprite(std::string("Meat"), HAPI_TColour(255,0, 0));

	return true;               
}

bool Game::Initialise() //creating the window
{
	if (!GraphicsSystem.Initialise(width, height))
	{
		HAPI.UserMessage("Error creating screen", "Error code 01");
		return false;

	}
	return true;
}

bool Game::WorldLoop() //main sim loop 
{

	for (int x = 0; x < 300; x++) //poppulating with food 
	{
		Food* newFood = new Food;
		newFood->Create(std::string("Grass"), Vector2(rand()%1000, rand()%400), Grass,0);
		m_Entities.push_back(newFood);
		m_Foods.push_back(newFood); //secondary vector is used to improve the rate at whcih the respawn algoiorhtm works
		//because its a pointer if its only affected in m_entities its still affected in m_Foods
	}
	


	MapLoad(); //Loading colours into graphics 


	while (HAPI.Update()) //loop that runs whilst the window is open
	{
		Physics();
		GraphicsFunction();
	}

	return false;
}

#pragma once

#include <HAPI_lib.h>
#include <unordered_map>
#include "Entity.h"
#include "Person.h"

using namespace HAPISPACE;


class Graphics
{
public:
	//Functions
	Graphics();
	void ScreenClear();
	bool CreateSprite(const std::string& SpriteName, const HAPI_TColour Col);
	bool DrawSprite(const std::string g_Name, Vector2 PV);
	bool Initialise(int width, int height);
	~Graphics();
private:
	//Variables
	int s_Width{ 0 }, s_Height{ 0 };
	std::unordered_map<std::string,Entity*> m_Entities;
	BYTE* m_Screen = { nullptr };
};


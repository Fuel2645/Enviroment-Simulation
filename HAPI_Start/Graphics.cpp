#include "Graphics.h"


Graphics::Graphics()
{
}

bool Graphics::Initialise(int width, int height) //initialising the window and setting values 
{
	if (!HAPI.Initialise(width, height))
		return false;

	s_Width = width;
	s_Height = height;

	m_Screen = HAPI.GetScreenPointer();
	HAPI.SetShowFPS(true);


	return true;
}

Graphics::~Graphics() //destructor 
{
	std::unordered_map<std::string, Entity*>::iterator it;
	for (it = m_Entities.begin(); it != m_Entities.end(); it++)
	{
		delete it->second; //only second value needs deleating due to it being a pointer
	}
}




void Graphics::ScreenClear() //sets screen to black
{
	memset(m_Screen, 0, s_Height * s_Width * 4);
}





bool Graphics::CreateSprite(const std::string& SpriteName, HAPI_TColour Col) //adding a value to the unordered map 
{
	
	Entity* NewEntity = new Entity;
	std::unordered_map<std::string, Entity*>::iterator it;

	it = m_Entities.find(SpriteName);

	if (it == m_Entities.end())
	{
		NewEntity->m_Colour = Col;
		m_Entities[SpriteName] = NewEntity;

		return true;
	}


	return true;
	
	

	
}

bool Graphics::DrawSprite(std::string g_Name, Vector2 GV) //drawing a pixel at a given spot 
{
	if ((GV.x > 0) && (GV.x < s_Width) && (GV.y > 0) && (GV.y < s_Height))
	{
		memcpy(m_Screen + (int(GV.x) + int(GV.y) * s_Width) * 4, &m_Entities[g_Name]->m_Colour, 4); //copying 4 bytes of code into the position given 
		return true;
	}
	return false;
	
}



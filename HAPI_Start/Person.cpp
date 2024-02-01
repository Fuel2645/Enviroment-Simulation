#include "Person.h"
#include "Graphics.h"


bool Person::Create(std::string EName, Vector2 PV, Type GType, float G_EatingTime) //Inaitlisation function - populates data in the class
{
	EatingTime = G_EatingTime * CLOCKS_PER_SEC;
	m_Name = EName;
	m_Vpos = PV;
	MoveToPos = PV;
	m_Type = GType;
	m_HP = 10;
	return true;
	
}



void Person::Update() //Main physics function 
{
	if (m_HP < 0)
	{
		isDead = true;
	}
	else if (((m_Vpos.x <= MoveToPos.x - 0.25) || (m_Vpos.x >= MoveToPos.x + 0.25)) && ((m_Vpos.y <= MoveToPos.y - 0.25) || (m_Vpos.y >= MoveToPos.y + 0.25))) //detecting if its reached where it needs to go
	{
		float XDif = MoveToPos.x - m_Vpos.x; 
		float YDif = MoveToPos.y - m_Vpos.y;

		if (XDif > YDif) //chekcing to see which difference is bigger to determain the ratio 
		{
			if (YDif < 0) //negative check
			{
				m_Vpos += Vector2(XDif / YDif, 1) * -0.5;
			}
			else
			{
				m_Vpos += Vector2(XDif / YDif, 1) * 0.5;
			}
			
		}
		else
		{
			if (XDif < 0) 
			{
				m_Vpos += Vector2(1, YDif / XDif) * -0.5;
			}
			else
			{
				m_Vpos += Vector2(1, YDif / XDif) * 0.5;
			}
			
		}
		m_HP -= 0.01f; //health decday occuring from moving 
	}	
	else
	{
		m_State = eating; //if its reached where it needs to be its eating
		
	}
	
}


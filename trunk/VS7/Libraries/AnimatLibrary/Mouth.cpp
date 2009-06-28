// Mouth.cpp: implementation of the Mouth class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "Stomach.h"
#include "Mouth.h"
#include "IODataEntry.h"
#include "Microcontroller.h"
#include "Structure.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"

namespace AnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*! \brief 
   Constructs a Mouth.
   		
   \param lpParent This is a pointer to the parent rigid body of this joint. 
   \param lpChild This is a pointer to the child rigid body of this joint. 

	 \return
	 No return value.

   \remarks
	 The constructor for a Mouth joint. 
*/

Mouth::Mouth()
{
	m_strID = "MOUTH";
	m_strName = "Mouth";
	m_lpStomach = NULL;
	m_fltEatingRate = 0;
	m_fltBiteSize = 2;
	m_fltMinFoodRadius = 10;
	m_fltRadius = 1;
}


/*! \brief 
   Destroys the Mouth joint object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the Mouth joint object..	 
*/

Mouth::~Mouth()
{
	try
	{
		m_lpStomach = NULL;
	}
	catch(...)
	{Std_TraceMsg(0, "Caught Error in desctructor of Mouth\r\n", "", -1, FALSE, TRUE);}
}

void Mouth::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	if(m_lpStomach && m_fltEatingRate > 0)
	{
		//Now lets find the closest food source.
		RigidBody *lpFood = lpSim->FindClosestFoodSource(this->GetCurrentPosition(), m_fltMinFoodRadius);

		if(lpFood)
		{
			float fltBiteSize = 0;

			if(lpFood->FoodQuantity() >= m_fltEatingRate)
				fltBiteSize = m_fltEatingRate;
			else
				fltBiteSize = lpFood->FoodQuantity();

			float fltEnergy = fltBiteSize*lpFood->FoodEnergyContent();
			fltEnergy += m_lpStomach->EnergyLevel(); //Find new total energy level.

			if(fltEnergy > m_lpStomach->MaxEnergyLevel())
			{
				float fltNeededEnergy = m_lpStomach->MaxEnergyLevel() - m_lpStomach->EnergyLevel();
				fltBiteSize = fltNeededEnergy/lpFood->FoodEnergyContent();
			}

			float fltFoodQty = lpFood->FoodQuantity() - fltBiteSize;

			lpFood->Eat(fltFoodQty, lpSim->TimeSlice());
			m_lpStomach->EnergyLevel(fltEnergy);			
		}
	}
}

float *Mouth::GetDataPointer(string strDataType)
{
	string strType = Std_CheckString(strDataType);
	float *lpData = NULL;

	if(strType == "EATINGRATE")
		return &m_fltEatingRate;

	lpData = RigidBody::GetDataPointer(strDataType);

	return lpData;
}

void Mouth::AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput)
{
	m_fltEatingRate = fltInput;
	if(m_fltEatingRate < 0)
		m_fltEatingRate = 0;
}

void Mouth::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	oXml.IntoElem();  //Into RigidBody Element

	//There can only be one mouth per organism and its ID is hardcoded.
	m_strID = "MOUTH";
	m_strName = "Mouth";

	Std_LoadPoint(oXml, "RelativePosition", m_oRelPosition);
	m_oAbsPosition = m_lpParent->AbsolutePosition() + m_oRelPosition;

	m_fltMinFoodRadius = AnimatLibrary::LoadScaledNumber(oXml, "MinimumFoodRadius", FALSE, m_fltMinFoodRadius);
	m_fltMinFoodRadius *= lpSim->InverseDistanceUnits();

	Std_IsAboveMin((float) 0, m_fltMinFoodRadius, TRUE, "MinFoodRadius");

	m_fltRadius = oXml.GetChildFloat("Radius", m_fltRadius);
	Std_IsAboveMin((float) 0,m_fltRadius, TRUE, "Radius");

	m_bIsVisible = oXml.GetChildBool("IsVisible", m_bIsVisible);

	if(oXml.FindChildElement("Color", FALSE))
	{
		oXml.IntoChildElement("Color");
		m_aryColor[0] = oXml.GetAttribFloat("Red");
		m_aryColor[1] = oXml.GetAttribFloat("Green");
		m_aryColor[2] = oXml.GetAttribFloat("Blue");
		m_aryColor[3] = oXml.GetAttribFloat("Alpha");
		oXml.OutOfElem();
	}
	else
	{
		m_aryColor[0] = 1;
		m_aryColor[1] = 0;
		m_aryColor[2] = 0;
		m_aryColor[3] = 1;
	}

	oXml.OutOfElem(); //OutOf RigidBody Element
}

		}		//Joints
	}			//Environment
}				//AnimatLibrary

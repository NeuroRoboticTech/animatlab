// Plane.cpp: implementation of the Plane class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "Plane.h"
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
   Constructs a plane object..
   		
   \param lpParent This is a pointer to the parent of this rigid body. 
	          If this value is null then it is assumed that this is
						a root object and no joint is loaded to connect this
						part to the parent.

	 \return
	 No return value.

   \remarks
	 The constructor for a plane. 
*/

Plane::Plane()
{
	m_fltHeight = 0;
	m_fltMapScale = 1;
}

/*! \brief 
   Destroys the plane object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the plane object..	 
*/

Plane::~Plane()
{

}

float Plane::Height()
{return m_fltHeight;}

void Plane::Height(float fltVal)
{m_fltHeight = fltVal;}

void Plane::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	oXml.IntoElem();  //Into RigidBody Element

	m_strType = oXml.GetChildString("Type");
	m_strID = Std_CheckString(oXml.GetChildString("ID"));
	m_strName = oXml.GetChildString("Name", "");

	if(Std_IsBlank(m_strID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	if(Std_IsBlank(m_strName)) 
		m_strName = m_strID;

	m_strTexture = oXml.GetChildString("Texture", "");

	m_fltHeight = oXml.GetChildFloat("Height");

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

	m_strHeightFieldImage = oXml.GetChildString("HeightFieldImage", "");

	if(!Std_IsBlank(m_strHeightFieldImage))
	{
		Std_LoadPoint(oXml, "MapLocation", m_ptMapLocation);
		Std_LoadPoint(oXml, "MapSize", m_ptMapSize);
		m_fltMapScale = oXml.GetChildFloat("MapScale");
		lpSim->HasHeightField(TRUE);

		Std_IsAboveMin((float) 0, m_ptMapSize.x, TRUE, "MapSize.x");
		Std_IsAboveMin((float) 0, m_ptMapSize.y, TRUE, "MapSize.y");
		Std_IsAboveMin((float) 0, m_fltMapScale, TRUE, "MapScale");
	}

	oXml.OutOfElem(); //OutOf RigidBody Element

	m_oRelPosition.Set(0, m_fltHeight, 0);
	m_oAbsPosition.Set(0, m_fltHeight, 0);
	m_oRotation.Set(0, 0, 0);
}


/*! \fn float Plane::Height()
   \brief
   Height property.
      
   \remarks
	 The height of the plane on the Y axis.
	 This is the accessor function for the m_fltHeight element.
*/
/*! \fn void Plane::Height(float fltVal)
   \brief
   Height property.
      
   \remarks
	 The height of the plane on the Y axis.
	 This is the mutator function for the m_fltHeight element.
*/


		}		//Bodies
	}			//Environment
}				//AnimatLibrary
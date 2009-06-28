// Box.cpp: implementation of the Box class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "Box.h"
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
   Constructs a box object..
   		
   \param lpParent This is a pointer to the parent of this rigid body. 
	          If this value is null then it is assumed that this is
						a root object and no joint is loaded to connect this
						part to the parent.

	 \return
	 No return value.

   \remarks
	 The constructor for a box. 
*/

Box::Box()
{
}

/*! \brief 
   Destroys the box object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the box object..	 
*/

Box::~Box()
{
}


void Box::Trace(ostream &oOs)
{
	oOs << "{";
	RigidBody::Trace(oOs);
	oOs << ", ColBox: " << m_oCollisionBoxSize << ", GraphBox: " << m_oGraphicsBoxSize << "}";
}


void Box::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	RigidBody::Load(lpSim, lpStructure, oXml);

	oXml.IntoElem();  //Into RigidBody Element
	Std_LoadPoint(oXml, "CollisionBoxSize", m_oCollisionBoxSize);
	Std_LoadPoint(oXml, "GraphicsBoxSize", m_oGraphicsBoxSize);
	oXml.OutOfElem(); //OutOf RigidBody Element

	Std_IsAboveMin((float) 0, m_oCollisionBoxSize.x, TRUE, "CollisionBoxSize.x");
	Std_IsAboveMin((float) 0, m_oCollisionBoxSize.y, TRUE, "CollisionBoxSize.y");
	Std_IsAboveMin((float) 0, m_oCollisionBoxSize.z, TRUE, "CollisionBoxSize.z");
	
	Std_IsAboveMin((float) 0, m_oGraphicsBoxSize.x, TRUE, "GraphicsBoxSize.x");
	Std_IsAboveMin((float) 0, m_oGraphicsBoxSize.y, TRUE, "GraphicsBoxSize.y");
	Std_IsAboveMin((float) 0, m_oGraphicsBoxSize.z, TRUE, "GraphicsBoxSize.z");
}

/*! \fn CStdFPoint CAlBox::CollisionBoxSize()
   \brief
   CollisionBoxSize property.
      
   \remarks
   The dimension vector for the collision box.
	 This is the accessor function for the m_oCollisionBoxSize element.
*/
/*! \fn void CAlBox::CollisionBoxSize(CStdFPoint &oPoint)
   \brief
   CollisionBoxSize property.
      
   \remarks
   The dimension vector for the collision box.
	 This is the mutator function for the m_oCollisionBoxSize element.
*/


/*! \fn CStdFPoint CAlBox::GraphicsBoxSize()
   \brief
   GraphicsBoxSize property.
      
   \remarks
	 The dimension vector for the graphic box.
	 This is the accessor function for the m_oGraphicsBoxSize element.
*/
/*! \fn void CAlBox::GraphicsBoxSize(CStdFPoint &oPoint)
   \brief
   GraphicsBoxSize property.
      
   \remarks
	 The dimension vector for the graphic box.
	 This is the mutator function for the m_oGraphicsBoxSize element.
*/


		}		//Bodies
	}			//Environment
}				//AnimatLibrary

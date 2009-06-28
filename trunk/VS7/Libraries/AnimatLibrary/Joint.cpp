// Joint.cpp: implementation of the Joint class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*! \brief 
   Constructs a joint object..
   		
   \param lpParent This is a pointer to the parent rigid body of this joint. 
   \param lpChild This is a pointer to the child rigid body of this joint. 

	 \return
	 No return value.

   \remarks
	 The constructor for a joint. 
*/

Joint::Joint()
{
	m_lpParent = NULL;
	m_lpChild = NULL;
	m_fltSetVelocity = 0;
	m_fltDesiredVelocity = 0;
	m_fltMaxVelocity = 100;
	m_fltPrevVelocity = -1000000;
	m_bEnableMotor = FALSE;
	m_bEnableLimits = TRUE;
	m_bEnableLock = FALSE;
	m_fltDamping = 0;
	m_fltRestitution = 1;
	m_fltStiffness = 5e6;
}


/*! \brief 
   Destroys the joint object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the joint object..	 
*/

Joint::~Joint()
{
	//We also do not delete our references to these objects.
	m_lpParent = NULL;
	m_lpChild = NULL;
}

//Node Overrides
void Joint::AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput)
{
	m_fltDesiredVelocity += fltInput;
}

void Joint::SetVelocityToDesired()
{
	m_fltSetVelocity = m_fltDesiredVelocity;
	m_fltDesiredVelocity = 0;
}

void Joint::EnableMotor(BOOL bVal)
{m_bEnableMotor = bVal;}

void Joint::CreateJoint(Simulator *lpSim, Structure *lpStructure)
{}
			
void Joint::Reset(Simulator *lpSim, Structure *lpStructure)
{
	ResetNode(lpSim, lpStructure);
}

/*! \brief 
   Allows the joint to update itself for each timeslice.
      
   \param lpSim This is a pointer to the simulator.
   \param lpStructure This is a pointer to the structure/Organism that
                      this rigid body is a part of.
   \param lStep This is the current time slice.

	 \return
	 No return value.

	 \remarks
   This function is called for each joint on every
   time slice. It allows the joint to update itself. You need 
	 to be VERY careful to keep all code within the StepSimulation methods short, sweet, 
	 and very fast. They are in the main processing loop and even a small increase in the
	 amount of processing time that occurrs within this loop will lead to major impacts on
	 the ultimate performance of the system. 

   \sa
   Joint::StepSimulation, Simulator::StepSimulation
*/

void Joint::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	UpdateData(lpSim, lpStructure);
}


float *Joint::GetDataPointer(string strDataType)
{
	string strType = Std_CheckString(strDataType);

	if(strType == "ENABLE")
		return &m_fltEnabled;

	return NULL;
}


/*! \brief 
   Loads a joint from an xml configuration file.
      
   \param lpSim This is a pointer to the simulator.
   \param lpStructure This is a pointer to the structure/Organism that
                      this rigid body is a part of.
   \param oXml This is an xml object.

	 \return
	 No return value.

	 \remarks
	 This method is responsible for loading the joint from a XMl
	 configuration file. You should call this method even in your 
	 overriden function becuase it loads all of the base properties
	 for the Joint. 
*/

void Joint::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	oXml.IntoElem();  //Into Joint Element

	m_strType = oXml.GetChildString("Type");
	m_strID = Std_CheckString(oXml.GetChildString("ID"));
	m_strName = oXml.GetChildString("Name", "");

	if(Std_IsBlank(m_strID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	if(Std_IsBlank(m_strName)) 
		m_strName = m_strID;

	if(!m_lpParent)
		THROW_PARAM_ERROR(Al_Err_lParentNotDefined, Al_Err_strParentNotDefined, "JointID", m_strName);

	if(!m_lpChild)
		THROW_PARAM_ERROR(Al_Err_lChildNotDefined, Al_Err_strChildNotDefined, "JointID", m_strName);

	Std_LoadPoint(oXml, "RelativePosition", m_oRelPosition);
	m_oAbsPosition = m_lpChild->AbsolutePosition() + m_oRelPosition;

	m_bEnableMotor = oXml.GetChildBool("EnableMotor", m_bEnableMotor);
	m_fltMaxVelocity = AnimatLibrary::LoadScaledNumber(oXml, "MaxVelocity", FALSE, m_fltMaxVelocity);

	if(!this->UsesRadians())
			m_fltMaxVelocity *= lpSim->InverseDistanceUnits();  //Convert distance units.

	if(oXml.FindChildElement("Constraint", FALSE))
	{
		oXml.IntoElem();
		m_bEnableLimits = oXml.GetChildBool("EnableLimits", m_bEnableLimits);
		m_fltDamping = AnimatLibrary::LoadScaledNumber(oXml, "Damping", FALSE, m_fltDamping);
		m_fltRestitution = AnimatLibrary::LoadScaledNumber(oXml, "Restitution", FALSE, m_fltRestitution);
		m_fltStiffness = AnimatLibrary::LoadScaledNumber(oXml, "Stiffness", FALSE, m_fltStiffness);
		oXml.OutOfElem();
	}

	m_fltDamping *= lpSim->InverseMassUnits();
	m_fltStiffness *= lpSim->InverseMassUnits();

	oXml.OutOfElem(); //OutOf Joint Element
}


void Joint::Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
}


/*! \fn virtual void Joint::EnableMotor(BOOL bVal)
   \brief
   Enables the motor for a motorized joint.
      
   \param bVal Turns the motor on or off.

	 \return
	 No return value.

	 \remarks
	 If this is a motorized joint then when you turn it on the
	 physics engine will calculate the torque that needs to be
	 applied to this joint in order for it to have the desired
	 Velocity for its current load. This is a pure virtual function
	 that must be overridden because you will need to call the
	 physics engine API to enable/disable the joint motor.

	 \sa
	 Velocity
*/


/*! \fn virtual void Joint::CreateJoint(Simulator *lpSim, Structure *lpStructure)
   \brief
   Enables the motor for a motorized joint.
      
   \param lpSim This is a pointer to the simulator.
   \param lpStructure This is a pointer to the structure/Organism that
                      this rigid body is a part of.

	 \return
	 No return value.

	 \remarks
	 This method is used to create the joint and attach it to the
	 parent and child parts.

	 \sa
	 CreateJoints, CreateParts
*/


/*! \fn unsigned char Joint::Type()
   \brief
   Joint type property.
      
   \remarks
   The type for this joint. Examples are Static, Hinge, etc..
	 This is the read-only accessor function for the m_iType element.
*/


/*! \fn string Joint::ID()
   \brief
   Joint ID property.
      
   \remarks
	 The unique Id for this joint. It is unique for each structure, 
	 but not across structures. So you could have two joints with the
	 same ID in two different organisms.
	 This is the accessor function for the m_strID element.
*/
/*! \fn void Joint::ID(string strValue)
   \brief
   Joint ID property.
      
   \remarks
	 The unique Id for this joint. It is unique for each structure, 
	 but not across structures. So you could have two joints with the
	 same ID in two different organisms.
	 This is the mutator function for the m_strID element.
*/


/*! \fn CStdFPoint Joint::RelativePosition()
   \brief
   RelativePosition property.
      
   \remarks
	 The relative position of the of this joint
	 in relation to the center of its parent rigid body. 
	 This is the accessor function for the m_oRelPosition element.
*/
/*! \fn void Joint::RelativePosition(CStdFPoint &oPoint)
   \brief
   RelativePosition property.
      
   \remarks
	 The relative position of the of this joint
	 in relation to the center of its parent rigid body. 
	 This is the mutator function for the m_oRelPosition element.
*/


/*! \fn CStdFPoint Joint::AbsolutePosition()
   \brief
   AbsolutePosition property.
      
   \remarks
	 The absolute position of the joint in world coordinates.
	 This is calcualted during loading of the joint using the position of 
	 the parent part and the relative position specified in the configuration file.
	 This is the accessor function for the m_oAbsPosition element.
*/
/*! \fn void Joint::AbsolutePosition(CStdFPoint &oPoint)
   \brief
   AbsolutePosition property.
      
   \remarks
	 The absolute position of the joint in world coordinates.
	 This is calcualted during loading of the joint using the position of 
	 the parent part and the relative position specified in the configuration file.
	 This is the mutator function for the m_oAbsPosition element.
*/


/*! \fn Body *Joint::Parent()
   \brief
   Parent property.
      
   \remarks
	 The parent rigid body for this joint. 
	 This is the accessor function for the m_lpParent element.
*/
/*! \fn void Joint::Parent(Body *lpValue)
   \brief
   Parent property.
      
   \remarks
	 The parent rigid body for this joint. 
	 This is the mutator function for the m_lpParent element.
*/


/*! \fn Body *Joint::Child()
   \brief
   Child property.
      
   \remarks
	 The child rigid body for this joint. 
	 This is the accessor function for the m_lpChild element.
*/
/*! \fn void Joint::Child(Body *lpValue)
   \brief
   Child property.
      
   \remarks
	 The child rigid body for this joint. 
	 This is the mutator function for the m_lpChild element.
*/


/*! \fn float Joint::Velocity()
   \brief
   Velocity property.
      
   \remarks
	 This is the velocity to use for the motorized joint. The motor must be enabled
	 for this parameter to have any effect.
	 This is the accessor functions for the m_fltVelocity element.
*/
/*! \fn virtual void Joint::Velocity(float fltVelocity)
   \brief
   Velocity property.
      
   \remarks
	 This is the velocity to use for the motorized joint. The motor must be enabled
	 for this parameter to have any effect.
	 This is the mutator functions for the m_fltVelocity element.
	 The mutator function is actually a pure virtual function that needs to
	 be overloaded this is because in the particular joint when you set the
	 velocity if the motor is enabled then you need to make the physics engine
	 API calls to set the velocity of that motor.
*/

	}			//Environment
}				//AnimatLibrary

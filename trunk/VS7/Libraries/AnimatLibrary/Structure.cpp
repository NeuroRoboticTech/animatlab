// AlStructure.cpp: implementation of the Structure class.
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
#include "NeuralModule.h"
#include "Adapter.h"
#include "NervousSystem.h"
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
   Constructs an structure object..
   		
	 \return
	 No return value.

   \remarks
	 The constructor for a structure. 
*/

Structure::Structure()
{
	m_lpBody = NULL;
}


/*! \brief 
   Destroys the structure object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the structure object..	 
*/

Structure::~Structure()
{

try
{
	m_aryRigidBodies.RemoveAll();
	m_aryJoints.RemoveAll();
	m_aryExcludeCollisionList.RemoveAll();
	m_aryMicrocontrollers.RemoveAll();
	if(m_lpBody) delete m_lpBody;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Structure\r\n", "", -1, FALSE, TRUE);}
}


/*! \brief 
   Initializes all parts and joints for this structure.
      
   \param lpSim This is a pointer to the simulator.

	 \return
	 No return value.

	 \remarks
   This method creates all of the parts and joints for this structure.
	 It calls the CreateParts and CreateJoints methods on the root 
	 rigid body. It then calls it recursively on all children.

	 \sa
	 Simulator::Initialize, CreateParts, CreateJoints
*/

void Structure::Initialize(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimNotDefined, Al_Err_strSimNotDefined);

	if(m_lpBody)
	{
		//First create all of the model objects.
		m_lpBody->CreateParts(lpSim, this);

		//Then create all of the joints between the models.
		m_lpBody->CreateJoints(lpSim, this);
	}
}

void Structure::Reset(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimNotDefined, Al_Err_strSimNotDefined);

	if(m_lpBody)
		m_lpBody->Reset(lpSim, this);
}


/*! \brief 
   Allows the organism and its parts to update at each time slice.
      
   \param lpSim This is a pointer to the simulator.
   \param lStep This is the current time slice.

	 \return
	 No return value.

	 \remarks
	 This function is called for each structure/organism at every time slice.
	 It gives the structures a chance to update any values. For instance, 
	 the hydrodynamic simulation code needs to manually add drag and buoyancy
	 forces to each rigid body every time slice. You need 
	 to be VERY careful to keep all code within the StepSimulation methods short, sweet, 
	 and very fast. They are in the main processing loop and even a small increase in the
	 amount of processing time that occurrs within this loop will lead to major impacts on
	 the ultimate performance of the system. 

	 \sa
	 Simulator::StepSimulation, Body::StepSimulation, Joint::StepSimulation
*/

void Structure::StepPhysicsEngine(Simulator *lpSim)
{
	if(m_lpBody)
		m_lpBody->StepSimulation(lpSim, this);
}


/*! \brief 
   Adds a new joint to the list of all joints for this structure.
      
   \param lpJoint The new joint to be added.

	 \return
	 No return value.

	 \remarks
	 There are two reasons for this method. The first is to get a list of
	 references to all joints in this structure that is mapped to their
	 ID value. This allows us to use the STL find funtions to find joints.
	 This is more efficeient that using a loop and recursion. The second
	 reason is that this also allows us to ensure that each joint that is
	 being added has a unique ID value. If you attempt to add a joint that
	 has a ID that is already in the list then an exception will be thrown.
	 Note that this method is NOT creating the object itself, that is done
	 elsewhere. It is simply adding a reference to that created object to 
	 the list. Also, items in that list are not destroyed when the list is
	 destroyed. It is a list of references only.

	 \sa
	 AddRigidBody, AddJoint, m_aryJoints, m_aryRigidBodies, FindJoint, FindRigidBody
*/

void Structure::AddJoint(Joint *lpJoint)
{
	if(!lpJoint)
		THROW_PARAM_ERROR(Al_Err_lJointNotDefined, Al_Err_strJointNotDefined, "StructureID", m_strName);

	try
	{
		m_aryJoints.Add(lpJoint->ID(), lpJoint);
	}
	catch(CStdErrorInfo oError)
	{
		oError.m_strError += " Duplicate joint Key: " + lpJoint->Name() + " in Structure: " + m_strName; 
		THROW_ERROR(oError.m_lError, oError.m_strError);
	}
}


/*! \brief 
   Adds a new rigid body to the list of all bodies for this structure.
      
   \param lpBody The new body to be added.

	 \return
	 No return value.

	 \remarks
	 There are two reasons for this method. The first is to get a list of
	 references to all rigid bodies in this structure that is mapped to their
	 ID value. This allows us to use the STL find funtions to find bodies.
	 This is more efficeient that using a loop and recursion. The second
	 reason is that this also allows us to ensure that each body that is
	 being added has a unique ID value. If you attempt to add a  body that
	 has a ID that is already in the list then an exception will be thrown.
	 Note that this method is NOT creating the object itself, that is done
	 elsewhere. It is simply adding a reference to that created object to 
	 the list. Also, items in that list are not destroyed when the list is
	 destroyed. It is a list of references only.

	 \sa
	 AddRigidBody, AddJoint, m_aryJoints, m_aryRigidBodies, FindJoint, FindRigidBody
*/

void Structure::AddRigidBody(RigidBody *lpBody)
{
	if(!lpBody)
		THROW_PARAM_ERROR(Al_Err_lBodyNotDefined, Al_Err_strBodyNotDefined, "StructureID", m_strName);

	try
	{
		m_aryRigidBodies.Add(lpBody->ID(), lpBody);
	}
	catch(CStdErrorInfo oError)
	{
		oError.m_strError += " Duplicate rigid body Key: " + lpBody->Name() + " in Structure: " + m_strName; 
		THROW_ERROR(oError.m_lError, oError.m_strError);
	}
}


/*! \brief 
   Finds a joint with a specified ID within this structure.
      
   \param strJointID ID of the joint to find. This is not case sensitive.
	 \param bThrowError If this is TRUE and the ID is not found then an
	               exception is thrown. If this is FALSE and the ID
								 is not found then NULL is returned.

	 \return
	 returns a pointer to the joint that was found with that ID, or NULL.

	 \remarks
	 This function searches the list of joints associated with this structure 
   to find one that matches the ID in a case-insensitive manner. If it finds
	 it then it returns a pointer to that joint. If it does not find it then
	 it will either throw an exception or return NULL based on the bThrowError
	 parameter.

	 \sa
	 AddRigidBody, AddJoint, m_aryJoints, m_aryRigidBodies, FindJoint, FindRigidBody
*/

Joint *Structure::FindJoint(string strJointID, BOOL bThrowError)
{
	Joint *lpJoint = NULL;
	CStdMap<string, Joint *>::iterator oPos;
	oPos = m_aryJoints.find(Std_CheckString(strJointID));

	if(oPos != m_aryJoints.end())
		lpJoint =  oPos->second;
	else if(bThrowError)
		THROW_TEXT_ERROR(Al_Err_lJointIDNotFound, Al_Err_strJointIDNotFound, " StructureID: " + m_strName + " JointID: " + strJointID);

	return lpJoint;
}


/*! \brief 
   Finds a rigid body with a specified ID within this structure.
      
   \param strBodyID ID of the body to find. This is not case sensitive.
	 \param bThrowError If this is TRUE and the ID is not found then an
	               exception is thrown. If this is FALSE and the ID
								 is not found then NULL is returned.

	 \return
	 returns a pointer to the body that was found with that ID, or NULL.

	 \remarks
	 This function searches the list of rigid bodies associated with this structure 
   to find one that matches the ID in a case-insensitive manner. If it finds
	 it then it returns a pointer to that body. If it does not find it then
	 it will either throw an exception or return NULL based on the bThrowError
	 parameter.

	 \sa
	 AddRigidBody, AddJoint, m_aryJoints, m_aryRigidBodies, FindJoint, FindRigidBody
*/

RigidBody *Structure::FindRigidBody(string strBodyID, BOOL bThrowError)
{
	RigidBody *lpBody = NULL;
	CStdMap<string, RigidBody *>::iterator oPos;
	oPos = m_aryRigidBodies.find(Std_CheckString(strBodyID));

	if(oPos != m_aryRigidBodies.end())
		lpBody =  oPos->second;
	else if(bThrowError)
		THROW_TEXT_ERROR(Al_Err_lRigidBodyIDNotFound, Al_Err_strRigidBodyIDNotFound, " StructureID: " + m_strName + " BodyID: " + strBodyID);

	return lpBody;
}

Node *Structure::FindNode(string strID, BOOL bThrowError)
{
	Node *lpNode = FindRigidBody(strID, FALSE);

	if(lpNode)
		return lpNode;

	lpNode = FindJoint(strID, FALSE);

	if(lpNode)
		return lpNode;

	if(bThrowError)
		THROW_TEXT_ERROR(Al_Err_lRigidBodyIDNotFound, Al_Err_strRigidBodyIDNotFound, " StructureID: " + m_strName + " BodyID: " + strID);

	return NULL;
}


/*! \brief 
   Enables the motor for the specified joint.
      
	 \param strJointID The ID of the joint to enable.
   \param bVal Turns the motor on or off.

	 \return
	 No return value.

	 \remarks
   This method attempts to locate a joint within a structure and enables it.

	 \sa
	 Velocity, EnableMotor, Joint, SetJointVelocity
*/

void Structure::EnableMotor(string strJointID, BOOL bVal)
{
	Joint *lpJoint = FindJoint(strJointID);
	lpJoint->EnableMotor(bVal);
}


/*! \brief 
   Sets the velocity for the specified joint.
      
	 \param strJointID The ID of the joint to enable.
   \param fltVelocity The velocity to set this joint to use.

	 \return
	 No return value.

	 \remarks
   This method attempts to locate a joint within a structure and set its velocity.
	 The velocity will only have any effect if the motor for that joint has been
	 enabled.

	 \sa
	 Velocity, EnableMotor, Joint, SetJointVelocity
*/

void Structure::SetMotorInput(string strJointID, float fltInput)
{
	Joint *lpJoint = FindJoint(strJointID);
	lpJoint->MotorInput(fltInput);
}


/*! \brief 
   Enables collision between the past-in object and all rigid bodies of this structure.
      
   \param lpCollisionBody This is a pointer to the body to enable collisions on.

	 \return
	 No return value.

	 \remarks
	 This method enables collision responses between the rigid body being past
	 in and all rigid bodies in the structure.

   \sa
   EnableCollision, DisableCollision	
*/

void Structure::EnableCollision(RigidBody *lpCollisionBody)
{
	CStdMap<string, RigidBody *>::iterator oPos;
	RigidBody *lpBody = NULL;
	for(oPos=m_aryRigidBodies.begin(); oPos!=m_aryRigidBodies.end(); ++oPos)
	{
		lpBody = oPos->second;
		lpBody->EnableCollision(lpCollisionBody);
	}
}


/*! \brief 
   Disables collision between the past-in object and all rigid bodies of this structure.
      
   \param lpCollisionBody This is a pointer to the body to disable collisions on.

	 \return
	 No return value.

	 \remarks
	 This method disables collision responses between the rigid body being past
	 in and all rigid bodies in the structure.

   \sa
   EnableCollision, DisableCollision	
*/

void Structure::DisableCollision(RigidBody *lpCollisionBody)
{
	CStdMap<string, RigidBody *>::iterator oPos;
	RigidBody *lpBody = NULL;
	for(oPos=m_aryRigidBodies.begin(); oPos!=m_aryRigidBodies.end(); ++oPos)
	{
		lpBody = oPos->second;
		lpBody->DisableCollision(lpCollisionBody);
	}
}


long Structure::CalculateSnapshotByteSize()
{return 0;}

void Structure::SaveKeyFrameSnapshot(byte *aryBytes, long &lIndex)
{}

void Structure::LoadKeyFrameSnapshot(byte *aryBytes, long &lIndex)
{}


/*! \brief 
   Loads a structure from an xml configuration file.
      
   \param lpSim This is a pointer to the simulator.
   \param oXml This is an xml object.

	 \return
	 No return value.

	 \remarks
	 This method is responsible for loading the structure from a XMl
	 configuration file. You should call this method even in your 
	 overriden function becuase it loads all of the base properties
	 for the Body. This includes the loading the rigid bodies
	 and joints for this structure. If the Layout tag for the structure
	 is missing or blank then this method assumes that you are defining
	 the structure directly in the ASIM file instead of specifying it in
	 a seperate ASL file.
*/

void Structure::Load(Simulator *lpSim, CStdXml &oXml)
{
	oXml.IntoElem();  //Into Layout Element

	m_strID = Std_CheckString(oXml.GetChildString("ID"));
	m_strName = oXml.GetChildString("Name", "");

	m_strLayoutFile = oXml.GetChildString("BodyPlan", "");
	if(Std_IsBlank(m_strLayoutFile))
		m_strLayoutFile = oXml.GetChildString("LayoutFile", "");

	if(oXml.FindChildElement("Position", FALSE))
		Std_LoadPoint(oXml, "Position", m_oPosition);
	else
		m_oPosition.Set(0, 0, 0);

	if(Std_IsBlank(m_strID)) 
		THROW_ERROR(Al_Err_lStructureIDBlank, Al_Err_strStructureIDBlank);

	if(Std_IsBlank(m_strName)) 
		m_strName = m_strID;

	//if(Std_IsBlank(m_strProjectPath)) 
	//	THROW_PARAM_ERROR(Al_Err_lProjectPathBlank, Al_Err_strProjectPathBlank, "StructureID", m_strName);

	if(Std_IsBlank(m_strLayoutFile)) 
		LoadLayout(lpSim, oXml);
	else
		LoadLayout(lpSim);

	oXml.OutOfElem(); //OutOf Layout Element
}



/*! \fn void Structure::LoadLayout(Simulator *lpSim)
   \brief
   Loads the layout for this structure from an asl configuration file.
      
   \param lpSim This is a pointer to the simulator.

	 \return
	 No return value.

	 \remarks
	 This method opens up the Animat Structure Layout (ASL) file that was 
	 associated with this structure in the Animat Simulation (ASIM) file.
	 Once it has loaded the file into a CStdXml object it then calls
	 the second method to actually create the structure from the configuration
	 file.
*/

void Structure::LoadLayout(Simulator *lpSim)
{
	CStdXml oXml;

	TRACE_DEBUG("Loading structure config file.\r\nProjectPath: " + m_strProjectPath + "\r\nLayoutFile: " + m_strLayoutFile);

	oXml.Load(AnimatLibrary::GetFilePath(m_strProjectPath, m_strLayoutFile));

	oXml.FindElement("Structure");

	if(oXml.FindChildElement("Body", FALSE) || oXml.FindChildElement("RigidBody", FALSE))
		LoadLayout(lpSim, oXml);

	TRACE_DEBUG("Finished loading organism config file.");
}

void Structure::LoadCollisionPair(CStdXml &oXml)
{
	CollisionPair *lpPair = NULL;
	string strID1, strID2;

try
{
	oXml.IntoElem();
	strID1 = oXml.GetChildString("Part1ID");
	strID2 = oXml.GetChildString("Part2ID");
	oXml.OutOfElem();

	lpPair = new CollisionPair();
	lpPair->m_strPart1ID = strID1;
	lpPair->m_strPart2ID = strID2;

	m_aryExcludeCollisionList.Add(lpPair);
}
catch(CStdErrorInfo oError)
{
	if(lpPair) delete lpPair;
	RELAY_ERROR(oError);
}
catch(...)
{
	if(lpPair) delete lpPair;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
}
}

/*! \fn void Structure::LoadLayout(Simulator *lpSim, CStdXml &oXml)
   \brief
   Loads the layout for this structure from an asl configuration file.
      
   \param lpSim This is a pointer to the simulator.
   \param oXml This is an xml object.

	 \return
	 No return value.

	 \remarks
	 This method opens up the Animat Structure Layout (ASL) file that was 
	 associated with this structure in the Animat Simulation (ASIM) file.
	 Once it has loaded the file into a CStdXml object it then calls
	 the second method to actually create the structure from the configuration
	 file.
*/

void Structure::LoadLayout(Simulator *lpSim, CStdXml &oXml)
{
	string strModule;
	string strType;

	if(!lpSim)
		THROW_ERROR(Al_Err_lSimNotDefined, Al_Err_strSimNotDefined);

	BOOL bFound = FALSE;
	if(oXml.FindChildElement("Body", FALSE))
	{
		oXml.IntoChildElement("Body"); //Into Body Element
		bFound = TRUE;
	}
	else if(oXml.FindChildElement("RigidBody", FALSE))
	{
		oXml.IntoChildElement("RigidBody"); //Into RootBody Element
		bFound = TRUE;
	}

	if(bFound)
	{
		strModule = oXml.GetChildString("ModuleName", "");
		strType = oXml.GetChildString("Type");
		oXml.OutOfElem(); //OutOf Body Element

		m_lpBody = dynamic_cast<RigidBody *>(lpSim->CreateObject(strModule, "RigidBody", strType));
		if(!m_lpBody)
			THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "RigidBody");
		m_lpBody->Parent(NULL);

		m_lpBody->AbsolutePosition(m_oPosition);
		m_lpBody->Load(lpSim, this, oXml);
		m_lpBody->CompileIDLists(lpSim, this);

		if(oXml.FindChildElement("CollisionExclusionPairs", FALSE))
		{
			oXml.IntoElem();  //Into CollisionExclusionPairs Element
			int iChildCount = oXml.NumberOfChildren();
			for(int iIndex=0; iIndex<iChildCount; iIndex++)
			{
				oXml.FindChildByIndex(iIndex);
				LoadCollisionPair(oXml);
			}
			oXml.OutOfElem(); //OutOf CollisionExclusionPairs Element
		}

		m_aryMicrocontrollers.RemoveAll();
		if(lpSim->UseMicrocontrollers() && oXml.FindChildElement("Microcontrollers", FALSE))
		{
			oXml.IntoElem();

			int iCount = oXml.NumberOfChildren();
			for(int iIndex=0; iIndex<iCount; iIndex++)
			{
				oXml.FindChildByIndex(iIndex);
				LoadMicrocontroller(lpSim, oXml);		
			}

			oXml.OutOfElem();
		}

	}
	else
		m_lpBody = NULL;

}

Microcontroller *Structure::LoadMicrocontroller(Simulator *lpSim, CStdXml &oXml)
{
	Microcontroller *lpController = NULL;

try
{
	oXml.IntoElem();
	string strModuleName = oXml.GetChildString("ModuleName", "");
	string strType = oXml.GetChildString("Type");
	oXml.OutOfElem();

	lpController = dynamic_cast<Microcontroller *>(lpSim->CreateObject(strModuleName, "Microcontroller", strType));
	if(!lpController)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Microcontroller");
	lpController->Load(lpSim, this, oXml);
	m_aryMicrocontrollers.Add(lpController);

	return lpController;
}
catch(CStdErrorInfo oError)
{
	if(lpController) delete lpController;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpController) delete lpController;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

/*! \fn string Structure::ID()
   \brief
   ID property.
      
   \remarks
	 The unique Id for this structure.
	 This is the accessor function for the m_strID element.
*/
/*! \fn void Structure::ID(string strValue)
   \brief
   ID property.
      
   \remarks
	 The unique Id for this structure.
	 This is the mutator function for the m_strID element.
*/


/*! \fn string Structure::ProjectPath() 
   \brief
   ProjectPath property.
      
   \remarks
	 The path to the directory to use for this project.
	 This is the accessor function for the m_strProjectPath element.
*/
/*! \fn void Structure::ProjectPath(string strValue)
   \brief
   ProjectPath property.
      
   \remarks
	 The path to the directory to use for this project.
	 This is the mutator function for the m_strProjectPath element.
*/


/*! \fn string Structure::LayoutFile() 
   \brief
   LayoutFile property.
      
   \remarks
	 The filename of the layout configuration of this structure.
	 This is the accessor function for the m_strLayoutFile element.
*/
/*! \fn void Structure::LayoutFile(string strValue)
   \brief
   LayoutFile property.
      
   \remarks
	 The filename of the layout configuration of this structure.
	 This is the mutator function for the m_strLayoutFile element.
*/


/*! \fn CStdFPoint Structure::Position()
   \brief
   Position property.
      
   \remarks
		The initial position of this structure in world coordinates.
	  The root rigid body position is relative to this position.
	  This is the accessor function for the m_oPosition element.
*/
/*! \fn void Structure::Position(CStdFPoint &oPoint)
   \brief
   Position property.
      
   \remarks
		The initial position of this structure in world coordinates.
	  The root rigid body position is relative to this position.
	  This is the mutator function for the m_oPosition element.
*/


/*! \fn Body *Structure::Body()
   \brief
   Body property.
      
   \remarks
	 The root rigid body object of this structure.
	 This is the read-only accessor function for the m_lpBody element.
*/

	}			// Environment
}				//AnimatLibrary

// Body.cpp: implementation of the Body class.
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
   Constructs a Rigid Body object..
   		
   \param lpParent This is a pointer to the parent of this rigid body. 
	          If this value is null then it is assumed that this is
						a root object and no joint is loaded to connect this
						part to the parent.

	 \return
	 No return value.

   \remarks
	 The constructor for a rigid body. 
*/

RigidBody::RigidBody()
{
	m_bUsesJoint = TRUE;
	m_lpParent = NULL;
	m_fltDensity = 1.0;
	m_fltVolume = 0;
	m_fltXArea = 0;
	m_fltYArea = 0;
	m_fltZArea = 0;
	m_aryColor[0] = m_aryColor[1] = m_aryColor[2] = m_aryColor[3] = 1;
	m_lpJointToParent = NULL;
	m_bFreeze = FALSE;
	m_bIsContactSensor = FALSE;
	m_bIsVisible = TRUE;
	m_fltSurfaceContactCount= 0 ;
	m_fltLinearVelocityDamping = 0;
	m_fltAngularVelocityDamping = 0;
	m_lpContactSensor = NULL;
	m_bFoodSource = FALSE;
	m_fltFoodEaten = 0;
	m_lEatTime = 0;
	m_fltFoodQuantity = 0;
	m_fltMaxFoodQuantity = 10000;
	m_fltFoodReplenishRate = 0;
	m_fltFoodEnergyContent = 0;

	m_fltCd = 1.0;
	m_fltCdr = 1.0;
	m_fltCa = 1.0;
	m_fltCar = 1.0;
}


/*! \brief 
   Destroys the Rigid Body object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the Rigid Body object..	 
*/

RigidBody::~RigidBody()
{

try
{
	m_lpParent = NULL;

	if(m_lpJointToParent) delete m_lpJointToParent;
	if(m_lpContactSensor) delete m_lpContactSensor;
	m_aryChildParts.RemoveAll();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Body\r\n", "", -1, FALSE, TRUE);}
}

string RigidBody::TextureFile(Simulator *lpSim, string strTexture)
{
	string strExt = Std_FileExtension(strTexture);

	if(Std_IsBlank(strExt))
		return AnimatLibrary::GetFilePath(lpSim->ProjectPath(), strTexture);
	else
	{
		string strTex = strTexture;
		strTex = Std_Replace(strTex, strExt, "");
		return AnimatLibrary::GetFilePath(lpSim->ProjectPath(), strTex);
	}
}

void RigidBody::AddSurfaceContact(Simulator *lpSim, RigidBody *lpContactedSurface)
{
	m_fltSurfaceContactCount++;
}

void RigidBody::RemoveSurfaceContact(Simulator *lpSim, RigidBody *lpContactedSurface)
{
	if(m_fltSurfaceContactCount<=0)
		THROW_ERROR(Al_Err_lInvalidSurceContactCount, Al_Err_strInvalidSurceContactCount);

	m_fltSurfaceContactCount--;
}

void RigidBody::Eat(float fltVal, long lTimeSlice)
{
	if(m_lEatTime != lTimeSlice)
		m_fltFoodEaten = 0;

	m_fltFoodEaten += fltVal;
	m_lEatTime = lTimeSlice;
	m_fltFoodQuantity = fltVal;
}

	
/*! \brief 
   Enables collision between the past-in object and this object.
      
   \param lpBody This is a pointer to the body to enable collisions on.

	 \return
	 No return value.

	 \remarks
	 This method enables collision responses between the rigid body being past
	 in and this rigid body. This is a virtual method that should be overridden 
	 in the simulator system. You need to call physics engine API's to enable
	 the collision responses between these two objects. This method does nothing
	 by default.

   \sa
   EnableCollision, DisableCollision	
*/

void RigidBody::EnableCollision(RigidBody *lpBody)
{}


/*! \brief 
   Disables collision between the past-in object and this object.
      
   \param lpBody This is a pointer to the body to disable collisions on.

	 \return
	 No return value.

	 \remarks
	 This method disables collision responses between the rigid body being past
	 in and this rigid body. This is a virtual method that should be overridden 
	 in the simulator system. You need to call physics engine API's to disable
	 the collision responses between these two objects. This method does nothing
	 by default.

   \sa
   EnableCollision, DisableCollision	
*/

void RigidBody::DisableCollision(RigidBody *lpBody)
{}

void RigidBody::Kill(Simulator *lpSim, Organism *lpOrganism, BOOL bState)
{
	Node::Kill(lpSim, lpOrganism, bState);

	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->Kill(lpSim, lpOrganism, bState);

	if(m_lpJointToParent)
		m_lpJointToParent->Kill(lpSim, lpOrganism, bState);
}

void RigidBody::Reset(Simulator *lpSim, Structure *lpStructure)
{
	ResetNode(lpSim, lpStructure);

	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->Reset(lpSim, lpStructure);

	if(m_lpJointToParent)
		m_lpJointToParent->Reset(lpSim, lpStructure);
}

/*! \brief 
   Allows the rigid body to create its parts using the chosen physics engine.
      
   \param lpSim This is a pointer to the simulator.
   \param lpStructure This is a pointer to the structure/Organism that
                  this rigid body is a part of.

	 \return
	 No return value.

	 \remarks
	 This function can not be truly implemented in the Animat library. It must
	 be implemented in the next layer sitting above it. The reason for this is
	 that the Animat library was made to be generalized so it could work with a
	 number of different physics engines. Therefore it is not tightly coupled with
	 any one engine. This in turn means that we can not implement the code in this
	 library neccessary to create a part or joint in the chosen engine. Several
	 overridable functions have been provided that allow you to do this. The two
	 that will always have to be overridden are the CreateParts and CreateJoints
	 methods. CreateParts makes the API calls to the physics engine to create the
	 collision models, graphics models and so on. You should still call the 
	 base class method at the end of your overridden method so the rigid body
	 can walk down the tree and create the parts for its children.

   \sa
   CreateParts, CreateJoints, StepSimulation	
*/

void RigidBody::CreateParts(Simulator *lpSim, Structure *lpStructure)
{
	if(m_bFoodSource)
	{
		lpSim->AddFoodSource(this);

		//We have the replenish rate in Quantity/s, but we need it in Quantity/timeslice. Lets recalculate it here.
		m_fltFoodReplenishRate = (m_fltFoodReplenishRate * lpSim->PhysicsTimeStep());
	}

	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->CreateParts(lpSim, lpStructure);
}

/*! \brief 
   Allows the rigid body to create its joints using the chosen physics engine.
      
   \param lpSim This is a pointer to the simulator.
   \param lpStructure This is a pointer to the structure/Organism that
                  this rigid body is a part of.

	 \return
	 No return value.

	 \remarks
	 This function can not be truly implemented in the Animat library. It must
	 be implemented in the next layer sitting above it. The reason for this is
	 that the Animat library was made to be generalized so it could work with a
	 number of different physics engines. Therefore it is not tightly coupled with
	 any one engine. This in turn means that we can not implement the code in this
	 library neccessary to create a part or joint in the chosen engine. Several
	 overridable functions have been provided that allow you to do this. The two
	 that will always have to be overridden are the CreateParts and CreateJoints
	 methods. CreateJoints makes the API calls to the physics engine to create the
	 joint and constraints and motors. You should still call the 
	 base class method at the end of your overridden method so the rigid body
	 can walk down the tree and create the joints for its children.

   \sa
   CreateParts, CreateJoints, StepSimulation	
*/

void RigidBody::CreateJoints(Simulator *lpSim, Structure *lpStructure)
{
	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->CreateJoints(lpSim, lpStructure);
}


/*! \brief 
   Copies the base value data elements of this rigid body.
   		
	 \param lpOrig This is the rigid body to copy.

	 \return
	 No return value.

   \remarks
   Copies the base value data elements of this rigid body.
*/

void RigidBody::Copy(RigidBody *lpOrig)
{
	m_strID = lpOrig->m_strID;
	m_strName = lpOrig->m_strName;
	m_oRelPosition = lpOrig->m_oRelPosition;
	m_oAbsPosition = lpOrig->m_oAbsPosition;
	m_oCenterOfMass = lpOrig->m_oCenterOfMass;
	m_fltDensity = lpOrig->m_fltDensity;
	m_lpParent = lpOrig->m_lpParent;
	m_lpJointToParent = lpOrig->m_lpJointToParent;
	CopyPtrArray(lpOrig->m_aryChildParts, m_aryChildParts);
}


/*! \brief 
   Dumps the key values of this object to a text stream.
      
   \param oOs Text stream.

	 \return
	 No return value.

	 \remarks
	 This method is used to trace the key values of an object.
	 You can dump these values to the debug window in Visual Studio or
	 to a log file. When you trace an array of objects this method is
	 automatically invoked for each object in the array.
*/

void RigidBody::Trace(ostream &oOs)
{
	/*
	ClassFactory *lpFactory = Al_ClassFactory();

	oOs << "ID: " << m_strName  << ", Type: " << lpFactory->RigidBodyTypeAbbrev(m_iType);
	oOs << ", RelPos: " << m_oRelPosition << ", AbsPos: " << m_oAbsPosition;
	oOs << ", Rot: " << m_oRotation << ", " << m_fltDensity;
	oOs << ", Cd: " << m_fltCd << ", Volume: " << m_fltVolume;
	oOs << ", Area (" << m_fltXArea << ", " <<m_fltYArea << ", " << m_fltZArea;
	oOs << "), Freeze: " << m_bFreeze;

	if(m_lpParent)
		oOs << ", Parent: " << m_lpParent->ID();

	if(m_lpJointToParent)
		oOs << ", Joint: " << m_lpJointToParent->ID();
	*/
}

//Node Overrides

void RigidBody::AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput)
{
}

/*! \brief 
   Allows the rigid body to update itself for each timeslice.
      
   \param lpSim This is a pointer to the simulator.
   \param lpStructure This is a pointer to the structure/Organism that
                  this rigid body is a part of.
   \param lStep This is the current time slice.

	 \return
	 No return value.

	 \remarks
   This function is called for each rigid body on every
   time slice. It allows the body to update itself. For
   instance, if this is a muscle type of rigid body that is
   connected to a motor neuron then it may need to adjust the
   force it is applying based on the firing frequency of that
   neuron. If you are doing hydrodynamics then you the bodies
   will need to calcuate the buoyancy and drag forces to apply
   to simulate those effects. You need 
	 to be VERY careful to keep all code within the StepSimulation methods short, sweet, 
	 and very fast. They are in the main processing loop and even a small increase in the
	 amount of processing time that occurrs within this loop will lead to major impacts on
	 the ultimate performance of the system. 

   \sa
   Joint::StepSimulation, Simulator::StepPhysicsEngine
*/

void RigidBody::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	if(m_bFoodSource)
	{
		m_fltFoodQuantity = m_fltFoodQuantity + m_fltFoodReplenishRate;
		if(m_fltFoodQuantity > m_fltMaxFoodQuantity)
			m_fltFoodQuantity = m_fltMaxFoodQuantity;

		//Clear the food eaten variable if it has been around for too long.
		if(m_fltFoodEaten && m_lEatTime != lpSim->TimeSlice())
			m_fltFoodEaten = 0;
	}

	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->StepSimulation(lpSim, lpStructure);

	if(m_lpJointToParent)
		m_lpJointToParent->StepSimulation(lpSim, lpStructure);

	UpdateData(lpSim, lpStructure);
}


float *RigidBody::GetDataPointer(string strDataType)
{
	string strType = Std_CheckString(strDataType);

	if(strType == "FOODQUANTITY")
		return &m_fltFoodQuantity;
	if(strType == "FOODEATEN")
		return &m_fltFoodEaten;
	if(strType == "ENABLE")
		return &m_fltEnabled;

	return NULL;
}

/*! \brief 
   Loads a rigid body from an xml configuration file.
      
   \param lpSim This is a pointer to the simulator.
   \param lpStructure This is a pointer to the structure/Organism that
                  this rigid body is a part of.
   \param oXml This is an xml object.

	 \return
	 No return value.

	 \remarks
	 This method is responsible for loading the rigid body from a XMl
	 configuration file. You should call this method even in your 
	 overriden function becuase it loads all of the base properties
	 for the Body. This includes the functionality to load the
	 joint and any children.
*/

void RigidBody::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	m_fltDensity = 0;
	if(m_lpJointToParent) {delete m_lpJointToParent; m_lpJointToParent=NULL;}
	m_aryChildParts.RemoveAll();

	oXml.IntoElem();  //Into RigidBody Element

	m_strType = oXml.GetChildString("Type");
	m_strID = Std_CheckString(oXml.GetChildString("ID"));
	m_strName = oXml.GetChildString("Name", "");

	if(Std_IsBlank(m_strID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	if(Std_IsBlank(m_strName)) 
		m_strName = m_strID;

	m_strTexture = oXml.GetChildString("Texture", "");

	if(oXml.FindChildElement("RelativePosition", FALSE))
		Std_LoadPoint(oXml, "RelativePosition", m_oRelPosition);
	else
		m_oRelPosition.Set(0, 0, 0);

	if(!m_lpParent)
		m_oAbsPosition += m_oRelPosition;
	else
		m_oAbsPosition = m_lpParent->AbsolutePosition() + m_oRelPosition;

	if(oXml.FindChildElement("Rotation", FALSE))
		Std_LoadPoint(oXml, "Rotation", m_oRotation);
	else
		m_oRotation.Set(0, 0, 0);

	if(oXml.FindChildElement("LocalPosition", FALSE))
		Std_LoadPoint(oXml, "LocalPosition", m_oLocalPosition);
	else
		m_oLocalPosition.Set(0, 0, 0);

	if(oXml.FindChildElement("LocalRotation", FALSE))
		Std_LoadPoint(oXml, "LocalRotation", m_oLocalRotation);
	else
		m_oLocalRotation.Set(0, 0, 0);

	if(oXml.FindChildElement("XCenterOfMass", FALSE))
			m_oCenterOfMass.x = AnimatLibrary::LoadScaledNumber(oXml, "XCenterOfMass", FALSE, m_fltDensity);
	else
		m_oCenterOfMass.x = 0;

	if(oXml.FindChildElement("YCenterOfMass", FALSE))
			m_oCenterOfMass.y = AnimatLibrary::LoadScaledNumber(oXml, "YCenterOfMass", FALSE, m_fltDensity);
	else
		m_oCenterOfMass.y = 0;

	if(oXml.FindChildElement("ZCenterOfMass", FALSE))
			m_oCenterOfMass.z = AnimatLibrary::LoadScaledNumber(oXml, "ZCenterOfMass", FALSE, m_fltDensity);
	else
		m_oCenterOfMass.z = 0;

	m_oCenterOfMass.x *= lpSim->InverseDistanceUnits();
	m_oCenterOfMass.y *= lpSim->InverseDistanceUnits();
	m_oCenterOfMass.z *= lpSim->InverseDistanceUnits();

	m_fltDensity = AnimatLibrary::LoadScaledNumber(oXml, "Density", FALSE, m_fltDensity);
	m_fltDensity /= lpSim->DensityMassUnits();	//Scale the mass units down to one. If we are using Kg then the editor will save it out as 1000. We need this to be 1 Kg though.
	m_fltDensity *=  pow(lpSim->DenominatorDistanceUnits(), 3); //Perform a conversion if necessary because we may be using different units in the denominator.

	Std_IsAboveMin((float) 0, m_fltDensity, TRUE, "Density");

	m_fltCd = oXml.GetChildFloat("Cd", m_fltCd);
	m_fltCdr = oXml.GetChildFloat("Cdr", m_fltCdr);
	m_fltCa = oXml.GetChildFloat("Ca", m_fltCa);
	m_fltCar = oXml.GetChildFloat("Car", m_fltCar);

	if(lpSim->SimulateHydrodynamics())
	{
		Std_IsAboveMin((float) 0, m_fltCd, TRUE, "Cd", true);
		Std_IsAboveMin((float) 0, m_fltCdr, TRUE, "Cdr", true);
		Std_IsAboveMin((float) 0, m_fltCa, TRUE, "Ca", true);
		Std_IsAboveMin((float) 0, m_fltCar, TRUE, "Car", true);
	}

	m_bFreeze = oXml.GetChildBool("Freeze", m_bFreeze);
	m_bIsVisible = oXml.GetChildBool("IsVisible", m_bIsVisible);

	m_bFoodSource = oXml.GetChildBool("FoodSource", m_bFoodSource);
	m_fltFoodQuantity = AnimatLibrary::LoadScaledNumber(oXml, "FoodQuantity", FALSE, m_fltFoodQuantity);
	m_fltMaxFoodQuantity = AnimatLibrary::LoadScaledNumber(oXml, "MaxFoodQuantity", FALSE, m_fltMaxFoodQuantity);
	m_fltFoodReplenishRate = AnimatLibrary::LoadScaledNumber(oXml, "FoodReplenishRate", FALSE, m_fltFoodReplenishRate);
	m_fltFoodEnergyContent = AnimatLibrary::LoadScaledNumber(oXml, "FoodEnergyContent", FALSE, m_fltFoodEnergyContent);

	Std_InValidRange((float) 0, (float) 100000, m_fltFoodQuantity, TRUE, "FoodQuantity");
	Std_InValidRange((float) 0, (float) 100000, m_fltFoodQuantity, TRUE, "MaxFoodQuantity");
	Std_InValidRange((float) 0, (float) 100000, m_fltFoodReplenishRate, TRUE, "FoodReplenishRate");
	Std_InValidRange((float) 0, (float) 100000, m_fltFoodEnergyContent, TRUE, "FoodEnergyContent");

	m_fltLinearVelocityDamping = oXml.GetChildFloat("LinearVelocityDamping", m_fltLinearVelocityDamping);
	m_fltAngularVelocityDamping = oXml.GetChildFloat("AngularVelocityDamping", m_fltAngularVelocityDamping);

	Std_InValidRange((float) 0, (float) 1000, m_fltLinearVelocityDamping, TRUE, "LinearVelocityDamping");
	Std_InValidRange((float) 0, (float) 1000, m_fltAngularVelocityDamping, TRUE, "AngularVelocityDamping");

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

	//Only load the joint if there is a parent object and this body uses joints.
	if(m_lpParent && m_bUsesJoint)
	{
		oXml.IntoChildElement("Joint"); //Into Joint Element
		string strModule = oXml.GetChildString("ModuleName", "");
		string strJointType = oXml.GetChildString("Type");
		oXml.OutOfElem();  //OutOf Joint Element

		m_lpJointToParent = dynamic_cast<Joint *>(lpSim->CreateObject(strModule, "Joint", strJointType));
		if(!m_lpJointToParent)
			THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Joint");
		m_lpJointToParent->Parent(m_lpParent);
		m_lpJointToParent->Child(this);

		m_lpJointToParent->Load(lpSim, lpStructure, oXml);
	}

	if(oXml.FindChildElement("ChildBodies", FALSE))
	{
		oXml.IntoElem();  //Into ChildBodies Element
		int iChildCount = oXml.NumberOfChildren();

		for(int iIndex=0; iIndex<iChildCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			LoadRigidBody(lpSim, lpStructure, oXml);
		}
		oXml.OutOfElem(); //OutOf ChildBodies Element
	}

	if(oXml.FindChildElement("ContactSensor", FALSE))
	{
		m_lpContactSensor = new AnimatLibrary::Environment::ContactSensor();
		m_lpContactSensor->Load(lpSim, lpStructure, oXml);
	}

	if(oXml.FindChildElement("OdorSources", FALSE))
	{
		oXml.IntoElem();  //Into OdorSources Element
		int iChildCount = oXml.NumberOfChildren();
		
		for(int iIndex=0; iIndex<iChildCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			LoadOdor(lpSim, oXml);
		}
		oXml.OutOfElem(); //OutOf OdorSources Element
	}	

	oXml.OutOfElem(); //OutOf RigidBody Element
}

RigidBody *RigidBody::LoadRigidBody(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	RigidBody *lpChild = NULL;
	string strType;

try
{
	oXml.IntoElem(); //Into Child Element
	string strModule = oXml.GetChildString("ModuleName", "");
	strType = oXml.GetChildString("Type");
	oXml.OutOfElem(); //OutOf Child Element

	lpChild = dynamic_cast<RigidBody *>(lpSim->CreateObject(strModule, "RigidBody", strType));
	if(!lpChild)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "RigidBody");
	lpChild->Parent(this);

	lpChild->Load(lpSim, lpStructure, oXml);

	m_aryChildParts.Add(lpChild);

	return lpChild;
}
catch(CStdErrorInfo oError)
{
	if(lpChild) delete lpChild;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpChild) delete lpChild;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

void RigidBody::CompileIDLists(Simulator *lpSim, Structure *lpStructure)
{
	if(m_lpJointToParent)
		lpStructure->AddJoint(m_lpJointToParent);

	//Add me and then add child parts
	lpStructure->AddRigidBody(this);

	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->CompileIDLists(lpSim, lpStructure);
}

void RigidBody::AddOdor(Odor *lpOdor)
{
	if(!lpOdor)
		THROW_ERROR(Al_Err_lOdorNotDefined, Al_Err_strOdorNotDefined);

	try
	{
			m_aryOdorSources.Add(lpOdor->ID(), lpOdor);
	}
	catch(CStdErrorInfo oError)
	{
		oError.m_strError += " Duplicate odor type Key: " + lpOdor->ID(); 
		RELAY_ERROR(oError);
	}
}

Odor *RigidBody::LoadOdor(Simulator *lpSim, CStdXml &oXml)
{
	Odor *lpOdor = NULL;

try
{
	lpOdor = new Odor(this);
	lpOdor->Load(lpSim, oXml);
	AddOdor(lpOdor);

	return lpOdor;
}
catch(CStdErrorInfo oError)
{
	if(lpOdor) delete lpOdor;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpOdor) delete lpOdor;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

void RigidBody::Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	//Currently not implemented
}

float *RigidBody::LoadMeshVertices(CStdXml &oXml, string strTagName, int &iVertCount, BOOL bThrowError)
{
	float *aryVerts = NULL;

	if(oXml.FindChildElement(strTagName, bThrowError))
	{
		oXml.IntoChildElement(strTagName);

		iVertCount = oXml.NumberOfChildren();

		if(!iVertCount)
			THROW_PARAM_ERROR(Al_Err_lNoVerticesDefined, Al_Err_strNoVerticesDefined, "BodyID", m_strName);

		aryVerts = new float[3*iVertCount];

		int iVerIndex=0;
		for(int iIndex=0; iIndex<iVertCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);

			aryVerts[iVerIndex] = oXml.GetChildAttribFloat("x");
			aryVerts[iVerIndex+1] = oXml.GetChildAttribFloat("y");
			aryVerts[iVerIndex+2] = oXml.GetChildAttribFloat("z");
			iVerIndex+=3;
		}

		oXml.OutOfElem();
	}

	return aryVerts;
}

void RigidBody::AddForce(Simulator *lpSim, float fltPx, float fltPy, float fltPz, float fltFx, float fltFy, float fltFz)
{}

void RigidBody::AddTorque(Simulator *lpSim, float fltTx, float fltTy, float fltTz)
{}

/*! \fn unsigned char Body::Type()
   \brief
   Rigid body type property.
      
   \remarks
	 The type for this rigid body. Examples are Box, Plane, etc.. 
	 This is the read-only accessor function for the m_iType element.
*/


/*! \fn string Body::ID()
   \brief
   Rigid body ID property.
      
   \remarks
	 The unique Id for this body. It is unique for each structure, 
	 but not across structures. So you could have two rigid bodies with the
	 same ID in two different organisms.
	 This is the accessor function for the m_strID element.
*/
/*! \fn void Body::ID(string strValue)
   \brief
   Rigid body Body::ID property.
      
   \remarks
	 The unique Id for this body. It is unique for each structure, 
	 but not across structures. So you could have two rigid bodies with the
	 same ID in two different organisms.
	 This is the mutator function for the m_strID element.
*/


/*! \fn string Body::Texture() 
   \brief
   Texture property.
      
   \remarks
	 An optional texture to apply to the rigid body.
	 This is the accessor function for the m_strTexture element.
*/
/*! \fn void Body::Texture(string strValue)
   \brief
   Texture property.
      
   \remarks
	 An optional texture to apply to the rigid body.
	 This is the mutator function for the m_strTexture element.
*/


/*! \fn CStdFPoint Body::RelativePosition()
   \brief
   RelativePosition property.
      
   \remarks
	 The relative position of the center of this rigid body
	 in relation to the center of its parent rigid body. 
	 This is the accessor function for the m_oRelPosition element.
*/
/*! \fn void Body::RelativePosition(CStdFPoint &oPoint)
   \brief
   RelativePosition property.
      
   \remarks
	 The relative position of the center of this rigid body
	 in relation to the center of its parent rigid body. 
	 This is the mutator function for the m_oRelPosition element.
*/


/*! \fn CStdFPoint Body::AbsolutePosition()
   \brief
   AbsolutePosition property.
      
   \remarks
	 The absolute position of the rigid body in world coordinates.
	 This is calcualted during loading of the part using the position of 
	 the parent part and the relative position specified in the configuration file.
	 This is the accessor function for the m_oAbsPosition element.
*/
/*! \fn void Body::AbsolutePosition(CStdFPoint &oPoint)
   \brief
   AbsolutePosition property.
      
   \remarks
	 The absolute position of the rigid body in world coordinates.
	 This is calcualted during loading of the part using the position of 
	 the parent part and the relative position specified in the configuration file.
	 This is the mutator function for the m_oAbsPosition element.
*/


/*! \fn CStdFPoint Body::Rotation()	
   \brief
   Rotation property.
      
   \remarks
	 The rotation to apply to this rigid body. It is defined by the three
	 euler angles in radians.
	 This is the accessor function for the m_oRotation element.
*/
/*! \fn void Body::Rotation(CStdFPoint &oPoint)
   \brief
   Rotation property.
      
   \remarks
	 The rotation to apply to this rigid body. It is defined by the three
	 euler angles in radians.
	 This is the mutator function for the m_oRotation element.
*/


/*! \fn Body *Body::Parent()
   \brief
   Parent property.
      
   \remarks
	 The parent rigid body of this part. If this value is null
	 then it is assumed that this is the root part of a structure.
	 This is the accessor function for the m_lpParent element.
*/
/*! \fn void Body::Parent(Body *lpValue)
   \brief
   Parent property.
      
   \remarks
	 The parent rigid body of this part. If this value is null
	 then it is assumed that this is the root part of a structure.
	 This is the mutator function for the m_lpParent element.
*/


/*! \fn float Body::Density()
   \brief
   Density property.
      
   \remarks
	 Uniform density for the rigid body.
	 This is the accessor function for the m_fltDensity element.
*/
/*! \fn void Body::Density(float fltVal)
   \brief
   Density property.
      
   \remarks
	 Uniform density for the rigid body.
	 This is the mutator function for the m_fltDensity element.
*/


/*! \fn float Body::Cd() 
   \brief
   Cd property.
      
   \remarks
	 Drag Coefficient
	 This is the accessor function for the m_fltCd element.
*/
/*! \fn void Body::Cd(float fltVal)
   \brief
   Cd property.
      
   \remarks
	 Drag Coefficient
	 This is the mutator function for the m_fltCd element.
*/


/*! \fn float Body::Volume()
   \brief
   Volume property.
      
   \remarks
	 Total volume for the rigid body. This is used in calculating the buoyancy.
	 This is the read-only accessor function for the m_fltVolume element.
*/


/*! \fn float Body::XArea()
   \brief
   XArea property.
      
   \remarks
	 The area of this rigid body in the x direction. This is used to calculate the
	 drag force in this direction.
	 This is the read-only accessor function for the m_fltXArea element.
*/


/*! \fn float Body::YArea()
   \brief
   YArea property.
       
   \remarks
	 The area of this rigid body in the y direction. This is used to calculate the
	 drag force in this direction.
	 This is the read-only accessor function for the m_fltYArea element.
*/


/*! \fn float Body::ZArea()
   \brief
   ZArea property.
      
   \remarks
	 The area of this rigid body in the z direction. This is used to calculate the
	 drag force in this direction.
	 This is the read-only accessor function for the m_fltZArea element.
*/


/*! \fn BOOL Body::Freeze()
   \brief
   Freeze property.
      
   \remarks
	 Specifies if the part should frozen in place to the world. If a rigid body 
	 is frozen then it is as if it is nailed in place and can not move. Gravity and 
	 and other forces will not act on it.
	 This is the accessor function for the m_bFreeze element.
*/
/*! \fn void Body::Freeze(BOOL bVal)
   \brief
   Freeze property.
      
   \remarks
	 Specifies if the part should frozen in place to the world. If a rigid body 
	 is frozen then it is as if it is nailed in place and can not move. Gravity and 
	 and other forces will not act on it.
	 This is the mutator function for the m_bFreeze element.
*/

	}			//Environment
}				//AnimatLibrary
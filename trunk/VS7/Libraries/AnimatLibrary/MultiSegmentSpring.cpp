// MultiSegmentSpring.cpp: implementation of the MultiSegmentSpring class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "MuscleAttachment.h"
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
#include "ExternalStimulus.h"
#include "MultiSegmentSpring.h"

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
   Constructs a MultiSegmentSpring joint.
   		
   \param lpParent This is a pointer to the parent rigid body of this joint. 
   \param lpChild This is a pointer to the child rigid body of this joint. 

	 \return
	 No return value.

   \remarks
	 The constructor for a MultiSegmentSpring joint. 
*/

MultiSegmentSpring::MultiSegmentSpring()
{
	m_bInitEnabled = FALSE;
	m_fltNaturalLength = 1;
	m_fltStiffness = 5000;
	m_fltDamping = 1000;
	m_fltLength = 0;
	m_fltDisplacement = 0;
	m_fltTension = 0;
	m_fltSpringLength = 0;
	m_fltPrevSpringLength = 0;
	m_fltVspring = 0;
	m_fltPrevVspring = 0;
	m_fltAspring = 0;
	m_fltPotEnergy = 0;
	m_fltSpringTension = 0;
	m_fltDampingTension = 0;

	m_bEnabled = TRUE;
	m_aryColor[0] = 0;
	m_aryColor[1] = 1;
	m_aryColor[2] = 0;
	m_aryColor[3] = 1;
	m_fltDensity = 0;
	m_lpJointToParent = NULL;
}


/*! \brief 
   Destroys the Spring joint object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the Spring joint object..	 
*/

MultiSegmentSpring::~MultiSegmentSpring()
{
	try
	{
		m_aryAttachmentPointIDs.Clear();
		m_aryAttachmentPoints.Clear();
	}
	catch(...)
	{Std_TraceMsg(0, "Caught Error in desctructor of MultiSegmentSpring\r\n", "", -1, FALSE, TRUE);}
}

void MultiSegmentSpring::Enabled(BOOL bVal)
{
	m_bEnabled = bVal;
	m_fltEnabled = (float) bVal;

	if(!bVal)
	{
		m_fltTension = 0;
		m_fltVspring = 0;
		m_fltPrevVspring = 0;
		m_fltAspring = 0;
		m_fltPotEnergy = 0;
		m_fltSpringTension = 0;
		m_fltDampingTension = 0;
	}
}

float MultiSegmentSpring::CalculateSpringLength(Simulator *lpSim)
{
	float fltLength=0;

	int iCount = m_aryAttachmentPoints.GetSize();

	if(iCount<2)
		return 0;

	MuscleAttachment *lpAttach1 = m_aryAttachmentPoints[0], *lpAttach2 = NULL;
	for(int iIndex=1; iIndex<iCount; iIndex++)
	{
		lpAttach2 = m_aryAttachmentPoints[iIndex];
		fltLength += Std_CalculateDistance(lpAttach1->CalculatePosition(lpSim), lpAttach2->CalculatePosition(lpSim));
		lpAttach1 = lpAttach2;
	}

	return (fltLength * lpSim->DistanceUnits());
}

void MultiSegmentSpring::CalculateTension(Simulator *lpSim)
{
	//Store the previous spring length
	m_fltPrevSpringLength = m_fltSpringLength;

	//Calculate the current spring length.
	m_fltSpringLength = CalculateSpringLength(lpSim);

	//Calculate the displacement of this muscle d = (x-x*)
	m_fltDisplacement = m_fltSpringLength-m_fltNaturalLength;
	m_fltDisplacementRatio = m_fltSpringLength/m_fltNaturalLength;

	//store the previous spring velocity
	m_fltPrevVspring = m_fltVspring;

	//Calculate the velocity of change of the spring length.
	m_fltVspring = (m_fltSpringLength-m_fltPrevSpringLength)/lpSim->PhysicsTimeStep();

	//Calculate the acceleration of the spring and the potential energy stored in the spring.
	m_fltAspring = (m_fltVspring-m_fltPrevVspring)/lpSim->PhysicsTimeStep();
	m_fltPotEnergy = 0.5*m_fltStiffness*m_fltDisplacement*m_fltDisplacement;

	//Calculation the tension using a standard equation 
	//T = -kx - bv. k is stiffness, x is displacement of the spring,
	//b is damping, and v is the velocity
	m_fltSpringTension = -m_fltStiffness*m_fltDisplacement;
	m_fltDampingTension = -m_fltDamping*m_fltVspring;

	m_fltTension = m_fltSpringTension + m_fltDampingTension;  

	//Make certain that the tension never exceed the absolute maximum set by the user.
	if(m_fltTension > m_fltMaxTension)
		m_fltTension = m_fltMaxTension;

	if(m_fltTension < -m_fltMaxTension)
		m_fltTension = -m_fltMaxTension;

	//If we are only doing compression or extension then cap the tension at 0 for those settings.
	if(m_iForceType == AL_SPRING_COMPRESSION_ONLY && m_fltTension < 0)
		m_fltTension = 0;

	if(m_iForceType == AL_SPRING_EXTENSION_ONLY && m_fltTension > 0)
		m_fltTension = 0;
}

// There are no parts or joints to create for muscle attachment points.
void MultiSegmentSpring::CreateParts(Simulator *lpSim, Structure *lpStructure)
{
}

void MultiSegmentSpring::CreateJoints(Simulator *lpSim, Structure *lpStructure)
{
	m_aryAttachmentPoints.Clear();

	string strID;
	MuscleAttachment *lpAttachment=NULL;
	int iCount = m_aryAttachmentPointIDs.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
	{
		strID = m_aryAttachmentPointIDs[iIndex];
		lpAttachment = dynamic_cast<MuscleAttachment *>(lpStructure->FindRigidBody(strID));
		m_aryAttachmentPoints.Add(lpAttachment);
	}

	if(m_aryAttachmentPoints.GetSize() < 2)
		m_bEnabled = FALSE;

	//Get the current length of the muscle.
	m_fltSpringLength = CalculateSpringLength(lpSim);
	m_fltPrevSpringLength = m_fltSpringLength;
	m_fltTension = 0;
}

float *MultiSegmentSpring::GetDataPointer(string strDataType)
{
	float *lpData=NULL;
	string strType = Std_CheckString(strDataType);

	if(strType == "TENSION")
		lpData = &m_fltTension;
	else if(strType == "SPRINGTENSION")
		lpData = &m_fltSpringTension;
	else if(strType == "DAMPINGTENSION")
		lpData = &m_fltDampingTension;
	else if(strType == "SPRINGLENGTH")
		lpData = &m_fltSpringLength;
	else if(strType == "VSPRING")
		lpData = &m_fltVspring;
	else if(strType == "ASPRING")
		lpData = &m_fltAspring;
	else if(strType == "POTENERGY")
		lpData = &m_fltPotEnergy;
	else if(strType == "DISPLACEMENT")
		lpData = &m_fltDisplacement;
	else if(strType == "DISPLACEMENTRATIO")
		lpData = &m_fltDisplacementRatio;
	else if(strType == "ENABLE")
		lpData = &m_fltEnabled;
	else
		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "RigidBodyID: " + STR(m_strName) + "  DataType: " + strDataType);

	return lpData;
}

int MultiSegmentSpring::ConvertForceType(string strOrigType)
{
	string strType = Std_CheckString(strOrigType);
	int iType = AL_SPRING_BOTH;

	if(strType == "COMPRESSIONONLY")
		iType = AL_SPRING_COMPRESSION_ONLY;
	else if(strType == "EXTENSIONONLY")
		iType = AL_SPRING_EXTENSION_ONLY;
	else if(strType == "BOTH")
		iType = AL_SPRING_BOTH;
	else
		THROW_TEXT_ERROR(Al_Err_lInvalidSpringForceType, Al_Err_strInvalidSpringForceType, "SpringID: " + STR(m_strName) + "  Type: " + strOrigType);

	return iType;
}

void MultiSegmentSpring::AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput)
{
	if(m_aryAttachmentPoints.GetSize() < 2)
		m_bEnabled = FALSE;
	else
	{
		if(fltInput > 0)
			m_bEnabled = !m_bInitEnabled;
		else
			m_bEnabled = m_bInitEnabled;
	}
}

void MultiSegmentSpring::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	if(!m_lpParent)
		THROW_ERROR(Al_Err_lParentNotDefined, Al_Err_strParentNotDefined);

	oXml.IntoElem();  //Into RigidBody Element

	m_strType = oXml.GetChildString("Type");
	m_strID = Std_CheckString(oXml.GetChildString("ID"));
	m_strName = oXml.GetChildString("Name", "");

	if(Std_IsBlank(m_strID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	if(Std_IsBlank(m_strName)) 
		m_strName = m_strID;

	m_bEnabled = oXml.GetChildBool("Enabled", m_bEnabled);
	m_bIsVisible = oXml.GetChildBool("IsVisible", m_bIsVisible);

	m_aryAttachmentPointIDs.Clear();
	if(oXml.FindChildElement("MuscleAttachments", FALSE))
	{
		oXml.IntoElem();
		int iCount = oXml.NumberOfChildren();
		string strID;
		for(int iIndex=0; iIndex<iCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			strID = oXml.GetChildString();
			m_aryAttachmentPointIDs.Add(strID);
		}
		oXml.OutOfElem(); //OutOf MuscleAttachments Element
	}

	if(m_aryAttachmentPointIDs.GetSize() < 2)
		m_bEnabled = FALSE;

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

	m_fltNaturalLength = AnimatLibrary::LoadScaledNumber(oXml, "NaturalLength", FALSE, m_fltNaturalLength);
	m_fltStiffness = AnimatLibrary::LoadScaledNumber(oXml, "Stiffness", FALSE, m_fltStiffness);
	m_fltDamping = AnimatLibrary::LoadScaledNumber(oXml, "Damping", FALSE, m_fltDamping);
	m_fltMaxTension = AnimatLibrary::LoadScaledNumber(oXml, "MaximumTension", FALSE, m_fltMaxTension);

	//The units for damping are Kg/s. So if we save it out as 1 Kg/s in editor then
	//damping will be read in as 1000. But we need to divide by 1000 to get it back to 1.
	m_fltDamping = m_fltDamping/1000;

	//m_fltNaturalLength *= lpSim->InverseDistanceUnits();
	//m_fltStiffness *= lpSim->InverseMassUnits();
	//m_fltDamping *= lpSim->InverseMassUnits();

	Std_IsAboveMin((float) 0, m_fltNaturalLength, TRUE, "NaturalLength");
	Std_IsAboveMin((float) 0, m_fltStiffness, TRUE, "Stiffness");
	Std_IsAboveMin((float) 0, m_fltDamping, TRUE, "Damping", TRUE);
	Std_IsAboveMin((float) 0, m_fltMaxTension, TRUE, "Max Tension");

	if(oXml.FindChildElement("ForceType", FALSE))
	{
		string strType = oXml.GetChildString("ForceType");
		m_iForceType = ConvertForceType(strType);
	}

	m_bInitEnabled = m_bEnabled;

	oXml.OutOfElem(); //OutOf RigidBody Element
}

		}		//Joints
	}			//Environment
}				//AnimatLibrary

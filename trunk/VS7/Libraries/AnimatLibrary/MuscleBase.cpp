// MuscleBase.cpp: implementation of the MuscleBase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <math.h>
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

#include "MuscleBase.h"

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
   Constructs a muscle object..
   		
   \param lpParent This is a pointer to the parent of this rigid body. 
	          If this value is null then it is assumed that this is
						a root object and no joint is loaded to connect this
						part to the parent.

	 \return
	 No return value.

   \remarks
	 The constructor for a muscle. 
*/

MuscleBase::MuscleBase()
{
	m_bEnabled = TRUE;

	m_aryColor[0] = 1;
	m_aryColor[1] = 0;
	m_aryColor[2] = 0;
	m_aryColor[3] = 1;
	m_fltDensity = 0;
	m_lpJointToParent = NULL;

	m_fltMaxTension = 0;
	m_fltVm = (float) -0.15;
	m_fltTdot = 0;
	m_fltTension = 0;
	m_fltPrevTension = 0;
	m_fltMuscleLength = 0;
	m_fltPrevMuscleLength = 0;
}

/*! \brief 
   Destroys the muscle object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the muscle object..	 
*/

MuscleBase::~MuscleBase()
{
	try
	{
		m_aryAttachmentPointIDs.Clear();
		m_aryAttachmentPoints.Clear();
	}
	catch(...)
	{Std_TraceMsg(0, "Caught Error in desctructor of MuscleBase\r\n", "", -1, FALSE, TRUE);}
}

void MuscleBase::Enabled(BOOL bVal)
{
	m_bEnabled = bVal;
	m_fltEnabled = (float) bVal;

	if(!bVal)
	{
		m_fltTdot = 0;
		m_fltTension = 0;
	}
}


float MuscleBase::CalculateMuscleLength(Simulator *lpSim)
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


// There are no parts or joints to create for muscle attachment points.
void MuscleBase::CreateParts(Simulator *lpSim, Structure *lpStructure)
{
}

void MuscleBase::CreateJoints(Simulator *lpSim, Structure *lpStructure)
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
	m_fltMuscleLength = CalculateMuscleLength(lpSim);
	m_fltPrevMuscleLength = m_fltMuscleLength;
}


void MuscleBase::AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput)
{
	//We are changing this. It is now really driven by the membrane voltage of the non-spiking neuron. Integration from 
	//different motor neurons takes place in the non-spiking neuron and we get that here instead of frequency and use that
	//to calculate the max isometric tension from the stim-tension curve.
	m_fltVm=fltInput;
}

void MuscleBase::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
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
		oXml.OutOfElem(); //OutOf NonSpikingSynapses Element
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

	m_fltMaxTension = AnimatLibrary::LoadScaledNumber(oXml, "MaximumTension", FALSE, m_fltMaxTension);
	Std_IsAboveMin((float) 0, m_fltMaxTension, TRUE, "Max Tension");

	oXml.OutOfElem(); //OutOf RigidBody Element
}

		}		//Bodies
	}			//Environment
}				//AnimatLibrary

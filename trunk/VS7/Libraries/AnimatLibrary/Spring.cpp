// Spring.cpp: implementation of the Spring class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "Spring.h"
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
   Constructs a Spring joint.
   		
   \param lpParent This is a pointer to the parent rigid body of this joint. 
   \param lpChild This is a pointer to the child rigid body of this joint. 

	 \return
	 No return value.

   \remarks
	 The constructor for a Spring joint. 
*/

Spring::Spring()
{
	m_bInitEnabled = FALSE;
	m_lpPrimaryAttachment = NULL;
	m_lpSecondaryAttachment = NULL;
	m_fltNaturalLength = 1;
	m_fltStiffness = 5000;
	m_fltDamping = 1000;
	m_fltLength = 0;
	m_fltDisplacement = 0;
	m_fltTension = 0;
	m_fltEnergy = 0;
}


/*! \brief 
   Destroys the Spring joint object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the Spring joint object..	 
*/

Spring::~Spring()
{
	try
	{
		//Don't delete these. They are only references
		m_lpPrimaryAttachment = NULL;
		m_lpSecondaryAttachment = NULL;
	}
	catch(...)
	{Std_TraceMsg(0, "Caught Error in desctructor of Spring\r\n", "", -1, FALSE, TRUE);}
}

// There are no parts or joints to create for muscle attachment points.
void Spring::CreateParts(Simulator *lpSim, Structure *lpStructure)
{
}

void Spring::AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput)
{
	if(m_lpPrimaryAttachment && m_lpSecondaryAttachment)
	{
		if(fltInput > 0 && m_bEnabled != !m_bInitEnabled)
			Enabled(!m_bInitEnabled);

		if(fltInput <= 0 && m_bEnabled != m_bInitEnabled)
			Enabled(m_bInitEnabled);
	}
	else
		m_bEnabled = FALSE;
}

void Spring::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
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

	m_strPrimaryAttachmentID = Std_CheckString(oXml.GetChildString("PrimaryAttachmentID", ""));
	m_strSecondaryAttachmentID = Std_CheckString(oXml.GetChildString("SecondaryAttachmentID", ""));

	if(Std_IsBlank(m_strPrimaryAttachmentID) || Std_IsBlank(m_strSecondaryAttachmentID))
		m_bEnabled = FALSE;

	m_fltNaturalLength = AnimatLibrary::LoadScaledNumber(oXml, "NaturalLength", FALSE, m_fltNaturalLength);
	m_fltStiffness = AnimatLibrary::LoadScaledNumber(oXml, "Stiffness", FALSE, m_fltStiffness);
	m_fltDamping = AnimatLibrary::LoadScaledNumber(oXml, "Damping", FALSE, m_fltDamping);
	
	m_fltNaturalLength *= lpSim->InverseDistanceUnits();
	m_fltStiffness *= lpSim->InverseMassUnits();
	m_fltDamping = m_fltDamping/lpSim->DensityMassUnits();

	Std_IsAboveMin((float) 0, m_fltNaturalLength, TRUE, "NaturalLength");
	Std_IsAboveMin((float) 0, m_fltStiffness, TRUE, "Stiffness");
	Std_IsAboveMin((float) 0, m_fltDamping, TRUE, "Damping", TRUE);

	m_bInitEnabled = m_bEnabled;

	oXml.OutOfElem(); //OutOf RigidBody Element
}

		}		//Joints
	}			//Environment
}				//AnimatLibrary

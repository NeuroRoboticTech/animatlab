// Organism.cpp: implementation of the Organism class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Gain.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "MuscleAttachment.h"
#include "IODataEntry.h"
#include "Microcontroller.h"
#include "Structure.h"
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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*! \brief 
   Constructs an organism object..
   		
	 \return
	 No return value.

   \remarks
	 The constructor for a organism. 
*/

Organism::Organism()
{
	m_bDead = FALSE;
	m_lpNervousSystem = NULL;
}

/*! \brief 
   Destroys the organism object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the organism object..	 
*/

Organism::~Organism()
{

try
{
	if(m_lpNervousSystem) 
		{delete m_lpNervousSystem; m_lpNervousSystem = NULL;}
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Organism\r\n", "", -1, FALSE, TRUE);}
}

void Organism::Kill(Simulator *lpSim, BOOL bState)
{
	m_bDead = bState;
	m_lpBody->Kill(lpSim, this, bState);
	m_lpNervousSystem->Kill(lpSim, this, bState);
}

void Organism::Initialize(Simulator *lpSim)
{
	Structure::Initialize(lpSim);

	m_lpNervousSystem->Initialize(lpSim, this);

	Microcontroller *lpController = NULL;
	int iCount = m_aryMicrocontrollers.GetSize();
	for(int iController=0; iController<iCount; iController++)
	{
		lpController = m_aryMicrocontrollers[iController];
		lpController->Initialize(lpSim, this);
	}
}

void Organism::Reset(Simulator *lpSim)
{
	Structure::Reset(lpSim);

	m_lpNervousSystem->Initialize(lpSim, this);
}

void Organism::StepNeuralEngine(Simulator *lpSim)
{
	if(!m_bDead)
		m_lpNervousSystem->StepSimulation(lpSim, this);
}

void Organism::StepPhysicsEngine(Simulator *lpSim)
{
	if(m_lpBody)
		m_lpBody->StepSimulation(lpSim, this);
}


float *Organism::GetDataPointer(string &strID, string &strDataType)
{
	RigidBody *lpBody = dynamic_cast<RigidBody *>(FindRigidBody(strID, FALSE));

	if(!lpBody)
	{
		Joint *lpJoint = dynamic_cast<Joint *>(FindJoint(strID, FALSE));
		
		if(!lpJoint)
			THROW_PARAM_ERROR(Al_Err_lBodyOrJointIDNotFound, Al_Err_strBodyOrJointIDNotFound, "ID", strID);

		return lpJoint->GetDataPointer(strDataType);		
	}
	else
		return lpBody->GetDataPointer(strDataType);
}

/*
CNlNeuron *Organism::GetNeuron(short iXPos, short iYPos, short iZPos)
{
	if(!m_lpNervousSystem)
		THROW_ERROR(Al_Err_lNervousSystemNotDefined, Al_Err_strNervousSystemNotDefined);

	return m_lpNervousSystem->GetNeuron(iXPos, iYPos, iZPos);
}
*/

AnimatLibrary::Behavior::NervousSystem *Organism::NervousSystem()
{return m_lpNervousSystem;}

long Organism::CalculateSnapshotByteSize()
{return m_lpNervousSystem->CalculateSnapshotByteSize();}

void Organism::SaveKeyFrameSnapshot(byte *aryBytes, long &lIndex)
{m_lpNervousSystem->SaveKeyFrameSnapshot(aryBytes, lIndex);}

void Organism::LoadKeyFrameSnapshot(byte *aryBytes, long &lIndex)
{m_lpNervousSystem->LoadKeyFrameSnapshot(aryBytes, lIndex);}

void Organism::Load(Simulator *lpSim, CStdXml &oXml)
{
	Structure::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Layout Element

	string strNeuralFile = oXml.GetChildString("BehavioralSystem", "");

	//dwc convert. Need to have a method to remove a nervous system. It needs to remove any added
	//modules from the list in the simulator.
	if(m_lpNervousSystem) {delete m_lpNervousSystem; m_lpNervousSystem = NULL;}
	m_lpNervousSystem = new AnimatLibrary::Behavior::NervousSystem;

	if(!Std_IsBlank(strNeuralFile)) 
		m_lpNervousSystem->Load(lpSim, this, lpSim->ProjectPath(), strNeuralFile);
	else
	{
		oXml.IntoChildElement("NervousSystem");
		m_lpNervousSystem->ProjectPath(lpSim->ProjectPath());
		m_lpNervousSystem->Load(lpSim, this, oXml);
		oXml.OutOfElem();
	}

	oXml.OutOfElem(); //OutOf Layout Element
}

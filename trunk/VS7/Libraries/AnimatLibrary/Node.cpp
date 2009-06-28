// Node.cpp: implementation of the Node class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Gain.h"
#include "Adapter.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "IODataEntry.h"
#include "Microcontroller.h"
#include "Structure.h"
#include "NeuralModule.h"
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
namespace AnimatLibrary
{

/*! \brief 
   Constructs an structure object..
   		
	 \return
	 No return value.

   \remarks
	 The constructor for a structure. 
*/

Node::Node()
{
	m_bEnabledMem = TRUE;
	m_bEnabled = TRUE;
	m_fltEnabled = 0;
}


/*! \brief 
   Destroys the structure object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the structure object..	 
*/

Node::~Node()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Node\r\n", "", -1, FALSE, TRUE);}
}

void Node::AttachSourceAdapter(Simulator *lpSim, Structure *lpStructure, Adapter *lpAdapter)
{
	lpSim->AttachSourceAdapter(lpStructure, lpAdapter);
}

void Node::AttachTargetAdapter(Simulator *lpSim, Structure *lpStructure, Adapter *lpAdapter)
{
	lpSim->AttachTargetAdapter(lpStructure, lpAdapter);
}

void Node::Kill(Simulator *lpSim, Organism *lpOrganism, BOOL bState)
{
	if(bState)
	{
		m_bEnabledMem = m_bEnabled;
		Enabled(FALSE);
	}
	else
		Enabled(m_bEnabledMem);

}

void *Node::GetDataItem(string strItemType, string strID, BOOL bThrowError) 
{
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidItemType, Al_Err_strInvalidItemType, "Item Type", strItemType);

	return NULL;
}

void Node::UpdateData(Simulator *lpSim, Structure *lpStructure)
{}

void Node::Reset(Simulator *lpSim, Structure *lpStructure)
{}

void Node::ResetNode(Simulator *lpSim, Structure *lpStructure)
{}

}			//AnimatLibrary

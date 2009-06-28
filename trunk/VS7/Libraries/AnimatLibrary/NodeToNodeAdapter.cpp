// NodeToNodeAdapter.cpp: implementation of the NodeToNodeAdapter class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Gain.h"
#include "Adapter.h"
#include "NodeToNodeAdapter.h"
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
	namespace Adapters
	{

/*! \brief 
   Constructs an structure object..
   		
	 \return
	 No return value.

   \remarks
	 The constructor for a structure. 
*/

NodeToNodeAdapter::NodeToNodeAdapter()
{
	m_lSourceNodeID = 0;
	m_lTargetNodeID = 0;
	m_lpSourceData = NULL;
}


/*! \brief 
   Destroys the structure object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the structure object..	 
*/

NodeToNodeAdapter::~NodeToNodeAdapter()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of NodeToNodeAdapter\r\n", "", -1, FALSE, TRUE);}
}


void NodeToNodeAdapter::Initialize(Simulator *lpSim, Structure *lpStructure)
{
	Organism *lpOrganism = dynamic_cast<Organism *>(lpStructure);
	if(!lpOrganism)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Organism");

	m_lpSourceNode = lpOrganism->NervousSystem()->FindNode(m_strSourceModule, m_lSourceNodeID);
	m_lpSourceData = m_lpSourceNode->GetDataPointer(m_strSourceDataType);

	if(!m_lpSourceData)
		THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
		("Adapter: " + m_strID + " OrganismID: " + lpOrganism->ID() + "Node: " + STR(m_lSourceNodeID) + " DataType: " + m_strSourceDataType));

	m_lpTargetNode = lpOrganism->NervousSystem()->FindNode(m_strTargetModule, m_lTargetNodeID);

	m_lpSourceNode->AttachSourceAdapter(lpSim, lpStructure, this);
	m_lpTargetNode->AttachTargetAdapter(lpSim, lpStructure, this);
}

string NodeToNodeAdapter::SourceModule()
{return m_strSourceModule;}

string NodeToNodeAdapter::TargetModule()
{return m_strTargetModule;}

//Node Overrides

void NodeToNodeAdapter::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	Adapter::Load(lpSim, lpStructure, oXml);

	oXml.IntoElem();  //Into Adapter Element

	m_strSourceModule = oXml.GetChildString("SourceModule");
	if(Std_IsBlank(m_strSourceModule)) 
		THROW_TEXT_ERROR(Al_Err_lModuleNameBlank, Al_Err_strModuleNameBlank, " Source Module");

	m_lSourceNodeID = oXml.GetChildLong("SourceNodeID");

	m_strTargetModule = oXml.GetChildString("TargetModule");
	if(Std_IsBlank(m_strTargetModule)) 
		THROW_TEXT_ERROR(Al_Err_lModuleNameBlank, Al_Err_strModuleNameBlank, " Target Module");

	m_lTargetNodeID = oXml.GetChildLong("TargetNodeID");

	oXml.OutOfElem(); //OutOf Adapter Element
}

	}			//Adapters
}			//AnimatLibrary

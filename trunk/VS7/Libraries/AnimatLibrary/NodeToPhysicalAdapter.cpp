// NodeToPhysicalAdapter.cpp: implementation of the NodeToPhysicalAdapter class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Gain.h"
#include "Adapter.h"
#include "NodeToPhysicalAdapter.h"
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

NodeToPhysicalAdapter::NodeToPhysicalAdapter()
{
	m_lSourceNodeID = 0;
}


/*! \brief 
   Destroys the structure object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the structure object..	 
*/

NodeToPhysicalAdapter::~NodeToPhysicalAdapter()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of NodeToPhysicalAdapter\r\n", "", -1, FALSE, TRUE);}
}


void NodeToPhysicalAdapter::Initialize(Simulator *lpSim, Structure *lpStructure)
{
	Organism *lpOrganism = dynamic_cast<Organism *>(lpStructure);
	if(!lpOrganism)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Organism");

	m_lpSourceNode = lpOrganism->NervousSystem()->FindNode(m_strSourceModule, m_lSourceNodeID);
	m_lpSourceData = m_lpSourceNode->GetDataPointer(m_strSourceDataType);

	if(!m_lpSourceData)
		THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
		("Adapter: " + m_strID + " OrganismID: " + lpOrganism->ID() + "Node: " + STR(m_lSourceNodeID) + " DataType: " + m_strSourceDataType));

	if(m_strTargetBodyType == "JOINT")
		m_lpTargetNode = lpOrganism->FindJoint(m_strTargetBodyID);
	else
		m_lpTargetNode = lpOrganism->FindRigidBody(m_strTargetBodyID);

	m_lpSourceNode->AttachSourceAdapter(lpSim, lpStructure, this);
	m_lpTargetNode->AttachTargetAdapter(lpSim, lpStructure, this);
}

string NodeToPhysicalAdapter::SourceModule()
{return m_strSourceModule;}

string NodeToPhysicalAdapter::TargetModule()
{return "AnimatLab";}

//Node Overrides

void NodeToPhysicalAdapter::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	Adapter::Load(lpSim, lpStructure, oXml);

	oXml.IntoElem();  //Into Adapter Element

	m_strSourceModule = oXml.GetChildString("SourceModule");
	if(Std_IsBlank(m_strSourceModule)) 
		THROW_TEXT_ERROR(Al_Err_lModuleNameBlank, Al_Err_strModuleNameBlank, " Source Module");

	m_lSourceNodeID = oXml.GetChildLong("SourceNodeID");

	m_strTargetBodyType = oXml.GetChildString("TargetBodyType");
	if(Std_IsBlank(m_strTargetBodyType)) 
		THROW_ERROR(Al_Err_lBodyTypeBlank, Al_Err_strBodyTypeBlank);
	m_strTargetBodyType = Std_CheckString(m_strTargetBodyType);

	m_strTargetBodyID = oXml.GetChildString("TargetBodyID");
	if(Std_IsBlank(m_strTargetBodyID)) 
		THROW_ERROR(Al_Err_lBodyIDBlank, Al_Err_strBodyIDBlank);

	oXml.OutOfElem(); //OutOf Adapter Element
}

	}			//Adapters
}			//AnimatLibrary

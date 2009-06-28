// PhysicalToNodeAdapter.cpp: implementation of the PhysicalToNodeAdapter class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Gain.h"
#include "Adapter.h"
#include "PhysicalToNodeAdapter.h"
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

PhysicalToNodeAdapter::PhysicalToNodeAdapter()
{
	m_lTargetNodeID = 0;
}


/*! \brief 
   Destroys the structure object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the structure object..	 
*/

PhysicalToNodeAdapter::~PhysicalToNodeAdapter()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of PhysicalToNodeAdapter\r\n", "", -1, FALSE, TRUE);}
}


void PhysicalToNodeAdapter::Initialize(Simulator *lpSim, Structure *lpStructure)
{
	Organism *lpOrganism = dynamic_cast<Organism *>(lpStructure);
	if(!lpOrganism)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Organism");

	if(m_strSourceBodyType == "JOINT")
		m_lpSourceNode = lpOrganism->FindJoint(m_strSourceBodyID);
	else
		m_lpSourceNode = lpOrganism->FindRigidBody(m_strSourceBodyID);

	m_lpSourceData = m_lpSourceNode->GetDataPointer(m_strSourceDataType);

	if(!m_lpSourceData)
		THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
		("Adapter: " + m_strID + " OrganismID: " + lpOrganism->ID() + "BodyID: " + m_strSourceBodyID + " DataType: " + m_strSourceDataType));

	m_lpTargetNode = lpOrganism->NervousSystem()->FindNode(m_strTargetModule, m_lTargetNodeID);

	m_lpSourceNode->AttachSourceAdapter(lpSim, lpStructure, this);
	m_lpTargetNode->AttachTargetAdapter(lpSim, lpStructure, this);
}

string PhysicalToNodeAdapter::SourceModule()
{return "AnimatLab";}

string PhysicalToNodeAdapter::TargetModule()
{return m_strTargetModule;}

//Node Overrides

void PhysicalToNodeAdapter::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	Adapter::Load(lpSim, lpStructure, oXml);

	oXml.IntoElem();  //Into Adapter Element

	m_strSourceBodyType = oXml.GetChildString("SourceBodyType");
	if(Std_IsBlank(m_strSourceBodyType)) 
		THROW_ERROR(Al_Err_lBodyTypeBlank, Al_Err_strBodyTypeBlank);
	m_strSourceBodyType = Std_CheckString(m_strSourceBodyType);

	m_strSourceBodyID = oXml.GetChildString("SourceBodyID");
	if(Std_IsBlank(m_strSourceBodyID)) 
		THROW_ERROR(Al_Err_lBodyIDBlank, Al_Err_strBodyIDBlank);

	m_strTargetModule = oXml.GetChildString("TargetModule");
	if(Std_IsBlank(m_strTargetModule)) 
		THROW_TEXT_ERROR(Al_Err_lModuleNameBlank, Al_Err_strModuleNameBlank, " Target Module");

	m_lTargetNodeID = oXml.GetChildLong("TargetNodeID");

	oXml.OutOfElem(); //OutOf Adapter Element
}

	}			//Adapters
}			//AnimatLibrary

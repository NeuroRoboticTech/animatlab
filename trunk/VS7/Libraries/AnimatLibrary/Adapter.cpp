// Adapter.cpp: implementation of the Adapter class.
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
	namespace Adapters
	{

/*! \brief 
   Constructs an structure object..
   		
	 \return
	 No return value.

   \remarks
	 The constructor for a structure. 
*/

Adapter::Adapter()
{
	m_lpGain = NULL;
	m_lpSourceNode = NULL;
	m_lpSourceData = NULL;
	m_lpTargetNode = NULL;
}


/*! \brief 
   Destroys the structure object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the structure object..	 
*/

Adapter::~Adapter()
{

try
{
	m_lpSourceNode = NULL;
	m_lpSourceData = NULL;
	m_lpTargetNode = NULL;
	if(m_lpGain) delete m_lpGain;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Adapter\r\n", "", -1, FALSE, TRUE);}
}


//Node Overrides
void Adapter::AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput)
{
	THROW_TEXT_ERROR(Al_Err_lOpNotDefinedForAdapter, Al_Err_strOpNotDefinedForAdapter, "AddExternalNodeInput");
}

void Adapter::AttachSourceAdapter(Simulator *lpSim, Structure *lpStructure, Node *lpNode)
{
	THROW_TEXT_ERROR(Al_Err_lOpNotDefinedForAdapter, Al_Err_strOpNotDefinedForAdapter, "AttachSourceAdapter");
}

void Adapter::AttachTargetAdapter(Simulator *lpSim, Structure *lpStructure, Node *lpNode)
{
	THROW_TEXT_ERROR(Al_Err_lOpNotDefinedForAdapter, Al_Err_strOpNotDefinedForAdapter, "AttachTargetAdapter");
}

float *Adapter::GetDataPointer(string strDataType)
{
	THROW_TEXT_ERROR(Al_Err_lOpNotDefinedForAdapter, Al_Err_strOpNotDefinedForAdapter, "GetDataPointer");
	return 0;
}

void Adapter::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	m_lpTargetNode->AddExternalNodeInput(lpSim, lpStructure, m_lpGain->CalculateGain(*m_lpSourceData));
}

void Adapter::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	oXml.IntoElem();  //Into Adapter Element

	//Load Source Data
	m_strSourceDataType = oXml.GetChildString("SourceDataType");
	if(Std_IsBlank(m_strSourceDataType)) 
		THROW_TEXT_ERROR(Al_Err_lDataTypeBlank, Al_Err_strDataTypeBlank, " Source DataType");

	//Now lets load this gain object.
	oXml.IntoChildElement("Gain");
	string strModuleName = oXml.GetChildString("ModuleName", "");
	string strType = oXml.GetChildString("Type");
	oXml.OutOfElem(); //OutOf Gain Element

	m_lpGain = dynamic_cast<AnimatLibrary::Gains::Gain *>(lpSim->CreateObject(strModuleName, "Gain", strType));
	if(!m_lpGain)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Gain");

	m_lpGain->Load(oXml);

	oXml.OutOfElem(); //OutOf Adapter Element
}

void Adapter::Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
}

	}			//Adapters
}			//AnimatLibrary

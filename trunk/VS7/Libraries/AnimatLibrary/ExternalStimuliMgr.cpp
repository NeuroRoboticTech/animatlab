// ExternalStimuliMgr.cpp: implementation of the ExternalStimuliMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "StdAfx.h"
#include "Gain.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "IODataEntry.h"
#include "Microcontroller.h"
#include "Structure.h"
#include "NeuralModule.h"
#include "Adapter.h"
#include "NervousSystem.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataChartMgr.h"
#include "ExternalStimulus.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"

namespace AnimatLibrary
{
	namespace ExternalStimuli
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ExternalStimuliMgr::ExternalStimuliMgr()
{

}

ExternalStimuliMgr::~ExternalStimuliMgr()
{

}


void ExternalStimuliMgr::Load(Simulator *lpSim, string strProjectPath, string strFileName)
{
	CStdXml oXml;

	TRACE_DEBUG("Loading external stimuli config file.\r\n" + strProjectPath + "\r\nFileName: " + strFileName);

	oXml.Load(AnimatLibrary::GetFilePath(strProjectPath, strFileName));

	oXml.FindElement("StimuliConfiguration");
	oXml.FindChildElement("");

	Load(lpSim, oXml);

	TRACE_DEBUG("Finished loading external stimuli config file.");
}


void ExternalStimuliMgr::Load(Simulator *lpSim, CStdXml &oXml)
{
	TRACE_DEBUG("Loading external stimuli from Xml.");

	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	Reset();

	if(oXml.FindChildElement("ExternalStimuli", FALSE))
	{
		oXml.IntoElem(); //Into ExternalStimuli Element

		int iCount = oXml.NumberOfChildren();
		for(int iIndex=0; iIndex<iCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			LoadExternalStimuli(lpSim, oXml);
		}

		oXml.OutOfElem(); //OutOf ExternalStimuli Element
	}

#if defined(STD_TRACING_ON) && (defined(STD_TRACE_INFO) || defined(STD_TRACE_DETAIL))
	TRACE_STL_CONTAINER_DESC(m_aryItems, "External Stimuli");
#endif

	TRACE_DEBUG("Finished loading external stimuli from Xml.");

}


ExternalStimulus *ExternalStimuliMgr::LoadExternalStimuli(Simulator *lpSim, CStdXml &oXml)
{
	ExternalStimulus *lpStimulus = NULL;
	string strModuleName, strType, strFilename;

try
{
	oXml.IntoElem(); //Into Stimulus Element
	strModuleName = oXml.GetChildString("ModuleName", "");
	strType = oXml.GetChildString("Type");
	oXml.OutOfElem(); //OutOf Stimulus Element

	lpStimulus = dynamic_cast<ExternalStimulus *>(lpSim->CreateObject(strModuleName, "ExternalStimulus", strType));
	if(!lpStimulus)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "ExternalStimulus");

	lpStimulus->Load(lpSim, oXml);

	AddItemInternal(lpSim, lpStimulus);
	return lpStimulus;
}
catch(CStdErrorInfo oError)
{
	if(lpStimulus) delete lpStimulus;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpStimulus) delete lpStimulus;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

	}			//ExternalStimuli
}				//AnimatLibrary


// DataChartMgr.cpp: implementation of the DataChartMgr class.
//
//////////////////////////////////////////////////////////////////////

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
#include "DataColumn.h"
#include "DataChart.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"

namespace AnimatLibrary
{
	namespace Charting
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataChartMgr::DataChartMgr()
{}

DataChartMgr::~DataChartMgr()
{}

DataColumn *DataChartMgr::FindDataColumn(string strChartKey, string strColumnName, BOOL bThrowError)
{
	DataChart *lpChart = dynamic_cast<DataChart *>(Find(strChartKey, bThrowError));
	if(!lpChart) return NULL;

	DataColumn *lpColumn = lpChart->FindColumn(strColumnName, bThrowError);
	return lpColumn;
}

void DataChartMgr::RemoveDataColumn(string strChartKey, string strColumnName, BOOL bThrowError)
{
	DataChart *lpChart = dynamic_cast<DataChart *>(Find(strChartKey, bThrowError));
	if(!lpChart) return;

	lpChart->RemoveColumn(strColumnName, bThrowError);
}

void DataChartMgr::AddDataColumn(string strChartKey, DataColumn *lpColumn)
{
	DataChart *lpChart = dynamic_cast<DataChart *>(Find(strChartKey));
	lpChart->AddColumn(lpColumn);	
}

void DataChartMgr::ModifyDataColumn(string strChartKey, string strColumnName, string strDataType)
{
	DataChart *lpChart = dynamic_cast<DataChart *>(Find(strChartKey));
	if(!lpChart) return;

	DataColumn *lpColumn = lpChart->FindColumn(strColumnName, TRUE);
	lpColumn->DataType(strDataType);
	lpColumn->BeenInitialized(FALSE);
}

void DataChartMgr::SetDataColumnIndex(string strChartKey, string strColumnName, int iIndex)
{
	DataChart *lpChart = dynamic_cast<DataChart *>(Find(strChartKey));
	if(!lpChart) return;

	DataColumn *lpColumn = lpChart->FindColumn(strColumnName, TRUE);
	lpColumn->Index(iIndex);
}

void DataChartMgr::Load(Simulator *lpSim, CStdXml &oXml)
{
	TRACE_DEBUG("Loading data charts from Xml.");

	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	Reset();

	if(oXml.FindChildElement("DataCharts", FALSE))
	{
		oXml.IntoElem(); //Into DataCharts Element

		int iCount = oXml.NumberOfChildren();
		for(int iIndex=0; iIndex<iCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			LoadDataChart(lpSim, oXml);
		}

		oXml.OutOfElem(); //OutOf Environment Element
	}


	//Now sort the charts based on their starting time slice.
	stable_sort(m_aryItems.begin(), m_aryItems.end(), LessThanActivatedItemCompare);

#if defined(STD_TRACING_ON) && (defined(STD_TRACE_INFO) || defined(STD_TRACE_DETAIL))
	TRACE_STL_CONTAINER_DESC(m_aryItems, "Data Charts");
#endif

	TRACE_DEBUG("Finished loading data charts from Xml.");
}


DataChart *DataChartMgr::LoadDataChart(Simulator *lpSim, CStdXml &oXml)
{
	DataChart *lpChart = NULL;
	string strModuleName, strType, strFilename;

try
{
	oXml.IntoElem(); //Into DataChart Element
	strModuleName = oXml.GetChildString("ModuleName", "");
	strType = oXml.GetChildString("Type");
	strFilename = oXml.GetChildString("Filename", "");
	oXml.OutOfElem(); //OutOf DataChart Element

	lpChart = dynamic_cast<DataChart *>(lpSim->CreateObject(strModuleName, "DataChart", strType));
	if(!lpChart)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "DataChart");

	if(!Std_IsBlank(strFilename))
		lpChart->Load(lpSim, lpSim->ProjectPath(), strFilename);
	else
	{
		lpChart->ProjectPath(lpSim->ProjectPath());
		lpChart->Load(lpSim, oXml);
	}

	AddItemInternal(lpSim, lpChart);
	return lpChart;
}
catch(CStdErrorInfo oError)
{
	if(lpChart) delete lpChart;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpChart) delete lpChart;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

	}			//Charting
}				//AnimatLibrary


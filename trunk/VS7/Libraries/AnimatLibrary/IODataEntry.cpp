// IODataEntry.cpp: implementation of the CIODataEntry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
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

IODataEntry::IODataEntry()
{
	m_lpDataValue = NULL;
	m_lpData = NULL;
	m_lpLastSentData = NULL;
	m_lpLastSentValue = NULL;
	m_iDataSize = 8;
	m_iByte = 0;
	m_iShift = 0;
	m_iMotorDataSize = 0;
	m_iSensorSize = 0;
	m_cVal = 0;
	m_lpGain = NULL;
}

IODataEntry::~IODataEntry()
{

try
{
	if(m_lpData)
		delete m_lpData;

	if(m_lpLastSentData)
		delete m_lpLastSentData;

	if(m_lpGain) 
		delete m_lpGain;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of IODataEntry\r\n", "", -1, FALSE, TRUE);}
}

void IODataEntry::Initialize(Simulator *lpSim, Structure *lpStructure, Microcontroller *lpController)
{
	m_lpData->Initialize(lpSim);
	m_lpDataValue = m_lpData->DataValue();

	if(m_lpLastSentData)
	{
		m_lpLastSentData->Initialize(lpSim);
		m_lpLastSentValue = m_lpLastSentData->DataValue();
	}

	//Get these values so that I do not have to retrieve them every time I make a call in the processing loop.
	m_lpParent = lpController;
	m_arySensorData = lpController->SensorData();
	m_aryMotorData = lpController->MotorData();
	m_iMotorDataSize = lpController->MotorDataSize();
	m_iSensorSize = lpController->SensorSize();
}

inline void IODataEntry::SetArrayElement(char *aryData, int iSize, int iVal, int iStart)
{
	m_iByte = (int) (iStart/8);  //This is the byte where start is located in the array.

	if(m_iByte < iSize)
	{
		if(m_iDataSize == 1)
		{
			//Bit size
			m_iShift = iStart - (m_iByte*8);  //Which bit are we setting.
			if(m_iShift < 8)
			{
				if(iVal)
				{
					m_cVal = (1 << m_iShift);
					aryData[m_iByte] = aryData[m_iByte] | m_cVal;
				}
				else
				{
					m_cVal = 0xFF ^ (1 << m_iShift);
					aryData[m_iByte] = aryData[m_iByte] & m_cVal;
				}
			}
		}
		else if(m_iDataSize == 4)
		{
			//Nibble size
			m_iShift = iStart - (m_iByte*8);  //Which bit are we setting.

			if(m_iShift<4)
			{
				m_cVal = aryData[m_iByte] & 0xF0;
				m_cVal = m_cVal | ((char) (iVal & 0x0F));
			}
			else
			{
				m_cVal = aryData[m_iByte] & 0x0F;
				m_cVal = m_cVal | (((char) (iVal & 0x0F)) << 4);
			}

			aryData[m_iByte] = m_cVal;
		}
		else if(m_iDataSize == 8)
		{
			//Byte size
			aryData[m_iByte] = (char) (iVal & 0x00FF);
		}
		else if(m_iDataSize == 16)
		{
			//Word size
			aryData[m_iByte] = (char) ((iVal & 0xFF00) >> 8);
			aryData[m_iByte+1] = (char) (iVal & 0x00FF);		
		}
	}
}

inline int IODataEntry::GetArrayElement(char *aryData, int iSize, int iStart)
{
	m_iByte = (int) (iStart/8);  //This is the byte where start is located in the array.

	if(m_iByte < iSize)
	{
		if(m_iDataSize == 1)
		{
			//Bit size
			m_iShift = iStart - (m_iByte*8);  //Which bit are we setting.
			if(m_iShift < 8)
			{
				if(aryData[m_iByte] & (1 << m_iShift))
					return 1;
				else
					return 0;
			}
		}
		else if(m_iDataSize == 4)
		{
			//Nibble size
			m_iShift = iStart - (m_iByte*8);  //Which bit are we setting.

			if(m_iShift<4)
				return (int) (aryData[m_iByte] & 0x0F);
			else
				return (int) ((aryData[m_iByte] & 0xF0) >> 4);
		}
		else if(m_iDataSize == 8)
			return (int) (aryData[m_iByte] & 0x00FF);
		else if(m_iDataSize == 16)
		{
			SHORT iVal = (((((int) aryData[m_iByte]) << 8) | (0x00FF & aryData[m_iByte+1])) & 0x00FFFF);
			return (int) iVal;
		}
	}
 
	return 0;
}

void IODataEntry::ProcessOutputArray(int &iStart)
{
	//Get the data value from the neural network.
	int iVal = m_lpGain->CalculateGain(*m_lpDataValue);
	SetArrayElement(m_aryMotorData, m_iMotorDataSize, iVal, iStart);
	iStart += m_iDataSize;

	//If we are setting a last sent value then set it here.
	if(m_lpLastSentValue)
		*m_lpLastSentValue = *m_lpDataValue;
}

void IODataEntry::ProcessInputArray(int &iStart)
{
	//Get the data value from the neural network.
	int iVal = GetArrayElement(m_arySensorData, m_iSensorSize, iStart);
	*m_lpDataValue = m_lpGain->CalculateGain(iVal);
	iStart += m_iDataSize;
}

void IODataEntry::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	oXml.IntoElem();  //Into Range Element

	m_strID = oXml.GetChildString("ID");
	m_strName = oXml.GetChildString("Name");

	m_iDataSize = oXml.GetChildInt("DataSize");
	Std_IsAboveMin((int) 0, m_iDataSize, true, "DataSize");

	oXml.FindChildElement("DataColumn");
	oXml.IntoElem();
	oXml.FindChildElement("Data");
	m_lpData = LoadDataColumn(lpSim, oXml);
	oXml.OutOfElem();

	m_lpLastSentData = NULL; m_lpLastSentValue = NULL;
	if(oXml.FindChildElement("LastSentData", FALSE))
	{
		oXml.IntoElem();
		oXml.FindChildElement("Data");
		m_lpLastSentData = LoadDataColumn(lpSim, oXml);
		oXml.OutOfElem();
	}

	//Now lets load this gain object.
	oXml.IntoChildElement("Gain");
	string strModuleName = oXml.GetChildString("ModuleName", "");
	string strType = oXml.GetChildString("Type");
	oXml.OutOfElem(); //OutOf Gain Element

	m_lpGain = dynamic_cast<AnimatLibrary::Gains::Gain *>(lpSim->CreateObject(strModuleName, "Gain", strType));
	if(!m_lpGain)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Gain");

	m_lpGain->LoadFull(oXml);

	oXml.OutOfElem(); //OutOf Range Element
}


DataColumn *IODataEntry::LoadDataColumn(Simulator *lpSim, CStdXml &oXml)
{
	DataColumn *lpColumn=NULL;
	string strModuleName, strType;

try
{
	oXml.IntoElem();  //Into Column Element
	strModuleName = oXml.GetChildString("ModuleName", "");
	strType = oXml.GetChildString("Type");
	oXml.OutOfElem();  //OutOf Column Element

	lpColumn = dynamic_cast<DataColumn *>(lpSim->CreateObject(strModuleName, "DataColumn", strType));
	if(!lpColumn)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "DataColumn");

	lpColumn->Load(lpSim, oXml);

	return lpColumn;
}
catch(CStdErrorInfo oError)
{
	if(lpColumn) delete lpColumn;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpColumn) delete lpColumn;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

void IODataEntry::Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{}


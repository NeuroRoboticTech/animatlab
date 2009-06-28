// Microcontroller.cpp: implementation of the Microcontroller class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <commctrl.h>
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
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Microcontroller::Microcontroller()
{
	m_lpSim = NULL;
	m_lpOrganism = NULL;
	m_iCommPort = 0;
	m_iBaudRate = 250000;
	m_iByteSize = 8;
	m_iStopBits = ONESTOPBIT;
	m_iParity = NOPARITY;
	m_iSensorDataSize = 0;
	m_iSensorSize = 0;
	m_iMotorSize = 0;
	m_iMotorOffset = 0;
	m_iMotorDataSize = 0;
	m_arySensorData = NULL;
	m_aryMotorData = NULL;
	m_cMotorCommand = 0;
	m_cSensorCommand = 1;
	m_strCommandHeader = "*Data";
	m_hCom = NULL;
	m_lUpdateCount = 0;
	m_iTransfer = 0;
	m_iMaxMotorUpdatesPerSec = 25;
	m_iMotorUpdateInterval = (int) (1000.0/m_iMaxMotorUpdatesPerSec);
	m_iLastMotorUpdate = 0;
	m_iInputCount = 0;
	m_iOutputCount = 0;
	m_iTickCount = 0;
	m_iUpdateTime = 0;
}

Microcontroller::~Microcontroller()
{

try
{
	if(m_hCom != NULL && m_hCom != INVALID_HANDLE_VALUE)
		CloseHandle(m_hCom);

	if(m_arySensorData)
		delete m_arySensorData;

	if(m_aryMotorData)
		delete m_aryMotorData;

	m_aryInputs.RemoveAll();
	m_aryOutputs.RemoveAll();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Microcontroller\r\n", "", -1, FALSE, TRUE);}
}


void Microcontroller::Initialize(Simulator *lpSim, Structure *lpStructure)
{
	m_lpSim = lpSim;
	m_lpOrganism = dynamic_cast<Organism *>(lpStructure);

	if(m_iSensorSize > 0)
	{
		m_iSensorDataSize = m_iSensorSize + m_strCommandHeader.length(); 
		m_arySensorData = new char[m_iSensorSize];
		memset(m_arySensorData, 0, m_iSensorSize);
	}

	if(m_iMotorSize > 0)
	{
		m_iMotorOffset =  m_strCommandHeader.length();  // + 1 was assuming a command was getting sent before.
		m_iMotorDataSize = m_iMotorSize + m_iMotorOffset;
		m_aryMotorData = new char[m_iMotorDataSize+1];
		memset(m_aryMotorData, 0, (m_iMotorDataSize+1));

		for(int iIndex=0; iIndex<m_strCommandHeader.length(); iIndex++)
			m_aryMotorData[iIndex] = m_strCommandHeader[iIndex];

		//Std_LogMsg(0, "MotorData Size: " + STR(m_iMotorDataSize) + ", Motor Size: " + STR(m_iMotorSize) + ", Offset Size: " + STR(m_iMotorOffset) + "\n");
		//m_aryMotorData[m_strCommandHeader.length()] = m_cMotorCommand;
	}

	//We need to init the arrays before initing the io data so that they
	//can grab hold of it during their init.
	IODataEntry *lpIOData = NULL;
	m_iInputCount = m_aryInputs.GetSize();
	for(int iData=0; iData<m_iInputCount; iData++)
	{
		lpIOData = m_aryInputs[iData];
		lpIOData->Initialize(lpSim, lpStructure, this);
	}

	m_iOutputCount = m_aryOutputs.GetSize();
	for(int iData=0; iData<m_iOutputCount; iData++)
	{
		lpIOData = m_aryOutputs[iData];
		lpIOData->Initialize(lpSim, lpStructure, this);
	}

	//Open the comm port for data communications
	string strCom = "COM" + STR(m_iCommPort) + ":";

#ifdef _WIN32_WCE
	wchar_t *sCom = Std_ConvertFromANSI(strCom);
	m_hCom = CreateFile(sCom, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(sCom) delete sCom;
#else
	m_hCom = CreateFile((LPCSTR) strCom.c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
#endif

	if(m_hCom == INVALID_HANDLE_VALUE)
		THROW_PARAM_ERROR(Al_Err_lOpenCommPort, Al_Err_strOpenCommPort, "CommPort", strCom);

	//Configure serial port
	DCB dcb;
	dcb.DCBlength = sizeof(dcb);

	if(!GetCommState(m_hCom, &dcb))
		THROW_PARAM_ERROR(Al_Err_lGetCommState, Al_Err_strGetCommState, "CommPort", strCom);

	dcb.BaudRate = m_iBaudRate;
	dcb.fNull = FALSE;
	dcb.StopBits = m_iStopBits;
	dcb.Parity = m_iParity;
	dcb.ByteSize = m_iByteSize;

	if(m_iParity == NOPARITY)
		dcb.fParity = FALSE;
	else
		dcb.fParity = TRUE;

	if(!SetCommState(m_hCom, &dcb))
		THROW_PARAM_ERROR(Al_Err_lSetCommState, Al_Err_strSetCommState, "CommPort", strCom);

	COMMTIMEOUTS commTimeout;
				
	if(!GetCommTimeouts(m_hCom, &commTimeout)) /* Configuring Read & Write Time Outs */
		THROW_PARAM_ERROR(Al_Err_lSetCommState, Al_Err_strSetCommState, "CommPort", strCom);

	commTimeout.ReadIntervalTimeout = MAXDWORD; 
	commTimeout.ReadTotalTimeoutMultiplier = 0;
	commTimeout.ReadTotalTimeoutConstant = 0;
	commTimeout.WriteTotalTimeoutMultiplier = 0;
	commTimeout.WriteTotalTimeoutConstant = 0;

	if(!SetCommTimeouts(m_hCom, &commTimeout))
		THROW_PARAM_ERROR(Al_Err_lSetCommState, Al_Err_strSetCommState, "CommPort", strCom);
	
	PurgeComm(m_hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);
}

BOOL Microcontroller::FindReadHeader(int iBufferSize, int &iDataStart)
{

	//Start at the end of the buffer and move back till we reach the end or find a read header.
	for(int iIndex=(iBufferSize - m_strCommandHeader.length()); iIndex>=0; iIndex--)
	{
		//Look for the first char. If we find it then check the rest of the chars.
		if(m_aryReadBuffer[iIndex] == m_strCommandHeader[0])
		{
			BOOL bNoMatch = FALSE;
			for(int iCmdIndex=1; iCmdIndex<m_strCommandHeader.length() && !bNoMatch; iCmdIndex++)
				if(m_aryReadBuffer[iIndex+iCmdIndex] != m_strCommandHeader[iCmdIndex])
					bNoMatch = TRUE;

			//We have found something that matches the header, now lets make sure there is enough data
			//left after the header to fill the sensory array. If not then keep searching.
			if(!bNoMatch)
			{
				if((iIndex+m_iSensorDataSize+1) <= iBufferSize)
				{
					iDataStart = iIndex+m_strCommandHeader.length();
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

char Microcontroller::CalculateChecksum(char *lpArray, int iStart, int iEnd)
{
	char iSum = 0;

	for(int iIndex=iStart; iIndex<iEnd; iIndex++)
		iSum = iSum ^ lpArray[iIndex];

	return iSum;
}

void Microcontroller::Update()
{
	//m_iUpdateTime++;
	//m_iTickCount = GetTickCount();

	////Every 5th update call we should check the tick count to see if the simulation is running too fast.
	////if it is then we need to just loop the update function until real time catches up with sim time.
	//if(m_iUpdateTime >= 5)
	//{
	//	Update();

	//	//While the real millisecond count is not greater than at least one second less than the sim milliseconds.
	//	while( !((m_iTickCount - m_lpSim->StartSimTick()) >= (m_lpSim->Millisecond()-1))  )
	//	{
	//		m_iTickCount = GetTickCount();
	//		Update();
	//	}

	//	m_iUpdateTime = 0; 
	//}
	//else
	//	Update();
}

void Microcontroller::UpdateData()
{
	DWORD iTransfer=0;
	int iIndex;
	IODataEntry *lpIOData = NULL;

	m_iTickCount = GetTickCount();
	if((m_iTickCount-m_iLastMotorUpdate) >= m_iMotorUpdateInterval)
	{
		//First lets write the motor data out.
		//Lets update the motor data with the latest info from the neural net
		lpIOData = NULL;
		iIndex = m_iMotorOffset*8;
		for(int iData=0; iData<m_iOutputCount; iData++)
		{
			lpIOData = m_aryOutputs[iData];
			lpIOData->ProcessOutputArray(iIndex);
		} 

		//Set the Checksum value
		m_aryMotorData[m_iMotorDataSize] = CalculateChecksum(m_aryMotorData, m_iMotorOffset, m_iMotorDataSize);

		//Debug code
		//string strMessage = "Motor data Size: " + STR(m_iMotorDataSize) + "  : ";
		//for(int iData=0; iData<=m_iMotorDataSize; iData++)
		//{
		//	strMessage += Std_ToStr("%2x", m_aryMotorData[iData]) + " ";
		//}
		//strMessage += "\n";
		//Std_TraceMsg(0, strMessage);
		//Std_LogMsg(0, strMessage);

		WriteFile(m_hCom, m_aryMotorData, (m_iMotorDataSize+1), &iTransfer, 0);
		m_iLastMotorUpdate = GetTickCount();
		//m_lUpdateCount++;
	}

	//Now lets try and read in the sensor data.
	ReadFile(m_hCom, m_aryReadBuffer, READ_BUF_SIZE, &iTransfer, 0);
	if(iTransfer >= m_iSensorDataSize)
	{
		int iDataStart=0;

		//Find the last instance of the read header in the buffer so we
		//use the last sensor data sent.
		if(FindReadHeader(iTransfer, iDataStart))
		{
			//Copy the sensory data from the read buffer to the sensory data array.
			memcpy(m_arySensorData, (m_aryReadBuffer+iDataStart), (m_iSensorSize+1));

			char iCheckSum = CalculateChecksum(m_arySensorData, 0, m_iSensorSize);

			//Debug code
			//string strMessage = "Sensor data: ";
			//for(int iData=0; iData<m_iSensorSize; iData++)
			//{
			//	strMessage += Std_ToStr("%2x", m_arySensorData[iData]) + " ";
			//}
			//strMessage += "\n";
			//Std_TraceMsg(0, strMessage);

			//Process the sensory data, but only if the checksum matches.
			//otherwise we have gotten corrupted data during the transmission.
			if(iCheckSum == m_arySensorData[m_iSensorSize])
			{
				m_lUpdateCount++;

				iIndex=0;
				for(int iData=0; iData<m_iInputCount; iData++)
				{
					lpIOData = m_aryInputs[iData];
					lpIOData->ProcessInputArray(iIndex);
				}
			}
		}
	}
}

void Microcontroller::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	oXml.IntoElem();  //Into Microcontroller Element

	m_strName = oXml.GetChildString("Name", "");
	m_strID = oXml.GetChildString("ID", "");
	m_iCommPort = oXml.GetChildInt("CommPort", m_iCommPort);
	m_iBaudRate = oXml.GetChildInt("BaudRate", m_iBaudRate);
	m_iByteSize = oXml.GetChildInt("ByteSize", m_iByteSize);
	m_iStopBits = oXml.GetChildInt("StopBits", m_iStopBits);
	m_iParity = oXml.GetChildInt("Parity", m_iParity);
	m_iSensorSize = oXml.GetChildInt("InputArraySize");
	m_iMotorSize = oXml.GetChildInt("OutputArraySize");
	m_iMaxMotorUpdatesPerSec = oXml.GetChildInt("MaxMotorUpdatesPerSec", m_iMaxMotorUpdatesPerSec);

	Std_IsAboveMin((int) 0, m_iSensorSize, true, "SensorSize", TRUE);
	Std_IsAboveMin((int) 0, m_iMotorSize, true, "MotorSize", TRUE);
	Std_IsAboveMin((int) 0, m_iMaxMotorUpdatesPerSec, true, "MaxMotorUpdatesPerSec", TRUE);

	m_iMotorUpdateInterval = (int) (1000.0/m_iMaxMotorUpdatesPerSec);

	IODataEntry *lpIOData = NULL;
	m_aryInputs.RemoveAll();
	if(oXml.FindChildElement("Inputs", FALSE))
	{
		oXml.IntoElem();

		int iCount = oXml.NumberOfChildren();
		for(int iIndex=0; iIndex<iCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			lpIOData = LoadIOData(lpSim, lpStructure, oXml);		
			m_aryInputs.Add(lpIOData);
		}

		oXml.OutOfElem(); //OutOf IOData Element
	}

	m_aryOutputs.RemoveAll();
	if(oXml.FindChildElement("Outputs", FALSE))
	{
		oXml.IntoElem();

		int iCount = oXml.NumberOfChildren();
		for(int iIndex=0; iIndex<iCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			lpIOData = LoadIOData(lpSim, lpStructure, oXml);		
			m_aryOutputs.Add(lpIOData);
		}

		oXml.OutOfElem(); //OutOf IOData Element
	}

	oXml.OutOfElem(); //OutOf Microcontroller Element
}

IODataEntry *Microcontroller::LoadIOData(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	IODataEntry *lpIOData = NULL;

try
{
	lpIOData = new IODataEntry();
	lpIOData->Load(lpSim, lpStructure, oXml);

	return lpIOData;
}
catch(CStdErrorInfo oError)
{
	if(lpIOData) delete lpIOData;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpIOData) delete lpIOData;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}


void Microcontroller::Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{

}



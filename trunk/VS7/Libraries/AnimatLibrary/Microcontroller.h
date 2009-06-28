// BistableNeuron.h: interface for the BistableNeuron class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROCONTROLLER_H__D9D0316C_3191_429A_B3DF_1B52CF9E68BC__INCLUDED_)
#define AFX_MICROCONTROLLER_H__D9D0316C_3191_429A_B3DF_1B52CF9E68BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{
	namespace Environment
	{
#define READ_BUF_SIZE 2048

		class ANIMAT_PORT Microcontroller  : public CStdSerialize 
		{
		protected:
			string m_strName;
			string m_strID;
			int m_iCommPort;
			int m_iBaudRate;
			int m_iByteSize;
			int m_iStopBits;
			int m_iParity;
			int m_iMaxMotorUpdatesPerSec;

			DWORD m_iLastMotorUpdate;
			DWORD m_iMotorUpdateInterval;

			//float m_fltLastMotorUpdate;
			//float m_fltMotorUpdateInterval;

			DWORD m_iTickCount;
			int m_iUpdateTime;

			CStdPtrArray<IODataEntry> m_aryInputs;
			CStdPtrArray<IODataEntry> m_aryOutputs;

			int m_iInputCount;
			int m_iOutputCount;

			string m_strCommandHeader;
			char m_cMotorCommand;
			char m_cSensorCommand;
			char m_cCmd[1];

			int m_iSensorDataSize;
			int m_iSensorSize;
			char *m_arySensorData;

			int m_iMotorSize;
			int m_iMotorOffset;
			int m_iMotorDataSize;
			char *m_aryMotorData;
			DWORD m_iTransfer;

			char m_aryReadBuffer[READ_BUF_SIZE];

			long m_lUpdateCount;

			HANDLE m_hCom;

			Simulator *m_lpSim;
			Organism *m_lpOrganism;

			IODataEntry *LoadIOData(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
			virtual BOOL FindReadHeader(int iBufferSize, int &iDataStart);

			void Update();

		public:
			Microcontroller();
			virtual ~Microcontroller();

			int MotorSize() {return m_iMotorSize;};
			int MotorOffset() {return m_iMotorOffset;};
			int MotorDataSize() {return m_iMotorDataSize;};
			char *MotorData() {return m_aryMotorData;};

			int SensorSize() {return m_iSensorSize;};
			char *SensorData() {return m_arySensorData;};

			Simulator *Sim() {return m_lpSim;};
			long UpdateCount() {return m_lUpdateCount;};

			char CalculateChecksum(char *lpArray, int iStart, int iEnd);

			virtual void Initialize(Simulator *lpSim, Structure *lpStructure);
			virtual void UpdateData();

			virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		};

	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_MICROCONTROLLER_H__D9D0316C_3191_429A_B3DF_1B52CF9E68BC__INCLUDED_)

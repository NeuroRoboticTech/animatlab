// Range.h: interface for the CDsRange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSRANGE_H__7D50A921_6E91_44ED_8056_D1089CC5BF45__INCLUDED_)
#define AFX_DSRANGE_H__7D50A921_6E91_44ED_8056_D1089CC5BF45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{
	namespace Environment
	{
		class Microcontroller;

		class ANIMAT_PORT IODataEntry : public CStdSerialize    
		{
		public:
			string m_strID;
			string m_strName;

			Microcontroller *m_lpParent;
			char *m_arySensorData;
			char *m_aryMotorData;
			int m_iMotorDataSize;
			int m_iSensorSize;
			int m_iDataSize;

			float *m_lpDataValue;
			DataColumn *m_lpData;
			float *m_lpLastSentValue;
			DataColumn *m_lpLastSentData;

			Gain *m_lpGain;

			//Temp Data
			int m_iByte;
			int m_iShift;
			char m_cVal;

			DataColumn *LoadDataColumn(Simulator *lpSim, CStdXml &oXml);
			void SetArrayElement(char *aryData, int iSize, int iVal, int iStart);
			int GetArrayElement(char *aryData, int iSize, int iStart);

		public:
			IODataEntry();
			virtual ~IODataEntry();

			virtual void Initialize(Simulator *lpSim, Structure *lpStructure, Microcontroller *lpController);

			void ProcessOutputArray(int &iStart);
			void ProcessInputArray(int &iStart);

			//CStdSerialize overloads
			//virtual CStdSerialize *Clone();
			//virtual void Trace(ostream &oOs) {};
			virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		};

	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_DSRANGE_H__7D50A921_6E91_44ED_8056_D1089CC5BF45__INCLUDED_)

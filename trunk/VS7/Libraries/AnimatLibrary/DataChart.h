// DataChart.h: interface for the DataChart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATACHART_H__D91DC66E_01F1_47FC_AB62_766BA63FCEF0__INCLUDED_)
#define AFX_DATACHART_H__D91DC66E_01F1_47FC_AB62_766BA63FCEF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{
	namespace Charting
	{

		class ANIMAT_PORT DataChart : public ActivatedItem  
		{
		protected:
			string m_strProjectPath;
			string m_strConfigFilename;
			BOOL m_bSetStartEndTime;
			BOOL m_bUpdateChartAtEnd;
			short m_iCollectInterval;
			long m_lCollectTimeWindow;
			float m_fltCollectTimeWindow;

			CStdMap<string, DataColumn *> m_aryColumnsMap;
			CStdPtrArray<DataColumn> m_aryDataColumns;
			float *m_aryTimeBuffer;
			float *m_aryDataBuffer;

			long m_lColumnCount;
			long m_lRowCount;
			int m_lCurrentCol;
			int m_lCurrentRow;

			virtual long CalculateChartColumnCount();
			DataColumn *LoadDataColumn(Simulator *lpSim, CStdXml &oXml);
			virtual DataColumn *FindColumn(string strID, int &iIndex, BOOL bThrowError);

		public:
			DataChart();
			virtual ~DataChart();

			long BufferSize() {return m_lRowCount*m_lColumnCount;};
			long UsedBufferSize() {return m_lCurrentCol*m_lCurrentRow;};

			long BufferByteSize() {return BufferSize()*sizeof(float);};
			long UsedBufferByteSize() {return UsedBufferSize()*sizeof(float);};

			float *TimeBuffer() {return m_aryTimeBuffer;};
			float *DataBuffer() {return m_aryDataBuffer;};

			int CollectInterval() {return m_iCollectInterval;};
			void CollectInterval(int iVal) {m_iCollectInterval = iVal;};

			long CollectTimeWindow() {return m_lCollectTimeWindow;};
			void CollectTimeWindow(long lVal) {m_lCollectTimeWindow = lVal;};

			string ProjectPath() {return m_strProjectPath;};
			void ProjectPath(string strVal) {m_strProjectPath = strVal;};

			long ColumnCount() {return m_lColumnCount;};

			long CurrentRow() {return m_lCurrentRow;};
			virtual void CurrentRow(long iVal) {m_lCurrentRow = iVal;};

			virtual void AddData(int iColumn, int iRow, float fltVal);

			virtual void Load(Simulator *lpSim, string strProjectPath, string strConfigFile);
			virtual void Load(Simulator *lpSim, CStdXml &oXml);
			virtual void LoadChart(Simulator *lpSim, CStdXml &oXml);

			virtual void Trace(ostream &oOs);

			//ActiveItem overrides
			virtual string Type() {return "DataChart";};

			virtual void AddColumn(DataColumn *lpColumn);
			virtual void RemoveColumn(string strID, BOOL bThrowError = TRUE);
			virtual DataColumn *FindColumn(string strID, BOOL bThrowError = TRUE);

			virtual BOOL operator<(ActivatedItem *lpItem);
			virtual void Initialize(Simulator *lpSim);
			virtual void ReInitialize(Simulator *lpSim);
			virtual void Activate(Simulator *lpSim);
			virtual void StepSimulation(Simulator *lpSim);
			virtual void Deactivate(Simulator *lpSim);

			virtual BOOL Lock() {return TRUE;};
			virtual void Unlock() {};

		};

	}			//Charting
}				//AnimatLibrary

#endif // !defined(AFX_DATACHART_H__D91DC66E_01F1_47FC_AB62_766BA63FCEF0__INCLUDED_)

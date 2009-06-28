// NeuralModule.h: interface for the NeuralModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEURAL_MODULE_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)
#define AFX_NEURAL_MODULE_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

/*! \brief 
   xxxx.

   \remarks
   xxxx
		 
   \sa
	 xxx
	 
	 \ingroup AnimatLibrary
*/

namespace AnimatLibrary
{
	namespace Behavior
	{

		class ANIMAT_PORT NeuralModule : public CStdSerialize 
		{
		protected:
			short m_iTimeStepInterval;
			float m_fltTimeStep;
			short m_iTimeStepCount;

			string m_strProjectPath;
			string m_strNeuralNetworkFile;

			IStdClassFactory *m_lpClassFactory;

			CStdArray<Adapter *> m_arySourceAdapters;
			CStdArray<Adapter *> m_aryTargetAdapters;

			short m_iTargetAdapterCount;

			Simulator *m_lpSim;
			Structure *m_lpStructure;

		public:
			NeuralModule();
			virtual ~NeuralModule();

			virtual string ModuleName() = 0;

			virtual string ProjectPath();
			virtual void ProjectPath(string strPath);

			virtual string NeuralNetworkFile();
			virtual void NeuralNetworkFile(string strFile);

			virtual short TimeStepInterval();
			virtual void TimeStepInterval(short iVal);

			virtual float TimeStep();
			virtual void TimeStep(float fltVal);

			virtual Node *FindNode(long lNodeID) = 0;
			virtual IStdClassFactory *ClassFactory() {return m_lpClassFactory;};

			Simulator *GetSimulator() {return m_lpSim;};
			Structure *GetStructure() {return m_lpStructure;};

			virtual void Kill(Simulator *lpSim, Organism *lpOrganism, BOOL bState = TRUE) = 0;
			virtual void Reset(Simulator *lpSim, Organism *lpOrganism) = 0;

			virtual void Initialize(Simulator *lpSim, Structure *lpStructure);

			virtual void AttachSourceAdapter(Adapter *lpAdapter);
			virtual void AttachTargetAdapter(Adapter *lpAdapter);

			virtual long CalculateSnapshotByteSize() = 0;
			virtual void SaveKeyFrameSnapshot(byte *aryBytes, long &lIndex) = 0;
			virtual void LoadKeyFrameSnapshot(byte *aryBytes, long &lIndex) = 0;

			virtual BOOL NeedToStep();
			virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
			virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml) = 0;
			virtual void Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml) = 0;
		};

	}			//Behavior
}			//AnimatLibrary

#endif // !defined(AFX_NEURAL_MODULE_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)

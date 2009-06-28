// Brain.h: interface for the FastNeuralModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BRAIN_H__092F1F0E_3D90_4E91_81E8_19022622D6EA__INCLUDED_)
#define AFX_BRAIN_H__092F1F0E_3D90_4E91_81E8_19022622D6EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace FastNeuralNet
{

	class FAST_NET_PORT FastNeuralModule : public AnimatLibrary::Behavior::NeuralModule  
	{
	protected:
		CStdIPoint m_oNetworkSize;
		CStdPtrArray<Neuron> m_aryNeurons;

		BOOL m_bActiveArray;

		Neuron *LoadNeuron(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		void LoadNetworkXml(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);

		void GenerateAutoSeed();

	public:
		FastNeuralModule();
		virtual ~FastNeuralModule();

		//NeuralModule overrides
		virtual string ModuleName() {return Nl_NeuralModuleName();};

		virtual Node *FindNode(long lNodeID);

		virtual void Kill(Simulator *lpSim, Organism *lpOrganism, BOOL bState = TRUE);
		virtual void Reset(Simulator *lpSim, Organism *lpOrganism);

		virtual void Initialize(Simulator *lpSim, Structure *lpStructure);
		virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
		virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		virtual void Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);

		BOOL ActiveArray();
		void ActiveArray(BOOL bVal);
		BOOL InactiveArray();
		void InactiveArray(BOOL bVal);

		Neuron *GetNeuron(short iXPos, 
											short iYPos, 
											short iZPos);
		void SetNeuron(unsigned char iXPos, 
									unsigned char iYPos, 
									unsigned char iZPos, 
									Neuron *lpNeuron);

		virtual long CalculateSnapshotByteSize();
		virtual void SaveKeyFrameSnapshot(byte *aryBytes, long &lIndex);
		virtual void LoadKeyFrameSnapshot(byte *aryBytes, long &lIndex);
	};

}				//FastNeuralNet

#endif // !defined(AFX_BRAIN_H__092F1F0E_3D90_4E91_81E8_19022622D6EA__INCLUDED_)

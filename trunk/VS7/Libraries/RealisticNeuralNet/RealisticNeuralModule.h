// RealisticNeuralModule.h: interface for the RealisticNeuralModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NERVOUSSYSTEM_H__6581CA8B_B028_4C79_A98B_33514514B867__INCLUDED_)
#define AFX_NERVOUSSYSTEM_H__6581CA8B_B028_4C79_A98B_33514514B867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// these forward declarations are necessary because 
//class Neuron;
//class SpikingChemicalSynapse;
//class NonSpikingChemicalSynapse;
//class ElectricalSynapse;
//class CurrentStimulus;
//class Connexion;

namespace RealisticNeuralNet
{

	class ADV_NEURAL_PORT RealisticNeuralModule : public AnimatLibrary::Behavior::NeuralModule 
	{
	protected:
	// NervousSystem
		double m_dTimeStep;
		double m_dTime;

		BOOL m_bTTX;
		BOOL m_bCd;
		BOOL m_bHH;

	// NervousSystem/Synapses/SpikingSynapses
		BOOL m_bRetainHebbMemory;
		BOOL m_bUseCriticalPeriod;
		double m_dStartCriticalPeriod;
		double m_dEndCriticalPeriod;
		BOOL m_bFreezeHebb;

	// internal Hebb stuff
		BOOL m_bNeedInitialiseHebb;		
		BOOL m_bRandomisedHebb;
		BOOL m_bFreezeLearning;	// used internally as flag, not saved

		CStdPtrArray<Neuron> m_aryNeurons;
		CStdPtrArray<SpikingChemicalSynapse> m_arySpikingChemSyn;
		CStdPtrArray<NonSpikingChemicalSynapse> m_aryNonSpikingChemSyn;
		CStdPtrArray<ElectricalSynapse> m_aryElecSyn;
		//CStdPtrArray<CurrentStimulus> m_aryStim;
		CStdPtrArray<Connexion> m_aryConnexion;

		Neuron *LoadNeuron(CStdXml &oXml);
		SpikingChemicalSynapse *LoadSpikingChemSyn(CStdXml &oXml);
		NonSpikingChemicalSynapse *LoadNonSpikingChemSyn(CStdXml &oXml);
		ElectricalSynapse *LoadElecSyn(CStdXml &oXml);
		//CurrentStimulus *LoadStim(CStdXml &oXml);
		Connexion *LoadConnexion(CStdXml &oXml);

	/////////////////////////////
	// ENGINE
		double m_dCurrentTime;

	public:
		RealisticNeuralModule();
		virtual ~RealisticNeuralModule();
		virtual void Load(CStdXml &oXml);

	////////////////////////
		void SetCurrentTime(double t) {m_dCurrentTime=t;}
		double GetCurrentTime() {return m_dCurrentTime;}
		double GetTimeStep() {return m_dTimeStep;}
		double GetTimeToRun() {return m_dTime;}

	// neuron stuff
		int GetNeuronCount() {return m_aryNeurons.size();}
		Neuron *GetNeuronAt(int i) {return m_aryNeurons[i];}

	// stim stuff
		//int GetStimCount() {return m_aryStim.size();}
		//CurrentStimulus *GetStimAt(int i) {return m_aryStim[i];}

	// connexion stuff
		int GetConnexionCount() {return m_aryConnexion.size();}
		Connexion *GetConnexionAt(int i) {return m_aryConnexion[i];}

		BOOL Cd() {return m_bCd;}
		BOOL TTX() {return m_bTTX;}
		BOOL HH() {return m_bHH;}

	// Synapse stuff
		int GetSpikingChemSynCount() {return m_arySpikingChemSyn.size();}
		SpikingChemicalSynapse *GetSpikingChemSynAt(int i) {return m_arySpikingChemSyn[i];}
		int GetNonSpikingChemSynCount() {return m_aryNonSpikingChemSyn.size();}
		NonSpikingChemicalSynapse *GetNonSpikingChemSynAt(int i) {return m_aryNonSpikingChemSyn[i];}
		int GetElecSynCount() {return m_aryElecSyn.size();}
		ElectricalSynapse *GetElecSynAt(int i) {return m_aryElecSyn[i];}

	// the CALCULATION
		void PreCalc();
		void CalcUpdate();
		void PostCalc();
		double GetMemPot(int neuronID);
		double GetScaleElecCond(double minG,double maxG,double jV, double ThreshV,double SaturateV);
		void ScaleCondForVoltDep(double& G,double postV,double maxV,double minV,double scl);
		void ScaleCondForNonSpiking(double& G,double PreV,double ThreshV,double SaturateV);


		//NeuralModule Overrides
		virtual string ModuleName() {return Rn_NeuralModuleName();};

		virtual Node *FindNode(long lNodeID);

		virtual void Kill(Simulator *lpSim, Organism *lpOrganism, BOOL bState = TRUE);
		virtual void Reset(Simulator *lpSim, Organism *lpOrganism);

		virtual void Initialize(Simulator *lpSim, Structure *lpStructure);

		virtual long CalculateSnapshotByteSize()  {return 0;};
		virtual void SaveKeyFrameSnapshot(byte *aryBytes, long &lIndex) {};
		virtual void LoadKeyFrameSnapshot(byte *aryBytes, long &lIndex) {};

		virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
		virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		virtual void Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		//NeuralModule Overrides

	};

}				//RealisticNeuralNet


#endif // !defined(AFX_NERVOUSSYSTEM_H__6581CA8B_B028_4C79_A98B_33514514B867__INCLUDED_)

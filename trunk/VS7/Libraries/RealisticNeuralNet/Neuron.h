// Neuron.h: interface for the Neuron class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEURON_H__8C9256F2_445F_4E31_963E_08B9DBE29601__INCLUDED_)
#define AFX_NEURON_H__8C9256F2_445F_4E31_963E_08B9DBE29601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RealisticNeuralModule;

namespace RealisticNeuralNet
{

	class ADV_NEURAL_PORT Neuron : public AnimatLibrary::Node
	{
	protected:
	/*
		string m_strID;

		float m_fltGk;
		float m_fltCk;

		float m_fltGna;
		float m_fltCna;
	*/

	/////////////////////////////////////
	// LOADABLE PARAMETERS

		static double m_dSpikePeak;
		static double m_dSpikeStrength;
		static double m_dAHPEquilPot;		// equil pot for K
		static double m_dCaEquilPot;
		static double m_dAbsoluteRefr;

		float m_fltIonChannelStandin;

	// electrical state
		BOOL m_bZapped;
	// individual, basic properties
			double m_dRestingPot;
			double m_dSize;
			double m_dTimeConst;
			double m_dCm;
			double m_dInitialThresh;
			double m_dRelativeAccom;
			double m_dAccomTimeConst;
			double m_dAHPAmp;
			double m_dAHPTimeConst;
	// burster properties
			double m_dGMaxCa;
			double m_dVM;		//activation mid point
			double m_dSM;		// activation slope
			double m_dMTimeConst;	// activation time constant
			double m_dVH;		// inactivation
			double m_dSH;
			double m_dHTimeConst;

		CStdArray<double> m_aryTonicInputPeriod;
		CStdArray<int> m_aryTonicInputPeriodType;
		double m_dToniCurrentStimulusulus;
		double m_dNoise;

		CStdPtrArray<IonChannel> m_aryIonChannels;

	//////////////////////////////
	// WORKING STUFF
		static double m_dDT;
		double m_dMemPot;
		double m_dNewMemPot;
		double m_dThresh;
		BOOL m_bSpike;		// spike flag

		// electrical synapse current
		double m_dElecSynCur;
		double m_dElecSynCond;
		// non-spiking chemical synapse current
		double m_dNonSpikingSynCur;
		double m_dNonSpikingSynCond;
		// calculation stuff
		double m_dRefrCountDown;
		double m_dDCTH;	// expon decline working factor for thresh accomm
		double m_dDGK;	// expon decline factor for AHP
		double m_dGK;	// cummulative amp of AHP conductance
		double m_dGTot;	// cummulative total contuctance

		double m_dStim;
		float m_fltAdapterI;
		float m_fltAdapterMemoryI;  //Used to allow datacharts to track current input from adapters.
		float m_fltExternalI;
		float m_fltChannelI;
		float m_fltChannelMemoryI;
		float m_fltTotalI;
		float m_fltTotalMemoryI;
		float m_fltMemPot;
		float m_fltThresholdMemory;  //Used to allow us to chart the threshold if needed.
		float m_fltElecSynCurMemory;
		float m_fltSpikingSynCurMemory;
		float m_fltNonSpikingSynCurMemory;
		float m_fltSpike;

		int m_iIonChannels;

		// for bursting	
		double m_dM;
		double m_dH;

		//Vars to calculate the firing frequency of this neuron.
		double m_fltLastSpikeTime;
		float m_fltFiringFreq;

		//Used to return the membrane conductance of this neuron in GetDataPointers.
		float m_fltGm;
		float m_fltVrest;

		CStdArray<double> m_arySynG;	// current conductance of each synaptic type
		
		CStdArray<double> m_aryFacilSponSynG;	// facilitated initial g increase caused by spontaneous input
		CStdArray<double> m_aryNextSponSynTime;	// time to next spontaneous occurrence of this syn type

		CStdArray<double> m_aryDG;	// exponential decline factor in syn G COULD THIS BE STATIC???? (or put in synapse??)
		CStdArray<double> m_aryFacilD;		// exponential decline factor in facilitation COULD THIS BE STATIC???? (or put in synapse??)

		virtual IonChannel *LoadIonChannel(CStdXml &oXml, RealisticNeuralModule *lpNS);
		IonChannel *FindIonChannel(string strID, BOOL bThrowError);

	protected:
		//void ClearSpikeTimes();
		//void StoreSpikeForFreqAnalysis(RealisticNeuralModule *lpNS);
		void CalculateFiringFreq(RealisticNeuralModule *lpNS);

	public:
		Neuron();
		virtual ~Neuron();
		virtual void Load(CStdXml &oXml, RealisticNeuralModule *lpNS);

		virtual BOOL Enabled() {return m_bZapped;};
		virtual void Enabled(BOOL bValue) {m_bZapped = bValue;};

	//////
	// ENGINE
		double GetRestingPot() {return m_dRestingPot;}
		double GetMemPot() {return m_bZapped?0:(m_bSpike?m_dSpikePeak:m_dMemPot);}
		double GetThresh() {return m_bZapped?0:m_dThresh;}
		BOOL GetSpike() {return m_bZapped?FALSE:m_bSpike;}
		BOOL GetZapped() {return m_bZapped;}
		void IncrementStim(double stim) {m_dStim+=stim;}
		void InElectricalSynapseCurr(double cur) {m_dElecSynCur+=cur;}
		void InElectricalSynapseCond(double cond) {m_dElecSynCond+=cond;}
		void IncNonSpikingSynCurr(double cur) {m_dNonSpikingSynCur+=cur;}
		void IncNonSpikingSynCond(double cond) {m_dNonSpikingSynCond+=cond;}

		CStdPtrArray<IonChannel> *IonChannels() {return &m_aryIonChannels;};

		void PreCalc(RealisticNeuralModule *lpNS);
		void CalcUpdate(RealisticNeuralModule *lpNS);
		void CalcUpdateFinal(RealisticNeuralModule *lpNS);
		void PostCalc(RealisticNeuralModule *lpNS);

		//Node Overrides
		virtual void AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput);
		virtual float *GetDataPointer(string strDataType);
		virtual void *GetDataItem(string strItemType, string strID, BOOL bThrowError = TRUE);
		virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
		virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		virtual void Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		//Node Overrides

	friend class RealisticNeuralModule;
	};

}				//RealisticNeuralNet

#endif // !defined(AFX_NEURON_H__8C9256F2_445F_4E31_963E_08B9DBE29601__INCLUDED_)

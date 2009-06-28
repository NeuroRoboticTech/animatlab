// Neuron.cpp: implementation of the Neuron class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IonChannel.h"
#include "RealisticNeuralModule.h"
#include "Connexion.h"
//#include "EventTrigger.h"
//#include "CurrentStimulus.h" 
#include "Neuron.h"
#include "ElectricalSynapse.h"
#include "NonSpikingChemicalSynapse.h"
#include "SpikingChemicalSynapse.h"
#include "Neuron.h"

namespace RealisticNeuralNet
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double Neuron::m_dSpikePeak=0;
double Neuron::m_dSpikeStrength=1;
double Neuron::m_dAHPEquilPot=-70;		// equil pot for K
double Neuron::m_dCaEquilPot=200;
double Neuron::m_dAbsoluteRefr=2;
double Neuron::m_dDT=0.5;

Neuron::Neuron()
{
	m_fltAdapterI = 0;
	m_fltAdapterMemoryI = 0;
	m_fltExternalI = 0;
	m_fltChannelI = 0;
	m_fltChannelMemoryI = 0;
	m_fltMemPot = 0;
	m_fltThresholdMemory = 0;
	m_fltLastSpikeTime = 0;
	m_fltFiringFreq = 0;
	m_fltElecSynCurMemory = 0;
	m_fltSpikingSynCurMemory = 0;
	m_fltNonSpikingSynCurMemory = 0;
	m_iIonChannels = 0;
	m_fltSpike = 0;
	m_fltIonChannelStandin = 0;
	m_dCm = 0;
}

Neuron::~Neuron()
{
	//if(m_arySpikeTimes)
	//{
	//	delete m_arySpikeTimes;
	//	m_arySpikeTimes = NULL;
	//}

}


void Neuron::Load(CStdXml &oXml, RealisticNeuralModule *lpNS)
{
	int i;
	int j;
	double d;

	m_aryTonicInputPeriod.RemoveAll();
	m_aryTonicInputPeriodType.RemoveAll();

	oXml.IntoElem();  //Into Neuron Element
	m_strName = oXml.GetChildString("Name");
	m_bZapped=oXml.GetChildBool("Zapped");
	m_dToniCurrentStimulusulus=oXml.GetChildDouble("TonicStimulus");
	m_dNoise=oXml.GetChildDouble("Noise");
	m_dRestingPot=oXml.GetChildDouble("RestingPot");
	m_dSize=oXml.GetChildDouble("Size");
	m_dTimeConst=oXml.GetChildDouble("TimeConst");
	m_dInitialThresh=oXml.GetChildDouble("InitialThresh");
	m_dRelativeAccom=oXml.GetChildDouble("RelativeAccom");
	m_dAccomTimeConst=oXml.GetChildDouble("AccomTimeConst");
	m_dAHPAmp=oXml.GetChildDouble("AHPAmp");
	m_dAHPTimeConst=oXml.GetChildDouble("AHPTimeConst");
	m_dGMaxCa=oXml.GetChildDouble("GMaxCa");
	m_dVM=oXml.GetChildDouble("VM");
	m_dSM=oXml.GetChildDouble("SM");
	m_dMTimeConst=oXml.GetChildDouble("MTimeConst");
	m_dVH=oXml.GetChildDouble("VH");
	m_dSH=oXml.GetChildDouble("SH");
	m_dHTimeConst=oXml.GetChildDouble("HTimeConst");

//	Std_LogMsg(0, "Test", __FILE__, __LINE__);
//	LOG_MSG(0, STR(m_dHTimeConst)+ " H time const: ");

	if(oXml.FindChildElement("NeuronTonicInputs", FALSE) )
	{
		oXml.IntoElem();
		int iTotalNeuronTonicInputs = oXml.NumberOfChildren();
		for(i=0; i<iTotalNeuronTonicInputs; i++)
		{
			oXml.FindChildByIndex(i);
			oXml.IntoElem();  //Into NeuronTonicInput Element
			d=oXml.GetChildDouble("TonicInputPeriod");
			j=oXml.GetChildInt("TonicInputPeriodType");
			m_aryTonicInputPeriod.Add(d);
			m_aryTonicInputPeriodType.Add(j);
			oXml.OutOfElem(); //OutOf NeuronTonicInput Element
		}
		oXml.OutOfElem(); //OutOf NeuronTonicInputs Element
	}
	else
	{
		int iSpikingChemSynCount=lpNS->GetSpikingChemSynCount();
		for(i=0; i<iSpikingChemSynCount; i++)
		{
			m_aryTonicInputPeriod.Add(0);
			m_aryTonicInputPeriodType.Add(0);
		}
	}


	m_aryIonChannels.RemoveAll();
	if(oXml.FindChildElement("IonChannels", FALSE) )
	{
		oXml.IntoElem();

		m_iIonChannels = oXml.NumberOfChildren();
		for(int iIndex=0; iIndex<m_iIonChannels; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			LoadIonChannel(oXml, lpNS);		
		}

		oXml.OutOfElem();
	}

	oXml.OutOfElem(); //OutOf Neuron Element
}

IonChannel *Neuron::LoadIonChannel(CStdXml &oXml, RealisticNeuralModule *lpNS)
{
	IonChannel *lpIonChannel = NULL;

try
{
	lpIonChannel = new IonChannel();
	lpIonChannel->Load(oXml, lpNS);
	m_aryIonChannels.Add(lpIonChannel);

	return lpIonChannel;
}
catch(CStdErrorInfo oError)
{
	if(lpIonChannel) delete lpIonChannel;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpIonChannel) delete lpIonChannel;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

//void Neuron::ClearSpikeTimes()
//{
//	for(int iIndex=0; iIndex<m_iSpikesToKeepForFreqAnal; iIndex++)
//		m_arySpikeTimes[iIndex] = -1;
//}
/*
void Neuron::StoreSpikeForFreqAnalysis(RealisticNeuralModule *lpNS)
{
	//First push all of the current values down the list.
	for(int iIndex=(m_iSpikesToKeepForFreqAnal-1); iIndex>0; iIndex--)
		m_arySpikeTimes[iIndex] = m_arySpikeTimes[iIndex-1];

	//Now put this one on as the first one.
	m_arySpikeTimes[0] = lpNS->GetCurrentTime();
}
*/

//void Neuron::CalculateFiringFreq(RealisticNeuralModule *lpNS)
//{
//	int iIndex = 0, iSpikeCount = 0;
//	double fltDiff=0, dblLastSpike=0;
//
//	//First push all of the current values down the list.
//	for(iIndex=(m_iSpikesToKeepForFreqAnal-1); iIndex>0; iIndex--)
//		m_arySpikeTimes[iIndex] = m_arySpikeTimes[iIndex-1];
//
//	//Now put this one on as the first one.
//	m_arySpikeTimes[0] = lpNS->GetCurrentTime();
//
//
//	//Lets loop through the stack of past spike times to count how many to use.
//	iIndex = 0;
//	while(iIndex<m_iSpikesToKeepForFreqAnal && m_arySpikeTimes[iIndex] >= 0)
//	{
//		fltDiff = lpNS->GetCurrentTime() - m_arySpikeTimes[iIndex];
//
//		//Only use spikes that occurred within 1 second of the current time.
//		if(fltDiff <= 1000)
//		{
//			iSpikeCount++;
//			dblLastSpike = m_arySpikeTimes[iIndex];
//		}
//		else
//			m_arySpikeTimes[iIndex] = -1;
//
//		iIndex++;
//	}
//
//	if(iSpikeCount>5 && dblLastSpike >0)
//	{
//		if(dblLastSpike == lpNS->GetCurrentTime())
//			m_fltFiringFreq = 1;
//		else
//			m_fltFiringFreq = iSpikeCount/((lpNS->GetCurrentTime()-dblLastSpike)/1000);
//	}
//	else
//		m_fltFiringFreq = 0;
//
//}

void Neuron::CalculateFiringFreq(RealisticNeuralModule *lpNS)
{
	if(m_bSpike)
	{
		double dblDiff = (lpNS->GetCurrentTime() - m_fltLastSpikeTime)/1000;
		
		if(dblDiff > 0)
			m_fltFiringFreq = 1/dblDiff;
		else
			m_fltFiringFreq = NO_FREQ_DATA;

		m_fltLastSpikeTime = lpNS->GetCurrentTime();
	}
	else
		m_fltFiringFreq = NO_FREQ_DATA;
}

///////////////////////////////////////
// ENGINE

void Neuron::PreCalc(RealisticNeuralModule *lpNS)
{
//Std_TraceMsg(0,"In Neuron::PreCalc");

	int i;
	int iSpikingChemSynCount=lpNS->GetSpikingChemSynCount();

	//Size is in Mohms, Current is in na, and volt is in mv, Time is in milliseconds.
	//so T=RC => C = T/R, where R = 1/Size. You make the size smaller it increases Rm.
	//So C = T*Size; C needs to be nF, (mS/Mohm) = nF
	m_dCm = m_dTimeConst*m_dSize;

	m_arySynG.SetSize(iSpikingChemSynCount);
    m_aryDG.SetSize(iSpikingChemSynCount);
	m_aryFacilD.SetSize(iSpikingChemSynCount);

    m_aryFacilSponSynG.SetSize(iSpikingChemSynCount);
	m_aryNextSponSynTime.SetSize(iSpikingChemSynCount);

	for (i=0; i<iSpikingChemSynCount; i++) 
	{
		if (lpNS->GetSpikingChemSynAt(i)->m_dSynAmp==0)
			continue;
		m_arySynG[i]=0.;
		if (m_aryTonicInputPeriodType[i]==0)
			m_aryNextSponSynTime[i]=0.;
		else
			m_aryNextSponSynTime[i]=(-m_aryTonicInputPeriod[i]*log(double (rand())/double(RAND_MAX))+1);
		m_aryFacilSponSynG[i]=lpNS->GetSpikingChemSynAt(i)->m_dSynAmp;

		m_aryDG[i]=exp(-m_dDT/lpNS->GetSpikingChemSynAt(i)->m_dDecay);
		m_aryFacilD[i]=exp(-m_dDT/lpNS->GetSpikingChemSynAt(i)->m_dFacilDecay);
	}

	m_dGK=0;
	m_bSpike=FALSE;
	m_dDCTH=exp(-m_dDT/m_dAccomTimeConst);
	m_dDGK=exp(-m_dDT/m_dAHPTimeConst);
	m_dRefrCountDown=0;
	
	m_dMemPot=m_dNewMemPot=m_dRestingPot;
	m_dThresh=m_dInitialThresh;
	m_fltThresholdMemory = (float) m_dThresh * 0.001;

// burster bits
// initialise to bottom of burst??
	if (m_dGMaxCa>0) 
	{
		m_dMemPot=m_dRestingPot+7.408;
		m_dM=0.0945;
		m_dH=0.0208;
	}

	m_dElecSynCur=m_dElecSynCond=0;
	m_dNonSpikingSynCur=m_dNonSpikingSynCond=0;

	//if(m_arySpikeTimes)
	//{
	//	delete m_arySpikeTimes;
	//	m_arySpikeTimes = NULL;
	//}

	//m_arySpikeTimes = new double[m_iSpikesToKeepForFreqAnal];
	//ClearSpikeTimes();

	m_dStim=0;
	m_fltAdapterI = 0;
	m_fltAdapterMemoryI = 0;
	m_fltMemPot = m_dMemPot * 0.001;
}

void Neuron::CalcUpdateFinal(RealisticNeuralModule *lpNS)
{
	int i;
	m_dMemPot=m_dNewMemPot;

	if(this->m_strName == "Flexion Command" && lpNS->GetCurrentTime() > 700)
		i=0;

	if (m_dRefrCountDown>0)
	{
		m_dRefrCountDown-=lpNS->GetTimeStep();
		m_bSpike=FALSE;
	}
	else if (lpNS->TTX() || lpNS->HH())
		m_bSpike=FALSE;
	else
		m_bSpike=(m_dMemPot>m_dThresh) ? TRUE : FALSE;
	
	if (m_bSpike)
		m_dRefrCountDown=m_dAbsoluteRefr;

	m_fltSpike = (float) m_bSpike;
	CalculateFiringFreq(lpNS);

	for (i=0; i<m_arySynG.size(); i++)
	{
		if (m_arySynG[i]>0)
			m_arySynG[i]*=m_aryDG[i];	// decrease previous syn G exponentially with time, unless never increased
	}

	m_fltMemPot = (m_bZapped?0:(m_bSpike?m_dSpikePeak:m_dMemPot)) * 0.001;
	m_fltThresholdMemory = (float) m_dThresh;

	//ASSERT(m_dStim==0.);	// ready for next iteration
}

void Neuron::PostCalc(RealisticNeuralModule *lpNS)
{
	m_arySynG.RemoveAll();	// current conductance of each synaptic type
    m_aryFacilSponSynG.RemoveAll();	// facilitated initial g increase caused by input
    m_aryDG.RemoveAll();	// exponential decline factor in syn G
	m_aryFacilD.RemoveAll();		// exponential decline factor in facilitation
	m_aryNextSponSynTime.RemoveAll();	// time to next occurrence of this syn type
}


void Neuron::CalcUpdate(RealisticNeuralModule *lpNS)
{
	double GS,GSI;		// total synaptic cond, current
	double gCa,iCa;		// Ca cond, current
	double E,DCE;			// mempot-restpot,expon decline factor in mem pot to resting pot
	int i;

	double testD;

	if (m_bZapped)		// don't bother calculating anything else if zapped
		return;

// do tonic current input
	m_dStim+= (m_dToniCurrentStimulusulus + m_fltAdapterI + (m_fltExternalI*1e9));
	m_fltAdapterI = 0;
	m_fltElecSynCurMemory = m_dElecSynCur * 1e-9;
	m_fltNonSpikingSynCurMemory = m_dNonSpikingSynCur * 1e-9;

//Go through the ion channels and calculate their currents.
//Convert time from ms to s, and membrane voltage from mv to v
	m_fltChannelMemoryI = 0;
	float fltStep = lpNS->GetTimeStep()*1e-3f;
	float fltVm = m_dMemPot*1e-3f;
	for(int iChannel=0; iChannel<m_iIonChannels; iChannel++)
		m_fltChannelMemoryI+=m_aryIonChannels[iChannel]->CalculateCurrent(fltStep, fltVm);
	m_fltChannelI=m_fltChannelMemoryI*1e9f;  //Currents are always in nA in this model. 

// adjust current injection for size of cell
	m_dStim/=m_dSize;
	m_fltChannelI/=m_dSize;

// Do spontaneous spiking synaptic input
// Each neuron can get tonic input from any synaptic type which
// acts as if a single unknown pre-synaptic neuron of that type inputs either
// regular (type==0) or random (type==1) PSPs.

	if (!lpNS->Cd())		// cadmium blocks all chem synapses & gCa
	{
		int iSpikingChemSynCount=lpNS->GetSpikingChemSynCount();
		for (i=0; i<iSpikingChemSynCount; i++)
		{
			SpikingChemicalSynapse *pSyn=lpNS->GetSpikingChemSynAt(i);
			if (pSyn->m_dSynAmp==0)
				continue;
			
			if (m_aryTonicInputPeriod[i]>0)	// if there is spontaneous input
			{
				if (pSyn->m_dRelFacil!=1)	// if facil exists, decrease its amount with time
					m_aryFacilSponSynG[i]=pSyn->m_dSynAmp+(m_aryFacilSponSynG[i]-pSyn->m_dSynAmp)*m_aryFacilD[i];

				testD=m_aryNextSponSynTime[i];

				if (m_aryNextSponSynTime[i]<=0)		// due another input
				{
	// check that synapse has not decremented below zero 
	//(you can't take away existing G)				
					if (m_aryFacilSponSynG[i]>0)
					{
						double block=1.;
	#if 0
						if (m_pNetworkData->m_bBlockedList[i])
						{
							//ASSERT(m_pNetworkData->GetPartialBlockInUse());
							double noise =double (rand())/ RAND_MAX;
							float top=m_pNetworkData->GetPartialBlockTop();
							float bottom=m_pNetworkData->GetPartialBlockBottom();
							block=bottom/100+noise*(top-bottom)/100;
							//ASSERT(block>=0. && block<=1.);
						}
	#endif
						m_arySynG[i]+=block*m_aryFacilSponSynG[i];	// add new synaptic occurrence
					}

	// facilitate next response, add residual facil to new syn amp
					m_aryFacilSponSynG[i]=(m_aryFacilSponSynG[i]-pSyn->m_dSynAmp) + (pSyn->m_dSynAmp*pSyn->m_dRelFacil);	// if <0, ignore
					
					if (m_aryTonicInputPeriodType[i]==0)
						m_aryNextSponSynTime[i]=m_aryTonicInputPeriod[i];
					else
						m_aryNextSponSynTime[i]=(-m_aryTonicInputPeriod[i]*log(double (rand())/double(RAND_MAX))+1);
				}
			m_aryNextSponSynTime[i]-=m_dDT;
			}
		}


	// adjust for voltage dependency by scaling conductance on Neuron
	// need local storage, so can do volt dep without altering basic cond
	// could do this within m_pInputCx, but since several cx may use same voltage dependent
	// synaptic type, more efficient to do outside??
		CStdArray<double> arySynG;
		arySynG.SetSize(iSpikingChemSynCount);
		for (i=0; i<iSpikingChemSynCount; i++)
		{
			SpikingChemicalSynapse *pSyn=lpNS->GetSpikingChemSynAt(i);
			arySynG[i]=m_arySynG[i];
			if (pSyn->m_bVoltDep)
				lpNS->ScaleCondForVoltDep(arySynG[i],GetMemPot(),
					pSyn->m_dSatPSPot,pSyn->m_dThreshPSPot,pSyn->m_dMaxRelCond);

		}

	// sum spiking synaptic stuff
		GS=GSI=0;
		for (i=0; i<iSpikingChemSynCount; i++)
		{
			SpikingChemicalSynapse *pSyn=lpNS->GetSpikingChemSynAt(i);
			if (pSyn->m_dSynAmp==0)
				continue;
			GS+=arySynG[i];
	// NOTE: the following looks wrong (driving force should be relative to current mempot,
	// not resting pot), but is actually right.  It's the exponential predictor maths
	// (all currents relative to rest ????)
			GSI+=(arySynG[i]*(pSyn->m_dEquil-m_dRestingPot));
		}

		m_fltSpikingSynCurMemory = GSI * 1e-9;

	// do burster
		if (m_dGMaxCa>0)
		{
			double z,tau,Minf,Hinf,dM,dH;
			gCa=m_dGMaxCa*m_dM*m_dH;
			//ASSERT(m_dM>=0 && m_dM <=1 && m_dH>=0 && m_dH<=1);
	// again, looks wrong but is right
			iCa=gCa*(m_dCaEquilPot-m_dRestingPot);
	// update M & H variables
			z=exp(-m_dSM*(m_dMemPot-m_dVM));
			Minf=1/(1+z);
			tau=Minf*sqrt(z)*m_dMTimeConst;
			dM=(Minf-m_dM)*(1-exp(-m_dDT/tau));
		
			z=exp(-m_dSH*(m_dMemPot-m_dVH))*0.5;
			Hinf=1/(1+z);
			tau=Hinf*sqrt(z)*m_dHTimeConst;
			dH=(Hinf-m_dH)*(1-exp(-m_dDT/tau));
			
			m_dM+=dM;
			m_dH+=dH;
	//TRACE("M= %lf\t\tH=%lf\tpot = %lf\n",m_M,m_H,m_MemPot);
		}
		else
			gCa=iCa=0;
	}
	else		// cadmium applied, no chem input or g/iCa
		gCa=iCa=GS=GSI=0;

	// do membrane potential
	//If the HH flag is not set then calculate E in standard way outlined by Heitler.
	//If HH flag is set then do not do integrate and fire portion, but instead just use basic equation of dividing
	//currents by capacitance.
	m_dGK=m_dGK*m_dDGK+m_bSpike*m_dAHPAmp;		// cummulative AHP cond
	m_dGTot=1+GS+m_dGK+gCa+m_dElecSynCond+m_dNonSpikingSynCond;					// total membrane cond
	DCE=exp(-m_dGTot*m_dDT/m_dTimeConst);
	if(!lpNS->HH())
	{
		E=(m_dMemPot-m_dRestingPot)*DCE+
			(m_dStim+m_fltChannelI+GSI+m_dGK*(m_dAHPEquilPot-m_dRestingPot)+iCa+m_dElecSynCur+m_dNonSpikingSynCur)
			*(1-DCE)/m_dGTot;
	}
	else
	{
		E=(m_dStim+m_fltChannelI+GSI+m_dGK*(m_dAHPEquilPot-m_dRestingPot)+iCa+m_dElecSynCur+m_dNonSpikingSynCur)/m_dCm;
	}

	m_dElecSynCur=m_dElecSynCond=0;
	m_dNonSpikingSynCur=m_dNonSpikingSynCond=0;
	m_dStim=0.;
// to add electrical synapse
// 1. sum together all elect syn conds (problem with scaling different sized cells here)
// (let neurons have a size factor (relative to 1), then scale defined cond by size 
// (increase if < 1, decrease if > 1), so get more current into small cells.
// do same thing for stimulus amp
// this means that chemical conds are per unit area, and so changes in cell size
// have no effect on voltage, but elect syn and injected current have voltage effects
// which scale with size
// 2. add total elect synaptic conductance to m_GTot 
// 3. sum all elect syn currents (G_Elec*(other_neuron_mempot-this_neuron_restpot)) 
// 4. put sum into current part of eqn E= ...

		
	// do threshold
	m_dThresh=m_dInitialThresh + (m_dThresh-m_dInitialThresh)*m_dDCTH + m_dRelativeAccom*E*(1-m_dDCTH);
	// do spike
	m_dNewMemPot=E+m_dRestingPot;

// add noise as mempot fluctuation
	if (m_dNoise!=0)
	{
		double noise =(double) rand()/ RAND_MAX;
		noise-=0.5;
		noise*=m_dNoise;
		m_dNewMemPot+=noise;
	}

}

//Node Overrides

void Neuron::AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput)
{
	if(!m_bZapped)
	{
		m_fltAdapterI += (fltInput*1e9);
		m_fltAdapterMemoryI = (m_fltAdapterI * 1e-9);
	}
}

float *Neuron::GetDataPointer(string strDataType)
{
	string strType = Std_CheckString(strDataType);

	if(strType == "MEMBRANEVOLTAGE")
		return &m_fltMemPot;

	if(strType == "ADAPTERCURRENT")
		return &m_fltAdapterMemoryI;

	if(strType == "EXTERNALCURRENT")
		return &m_fltExternalI;

	if(strType == "FIRINGFREQUENCY")
		return &m_fltFiringFreq;

	if(strType == "THRESHOLD")
		return &m_fltThresholdMemory;

	if(strType == "ELECTRICALSYNAPTICCURRENT")
		return &m_fltElecSynCurMemory;

	if(strType == "NONSPIKINGSYNAPTICCURRENT")
		return &m_fltNonSpikingSynCurMemory;

	if(strType == "SPIKINGSYNAPTICCURRENT")
		return &m_fltSpikingSynCurMemory;

	if(strType == "IONCHANNELCURRENT")
		return &m_fltChannelMemoryI;

	if(strType == "SPIKE")
		return &m_fltSpike;

	//If it was not one of those above then we have a problem.
	THROW_PARAM_ERROR(Rn_Err_lInvalidNeuronDataType, Rn_Err_strInvalidNeuronDataType, "Neuron Data Type", strDataType);

	return NULL;
}

IonChannel *Neuron::FindIonChannel(string strID, BOOL bThrowError)
{
	for(int iChannel=0; iChannel<m_iIonChannels; iChannel++)
		if(m_aryIonChannels[iChannel]->ID() == strID)
			return m_aryIonChannels[iChannel];

	if(bThrowError)
		THROW_PARAM_ERROR(Rn_Err_lIonChannelNotFound, Rn_Err_strIonChannelNotFound, "ID", strID);

	return NULL;
}	

void *Neuron::GetDataItem(string strItemType, string strID, BOOL bThrowError)
{
	string strType = Std_CheckString(strItemType);

	if(strType == "IONCHANNEL")
	{
		IonChannel *lpChannel = FindIonChannel(strID, FALSE);

		//If we cannot find an ion channel then return back a standin variable so we do not get errors
		//when we disable a channel and try and plot its values.
		if(!lpChannel)
			return &m_fltIonChannelStandin;

		return lpChannel;
	}
	else if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidItemType, Al_Err_strInvalidItemType, "Neuron Item Type", strItemType);

	return NULL;
}


//These methods are not used in this system.
void Neuron::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{}

void Neuron::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{}

void Neuron::Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{}

//Node Overrides

}				//RealisticNeuralNet

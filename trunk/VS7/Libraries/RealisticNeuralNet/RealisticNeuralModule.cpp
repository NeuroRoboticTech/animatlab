// NervousSystem.cpp: implementation of the RealisticNeuralModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IonChannel.h"
#include "Connexion.h"
#include "Neuron.h"
#include "ElectricalSynapse.h"
#include "NonSpikingChemicalSynapse.h"
#include "SpikingChemicalSynapse.h"
#include "RealisticNeuralModule.h"
#include "ClassFactory.h"
#include <time.h>

namespace RealisticNeuralNet
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RealisticNeuralModule::RealisticNeuralModule()
{
	m_lpClassFactory =  new RealisticNeuralNet::ClassFactory;

//	m_fltStepSize = 0;
	m_dTimeStep =1.;
	m_dTime=10.;

	m_bNeedInitialiseHebb=TRUE;		
	m_bRandomisedHebb=FALSE;
	m_bFreezeLearning=FALSE;	// used internally as flag, not saved

	m_bTTX = FALSE;
	m_bCd = FALSE;
	m_bHH = FALSE;
}

RealisticNeuralModule::~RealisticNeuralModule()
{

	try
	{
		m_aryNeurons.RemoveAll();
		m_arySpikingChemSyn.RemoveAll();
	}
	catch(...)
	{Std_TraceMsg(0, "Caught Error in desctructor of RealisticNeuralModule\r\n", "", -1, FALSE, TRUE);}
}


void RealisticNeuralModule::Load(CStdXml &oXml)
{
	int i;
//Std_TraceMsg(0,"Starting to load");

	m_aryNeurons.RemoveAll();

	m_fltTimeStep = oXml.GetChildFloat("TimeStep");
	m_dTimeStep = m_fltTimeStep * 1000;
	Std_InValidRange((float) 1e-10, (float) 1, m_fltTimeStep, TRUE, "TimeStep");
	m_dTime = oXml.GetChildDouble("Time",  200);

	m_bTTX = oXml.GetChildBool("TTX", FALSE);
	m_bCd = oXml.GetChildBool("Cd", FALSE);
	m_bHH = oXml.GetChildBool("HH", FALSE);

	m_bRetainHebbMemory = oXml.GetChildBool("RetainHebbMemory", 0);
	m_bUseCriticalPeriod = oXml.GetChildBool("UseCriticalPeriod");
	m_dStartCriticalPeriod = oXml.GetChildDouble("StartCriticalPeriod");
	m_dEndCriticalPeriod = oXml.GetChildDouble("EndCriticalPeriod");
	m_bFreezeHebb = oXml.GetChildBool("FreezeHebb", 0);

	Neuron::m_dSpikePeak= oXml.GetChildDouble("SpikePeak");
	Neuron::m_dSpikeStrength=oXml.GetChildDouble("SpikeStrength");
	Neuron::m_dAHPEquilPot=oXml.GetChildDouble("AHPEquilPot");
	Neuron::m_dCaEquilPot=oXml.GetChildDouble("CaEquilPot");
	Neuron::m_dAbsoluteRefr=oXml.GetChildDouble("AbsoluteRefr");

	oXml.IntoChildElement("Synapses");

	oXml.IntoChildElement("SpikingSynapses");
	int iTotalSpikingSynapses = oXml.NumberOfChildren();
	for(i=0; i<iTotalSpikingSynapses; i++)
	{
		oXml.FindChildByIndex(i);
		LoadSpikingChemSyn(oXml);
	}
	oXml.OutOfElem(); //OutOf SpikingSynapses Element
	
	oXml.IntoChildElement("NonSpikingSynapses");
	int iTotalNonSpikingSynapses = oXml.NumberOfChildren();
	for(i=0; i<iTotalNonSpikingSynapses; i++)
	{
		oXml.FindChildByIndex(i);
		LoadNonSpikingChemSyn(oXml);
	}
	oXml.OutOfElem(); //OutOf NonSpikingSynapses Element
	
	oXml.IntoChildElement("ElectricalSynapses");
	int iTotalElecSynapses = oXml.NumberOfChildren();
	for(i=0; i<iTotalElecSynapses; i++)
	{
		oXml.FindChildByIndex(i);
		LoadElecSyn(oXml);
	}
	oXml.OutOfElem(); //OutOf ElectricalSynapses Element

	oXml.OutOfElem(); //OutOf Synapses Element


	//*** Begin Loading Neurons. *****
	oXml.IntoChildElement("Neurons");
	int iTotalNeurons = oXml.NumberOfChildren();
	for(i=0; i<iTotalNeurons; i++)
	{
		oXml.FindChildByIndex(i);
		LoadNeuron(oXml);
	}
	oXml.OutOfElem(); //OutOf Neurons Element


	/*
	if(oXml.FindChildElement("Stimuli", false))
	{
		oXml.IntoChildElement("Stimuli");
		int iTotalStimuli = oXml.NumberOfChildren();
		for(i=0; i<iTotalStimuli; i++)
		{
			oXml.FindChildByIndex(i);
			LoadStim(oXml);
		}
		oXml.OutOfElem(); //OutOf Stimuli Element
	}
	*/

	oXml.IntoChildElement("Connexions");
	int iTotalConnexions = oXml.NumberOfChildren();
	for(i=0; i<iTotalConnexions; i++)
	{
		oXml.FindChildByIndex(i);
		//Std_TraceMsg(0, STR(i));
		LoadConnexion(oXml);
	}
	oXml.OutOfElem(); //OutOf Stimuli Element


//Std_TraceMsg(0,"neuron count = "+STR(GetNeuronCount()));
//Std_TraceMsg(0,"stim count = "+STR(GetStimCount()));
//Std_TraceMsg(0,"connexion count = "+STR(GetConnexionCount()));
}


Neuron *RealisticNeuralModule::LoadNeuron(CStdXml &oXml)
{
	Neuron *lpNeuron=NULL;
	string strType;

	try
	{

		lpNeuron = new Neuron;
		lpNeuron->Load(oXml, this);

		m_aryNeurons.Add(lpNeuron);

		return lpNeuron;
	}
	catch(CStdErrorInfo oError)
	{
		if(lpNeuron) delete lpNeuron;
		RELAY_ERROR(oError);
		return NULL;
	}
	catch(...)
	{
		if(lpNeuron) delete lpNeuron;
		THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
		return NULL;
	}
}

SpikingChemicalSynapse *RealisticNeuralModule::LoadSpikingChemSyn(CStdXml &oXml)
{
	SpikingChemicalSynapse *pSpikingChemSyn=NULL;
	string strType;
//

	try
	{

		pSpikingChemSyn = new SpikingChemicalSynapse;
		pSpikingChemSyn->Load(oXml);

		m_arySpikingChemSyn.Add(pSpikingChemSyn);

		return pSpikingChemSyn;
	}
	catch(CStdErrorInfo oError)
	{
		if(pSpikingChemSyn) delete pSpikingChemSyn;
		RELAY_ERROR(oError);
		return NULL;
	}
	catch(...)
	{
		if(pSpikingChemSyn) delete pSpikingChemSyn;
		THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
		return NULL;
	}
}

NonSpikingChemicalSynapse *RealisticNeuralModule::LoadNonSpikingChemSyn(CStdXml &oXml)
{
	NonSpikingChemicalSynapse *pNonSpikingChemSyn=NULL;
	string strType;
//

	try
	{

		pNonSpikingChemSyn = new NonSpikingChemicalSynapse;
		pNonSpikingChemSyn->Load(oXml);

		m_aryNonSpikingChemSyn.Add(pNonSpikingChemSyn);

		return pNonSpikingChemSyn;
	}
	catch(CStdErrorInfo oError)
	{
		if(pNonSpikingChemSyn) delete pNonSpikingChemSyn;
		RELAY_ERROR(oError);
		return NULL;
	}
	catch(...)
	{
		if(pNonSpikingChemSyn) delete pNonSpikingChemSyn;
		THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
		return NULL;
	}
}

ElectricalSynapse *RealisticNeuralModule::LoadElecSyn(CStdXml &oXml)
{
	ElectricalSynapse  *pElecSyn=NULL;
	string strType;
//

	try
	{

		pElecSyn = new ElectricalSynapse;
		pElecSyn->Load(oXml);

		m_aryElecSyn.Add(pElecSyn);

		return pElecSyn;
	}
	catch(CStdErrorInfo oError)
	{
		if(pElecSyn) delete pElecSyn;
		RELAY_ERROR(oError);
		return NULL;
	}
	catch(...)
	{
		if(pElecSyn) delete pElecSyn;
		THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
		return NULL;
	}
}

/*
CurrentStimulus *RealisticNeuralModule::LoadStim(CStdXml &oXml)
{
	CurrentStimulus *pStim=NULL;
	string strType;

	try
	{

		pStim = new CurrentStimulus;
		pStim->Load(oXml);

		m_aryStim.Add(pStim);

		return pStim;
	}
	catch(CStdErrorInfo oError)
	{
		if(pStim) delete pStim;
		RELAY_ERROR(oError);
		return NULL;
	}
	catch(...)
	{
		if(pStim) delete pStim;
		THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
		return NULL;
	}
}
*/

Connexion *RealisticNeuralModule::LoadConnexion(CStdXml &oXml)
{
	Connexion *pConnexion=NULL;
	string strType;

	try
	{

		pConnexion = new Connexion;
		pConnexion->Load(oXml);

		m_aryConnexion.Add(pConnexion);

		return pConnexion;
	}
	catch(CStdErrorInfo oError)
	{
		if(pConnexion) delete pConnexion;
		RELAY_ERROR(oError);
		return NULL;
	}
	catch(...)
	{
		if(pConnexion) delete pConnexion;
		THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
		return NULL;
	}
}

////////////////////////////
// THE ENGINE

double RealisticNeuralModule::GetMemPot(int neuronID)
{
	return GetNeuronAt(neuronID)->GetMemPot();
}

double RealisticNeuralModule::GetScaleElecCond(double minG,double maxG,double jV, double ThreshV,double SaturateV)
{
	if (maxG==minG)			// NON rectifying
		return minG;
	if (jV<=ThreshV)
		return minG;
	if (jV>SaturateV)
		return maxG;

	return minG+(maxG-minG)*(jV-ThreshV)/(SaturateV-ThreshV);
}

void RealisticNeuralModule::ScaleCondForVoltDep(double& G,double postV,double maxV,double minV,double scl)
{
	//dwc There was an inconsistency here. If the post-Vm was less than maxV then it
	//could range from 1 to (1+scl), but as soon as the Vm goes to maxV or above it
	//suddenly jumps to scaling by scl instead of (1+scl). I changed it so that it scales correclty.
	if (postV >= maxV)
		G*=(1+scl);   
	else if (postV > minV && postV < maxV)
	{
#ifdef _DEBUG
double vScl=(postV-minV)/(maxV-minV);
//ASSERT(vScl>0 && vScl<1);
#endif
			G*=(1+(scl*(postV-minV)/(maxV-minV)));
	}
}

// pass in reference to maximum conductance
void RealisticNeuralModule::ScaleCondForNonSpiking(double& G,double PreV,double ThreshV,double SaturateV)
{
	if (PreV<=ThreshV)
		G=0.0;
	else if (PreV<SaturateV)
		G=G*(PreV-ThreshV)/(SaturateV-ThreshV);
}

void RealisticNeuralModule::PostCalc()
{
	int i;
	Connexion *pCx;
	
	for (i=0; i<GetNeuronCount(); i++)
	{
		GetNeuronAt(i)->PostCalc(this);
	}
	for (i=0; i<GetConnexionCount(); i++)
	{
		pCx=GetConnexionAt(i);
		pCx->m_TransitCx.Release();
		pCx->m_HebbList.Release();
	}
}
void RealisticNeuralModule::PreCalc()
{
	int i;
	Connexion *pCx;

	m_dCurrentTime=0;
	Neuron::m_dDT=m_dTimeStep;
	for (i=0; i<GetNeuronCount(); i++)
	{
		GetNeuronAt(i)->PreCalc(this);
	}


/*
FOLLOWING COMMENTS ARE FROM NEUROSIM - I don't really understand them any more!!!!
*/

/*
 THE LOGIC
 Normally need to set initial conductance of Hebb synapses to unaugmented 
 state at start of run.
 If RetainMemory, still need to set as above, until run once. 
 Problem: if do run, then change  synapses, then set RetainMem - 
 retains conditions of previous synapses.
 So set NeedInitHebb flag when change synapses or circuit (and in NewDoc).
 If load file, set NeedInit UNLESS memory was retained in loaded file.
 */

// could skip all pCx initialisation if retain memory, so long as done it at least once.
// since can't change circuit while retain memory, so no need to worry about
// params changing.
// set DoneInitHebb FALSE when setting RetainMemory
// BUT since may be loading from file, and don't want to save all m_FacilD etc,
// just skip initialising the bits that hold the memory trace

	for (i=0; i<GetConnexionCount(); i++)
	{
		pCx=GetConnexionAt(i);
	
		if (pCx->m_iType==0)	// spiking synapse
		{
			pCx->m_dFacilD=exp(-m_dTimeStep/m_arySpikingChemSyn[pCx->m_iID]->m_dFacilDecay);
			pCx->m_dRelFacil=m_arySpikingChemSyn[pCx->m_iID]->m_dRelFacil;
						
			pCx->m_bVoltDep=m_arySpikingChemSyn[pCx->m_iID]->m_bVoltDep;
			pCx->m_bHebbian=m_arySpikingChemSyn[pCx->m_iID]->m_bHebbian;
// we need to initialise conductances unless already done;
// can be done for Hebbian synapses by Randomise, or by loading a file with saved Hebb memory
// or previously, with MaintainMemory set
// so skip if Hebbian and Randomise,
// or if Hebbian and RetainMemory and DoneInit
			//DWC Always use the synaptic conductance loaded into the connection and not the one from the synapse type.
			//if (pCx->m_bHebbian && (m_bRandomisedHebb || ((!m_bNeedInitialiseHebb) && m_bRetainHebbMemory)))
			//	;
			//else
			//	pCx->m_dGFacilCx=pCx->m_dG=m_arySpikingChemSyn[pCx->m_iID]->m_dSynAmp;
			pCx->m_dHebbIncrement=m_arySpikingChemSyn[pCx->m_iID]->m_dLearningInc;
			pCx->m_dHebbTimeWindow=m_arySpikingChemSyn[pCx->m_iID]->m_dLearningTime;
			pCx->m_dMaxGHebb=m_arySpikingChemSyn[pCx->m_iID]->m_dMaxAugCond;
			pCx->m_bAllowForgetting=m_arySpikingChemSyn[pCx->m_iID]->m_bAllowForget;
			pCx->m_dForgettingWindow=m_arySpikingChemSyn[pCx->m_iID]->m_dForgetTime;
			pCx->m_dConsolidation=m_arySpikingChemSyn[pCx->m_iID]->m_dConsolidation;
						
			//pCx->m_dBaseG=m_arySpikingChemSyn[pCx->m_iID]->m_dSynAmp;
			pCx->m_dPreviousSpikeLatency=-1;
						
			pCx->m_dMaxGVoltDepRel=m_arySpikingChemSyn[pCx->m_iID]->m_dMaxRelCond;
		
			if (pCx->m_bHebbian)
				pCx->m_dTimeSincePrevHebbEvent=0;
		}
    }
	m_bNeedInitialiseHebb=FALSE;
	m_bRandomisedHebb=FALSE;     // randomising done in menu handler
	
// do any zero-latency stimulus
	//for (i=0; i<GetStimCount(); i++)
	//{
	//	GetStimAt(i)->Reset();
	//}
}

void RealisticNeuralModule::CalcUpdate()
{
	int i;
	Neuron *pSourceN;
	Neuron *pTargetN;
	Connexion *pCx;

// could use 		
//	Sleep(m_SlowDownFactor);
// but has granularity of 1 msec, which is a bit thick

	m_dCurrentTime+=m_dTimeStep;


	//CurrentStimulus *pStim;
	//for (i=0; i<GetStimCount(); i++)
	//{
	//	pStim=GetStimAt(i);
	//	GetNeuronAt(pStim->GetNeuronID())->IncrementStim(pStim->GetStim(m_dCurrentTime));
	//}

// check if in Critical Period( if used)
	m_bFreezeLearning=m_bFreezeHebb;
	if (m_bFreezeLearning==FALSE && m_bUseCriticalPeriod)
	{
		if (m_dCurrentTime<m_dStartCriticalPeriod || m_dCurrentTime>m_dEndCriticalPeriod)
			m_bFreezeLearning=TRUE;
	}

	
// We calculate the conductance of each spiking synaptic type, and pass it to
// neuron. We cannot add to general conductance (as we can with non-spiking or 
// chemical, because of time dependence (time constant, facilitation etc) 
// which means that each type has to be treated separately

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


// go through connexion list for electrical and non-spiking chemical synapses
	double sourceV,targetV,junctV;		// source,target mempot & junctional pot (pre-post)
	double junctG,G;
	for (i=0; i<GetConnexionCount(); i++)
	{
		pCx=GetConnexionAt(i);

		pSourceN=GetNeuronAt(pCx->m_iSource);
		pTargetN=GetNeuronAt(pCx->m_iTarget);
		if (pSourceN->GetZapped() || pTargetN->GetZapped())
			continue;

		sourceV=pSourceN->GetMemPot();
		if (pCx->m_iType==0)
		{
			if (!m_bCd)			// Cadmium blocks all chemical synapses
			{
				double *pTimeToNext;
				pCx->DecrementLatencies(m_dTimeStep,m_bFreezeLearning);
				pCx->DecrementFacilitation();
				if ((pTimeToNext=pCx->GetTimeToNextSpikePtr())!=NULL) // got spike in transit
				{
					if ((*pTimeToNext)<0) 	// spike has arrived
						pTargetN->m_arySynG[pCx->m_iID]+=(pCx->m_dPartialBlock*pCx->ProcessOutput(m_bFreezeLearning));
				}
			}
		}
		else if (pCx->m_iType==1)						// non-spiking chemical
		{
			if (!m_bCd)			// Cadmium blocks all chemical synapses
			{
				G=pCx->m_dPartialBlock*m_aryNonSpikingChemSyn[pCx->m_iID]->m_dSynAmp;
	//ASSERT(G>=0);
				ScaleCondForNonSpiking(G,sourceV,
					m_aryNonSpikingChemSyn[pCx->m_iID]->m_dThreshV,
					m_aryNonSpikingChemSyn[pCx->m_iID]->m_dSaturateV);

				if (G==0.0)
					continue;

				pTargetN->IncNonSpikingSynCond(G);
				pTargetN->IncNonSpikingSynCurr(G * (m_aryNonSpikingChemSyn[pCx->m_iID]->m_dEquil-pTargetN->m_dRestingPot));
			}
		}
				
		else // electrical
		{
//ASSERT(pCx->m_iType==2);
			double sG,tG;

// if it has a delay, then it just injects into the post-synaptic neuron, no feedback onto pre-synaptic neurons
			if (pCx->m_dDelay>0.)
			{
				double *pTimeToNext;
				pCx->DecrementLatencies(m_dTimeStep);
				if ((pTimeToNext=pCx->GetTimeToNextSpikePtr())!=NULL) // got spike in transit
				{
					if ((*pTimeToNext)<0) 	// spike has arrived
					{
						pCx->m_TransitCx.Del();				// remove spike from list
						targetV=pTargetN->GetMemPot();
						junctV=sourceV-targetV;
						junctG=pCx->m_dPartialBlock*GetScaleElecCond(m_aryElecSyn[pCx->m_iID]->m_dLowCoup,
							m_aryElecSyn[pCx->m_iID]->m_dHiCoup,
							junctV,m_aryElecSyn[pCx->m_iID]->m_dTurnOnV,
							m_aryElecSyn[pCx->m_iID]->m_dSaturateV);

						tG=junctG/pTargetN->m_dSize;
						pTargetN->InElectricalSynapseCond(tG);
						pTargetN->InElectricalSynapseCurr(tG*(pSourceN->GetRestingPot()+(Neuron::m_dSpikePeak-pSourceN->GetRestingPot())*Neuron::m_dSpikeStrength-pTargetN->GetRestingPot()));
					}
				}
			}
			else
			{
				
				targetV=pTargetN->GetMemPot();
				junctV=sourceV-targetV;
				junctG=pCx->m_dPartialBlock*GetScaleElecCond(m_aryElecSyn[pCx->m_iID]->m_dLowCoup,
					m_aryElecSyn[pCx->m_iID]->m_dHiCoup,
					junctV,m_aryElecSyn[pCx->m_iID]->m_dTurnOnV,
					m_aryElecSyn[pCx->m_iID]->m_dSaturateV);
	// adjust for size of cell
	// for small cells, the electrical conductance is relatively bigger
				sG=junctG/pSourceN->m_dSize;
				tG=junctG/pTargetN->m_dSize;
				pSourceN->InElectricalSynapseCond(sG);
				pTargetN->InElectricalSynapseCond(tG);


				if (pTargetN->GetSpike())
					pSourceN->InElectricalSynapseCurr(sG*(pTargetN->GetRestingPot()+(Neuron::m_dSpikePeak-pTargetN->GetRestingPot())*Neuron::m_dSpikeStrength-pSourceN->m_dRestingPot));
				else
					pSourceN->InElectricalSynapseCurr(sG*(pTargetN->GetMemPot()-pSourceN->m_dRestingPot));

				
				if (pSourceN->GetSpike())
					pTargetN->InElectricalSynapseCurr(tG*(pSourceN->GetRestingPot()+(Neuron::m_dSpikePeak-pSourceN->GetRestingPot())*Neuron::m_dSpikeStrength-pTargetN->m_dRestingPot));
				else
					pTargetN->InElectricalSynapseCurr(tG*(pSourceN->GetMemPot()-pTargetN->m_dRestingPot));
			}
		}
	}

// Neuron::Update does stimulus, endogenous, + spontaneous spiking chem input. 
// it stores m_NewMemPot, so can update when all neurons have done their updates using memPot from previous iteration 
	for (i=0; i<GetNeuronCount(); i++)
	{
		GetNeuronAt(i)->CalcUpdate(this);
	}

// now switch newMemPot for memPot, decide if spiking etc
// Draws neuron if colour-from-potential
	for (i=0; i<GetNeuronCount(); i++)
	{
		GetNeuronAt(i)->CalcUpdateFinal(this);
	}

	
// get all spiking neurons after doing all updates 
// and save old membrane potentials for non-spiking and electrical synapses
// ZAPPED neurons never spike
	for (i=0; i<GetConnexionCount(); i++)
	{
		pCx=GetConnexionAt(i);
		pSourceN=GetNeuronAt(pCx->m_iSource);
		if (pSourceN->GetSpike())
			pCx->AppendTransitSpike();
		pTargetN=GetNeuronAt(pCx->m_iTarget);
		if (pTargetN->GetSpike() && !m_bFreezeLearning)
			pCx->IncrementHebbian();
	}


#if 0
	for (i=0; i<NETWORK_MAXTRACE; i++)
	{
		if (m_bShowAxis[i])
		{
			pN=GetNeuronAt(m_MapNeuronToAxis[i]);
			m_Dat[i*2]=pN->GetMemPot();
			m_Dat[i*2+1]=pN->GetThresh();
		}
	}
#endif
}

//NeuralModule overrides

Node *RealisticNeuralModule::FindNode(long lNodeID) 
{
	int iSize = m_aryNeurons.GetSize();

	Std_InValidRange((long) 0, (long) (iSize-1), lNodeID, TRUE, "NodeID");

	return m_aryNeurons[lNodeID];
};

void RealisticNeuralModule::Kill(Simulator *lpSim, Organism *lpOrganism, BOOL bState)
{
	int iSize = m_aryNeurons.GetSize(), iNeuron;
	for(iNeuron=0; iNeuron; iNeuron++)
		m_aryNeurons[iNeuron]->Kill(lpSim, lpOrganism, bState);
}

void RealisticNeuralModule::Reset(Simulator *lpSim, Organism *lpOrganism)
{
	int iSize = m_aryNeurons.GetSize(), iNeuron;
	for(iNeuron=0; iNeuron; iNeuron++)
		m_aryNeurons[iNeuron]->Reset(lpSim, lpOrganism);
}

void RealisticNeuralModule::Initialize(Simulator *lpSim, Structure *lpStructure)
{
	srand(lpSim->ManualRandomSeed());

	NeuralModule::Initialize(lpSim, lpStructure);
	PreCalc();
}

void RealisticNeuralModule::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	NeuralModule::StepSimulation(lpSim, lpStructure);
	CalcUpdate();
}

void RealisticNeuralModule::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	CStdXml oNetXml;

	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	//if(Std_IsBlank(m_strProjectPath)) 
	//	THROW_ERROR(Al_Err_lProjectPathBlank, Al_Err_strProjectPathBlank);

	m_lpSim = lpSim;
	m_lpStructure = lpStructure;

	m_arySourceAdapters.RemoveAll();
	m_aryTargetAdapters.RemoveAll();

	oXml.IntoElem();  //Into NeuralModule Element

	m_strNeuralNetworkFile = oXml.GetChildString("NeuralNetFile", "");

	TRACE_DEBUG("Loading nervous system config file.\r\nProjectPath: " + m_strProjectPath + "\r\nFile: " + m_strNeuralNetworkFile);

	if(!Std_IsBlank(m_strNeuralNetworkFile)) 
	{
		oNetXml.Load(AnimatLibrary::GetFilePath(m_strProjectPath, m_strNeuralNetworkFile));

		oNetXml.FindElement("NeuralModule");
		oNetXml.FindChildElement("UseCriticalPeriod");

		Load(oNetXml);
	}
	else
		Load(oXml);

	oXml.OutOfElem(); //OutOf NeuralModule Element

	TRACE_DEBUG("Finished loading nervous system config file.");
}

void RealisticNeuralModule::Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml) 
{};


//NeuralModule overrides


}				//RealisticNeuralNet


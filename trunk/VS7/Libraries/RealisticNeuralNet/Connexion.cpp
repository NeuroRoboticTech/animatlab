// Connexion.cpp: implementation of the Connexion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Connexion.h"

namespace RealisticNeuralNet
{
	namespace Synapses
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Connexion::Connexion()
{
	m_dPartialBlock=1.0;
}
Connexion::Connexion(int source, int target, int type, int ID, double delay,float topBlock,float botBlock)
{
	m_iSource=source;
	m_iTarget=target;
	m_iType=type;
	m_iID=ID;
	m_dDelay=delay;

	m_dPartialBlock=1.0;
//	GeneratePartialBlock(topBlock,botBlock);
}

Connexion::~Connexion()
{

}

void Connexion::Load(CStdXml &oXml)
{
	oXml.IntoElem();  //Into SpikingChemSyn Element
//	m_dLowCoup= oXml.GetChildDouble("dLowCoup");

	m_iSource=oXml.GetChildInt("Source");
	m_iTarget=oXml.GetChildInt("Target");
	m_iType=oXml.GetChildInt("Type");
	m_iID=oXml.GetChildInt("ID");
	m_dDelay=oXml.GetChildDouble("Delay");
	m_dGFacilCx=oXml.GetChildDouble("GFacilCx");
	m_dG=oXml.GetChildDouble("G");
	m_dBaseG = m_dG;
	m_dPartialBlockHold=oXml.GetChildDouble("PartialBlockHold", 0);

	oXml.OutOfElem(); //OutOf Neuron Element
}

////////////////////////////////////
// WORKING

void Connexion::DecrementLatencies(double dt,BOOL FreezeLearning)
{
	double *pTimeToNext;
	while (1) 
	{			// decrease latency for all spikes in transit
		pTimeToNext=m_TransitCx.Iterate();
		if (pTimeToNext==NULL)		// end of spike list for connexion
			break;
		(*pTimeToNext)-=dt;
	}
	
	if (m_bHebbian && !FreezeLearning)
	{
		BOOL bTimedOut=FALSE;
		while (1) 
		{			// decrease time marker latency for all previous Hebb events
			pTimeToNext=m_HebbList.Iterate();
			if (pTimeToNext==NULL)		// end of Hebb list 
				break;
//TRACE("decrementing Hebbian time window\n");
			(*pTimeToNext)-=dt;
			if ((*pTimeToNext)<=0)		// can't delete now because resets iterator
				bTimedOut=TRUE;
		}
		if (bTimedOut==TRUE)
		{
//TRACE("a Hebb input timed out; deleting from list\n");
			m_HebbList.Del();
		}
		if (m_dTimeSincePrevHebbEvent<m_dForgettingWindow)		// prevent overflow
			m_dTimeSincePrevHebbEvent+=dt;
	}
}

void Connexion::IncrementHebbian()
{
	if (m_bHebbian)
	{
		double newG, *pTimeLeftInHebbWindow;
		while (1) 	// iterate through Hebb list
		{			
			pTimeLeftInHebbWindow=m_HebbList.Iterate();
			if (pTimeLeftInHebbWindow==NULL)		// end of Hebb list 
				break;

//TRACE("Incrementing Hebbian synapse: m_G was %lf, facil G was %lf\n",m_G,m_GFacilCx);
			newG=m_dG+(m_dMaxGHebb-m_dG)*m_dHebbIncrement*((*pTimeLeftInHebbWindow)/m_dHebbTimeWindow);
			//ASSERT(newG<=m_dMaxGHebb);
// increment current facilitation state by same percentage as base conductance
			m_dGFacilCx=m_dGFacilCx*newG/m_dG;
			m_dG=newG;
//TRACE("time left in Hebb window = %lf, increment factor = %lf\n",*pTimeLeftInHebbWindow,m_HebbIncrement*((*pTimeLeftInHebbWindow)/m_HebbTimeWindow));
//TRACE("and now m_G is %lf, facil G is %lf\n",m_G, m_GFacilCx);
			m_dTimeSincePrevHebbEvent=0;
		}
	}
}

double Connexion::ProcessOutput(BOOL bFreezeHebb)
{
// if allowing forgetting, decrement Hebb augmentation by fraction of ForgettingWindow
// since last Hebb augmentation event
	if (m_bHebbian && m_bAllowForgetting && !bFreezeHebb)
	{
		double newG;
		double ForgettingWindow=m_dForgettingWindow;
		if (m_dConsolidation!=1)
			ForgettingWindow=ForgettingWindow*(1+((m_dConsolidation-1)*(m_dG-m_dBaseG)/(m_dMaxGHebb-m_dBaseG)));
		if (m_dTimeSincePrevHebbEvent>=ForgettingWindow)
			newG=m_dBaseG;
		else
			newG=m_dG-(m_dG-m_dBaseG)*m_dTimeSincePrevHebbEvent/ForgettingWindow;
//ASSERT(m_dG>=m_dBaseG);
// decrease current facilitation state by same percentage as base conductance
		m_dGFacilCx=m_dGFacilCx*newG/m_dG;
		m_dG=newG;
//TRACE("forgetting window = %lf\n",ForgettingWindow);
	}

	double G;
	G=max(0,m_dGFacilCx);	// get conductance, if not facil below 0
	m_dGFacilCx=(m_dGFacilCx-m_dG)+(m_dG*m_dRelFacil);	// facilitate next response
	m_TransitCx.Del();				// remove spike from list

// if Hebbian, store set time window
	if (m_bHebbian)
	{
		m_HebbList.AddTail(m_dHebbTimeWindow);
//		m_PreviousSpikeLatency=m_HebbTimeWindow;
//TRACE("appending Hebbian time window to Hebb list\n");

	}
	return G;
}

double Connexion::GetProspectiveCond(BOOL bFreezeHebb)
{
	double G=m_dGFacilCx;
// if allowing forgetting, decrement Hebb augmentation by fraction of ForgettingWindow
// since last Hebb augmentation event
	if (m_bHebbian && m_bAllowForgetting && !bFreezeHebb)
	{
		double newG;
		double ForgettingWindow=m_dForgettingWindow;
		if (m_dConsolidation!=1)
			ForgettingWindow=ForgettingWindow*(1+((m_dConsolidation-1)*(m_dG-m_dBaseG)/(m_dMaxGHebb-m_dBaseG)));
		if (m_dTimeSincePrevHebbEvent>=ForgettingWindow)
			newG=m_dBaseG;
		else
			newG=m_dG-(m_dG-m_dBaseG)*m_dTimeSincePrevHebbEvent/ForgettingWindow;
//ASSERT(m_dG>=m_dBaseG);
// decrease current facilitation state by same percentage as base conductance
		G=m_dGFacilCx*newG/m_dG;
	}
//TRACE("GetProspectiveCond = %lf\n",G);
	return(max(0,G));	// get conductance, if not facil below 0
}

	}			//Synapses
}				//RealisticNeuralNet


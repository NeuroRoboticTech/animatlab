// SpikingChemSyn.cpp: implementation of the SpikingChemicalSynapse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpikingChemicalSynapse.h"

namespace RealisticNeuralNet
{
	namespace Synapses
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SpikingChemicalSynapse::SpikingChemicalSynapse()
{

}

SpikingChemicalSynapse::~SpikingChemicalSynapse()
{

}

void SpikingChemicalSynapse::Load(CStdXml &oXml)
{
	oXml.IntoElem();  //Into SpikingChemSyn Element

	m_strName = oXml.GetChildString("Name", "");
	m_dEquil = oXml.GetChildDouble("Equil");
	m_dSynAmp = oXml.GetChildDouble("SynAmp");
	m_dDecay = oXml.GetChildDouble("Decay");
	m_dRelFacil = oXml.GetChildDouble("RelFacil");
	m_dFacilDecay = oXml.GetChildDouble("FacilDecay");

	m_bVoltDep = oXml.GetChildBool("VoltDep");
	m_dMaxRelCond = oXml.GetChildDouble("MaxRelCond");
	m_dSatPSPot = oXml.GetChildDouble("SatPSPot");
	m_dThreshPSPot = oXml.GetChildDouble("ThreshPSPot");

	m_bHebbian = oXml.GetChildBool("Hebbian");
	m_dMaxAugCond = oXml.GetChildDouble("MaxAugCond");
	m_dLearningInc = oXml.GetChildDouble("LearningInc");
	m_dLearningTime = oXml.GetChildDouble("LearningTime");
	m_bAllowForget = oXml.GetChildBool("AllowForget");
	m_dForgetTime = oXml.GetChildDouble("ForgetTime");
	m_dConsolidation = oXml.GetChildDouble("Consolidation");

	oXml.OutOfElem(); //OutOf Neuron Element
}

	}			//Synapses
}				//RealisticNeuralNet

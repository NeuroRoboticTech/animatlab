// NonSpikingChemSyn.cpp: implementation of the NonSpikingChemicalSynapse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NonSpikingChemicalSynapse.h"

namespace RealisticNeuralNet
{
	namespace Synapses
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NonSpikingChemicalSynapse::NonSpikingChemicalSynapse()
{

}

NonSpikingChemicalSynapse::~NonSpikingChemicalSynapse()
{

}

void NonSpikingChemicalSynapse::Load(CStdXml &oXml)
{
	oXml.IntoElem();  //Into SpikingChemSyn Element

	m_strName = oXml.GetChildString("Name", "");
	m_dEquil = oXml.GetChildDouble("Equil");
	m_dSynAmp = oXml.GetChildDouble("SynAmp");
	m_dThreshV= oXml.GetChildDouble("ThreshV");
	m_dSaturateV= oXml.GetChildDouble("SaturateV");

	oXml.OutOfElem(); //OutOf Neuron Element
}

	}			//Synapses
}				//RealisticNeuralNet
// ElecSyn.cpp: implementation of the ElectricalSynapse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ElectricalSynapse.h"

namespace RealisticNeuralNet
{
	namespace Synapses
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ElectricalSynapse::ElectricalSynapse()
{

}

ElectricalSynapse::~ElectricalSynapse()
{

}

void ElectricalSynapse::Load(CStdXml &oXml)
{
	oXml.IntoElem();  //Into SpikingChemSyn Element

	m_strName = oXml.GetChildString("Name", "");
	m_dLowCoup= oXml.GetChildDouble("LowCoup");
	m_dHiCoup= oXml.GetChildDouble("HiCoup");
	m_dTurnOnV= oXml.GetChildDouble("TurnOnV");
	m_dSaturateV= oXml.GetChildDouble("SaturateV");

	oXml.OutOfElem(); //OutOf Neuron Element
}

	}			//Synapses
}				//RealisticNeuralNet
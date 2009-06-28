// GatedSynapse.cpp: implementation of the GatedSynapse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Synapse.h"
#include "GatedSynapse.h"
#include "Neuron.h" 
#include "FastNeuralModule.h"

namespace FastNeuralNet
{
	namespace Synapses
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GatedSynapse::GatedSynapse()
{
	m_iInitialGateValue=0;
}

GatedSynapse::~GatedSynapse()
{

}

float GatedSynapse::CalculateModulation(FastNeuralModule *lpModule)
{
	Neuron *lpNeuron=NULL;
	m_fltModulation=0;

	if(m_bEnabled)
	{
		lpNeuron = lpModule->GetNeuron(m_iFromX, m_iFromY, m_iFromZ);
		if(lpNeuron) m_fltModulation = m_iInitialGateValue + (lpNeuron->FiringFreq(lpModule) * m_fltWeight);
	}
	else
		m_fltModulation = 1;

	return m_fltModulation;
}

void GatedSynapse::Load(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml)
{

	oXml.IntoElem();  //Into GatedSynapse Element

	CStdIPoint oFrom;
	Std_LoadPoint(oXml, "From", oFrom);

	m_bEnabled = oXml.GetChildBool("Enabled", TRUE);
	m_iFromX = (unsigned char) oFrom.x;
	m_iFromY = (unsigned char) oFrom.y;
	m_iFromZ = (unsigned char) oFrom.z;
	m_fltWeight = oXml.GetChildFloat("Weight");
	m_iInitialGateValue = (unsigned char) oXml.GetChildInt("InitialGateValue");


	oXml.OutOfElem(); //OutOf GatedSynapse Element
}



void GatedSynapse::Save(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml)
{

	oXml.AddChildElement("CompoundSynapse");
	oXml.IntoElem();  //Into CompoundSynapse Element


	oXml.AddChildElement("SynapseType", GATED_SYNAPSE);
	oXml.AddChildElement("FromX", m_iFromX);
	oXml.AddChildElement("FromY", m_iFromY);
	oXml.AddChildElement("FromZ", m_iFromZ);
	oXml.AddChildElement("Weight", m_fltWeight);
	oXml.AddChildElement("InitialGateValue", m_iInitialGateValue);


	oXml.OutOfElem();  //OutOf CompoundSynapse Element
}

	}			//Synapses
}				//FastNeuralNet







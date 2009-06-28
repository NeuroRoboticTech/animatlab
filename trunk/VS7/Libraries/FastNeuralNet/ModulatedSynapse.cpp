// ModulatedSynapse.cpp: implementation of the ModulatedSynapse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Synapse.h"
#include "ModulatedSynapse.h"
#include "Neuron.h"
#include "FastNeuralModule.h"

namespace FastNeuralNet
{
	namespace Synapses
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ModulatedSynapse::ModulatedSynapse()
{
}

ModulatedSynapse::~ModulatedSynapse()
{

}

float ModulatedSynapse::CalculateModulation(FastNeuralModule *lpModule)
{
	Neuron *lpNeuron=NULL;
	float fltIm=0;
	m_fltModulation=0;

	if(m_bEnabled)
	{
		lpNeuron = lpModule->GetNeuron(m_iFromX, m_iFromY, m_iFromZ);
		if(lpNeuron) 
		{
			fltIm = lpNeuron->FiringFreq(lpModule) * m_fltWeight;
			
			if(fltIm>=0)
				m_fltModulation = 1 + fltIm;
			else 
				m_fltModulation = 1/(1-fltIm);
		}
	}
	else
		m_fltModulation = 1;

	return m_fltModulation;
}

void ModulatedSynapse::Load(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml)
{

	oXml.IntoElem();  //Into GatedSynapse Element

	CStdIPoint oFrom;
	Std_LoadPoint(oXml, "From", oFrom);

	m_bEnabled = oXml.GetChildBool("Enabled", TRUE);
	m_iFromX = (unsigned char) oFrom.x;
	m_iFromY = (unsigned char) oFrom.y;
	m_iFromZ = (unsigned char) oFrom.z;
	m_fltWeight = oXml.GetChildFloat("Weight");


	oXml.OutOfElem(); //OutOf GatedSynapse Element
}



void ModulatedSynapse::Save(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml)
{

	oXml.AddChildElement("CompoundSynapse");
	oXml.IntoElem();  //Into CompoundSynapse Element


	oXml.AddChildElement("SynapseType", MODULATED_SYNAPSE);
	oXml.AddChildElement("FromX", m_iFromX);
	oXml.AddChildElement("FromY", m_iFromY);
	oXml.AddChildElement("FromZ", m_iFromZ);
	oXml.AddChildElement("Weight", m_fltWeight);


	oXml.OutOfElem();  //OutOf CompoundSynapse Element
}

	}			//Synapses
}				//FastNeuralNet


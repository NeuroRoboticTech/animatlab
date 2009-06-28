// TonicNeuron.cpp: implementation of the TonicNeuron class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Synapse.h"
#include "Neuron.h"
#include "TonicNeuron.h"
#include "FastNeuralModule.h"

namespace FastNeuralNet
{
	namespace Neurons
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TonicNeuron::TonicNeuron()
{
	m_fltIh=0;
}

TonicNeuron::~TonicNeuron()
{

}


float TonicNeuron::Ih()
{return m_fltIh;}

void TonicNeuron::Ih(float fltVal)
{m_fltIh=fltVal;}

unsigned char TonicNeuron::NeuronType()
{return TONIC_NEURON;}

float TonicNeuron::CalculateIntrinsicCurrent(FastNeuralModule *lpModule, float fltInputCurrent)
{return m_fltIh;}

void TonicNeuron::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	Neuron::Load(lpSim, lpStructure, oXml);


	oXml.IntoElem();  //Into Neuron Element


	m_fltIh = oXml.GetChildFloat("Ih");


	oXml.OutOfElem(); //OutOf Neuron Element
}



void TonicNeuron::Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	Neuron::Save(lpSim, lpStructure, oXml);

	oXml.AddChildElement("Ih", m_fltIh);
}

	}			//Neurons
}				//FastNeuralNet


// Synapse.cpp: implementation of the Synapse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Synapse.h"
#include "Neuron.h"
#include "FastNeuralModule.h"

namespace FastNeuralNet
{
	namespace Synapses
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Synapse::Synapse()
{
	m_bEnabled = TRUE;
	m_iFromX=0;
	m_iFromY=0;
	m_iFromZ=0;
	m_fltWeight=0;
	m_fltModulation=0;
}

Synapse::~Synapse()
{

}

BOOL Synapse::Enabled()
{return m_bEnabled;}

void Synapse::Enabled(BOOL bVal)
{m_bEnabled=bVal;}

unsigned char Synapse::FromX()
{return m_iFromX;}

void Synapse::FromX(unsigned char iVal)
{m_iFromX=iVal;}

unsigned char Synapse::FromY()
{return m_iFromY;}

void Synapse::FromY(unsigned char iVal)
{m_iFromY=iVal;}

unsigned char Synapse::FromZ()
{return m_iFromZ;}

void Synapse::FromZ(unsigned char iVal)
{m_iFromZ=iVal;}

float Synapse::Weight()
{return m_fltWeight;}

void Synapse::Weight(float fltVal)
{m_fltWeight=fltVal;}

float *Synapse::WeightPointer()
{return &m_fltWeight;}

float Synapse::Modulation()
{return m_fltModulation;}

float *Synapse::ModulationPointer()
{return &m_fltModulation;}

float Synapse::CalculateModulation(FastNeuralModule *lpModule)
{return 1;}

float *Synapse::GetDataPointer(short iCompoundIndex, short iDataType)
{
	float *lpData = NULL;

	switch(iDataType)
	{
	case SYNAPSE_WEIGHT_DATA: 
		lpData = &m_fltWeight;
		break;

	case SYNAPSE_COMPOUND_WEIGHT_DATA: 
		THROW_ERROR(Nl_Err_lCompoundNotSupported, Nl_Err_strCompoundNotSupported);
		break;

	case SYNAPSE_COMPOUND_MODULATION_DATA:
		THROW_ERROR(Nl_Err_lCompoundNotSupported, Nl_Err_strCompoundNotSupported);
		break;

	default:
		THROW_PARAM_ERROR(Nl_Err_lInvalidNeuronDataType, Nl_Err_strInvalidNeuronDataType, "Synapse Data Type", (long) iDataType);
	}

	return lpData;
}

void Synapse::Load(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml)
{

	oXml.IntoElem();  //Into Synapse Element

	CStdIPoint oFrom;
	Std_LoadPoint(oXml, "From", oFrom);

	m_bEnabled = oXml.GetChildBool("Enabled", TRUE);
	m_iFromX = (unsigned char) oFrom.x;
	m_iFromY = (unsigned char) oFrom.y;
	m_iFromZ = (unsigned char) oFrom.z;
	m_fltWeight = oXml.GetChildFloat("Weight");

	oXml.OutOfElem(); //OutOf Synapse Element
}



void Synapse::Save(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml)
{

	oXml.AddChildElement("Synapse");
	oXml.IntoElem();  //Into Synapse Element

	//oXml.AddChildElement("SynapseType", m_strType);
	oXml.AddChildElement("FromX", m_iFromX);
	oXml.AddChildElement("FromY", m_iFromY);
	oXml.AddChildElement("FromZ", m_iFromZ);
	oXml.AddChildElement("Weight", m_fltWeight);

	oXml.OutOfElem();  //OutOf Synapse Element
}

	}			//Synapses
}				//FastNeuralNet







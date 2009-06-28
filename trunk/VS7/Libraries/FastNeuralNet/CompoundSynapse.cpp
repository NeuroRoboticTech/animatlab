// CompoundSynapse.cpp: implementation of the CompoundSynapse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Synapse.h"
#include "CompoundSynapse.h"
#include "Neuron.h"
#include "FastNeuralModule.h"
#include "ClassFactory.h"

namespace FastNeuralNet
{
	namespace Synapses
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CompoundSynapse::CompoundSynapse()
{

}

CompoundSynapse::~CompoundSynapse()
{

try
{
	m_arySynapses.RemoveAll();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of CompoundSynapse\r\n", "", -1, FALSE, TRUE);}
}


float CompoundSynapse::CalculateModulation(FastNeuralModule *lpModule)
{
	m_fltModulation=1;
	int iCount, iIndex;
	Synapse *lpSynapse=NULL;

	iCount = m_arySynapses.GetSize();
	for(iIndex=0; iIndex<iCount; iIndex++)
	{
		lpSynapse = m_arySynapses[iIndex];
		m_fltModulation*=lpSynapse->CalculateModulation(lpModule);
	}

	return m_fltModulation;
}



float *CompoundSynapse::GetDataPointer(short iCompoundIndex, short iDataType)
{
	Synapse *lpSynapse=NULL;
	float *lpData;

	switch(iDataType)
	{
	case SYNAPSE_WEIGHT_DATA: 
		lpData = &m_fltWeight;
		break;

	case SYNAPSE_COMPOUND_WEIGHT_DATA:
		lpSynapse = GetCompoundSynapse(iCompoundIndex);
		lpData = lpSynapse->WeightPointer();
		break;

	case SYNAPSE_COMPOUND_MODULATION_DATA:
		lpSynapse = GetCompoundSynapse(iCompoundIndex);
		lpData = lpSynapse->ModulationPointer();
		break;

	default:
		THROW_PARAM_ERROR(Nl_Err_lInvalidNeuronDataType, Nl_Err_strInvalidNeuronDataType, "Syanpse Data Type", (long) iDataType);
	}

	return lpData;
}



Synapse *CompoundSynapse::GetCompoundSynapse(short iCompoundIndex)
{

	if( iCompoundIndex<0 || iCompoundIndex>=m_arySynapses.GetSize() ) 
		THROW_ERROR(Std_Err_lInvalidIndex, Std_Err_strInvalidIndex);
	return m_arySynapses[iCompoundIndex];
}


void CompoundSynapse::Load(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml)
{
	int iCount, iIndex;

	oXml.IntoElem();  //Into Synapse Element

	CStdIPoint oFrom;
	Std_LoadPoint(oXml, "From", oFrom);

	m_iFromX = (unsigned char) oFrom.x;
	m_iFromY = (unsigned char) oFrom.y;
	m_iFromZ = (unsigned char) oFrom.z;
	m_fltWeight = oXml.GetChildFloat("Weight");

	m_arySynapses.RemoveAll();

	//*** Begin Loading CompoundSynapses. *****
	oXml.IntoChildElement("CompoundSynapses");

	iCount = oXml.NumberOfChildren();
	for(iIndex=0; iIndex<iCount; iIndex++)
	{
		oXml.FindChildByIndex(iIndex);
		LoadSynapse(lpSim, lpStructure, lpNeuron, oXml);
	}

	oXml.OutOfElem();
	//*** End Loading CompoundSynapses. *****

	oXml.OutOfElem(); //OutOf Synapse Element
}


Synapse *CompoundSynapse::LoadSynapse(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml)
{
	Synapse *lpSynapse=NULL;
	string strType;

try
{
	oXml.IntoElem();  //Into Synapse Element
	strType = oXml.GetChildString("Type");
	oXml.OutOfElem(); //OutOf Synapse Element

	lpSynapse = dynamic_cast<Synapse *>(lpSim->CreateObject(Nl_NeuralModuleName(), "Synapse", strType));
	if(!lpSynapse)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Synapse");

	lpSynapse->Load(lpSim, lpStructure, lpNeuron, oXml);
	m_arySynapses.Add(lpSynapse);
	return lpSynapse;
}
catch(CStdErrorInfo oError)
{
	if(lpSynapse) delete lpSynapse;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpSynapse) delete lpSynapse;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}


void CompoundSynapse::Save(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml)
{
	Synapse *lpSynapse=NULL;
	int iCount, iIndex;

	oXml.AddChildElement("Synapse");
	oXml.IntoElem();  //Into Synapse Element


	oXml.AddChildElement("SynapseType", COMPOUND_SYNAPSE);
	oXml.AddChildElement("FromX", m_iFromX);
	oXml.AddChildElement("FromY", m_iFromY);
	oXml.AddChildElement("FromZ", m_iFromZ);
	oXml.AddChildElement("Weight", m_fltWeight);

	//*** Begin Saving CompoundSynapses. *****
	oXml.AddChildElement("CompoundSynapses");
	oXml.IntoChildElement("CompoundSynapses");

	iCount = m_arySynapses.GetSize();
	for(iIndex=0; iIndex<iCount; iIndex++)
		m_arySynapses[iIndex]->Save(lpSim, lpStructure, lpNeuron, oXml);

	oXml.OutOfElem();
	//*** End Saving CompoundSynapses. *****


	oXml.OutOfElem();  //OutOf Synapse Element
}

	}			//Synapses
}				//FastNeuralNet



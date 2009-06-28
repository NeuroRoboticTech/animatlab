// RNeuron.cpp: implementation of the Neuron class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Synapse.h"
#include "Neuron.h"
#include "[*PROJECT_NAME*]NeuralModule.h"

namespace [*PROJECT_NAME*]
{
	namespace Neurons
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Neuron::Neuron()
{
	m_bEnabled = TRUE;

	m_fltExternalI = 0;			//Externally injected current
	m_fltIntrinsicI = 0;
	m_fltSynapticI = 0;
	m_fltAdapterI = 0;
	m_fltAdapterMemoryI = 0;
	m_fltFiringFreq = 0;
	m_fltVndisp = 0;
}

Neuron::~Neuron()
{

try
{
	m_arySynapses.RemoveAll();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Neuron\r\n", "", -1, FALSE, TRUE);}
}

float Neuron::ExternalI()
{return m_fltExternalI;}

void Neuron::ExternalI(float fltVal)
{m_fltExternalI=fltVal;}

float Neuron::IntrinsicCurrent()
{return m_fltIntrinsicI;}

void Neuron::IntrinsicCurrent(float fltVal)
{m_fltIntrinsicI = fltVal;}

unsigned char Neuron::NeuronType()
{return RUGULAR_NEURON;}

CStdPtrArray<Synapse> *Neuron::GetSynapses()
{return &m_arySynapses;}

void Neuron::AddSynapse(Synapse *lpSynapse)
{
	if(!lpSynapse) 
		THROW_ERROR(Nl_Err_lSynapseToAddNull, Nl_Err_strSynapseToAddNull);
	m_arySynapses.Add(lpSynapse);
}

void Neuron::RemoveSynapse(int iIndex)
{
	if( iIndex<0 || iIndex>=m_arySynapses.GetSize() ) 
		THROW_ERROR(Std_Err_lInvalidIndex, Std_Err_strInvalidIndex);
	m_arySynapses.RemoveAt(iIndex);
}

Synapse *Neuron::GetSynapse(int iIndex)
{
	if( iIndex<0 || iIndex>=m_arySynapses.GetSize() ) 
		THROW_ERROR(Std_Err_lInvalidIndex, Std_Err_strInvalidIndex);
	return m_arySynapses[iIndex];
}


int Neuron::TotalSynapses()
{return m_arySynapses.GetSize();}

void Neuron::ClearSynapses()
{m_arySynapses.RemoveAll();}


void Neuron::StepSimulation(Simulator *lpSim, Organism *lpOrganism, [*PROJECT_NAME*]NeuralModule *lpModule, unsigned char iXPos, unsigned char iYPos, unsigned char iZPos)
{
	if(m_bEnabled)
	{
	}
}

float Neuron::CalculateIntrinsicCurrent(Simulator *lpSim, Organism *lpOrganism, [*PROJECT_NAME*]NeuralModule *lpModule, float fltInputCurrent)
{return 0;}

float Neuron::CalculateSynapticCurrent(Simulator *lpSim, Organism *lpOrganism, [*PROJECT_NAME*]NeuralModule *lpModule)
{
	unsigned char iSynapse, iCount;
	float fltSynapticI=0;
	Synapse *lpSynapse=NULL;
	Neuron *lpNeuron=NULL;

	iCount = m_arySynapses.GetSize();
	for(iSynapse=0; iSynapse<iCount; iSynapse++)
	{
		lpSynapse = m_arySynapses[iSynapse];

		if(lpSynapse->Enabled())
		{
			lpNeuron = lpModule->GetNeuron(lpSynapse->FromX(), lpSynapse->FromY(), lpSynapse->FromZ()); 
			if(lpNeuron) 
				fltSynapticI+= lpSynapse->CalculateCurrent(lpSim, lpOrganism, lpModule, this); 
		}
	}

	return fltSynapticI;
}

void Neuron::Initialize(Simulator *lpSim, Organism *lpOrganism, [*PROJECT_NAME*]NeuralModule *lpModule)
{
} 

void Neuron::AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput)
{
	m_fltAdapterI += fltInput;
	m_fltAdapterMemoryI = m_fltAdapterI;
}

float *Neuron::GetDataPointer(string strDataType)
{
	string strType = Std_CheckString(strDataType);

	if(strType == "INTRINSICCURRENT")
		return &m_fltIntrinsicI;

	if(strType == "EXTERNALCURRENT")
		return &m_fltExternalI;

	if(strType == "SYNAPTICCURRENT")
		return &m_fltSynapticI;

	if(strType == "ADAPTERCURRENT")
		return &m_fltAdapterMemoryI;

	if(strType == "MEMBRANEVOLTAGE")
		return &m_fltVndisp;

	if(strType == "FIRINGFREQUENCY")
		return &m_fltFiringFreq;

	//If it was not one of those above then we have a problem.
	THROW_PARAM_ERROR(Nl_Err_lInvalidNeuronDataType, Nl_Err_strInvalidNeuronDataType, "Neuron Data Type", strDataType);

	return NULL;
}

long Neuron::CalculateSnapshotByteSize()
{
	//We need bytes for the internal state variables for this neuron.
	return (sizeof(m_fltExternalI) + sizeof(m_fltIntrinsicI) + sizeof(m_fltSynapticI));
}

void Neuron::SaveKeyFrameSnapshot(byte *aryBytes, long &lIndex)
{
	memcpy((void *) (aryBytes+lIndex), (void *)&m_fltExternalI, sizeof(m_fltExternalI));
    lIndex += sizeof(m_fltExternalI);

	memcpy((void *) (aryBytes+lIndex), (void *)&m_fltIntrinsicI, sizeof(m_fltIntrinsicI));
    lIndex += sizeof(m_fltIntrinsicI);

	memcpy((void *) (aryBytes+lIndex), (void *)&m_fltSynapticI, sizeof(m_fltSynapticI));
    lIndex += sizeof(m_fltSynapticI);
}

void Neuron::LoadKeyFrameSnapshot(byte *aryBytes, long &lIndex)
{
	memcpy((void *)&m_fltExternalI, (void *) (aryBytes+lIndex), sizeof(m_fltExternalI));
    lIndex += sizeof(m_fltExternalI);

	memcpy((void *)&m_fltIntrinsicI, (void *) (aryBytes+lIndex), sizeof(m_fltIntrinsicI));
    lIndex += sizeof(m_fltIntrinsicI);

	memcpy((void *)&m_fltSynapticI, (void *) (aryBytes+lIndex), sizeof(m_fltSynapticI));
    lIndex += sizeof(m_fltSynapticI);
}

void Neuron::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	int iCount, iIndex;

	oXml.IntoElem();  //Into Neuron Element

	m_arySynapses.RemoveAll();

	m_strName = oXml.GetChildString("Name", "");
	m_bEnabled = oXml.GetChildBool("Enabled", TRUE);

	m_fltExternalI = oXml.GetChildFloat("ExternalI", 0);

	//*** Begin Loading Synapses. *****
	if(oXml.FindChildElement("Synapses", FALSE))
	{
		oXml.IntoElem();  //Into Synapses Element

		iCount = oXml.NumberOfChildren();
		for(iIndex=0; iIndex<iCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			LoadSynapse(lpSim, lpStructure, oXml);
		} 

		oXml.OutOfElem();
	}
	//*** End Loading Synapses. *****


	oXml.OutOfElem(); //OutOf Neuron Element
}


Synapse *Neuron::LoadSynapse(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	string strType;
	Synapse *lpSynapse=NULL;

try
{
	oXml.IntoElem();  //Into Synapse Element
	strType = oXml.GetChildString("Type");
	oXml.OutOfElem(); //OutOf Synapse Element

	lpSynapse = dynamic_cast<Synapse *>(lpSim->CreateObject("[*PROJECT_NAME*]", "Synapse", strType));
	if(!lpSynapse)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Synapse");

	lpSynapse->Load(lpSim, lpStructure, this, oXml);
	AddSynapse(lpSynapse);
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

void Neuron::Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	int iCount, iIndex;

	//Do NOT create and go into the nueron element here. We had to create
	//the neuron element in the brain object in order to save the x,y,z params.


	oXml.AddChildElement("NeuronType", NeuronType());
	oXml.AddChildElement("ExternalI", m_fltExternalI);

	//*** Begin Saving Synapses. *****
	oXml.AddChildElement("Synapses");
	oXml.IntoChildElement("Synapses");

	iCount = m_arySynapses.GetSize();
	for(iIndex=0; iIndex<iCount; iIndex++)
		m_arySynapses[iIndex]->Save(lpSim, lpStructure, this, oXml);

	oXml.OutOfElem();
	//*** End Saving Synapses. *****


	//Do NOT exit out of the neuron element here.
}

	}			//Neurons
}				//[*PROJECT_NAME*]




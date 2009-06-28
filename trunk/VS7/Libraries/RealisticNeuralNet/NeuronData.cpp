// NlNeuronDataColumn.cpp: implementation of the NeuronData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IonChannel.h"
#include "RealisticNeuralModule.h"
#include "Connexion.h"
#include "Neuron.h"
#include "ElectricalSynapse.h"
#include "NonSpikingChemicalSynapse.h"
#include "SpikingChemicalSynapse.h"
#include "Neuron.h"
#include "NeuronData.h"

namespace RealisticNeuralNet
{
	namespace DataColumns
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NeuronData::NeuronData()
{
	m_iNeuronIndex=0;
}

NeuronData::~NeuronData()
{

}


void NeuronData::Initialize(Simulator *lpSim)
{
	Organism *lpOrganism = lpSim->FindOrganism(m_strOrganismID);

	RealisticNeuralModule *lpModule = dynamic_cast<RealisticNeuralModule *>(lpOrganism->NervousSystem()->FindNeuralModule(Rn_NeuralModuleName()));
	if(!lpModule)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "RealisticNeuralModule");

	Node *lpNeuron = lpModule->FindNode(m_iNeuronIndex);

	if(!lpNeuron)
		THROW_TEXT_ERROR(Rn_Err_lNeuronNotFound, Rn_Err_strNeuronNotFound, " NeuronIndex: (" + STR(m_iNeuronIndex) + ")");

	m_lpDataValue = lpNeuron->GetDataPointer(m_strDataType);

	if(!m_lpDataValue)
		THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
		("DataColumn: " + m_strID + " Index: " + STR(m_iNeuronIndex) + " DataType: " + m_strDataType));

	m_bInitialized = TRUE;
}


void NeuronData::Load(Simulator *lpSim, CStdXml &oXml)
{

	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);
		
	DataColumn::Load(lpSim, oXml);

	oXml.IntoElem();  //Into DataColumn Element

	m_strOrganismID = oXml.GetChildString("OrganismID");

	if(Std_IsBlank(m_strOrganismID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	m_iNeuronIndex = oXml.GetChildInt("NeuronIndex");
	Std_IsAboveMin((int) -1, (int) m_iNeuronIndex, TRUE, "NeuronIndex");

	m_strDataType = oXml.GetChildString("DataType");
	
	if(Std_IsBlank(m_strDataType)) 
		THROW_TEXT_ERROR(Al_Err_lDataTypeBlank, Al_Err_strDataTypeBlank, 
		  "NeurondData: OrganismID: " + m_strOrganismID + " NeuronIndex: (" + STR(m_iNeuronIndex) + ")");

	oXml.OutOfElem(); //OutOf DataColumn Element
}

	}			//DataColumns
}				//RealisticNeuralNet

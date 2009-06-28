// NlChannelDataColumn.cpp: implementation of the ChannelData class.
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
#include "ChannelData.h"

namespace RealisticNeuralNet
{
	namespace DataColumns
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ChannelData::ChannelData()
{
	m_iNeuronIndex=0;
}

ChannelData::~ChannelData()
{

}


void ChannelData::Initialize(Simulator *lpSim)
{
	Organism *lpOrganism = lpSim->FindOrganism(m_strOrganismID);

	RealisticNeuralModule *lpModule = dynamic_cast<RealisticNeuralModule *>(lpOrganism->NervousSystem()->FindNeuralModule(Rn_NeuralModuleName()));
	if(!lpModule)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "RealisticNeuralModule");

	Node *lpNeuron = lpModule->FindNode(m_iNeuronIndex);

	if(!lpNeuron)
		THROW_TEXT_ERROR(Rn_Err_lNeuronNotFound, Rn_Err_strNeuronNotFound, " NeuronIndex: (" + STR(m_iNeuronIndex) + ")");

	IonChannel *lpChannel = (IonChannel *)(lpNeuron->GetDataItem("IONCHANNEL", m_strChannelID));

	m_lpDataValue = lpChannel->GetDataPointer(m_strDataType);

	if(!m_lpDataValue)
		THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
		("DataColumn: " + m_strID + " Index: " + STR(m_iNeuronIndex) + " DataType: " + m_strDataType));

	m_bInitialized = TRUE;
}


void ChannelData::Load(Simulator *lpSim, CStdXml &oXml)
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

	m_strChannelID = oXml.GetChildString("ChannelID");

	if(Std_IsBlank(m_strChannelID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	m_strDataType = oXml.GetChildString("DataType");
	
	if(Std_IsBlank(m_strDataType)) 
		THROW_TEXT_ERROR(Al_Err_lDataTypeBlank, Al_Err_strDataTypeBlank, 
		  "NeurondData: OrganismID: " + m_strOrganismID + " NeuronIndex: (" + STR(m_iNeuronIndex) + ")");

	oXml.OutOfElem(); //OutOf DataColumn Element
}

	}			//DataColumns
}				//RealisticNeuralNet

// NlNeuronDataColumn.cpp: implementation of the NeuronData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Synapse.h"
#include "Neuron.h"
#include "FastNeuralModule.h"
#include "NeuronData.h"

namespace FastNeuralNet
{
	namespace DataColumns
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NeuronData::NeuronData()
{

}

NeuronData::~NeuronData()
{

}


void NeuronData::Initialize(Simulator *lpSim)
{
	Organism *lpOrganism = lpSim->FindOrganism(m_strOrganismID);

	FastNeuralModule *lpModule = dynamic_cast<FastNeuralModule *>(lpOrganism->NervousSystem()->FindNeuralModule(Nl_NeuralModuleName()));
	if(!lpModule)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "FastNeuralModule");

	Neuron *lpNeuron = lpModule->GetNeuron(m_oPosition.x, m_oPosition.y, m_oPosition.z);

	if(!lpNeuron)
		THROW_TEXT_ERROR(Nl_Err_lNeuronNotFound, Nl_Err_strNeuronNotFound, " Neuron: (" + STR(m_oPosition.x) + 
		                 ", " + STR(m_oPosition.y) + ", " + STR(m_oPosition.z) + ")");

	m_lpDataValue = lpNeuron->GetDataPointer(m_strDataType);

	if(!m_lpDataValue)
		THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
		("DataColumn: " + m_strID + " OrganismID: " + m_strOrganismID + " Neuron: (" + STR(m_oPosition.x) + 
		                 ", " + STR(m_oPosition.y) + ", " + STR(m_oPosition.z) + ")" + " DataType: " + m_strDataType));

	m_bInitialized = TRUE;
}


void NeuronData::Load(Simulator *lpSim, CStdXml &oXml)
{

	if(!lpSim)
		THROW_ERROR(Nl_Err_lSimulationNotDefined, Nl_Err_strSimulationNotDefined);
		
	DataColumn::Load(lpSim, oXml);

	oXml.IntoElem();  //Into DataColumn Element

	m_strOrganismID = oXml.GetChildString("OrganismID");

	if(Std_IsBlank(m_strOrganismID)) 
		THROW_ERROR(Nl_Err_lIDBlank, Nl_Err_strIDBlank);

	Std_LoadPoint(oXml, "Position", m_oPosition);

	m_strDataType = oXml.GetChildString("DataType");
	
	if(Std_IsBlank(m_strDataType)) 
		THROW_TEXT_ERROR(Al_Err_lDataTypeBlank, Al_Err_strDataTypeBlank, 
		  "NeurondData: OrganismID: " + m_strOrganismID + " Position: (" + STR(m_oPosition.x) + ", " + STR(m_oPosition.y) + ", " + STR(m_oPosition.z) + ")");

	oXml.OutOfElem(); //OutOf DataColumn Element
}

	}			//DataColumns
}				//FastNeuralNet

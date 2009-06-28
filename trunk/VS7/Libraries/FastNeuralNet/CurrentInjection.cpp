// CurrentInjection.cpp: implementation of the CurrentInjection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Synapse.h"
#include "Neuron.h"
#include "FastNeuralModule.h"
#include "CurrentInjection.h"

namespace FastNeuralNet
{
	namespace ExternalStimuli
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CurrentInjection::CurrentInjection()
{
	m_fltCurrent = 0;
	m_lpNeuron = NULL;
}

CurrentInjection::~CurrentInjection()
{
	m_lpNeuron = NULL;
}


void CurrentInjection::Initialize(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Nl_Err_lSimulationNotDefined, Nl_Err_strSimulationNotDefined);

	ActivatedItem::Initialize(lpSim);

	//First lets find the correct organism
	Organism *lpOrganism = lpSim->FindOrganism(m_strOrganismID);

	//Now lets get the correct neural module.
	FastNeuralModule *lpModule = dynamic_cast<FastNeuralModule *>(lpOrganism->NervousSystem()->FindNeuralModule(Nl_NeuralModuleName()));

	if(!lpModule)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "FastNeuralModule");

	//Lets try and get the neuron we will be injecting.
	m_lpNeuron = lpModule->GetNeuron(m_oPosition.x, m_oPosition.y, m_oPosition.z);

	if(!m_lpNeuron)
		THROW_TEXT_ERROR(Nl_Err_lNeuronToInjectNotFound, Nl_Err_strNeuronToInjectNotFound, 
		                 "OrganismID: " + m_strOrganismID + " Position: (" + STR(m_oPosition.x) + 
										 ", " + STR(m_oPosition.y) + ", " + STR(m_oPosition.z) + ")");
}

void CurrentInjection::Activate(Simulator *lpSim)
{
	m_lpNeuron->InjectCurrent(m_fltCurrent);
}


void CurrentInjection::StepSimulation(Simulator *lpSim)
{}


void CurrentInjection::Deactivate(Simulator *lpSim)
{
	m_lpNeuron->InjectCurrent(-m_fltCurrent);
}

void CurrentInjection::Load(Simulator *lpSim, CStdXml &oXml)
{
	ActivatedItem::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strOrganismID = oXml.GetChildString("OrganismID");
	if(Std_IsBlank(m_strOrganismID)) 
		THROW_ERROR(Nl_Err_lIDBlank, Nl_Err_strIDBlank);

	Std_LoadPoint(oXml, "Position", m_oPosition);

	m_fltCurrent = oXml.GetChildFloat("Current");

	oXml.OutOfElem(); //OutOf Simulus Element
}


void CurrentInjection::Save(Simulator *lpSim, CStdXml &oXml)
{

	oXml.AddChildElement("Simulus");
	oXml.IntoElem();  //Into Simulus Element

	oXml.AddChildElement("OrganismID", m_strOrganismID);
	Std_SavePoint(oXml, "Position", m_oPosition);
	oXml.AddChildElement("StartSlice", m_lStartSlice);
	oXml.AddChildElement("EndSlice", m_lEndSlice);
	oXml.AddChildElement("Current", m_fltCurrent);

	oXml.OutOfElem();  //OutOf Simulus Element
}

	}			//ExternalStimuli
}				//FastNeuralNet




// ClassFactory.cpp: implementation of the ClassFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IonChannel.h"
#include "Connexion.h"
#include "Neuron.h"
#include "ElectricalSynapse.h"
#include "NonSpikingChemicalSynapse.h"
#include "SpikingChemicalSynapse.h"
#include "NeuronData.h"
#include "RealisticNeuralModule.h"
#include "IonChannelSigmoid.h"
#include "ChannelData.h"
#include "ClassFactory.h"

namespace RealisticNeuralNet
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ClassFactory::ClassFactory()
{

}

ClassFactory::~ClassFactory()
{

}


// ************* External Stimulus Type Conversion functions ******************************

ExternalStimulus *ClassFactory::CreateExternalStimulus(string strType, BOOL bThrowError)
{
	ExternalStimulus *lpStimulus=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "CURRENT")
		lpStimulus = new AnimatLibrary::ExternalStimuli::CurrentStimulus;
	else
	{
		lpStimulus = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidExternalStimulusType, Al_Err_strInvalidExternalStimulusType, "ExternalStimulusType", strType);
	}

	return lpStimulus;
}
catch(CStdErrorInfo oError)
{
	if(lpStimulus) delete lpStimulus;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpStimulus) delete lpStimulus;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

// ************* External Stimulus Type Conversion functions ******************************


// ************* External Neural Module Conversion functions ******************************

NeuralModule *ClassFactory::CreateNeuralModule(string strType, BOOL bThrowError)
{
	NeuralModule *lpModule=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "REALISTICNEURALMODULE")
		lpModule = new RealisticNeuralModule;
	else
	{
		lpModule = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidNeuralModuleType, Al_Err_strInvalidNeuralModuleType, "NeuralModule", strType);
	}

	return lpModule;
}
catch(CStdErrorInfo oError)
{
	if(lpModule) delete lpModule;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpModule) delete lpModule;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}


// ************* Neural Module Type Conversion functions ******************************


// ************* DataColumn Type Conversion functions ******************************

DataColumn *ClassFactory::CreateDataColumn(string strType, BOOL bThrowError)
{
	DataColumn *lpColumn=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "NEURONDATA")
		lpColumn = new NeuronData;
	else if(strType == "IONCHANNELDATA")
		lpColumn = new ChannelData;
	else
	{
		lpColumn = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidDataColumnType, Al_Err_strInvalidDataColumnType, "DataColumnType", strType);
	}

	return lpColumn;
}
catch(CStdErrorInfo oError)
{
	if(lpColumn) delete lpColumn;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpColumn) delete lpColumn;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}


// ************* DataColumn Type Conversion functions ******************************


// ************* Gain Type Conversion functions ******************************

Gain *ClassFactory::CreateGain(string strType, BOOL bThrowError)
{
	Gain *lpGain=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "IONCHANNELSIGMOID")
		lpGain = new RealisticNeuralNet::Gains::IonChannelSigmoid;
	else
	{
		lpGain = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidGainType, Al_Err_strInvalidGainType, "GainType", strType);
	}

	return lpGain;
}
catch(CStdErrorInfo oError)
{
	if(lpGain) delete lpGain;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpGain) delete lpGain;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}


// ************* Gain Conversion functions ******************************


// ************* IStdClassFactory functions ******************************

CStdSerialize *ClassFactory::CreateObject(string strClassType, string strObjectType, BOOL bThrowError)
{
	CStdSerialize *lpObject=NULL;

	strClassType = Std_ToUpper(Std_Trim(strClassType));

	if(strClassType == "EXTERNALSTIMULUS")
		lpObject = CreateExternalStimulus(strObjectType, bThrowError);
	else if(strClassType == "NEURALMODULE")
		lpObject = CreateNeuralModule(strObjectType, bThrowError);
	else if(strClassType == "DATACOLUMN")
		lpObject = CreateDataColumn(strObjectType, bThrowError);
	else if(strClassType == "GAIN")
		lpObject = CreateGain(strObjectType, bThrowError);
	else
	{
		lpObject = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Std_Err_lInvalidClassType, Std_Err_strInvalidClassType, "ClassType", strClassType);
	}

	return lpObject;
}

// ************* IStdClassFactory functions ******************************

}				//RealisticNeuralNet

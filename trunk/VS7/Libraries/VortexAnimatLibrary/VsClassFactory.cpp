// VsClassFactory.cpp: implementation of the VsClassFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VsClassFactory.h"
#include "VsRigidBody.h"
#include "VsPlane.h"
#include "VsBox.h"
#include "VsBoxContactSensor.h"
#include "VsCylinder.h"
#include "VsCylinderContactSensor.h"
#include "VsCone.h" 
#include "VsSphere.h"
#include "VsJoint.h"
#include "VsBallSocket.h"
#include "VsHingeJoint.h"
#include "VsPrismatic.h"
#include "VsSpring.h"
#include "VsMultiSegmentSpring.h"
#include "VsStaticJoint.h"
#include "VsMesh.h"
#include "VsLinearHillMuscle.h"
#include "VsLinearHillStretchReceptor.h"
#include "VsMuscleAttachment.h"
#include "VsOdorSensor.h"
#include "VsMouth.h"
#include "VsSimulator.h"
#include "VsStructure.h"
#include "VsOrganism.h"

#include "VsVideoKeyFrame.h"
#include "VsSnapshotKeyFrame.h"

#include "VsMotorVelocityStimulus.h"
#include "VsForceStimulus.h"
#include "VsPositionClamp.h"
#include "VsInverseMuscleCurrent.h"

namespace VortexAnimatLibrary
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VsClassFactory::VsClassFactory()
{

}

VsClassFactory::~VsClassFactory()
{

}

// ************* Body Type Conversion functions ******************************

RigidBody *VsClassFactory::CreateRigidBody(string strType, BOOL bThrowError)
{
	RigidBody *lpPart=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "BOX")
		lpPart = new VsBox;
	else if(strType == "BOXCONTACTSENSOR")
		lpPart = new VsBoxContactSensor;
	else if(strType == "CYLINDER")
		lpPart = new VsCylinder;
	else if(strType == "CYLINDERCONTACTSENSOR")
		lpPart = new VsCylinderContactSensor;
	else if(strType == "CONE")
		lpPart = new VsCone;
	else if(strType == "SPHERE")
		lpPart = new VsSphere;
	else if(strType == "PLANE")
		lpPart = new VsPlane;
	else if(strType == "MESH")
		lpPart = new VsMesh;
	else if(strType == "MUSCLEATTACHMENT")
		lpPart = new VsMuscleAttachment;
	else if(strType == "LINEARHILLMUSCLE")
		lpPart = new VsLinearHillMuscle;
	else if(strType == "LINEARHILLSTRETCHRECEPTOR")
		lpPart = new VsLinearHillStretchReceptor;
	else if(strType == "SPRING")
		lpPart = new VsSpring;
	else if(strType == "MULTISEGMENTSPRING")
		lpPart = new VsMultiSegmentSpring;
	else if(strType == "ODORSENSOR")
		lpPart = new VsOdorSensor;
	else if(strType == "STOMACH")
		lpPart = new AnimatLibrary::Environment::Bodies::Stomach;
	else if(strType == "MOUTH")
		lpPart = new VsMouth;
	else
	{
		lpPart = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidPartType, Al_Err_strInvalidPartType, "PartType", strType);
	}
	
	return lpPart;
}
catch(CStdErrorInfo oError)
{
	if(lpPart) delete lpPart;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpPart) delete lpPart;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

// ************* Body Type Conversion functions ******************************


// ************* Body Joint Conversion functions ******************************

Joint *VsClassFactory::CreateJoint(string strType, BOOL bThrowError)
{
	Joint *lpJoint=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "HINGE")
		lpJoint = new VsHinge;
	else if(strType == "BALLSOCKET")
		lpJoint = new VsBallSocket;
	else if(strType == "PRISMATIC")
		lpJoint = new VsPrismatic;
	else if(strType == "STATIC")
		lpJoint = new VsStatic;
	else
	{
		lpJoint = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidJointType, Al_Err_strInvalidJointType, "JointType", strType);
	}

	return lpJoint;
}
catch(CStdErrorInfo oError)
{
	if(lpJoint) delete lpJoint;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpJoint) delete lpJoint;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

// ************* Body Joint Conversion functions ******************************

// ************* Organism Type Conversion functions ******************************

Structure *VsClassFactory::CreateStructure(string strType, BOOL bThrowError)
{
	Structure *lpStructure=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "BASIC")
		lpStructure = new VsOrganism;
	else if(strType == "ORGANISM")
		lpStructure = new VsOrganism;
	else if(strType == "STRUCTURE")
		lpStructure = new VsStructure;
	else
	{
		lpStructure = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidOrganismType, Al_Err_strInvalidOrganismType, "OrganismType", strType);
	}

	return lpStructure;
}
catch(CStdErrorInfo oError)
{
	if(lpStructure) delete lpStructure;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpStructure) delete lpStructure;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

// ************* Organism Type Conversion functions ******************************


// ************* Simulator Type Conversion functions ******************************

Simulator *VsClassFactory::CreateSimulator(string strType, BOOL bThrowError)
{
	Simulator *lpSimulator=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "VORTEXSIMULATOR")
		lpSimulator = new VsSimulator;
	else if(strType == "")
		lpSimulator = new VsSimulator;
	else
	{
		lpSimulator = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidSimulatorType, Al_Err_strInvalidSimulatorType, "SimulatorType", strType);
	}

	return lpSimulator;
}
catch(CStdErrorInfo oError)
{
	if(lpSimulator) delete lpSimulator;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpSimulator) delete lpSimulator;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

// ************* Organism Type Conversion functions ******************************



// ************* KeyFrame Type Conversion functions ******************************
KeyFrame *VsClassFactory::CreateKeyFrame(string strType, BOOL bThrowError)
{
	KeyFrame *lpFrame=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "VIDEO")
		lpFrame = new VsVideoKeyFrame;
	else if(strType == "SNAPSHOT")
		lpFrame = new VsSnapshotKeyFrame;
	else
	{
		lpFrame = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidKeyFrameType, Al_Err_strInvalidKeyFrameType, "KeyFrameType", strType);
	}

	return lpFrame;
}
catch(CStdErrorInfo oError)
{
	if(lpFrame) delete lpFrame;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpFrame) delete lpFrame;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

// ************* KeyFrame Type Conversion functions ******************************


// ************* DataChart Type Conversion functions ******************************

DataChart *VsClassFactory::CreateDataChart(string strType, BOOL bThrowError)
{
	DataChart *lpChart=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "TABFILE")
		lpChart = new FileChart;
	else if(strType == "FILECHART")
		lpChart = new FileChart;
	else if(strType == "MEMORYCHART")
		lpChart = new MemoryChart;
	else if(strType == "ARRAYCHART")
		lpChart = new ArrayChart;
	else
	{
		lpChart = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidDataChartType, Al_Err_strInvalidDataChartType, "DataChartType", strType);
	}

	return lpChart;
}
catch(CStdErrorInfo oError)
{
	if(lpChart) delete lpChart;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpChart) delete lpChart;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}
// ************* DataChart Type Conversion functions ******************************


// ************* DataColumn Type Conversion functions ******************************

DataColumn *VsClassFactory::CreateDataColumn(string strType, BOOL bThrowError)
{
	DataColumn *lpColumn=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "RIGIDBODYDATA")
		lpColumn = new RigidBodyDataColumn;
	else if(strType == "JOINTDATA")
		lpColumn = new JointDataColumn;
	else if(strType == "STIMULUSDATA")
		lpColumn = new StimulusDataColumn;
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


// ************* Adapter Type Conversion functions ******************************

Adapter *VsClassFactory::CreateAdapter(string strType, BOOL bThrowError)
{
	Adapter *lpAdapter=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "NODETONODE")
		lpAdapter = new NodeToNodeAdapter;
	else if(strType == "NODETOPHYSICAL")
		lpAdapter = new NodeToPhysicalAdapter;
	else if(strType == "PHYSICALTONODE")
		lpAdapter = new PhysicalToNodeAdapter;
	else if(strType == "CONTACT")
		lpAdapter = new ContactAdapter;
	else
	{
		lpAdapter = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidAdapterType, Al_Err_strInvalidAdapterType, "AdapterType", strType);
	}

	return lpAdapter;
}
catch(CStdErrorInfo oError)
{
	if(lpAdapter) delete lpAdapter;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpAdapter) delete lpAdapter;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

// ************* Adpater Type Conversion functions ******************************


// ************* Gain Type Conversion functions ******************************

Gain *VsClassFactory::CreateGain(string strType, BOOL bThrowError)
{
	Gain *lpGain=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "BELL")
		lpGain = new BellGain;
	else if(strType == "EQUATION")
		lpGain = new EquationGain;
	else if(strType == "POLYNOMIAL")
		lpGain = new PolynomialGain;
	else if(strType == "SIGMOID")
		lpGain = new SigmoidGain;
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

// ************* Adpater Type Conversion functions ******************************


// ************* External Stimulus Type Conversion functions ******************************

ExternalStimulus *VsClassFactory::CreateExternalStimulus(string strType, BOOL bThrowError)
{
	ExternalStimulus *lpStimulus=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "MOTORVELOCITY")
		lpStimulus = new VsMotorVelocityStimulus;
	else if(strType == "FORCEINPUT")
		lpStimulus = new VsForceStimulus;
	else if(strType == "NODEINPUT")
		lpStimulus = new NodeInputStimulus;
	else if(strType == "RIGIDBODYINPUT")
		lpStimulus = new RigidBodyInputStimulus;
	else if(strType == "JOINTINPUT")
		lpStimulus = new JointInputStimulus;
	else if(strType == "ENABLERINPUT")
		lpStimulus = new EnablerStimulus;
	else if(strType == "POSITIONCLAMP")
		lpStimulus = new VsPositionClamp;
	else if(strType == "VELOCITYCLAMP")
		lpStimulus = new VsPositionClamp;
	else if(strType == "INVERSEMUSCLECURRENT")
		lpStimulus = new VsInverseMuscleCurrent;
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

// ************* Microcontroller Type Conversion functions ******************************

Microcontroller *VsClassFactory::CreateMicrocontroller(string strType, BOOL bThrowError)
{
	Microcontroller *lpMicrocontroller=NULL;

try
{
	strType = Std_ToUpper(Std_Trim(strType));

	if(strType == "MICROCONTROLLER")
		lpMicrocontroller = new Microcontroller;
	else
	{
		lpMicrocontroller = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Al_Err_lInvalidMicrocontrollerType, Al_Err_strInvalidMicrocontrollerType, "MicrocontrollerType", strType);
	}

	return lpMicrocontroller;
}
catch(CStdErrorInfo oError)
{
	if(lpMicrocontroller) delete lpMicrocontroller;
	RELAY_ERROR(oError); 
	return NULL;
}
catch(...)
{
	if(lpMicrocontroller) delete lpMicrocontroller;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

// ************* Adpater Type Conversion functions ******************************


// ************* IStdClassFactory functions ******************************

CStdSerialize *VsClassFactory::CreateObject(string strClassType, string strObjectType, BOOL bThrowError)
{
	CStdSerialize *lpObject=NULL;

	strClassType = Std_ToUpper(Std_Trim(strClassType));

	if(strClassType == "RIGIDBODY")
		lpObject = CreateRigidBody(strObjectType, bThrowError);
	else if(strClassType == "JOINT")
		lpObject = CreateJoint(strObjectType, bThrowError);
	else if(strClassType == "ORGANISM")
		lpObject = CreateStructure(strObjectType, bThrowError);
	else if(strClassType == "STRUCTURE")
		lpObject = CreateStructure(strObjectType, bThrowError);
	else if(strClassType == "SIMULATOR")
		lpObject = CreateSimulator(strObjectType, bThrowError);
	else if(strClassType == "KEYFRAME")
		lpObject = CreateKeyFrame(strObjectType, bThrowError);
	else if(strClassType == "DATACHART")
		lpObject = CreateDataChart(strObjectType, bThrowError);
	else if(strClassType == "DATACOLUMN")
		lpObject = CreateDataColumn(strObjectType, bThrowError);
	else if(strClassType == "EXTERNALSTIMULUS")
		lpObject = CreateExternalStimulus(strObjectType, bThrowError);
	else if(strClassType == "ADAPTER")
		lpObject = CreateAdapter(strObjectType, bThrowError);
	else if(strClassType == "GAIN")
		lpObject = CreateGain(strObjectType, bThrowError);
	else if(strClassType == "MICROCONTROLLER")
		lpObject = CreateMicrocontroller(strObjectType, bThrowError);
	else
	{
		lpObject = NULL;
		if(bThrowError)
			THROW_PARAM_ERROR(Std_Err_lInvalidClassType, Std_Err_strInvalidClassType, "ClassType", strClassType);
	}

	return lpObject;
}
// ************* IStdClassFactory functions ******************************



}			//VortexAnimatLibrary

extern "C" __declspec(dllexport) IStdClassFactory* __cdecl GetStdClassFactory() 
{
	IStdClassFactory *lpFactory = new VsClassFactory;
	return lpFactory;
}

extern "C" __declspec(dllexport) int __cdecl BootstrapRunLibrary(int argc, const char **argv) 
{
	Simulator *lpSim = NULL;

try
{
	Simulator *lpSim = Simulator::CreateSimulator(argc, argv);

	lpSim->Load();
	lpSim->Initialize(argc, argv);
	lpSim->Simulate();

	if(lpSim) delete lpSim;

	return 0;
}
catch(CStdErrorInfo oError)
{
	if(lpSim) delete lpSim;
	printf("Error occurred: %s\n", oError.m_strError) ;
	return (int) oError.m_lError;
}
catch(...)
{
	if(lpSim) delete lpSim;
  printf("An Unknown Error occurred.\n") ;
	return -1;
}
}


// LinearHillMuscle.cpp: implementation of the LinearHillMuscle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <math.h>
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "MuscleAttachment.h"
#include "IODataEntry.h"
#include "Microcontroller.h"
#include "Structure.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"

#include "ExternalStimulus.h"

#include "MuscleBase.h"
#include "LinearHillMuscle.h"

namespace AnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*! \brief 
   Constructs a muscle object..
   		
   \param lpParent This is a pointer to the parent of this rigid body. 
	          If this value is null then it is assumed that this is
						a root object and no joint is loaded to connect this
						part to the parent.

	 \return
	 No return value.

   \remarks
	 The constructor for a muscle. 
*/

LinearHillMuscle::LinearHillMuscle()
{
	m_fltKse = 0;
	m_fltKpe = 0;
	m_fltB = 0;
	m_fltKseByB = 0;
	m_fltKpeByKse = 0;
	m_fltMuscleRestingLength = 0;
	m_fltA1 = 0;
	m_fltA2 = 0;
	m_fltA3 = 0;
	m_fltA4 = 0;
	m_bUseStimLimits = FALSE;
	m_fltStimLowerLimit = -0.1f;
	m_fltStimUpperLimit = 0;
	m_fltStimLowerOutput = 0;
	m_fltStimUpperOutput = 6;
	m_fltMaxTension = 0;
	m_fltDisplacement = 0;
	m_fltDisplacementRatio = 0;
	m_fltVmuscle = 0;
	m_fltVm = (float) -0.15;
	m_fltTL = 0;
	m_fltTLPerc = 0;
	m_fltAct = 0;
	m_fltA = 0;
	m_fltTdot = 0;
	m_fltTension = 0;
	m_fltPrevTension = 0;
	m_fltInternalTension = 0;
	m_fltMuscleLength = 0;
	m_fltPrevMuscleLength = 0;
	m_fltTLwidth = 0;
	m_fltTLc = 0;
	m_fltSeRestLength = 0;
	m_fltPeLengthPercentage = 90;
	m_fltMinPeLengthPercentage = 5;
	m_fltMinPeLength = 0;
	m_fltIbDischargeConstant = 100;
	m_fltIbRate = 0;

	m_fltSeLPrev = 0;
	m_fltPeLPrev = 0;
	m_fltVse = 0;
	m_fltVpe = 0;

	m_aryMuscleVelocities = NULL;
	m_iMuscleVelAvgCount = 4;
	m_iVelAvgIndex = 0;
	m_fltAvgMuscleVel = 0;
}

/*! \brief 
   Destroys the muscle object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the muscle object..	 
*/

LinearHillMuscle::~LinearHillMuscle()
{
	try
	{
		if(m_aryMuscleVelocities) 
		{delete[] m_aryMuscleVelocities; m_aryMuscleVelocities = NULL;}

	}
	catch(...)
	{Std_TraceMsg(0, "Caught Error in desctructor of LinearHillMuscle\r\n", "", -1, FALSE, TRUE);}
}

void LinearHillMuscle::Enabled(BOOL bVal)
{
	m_bEnabled = bVal;
	m_fltEnabled = (float) bVal;

	if(!bVal)
	{
		m_fltInternalTension = 0;
		m_fltTdot = 0;
		m_fltTension = 0;
	}
}

inline float LinearHillMuscle::Ftl(float fltL)
{
	float fltLceNorm = fltL - m_fltMuscleRestingLength;
	float fltTl = (-(pow(fltLceNorm, 2)/m_fltTLc)  + 1);
	if(fltTl<0) fltTl = 0;
	return fltTl;
	return 0;
}

inline float LinearHillMuscle::Fact(float fltStim)
{	
	float fltAct=0;

	if(m_bUseStimLimits && ((fltStim <= m_fltStimLowerLimit) || (fltStim >=m_fltStimUpperLimit)))
	{
		if(fltStim<=m_fltStimLowerLimit)
			fltAct = m_fltStimLowerOutput;
		else
			fltAct = m_fltStimUpperOutput;
	}
	else
		fltAct = (m_fltA2/(1 + exp(m_fltA3*(m_fltA1-fltStim)))) + m_fltA4;

	if(fltAct <0)
		fltAct = 0;

	return fltAct;
}

void LinearHillMuscle::CalculateTension(Simulator *lpSim)
{
	//int i=0;
	//if(lpSim->Time() >= 0.419)
	//	i=6;

	//Store the previous muscle length
	m_fltPrevMuscleLength = m_fltMuscleLength;

	//Calculate the current muscle length.
	m_fltMuscleLength = CalculateMuscleLength(lpSim);

	//Calculate the displacement of this muscle d = (x-x*)
	m_fltDisplacement = m_fltMuscleLength-m_fltMuscleRestingLength;
	m_fltDisplacementRatio = m_fltMuscleLength/m_fltMuscleRestingLength;

	//Calculate the instantaneous velocity of change of the muscle length.
	m_fltVmuscle = (m_fltMuscleLength-m_fltPrevMuscleLength)/lpSim->PhysicsTimeStep();

	//Calculate averaged velocity
	m_aryMuscleVelocities[m_iVelAvgIndex] = m_fltVmuscle;
	m_iVelAvgIndex++;
	if(m_iVelAvgIndex >= m_iMuscleVelAvgCount) m_iVelAvgIndex = 0;

	m_fltAvgMuscleVel = 0;
	for(int i=0; i<m_iMuscleVelAvgCount; i++)
		m_fltAvgMuscleVel+= m_aryMuscleVelocities[i];
	m_fltAvgMuscleVel = m_fltAvgMuscleVel/m_iMuscleVelAvgCount;


	//Calculate the active force that is generated by neural stimulation
	m_fltTL = Ftl(m_fltMuscleLength);
	m_fltAct = Fact(m_fltVm);
	m_fltA = m_fltTL * m_fltAct;

	m_fltTLPerc = m_fltTL*100;

	//Calculation of the derivitave of the tension
	m_fltTdot = m_fltKseByB*(m_fltKpe*m_fltDisplacement + m_fltB*m_fltVmuscle - m_fltKpeByKse*m_fltInternalTension + m_fltA);  

	//The new tension
	m_fltInternalTension = m_fltInternalTension + m_fltTdot*lpSim->PhysicsTimeStep();

	//tension can never be negative, but we want to maintain the "internal" calculations so that the
	//time constants are correct. If you shorten the muscle rapidly it will take it some time to 
	//rebuild its tension. This time will be determined by the internal tension, new length,  and the params like viscosity.
	//But the force seen on the muscle itself will still be 0 N because it is slack.
	//If we did not do this then if your muscle is at rest and is pulled, stays steady, and then relaxed, then
	//it would end up generating a negative tension. (See fig 4 of shadmehr web doc on muscle model).
	//There is a problem in this figure in that you can see that the muscle produced negative tension and this is not 
	//possible in a real muscle.
	if(m_fltInternalTension >= 0)
		m_fltTension = m_fltInternalTension;
	else
		m_fltTension = 0;

	//Make certain that the tension never exceed the absolute maximum set by the user.
	if(m_fltTension > m_fltMaxTension)
		m_fltTension = m_fltMaxTension;

	m_fltSeLPrev = m_fltSeLength;
	m_fltPeLPrev = m_fltPeLength;

	m_fltSeLength = m_fltSeRestLength + (m_fltTension/m_fltKse);
	m_fltPeLength = m_fltMuscleLength - m_fltSeLength;
	if(m_fltPeLength < m_fltMinPeLength)
	{
		m_fltSeLength = m_fltMuscleLength  - m_fltMinPeLength;
		m_fltPeLength = m_fltMinPeLength;
	}

	m_fltVse = (m_fltSeLength-m_fltSeLPrev)/lpSim->PhysicsTimeStep();
	m_fltVpe = (m_fltPeLength-m_fltPeLPrev)/lpSim->PhysicsTimeStep();

	m_fltIbRate = m_fltIbDischargeConstant*m_fltTension;
}

//Calculates the membrane voltage needed for the inverse dynamics of the muscle.
void LinearHillMuscle::CalculateInverseDynamics(Simulator *lpSim, float fltLength, float fltVelocity, float fltT, float &fltVm, float &fltA)
{
	//Calculate inverse dynamics force needed
	m_fltPrevA = fltA;
	fltA = fltT - m_fltKpe*(fltLength-m_fltMuscleRestingLength) - m_fltB*fltVelocity + (m_fltKpe/m_fltKse)*fltT;

	if(fltA<0)
		fltA = m_fltPrevA;

	//Calculate tension length percentage
	float fltTl = Ftl(fltLength);

	//Increase A to take Tension-length curve into account.
	fltA = fltA/fltTl;

	//Use A to calculate voltage required.
	if(fltA > 0)
		fltVm = (float) (m_fltA1 - (1/m_fltA3)*log((m_fltA2-fltA)/fltA));
	else
		fltVm = 0;

	//if(lpSim->Time() > 2.092 && m_strName == "Left Tricep Stretch Receptor")
	//	fltVm=fltVm;
}

void LinearHillMuscle::CreateJoints(Simulator *lpSim, Structure *lpStructure)
{
	MuscleBase::CreateJoints(lpSim, lpStructure);

	m_fltTLc = pow(m_fltTLwidth, 2);

	m_fltKseByB = m_fltKse/m_fltB;
	m_fltKpeByKse = (1 + (m_fltKpe/m_fltKse));

	m_fltSeRestLength = m_fltMuscleRestingLength - (m_fltMuscleRestingLength * m_fltPeLengthPercentage);
	m_fltMinPeLength = m_fltMuscleRestingLength * m_fltMinPeLengthPercentage;

	m_fltSeLength = m_fltSeRestLength;
	m_fltPeLength = m_fltMuscleRestingLength - m_fltSeRestLength;

	//Lets create the muscle velocity averaging array.
	if(m_iMuscleVelAvgCount <= 0)
		THROW_TEXT_ERROR(Al_Err_lInvalidMusc_Vel_Avg, Al_Err_strInvalidMusc_Vel_Avg, "Muscle Velocity Average: " + STR(m_iMuscleVelAvgCount));

	m_aryMuscleVelocities = new float[m_iMuscleVelAvgCount];

	//Zero out the average array
	for(int i=0; i<m_iMuscleVelAvgCount; i++)
		m_aryMuscleVelocities[i] = 0;
}

float *LinearHillMuscle::GetDataPointer(string strDataType)
{
	float *lpData=NULL;
	string strType = Std_CheckString(strDataType);

	if(strType == "TENSION")
		lpData = &m_fltTension;
	else if(strType == "TDOT")
		lpData = &m_fltTdot;
	else if(strType == "MUSCLELENGTH")
		lpData = &m_fltMuscleLength;
	else if(strType == "VMUSCLE")
		lpData = &m_fltVmuscle;
	else if(strType == "VSE")
		lpData = &m_fltVse;
	else if(strType == "VPE")
		lpData = &m_fltVpe;
	else if(strType == "AVGVMUSCLE")
		lpData = &m_fltAvgMuscleVel;
	else if(strType == "MEMBRANEVOLTAGE")
		lpData = &m_fltVm;
	else if(strType == "DISPLACEMENT")
		lpData = &m_fltDisplacement;
	else if(strType == "DISPLACEMENTRATIO")
		lpData = &m_fltDisplacementRatio;
	else if(strType == "ACTIVATION")
		lpData = &m_fltAct;
	else if(strType == "A")
		lpData = &m_fltA;
	else if(strType == "SELENGTH")
		lpData = &m_fltSeLength;
	else if(strType == "PELENGTH")
		lpData = &m_fltPeLength;
	else if(strType == "IB")
		lpData = &m_fltIbRate;
	else if(strType == "TL")
		lpData = &m_fltTLPerc;
	else if(strType == "ENABLE")
		lpData = &m_fltEnabled;
	else
		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "RigidBodyID: " + STR(m_strName) + "  DataType: " + strDataType);

	return lpData;
}

void LinearHillMuscle::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	MuscleBase::Load(lpSim, lpStructure, oXml);

	oXml.IntoElem();  //Into RigidBody Element

	//Load the Stimulus-Tension params.
	if(oXml.FindChildElement("StimulusTension", FALSE))
	{
		oXml.IntoChildElement("StimulusTension");
		m_fltA1 = AnimatLibrary::LoadScaledNumber(oXml, "A1", FALSE, m_fltA1);
		m_fltA2 = AnimatLibrary::LoadScaledNumber(oXml, "A2", FALSE, m_fltA2);
		m_fltA3 = AnimatLibrary::LoadScaledNumber(oXml, "A3", FALSE, m_fltA3);
		m_fltA4 = AnimatLibrary::LoadScaledNumber(oXml, "A4", FALSE, m_fltA4);
		m_bUseStimLimits = oXml.GetChildBool("UseLimits", m_bUseStimLimits);
		if(m_bUseStimLimits)
		{
			m_fltStimLowerLimit = AnimatLibrary::LoadScaledNumber(oXml, "LowerLimitScale", FALSE, m_fltStimLowerLimit);
			m_fltStimUpperLimit = AnimatLibrary::LoadScaledNumber(oXml, "UpperLimitScale", FALSE, m_fltStimUpperLimit);
			m_fltStimLowerOutput = AnimatLibrary::LoadScaledNumber(oXml, "LowerOutputScale", FALSE, m_fltStimLowerOutput);
			m_fltStimUpperOutput = AnimatLibrary::LoadScaledNumber(oXml, "UpperOutputScale", FALSE, m_fltStimUpperOutput);
		}

		oXml.OutOfElem();
	}

	//Load the Length-Tension params.
	if(oXml.FindChildElement("LengthTension", FALSE))
	{
		oXml.IntoChildElement("LengthTension");
		m_fltMuscleRestingLength = AnimatLibrary::LoadScaledNumber(oXml, "RestingLength", FALSE, m_fltMuscleRestingLength);
		m_fltTLwidth = AnimatLibrary::LoadScaledNumber(oXml, "Lwidth", FALSE, m_fltTLwidth);
		oXml.OutOfElem();
	}

	m_fltKse = AnimatLibrary::LoadScaledNumber(oXml, "Kse", FALSE, m_fltKse);
	m_fltKpe = AnimatLibrary::LoadScaledNumber(oXml, "Kpe", FALSE, m_fltKpe);
	m_fltB = AnimatLibrary::LoadScaledNumber(oXml, "B", FALSE, m_fltB);
	m_fltPeLengthPercentage = AnimatLibrary::LoadScaledNumber(oXml, "PeLength", FALSE, m_fltPeLengthPercentage)/100;
	m_fltMinPeLengthPercentage = AnimatLibrary::LoadScaledNumber(oXml, "MinPeLength", FALSE, m_fltMinPeLengthPercentage)/100;
	m_fltIbDischargeConstant = AnimatLibrary::LoadScaledNumber(oXml, "IbDischarge", FALSE, m_fltIbDischargeConstant);

	Std_IsAboveMin((float) 0, m_fltMuscleRestingLength, TRUE, "MuscleRestingLength");
	Std_InValidRange((float) 0, (float) 1e11, m_fltKpe, TRUE, "Kpe");
	Std_InValidRange((float) 0.00001, (float) 1e11, m_fltKse, TRUE, "Kse");
	Std_InValidRange((float) 0, (float) 1e11, m_fltB, TRUE, "B");
	Std_InValidRange((float) 0, (float) 1, m_fltPeLengthPercentage, TRUE, "PeLengthPercentage");
	Std_InValidRange((float) 0, m_fltPeLengthPercentage, m_fltMinPeLengthPercentage, TRUE, "MinPeLengthPercentage");
	Std_InValidRange((float) 0, (float) 1e11, m_fltIbDischargeConstant, TRUE, "IbDischargeConstant");

	oXml.OutOfElem(); //OutOf RigidBody Element
}

		}		//Bodies
	}			//Environment
}				//AnimatLibrary

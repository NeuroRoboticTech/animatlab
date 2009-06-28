// VsHinge.cpp: implementation of the VsHinge class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsHingeJoint.h"
#include "VsSimulator.h"

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Joints
		{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VsHinge::VsHinge()
{
	m_fltRotationDeg = 0;
	m_fltConstraintLow = 0.25*ME_PI;
	m_fltConstraintHigh = -0.25*ME_PI;
}

VsHinge::~VsHinge()
{

}

//If this is a servo motor then the "velocity" signal is not really a velocity signal in this case. 
//It is the desired position and we must convert it to the velocity needed to reach and maintian that position.
void VsHinge::CalculateServoVelocity()
{
	if(!m_lpLimitID)
		return;

	if(m_bEnableLimits)
	{
		if(m_fltDesiredVelocity>m_fltConstraintHigh)
			m_fltDesiredVelocity = m_fltConstraintHigh;
		if(m_fltDesiredVelocity<m_fltConstraintLow)
			m_fltDesiredVelocity = m_fltConstraintLow;

		float fltError = m_fltDesiredVelocity - m_lpLimitID->position;
		float fltProp = fltError / (m_fltConstraintHigh-m_fltConstraintLow);

		m_fltDesiredVelocity = fltProp * m_ftlServoGain; 
	}
	else
	{
		float fltError = m_fltDesiredVelocity - m_lpLimitID->position;
		m_fltDesiredVelocity = fltError * m_fltMaxVelocity; 
	}

}

void VsHinge::SetVelocityToDesired()
{
	if(m_bEnableMotor)
	{			
		if(m_bServoMotor)
			CalculateServoVelocity();

		if(m_fltDesiredVelocity>m_fltMaxVelocity)
			m_fltDesiredVelocity = m_fltMaxVelocity;

		if(m_fltDesiredVelocity < -m_fltMaxVelocity)
			m_fltDesiredVelocity = -m_fltMaxVelocity;

		m_fltSetVelocity = m_fltDesiredVelocity;
		m_fltDesiredVelocity = 0;

		//Only do anything if the velocity value has changed
		if( fabs(m_lpLimitID->velocity - m_fltSetVelocity) > 1e-4)
		{
			//VsJoint::SetVelocity(m_fltSetVelocity, m_fltMaxTorque);

			if(fabs(m_fltSetVelocity) > 1e-4)
				VsJoint::SetVelocity(m_fltSetVelocity, m_fltMaxTorque);
			else
				VsJoint::EnableLock(TRUE, m_lpLimitID->position, m_fltMaxTorque);
		}
		
		m_fltPrevVelocity = m_fltSetVelocity;
	}
}

void VsHinge::EnableMotor(BOOL bVal)
{
	VsJoint::EnableMotor(bVal, m_fltSetVelocity, m_fltMaxTorque);
	m_bEnableMotor = bVal;
	m_fltPrevVelocity = -1000000;  //reset the prev velocity for the next usage
}

void VsHinge::CreateJoint(Simulator *lpSim, Structure *lpStructure)
{
	if(!m_lpParent)
		THROW_ERROR(Al_Err_lParentNotDefined, Al_Err_strParentNotDefined);

	if(!m_lpChild)
		THROW_ERROR(Al_Err_lChildNotDefined, Al_Err_strChildNotDefined);

	VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);
	if(!lpVsSim)
		THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

	VsRigidBody *lpVsParent = dynamic_cast<VsRigidBody *>(m_lpParent);
	if(!lpVsParent)
		THROW_ERROR(Vs_Err_lUnableToConvertToVsRigidBody, Vs_Err_strUnableToConvertToVsRigidBody);

	VsRigidBody *lpVsChild = dynamic_cast<VsRigidBody *>(m_lpChild);
	if(!lpVsChild)
		THROW_ERROR(Vs_Err_lUnableToConvertToVsRigidBody, Vs_Err_strUnableToConvertToVsRigidBody);

	m_lpConstraintID = MdtHingeCreate(lpVsSim->World());
	MdtConstraintSetBodies(m_lpConstraintID, lpVsParent->BodyID(), lpVsChild->BodyID());
	  
	m_lpLimitID = MdtConstraintGetLimit(m_lpConstraintID);
	MdtConstraintSetAxis(m_lpConstraintID, m_oRotationAxis.x, m_oRotationAxis.y, m_oRotationAxis.z);
	MdtConstraintSetPosition(m_lpConstraintID, m_oAbsPosition.x, m_oAbsPosition.y, m_oAbsPosition.z);
 
	MdtSingleLimitSetStop(MdtLimitGetLowerLimit(m_lpLimitID), m_fltConstraintLow);
	MdtSingleLimitSetStop(MdtLimitGetUpperLimit(m_lpLimitID), m_fltConstraintHigh);

	MdtConstraintEnable(m_lpConstraintID);

	McdSpaceDisablePair(lpVsParent->ModelID(), lpVsChild->ModelID());

	MdtLimitActivateLimits(m_lpLimitID, m_bEnableLimits);
	MdtLimitSetLimitedForceMotor(m_lpLimitID, 0, -m_fltMaxTorque, m_fltMaxTorque);

	MdtSingleLimitSetStiffness(MdtLimitGetLowerLimit(m_lpLimitID), m_fltStiffness);
	MdtSingleLimitSetStiffness(MdtLimitGetUpperLimit(m_lpLimitID), m_fltStiffness);

	MdtSingleLimitSetRestitution(MdtLimitGetLowerLimit(m_lpLimitID), m_fltRestitution);
	MdtSingleLimitSetRestitution(MdtLimitGetUpperLimit(m_lpLimitID), m_fltRestitution);

	MdtSingleLimitSetDamping(MdtLimitGetLowerLimit(m_lpLimitID), m_fltDamping);
	MdtSingleLimitSetDamping(MdtLimitGetUpperLimit(m_lpLimitID), m_fltDamping);

	if(!m_bEnableMotor) 
		EnableMotor(m_bEnableMotor);
}


float *VsHinge::GetDataPointer(string strDataType)
{
	float *lpData=NULL;
	string strType = Std_CheckString(strDataType);

	if(strType == "JOINTROTATION")
	{
		if(!m_lpLimitID)
			THROW_PARAM_ERROR(Vs_Err_lLimitIDNotDefined, Vs_Err_strLimitIDNotDefined, "JointID", m_strName);

		lpData = &m_lpLimitID->position;
	}
	else if(strType == "JOINTROTATIONDEG")
		return &m_fltRotationDeg;
	else if(strType == "JOINTPOSITION")
	{
		if(!m_lpLimitID)
			THROW_PARAM_ERROR(Vs_Err_lLimitIDNotDefined, Vs_Err_strLimitIDNotDefined, "JointID", m_strName);

		lpData = &m_lpLimitID->position;
	}
	else if(strType == "JOINTACTUALVELOCITY")
	{
		if(!m_lpLimitID)
			THROW_PARAM_ERROR(Vs_Err_lLimitIDNotDefined, Vs_Err_strLimitIDNotDefined, "JointID", m_strName);

		lpData = &m_lpLimitID->velocity;
	}
	else if(strType == "JOINTDESIREDVELOCITY")
		return &m_fltSetVelocity;
	else if(strType == "JOINTSETVELOCITY")
		return &m_fltSetVelocity;
	else if(strType == "ENABLE")
		return &m_fltEnabled;
	else if(strType == "CONTACTCOUNT")
		THROW_PARAM_ERROR(Al_Err_lMustBeContactBodyToGetCount, Al_Err_strMustBeContactBodyToGetCount, "JointID", m_strName);
	else
	{
		lpData = VsJoint::GetVortexDataPointer(strDataType);
		if(lpData) return lpData;

		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "JointID: " + STR(m_strName) + "  DataType: " + strDataType);
	}

	return lpData;
}


void VsHinge::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	m_fltRotationDeg = ((m_lpLimitID->position/3.1412)*180);
	VsJoint::CollectJointData(lpSim);
	SetVelocityToDesired();
}

		}		//Joints
	}			// Environment
}				//VortexAnimatLibrary

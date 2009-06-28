// VsBallSocket.cpp: implementation of the VsBallSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsBallSocket.h"
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

VsBallSocket::VsBallSocket()
{
	m_lpConeLimitID = NULL;
}

VsBallSocket::~VsBallSocket()
{
	//We do not delete vortex objects.
	m_lpConeLimitID = NULL;
}

void VsBallSocket::SetVelocityToDesired()
{
}

void VsBallSocket::EnableMotor(BOOL bVal)
{
}


void VsBallSocket::CreateJoint(Simulator *lpSim, Structure *lpStructure)
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

	//create the ball socket constraint
	m_lpConstraintID = MdtBSJointCreate(lpVsSim->World());

	//set the bodies for the joint
	MdtBSJointSetBodies(m_lpConstraintID, lpVsParent->BodyID(), lpVsChild->BodyID());

	//set the position of the joint
	MdtBSJointSetPosition(m_lpConstraintID, m_oAbsPosition.x, m_oAbsPosition.y, m_oAbsPosition.z);

	//create the cone limit for the ball socket
	m_lpConeLimitID = MdtConeLimitCreate(lpVsSim->World());

	//set the bodies for the cone limit
	MdtConeLimitSetBodies(m_lpConeLimitID, lpVsParent->BodyID(), lpVsChild->BodyID());
	
	//set the body axes
	MdtConeLimitBodySetAxes(m_lpConeLimitID, 0,
      m_oConstraintAxis.x, m_oConstraintAxis.y, m_oConstraintAxis.z,
      0, 0, 1);
	MdtConeLimitBodySetAxes(m_lpConeLimitID, 1,
      0, -1, 0,
      0, 0, 1);

	MdtConeLimitSetConeHalfAngle(m_lpConeLimitID, m_fltConstraintAngle);

	MdtConeLimitSetStiffness(m_lpConeLimitID, m_fltStiffness);
	MdtConeLimitSetDamping(m_lpConeLimitID, m_fltDamping);

	/*MdtConstraintSetAxis(m_lpConstraintID, m_oRotationAxis.x, m_oRotationAxis.y, m_oRotationAxis.z);
  
	MdtConstraintEnable(m_lpConstraintID);*/

	McdSpaceDisablePair(lpVsParent->ModelID(), lpVsChild->ModelID());

	//enable the ball socket joint
	MdtBSJointEnable(m_lpConstraintID);

	//enable the cone limit
	MdtConeLimitEnable(m_lpConeLimitID);
}


float *VsBallSocket::GetDataPointer(string strDataType)
{
	float *lpData=NULL;
	string strType = Std_CheckString(strDataType);

	/*
	if(strType == "JOINTROTATION")
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
		return &m_fltDesiredVelocity;
	else if(strType == "JOINTSETVELOCITY")
		return &m_fltSetVelocity;
	else if(strType == "CONTACTCOUNT")
		THROW_PARAM_ERROR(Al_Err_lMustBeContactBodyToGetCount, Al_Err_strMustBeContactBodyToGetCount, "JointID", m_strName);
	else
		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "JointID: " + STR(m_strName) + "  DataType: " + strDataType);
*/
	return lpData;
}


//void VsBallSocket::StepSimulation(Simulator *lpSim, Structure *lpStructure)
//{
//	SetVelocityToDesired();
//}

		}		//Joints
	}			// Environment
}				//VortexAnimatLibrary

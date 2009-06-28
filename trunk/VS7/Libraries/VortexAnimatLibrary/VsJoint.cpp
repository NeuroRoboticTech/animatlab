// VsJoint.cpp: implementation of the VsJoint class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsSimulator.h"

namespace VortexAnimatLibrary
{
	namespace Environment
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VsJoint::VsJoint()
{
	m_lpConstraintID = NULL;
	m_lpLimitID = NULL;
	m_bMotorOn = FALSE;
	m_bLockOn = FALSE;

	m_bCollectData = FALSE;
	m_vPosition[0] = 0;
	m_vPosition[1] = 0;
	m_vPosition[2] = 0;
}

VsJoint::~VsJoint()
{
	//We do not delete vortex objects.
	m_lpConstraintID = NULL;
	m_lpLimitID = NULL;
}

MdtConstraintID VsJoint::ConstraintID()
{return m_lpConstraintID;}

MdtLimitID  VsJoint::LimitID()
{return m_lpLimitID;}

void VsJoint::EnableMotor(BOOL bOn, float fltDesiredVelocity, float fltMaxForce)
{
  if (m_lpConstraintID)
  { 
		//If it is locked then unlock it first
		if(bOn && MdtLimitIsLocked(m_lpLimitID))
			EnableLock(FALSE, 0, fltMaxForce);

    MdtLimitActivateMotor(m_lpLimitID, bOn);

    if(bOn)
      MdtLimitSetMotor(m_lpLimitID, fltDesiredVelocity, -fltMaxForce, fltMaxForce);

		m_bMotorOn = bOn;
  }
}

void VsJoint::EnableLock(MeBool bOn, float fltPosition, float fltMaxLockForce)
{
  if (m_lpConstraintID)
  { 
    MdtLimitActivateLock(m_lpLimitID, bOn);

    if(bOn)
    {
	  if(!m_bLockOn)
	  {
		if (m_bMotorOn)
		{
			EnableMotor(FALSE, 0, fltMaxLockForce);
			m_bMotorOn = 1;
		}

		MdtLimitSetLock(m_lpLimitID, fltPosition, -fltMaxLockForce, fltMaxLockForce);
		m_bLockOn = TRUE;
	  }
    }
    else if (m_bMotorOn)
	{
        EnableMotor(TRUE, 0, fltMaxLockForce);
		m_bLockOn = FALSE;
	}
  }
}

void VsJoint::SetVelocity(float fltDesiredVelocity, float fltMaxForce)
{
  if (m_lpConstraintID)
  {
		//Turn the lock off
		if(MdtLimitIsLocked(m_lpLimitID))
			EnableLock(FALSE, 0, fltMaxForce);

		//If the motor is not on then turn it on. Otherwise just set the new velocity
		if(!m_bMotorOn)
			EnableMotor(TRUE, fltDesiredVelocity, fltMaxForce);
		else
	   MdtLimitSetMotor(m_lpLimitID, fltDesiredVelocity, -fltMaxForce, fltMaxForce);
  }
}

void VsJoint::CollectJointData(Simulator *lpSim)
{
	if(m_bCollectData && m_lpConstraintID)
	{
		MdtConstraintGetPosition(m_lpConstraintID, m_vPosition);
		m_vPosition[0] = m_vPosition[0] * lpSim->DistanceUnits();
		m_vPosition[1] = m_vPosition[1] * lpSim->DistanceUnits();
		m_vPosition[2] = m_vPosition[2] * lpSim->DistanceUnits();
	}
}

float *VsJoint::GetVortexDataPointer(string &strDataType)
{
	string strType = Std_CheckString(strDataType);
	Joint *lpBody = dynamic_cast<Joint *>(this);
	m_bCollectData = TRUE;

	if(strType == "BODYPOSITIONX")
		return (&m_vPosition[0]);

	if(strType == "BODYPOSITIONY")
		return (&m_vPosition[1]);

	if(strType == "BODYPOSITIONZ")
		return (&m_vPosition[2]);

	return NULL;
}

	}			// Environment
}				//VortexAnimatLibrary
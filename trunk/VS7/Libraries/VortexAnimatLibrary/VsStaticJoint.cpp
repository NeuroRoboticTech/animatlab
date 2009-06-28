// VsStaticJoint.cpp: implementation of the VsStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsStaticJoint.h"
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

VsStatic::VsStatic()
{

}

VsStatic::~VsStatic()
{

}

void VsStatic::EnableMotor(BOOL bVal)
{}


void VsStatic::CreateJoint(Simulator *lpSim, Structure *lpStructure)
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

  m_lpConstraintID = MdtRPROJointCreate(lpVsSim->World());
  MdtConstraintSetBodies(m_lpConstraintID, lpVsParent->BodyID(), lpVsChild->BodyID());

  MeVector3 vPos;
  MdtBodyGetPosition(lpVsParent->BodyID(), vPos);
  MdtConstraintSetPosition(m_lpConstraintID, vPos[0], vPos[1], vPos[2]);

  MdtConstraintEnable(m_lpConstraintID);
	McdSpaceDisablePair(lpVsParent->ModelID(), lpVsChild->ModelID());

  MdtRPROJointSetAttachmentQuaternion(m_lpConstraintID, 1, 0, 0, 0, 0);
  MdtRPROJointSetAttachmentQuaternion(m_lpConstraintID, 1, 0, 0, 0, 0);
	MdtRPROJointSetRelativeQuaternionFromBodies(m_lpConstraintID);

	MdtRPROJointSetLinearStrength(m_lpConstraintID, MEINFINITY, MEINFINITY, MEINFINITY);
	MdtRPROJointSetAngularStrength(m_lpConstraintID, MEINFINITY, MEINFINITY, MEINFINITY);
}

		}		//Joints
	}			// Environment
}				//VortexAnimatLibrary



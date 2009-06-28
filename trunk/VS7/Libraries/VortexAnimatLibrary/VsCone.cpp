// VsCone.cpp: implementation of the VsCone class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VsRigidBody.h"
#include "VsCone.h"
#include "VsSimulator.h"

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VsCone::VsCone()
{
}

VsCone::~VsCone()
{

}

CStdFPoint VsCone::GetCurrentPosition()
{
	m_oAbsPosition.x = m_lpBodyID->bodyTM[3][0];
	m_oAbsPosition.y = m_lpBodyID->bodyTM[3][1];
	m_oAbsPosition.z = m_lpBodyID->bodyTM[3][2];

	return m_oAbsPosition;
}

void VsCone::CreateParts(Simulator *lpSim, Structure *lpStructure)
{
	TRACE_DEBUG("Statring Parts Creation for " + m_strName);
	TRACE_DETAIL_NS("PartType: Cone");

	VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);
	if(!lpVsSim)
		THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

	MeMatrix4 aryRotationTM, aryBodyTM;
	MeMatrix3 aryRotation;

	//We are going to create the collision mesh directly from the vertices of this temporary graphics mesh.
	MeMatrix4TMMakeIdentity(aryBodyTM);
	m_lpGraphic = RGraphicConeCreate_New(lpVsSim->RenderContext(), m_fltCollisionLowerRadius, m_fltCollisionUpperRadius, m_fltCollisionHeight, m_aryColor, aryBodyTM);
	m_lpGeometryID = VsRigidBody::CreateConeGeometry(m_lpGraphic);
	if(!m_lpGeometryID)
		THROW_PARAM_ERROR(Vs_Err_lCreatingGeometry, Vs_Err_strCreatingGeometry, "BodyID", m_strName);

	if(m_lpGraphic)
		RGraphicDelete(lpVsSim->RenderContext(), m_lpGraphic, 0);
	m_lpGraphic = NULL;

	//Now we can create the rest of the cone.
	MeMatrix4TMSetRotationFromEulerAngles(aryRotationTM, m_oRotation.x, m_oRotation.y, m_oRotation.z);
	MeMatrix4TMGetRotation(aryRotation, aryRotationTM) ;
	MeMatrix4TMMakeFromRotationAndPosition(aryBodyTM, aryRotation, m_oAbsPosition.x, m_oAbsPosition.y, m_oAbsPosition.z);

	m_lpModelID = MstModelAndBodyCreate(lpVsSim->Universe(), m_lpGeometryID, m_fltDensity);

	m_lpBodyID = McdModelGetBody(m_lpModelID);
	MdtBodySetTransform (m_lpBodyID, aryBodyTM);

	MeMatrix4Copy(m_aryInitialBodyTM, aryBodyTM);

	if(m_oCenterOfMass.x != 0 || m_oCenterOfMass.y != 0 || m_oCenterOfMass.z != 0)
	{
		MeMatrix4 aryCenterOfMassTM;
		MeVector3 aryFinalPos, aryCOM;
		aryCOM[0] = m_oCenterOfMass.x; aryCOM[1] = m_oCenterOfMass.y; aryCOM[2] = m_oCenterOfMass.z;

		//First lets account for any rotations of this body.
		MeMatrix4TMMakeIdentity(aryRotationTM);
		MeMatrix4TMSetRotationFromEulerAngles(aryRotationTM, m_oRotation.x, m_oRotation.y, m_oRotation.z);
		MeMatrix4TMTransformVector3(aryFinalPos, aryRotationTM, aryCOM);

		//Now create the center of mass transformation matrix and set the center of mass
		MeMatrix4TMSetRotationFromEulerAngles(aryRotationTM, 0, 0, 0);
		MeMatrix4TMGetRotation(aryRotation, aryRotationTM) ;
		MeMatrix4TMMakeFromRotationAndPosition(aryCenterOfMassTM, aryRotation, aryFinalPos[0], aryFinalPos[1], aryFinalPos[2]);
		MdtBodySetCenterOfMassRelativeTransform(m_lpBodyID, aryCenterOfMassTM);
	}

	MdtBodyEnable(m_lpBodyID);

	if(m_bIsVisible)
	  m_lpGraphic = RGraphicConeCreate_New(lpVsSim->RenderContext(), m_fltLowerRadius, m_fltUpperRadius, m_fltHeight, m_aryColor, MdtBodyGetTransformPtr(m_lpBodyID));

	//Lets get the volume and areas
	m_fltVolume = 2*PI*m_fltLowerRadius*m_fltLowerRadius*m_fltHeight;
	m_fltXArea = 2*m_fltLowerRadius*m_fltHeight;
	m_fltYArea = 2*m_fltLowerRadius*m_fltHeight;
	m_fltZArea = 2*PI*m_fltLowerRadius*m_fltLowerRadius;

	if(m_bFreeze)
		MstModelFreeze(m_lpModelID);

	if(!Std_IsBlank(m_strTexture))
		RGraphicSetTexture(lpVsSim->RenderContext(), m_lpGraphic, TextureFile(lpSim, m_strTexture).c_str());

	if(m_fltLinearVelocityDamping > 0)
		MdtBodySetLinearVelocityDamping(m_lpBodyID, m_fltLinearVelocityDamping);

	if(m_fltAngularVelocityDamping > 0)
		MdtBodySetAngularVelocityDamping(m_lpBodyID, m_fltAngularVelocityDamping);

	TRACE_DETAIL_NS("Volume: " + STR(m_fltVolume) + "   XArea: " + STR(m_fltXArea) + "  YArea: " + STR(m_fltYArea) + "  Z: " + STR(m_fltZArea));

	Cone::CreateParts(lpSim, lpStructure);

	TRACE_DEBUG("Ending Parts Creation for " + m_strName);
}

void VsCone::CreateJoints(Simulator *lpSim, Structure *lpStructure)
{
	TRACE_DEBUG("Statring Joints Creation for Part " + m_strName);
	TRACE_DETAIL_NS("PartType: Cone");

	if(m_lpParent && !m_lpJointToParent)
		THROW_PARAM_ERROR(Al_Err_lJointParentMismatch, Al_Err_strJointParentMismatch, "Part", m_strName);

	if(m_lpJointToParent)
		m_lpJointToParent->CreateJoint(lpSim, lpStructure);

	Cone::CreateJoints(lpSim, lpStructure);

	if(m_bIsContactSensor)
		McdModelSetResponseID(m_lpModelID, kMstResponseSensor);

	McdModelSetUserData(m_lpModelID, (void*) this); 

	VsRigidBody::Initialize(lpSim, lpStructure);

	TRACE_DEBUG("Ending Joints Creation for Part " + m_strName);
}

void VsCone::ResetNode(Simulator *lpSim, Structure *lpStructure)
{VsRigidBody::ResetNode(lpSim, lpStructure);}

void VsCone::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	Cone::StepSimulation(lpSim, lpStructure);
	VsRigidBody::CollectBodyData(lpSim);

	if(lpSim->SimulateHydrodynamics())
		SimulateHydrodynamics(lpSim, lpStructure);

	TRACE_DETAIL_NS("");
}

void VsCone::EnableCollision(RigidBody *lpBody)
{VsRigidBody::EnableCollision(lpBody);}

void VsCone::DisableCollision(RigidBody *lpBody)
{VsRigidBody::DisableCollision(lpBody);}

float *VsCone::GetDataPointer(string strDataType)
{
	string strType = Std_CheckString(strDataType);
	float *lpData = NULL;

	lpData = Cone::GetDataPointer(strDataType);
	if(lpData) return lpData;

	lpData = VsRigidBody::GetVortexDataPointer(strDataType);
	if(lpData) return lpData;

	THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "RigidBodyID: " + STR(m_strName) + "  DataType: " + strDataType);

	return NULL;
}

void VsCone::AddForce(Simulator *lpSim, float fltPx, float fltPy, float fltPz, float fltFx, float fltFy, float fltFz)
{VsRigidBody::AddBodyForce(lpSim, fltPx, fltPy, fltPz, fltFx, fltFy, fltFz);}

void VsCone::AddTorque(Simulator *lpSim, float fltTx, float fltTy, float fltTz)
{VsRigidBody::AddBodyTorque(lpSim, fltTx, fltTy, fltTz);}


		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

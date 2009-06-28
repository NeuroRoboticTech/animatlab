// VsMesh.cpp: implementation of the VsMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsMesh.h"
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

VsMesh::VsMesh()
{
}

VsMesh::~VsMesh()
{

try
{
	//We do not delete the vortex stuff. It gets deleted when we destroy their objects.
	if(m_lpGeometryID) 
	{
		if(m_strCollisionMeshType == "CONVEX")
			McdConvexMeshDestroy(m_lpGeometryID);
		else
		McdGeometryDestroy(m_lpGeometryID);   

		m_lpGeometryID = NULL;
	}

}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VsMesh\r\n", "", -1, FALSE, TRUE);}
}

CStdFPoint VsMesh::GetCurrentPosition()
{
	m_oAbsPosition.x = m_lpBodyID->bodyTM[3][0];
	m_oAbsPosition.y = m_lpBodyID->bodyTM[3][1];
	m_oAbsPosition.z = m_lpBodyID->bodyTM[3][2];

	return m_oAbsPosition;
}

void VsMesh::InitializeContactSensor(Simulator *lpSim)
{
	VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);
	if(!lpVsSim)
		THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

	AnimatLibrary::Environment::ContactSensor *lpSensor = this->ContactSensor();

	RGraphic *lpGraphic;
	MeMatrix4 aryBodyTM;
	MeMatrix4TMMakeIdentity(aryBodyTM);

	if(Std_IsBlank(m_strReceptiveFieldMesh))
		lpGraphic = m_lpGraphic;
	else
		lpGraphic = RGraphicCreate(lpVsSim->RenderContext(), (char *) AnimatLibrary::GetFilePath(lpSim->ProjectPath(), m_strReceptiveFieldMesh).c_str(), 1, 1, 1, m_aryColor, aryBodyTM, 0, 1);

	//for(int iVertex=0; iVertex<lpGraphic->m_pObject->m_nNumVertices; iVertex++)
	//	lpSensor->AddVertex(lpGraphic->m_pVertices[iVertex].m_X, lpGraphic->m_pVertices[iVertex].m_Y, lpGraphic->m_pVertices[iVertex].m_Z);

	if(!Std_IsBlank(m_strReceptiveFieldMesh))
		RGraphicDelete(lpVsSim->RenderContext(), lpGraphic, 0);

	lpSensor->FinishedAddingVertices();
}

void VsMesh::CreateParts(Simulator *lpSim, Structure *lpStructure)
{
	TRACE_DEBUG("Statring Parts Creation for " + m_strName);
	TRACE_DETAIL_NS("PartType: Mesh");

	VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);
	if(!lpVsSim)
		THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

	MeMatrix4 aryRotationTM, aryBodyTM;
	MeMatrix3 aryRotation;

	MeMatrix4TMSetRotationFromEulerAngles(aryRotationTM, m_oRotation.x, m_oRotation.y, m_oRotation.z);
	MeMatrix4TMGetRotation(aryRotation, aryRotationTM) ;
	MeMatrix4TMMakeFromRotationAndPosition(aryBodyTM, aryRotation, m_oAbsPosition.x, m_oAbsPosition.y, m_oAbsPosition.z);

	if(m_strCollisionMeshType == "CONVEX")
		m_lpGeometryID = McduConvexMeshCreateHullFromObj(AnimatLibrary::GetFilePath(lpSim->ProjectPath(), m_strCollisionMesh).c_str(), 1, 0);
	else
	{
    int iVertexCount = 0;
		float *lpVertexPtr = NULL;

		m_lpGeometryID = McduTriangleMeshCreateFromObj(AnimatLibrary::GetFilePath(lpSim->ProjectPath(), m_strCollisionMesh).c_str(), 0, 1, 0, 0, 0, 0, &lpVertexPtr, &iVertexCount, 0);
    McdTriangleMeshBuild(m_lpGeometryID);
	}
	if(!m_lpGeometryID)
		THROW_TEXT_ERROR(Vs_Err_lCreatingGeometry, Vs_Err_strCreatingGeometry, "Body: " + m_strName + " Mesh: " + AnimatLibrary::GetFilePath(lpSim->ProjectPath(), m_strCollisionMesh));

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
		m_lpGraphic = RGraphicLoadASE(lpVsSim->RenderContext(), (char *) AnimatLibrary::GetFilePath(lpSim->ProjectPath(), m_strGraphicsMesh).c_str(), 1, 1, 1, m_aryColor, MdtBodyGetTransformPtr(m_lpBodyID)); 
		//m_lpGraphic = RGraphicCreate(lpVsSim->RenderContext(), (char *) AnimatLibrary::GetFilePath(lpSim->ProjectPath(), m_strGraphicsMesh).c_str(), 1, 1, 1, m_aryColor, MdtBodyGetTransformPtr(m_lpBodyID), 0, 1);

	//Lets get the volume and areas
	m_fltVolume = m_oCollisionBoxSize.x * m_oCollisionBoxSize.y * m_oCollisionBoxSize.z;
	m_fltXArea = m_oCollisionBoxSize.y * m_oCollisionBoxSize.z;
	m_fltYArea = m_oCollisionBoxSize.x * m_oCollisionBoxSize.z;
	m_fltZArea = m_oCollisionBoxSize.y * m_oCollisionBoxSize.x;

	if(m_bFreeze)
		MstModelFreeze(m_lpModelID);

	if(!Std_IsBlank(m_strTexture))
		RGraphicSetTexture(lpVsSim->RenderContext(), m_lpGraphic, TextureFile(lpSim, m_strTexture).c_str());

	if(m_fltLinearVelocityDamping > 0)
		MdtBodySetLinearVelocityDamping(m_lpBodyID, m_fltLinearVelocityDamping);

	if(m_fltAngularVelocityDamping > 0)
		MdtBodySetAngularVelocityDamping(m_lpBodyID, m_fltAngularVelocityDamping);

	TRACE_DETAIL_NS("Volume: " + STR(m_fltVolume) + "   XArea: " + STR(m_fltXArea) + "  YArea: " + STR(m_fltYArea) + "  Z: " + STR(m_fltZArea));

	Mesh::CreateParts(lpSim, lpStructure);

	TRACE_DEBUG("Ending Parts Creation for " + m_strName);
}

void VsMesh::CreateJoints(Simulator *lpSim, Structure *lpStructure)
{
	TRACE_DEBUG("Statring Joints Creation for Part " + m_strName);
	TRACE_DETAIL_NS("PartType: Rectangle");

	if(m_lpParent && !m_lpJointToParent)
		THROW_PARAM_ERROR(Al_Err_lJointParentMismatch, Al_Err_strJointParentMismatch, "Part", m_strName);

	if(m_lpJointToParent)
		m_lpJointToParent->CreateJoint(lpSim, lpStructure);

	Mesh::CreateJoints(lpSim, lpStructure);

	if(m_bIsContactSensor)
		McdModelSetResponseID(m_lpModelID, kMstResponseSensor);

	McdModelSetUserData(m_lpModelID, (void*) this); 

	VsRigidBody::Initialize(lpSim, lpStructure);

	TRACE_DEBUG("Ending Joints Creation for Part " + m_strName);
}

void VsMesh::ResetNode(Simulator *lpSim, Structure *lpStructure)
{VsRigidBody::ResetNode(lpSim, lpStructure);}

void VsMesh::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	Mesh::StepSimulation(lpSim, lpStructure);
	VsRigidBody::CollectBodyData(lpSim);

	if(lpSim->SimulateHydrodynamics())
		SimulateHydrodynamics(lpSim, lpStructure);
}

void VsMesh::EnableCollision(RigidBody *lpBody)
{VsRigidBody::EnableCollision(lpBody);}

void VsMesh::DisableCollision(RigidBody *lpBody)
{VsRigidBody::DisableCollision(lpBody);}

float *VsMesh::GetDataPointer(string strDataType)
{
	string strType = Std_CheckString(strDataType);
	float *lpData = NULL;

	lpData = Mesh::GetDataPointer(strDataType);
	if(lpData) return lpData;

	lpData = VsRigidBody::GetVortexDataPointer(strDataType);
	if(lpData) return lpData;

	THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "RigidBodyID: " + STR(m_strName) + "  DataType: " + strDataType);

	return NULL;
}

void VsMesh::AddForce(Simulator *lpSim, float fltPx, float fltPy, float fltPz, float fltFx, float fltFy, float fltFz)
{VsRigidBody::AddBodyForce(lpSim, fltPx, fltPy, fltPz, fltFx, fltFy, fltFz);}

void VsMesh::AddTorque(Simulator *lpSim, float fltTx, float fltTy, float fltTz)
{VsRigidBody::AddBodyTorque(lpSim, fltTx, fltTy, fltTz);}

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

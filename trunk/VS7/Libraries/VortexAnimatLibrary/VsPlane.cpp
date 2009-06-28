// VsPlane.cpp: implementation of the VsPlane class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsPlane.h"
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

VsPlane::VsPlane()
{
	m_aryGroundTM[0][0] = 1;
	m_aryGroundTM[0][1] = 0;
	m_aryGroundTM[0][2] = 0;
	m_aryGroundTM[0][3] = 0;
		
	m_aryGroundTM[1][0] = 0;
	m_aryGroundTM[1][1] = 0;
	m_aryGroundTM[1][2] = -1;
	m_aryGroundTM[1][3] = 0;
		
	m_aryGroundTM[2][0] = 0;
	m_aryGroundTM[2][1] = 1;
	m_aryGroundTM[2][2] = 0;
	m_aryGroundTM[2][3] = 0;
		
	m_aryGroundTM[3][0] = 0;
	m_aryGroundTM[3][1] = 0;
	m_aryGroundTM[3][2] = 0;
	m_aryGroundTM[3][3] = 1;
}

VsPlane::~VsPlane()
{
}

void VsPlane::CreateParts(Simulator *lpSim, Structure *lpStructure)
{
	TRACE_DEBUG("Statring Parts Creation for " + m_strName);
	TRACE_DETAIL_NS("PartType: Plane");

	VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);
	if(!lpVsSim)
		THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

	int iImageWidth, iImageHeight;
	float fltImageDx, fltImageDy;
	if(Std_IsBlank(m_strHeightFieldImage))
	{
		m_aryGroundTM[3][1] = m_fltHeight;
		m_lpGeometryID = McdPlaneCreate();
	}
	else
	{
		m_lpGeometryID = McduRGHeighFieldCreateFromBmpEx((char *) AnimatLibrary::GetFilePath(lpSim->ProjectPath(), m_strHeightFieldImage).c_str(), 
													m_ptMapLocation.x, m_ptMapLocation.y, m_ptMapSize.x, m_ptMapSize.y, m_fltMapScale, m_fltMapScale, m_fltMapScale, 
													m_fltHeight, &iImageWidth, &iImageHeight, &fltImageDx, &fltImageDy);
		if(!m_lpGeometryID)
			THROW_PARAM_ERROR(Vs_Err_lCreatingGeometry, Vs_Err_strCreatingGeometry, "BodyID", m_strName);

		McdRGHeightFieldSetForceTriangleNormalTolerance(m_lpGeometryID, 1);
	}

	m_lpModelID = MstFixedModelCreate(lpVsSim->Universe(), m_lpGeometryID, m_aryGroundTM);

	if(Std_IsBlank(m_strHeightFieldImage))
	  m_lpGraphic = RGraphicGroundPlaneCreate(lpVsSim->RenderContext(), 244, 2, m_aryColor, m_aryGroundTM[3][1]);
	else
	{
		m_lpGraphic = RGraphicRGHeightfieldCreate(lpVsSim->RenderContext(), iImageWidth, iImageHeight, fltImageDx, fltImageDy, m_ptMapLocation.x, m_ptMapLocation.y,
													    McdRGHeightFieldGetHeightArray(m_lpGeometryID), m_aryColor, McdModelGetTransformPtr(m_lpModelID));
	}

	if(!Std_IsBlank(m_strTexture))
		RGraphicSetTexture(lpVsSim->RenderContext(), m_lpGraphic, TextureFile(lpSim, m_strTexture).c_str());

	Plane::CreateParts(lpSim, lpStructure);

	McdModelSetUserData(m_lpModelID, (void*) this); 

	TRACE_DEBUG("Ending Parts Creation for " + m_strName);
}


void VsPlane::EnableCollision(RigidBody *lpBody)
{VsRigidBody::EnableCollision(lpBody);}

void VsPlane::DisableCollision(RigidBody *lpBody)
{VsRigidBody::DisableCollision(lpBody);}


		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

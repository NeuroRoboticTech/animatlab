// VsRigidBody.cpp: implementation of the VsRigidBody class.
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

VsRigidBody::VsRigidBody()
{
	m_lpGeometryID = NULL;
	m_lpModelID = NULL;
	m_lpBodyID = NULL;
	m_lpGraphic = NULL;
	m_fltBuoyancy = 0;
	m_fltReportBuoyancy = 0;
	m_fltMass = 0;
	m_fltReportMass = 0;
	m_fltReportVolume = 0;

	m_fltLinearDragPrecomp = 0;
	m_fltAngularDragPrecomp = 0;
	m_fltLinearMassPrecomp = 0;
	m_fltAngularMassPrecomp = 0;

	m_aryDragForce[0] = 0;
	m_aryDragForce[1] = 0;
	m_aryDragForce[2] = 0;
	m_aryDragForce[3] = 0;

	m_aryDragTorque[0] = 0;
	m_aryDragTorque[1] = 0;
	m_aryDragTorque[2] = 0;
	m_aryDragTorque[3] = 0;

	m_aryAddedMassForce[0] = 0;
	m_aryAddedMassForce[1] = 0;
	m_aryAddedMassForce[2] = 0;
	m_aryAddedMassForce[3] = 0;

	m_aryAddedMassTorque[0] = 0;
	m_aryAddedMassTorque[1] = 0;
	m_aryAddedMassTorque[2] = 0;
	m_aryAddedMassTorque[3] = 0;

	m_aryArea[0] = 0;
	m_aryArea[1] = 0;
	m_aryArea[2] = 0;
	m_aryArea[3] = 0;

	m_bCollectData = FALSE;
	m_bCollectRotation = FALSE;

	m_lpThis = NULL;

	for(int i=0; i<3; i++)
	{
		m_vPosition[i] = 0;
		m_vRotation[i] = 0;
		m_vTorque[i] = 0;
		m_vForce[i] = 0;
		m_vLinearVelocity[i] = 0;
		m_vAngularVelocity[i] = 0;
		m_vReportDragForce[i] = 0;
		m_vReportDragTorque[i] = 0;
		m_vReportAddedMassForce[i] = 0;
		m_vReportAddedMassTorque[i] = 0;
		m_vLinearAcceleration[i] = 0;
		m_vAngularAcceleration[i] = 0;
	}

}

VsRigidBody::~VsRigidBody()
{

try
{
	//We do not delete the vortex stuff. It gets deleted when we destroy their objects.
	if(m_lpGeometryID) 
		McdGeometryDestroy(m_lpGeometryID);   

	if(m_lpModelID)
		McdModelDestroy(m_lpModelID);   

	if(m_lpBodyID)
		MdtBodyDestroy(m_lpBodyID);   

	//Graphics are destroyed when the render context is destroyed.
	m_lpGraphic = NULL;
	m_lpGeometryID = NULL;
	m_lpModelID = NULL;
	m_lpBodyID = NULL;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VsRigidBody\r\n", "", -1, FALSE, TRUE);}
}

McdGeometryID VsRigidBody::GeometryID()
{return m_lpGeometryID;}

McdModelID VsRigidBody::ModelID()
{return m_lpModelID;}

MdtBodyID VsRigidBody::BodyID()
{return m_lpBodyID;}

RGraphic *VsRigidBody::Graphic()
{return m_lpGraphic;}

void VsRigidBody::Copy(VsRigidBody *lpOrig)
{
	m_lpGeometryID = lpOrig->m_lpGeometryID;
	m_lpModelID = lpOrig->m_lpModelID;
	m_lpBodyID = lpOrig->m_lpBodyID;
	m_lpGraphic = lpOrig->m_lpGraphic;
}

void VsRigidBody::Initialize(Simulator *lpSim, Structure *lpStructure)
{
	m_lpThis = dynamic_cast<RigidBody *>(this);

	//Fluid Density is in the units being used. So if the user set 1 g/cm^3 
	//and the units were grams and decimeters then density would be 1000 g/dm^3

	//Simulate buoyancy
	//Fb = Pl * (Ms/Ps) * G
	m_fltMass = MdtBodyGetMass(m_lpBodyID);  //mass is in unit scale values. (ie. grams)
	float fltVolume = m_fltMass/m_lpThis->Density();   //volume is in unit scale values. (ie. decimeters^3)

	//We need to convert the mass to grams and the volume to cubic meters for reporting purposes.
	m_fltReportMass = m_fltMass*lpSim->MassUnits();
	m_fltReportVolume = fltVolume*(pow(lpSim->DistanceUnits(), 3));

	if(lpSim->SimulateHydrodynamics())
	{
		m_fltBuoyancy = -(lpSim->FluidDensity() * fltVolume * lpSim->Gravity());
		m_fltReportBuoyancy = m_fltBuoyancy * lpSim->MassUnits() * lpSim->DistanceUnits();
	}
	else
	{
		m_fltBuoyancy = 0;
		m_fltReportBuoyancy = 0;
	}

	m_fltLinearDragPrecomp = 0.5 * m_lpThis->Cd() * lpSim->FluidDensity();
	m_fltAngularDragPrecomp = m_lpThis->Cdr() * lpSim->FluidDensity();
	
	m_fltLinearMassPrecomp = m_lpThis->Ca() * lpSim->FluidDensity() * fltVolume * lpSim->InverseMassUnits();
	m_fltAngularMassPrecomp = m_lpThis->Car() * lpSim->FluidDensity() * fltVolume * lpSim->InverseMassUnits();

	m_aryArea[0] = m_lpThis->XArea();
	m_aryArea[1] = m_lpThis->YArea();
	m_aryArea[2] = m_lpThis->ZArea();
	m_aryArea[3] = 0;

	if(m_lpThis->ContactSensor())
		InitializeContactSensor(lpSim);
}

void VsRigidBody::InitializeContactSensor(Simulator *lpSim)
{
	//ContactSensor *lpSensor = m_lpThis->ContactSensor();

	////Lets loop through the list of vertices for this objects receptive field mesh and add it to the contact sensor.
	////the contact sensor will only keep unique vertices. Duplicates will be discared.
	//for(int iVertex=0; iVertex<m_lpGraphic->m_pObject->m_nNumVertices; iVertex++)
	//	lpSensor->AddVertex(m_lpGraphic->m_pVertices[iVertex].m_X, m_lpGraphic->m_pVertices[iVertex].m_Y, m_lpGraphic->m_pVertices[iVertex].m_Z);
}

void VsRigidBody::ProcessContacts(Simulator *lpSim)
{
	ContactSensor *lpSensor = m_lpThis->ContactSensor();

	lpSensor->ClearCurrents();

	MdtCListNode *node;
	MdtBaseConstraint *cp;
	MeVector3 vWorldPos, vBodyPos;
	float fltForceMag = 0, fltForceMag1 = 0, fltForceMag2 = 0, fltForceMag3 =0, fltForceMag4  = 0;

	node = m_lpBodyID->constraintList;
	while (node != 0)
	{
			cp = node->constraint;

			if(cp->bclH.tag == MdtBclCONTACT) 
			{ 
				MdtContactGetPosition(cp, vWorldPos);
				WorldToBodyCoords(vWorldPos, vBodyPos);

				fltForceMag1 = V3_MAG(cp->head.resultForce[0]);
				fltForceMag2 = V3_MAG(cp->head.resultForce[1]);
				fltForceMag3 = V3_MAG(cp->head.resultForce[2]);
				fltForceMag4 = V3_MAG(cp->head.resultForce[3]);

				fltForceMag = max(fltForceMag1, fltForceMag2);
				fltForceMag = max(fltForceMag, fltForceMag3);
				fltForceMag = max(fltForceMag, fltForceMag4);

				fltForceMag = fltForceMag * lpSim->MassUnits() * lpSim->DistanceUnits();
				lpSensor->ProcessContact(lpSim, vBodyPos, fltForceMag);
			}
 
			node = node->next;
	}
}

void VsRigidBody::CollectBodyData(Simulator *lpSim)
{
	if(m_bCollectData && m_lpBodyID)
	{
		m_vPosition[0] = m_lpBodyID->bodyTM[3][0] * lpSim->DistanceUnits();
		m_vPosition[1] = m_lpBodyID->bodyTM[3][1] * lpSim->DistanceUnits();
		m_vPosition[2] = m_lpBodyID->bodyTM[3][2] * lpSim->DistanceUnits();

		m_vLinearVelocity[0] = m_lpBodyID->keaBody.vel[0] * lpSim->DistanceUnits();
		m_vLinearVelocity[1] = m_lpBodyID->keaBody.vel[1] * lpSim->DistanceUnits();
		m_vLinearVelocity[2] = m_lpBodyID->keaBody.vel[2] * lpSim->DistanceUnits();

		m_vAngularVelocity[0] = m_lpBodyID->keaBody.velrot[0];
		m_vAngularVelocity[1] = m_lpBodyID->keaBody.velrot[1];
		m_vAngularVelocity[2] = m_lpBodyID->keaBody.velrot[2];

		m_vForce[0] = m_lpBodyID->lastForce[0] * lpSim->MassUnits() * lpSim->DistanceUnits();
		m_vForce[1] = m_lpBodyID->lastForce[1] * lpSim->MassUnits() * lpSim->DistanceUnits();
		m_vForce[2] = m_lpBodyID->lastForce[2] * lpSim->MassUnits() * lpSim->DistanceUnits();

		m_vTorque[0] = m_lpBodyID->lastTorque[0] * lpSim->MassUnits() * lpSim->DistanceUnits() * lpSim->DistanceUnits();
		m_vTorque[1] = m_lpBodyID->lastTorque[1] * lpSim->MassUnits() * lpSim->DistanceUnits() * lpSim->DistanceUnits();
		m_vTorque[2] = m_lpBodyID->lastTorque[2] * lpSim->MassUnits() * lpSim->DistanceUnits() * lpSim->DistanceUnits();

		m_vReportDragForce[0] = m_aryDragForce[0] * lpSim->MassUnits() * lpSim->DistanceUnits();
		m_vReportDragForce[1] = m_aryDragForce[1] * lpSim->MassUnits() * lpSim->DistanceUnits();
		m_vReportDragForce[2] = m_aryDragForce[2] * lpSim->MassUnits() * lpSim->DistanceUnits();

		m_vReportDragTorque[0] = m_aryDragTorque[0] * lpSim->MassUnits() * lpSim->DistanceUnits() * lpSim->DistanceUnits();
		m_vReportDragTorque[1] = m_aryDragTorque[1] * lpSim->MassUnits() * lpSim->DistanceUnits() * lpSim->DistanceUnits();
		m_vReportDragTorque[2] = m_aryDragTorque[2] * lpSim->MassUnits() * lpSim->DistanceUnits() * lpSim->DistanceUnits();

		m_vReportAddedMassForce[0] = m_aryAddedMassForce[0] * lpSim->MassUnits() * lpSim->DistanceUnits();
		m_vReportAddedMassForce[1] = m_aryAddedMassForce[1] * lpSim->MassUnits() * lpSim->DistanceUnits();
		m_vReportAddedMassForce[2] = m_aryAddedMassForce[2] * lpSim->MassUnits() * lpSim->DistanceUnits();

		m_vReportAddedMassTorque[0] = m_aryAddedMassTorque[0] * lpSim->MassUnits() * lpSim->DistanceUnits() * lpSim->DistanceUnits();
		m_vReportAddedMassTorque[1] = m_aryAddedMassTorque[1] * lpSim->MassUnits() * lpSim->DistanceUnits() * lpSim->DistanceUnits();
		m_vReportAddedMassTorque[2] = m_aryAddedMassTorque[2] * lpSim->MassUnits() * lpSim->DistanceUnits() * lpSim->DistanceUnits();

		MeVector4 aryLinearAccel, aryAngularAccel;
		MdtBodyGetLinearAcceleration(m_lpBodyID, aryLinearAccel);
		MdtBodyGetAngularAcceleration(m_lpBodyID, aryAngularAccel);

		m_vLinearAcceleration[0] = aryLinearAccel[0] * lpSim->DistanceUnits();
		m_vLinearAcceleration[1] = aryLinearAccel[1] * lpSim->DistanceUnits();
		m_vLinearAcceleration[2] = aryLinearAccel[2] * lpSim->DistanceUnits();

		m_vAngularAcceleration[0] = aryAngularAccel[0];
		m_vAngularAcceleration[1] = aryAngularAccel[1];
		m_vAngularAcceleration[2] = aryAngularAccel[2];

		if(m_bCollectRotation)
			MeMatrix4TMGetEulerAngles (m_lpBodyID->bodyTM, m_vRotation);
	}

	if(m_lpThis->ContactSensor()) 
		ProcessContacts(lpSim);
}

void VsRigidBody::SimulateHydrodynamics(Simulator *lpSim, Structure *lpStructure)
{
	//Only simulate the hydrodynamics if this object is actually in the water.
	if(lpSim->IsInWater( m_lpBodyID->bodyTM[3][1]))
	{
		MdtBodyAddForce(m_lpBodyID, 0, m_fltBuoyancy, 0);
		TRACE_DETAIL_NS("BuoyancyForce: (0, " + STR(m_fltBuoyancy) + ", 0)");

		MeVector4 aryLinearVel, aryAngularVel, aryLinearAccel, aryAngularAccel;
		MdtBodyGetLinearVelocity(m_lpBodyID, aryLinearVel);
		MdtBodyGetAngularVelocity(m_lpBodyID, aryAngularVel);
		MdtBodyGetLinearAcceleration(m_lpBodyID, aryLinearAccel);
		MdtBodyGetAngularAcceleration(m_lpBodyID, aryAngularAccel);

		//Scale the velocity and accelerations
		aryLinearVel[3] = 1;
		aryAngularVel[3] = 1;
		aryLinearAccel[3] = 1;
		aryAngularAccel[3] = 1;

		MeMatrix4 aryBodyTM;
		MeVector4 aryTranslatedArea;
		
		MdtBodyGetTransform(m_lpBodyID, aryBodyTM);
		MeMatrix4TMInvert(aryBodyTM);
		MeMatrix4MultiplyVector(aryTranslatedArea, aryBodyTM, m_aryArea);

		//Fd = 0.5*Cd*p*A*v^2
		m_aryDragForce[0] = m_fltLinearDragPrecomp * fabs(aryTranslatedArea[0]) * (aryLinearVel[0] * aryLinearVel[0]) * -Std_Sign(aryLinearVel[0]);
		m_aryDragForce[1] = m_fltLinearDragPrecomp * fabs(aryTranslatedArea[1]) * (aryLinearVel[1] * aryLinearVel[1]) * -Std_Sign(aryLinearVel[1]);
		m_aryDragForce[2] = m_fltLinearDragPrecomp * fabs(aryTranslatedArea[2]) * (aryLinearVel[2] * aryLinearVel[2]) * -Std_Sign(aryLinearVel[2]);
		m_aryDragForce[3] = 1;

		if(m_aryDragForce[0] > lpSim->MaxHydroForce()) m_aryDragForce[0] = lpSim->MaxHydroForce();
		if(m_aryDragForce[0] < -lpSim->MaxHydroForce()) m_aryDragForce[0] = -lpSim->MaxHydroForce();

		if(m_aryDragForce[1] > lpSim->MaxHydroForce()) m_aryDragForce[1] = lpSim->MaxHydroForce();
		if(m_aryDragForce[1] < -lpSim->MaxHydroForce()) m_aryDragForce[1] = -lpSim->MaxHydroForce();

		if(m_aryDragForce[2] > lpSim->MaxHydroForce()) m_aryDragForce[2] = lpSim->MaxHydroForce();
		if(m_aryDragForce[2] < -lpSim->MaxHydroForce()) m_aryDragForce[2] = -lpSim->MaxHydroForce();

		MdtBodyAddForce(m_lpBodyID, m_aryDragForce[0], m_aryDragForce[1], m_aryDragForce[2]);

		//TRACE_DETAIL_NS("DragForce: (" + STR(m_aryDragForce[0]) + ", " + STR(m_aryDragForce[1]) + ", " + STR(m_aryDragForce[2]) + ")");

		//Td = Cdr*p*A*w^2
		m_aryDragTorque[0] = m_fltAngularDragPrecomp * fabs(aryTranslatedArea[0]) * (aryAngularVel[0] * aryAngularVel[0]) * -Std_Sign(aryAngularVel[0]);
		m_aryDragTorque[1] = m_fltAngularDragPrecomp * fabs(aryTranslatedArea[1]) * (aryAngularVel[1] * aryAngularVel[1]) * -Std_Sign(aryAngularVel[1]);
		m_aryDragTorque[2] = m_fltAngularDragPrecomp * fabs(aryTranslatedArea[2]) * (aryAngularVel[2] * aryAngularVel[2]) * -Std_Sign(aryAngularVel[2]);
		m_aryDragTorque[3] = 1;

		if(m_aryDragTorque[0] > lpSim->MaxHydroTorque()) m_aryDragTorque[0] = lpSim->MaxHydroTorque();
		if(m_aryDragTorque[0] < -lpSim->MaxHydroTorque()) m_aryDragTorque[0] = -lpSim->MaxHydroTorque();

		if(m_aryDragTorque[1] > lpSim->MaxHydroTorque()) m_aryDragTorque[1] = lpSim->MaxHydroTorque();
		if(m_aryDragTorque[1] < -lpSim->MaxHydroTorque()) m_aryDragTorque[1] = -lpSim->MaxHydroTorque();

		if(m_aryDragTorque[2] > lpSim->MaxHydroTorque()) m_aryDragTorque[2] = lpSim->MaxHydroTorque();
		if(m_aryDragTorque[2] < -lpSim->MaxHydroTorque()) m_aryDragTorque[2] = -lpSim->MaxHydroTorque();

		MdtBodyAddTorque(m_lpBodyID, m_aryDragTorque[0], m_aryDragTorque[1], m_aryDragTorque[2]);

		//TRACE_DETAIL_NS("DragForce: (" + STR(m_aryDragTorque[0]) + ", " + STR(m_aryDragTorque[1]) + ", " + STR(m_aryDragTorque[2]) + ")");

		//Added mass force
		m_aryAddedMassForce[0] = m_fltLinearMassPrecomp * aryLinearAccel[0];
		m_aryAddedMassForce[1] = m_fltLinearMassPrecomp * aryLinearAccel[1];
		m_aryAddedMassForce[2] = m_fltLinearMassPrecomp * aryLinearAccel[2];
		m_aryAddedMassForce[3] = 1;

		if(m_aryAddedMassForce[0] > lpSim->MaxHydroForce()) m_aryAddedMassForce[0] = lpSim->MaxHydroForce();
		if(m_aryAddedMassForce[0] < -lpSim->MaxHydroForce()) m_aryAddedMassForce[0] = -lpSim->MaxHydroForce();

		if(m_aryAddedMassForce[1] > lpSim->MaxHydroForce()) m_aryAddedMassForce[1] = lpSim->MaxHydroForce();
		if(m_aryAddedMassForce[1] < -lpSim->MaxHydroForce()) m_aryAddedMassForce[1] = -lpSim->MaxHydroForce();

		if(m_aryAddedMassForce[2] > lpSim->MaxHydroForce()) m_aryAddedMassForce[2] = lpSim->MaxHydroForce();
		if(m_aryAddedMassForce[2] < -lpSim->MaxHydroForce()) m_aryAddedMassForce[2] = -lpSim->MaxHydroForce();

		//MdtBodyAddForce(m_lpBodyID, m_aryAddedMassForce[0], m_aryAddedMassForce[1], m_aryAddedMassForce[2]);

		//Added mass torque
		m_aryAddedMassTorque[0] = m_fltAngularMassPrecomp * aryAngularAccel[0];
		m_aryAddedMassTorque[1] = m_fltAngularMassPrecomp * aryAngularAccel[1];
		m_aryAddedMassTorque[2] = m_fltAngularMassPrecomp * aryAngularAccel[2];
		m_aryAddedMassTorque[3] = 1;

		if(m_aryAddedMassTorque[0] > lpSim->MaxHydroTorque()) m_aryAddedMassTorque[0] = lpSim->MaxHydroTorque();
		if(m_aryAddedMassTorque[0] < -lpSim->MaxHydroTorque()) m_aryAddedMassTorque[0] = -lpSim->MaxHydroTorque();

		if(m_aryAddedMassTorque[1] > lpSim->MaxHydroTorque()) m_aryAddedMassTorque[1] = lpSim->MaxHydroTorque();
		if(m_aryAddedMassTorque[1] < -lpSim->MaxHydroTorque()) m_aryAddedMassTorque[1] = -lpSim->MaxHydroTorque();

		if(m_aryAddedMassTorque[2] > lpSim->MaxHydroTorque()) m_aryAddedMassTorque[2] = lpSim->MaxHydroTorque();
		if(m_aryAddedMassTorque[2] < -lpSim->MaxHydroTorque()) m_aryAddedMassTorque[2] = -lpSim->MaxHydroTorque();

		//MdtBodyAddForce(m_lpBodyID, m_aryAddedMassTorque[0], m_aryAddedMassTorque[1], m_aryAddedMassTorque[2]);

	}
}

void VsRigidBody::ResetNode(Simulator *lpSim, Structure *lpStructure)
{
	if(m_lpBodyID)
	{
		MdtBodySetTransform(m_lpBodyID, m_aryInitialBodyTM);
		MdtBodySetLinearVelocity(m_lpBodyID, 0, 0, 0);
		MdtBodySetAngularVelocity(m_lpBodyID, 0, 0, 0);
		MdtBodyEnable(m_lpBodyID);
	}
}

void VsRigidBody::EnableCollision(RigidBody *lpBody)
{
	if(!lpBody)
		THROW_ERROR(Al_Err_lBodyNotDefined, Al_Err_strBodyNotDefined);

	VsRigidBody *lpVsBody = dynamic_cast<VsRigidBody *>(lpBody);

	if(!lpVsBody)
		THROW_PARAM_ERROR(Al_Err_lUnableToCastBodyToDesiredType, Al_Err_strUnableToCastBodyToDesiredType, "Type", "VsRigidBody");

	McdSpaceEnablePair(m_lpModelID, lpVsBody->ModelID());
}

void VsRigidBody::DisableCollision(RigidBody *lpBody)
{
	if(!lpBody)
		THROW_ERROR(Al_Err_lBodyNotDefined, Al_Err_strBodyNotDefined);

	VsRigidBody *lpVsBody = dynamic_cast<VsRigidBody *>(lpBody);

	if(!lpVsBody)
		THROW_PARAM_ERROR(Al_Err_lUnableToCastBodyToDesiredType, Al_Err_strUnableToCastBodyToDesiredType, "Type", "VsRigidBody");

	McdSpaceDisablePair(m_lpModelID, lpVsBody->ModelID());
}

float *VsRigidBody::GetVortexDataPointer(string &strDataType)
{
	string strType = Std_CheckString(strDataType);
	RigidBody *lpBody = dynamic_cast<RigidBody *>(this);
	m_bCollectData = TRUE;

	if(strType == "BODYTORQUEX")
		return (&m_vTorque[0]);

	if(strType == "BODYTORQUEY")
		return (&m_vTorque[1]);

	if(strType == "BODYTORQUEZ")
		return (&m_vTorque[2]);

	if(strType == "BODYFORCEX")
		return (&m_vForce[0]);

	if(strType == "BODYFORCEY")
		return (&m_vForce[1]);

	if(strType == "BODYFORCEZ")
		return (&m_vForce[2]);

	if(strType == "BODYPOSITIONX")
		return (&m_vPosition[0]);

	if(strType == "BODYPOSITIONY")
		return (&m_vPosition[1]);

	if(strType == "BODYPOSITIONZ")
		return (&m_vPosition[2]);

	if(strType == "BODYROTATIONX")
		{m_bCollectRotation = TRUE; return (&m_vRotation[0]);}

	if(strType == "BODYROTATIONY")
		{m_bCollectRotation = TRUE; return (&m_vRotation[1]);}

	if(strType == "BODYROTATIONZ")
		{m_bCollectRotation = TRUE; return (&m_vRotation[2]);}

	if(strType == "BODYLINEARVELOCITYX")
		return (&m_vLinearVelocity[0]);

	if(strType == "BODYLINEARVELOCITYY")
		return (&m_vLinearVelocity[1]);

	if(strType == "BODYLINEARVELOCITYZ")
		return (&m_vLinearVelocity[2]);

	if(strType == "BODYANGULARVELOCITYX")
		return (&m_vAngularVelocity[0]);

	if(strType == "BODYANGULARVELOCITYY")
		return (&m_vAngularVelocity[1]);

	if(strType == "BODYANGULARVELOCITYZ")
		return (&m_vAngularVelocity[2]);

	if(strType == "BODYDRAGFORCEX")
		return (&m_vReportDragForce[0]);

	if(strType == "BODYDRAGFORCEY")
		return (&m_vReportDragForce[1]);

	if(strType == "BODYDRAGFORCEZ")
		return (&m_vReportDragForce[2]);

	if(strType == "BODYDRAGTORQUEX")
		return (&m_vReportDragTorque[0]);

	if(strType == "BODYDRAGTORQUEY")
		return (&m_vReportDragTorque[1]);

	if(strType == "BODYDRAGTORQUEZ")
		return (&m_vReportDragTorque[2]);

	if(strType == "BODYBUOYANCY")
		return (&m_fltReportBuoyancy);

	if(strType == "BODYADDEDMASSFORCEX")
		return (&m_vReportAddedMassForce[0]);

	if(strType == "BODYADDEDMASSFORCEY")
		return (&m_vReportAddedMassForce[1]);

	if(strType == "BODYADDEDMASSFORCEZ")
		return (&m_vReportAddedMassForce[2]);

	if(strType == "BODYADDEDMASSTORQUEX")
		return (&m_vReportAddedMassTorque[0]);

	if(strType == "BODYADDEDMASSTORQUEY")
		return (&m_vReportAddedMassTorque[1]);

	if(strType == "BODYADDEDMASSTORQUEZ")
		return (&m_vReportAddedMassTorque[2]);

	if(strType == "BODYLINEARACCELERATIONX")
		return (&m_vLinearAcceleration[0]);

	if(strType == "BODYLINEARACCELERATIONY")
		return (&m_vLinearAcceleration[1]);

	if(strType == "BODYLINEARACCELERATIONZ")
		return (&m_vLinearAcceleration[2]);

	if(strType == "BODYANGULARACCELERATIONX")
		return (&m_vAngularAcceleration[0]);

	if(strType == "BODYANGULARACCELERATIONY")
		return (&m_vAngularAcceleration[1]);

	if(strType == "BODYANGULARACCELERATIONZ")
		return (&m_vAngularAcceleration[2]);

	if(strType == "MASS")
		return &m_fltReportMass;

	if(strType == "VOLUME")
		return &m_fltReportVolume;

	THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "RigidBodyID: " + STR(lpBody->ID()) + "  DataType: " + strDataType);

	return NULL;
}

CStdFPoint VsRigidBody::BodyToWorldCoords(CStdFPoint &oPos)
{
	MeMatrix4 aryParentTM;
	MeVector4 aryFinalPos, aryTranslate;
	aryTranslate[0] = oPos.x;
	aryTranslate[1] = oPos.y;
	aryTranslate[2] = oPos.z;
	aryTranslate[3] = 1.0;

	MdtBodyGetTransform(m_lpBodyID, aryParentTM);
	MeMatrix4MultiplyVector(aryFinalPos, aryParentTM, aryTranslate);

	CStdFPoint oActualPos;
	oActualPos.x = aryFinalPos[0];
	oActualPos.y = aryFinalPos[1];
	oActualPos.z = aryFinalPos[2];
	return oActualPos;
}

void VsRigidBody::WorldToBodyCoords(MeVector3 vWorldPos, MeVector3 &vBodyPos)
{
	MeMatrix4 aryInvParentTM;

  MeMatrix4Copy(aryInvParentTM, *(const MeMatrix4*)&m_lpBodyID->bodyTM);
	MeMatrix4TMInvert(aryInvParentTM);
	MeMatrix4TMTransformVector3(vBodyPos, aryInvParentTM, vWorldPos);
}

McdGeometryID VsRigidBody::CreateConeGeometry(RGraphic *lpGraphic)
{
  RObjectVertex *vtx;
	int iVertex;
	int iVertexCount = lpGraphic->m_pObject->m_nNumVertices;

  MeReal *aryVertices = (MeReal*) malloc ( sizeof(MeReal) * 3 * (iVertexCount) );

  vtx = lpGraphic->m_pVertices;
	for(iVertex=0; iVertex<iVertexCount; iVertex++)
	{
		aryVertices[(3*iVertex)+0] = vtx->m_X;
		aryVertices[(3*iVertex)+1] = vtx->m_Y;
		aryVertices[(3*iVertex)+2] = vtx->m_Z;
		vtx++;
	}

	return McdConvexMeshCreateHull((MeVector3*) aryVertices, iVertexCount, 0); 
}


void VsRigidBody::AddBodyForce(Simulator *lpSim, float fltPx, float fltPy, float fltPz, float fltFx, float fltFy, float fltFz)
{
	if(m_lpBodyID && (fltFx || fltFy || fltFz) )
	{
		fltFx *= (lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits());
		fltFy *= (lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits());
		fltFz *= (lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits());

		MdtBodyAddForceAtPosition(m_lpBodyID, fltFx, fltFy, fltFz, fltPx, fltPy, fltPz);
	}
}

void VsRigidBody::AddBodyTorque(Simulator *lpSim, float fltTx, float fltTy, float fltTz)
{
	if(m_lpBodyID && (fltTx || fltTy || fltTz))
	{
		fltTx *= (lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits() * lpSim->InverseDistanceUnits());
		fltTy *= (lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits() * lpSim->InverseDistanceUnits());
		fltTz *= (lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits() * lpSim->InverseDistanceUnits());

		MdtBodyAddTorque(m_lpBodyID, fltTx, fltTy, fltTz);
	}
}


	}			// Environment
}				//VortexAnimatLibrary

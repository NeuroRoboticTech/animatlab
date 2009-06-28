// VsRigidBody.h: interface for the VsRigidBody class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSRIGIDBODY_H__BE00E72D_B205_450A_9A20_58752ED37EED__INCLUDED_)
#define AFX_VSRIGIDBODY_H__BE00E72D_B205_450A_9A20_58752ED37EED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{

		/*! \brief 
			A common class for all rigid body data specific to vortex.

			\remarks
			This is a common class for the rigid body objects that 
			specifically holds vortex data and methods. I had hoped to
			not have this class. However, it proved necessary. The reason
			is that rigid bodies like the box and cylinder all had common
			data items asociated with them like the m_iBodyID that need
			to be used in various places in order to get things done. 
			When we are in one of the overridden virtual functions we
			need a way to get at these data members for a rigid body.
			If we have this class then we can just do a dynamic cast
			to convert it into a VsRigidBody and access those items that
			way. If we do not have this common class and instead just duplicated
			these items in each of the different VsBox, VsCylinder, etc. 
			classes then we have a problem. If we put virtual accessor 
			functions for those items in the RigidBody class then that
			is not appropriate because the animat library is not supposed 
			to know anything about data elements specific to vortex. 
			What happens if you move to a different physics engine? Do
			you put data elements for that one in RigidBody also. 
			Clearly that will not work as a solution. The only other
			way would be to try and determine the actual type of object
			you have and do a specific cast to that one. So if we are
			looking at the parent of the current rigid body and we have
			it as a RigidBody pointer then we would have to find 
			some way to determine that it was say a VsBox type of object.
			And then we would have to dynamic cast it to VsBox. Again,
			this would be horribly messy with switches and other stuff.
			This was the cleanest solution that I could see. With this
			we just always cast the RigidBody to a VsRigidBody to
			get access to the vortex specific data elements.

			\sa
			RigidBody, VsBox, VsPlane, VsCylinder
		*/

		class VORTEX_PORT VsRigidBody
		{
		protected:
			//The ID for the geometry instance.
			McdGeometryID m_lpGeometryID;

			//The ID for the collision model instance.
			McdModelID m_lpModelID;

			//The ID for the body instance.
			MdtBodyID m_lpBodyID;

			//A pointer to the graphics instance.
			RGraphic *m_lpGraphic;

			RigidBody *m_lpThis;

			//The initial body matrix. This is used to reset the body to the starting position
			MeMatrix4 m_aryInitialBodyTM;

			//Keeps track of the bouyancy force applied to this rigid body at each time step.
			float m_fltBuoyancy;
			float m_fltReportBuoyancy;

			//We need these arrays to store body data that could potentially be charted.
			//this may be scaled so we need to store it in here instead of just using the
			//body data directly from the physics engine.
			BOOL m_bCollectData;
			BOOL m_bCollectRotation;
			MeVector3 m_vPosition;
			MeVector3 m_vRotation;
			MeVector3 m_vLinearVelocity;
			MeVector3 m_vAngularVelocity;
			MeVector3 m_vLinearAcceleration;
			MeVector3 m_vAngularAcceleration;
			MeVector3 m_vForce;
			MeVector3 m_vTorque;
			MeVector3 m_vReportDragForce;
			MeVector3 m_vReportDragTorque;
			MeVector3 m_vReportAddedMassForce;
			MeVector3 m_vReportAddedMassTorque;
			float m_fltMass;
			float m_fltReportMass;
			float m_fltReportVolume;

			float m_fltLinearDragPrecomp;
			float m_fltAngularDragPrecomp;
			float m_fltLinearMassPrecomp;
			float m_fltAngularMassPrecomp;

			MeVector4 m_aryDragForce;
			MeVector4 m_aryDragTorque;
			MeVector4 m_aryAddedMassForce;
			MeVector4 m_aryAddedMassTorque;
			MeVector4 m_aryArea;

			void Copy(VsRigidBody *lpOrig);

			void CollectBodyData(Simulator *lpSim);
			float *GetVortexDataPointer(string &strDataType);
			McdGeometryID CreateConeGeometry(RGraphic *lpGraphic);

			virtual void InitializeContactSensor(Simulator *lpSim);
			virtual void ProcessContacts(Simulator *lpSim);

		public:
			VsRigidBody();
			virtual ~VsRigidBody();

			McdGeometryID GeometryID();
			McdModelID ModelID();
			MdtBodyID BodyID();
			RGraphic *Graphic();

			void Initialize(Simulator *lpSim, Structure *lpStructure);
			void SimulateHydrodynamics(Simulator *lpSim, Structure *lpStructure);
			void ResetNode(Simulator *lpSim, Structure *lpStructure);
			void EnableCollision(RigidBody *lpBody);
			void DisableCollision(RigidBody *lpBody);
			CStdFPoint BodyToWorldCoords(CStdFPoint &oPos);
			void WorldToBodyCoords(MeVector3 vWorldPos, MeVector3 &vBodyPos);

			void AddBodyForce(Simulator *lpSim, float fltPx, float fltPy, float fltPz, float fltFx, float fltFy, float fltFz);
			void AddBodyTorque(Simulator *lpSim, float fltTx, float fltTy, float fltTz);
		};

	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSRIGIDBODY_H__BE00E72D_B205_450A_9A20_58752ED37EED__INCLUDED_)

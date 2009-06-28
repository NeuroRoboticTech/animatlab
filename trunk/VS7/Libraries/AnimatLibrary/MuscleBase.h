// MuscleBase.h: interface for the MuscleBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSCLE_BASE_H__C0EE52F2_D31B_48CD_B5F7_B18EEE81BE72__INCLUDED_)
#define AFX_MUSCLE_BASE_H__C0EE52F2_D31B_48CD_B5F7_B18EEE81BE72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace AnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{
			class ANIMAT_PORT MuscleBase : public RigidBody  
			{
			protected:
				///The maximum tension that this muscle can ever generate. This is an upper limit to prevent unrealistic tension values.
				float m_fltMaxTension;

				///Keeps track of the total stimulation being supplied to this muscle.
				///This is the summation of the firing frequenies from all neurons
				///that are stimulating this muscle.
				float m_fltVm;

				///The derivative of tension at the current time step.
				float m_fltTdot;

				///Tension of the muscle
				float m_fltTension;

				///Tension of the muscle in the last time slice.
				float m_fltPrevTension;

				///Current length of the muscle.
				float m_fltMuscleLength;

				///Length of the muscle in the previous timestep
				float m_fltPrevMuscleLength;

				///The ID's of the attachment points for this muscle. This is used during the load/initialization process.
				CStdArray<string> m_aryAttachmentPointIDs;

				///A pointer to the primary attachment part.
				CStdArray<MuscleAttachment *> m_aryAttachmentPoints;

				virtual void CalculateTension(Simulator *lpSim) = 0;

			public:
				MuscleBase();
				virtual ~MuscleBase();

				float Tension() {return m_fltTension;};

				void Tension(float fltVal)
				{
					if(fltVal < 0)
						THROW_PARAM_ERROR(Al_Err_lForceLessThanZero, Al_Err_strForceLessThanZero, "MuscleID", m_strName);

					m_fltTension = fltVal;
				};

				float MaxTension() {return m_fltMaxTension;};
				float Vm() {return m_fltVm;};
				float Tdot() {return m_fltTdot;};
				float PrevTension() {return m_fltPrevTension;};
				float MuscleLength() {return m_fltMuscleLength;};
				float PrevMuscleLength() {return m_fltPrevMuscleLength;};

				CStdArray<MuscleAttachment *>  AttachmentPoints() {return m_aryAttachmentPoints;};

				virtual void Enabled(BOOL bVal);

				virtual float CalculateMuscleLength(Simulator *lpSim);
				virtual void CalculateInverseDynamics(Simulator *lpSim, float fltLength, float fltVelocity, float fltT, float &fltVm, float &fltA) = 0;

				virtual void CreateParts(Simulator *lpSim, Structure *lpStructure);
				virtual void CreateJoints(Simulator *lpSim, Structure *lpStructure);
				virtual void AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput);

				virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
			};

		}		//Bodies
	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_MUSCLE_BASE_H__C0EE52F2_D31B_48CD_B5F7_B18EEE81BE72__INCLUDED_)

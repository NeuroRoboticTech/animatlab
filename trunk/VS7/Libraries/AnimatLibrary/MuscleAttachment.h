// MuscleAttachment.h: interface for the MuscleAttachment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALMUSCLEATTACHMENT_H__EBC36518_3B2A_4634_9AB6_474F65149FCF__INCLUDED_)
#define AFX_ALMUSCLEATTACHMENT_H__EBC36518_3B2A_4634_9AB6_474F65149FCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace AnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

			/*! \brief 
				Specifies a point on a rigid body where a muscle is to be attached.
			   
				\remarks
				This type of part specifies the position of a muscle attachment point.
				All CAlMuscle objects must specify the ID for two of these types of 
				object to determine where the ends of the muscle connect to the 
				parent and child rigid bodies. 

				\sa
				Body, Joint, CAlBox, CAlPlane, CAlCylinder, 
				CAlCone, CAlMuscle, CAlMuscleAttachment, CAlSphere                                
				 
				\ingroup AnimatLibrary
			*/

			class ANIMAT_PORT MuscleAttachment : public RigidBody  
			{
			protected:
				float m_fltRadius;

				float m_fltReportX;
				float m_fltReportY;
				float m_fltReportZ;

			public:
				MuscleAttachment();
				virtual ~MuscleAttachment();

				virtual CStdFPoint CalculatePosition(Simulator *lpSim);

				virtual void CreateParts(Simulator *lpSim, Structure *lpStructure);
				virtual void CreateJoints(Simulator *lpSim, Structure *lpStructure);
				virtual float *GetDataPointer(string strDataType);
				virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
			};

		}		//Bodies
	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_ALMUSCLEATTACHMENT_H__EBC36518_3B2A_4634_9AB6_474F65149FCF__INCLUDED_)

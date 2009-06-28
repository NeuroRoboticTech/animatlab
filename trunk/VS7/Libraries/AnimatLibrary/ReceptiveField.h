// Plane.h: interface for the Plane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALRECEPTIVE_FIELD_H__8AABAE57_5434_4AEE_9C0B_B494E10A7AAC__INCLUDED_)
#define AFX_ALRECEPTIVE_FIELD_H__8AABAE57_5434_4AEE_9C0B_B494E10A7AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace AnimatLibrary
{
	namespace Environment
	{
		/*! \brief 
			Keeps track of pairs of rigid bodies that should not be allowed to collide.

			\remarks

			\sa
			Body, Joint, CAlBox, Plane, CAlCylinder, 
			CAlCone, CAlMuscle, CAlMuscleAttachment, CAlSphere                                
				
			\ingroup AnimatLibrary
		*/

		class ANIMAT_PORT ReceptiveField : public CStdSerialize 
		{
		public:
			StdVector3 m_vVertex;
			float m_fltCurrent;

			ReceptiveField();
			ReceptiveField(float fltX, float fltY, float fltZ, float fltStim);
			virtual ~ReceptiveField();

			BOOL operator<(ReceptiveField *lpItem);
			BOOL operator>(ReceptiveField *lpItem);
			BOOL operator==(ReceptiveField *lpItem);
			BOOL LessThanThan(float fltX, float fltY, float fltZ);
			BOOL GreaterThanThan(float fltX, float fltY, float fltZ);
			BOOL Equals(float fltX, float fltY, float fltZ);

			virtual void Trace(ostream &oOs);
		};

	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_ALRECEPTIVE_FIELD_H__8AABAE57_5434_4AEE_9C0B_B494E10A7AAC__INCLUDED_)

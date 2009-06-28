// ReceptiveFieldPair.h: interface for the ReceptiveFieldPair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALRECEPTIVE_FIELD_PAIR_H__8AABAE57_5434_4AEE_9C0B_B494E10A7AAC__INCLUDED_)
#define AFX_ALRECEPTIVE_FIELD_PAIR_H__8AABAE57_5434_4AEE_9C0B_B494E10A7AAC__INCLUDED_

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

		class ANIMAT_PORT ReceptiveFieldPair : public CStdSerialize 
		{
		public:
			StdVector3 m_vVertex;
			Node *m_lpTargetNode;
			long m_lTargetNodeID;
			ReceptiveField *m_lpField;

			ReceptiveFieldPair();
			virtual ~ReceptiveFieldPair();

			void Initialize(Simulator *lpSim, Organism *lpOrganism, Node *lpSourceNode, string strTargetModule);
			void ReceptiveFieldPair::StepSimulation(Simulator *lpSim, Structure *lpStructure);

			virtual void Trace(ostream &oOs);
			virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		};

	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_ALRECEPTIVE_FIELD_PAIR_H__8AABAE57_5434_4AEE_9C0B_B494E10A7AAC__INCLUDED_)

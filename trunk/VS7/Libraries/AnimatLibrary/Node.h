// Node.h: interface for the Adapter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)
#define AFX_NODE_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

/*! \brief 
   xxxx.

   \remarks
   xxxx
		 
   \sa
	 xxx
	 
	 \ingroup AnimatLibrary
*/

namespace AnimatLibrary
{

	class ANIMAT_PORT Node : public CStdSerialize 
	{
	protected:
			///The unique Id for this body. It is unique for each structure, 
			///but not across structures. So you could have two rigid bodies with the
			///same ID in two different organisms.
			string m_strID;  

			///The type for this object. Examples are Box, Plane, Neuron, etc.. 
			string m_strType;  

			///The name for this body. 
			string m_strName;  

			///Determines if this node is enabled. This will only have any effect if this node can be disabled.
			///The majority of nodes, like rigid bodies, can not be disabled.
			BOOL m_bEnabled;

			///Keeps track of the enabled state before this node is killed.
			///If the node is killed and then turned on again we can reset the
			///enabled state to what it was before it was killed.
			BOOL m_bEnabledMem;

			///This is for reporting purposes.
			float m_fltEnabled;

			virtual void ResetNode(Simulator *lpSim, Structure *lpStructure);
			virtual void UpdateData(Simulator *lpSim, Structure *lpStructure);

	public:
		Node();
		virtual ~Node();

		string ID() {return m_strID;};
		void ID(string strValue) {m_strID = strValue;};

		string Name() {return m_strName;};
		void Name(string strValue) {m_strName = strValue;};

		string Type() {return m_strType;};

		virtual BOOL Enabled() {return m_bEnabled;};
		virtual void Enabled(BOOL bValue) 
		{
			m_bEnabled = bValue;
			m_fltEnabled = (float) m_bEnabled;
		};

		virtual void Kill(Simulator *lpSim, Organism *lpOrganism, BOOL bState = TRUE);
		virtual void Reset(Simulator *lpSim, Structure *lpStructure);

		virtual void AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput) = 0;
		virtual void AttachSourceAdapter(Simulator *lpSim, Structure *lpStructure, Adapter *lpAdapter);
		virtual void AttachTargetAdapter(Simulator *lpSim, Structure *lpStructure, Adapter *lpAdapter);
		virtual float *GetDataPointer(string strDataType) = 0;
		virtual void *GetDataItem(string strItemType, string strID, BOOL bThrowError = TRUE); 
		virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure) = 0;
		virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml) = 0;
		virtual void Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml) = 0;
	};

}				//AnimatLibrary

#endif // !defined(AFX_NODE_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)

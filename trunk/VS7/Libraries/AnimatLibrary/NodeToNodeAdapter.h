// NodeToNodeAdapter.h: interface for the NodeToNodeAdapter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_TO_NODE_ADAPTER_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)
#define AFX_NODE_TO_NODE_ADAPTER_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_

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
	namespace Adapters
	{

		class ANIMAT_PORT NodeToNodeAdapter : public Adapter 
		{
		protected:
			string m_strSourceModule;
			long m_lSourceNodeID;

			string m_strTargetModule;
			long m_lTargetNodeID;

		public:
			NodeToNodeAdapter();
			virtual ~NodeToNodeAdapter();

			virtual void Initialize(Simulator *lpSim, Structure *lpStructure);
			virtual string SourceModule();
			virtual string TargetModule();

			//Node Overrides
			virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		};

	}			//Adapters
}				//AnimatLibrary

#endif // !defined(AFX_NODE_TO_NODE_ADAPTER_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)

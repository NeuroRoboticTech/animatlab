// NodeToPhysicalAdapter.h: interface for the NodeToPhysicalAdapter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_TO_PHYSICAL_ADAPTER_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)
#define AFX_NODE_TO_PHYSICAL_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_

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

		class ANIMAT_PORT NodeToPhysicalAdapter : public Adapter 
		{
		protected:
			string m_strSourceModule;
			long m_lSourceNodeID;

			string m_strTargetBodyType;
			string m_strTargetBodyID;

		public:
			NodeToPhysicalAdapter();
			virtual ~NodeToPhysicalAdapter();

			virtual void Initialize(Simulator *lpSim, Structure *lpStructure);
			virtual string SourceModule();
			virtual string TargetModule();

			//Node Overrides
			virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		};

	}			//Adapters
}				//AnimatLibrary

#endif // !defined(AFX_NODE_TO_PHYSICAL_ADAPTER_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)

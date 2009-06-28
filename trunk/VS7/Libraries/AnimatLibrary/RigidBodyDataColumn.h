// RigidBodyDataColumn.h: interface for the RigidBodyDataColumn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIGIDBODYDATACOLUMN_H__FB16C766_19B2_4ADD_BE99_62E6C1FB229B__INCLUDED_)
#define AFX_RIGIDBODYDATACOLUMN_H__FB16C766_19B2_4ADD_BE99_62E6C1FB229B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{
	namespace Charting
	{
		namespace DataColumns
		{

			class ANIMAT_PORT RigidBodyDataColumn : public DataColumn  
			{
			protected:
				string m_strStructureID;
				string m_strRigidBodyID;
				string m_strDataType;

			public:
				RigidBodyDataColumn();
				virtual ~RigidBodyDataColumn();

				virtual void Initialize(Simulator *lpSim);
				virtual void Load(Simulator *lpSim, CStdXml &oXml);
			};

		}		//DataColumns
	}			//Charting
}				//AnimatLibrary

#endif // !defined(AFX_RIGIDBODYDATACOLUMN_H__FB16C766_19B2_4ADD_BE99_62E6C1FB229B__INCLUDED_)

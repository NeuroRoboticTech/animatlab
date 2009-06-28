// JointDataColumn.h: interface for the JointDataColumn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOINTDATACOLUMN_H__FB16C766_19B2_4ADD_BE99_62E6C1FB229B__INCLUDED_)
#define AFX_JOINTDATACOLUMN_H__FB16C766_19B2_4ADD_BE99_62E6C1FB229B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{
	namespace Charting
	{
		namespace DataColumns
		{

			class ANIMAT_PORT JointDataColumn : public DataColumn  
			{
			protected:
				string m_strStructureID;
				string m_strJointID;
				string m_strDataType;

			public:
				JointDataColumn();
				virtual ~JointDataColumn();

				virtual void Initialize(Simulator *lpSim);
				virtual void Load(Simulator *lpSim, CStdXml &oXml);
			};

		}		//DataColumns
	}			//Charting
}				//AnimatLibrary

#endif // !defined(AFX_JOINTDATACOLUMN_H__FB16C766_19B2_4ADD_BE99_62E6C1FB229B__INCLUDED_)

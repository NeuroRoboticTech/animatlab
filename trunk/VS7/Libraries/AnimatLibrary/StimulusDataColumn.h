// StimulusDataColumn.h: interface for the StimulusDataColumn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STIMULUSDATACOLUMN_H__FB16C766_19B2_4ADD_BE99_62E6C1FB229B__INCLUDED_)
#define AFX_STIMULUSDATACOLUMN_H__FB16C766_19B2_4ADD_BE99_62E6C1FB229B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{
	namespace Charting
	{
		namespace DataColumns
		{

			class ANIMAT_PORT StimulusDataColumn : public DataColumn  
			{
			protected:
				string m_strStimulusID;
				string m_strDataType;
				float m_fltInternalValue;

			public:
				StimulusDataColumn();
				virtual ~StimulusDataColumn();

				virtual void Initialize(Simulator *lpSim);
				virtual void Load(Simulator *lpSim, CStdXml &oXml);
			};

		}		//DataColumns
	}			//Charting
}				//AnimatLibrary

#endif // !defined(AFX_JOINTDATACOLUMN_H__FB16C766_19B2_4ADD_BE99_62E6C1FB229B__INCLUDED_)

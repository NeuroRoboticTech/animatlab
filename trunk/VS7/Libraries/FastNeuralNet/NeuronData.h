// NeuronData.h: interface for the NeuronData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NLNEURONDATACOLUMN_H__84D5E187_145D_401E_89AE_5FD5F2CB17E7__INCLUDED_)
#define AFX_NLNEURONDATACOLUMN_H__84D5E187_145D_401E_89AE_5FD5F2CB17E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace FastNeuralNet
{
	namespace DataColumns
	{

		class FAST_NET_PORT NeuronData : public DataColumn    
		{
		protected:
			string m_strOrganismID;
			CStdIPoint m_oPosition;
			string m_strDataType;

		public:
			NeuronData();
			virtual ~NeuronData();

			virtual string DataType() {return m_strDataType;}
			virtual void DataType(string strType) {m_strDataType = strType;}

			virtual void Initialize(Simulator *lpSim);
			virtual void Load(Simulator *lpSim, CStdXml &oXml);
		};

	}			//DataColumns
}				//FastNeuralNet

#endif // !defined(AFX_NLNEURONDATACOLUMN_H__84D5E187_145D_401E_89AE_5FD5F2CB17E7__INCLUDED_)

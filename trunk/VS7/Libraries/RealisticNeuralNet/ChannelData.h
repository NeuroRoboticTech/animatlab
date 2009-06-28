// ChannelData.h: interface for the ChannelData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NLCHANNELDATACOLUMN_H__84D5E187_145D_401E_89AE_5FD5F2CB17E7__INCLUDED_)
#define AFX_NLCHANNELDATACOLUMN_H__84D5E187_145D_401E_89AE_5FD5F2CB17E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace RealisticNeuralNet
{
	namespace DataColumns
	{

		class ADV_NEURAL_PORT ChannelData : public DataColumn    
		{
		protected:
			string m_strOrganismID;
			int m_iNeuronIndex;
			string m_strChannelID;
			string m_strDataType;

		public:
			ChannelData();
			virtual ~ChannelData();

			virtual string DataType() {return m_strDataType;}
			virtual void DataType(string strType) {m_strDataType = strType;}

			virtual void Initialize(Simulator *lpSim);
			virtual void Load(Simulator *lpSim, CStdXml &oXml);
		};

	}			//DataColumns
}				//FastNeuralNet

#endif // !defined(AFX_NLCHANNELDATACOLUMN_H__84D5E187_145D_401E_89AE_5FD5F2CB17E7__INCLUDED_)

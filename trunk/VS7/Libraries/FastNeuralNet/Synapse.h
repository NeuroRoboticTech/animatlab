// CNLSynapse.h: interface for the Synapse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCNLSYNAPSE_H__0D6FBDE5_468A_44C1_8630_9279DDB69CA8__INCLUDED_)
#define AFX_RCNLSYNAPSE_H__0D6FBDE5_468A_44C1_8630_9279DDB69CA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace FastNeuralNet
{
	namespace Synapses
	{

		class FAST_NET_PORT Synapse : public CStdSerialize   
		{
		protected:
			BOOL m_bEnabled;
			unsigned char m_iFromX;
			unsigned char m_iFromY;
			unsigned char m_iFromZ;
			float m_fltWeight;
			float m_fltModulation;

		public:
			Synapse();
			virtual ~Synapse();

			virtual unsigned char Type()
			{return REGULAR_SYNAPSE;};

			BOOL Enabled();
			void Enabled(BOOL bVal);

			unsigned char FromX();
			void FromX(unsigned char iVal);

			unsigned char FromY();
			void FromY(unsigned char iVal);

			unsigned char FromZ();
			void FromZ(unsigned char iVal);

			float Weight();
			void Weight(float fltVal);
			float *WeightPointer();

			float Modulation();
			float *ModulationPointer();
			virtual float CalculateModulation(FastNeuralModule *lpModule);

			virtual float *GetDataPointer(short iCompoundIndex, short iDataType);

			virtual void Load(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml);
		};

	}			//Synapses
}				//FastNeuralNet

#endif // !defined(AFX_RCNLSYNAPSE_H__0D6FBDE5_468A_44C1_8630_9279DDB69CA8__INCLUDED_)

// BistableNeuron.h: interface for the BistableNeuron class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BISTABLENEURON_H__D9D0316C_3191_429A_B3DF_1B52CF9E68BC__INCLUDED_)
#define AFX_BISTABLENEURON_H__D9D0316C_3191_429A_B3DF_1B52CF9E68BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace FastNeuralNet
{
	namespace Neurons
	{

		class FAST_NET_PORT BistableNeuron  : public Neuron 
		{
		protected:
			float m_fltIntrinsic;
			float m_fltVsth;

			float m_fltIl;
			float m_fltIh;

			virtual float CalculateIntrinsicCurrent(FastNeuralModule *lpModule, float fltInputCurrent);

		public:
			BistableNeuron();
			virtual ~BistableNeuron();

			float Vsth();
			void Vsth(float fltVal);

			float IntrinsicCurrent();
			void IntrinsicCurrent(float fltVal);

			float Il();
			void Il(float fltVal);

			float Ih();
			void Ih(float fltVal);

			virtual unsigned char NeuronType();

			//virtual long CalculateSnapshotByteSize();
			//virtual void SaveKeyFrameSnapshot(byte *aryBytes, long &lIndex);
			//virtual void LoadKeyFrameSnapshot(byte *aryBytes, long &lIndex);

			virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		};

	}			//Neurons
}				//FastNeuralNet

#endif // !defined(AFX_BISTABLENEURON_H__D9D0316C_3191_429A_B3DF_1B52CF9E68BC__INCLUDED_)

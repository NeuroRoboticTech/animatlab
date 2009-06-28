// RandomNeuron.h: interface for the RandomNeuron class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANDOMNEURON_H__D9D0316C_3191_429A_B3DF_1B52CF9E68BC__INCLUDED_)
#define AFX_RANDOMNEURON_H__D9D0316C_3191_429A_B3DF_1B52CF9E68BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace FastNeuralNet
{
	namespace Neurons
	{

		class FAST_NET_PORT RandomNeuron  : public Neuron 
		{
		protected:
			float m_fltITime;
			unsigned char m_iIntrinsicType;
			float m_fltIntrinsic;

			float m_fltIl;

			AnimatLibrary::Gains::Gain *m_lpCurrentGraph;
			AnimatLibrary::Gains::Gain *m_lpBurstGraph;
			AnimatLibrary::Gains::Gain *m_lpIBurstGraph;

			void HighCurrentOn();
			void LowCurrentOn();

			virtual float CalculateIntrinsicCurrent(FastNeuralModule *lpModule, float fltInputCurrent);

		public:
			RandomNeuron();
			virtual ~RandomNeuron();

			float ITime();
			void ITime(float fltVal);

			unsigned char IntrinsicType();
			void IntrinsicType(unsigned char iVal);

			float IntrinsicCurrent();
			void IntrinsicCurrent(float fltVal);

			float Il();
			void Il(float fltVal);

			virtual unsigned char NeuronType();

			virtual long CalculateSnapshotByteSize();
			virtual void SaveKeyFrameSnapshot(byte *aryBytes, long &lIndex);
			virtual void LoadKeyFrameSnapshot(byte *aryBytes, long &lIndex);

			virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		};

	}			//Neurons
}				//FastNeuralNet

#endif // !defined(AFX_RANDOMNEURON_H__D9D0316C_3191_429A_B3DF_1B52CF9E68BC__INCLUDED_)

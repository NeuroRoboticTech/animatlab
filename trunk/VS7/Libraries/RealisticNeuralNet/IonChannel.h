// IonChannel.h: interface for the IonChannel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ION_CHANNEL_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)
#define AFX_ION_CHANNEL_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

/*! \brief 
   xxxx.

   \remarks
   xxxx
		 
   \sa
	 xxx
	 
	 \ingroup RealisticNeuralNet
*/

namespace RealisticNeuralNet
{
	class ADV_NEURAL_PORT IonChannel : public CStdSerialize 
	{
	protected:
		string m_strID;
		string m_strName;

		BOOL m_bEnabled;
		float m_fltGmax;
		float m_fltG;
		float m_fltMPower;
		float m_fltHPower;
		float m_fltEquilibriumPotential;

		float m_fltM;
		float m_fltNm;
		AnimatLibrary::Gains::Gain *m_lpMinf;
		AnimatLibrary::Gains::Gain *m_lpTm;

		float m_fltH;
		float m_fltNh;
		AnimatLibrary::Gains::Gain *m_lpHinf;
		AnimatLibrary::Gains::Gain *m_lpTh;

		float m_fltTotalAct;
		float m_fltI;

		float m_fltMinf;
		float m_fltHinf;
		float m_fltTm;
		float m_fltTh;

	public:
		IonChannel();
		virtual ~IonChannel();

		string ID() {return m_strID;};

		virtual void Initialize(Simulator *lpSim, Structure *lpStructure);
		virtual float *GetDataPointer(string strDataType);
		virtual void Load(CStdXml &oXml, RealisticNeuralModule *lpNS);
		virtual float CalculateCurrent(float fltStep, float fltVm);
	};
}				//AnimatLibrary

#endif // !defined(AFX_ION_CHANNEL_H__9FEE3153_B3B6_4064_B93B_35265C06E366__INCLUDED_)

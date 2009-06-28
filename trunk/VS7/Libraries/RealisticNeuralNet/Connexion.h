// Connexion.h: interface for the Connexion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONNEXION_H__C287771C_6873_4883_829A_CB726940617B__INCLUDED_)
#define AFX_CONNEXION_H__C287771C_6873_4883_829A_CB726940617B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace RealisticNeuralNet
{
	namespace Synapses
	{

		class ADV_NEURAL_PORT Connexion : public CStdSerialize  
		{
		public:
			Connexion();
			Connexion(int source, int target, int type, int id, double d,float topBlock=75,float botBlock=25);
			virtual ~Connexion();
			virtual void Load(CStdXml &oXml);

		protected:

		// LOADABLE
			int m_iSource;
			int m_iTarget;
			int m_iType;
			int m_iID;
			double m_dDelay;
			double m_dGFacilCx;		// facilitated increase in g when input occurs
			double m_dG;
			double m_dPartialBlockHold;

		// WORKING
		// stuff for spiking chemical synapses
			DoubleList m_TransitCx;
			double m_dMaxGHebb;			// full Hebb incremented amp
			double m_dMaxGVoltDepRel;
			double m_dRelFacil;		// relative facilitation, 1 = none
			double m_dFacilD;		// facilitation decrement per cycle 
			double m_dBaseG;			// standard baseline conductance
			
			BOOL m_bHebbian;
			BOOL m_bVoltDep;
			double m_dHebbTimeWindow;
			double m_dHebbIncrement;
			DoubleList m_HebbList;
			double m_dTimeSincePrevHebbEvent;
			BOOL m_bAllowForgetting;
			double m_dForgettingWindow;
			double m_dConsolidation;

			double m_dPartialBlock;

			double m_dPreviousSpikeLatency;

			void AppendTransitSpike() {m_TransitCx.AddTail(m_dDelay);}
			void DecrementLatencies(double dt,BOOL FreezeLearning=FALSE);
			void DecrementFacilitation() 
			{
				if (m_dRelFacil==1) return;
				else m_dGFacilCx=m_dG+(m_dGFacilCx-m_dG)*m_dFacilD;
			}
			double *GetTimeToNextSpikePtr() {return m_TransitCx.First();}
			double ProcessOutput(BOOL bFreezeHebb=FALSE);
			double GetProspectiveCond(BOOL bFreezeHebb);
			void IncrementHebbian();

		friend class RealisticNeuralModule;
		};

	}			//Synapses
}				//RealisticNeuralNet

#endif // !defined(AFX_CONNEXION_H__C287771C_6873_4883_829A_CB726940617B__INCLUDED_)

// IonChannelSigmoid.cpp: implementation of the IonChannelSigmoid class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "IonChannelSigmoid.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace RealisticNeuralNet
{
	namespace Gains
	{

/*! \brief 
   Constructs an structure object..
   		
	 \return
	 No return value.

   \remarks
	 The constructor for a structure. 
*/

IonChannelSigmoid::IonChannelSigmoid()
{
	m_fltA = 0;
	m_fltB = 0;
	m_fltC = 0;
	m_fltD = 0;
	m_fltE = 0;
	m_fltF = 0;
	m_fltG = 0;
	m_fltH = 1;
}


/*! \brief 
   Destroys the structure object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the structure object..	 
*/

IonChannelSigmoid::~IonChannelSigmoid()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of IonChannelSigmoid\r\n", "", -1, FALSE, TRUE);}
}

float IonChannelSigmoid::CalculateGain(float fltInput)
{
	if(InLimits(fltInput))
		return (m_fltA + (m_fltB/(m_fltH + exp(m_fltC*(fltInput+m_fltD)) + m_fltE*exp(m_fltF*(fltInput+m_fltG)) )));
	else
		return CalculateLimitOutput(fltInput);
}

void IonChannelSigmoid::Load(CStdXml &oXml)
{
	Gain::Load(oXml);

	oXml.IntoElem();  //Into Adapter Element

	m_fltA = oXml.GetChildFloat("A");
	m_fltB = oXml.GetChildFloat("B");
	m_fltC = oXml.GetChildFloat("C");
	m_fltD = oXml.GetChildFloat("D");
	m_fltE = oXml.GetChildFloat("E");
	m_fltF = oXml.GetChildFloat("F");
	m_fltG = oXml.GetChildFloat("G");
	m_fltH = oXml.GetChildFloat("H");

	oXml.OutOfElem(); //OutOf Adapter Element
}

void IonChannelSigmoid::LoadFull(CStdXml &oXml)
{
	Gain::LoadFull(oXml);

	oXml.IntoElem();  //Into Adapter Element

	m_fltA = AnimatLibrary::LoadScaledNumber(oXml, "A", FALSE, m_fltA);
	m_fltB = AnimatLibrary::LoadScaledNumber(oXml, "B", FALSE, m_fltB);
	m_fltC = AnimatLibrary::LoadScaledNumber(oXml, "C", FALSE, m_fltC);
	m_fltD = AnimatLibrary::LoadScaledNumber(oXml, "D", FALSE, m_fltD);
	m_fltE = AnimatLibrary::LoadScaledNumber(oXml, "E", FALSE, m_fltE);
	m_fltF = AnimatLibrary::LoadScaledNumber(oXml, "F", FALSE, m_fltF);
	m_fltG = AnimatLibrary::LoadScaledNumber(oXml, "G", FALSE, m_fltG);
	m_fltH = AnimatLibrary::LoadScaledNumber(oXml, "H", FALSE, m_fltH);

	oXml.OutOfElem(); //OutOf Adapter Element
}

	}			//Gains
}			//AnimatLibrary

// SigmoidGain.cpp: implementation of the SigmoidGain class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Gain.h"
#include "SigmoidGain.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace AnimatLibrary
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

SigmoidGain::SigmoidGain()
{
	m_fltA = 0;
	m_fltB = 0;
	m_fltC = 0;
	m_fltD = 0;
}


/*! \brief 
   Destroys the structure object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the structure object..	 
*/

SigmoidGain::~SigmoidGain()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of SigmoidGain\r\n", "", -1, FALSE, TRUE);}
}

float SigmoidGain::CalculateGain(float fltInput)
{
	if(InLimits(fltInput))
		return ((m_fltB/(1+exp(m_fltC*(m_fltA-fltInput)))) + m_fltD);
	else
		return CalculateLimitOutput(fltInput);
}

void SigmoidGain::Load(CStdXml &oXml)
{
	Gain::Load(oXml);

	oXml.IntoElem();  //Into Adapter Element

	m_fltA = oXml.GetChildFloat("A");
	m_fltB = oXml.GetChildFloat("B");
	m_fltC = oXml.GetChildFloat("C");
	m_fltD = oXml.GetChildFloat("D");

	oXml.OutOfElem(); //OutOf Adapter Element
}

void SigmoidGain::LoadFull(CStdXml &oXml)
{
	Gain::LoadFull(oXml);

	oXml.IntoElem();  //Into Adapter Element

		m_fltA = AnimatLibrary::LoadScaledNumber(oXml, "A", FALSE, m_fltA);
		m_fltB = AnimatLibrary::LoadScaledNumber(oXml, "B", FALSE, m_fltB);
		m_fltC = AnimatLibrary::LoadScaledNumber(oXml, "C", FALSE, m_fltC);
		m_fltD = AnimatLibrary::LoadScaledNumber(oXml, "D", FALSE, m_fltD);

	oXml.OutOfElem(); //OutOf Adapter Element
}

	}			//Gains
}			//AnimatLibrary

// Gain.cpp: implementation of the Gain class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Gain.h"

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

Gain::Gain()
{
	m_bUseLimits = FALSE;
	m_fltLowerLimit = 0;
	m_fltLowerOutput = 0;
	m_fltUpperLimit = 0;
	m_fltUpperOutput = 0;
}


/*! \brief 
   Destroys the structure object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the structure object..	 
*/

Gain::~Gain()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Gain\r\n", "", -1, FALSE, TRUE);}
}

void Gain::Load(CStdXml &oXml)
{
	oXml.IntoElem();  //Into Adapter Element

	m_bUseLimits = oXml.GetChildBool("UseLimits", m_bUseLimits);

	if(m_bUseLimits)
	{
		m_fltLowerLimit = oXml.GetChildFloat("LowerLimit");
		m_fltLowerOutput = oXml.GetChildFloat("LowerOutput");
		m_fltUpperLimit = oXml.GetChildFloat("UpperLimit");
		m_fltUpperOutput = oXml.GetChildFloat("UpperOutput");
		
		Std_IsAboveMin(m_fltLowerLimit, m_fltUpperLimit, TRUE, "UpperLimit");
	}

	oXml.OutOfElem(); //OutOf Adapter Element
}

void Gain::LoadFull(CStdXml &oXml)
{
	oXml.IntoElem();  //Into Adapter Element

	m_bUseLimits = oXml.GetChildBool("UseLimits", m_bUseLimits);

	if(m_bUseLimits)
	{
		m_fltLowerLimit = AnimatLibrary::LoadScaledNumber(oXml, "LowerLimitScale", FALSE, m_fltLowerLimit);
		m_fltLowerOutput = AnimatLibrary::LoadScaledNumber(oXml, "LowerOutputScale", FALSE, m_fltLowerOutput);
		m_fltUpperLimit = AnimatLibrary::LoadScaledNumber(oXml, "UpperLimitScale", FALSE, m_fltUpperLimit);
		m_fltUpperOutput = AnimatLibrary::LoadScaledNumber(oXml, "UpperOutputScale", FALSE, m_fltUpperOutput);

		Std_IsAboveMin(m_fltLowerLimit, m_fltUpperLimit, TRUE, "UpperLimit");
	}

	oXml.OutOfElem(); //OutOf Adapter Element
}

	}			//Gains
}			//AnimatLibrary

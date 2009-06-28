// ExternalStimulus.cpp: implementation of the ExternalStimulus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "ExternalStimulus.h"

namespace AnimatLibrary
{
	namespace ExternalStimuli
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ExternalStimulus::ExternalStimulus()
{
}

ExternalStimulus::~ExternalStimulus()
{
	m_fltInput = 0;
}

BOOL ExternalStimulus::operator<(ActivatedItem *lpItem)
{
	ExternalStimulus *lpStimulus = dynamic_cast<ExternalStimulus *>(lpItem);

	if(!lpStimulus)
		THROW_ERROR(Al_Err_lItemNotStimulusType, Al_Err_strItemNotStimulusType);

	if(m_lStartSlice < lpStimulus->m_lStartSlice)
		return TRUE;

	if( (m_lStartSlice == lpStimulus->m_lStartSlice) && (m_lEndSlice < lpStimulus->m_lEndSlice))
		return TRUE;

	return FALSE;
}

	}			//ExternalStimuli
}				//AnimatLibrary

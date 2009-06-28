// ActivatedItemMgr.cpp: implementation of the ActivatedItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
//#include "ClassFactory.h"
#include "Node.h"
#include "Adapter.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "IODataEntry.h"
#include "Microcontroller.h"
#include "Structure.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"

namespace AnimatLibrary
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ActivatedItemMgr::ActivatedItemMgr()
{
	m_bAllItemsActivated = FALSE;
	m_bFinished = FALSE;
	m_lNextStartSlice = -1;
	m_lNextEndSlice = -1;
	m_iNextItem = -1;
}

ActivatedItemMgr::~ActivatedItemMgr()
{

try
{
	Reset();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of ActivatedItemMgr\r\n", "", -1, FALSE, TRUE);}
}

void ActivatedItemMgr::Reset()
{
try
{
	m_bAllItemsActivated = FALSE;
	m_bFinished = FALSE;
	m_lNextStartSlice = -1;
	m_lNextEndSlice = -1;
	m_iNextItem = -1;

	m_aryItems.RemoveAll();
	m_aryActiveItems.RemoveAll();
	m_aryItemsMap.RemoveAll();
	m_aryAlwaysActiveItems.RemoveAll();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of DataChart\r\n", "", -1, FALSE, TRUE);}
}

void ActivatedItemMgr::AddItemInternal(Simulator *lpSim, ActivatedItem *lpItem, int iIndex)
{
	//First lets make sure this is a unique item key.
	try
	{
		m_aryItemsMap.Add(lpItem->ID(), lpItem);
	}
	catch(CStdErrorInfo oError)
	{
		oError.m_strError += " Duplicate activate item Key: " + lpItem->ID(); 
		THROW_ERROR(oError.m_lError, oError.m_strError);
	}

	if(!lpItem->AlwaysActive())
	{
		if(iIndex < 0)
		{
			m_aryItems.Add(lpItem);
			iIndex = m_aryItems.GetSize() - 1;
		}
		else
			m_aryItems.InsertAt(iIndex, lpItem);

		//Lets check to see if this item should be currently active or active in the future.
		//Only bother with these checks if you are adding items after the system has been initialized.
		if(lpSim->Initialized())
		{
			if( lpItem->StartSlice() > lpSim->TimeSlice() )
			{
				m_bAllItemsActivated = FALSE;

				if(m_lNextStartSlice == -1 || (lpItem->StartSlice() < m_lNextStartSlice) || ((lpItem->StartSlice() < m_lNextStartSlice) && (iIndex < m_iNextItem)) )
				{
					m_iNextItem = iIndex;
					m_lNextStartSlice = lpItem->StartSlice();
				}
			}
			else if( (lpItem->StartSlice() <= lpSim->TimeSlice()) && (lpItem->EndSlice() >= lpSim->TimeSlice()) )
			{
				lpItem->Initialize(lpSim);
				ActivateItem(lpSim, iIndex, lpItem, FALSE); 

				if(m_iNextItem != -1 && m_iNextItem <= iIndex)
					m_iNextItem++;
			}
			else if(m_iNextItem != -1 && (lpItem->EndSlice() < lpSim->TimeSlice()) && (m_iNextItem+1 < m_aryItems.GetSize()) )
				m_iNextItem++;

		}
	}
	else
		m_aryAlwaysActiveItems.Add(lpItem);

}

void ActivatedItemMgr::Add(Simulator *lpSim, ActivatedItem *lpItem)
{
	if(!lpItem)
		THROW_ERROR(Al_Err_lActivatedItemNull, Al_Err_strActivatedItemNull);

	//Lets make sure the ID of the item is in upper case.
	lpItem->ID(Std_CheckString(lpItem->ID()));

	if(!lpItem->AlwaysActive())
	{
		int iCount = m_aryItems.GetSize();
		ActivatedItem *lpTest = NULL;
		for(int iIndex=0; iIndex<iCount; iIndex++)
		{
			lpTest = m_aryItems[iIndex];

			if(lpTest->StartSlice() >= lpItem->StartSlice())
			{
				AddItemInternal(lpSim, lpItem, iIndex);
				return;
			}
		}

	}

	AddItemInternal(lpSim, lpItem);
}

void ActivatedItemMgr::Remove(Simulator *lpSim, string strID, BOOL bThrowError)
{
	int iIndex=0;
	ActivatedItem *lpItem = Find(strID, iIndex, bThrowError);
	if(!lpItem) return;

	int iActiveIndex=0;
	ActivatedItem *lpActiveItem = FindActive(strID, iActiveIndex, FALSE);
	
	m_aryItemsMap.Remove(Std_CheckString(strID));

	if(lpItem->AlwaysActive())
	{
		lpItem->Deactivate(lpSim);
		m_aryAlwaysActiveItems.RemoveAt(iIndex);
	}
	else
	{
		if(lpActiveItem) 
		{
			lpActiveItem->Deactivate(lpSim);
			m_aryActiveItems.RemoveAt(iActiveIndex);
		}

		m_aryItems.RemoveAt(iIndex);

		if(m_iNextItem != -1 && m_iNextItem > iIndex)
			m_iNextItem--;

		if(m_aryActiveItems.GetSize() > 0)
			m_lNextEndSlice = m_aryActiveItems[0]->EndSlice();
	}
}

ActivatedItem *ActivatedItemMgr::Find(string strID, int &iIndex, BOOL bThrowError)
{
	int iCount = m_aryItems.GetSize();
	ActivatedItem *lpItem = NULL;
	strID = Std_CheckString(strID);
	for(iIndex=0; iIndex<iCount; iIndex++)
	{
		lpItem = m_aryItems[iIndex];

		if(lpItem->ID() == strID)
			return lpItem;
	}

	iCount = m_aryAlwaysActiveItems.GetSize();
	for(iIndex=0; iIndex<iCount; iIndex++)
	{
		lpItem = m_aryAlwaysActiveItems[iIndex];

		if(lpItem->ID() == strID)
			return lpItem;
	}

	if(bThrowError)
		THROW_TEXT_ERROR(Al_Err_lActivatedItemIDNotFound, Al_Err_strActivatedItemIDNotFound, " ActivatedItemID: " + strID);

	return NULL;
}

ActivatedItem *ActivatedItemMgr::FindActive(string strID, int &iIndex, BOOL bThrowError)
{
	int iCount = m_aryActiveItems.GetSize();
	ActivatedItem *lpItem = NULL;
	strID = Std_CheckString(strID);
	for(iIndex=0; iIndex<iCount; iIndex++)
	{
		lpItem = m_aryActiveItems[iIndex];

		if(lpItem->ID() == strID)
			return lpItem;
	}

	iCount = m_aryAlwaysActiveItems.GetSize();
	for(iIndex=0; iIndex<iCount; iIndex++)
	{
		lpItem = m_aryAlwaysActiveItems[iIndex];

		if(lpItem->ID() == strID)
			return lpItem;
	}

	if(bThrowError)
		THROW_TEXT_ERROR(Al_Err_lActivatedItemIDNotFound, Al_Err_strActivatedItemIDNotFound, " ActivatedItemID: " + strID);

	return NULL;
}

ActivatedItem *ActivatedItemMgr::Find(string strID, BOOL bThrowError)
{
	ActivatedItem *lpItem = NULL;
	CStdMap<string, ActivatedItem *>::iterator oPos;
	oPos = m_aryItemsMap.find(Std_CheckString(strID));

	if(oPos != m_aryItemsMap.end())
		lpItem =  oPos->second;
	else if(bThrowError)
		THROW_TEXT_ERROR(Al_Err_lActivatedItemIDNotFound, Al_Err_strActivatedItemIDNotFound, " ActivatedItemID: " + strID);

	return lpItem;
}

void ActivatedItemMgr::Initialize(Simulator *lpSim)
{
	int iCount = m_aryItems.GetSize();

	for(int iChart=0; iChart<iCount; iChart++)
		m_aryItems[iChart]->Initialize(lpSim);

	//Always active items need to be initialized and then activated.
	iCount = m_aryAlwaysActiveItems.GetSize();
	for(int iChart=0; iChart<iCount; iChart++)
	{
		m_aryAlwaysActiveItems[iChart]->Initialize(lpSim);
		m_aryAlwaysActiveItems[iChart]->Activate(lpSim);
	}

	//Now sort the charts based on their starting time slice.
	stable_sort(m_aryItems.begin(), m_aryItems.end(), LessThanActivatedItemCompare);

	//if(m_aryItems.GetSize() > 0)
	//	TRACE_STL_CONTAINER_DESC(m_aryItems, "Items");
}

void ActivatedItemMgr::ReInitialize(Simulator *lpSim)
{
	int iCount = m_aryItems.GetSize();

	for(int iChart=0; iChart<iCount; iChart++)
		m_aryItems[iChart]->ReInitialize(lpSim);

	iCount = m_aryAlwaysActiveItems.GetSize();
	for(int iChart=0; iChart<iCount; iChart++)
		m_aryAlwaysActiveItems[iChart]->ReInitialize(lpSim);

	//There should be no need to resort the items. They are sorted initially
	//the first time the system is initialized, and then newly added items
	//are added at the correct position to maintain a sorted order.
}

void ActivatedItemMgr::ActivateItem(Simulator *lpSim, int iSelChart, ActivatedItem *lpItem, BOOL bSetNextItem)
{
	int iActiveCount = m_aryActiveItems.GetSize(), iFound = -1;
	for(int iActive=0; iActive<iActiveCount; iActive++)
	{	
		if(lpItem->EndSlice() > m_aryActiveItems[iActive]->EndSlice()) 
		{
			//If there is more than one chart in the list with the same end
			//slice then keep skipping till we get to that one.
			if( (iActiveCount == (iActive+1)) || ((iActiveCount > (iActive+1)) 
				   && (m_aryActiveItems[iActive+1]->EndSlice() != m_aryActiveItems[iActive]->EndSlice())) )
				iFound = iActive;
		}
	}

	if(!iActiveCount || (iFound+1) >= iActiveCount)
		m_aryActiveItems.Add(lpItem);
	else
		m_aryActiveItems.InsertAt((iFound+1), lpItem);

	m_lNextEndSlice = m_aryActiveItems[0]->EndSlice();

	if(bSetNextItem)
	{
		if(iSelChart == (m_aryItems.GetSize()-1) )
		{
			m_bAllItemsActivated = TRUE;
			m_lNextStartSlice = -1;
		}
		else
		{
			m_iNextItem = iSelChart+1;
			m_lNextStartSlice = m_aryItems[m_iNextItem]->StartSlice();
		}
	}

	lpItem->Activate(lpSim);

	//TRACE_STL_CONTAINER_DESC(m_aryActiveItems, "ActiveItems");
	//TRACE_DETAIL_NS("NextEndSlice: " + STR(m_lNextEndSlice));
}	


void ActivatedItemMgr::ActivateItems(Simulator *lpSim, long lSlice)
{
	if(!m_bAllItemsActivated)
	{
		//Initialize the stuff if this is the beginning.
		if(m_iNextItem == -1)
		{
			if(m_aryItems.GetSize() > 0)
			{
				m_iNextItem = 0;
				m_lNextStartSlice = m_aryItems[0]->StartSlice();
			}
			else if(m_aryAlwaysActiveItems.GetSize() == 0)
				m_bFinished = TRUE;
		}

		//Check to see if we need to activate some charts.
		if(lSlice == m_lNextStartSlice)
		{
			BOOL bDone = FALSE;
			ActivatedItem *lpItem = NULL;
			int iCount = m_aryItems.GetSize();
			for(int iChart=m_iNextItem; iChart<iCount && !bDone; iChart++)
			{
				lpItem = m_aryItems[iChart];
				if(lpItem->StartSlice() <= lSlice)
					ActivateItem(lpSim, iChart, lpItem);
				else
					bDone = TRUE;
			}
		}
	}
}


void ActivatedItemMgr::DeactivateItem(Simulator *lpSim)
{
	m_aryActiveItems[0]->Deactivate(lpSim);
	
	m_aryActiveItems.RemoveAt(0);

	if(m_aryActiveItems.GetSize() > 0)
		m_lNextEndSlice = m_aryActiveItems[0]->EndSlice();
	else
	{
		m_lNextEndSlice = -1;

		//If all of the charts have been activated and this
		//is the last active one then we are finsihed charting.
		if(m_bAllItemsActivated && m_aryAlwaysActiveItems.GetSize() == 0) 
			m_bFinished = TRUE;
	}
}


void ActivatedItemMgr::StepItems(Simulator *lpSim)
{
	int iCount = m_aryActiveItems.GetSize();
	for(int iActive=0; iActive<iCount; iActive++)
		if(m_aryActiveItems[iActive]->NeedToStep())
			m_aryActiveItems[iActive]->StepSimulation(lpSim);

	iCount = m_aryAlwaysActiveItems.GetSize();
	for(int iActive=0; iActive<iCount; iActive++)
		if(m_aryAlwaysActiveItems[iActive]->NeedToStep())
			m_aryAlwaysActiveItems[iActive]->StepSimulation(lpSim);
}


void ActivatedItemMgr::DeactivateItems(Simulator *lpSim, long lSlice)
{
	if(m_lNextEndSlice == lSlice)
	{
		BOOL bDone = FALSE;
		while(!bDone && m_aryActiveItems.GetSize() > 0)
		{
			if(m_aryActiveItems[0]->EndSlice() <= lSlice)
				DeactivateItem(lpSim);
			else
				bDone = TRUE;
		}
	}
}

void ActivatedItemMgr::StepSimulation(Simulator *lpSim)
{
	if(!m_bFinished)
	{
		long lStep = lpSim->TimeSlice();

		//First lets see if any charts need to be activated.
		ActivateItems(lpSim, lStep);

		//Now step the active charts
		StepItems(lpSim);

		//Finally lets deactivate any charts that need it.
		DeactivateItems(lpSim, lStep);

#if defined(STD_TRACING_ON) && defined(STD_TRACE_DETAIL)
	TRACE_STL_CONTAINER_DESC(m_aryActiveItems, "Active Items");
#endif

	}
}

}			//AnimatLibrary

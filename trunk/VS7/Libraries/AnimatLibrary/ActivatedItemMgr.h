// ActivatedItemMgr.h: interface for the ActivatedItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIVATEDITEMMGR_H__CBDA72E7_469E_4C42_BEF4_41F1D7F40627__INCLUDED_)
#define AFX_ACTIVATEDITEMMGR_H__CBDA72E7_469E_4C42_BEF4_41F1D7F40627__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{

	class ANIMAT_PORT ActivatedItemMgr : public CStdSerialize    
	{
	protected:
		CStdPtrArray<ActivatedItem> m_aryAlwaysActiveItems;
		CStdPtrArray<ActivatedItem> m_aryItems;
		CStdArray<ActivatedItem *> m_aryActiveItems;
		CStdMap<string, ActivatedItem *> m_aryItemsMap;

		BOOL 	m_bAllItemsActivated;
		BOOL m_bFinished;
		int m_iNextItem;
		long m_lNextStartSlice;
		long m_lNextEndSlice;

		virtual void ActivateItem(Simulator *lpSim, int iSelItem, ActivatedItem *lpItem, BOOL bSetNextItem = TRUE);
		virtual void ActivateItems(Simulator *lpSim, long lSlice);
		virtual void DeactivateItem(Simulator *lpSim);
		virtual void DeactivateItems(Simulator *lpSim, long lSlice);
		virtual void StepItems(Simulator *lpSim);

		virtual void AddItemInternal(Simulator *lpSim, ActivatedItem *lpItem, int iIndex = -1);
		virtual ActivatedItem *Find(string strID, int &iIndex, BOOL bThrowError);
		virtual ActivatedItem *FindActive(string strID, int &iIndex, BOOL bThrowError);

	public:
		ActivatedItemMgr();
		virtual ~ActivatedItemMgr();

		BOOL AllItemsActivated() {return m_bAllItemsActivated;};
		BOOL Finished() {return m_bFinished;};
		long NextStartSlice() {return m_lNextStartSlice;};
		long NextEndSlice() {return m_lNextEndSlice;};
		int NextItem() {return m_iNextItem;};

		CStdArray<ActivatedItem *> *ActiveItems() {return &m_aryActiveItems;};

		virtual void Add(Simulator *lpSim, ActivatedItem *lpItem);
		virtual void Remove(Simulator *lpSim, string strID, BOOL bThrowError = TRUE);
		virtual ActivatedItem *Find(string strID, BOOL bThrowError = TRUE);

		virtual void Reset();
		virtual void Initialize(Simulator *lpSim);
		virtual void ReInitialize(Simulator *lpSim);
		virtual void StepSimulation(Simulator *lpSim);
		virtual void Load(Simulator *lpSim, CStdXml &oXml) = 0;
	};

}			//AnimatLibrary

#endif // !defined(AFX_ACTIVATEDITEMMGR_H__CBDA72E7_469E_4C42_BEF4_41F1D7F40627__INCLUDED_)

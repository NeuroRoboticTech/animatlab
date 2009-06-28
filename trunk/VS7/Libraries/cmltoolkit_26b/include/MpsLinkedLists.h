#ifndef _MPSLINKEDLISTS_H
#define _MPSLINKEDLISTS_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:51 $
   
   This software and its accompanying manuals have been developed by CMLabs
   Simulations Inc. ("CMLabs") and MathEngine PLC ("MathEngine").  The
   copyright to the Vortex Toolkits and all associated materials belongs to
   CMLabs.
   
   All intellectual property rights in the software belong to CMLabs,
   except for portions that have been licensed from MathEngine.

   All rights conferred by law (including rights under international
   copyright conventions) are reserved to CMLabs. This software may also
   incorporate information which is confidential to CMLabs.

   Save to the extent permitted by law, or as otherwise expressly permitted
   by CMLabs, this software and the manuals must not be copied (in whole or
   in part), re-arranged, altered or adapted in any way without the prior
   written consent of CMLabs. In addition, the information contained in the
   software may not be disseminated without the prior written consent of
   CMLabs.

 */


/*
    \file
    Linked Lists for internal use.
*/
/** linked-list node structure  */
typedef struct _MpsLinkedListNode
{
  /** forward links  */
  struct _MpsLinkedListNode *m_Next;
  /**  backward links  */
  struct _MpsLinkedListNode *m_Prev;
  /** the data element  */
  void *m_Element;
}
MpsLinkedListNode;

/* linked-list node structure functions  */
/* return new linked-list head  */
extern MpsLinkedListNode *NewLinkedList(void);

/** remove all list nodes and list head. 
    But not the data the list
    points to!  
*/
extern void DeleteLinkedList(MpsLinkedListNode *);

/** Add element to the end of a list  */
extern void AddElement(MpsLinkedListNode *, void *);

/* remove element from list - DOES NOT FREE MEMORY FOR THE DATA IT
   POINTS TO!  */
extern void RemoveElement(MpsLinkedListNode *);

/* find element in list  */
extern MpsLinkedListNode *FindElement(const MpsLinkedListNode *, const void *);


/* map to single pointer structure  */
typedef struct _MpsMapToSingle
{
    /* forward and backward links  */
    struct _MpsMapToSingle *m_Next;
    struct _MpsMapToSingle *m_Prev;
    /* data  */
    void *m_Key;
    void *m_Data;
}
MpsMapToSingle;

/* map to single pointer structure functions  */
/* return new Map head  */
extern MpsMapToSingle *NewMapToSingle(void);

/* remove all nodes in map list  */
extern void DeleteMapToSingle(MpsMapToSingle *);

/* add new mapping  */
extern void AddSingleMapping(MpsMapToSingle *, void *, void *);

/* remove mapping  */
extern void RemoveSingleMapping(MpsMapToSingle *);

/* find a mapping in list  */
extern MpsMapToSingle *FindSingleMapping(const MpsMapToSingle *,
    const void *, const void *);


/* map to pair of pointers structure  */
typedef struct _MpsMapToPair
{
    /* forward and backward links  */
    struct _MpsMapToPair *m_Next;
    struct _MpsMapToPair *m_Prev;
    /* data  */
    void *m_Key;
    void *m_Data1;
    void *m_Data2;
}
MpsMapToPair;

/* map to pair of pointers structure functions  */
/* return new Map head  */
extern MpsMapToPair *NewMapToPair(void);

/* remove all nodes in map list  */
extern void DeleteMapToPair(MpsMapToPair *);

/* add new mapping  */
extern void AddPairMapping(MpsMapToPair *, void *, void *, void *);

/* remove mapping  */
extern void RemovePairMapping(MpsMapToPair *);

/* find a mapping in list  */
extern MpsMapToPair *FindPairMapping(const MpsMapToPair *, const void *,
    const void *, const void *);


#endif

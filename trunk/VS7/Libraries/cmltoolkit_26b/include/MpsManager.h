#ifndef _MPSMANAGER_H
#define _MPSMANAGER_H
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

#include "MpsLinkedLists.h"

/**
   \file
   Particle Manager.
   The Particle Manager allocates all memory required and
   keeps track of how forces are bound to each particle system. 
   Many particle systems may be associated with a single manager.
*/

/**
   Various lists relating to systems emitters and forces.
   The Manager allows for update to all particle systems through one managable 
   interface.  The manager can control all updates to any systems created
   and ensure the proper application of forces, interactions, spawns, lifetimes, etc...
*/
struct _MpsManager
{
    /** Particle System lists */
    MpsLinkedListNode *m_SystemList;
    /**  Emitter lists */
    MpsLinkedListNode *m_EmitterList;
    /** List of all global forces */
    MpsLinkedListNode *m_GlobalForceList;
    /** List of all local forces available */
    MpsLinkedListNode *m_LocalForceList;
    /** List of any MpsInteractions created / used */
    MpsLinkedListNode *m_InteractionList;
    /** Bindings - Global Forces to Particle Systems */
    MpsMapToSingle *m_GlobalForceBindings;
    /** Bindings - Local Forces to Particle Systems  */
    MpsMapToSingle *m_LocalForceBindings;
    /** Bindings - particle-particle interactions to Particle Systems */
    MpsMapToPair *m_InteractionBindings;
};

#endif

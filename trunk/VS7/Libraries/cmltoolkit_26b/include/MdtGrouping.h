#ifndef _MDTGROUPING_H
#define _MDTGROUPING_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/03/16 22:06:15 $
   
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

/** @file
 * MdtGrouping API functions.
 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


    MdtGroupingID                  MEAPI MdtGroupingCreate(const MdtWorldID w);
    void                           MEAPI MdtGroupingAddBody(const MdtGroupingID g, MdtBodyID b);
    void                           MEAPI MdtGroupingRemoveBody(const MdtGroupingID g, MdtBodyID b);
    MdtKeaParameters*              MEAPI MdtGroupingGetSolverParameters(const MdtGroupingID g, MdtBodyID b);
    MdtBclConstraintParameters*    MEAPI MdtGroupingGetConstraintsParameters(const MdtGroupingID g, MdtBodyID b);
    void                           MEAPI MdtGroupingSetConstraintsParameters(const MdtGroupingID g, MdtBclConstraintParameters p);
    void                           MEAPI MdtGroupingSetSolverParameters(const MdtGroupingID g, MdtKeaParameters p);
    int                            MEAPI MdtGroupingGetIndex(const MdtGroupingID g);


#ifdef __cplusplus
}
#endif


#endif

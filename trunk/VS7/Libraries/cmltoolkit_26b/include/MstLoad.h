#ifndef _MSTLOAD_H
#define _MSTLOAD_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:03 $
   
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
    Mst API for loading.
*/

#include "MstTypes.h"
#include "MdtLoad.h"
#include "McdLoad.h"
#include "MeLoadContext.h"

#ifdef __cplusplus
extern "C" {
#endif

MeBool          MEAPI MstUniverseLoad(MstUniverseID universe,
                                        MeStream stream,MeLoadContext *lc);

/* Internal */
MeXMLError      MEAPI Handle_Mst(MeXMLElement * elem);
void            MEAPI MstResolveHash(MeLoadContext *lc);


#ifdef __cplusplus
}
#endif

#endif





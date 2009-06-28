#ifndef _MDTLOAD_H
#define _MDTLOAD_H
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
    Mdt API for loading.
*/

#include "MdtTypes.h"
#include "MeLoadContext.h"

#ifdef __cplusplus
extern "C" {
#endif


MeBool            MEAPI MdtWorldLoad(MdtWorldID world,
                                     MeStream stream,MeLoadContext *lc);

MdtBodyID         MEAPI MdtBodyLookup(char *ID,MeHash *h);
MdtConstraintID   MEAPI MdtConstraintLookup(char *ID,MeHash *h);

#define                 MdtBSJointLookup(id, hash) \
                            (MdtBSJointID)MdtConstraintLookup(id,hash)
#define                 MdtBSJointLookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

#define                 MdtHingeLookup(id, hash) \
                            (MdtBSJointID)MdtConstraintLookup(id,hash)
#define                 MdtHingeLookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

#define                 MdtPrismaticLookup(id, hash) \
                            (MdtPrismaticID)MdtConstraintLookup(id,hash)
#define                 MdtPrismaticLookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

#define                 MdtCarWheelLookup(id, hash) \
                            (MdtCarWheelID)MdtConstraintLookup(id,hash)
#define                 MdtCarWheelLookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

#define                 MdtFixedPathLookup(id, hash) \
                            (MdtFixedPathID)MdtConstraintLookup(id,hash)
#define                 MdtFixedPathLookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

#define                 MdtLinear1Lookup(id, hash) \
                            (MdtLinear1ID)MdtConstraintLookup(id,hash)
#define                 MdtLinear1LookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

#define                 MdtLinear2Lookup(id, hash) \
                            (MdtLinear2ID)MdtConstraintLookup(id,hash)
#define                 MdtLinear2LookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

#define                 MdtUniversalLookup(id, hash) \
                            (MdtUniversalID)MdtConstraintLookup(id,hash)
#define                 MdtUniversalLookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

#define                 MdtSpringLookup(id, hash) \
                            (MdtSpringID)MdtConstraintLookup(id,hash)
#define                 MdtSpringLookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

#define                 MdtAngular3Lookup(id, hash) \
                            (MdtAngular3ID)MdtConstraintLookup(id,hash)
#define                 MdtAngular3LookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

#define                 MdtRPROJointLookup(id, hash) \
                            (MdtRPROJointID)MdtConstraintLookup(id,hash)
#define                 MdtRPROJointLookupStringID(j, hash) \
                            MdtConstraintLookupStringID(j,hash)

/* Internal */
MeXMLError        MEAPI Handle_Mdt(MeXMLElement * elem);



#ifdef __cplusplus
}
#endif


#endif

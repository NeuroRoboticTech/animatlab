#ifndef MDTDOUBLEHINGE_H
#define MDTDOUBLEHINGE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/09/27 21:30:57 $
   
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
 * MdtDoubleHinge API functions.

 The geometry here is that of two hinges which are a fixed distance apart.
 The center of rotation of the first hinge is fixed in body 0 and that of
 the second hinge is fixed in body 1.  The distance between the center of
 the two hinges is kept constant and is set through the MdtLimit interface,
 using index 0.  The center of rotation of the second hinge is kept in a
 plane defined by the location and the axis of rotation of the first hinge.
 The orientation of the first hinge is controlled with the MdtLimit
 interface, using index 1 this time.  The rotation axis of the second hinge
 is independent of that of the first.  The rotation about the second hinge
 is controlled with the MdtLimit using index 2.


 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif



    MdtDoubleHingeID  MEAPI MdtDoubleHingeCreate(const MdtWorldID w);
    void              MEAPI MdtDoubleHingeReset(MdtHingeID j);
    MdtDoubleHingeID  MEAPI MdtDoubleHingeCreateInPool(MePool *pool);



#ifdef __cplusplus
}
#endif


#endif

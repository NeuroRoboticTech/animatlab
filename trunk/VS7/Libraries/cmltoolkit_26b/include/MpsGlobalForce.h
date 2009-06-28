#ifndef _MPSGLOBALFORCE_H
#define _MPSGLOBALFORCE_H
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

#include "MePrecision.h"
#include "MeMath.h"
#include "MpsEnums.h"
#include "MpsObjects.h"

/**
   \file
   Users can create their own global forces.
   Customization to the library allows for easy extension to the functionality of
   the particle system
*/

/** Global force parameters */
struct _MpsGlobalForce
{
    /** vector style parameter for directional application */
    MeVector3 m_VectorParameter;
    /** scalar parameter for augmentation without direction */
    MeReal m_ScalarParameter;
    /** the type of force to apply */
    MpsGlobalForceType m_ForceType;
};

/* new and delete */
extern MpsGlobalForce *NewGlobalForce(const MpsGlobalForceType);
/* new and delete */
extern void DeleteGlobalForce(MpsGlobalForce *);
/* accumulation of global forces */
extern void AccumulateGlobalForce(const MpsGlobalForce *, MpsSystem *);
/* set reset global force references for a system */
extern void SetGlobalForce(const MpsGlobalForce *, MpsSystem *);


#endif

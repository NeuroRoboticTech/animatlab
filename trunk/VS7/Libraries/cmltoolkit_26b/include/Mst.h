#ifndef _MST_H
#define _MST_H
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

/** @file
 * Vortex Simulation Toolkit Main Header.
 * This library provides useful functions for creating and simulating
 * objects using both the Vortex Dynamics Toolkit and the Vortex Collision
 * Toolkit.
 * An MstUniverse contains an McdSpace collision farfield, an MdtWorld, an
 * MstBridge, and some buffers for moving contacts between MCD and MDT.
 * MstUniverseCreate creates these things in one handy function.
 * A collision McdModel is the main structure, with an optional dynamics
 * MdtBody associated with it.
 * Mst contains functions for creating these together (for convenience),
 * or seperately and associating them with each other using McdModelSetBody.
 * It can also set the mass and inertia tensor of an MdtBody to a sensible
 * default based on the collision geometry and a density.
 * MstUniverseStep is a dynamics and collision 'main loop'.
 */

/* ME Globals and Memory */
#include "MePrecision.h"
#include "MeAssert.h"
#include "MeMessage.h"

#include "MeMemory.h"

/* Vortex Dynamics Toolkit */
#include "Mdt.h"

/* Vortex Collision Toolkit */
#include "McdFrame.h"
#include "McdPrimitives.h"

#include "MstTypes.h"

#include "MstBridge.h"
#include "MstUniverse.h"
#include "MstModelDynamics.h"
#include "MstUtils.h"


#endif

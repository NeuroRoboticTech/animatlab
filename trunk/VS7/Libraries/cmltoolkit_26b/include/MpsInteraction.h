#ifndef _MPSINTERACTION_H
#define _MPSINTERACTION_H
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
#include "MpsEnums.h"
#include "MpsObjects.h"
#include "MpsInteractionBox.h"
/**
   \file
   Interactions based on range groupings.
   These interactions can be applied to particles from a system that 
   are found to be in a particular range in space, specified by axis-aligned bounding box.
*/

/** Interaction attributes to be applied to appropriate particles */
struct _MpsInteraction
{
    /** reference to the appropriate interaction box*/
    MpsInteractionBox *m_InteractionBox;
    /** the strength of the interaction on particles.  relates to mass/charge. */
    MeReal m_Strength;
    /** The range of the interaction effect */
    MeReal m_Range;
    /** amount of damping, a drag like effect, to apply to particles*/
    MeReal m_Damping;
    /** enum of interaction type.
	MpsAttractive,
	MpsRepulsive,
	MpsAttractiveRepulsive,
	MpsUserDefinedInteraction,
	MpsElectricLocalForce,
	MpsMagneticLocalForce.
    */
    MpsInteractionType m_Type;
    /** boolean for enabling / disabling an interation box*/
    int m_InteractionBoxIsEnabled;
    /** void * for [optional] user defined functions with interaction boxes */
    void (*m_UserFunction) (MpsSystem *, int, MpsSystem *, int);
};

/* create and delete */
extern MpsInteraction *NewInteraction(const MpsInteractionType);
/* create and delete */
extern void DeleteInteraction(MpsInteraction *);

/* External interface to evaluate interaction forces - 
    forces are added into the particles internally */
extern void CalculateInteraction(const MpsInteraction *interact, 
				 MpsSystem *Sys1, MpsSystem *Sys2);


#endif

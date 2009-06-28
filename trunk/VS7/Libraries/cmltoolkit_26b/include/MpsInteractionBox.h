#ifndef _MPSINTERACTIONBOX_H
#define _MPSINTERACTIONBOX_H
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
#include "MpsObjects.h"
#include "IntegerMatrix.h"
/**
   \file
   MpsInteraction Boxes for group interactions.
   Containers for Interaction boxes for use with particle systems.
*/

/** MpsInteraction box members. */
typedef struct _MpsInteractionBox
{
    /** The center of the box */
    MeVector3 m_BoxCenter;
    /** The dimensions of the box in XYZ directions */
    MeVector3 m_BoxDimensions;
    /** 1.0/(Size of cells) in X,Y,Z directions */
    MeVector3 m_InvBinDimensions;
    /** Number of cells in X,Y,Z directions */
    int m_NumX, m_NumY, m_NumZ;
    /** Interaction cutoff */
    MeReal m_CutOff;

    /** m_BinLists[i,j] = index of the jth particle in bin i */
    IntegerMatrix *m_BinLists;
    /** Integer Matrix containing the neighbour lists */
    IntegerMatrix *m_NeighbourBins;
    /** Array of number of particles in each bin */
    int *m_NumberInBin;
    /** C-style array of ints contining indices of bins in use,
     *  used for fast zeroing of bin lists
     */
    int *m_UsedBins;
    int m_NumOfUsedBins;
    /** C-style array of pointers to pairs of particles, ends with 0 */
    int *m_ParticleIndexPairs;

    int m_NumOfPairs;
    int m_MaxNumOfPairs;
    int m_MaxNumInBin;

    /** Flag to check whether the above has been changed,
     *  used to recalculate neighbour lists if necessary
     */
    int m_Changed;
}
MpsInteractionBox;



/* create an interaction box */
extern MpsInteractionBox *NewInteractionBox(void);
/* delete an interaction box */
extern void DeleteInteractionBox(MpsInteractionBox *);

/* External inteface to routines */
extern int *GetParticlePairList(MpsInteractionBox *, const MpsSystem *, const MpsSystem *);



#endif

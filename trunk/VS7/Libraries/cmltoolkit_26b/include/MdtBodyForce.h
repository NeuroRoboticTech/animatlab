#ifndef _MDTBODYFORCE_H
#define _MDTBODYFORCE_H
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
 * MdtBodyForce API functions.
 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif






/**
 *  Allocate an empty body force header from the respective pool.  An 
 *  array of MdtBodyID of size 3 is allocated from that pool as well and
 *  the pointer b is initialized to point to that.  User must overwrite
 *  this pointer if she wants a larger array.
 */
MdtBodyForceID MdtBodyForceCreate(MdtWorldID w); 


/**
   Call the user specified 'destroy'  callback, unhook force from the
   lists in the world, and return header memory to the pool.
 */
void MdtBodyForceDestroy(MdtBodyForceID f); 


/**
 * Call the user specified diable function, remove from enabled list in world.
 */
void MdtBodyForceDisable(MdtBodyForceID f); 


/**
 * If force is currently inactive, Invoke the user specified 'enable'  
 *  callback function, add to list of enabled (active) forces in world.  Otherwise,
 *  do nothing.  Return 0 if force gets activated and -1 otherwise.
 */
int MdtBodyForceEnable(MdtBodyForceID f); 


/**
 * Invoke the user specified 'enable'  callback function, add to list of
 * active forces in world.
 */
MeBool MdtBodyForceIsEnabled(MdtBodyForceID f); 


/**
 *  Given an MdtWorldID w, return the pointer to the head of the list of
 *  enabled MdtBodyForce objects.
 */
MdtBodyForceID  MEAPI MdtBodyForceGetFirst(const MdtWorldID w);
#define MDTBODYFORCEGETFIRST(w)   (w)->enabledForceHead;
/**
 *  Given an MdtBodyForceID, get pointer to next enabled force in list.
 */
MdtBodyForceID   MEAPI MdtBodyForceGetNext(const MdtBodyForceID f);
#define MDTBODYFORCEGETNEXT(f)   (f)->enabledNext







/**
 * Given an MdtBodyForceID, replace the default MdtBodyID array of size 3
 * with the user supplied pointer .  Your array must be a null terminated.
 * No memory management is done on this: it's up to you to free this array
 * at the end.  If the current body array was also set by the user, return
 * it.  Otherwise, return 0 ; 
*/
MdtBodyID *   MEAPI MdtBodyForceSetUserBodyArray(MdtBodyForceID f, MdtBodyID *B ); 


/**
 *   If this the body array has been allocated by the user, return the
 *   pointer.  Otherwise, return 0.  This is to allow the user to free the
 *   memory they have allocated.
 */
MdtBodyID *   MEAPI MdtBodyForceGetUserBodyArray(const MdtBodyForceID f);


/**
 *  Simple accessors/mutators for standard forces: just set the two bodies:
 *  either can be 0 if one wishes.
 */
void MEAPI MdtBodyForceSetBodies(MdtBodyForceID f, const MdtBodyID b1, const MdtBodyID b2);



/**
 * Just lookup the body pointer for the user.
 *  Code for bounds checking is provided but this could be slow.
 */
MdtBodyID MEAPI MdtBodyForceGetBody(MdtBodyForceID f, unsigned int index);

  /** 
   * Call the user 'disable' and 'destroy'  functions and reset everything: afterwards,
   * the force is disabled and the body pointer is reset to default.
   */
  void MEAPI MdtBodyForceReset(MdtBodyForceID f); 


/**
 *  Assigns callbacks and 
 */
int MEAPI   MdtBodyForceSetConfig(MdtBodyForceID f, int id, MdtBodyForceFn update, MdtBodyForceFn enable, 
				  MdtBodyForceFn disable, MdtBodyForceFn
				  destroy, void *data); 







#ifdef __cplusplus
}
#endif


#endif

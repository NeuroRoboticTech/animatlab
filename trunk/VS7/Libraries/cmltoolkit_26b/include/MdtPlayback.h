#ifndef __MDTPLAYBACK_H_
#define  __MDTPLAYBACK_H_


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




#include "Mdt.h"

#ifdef __cplusplus
extern "C"
{
#endif



/*

   Interface to kinematic store/playback module. 

   This is intended to store/playback the kinematic data of the rigid
   bodies in an MdtWorld from and to memory and from there, to and from a
   file.  This is useful to record sequences of an application for later
   playback at which time the frames can be dumped to make movies for
   instance.

   The only data stored here is that of the rigid bodies and specifically,
   the position, orientation, linear and angular velocities; an integer
   tag is provided to match the bodies to its data.  This is
   organized in a struct which are organized into an array.  A second
   struct holds the bookkeeping data: frame number, how many rigid bodies
   in that frame, and the viewpoint for that frame.

   Two resizable arrays are provided for each type of data.

   When the whole thing is dumped into a file, we first write a string that
   identifies which format is used and a magic key to negotiate with the
   loading application.  Then, each of elements of the two arrays are dumped
   in binary format in an overleafed manner.
   
   
*/




/*
  Barebones kinematic data for a rigid body.
*/
  
typedef struct _MdtBodyKinematicData
{
  int id; 
  MeVector3 x;		/* position */
  MeVector4 q;		/* Euler parameters */
  MeVector3 v;		/* velocity */
  MeVector3 w;		/* angular velocity */
} MdtBodyKinematicData;


/* resizeable array type for kinemtaic data array */
typedef struct _MdtBodyKinematicDataVector
{
  int size; 
  int capacity; 
  MdtBodyKinematicData *v ; 
} MdtBodyKinematicDataVector ; 



/*
  Kinematic data for a whole world including the viewpoint into the scene.
 */

typedef struct _MdtWorldKinematicData
{
  int                         frame;	/* time stamp  */
  int                         nb;	/* current number of bodies */
  MeMatrix4                   vp;	/* xform for the current viewpoint */
  int                         idx;	/* index to the first kinematic data structure in the vector */
  MdtBodyKinematicDataVector   *v;      /* the vector on which the data is stored */

} MdtWorldKinematicData ; 


/* resizeable array type for kinemtaic data array */
typedef struct _MdtKinematicDataVector
{
  int size;			/* current size of array */
  int capacity;			/* total allocated memory  */
  MdtWorldKinematicData *v ; 
} MdtWorldKinematicDataVector ; 



/* API for body kinematic data vector */ 


/* create new array with size 0 */
MdtBodyKinematicDataVector * MEAPI 
MdtBodyKinematicDataVectorCreate(void); 

/* cleanup all memory */

MdtBodyKinematicDataVector * MEAPI 
MdtBodyKinematicDataVectorDestroy(MdtBodyKinematicDataVector * v); 


/*    reset new size with realloc */
int MEAPI 
MdtBodyKinematicDataVectorResize(MdtBodyKinematicDataVector * v,  size_t new_size); 

int MEAPI 
MdtBodyKinematicDataVectorAppend(MdtBodyKinematicDataVector *v, MdtBodyKinematicData *kd); 

int MEAPI 
MdtBodyKinematicDataVectorAppendDirect(MdtBodyKinematicDataVector *v, MdtBodyID b);

/* same thing for world kinematic data */ 
MdtWorldKinematicDataVector * MEAPI 
MdtWorldKinematicDataVectorCreate(); 


MdtWorldKinematicDataVector * MEAPI 
MdtWorldKinematicDataVectorDestroy(); 

int MEAPI 
MdtWorldKinematicDataVectorResize(MdtWorldKinematicDataVector *v, size_t new_size); 

int MEAPI 
MdtWorldKinematicDataVectorAppend(MdtWorldKinematicDataVector *v, MdtWorldKinematicData *kd); 

int MEAPI 
DumpKinematicDataToFile(); 

int MEAPI 
FlushFrameDataToFile(int start_frame, int end_frame); 

int MEAPI 
LoadKinematicDataFromFile(); 

int MEAPI 
LoadFrameDataFromFile(int start_frame, int end_frame); 


/* reset rigid body data from a kinematic data set */
int MEAPI 
MdtBodyFromKinematicData(MdtBodyID b, const MdtBodyKinematicData *kd) ; 

/* get kinematic data from rigid body */
int MEAPI 
MdtBodyToKinematicData(MdtBodyKinematicData *kd, const MdtBodyID b) ; 

/* loop over all bodies in the world and dump kinematic data */ 
int MEAPI
MdtWorldStepFromKinematicData(MdtWorldID w, const MdtWorldKinematicData *kd); 


/* loop over all bodies in the world and copy kinematic data */ 
int MEAPI
MdtWorldStoreKinematicData(MdtWorldKinematicData *kd , const MdtWorldID w); 
  
#ifdef __cplusplus
}
#endif

#endif

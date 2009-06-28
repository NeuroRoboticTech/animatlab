#ifndef _METHREAD_H
#define _METHREAD_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:07 $
   
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
    Simple cross platform thread library. 

  @warning This library is experimental and is not supported.

These functions are not used inside Vortex, only by some examples.

Vortex can now work in the following multithreaded mode in
certain licensing configurations. Note that currently, the
flexlm licensing does not support multithreading.

Initialization (McdInit, geometry registration) must be done
in a single thread.
Mst must be used to create objects(MstFixedModelCreate, MstModelAndBodyCreate)
and step the system.
Several threads can then be spawned that optionally each
create objects and call MstUniverseStep.

If more than one universe is spawned, McdInit+registration
must be called at initialization time.

Convex meshes are not thread safe at the moment.

It is possible to call Vortex functions that modify state
or create/destroy objects from a different thread 
(eg. MdtBodyAddForce, ...) but the user is responsible
for wrapping a mutex around those calls and MstUniverseStep
to ensure correctness.

MeViewer is typically run in the main thread. 
It would be best to copy the matrices between the viewer and
universe while both are waiting for the copy (mutex).

MeViewer can also be launched in a separate thread as long
as creating the render context and calling RRun is
done in the same thread.

*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN32
#define METHREAD_DECL __cdecl
typedef struct MeThread *MeThreadID;

#define METHREAD_RETURN void
#define METHREAD_DO_RETURN()

#else
#include <pthread.h>

#define METHREAD_DECL
#define METHREAD_RETURN void*
#define METHREAD_DO_RETURN() return 0;

typedef pthread_t  MeThreadID;
#endif  

struct MeThread;
struct MeThreadMutex;
typedef struct MeThreadMutex *MeThreadMutexID;
struct MeThreadCondition;
typedef struct MeThreadCondition *MeThreadConditionID;

typedef METHREAD_RETURN (METHREAD_DECL *MeThreadFunctionPtr)(void *);


MeThreadID          MeThreadCreate(MeThreadFunctionPtr function, void *data);
MeThreadID          MeThreadGetID();
void                MeThreadExit();
void                MeThreadSleep(unsigned long miliseconds);

MeThreadMutexID     MeThreadMutexCreate();
void                MeThreadMutexDestroy(MeThreadMutexID mutex);
int                 MeThreadMutexLock(MeThreadMutexID mutex);
int                 MeThreadMutexUnlock(MeThreadMutexID mutex);


#ifdef __cplusplus
}
#endif

#endif

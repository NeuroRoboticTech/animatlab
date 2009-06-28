#ifndef __RECORDER_H_
#define  __RECORDER_H_

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:01 $
   
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
#include "MdtPlayback.h"

#include "Mps.h"
#include "MpsPlayback.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef int (MEAPI *MdtRecorderDumpCallback)(char* basename, int i, void* rc);
typedef void (MEAPI *MpsRecorderStepCallback)(MeReal step);

typedef struct MdtRecorder
{
  int currentFrame;
  int startFrame;  /* first frame to be replayed */
  int endFrame;    /* last frame to be replayed */
  int frameCount;  /* number of frames recorded */
  int frameStep;   /* number of frames to step by during replay */
	int dumpFrame;   /* the sequential frame number to use when dumping sequential */

  int integrating; /* state: are we doing dynamics? */
  int recording;   /* state: are we storing it to memory? */
  int dumping;     /* dumping frames to disk via callback */
	int dumpSequential;   /* Tells whether to dump the frame filenames sequentially instead of using actual frame count.*/
  int paused;      /* 2 = singlestep, 1 = paused */
  int sequence;    /* incremented everytime a new recording is done. starts at 0 */

  MdtWorldID world;
  MdtBodyKinematicDataVector *vb; 
  MdtWorldKinematicDataVector *vw;

  /* particle data */
  MpsKinematicData *ps;
  MpsRecorderStepCallback psStep;
  MeReal psStepData;

  MdtRecorderDumpCallback dumpCB;
  char* dumpBaseFileName;
  char* sequenceBaseFileName;
  void* dumpCBUserData;
  
} MdtRecorder;

void MEAPI MdtRecorderInit(MdtRecorder* r, MdtWorldID w, MdtRecorderDumpCallback diskDumpFn, char* diskDumpBaseFilename, void* diskDumpUserData);

void MEAPI MdtRecorderInitWithPS(MdtRecorder* r, MdtWorldID w, MpsManager *pm, MpsRecorderStepCallback psstep, MeReal psstepData,
                                 MdtRecorderDumpCallback diskdumpFn, char*filename, void* diskdumpUserData);

void MEAPI MdtRecorderTerm(MdtRecorder*r);
void MEAPI MdtRecorderReset(MdtRecorder *r);
void MEAPI MdtRecorderStartRecording(MdtRecorder *r);
void MEAPI MdtRecorderStopRecording(MdtRecorder *r);
void MEAPI MdtRecorderRewind(MdtRecorder *r);
int  MEAPI MdtRecorderGetCurrentFrame(MdtRecorder *r);
int  MEAPI MdtRecorderGetFrameCount(MdtRecorder *r);
void MEAPI MdtRecorderSetCurrentFrame(MdtRecorder *r, int f);
void MEAPI MdtRecorderStartPlayback(MdtRecorder *r,int frameStep, int isDumping, char *optionalDiskDumpBaseFilename);
void MEAPI MdtRecorderStopPlayback(MdtRecorder *r);
void MEAPI MdtRecorderSetStartFrame(MdtRecorder *r, int f);
void MEAPI MdtRecorderSetEndFrame(MdtRecorder *r, int f);
int  MEAPI MdtRecorderGetStartFrame(MdtRecorder *r);
int  MEAPI MdtRecorderGetEndFrame(MdtRecorder *r);
void MEAPI MdtWorldStepOrPlay(MdtRecorder* r, MeReal timeStep);
void MEAPI MdtRecorderSingleStep(MdtRecorder *r, int dir);
void MEAPI MdtRecorderContinuous(MdtRecorder *r, int dir);
void MEAPI MdtRecorderPause(MdtRecorder *r);
int  MEAPI MdtRecorderIsPaused(MdtRecorder *r);
void MEAPI MdtRecorderStartSingleStep(MdtRecorder *r, int frameStep, int isDumping, char*filename);
void MEAPI MdtRecorderStoreFrame(MdtRecorder* r);

  struct MstUniverse;
  
void MEAPI MstUniverseStepOrPlay(const struct MstUniverse *u, MdtRecorder* rec, const MeReal stepSize);
#ifdef __cplusplus
}
#endif

#endif

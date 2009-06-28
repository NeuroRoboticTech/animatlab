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

#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include "MeViewer.h"
#include "MdtRecorder.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
void MEAPI toggleRecording(RRender *rc, void * userData);

void MEAPI toggleKinematicRecording(RRender *rc, void *userData);
void MEAPI play(RRender *rc, void *userData, int dir);
void MEAPI playForward(RRender *rc, void *userData);
void MEAPI playBackward(RRender *rc, void *userData);
void MEAPI playGotoBegin(RRender *rc, void *userData);
void MEAPI playGotoEnd(RRender *rc, void *userData);
void MEAPI playForwardToDisk(RRender *rc, void *userData);
void MEAPI playRecording(RRender *rc, void *userData);
void MEAPI playEnd(RRender *rc, void *userData);
*/



void MEAPI MeAppRecorderInit(RRender *rc, MdtRecorder *r);
int MEAPI MeAppRecorderDumpPPM(char* basename, int i, void* rc);
int MEAPI MeAppRecorderDumpBMP(char* basename, int i, void* rc);

#ifdef __cplusplus
}
#endif

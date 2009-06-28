#ifndef _MECOMMANDLINE_H
#define _MECOMMANDLINE_H
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
 * Handling of command line parameters.
 */

#include "MeCall.h"
#include "MeMemory.h"
#include "MeMessage.h"
#include "MePrecision.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MeCommandLineOptions
{
    int m_argc;
    const char ** p_argv;
} MeCommandLineOptions;

MeCommandLineOptions* MEAPI MeCommandLineOptionsCreate(const int argc, const char * argv[]);
void MEAPI MeCommandLineOptionsDestroy(MeCommandLineOptions* options);
void MEAPI MeCommandLineOptionsEat(MeCommandLineOptions* options, const int* eat, const int num_eat);
int MEAPI MeCommandLineOptionsGetPos(MeCommandLineOptions* options, const char * arg);
MeBool MEAPI MeCommandLineOptionsCheckFor(MeCommandLineOptions* options, const char * arg, const MeBool eat);
MeBool MEAPI MeCommandLineOptionsCheckForList(MeCommandLineOptions* options, const char * arglist[], const MeBool eat);
int MEAPI MeCommandLineOptionsGetNumeric(MeCommandLineOptions* options, const char * arg, const MeBool eat);
double MEAPI MeCommandLineOptionsGetFloat(MeCommandLineOptions* options, const char * arg, const MeBool eat);
char * MEAPI MeCommandLineOptionsGetString(MeCommandLineOptions* options, const char * arg, const MeBool eat);
int MEAPI MeCommandLineOptionsGetFloats(MeCommandLineOptions* options, const char * arg, const MeBool eat, MeReal *out, int size);


#ifdef __cplusplus
}
#endif

#endif /* _MECOMMANDLINE_H */

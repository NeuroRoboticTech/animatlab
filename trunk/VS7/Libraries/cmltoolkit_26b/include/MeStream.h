#ifndef _MESTREAM_H
#define _MESTREAM_H
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
    Streaming functions. This is not a proper streaming implementation.
    For platforms that support streaming, MeStream just wraps fopen/fclose/
    fread/fwrite. For platforms that don't support streaming open/close/read/
    write are used. MeStream is used in case we want to implement a full
    streaming solution on all platforms.
*/

#include "MeSimpleFile.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _MeStream _MeStream;
typedef _MeStream* MeStream;

struct _MeStream
{
    char *filename;
#ifndef PS2
    void *handle;
#else
    /* On PS2 we load the whole file into memory at the start to speed
       things up a little! */
    int handle;
    char* buffer;
    int bufferSize;
    int whereAmI;
    MeBool modified;
#endif
};

MeStream      MEAPI MeStreamOpen(const char *filename,MeOpenMode_enum mode);
MeStream      MEAPI MeStreamOpenWithSearch(const char *filename,MeOpenMode_enum mode);
void          MEAPI MeStreamClose(MeStream stream);
size_t        MEAPI MeStreamRead(void *buffer,size_t size,size_t count,MeStream stream);
char *        MEAPI MeStreamReadLine(char *string, int n, MeStream stream);
size_t        MEAPI MeStreamWrite(void *buffer,size_t size,size_t count,MeStream stream);



#ifdef __cplusplus
}
#endif

#endif

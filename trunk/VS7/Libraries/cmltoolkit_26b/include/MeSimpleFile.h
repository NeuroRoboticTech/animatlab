#ifndef _MESIMPLEFILE_H
#define _MESIMPLEFILE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/10/21 19:38:23 $
   
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
    Cross platform wrappers for simple file handling.
*/

#include <stdio.h>
#include <stdarg.h>
#include "MeCall.h"
#include "MePrecision.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * File modes.
 */
typedef enum
{
    kMeOpenModeRDONLY,  /**< Open as read only. */
    kMeOpenModeWRONLY,  /**< Open new file for writing. */
    kMeOpenModeWRONLYBINARY,  /**< Open new file for writing in binary. */
    kMeOpenModeRDWR,    /**< Open existing file (for appending?). */
    kMeOpenModeRDBINARY /**< Open for binary read. */
} MeOpenMode_enum;

/** MeLseek constants. Defines the starting point from which we travel
    @param offset bytes.
*/
typedef enum
{
    kMeSeekSET, /** < Start of file. */
    kMeSeekCUR, /** < Current file pointer position. */
    kMeSeekEND  /** < End of file. */
} MeSeekOrigin_enum;


/** Opens a file.*/
int               MEAPI MeOpen(const char *filename, MeOpenMode_enum mode);

/** Searches Vortex paths and MeOpens file */
int               MEAPI MeOpenWithSearch(const char *filename,MeOpenMode_enum mode);

/** Searches Vortex paths and locates file */
const char *      MEAPI MeGetFileLocation(const char *filename);

/** Closes a file */
int               MEAPI MeClose(int file);

/** Reads from a file */
int               MEAPI MeRead(int file, void * buf, int count);

/** Writes to a file */
int               MEAPI MeWrite(int file, void * buf, int count);

/** Seeks within a file */
int               MEAPI MeLseek(int file, int offset,
                            MeSeekOrigin_enum origin);

/** Get search path from array */
const char *      MEAPI MeGetDefaultFileLocation(int i);

/** Gets the contents of a file and puts it into memory */
MeU8* MEAPI MeLoadWholeFile(const char* name, int* size);

/** Gets the contents of a file and puts it into memory */
MeU8* MEAPI MeLoadWholeFileWithSearch(const char* name, int* size);

/** Save a block of memory to a file */
void MEAPI MeSaveWholeFile(const char* name, const MeU8* data, const int size);

const char * MEAPI MeGetExecutablePath( char * path , size_t buffsz);
void MEAPI MeFileSearchPathAdd(const char *path);
void MEAPI MeFileSearchPathDestroy();
void MEAPI MeFileSearchPathAddAllRelativeDefaults(const char *path);

#ifdef __cplusplus
}
#endif


#endif /* _MESIMPLEFILE_H */

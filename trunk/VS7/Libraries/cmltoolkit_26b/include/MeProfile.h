#ifndef _MEPROFILE_H
#define _MEPROFILE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/04/15 11:05:24 $
   
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
 * Public profiling API
 */

#include "MePrecision.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Profiling output styles */
typedef enum {
  kMeProfileOutputAverage, /**< A compact average display (default) */
  kMeProfileOutputNormal,  /**< Lists everything sequentially */
  kMeProfileOutputGnuplot  /**< Produces several files in Gnuplot format */
} MeProfileOutput_enum;

typedef struct _MeProfileTimerResult {
    MeU64 cpuCycles;
    MeU64 count0;
    MeU64 count1;
} MeProfileTimerResult;

/** Counter modes. These areonly relevant for PS2 and IRIX */
typedef enum {
    /** Counts floating point operations */
    kMeProfileCounterModeFlops,
    /** Counts cache misses */
    kMeProfileCounterModeCache
} MeProfileCounterModes_enum;

/** Timer mode. These settings are only relevant for PS2 */
typedef struct {
    unsigned char granularity;  /**< Granularity */
    MeProfileCounterModes_enum counterMode;   /**< Counter mode */
} MeProfileTimerMode;

/** Output style parameters */
typedef struct _MeProfileOutput {
    MeProfileOutput_enum style; /**< Output style */
    float jaggedness;           /**< For gnuplot */
    unsigned char settletime;   /**< For gnuplot */
} MeProfileOutput;


/** Initialises profiling */
void              MEAPI MeProfileStartTiming(MeProfileTimerMode mode,
                            MeBool logging);

/** Cleans up profiling */
void              MEAPI MeProfileStopTiming();

/** Indicates where in the code a frame is said to have started. */
void              MEAPI MeProfileStartFrame();

/** Indicates that the end of a frame has been reached. */
void              MEAPI MeProfileEndFrame();

/** Start timing a section of code.
 *  @param codeSection Name of the section to be timed
 *  @param autoStop If 1 then the timing of this section of code will
 *                  stop automatically at the next call to %MeProfileStartSection().
 *                  If 0 then an explicit call to %MeProfileEndSection() will
 *                  have to be made. This allows for concurrent sections of
 *                  timed code.
 */
#ifdef _ME_NOPROFILING
#define MeProfileStartSection(x,y)
#else
#define MeProfileStartSection(x,y) MeProfileStartSectionFn(x, y)
void              MEAPI MeProfileStartSectionFn(const char* codeSection,
                            unsigned char autoStop);
#endif

/** Explicitly stops timing the specified section of code.
 *  You should not call this if you set autostop to 1 in MeProfileStartSection().
 */
#ifdef _ME_NOPROFILING
#define MeProfileEndSection(x) 0
#else
#define MeProfileEndSection(x) MeProfileEndSectionFn(x)
MeReal              MEAPI MeProfileEndSectionFn(const char* codeSection);
#endif

MeReal MEAPI MeProfileEndSectionAndReset(const char* cs);

/** Gets timer value */
MeProfileTimerResult MEAPI MeProfileGetTimerValue();

/** Can be called just before the end of a frame
 *  to make sure all timers are stopped.
 */
void              MEAPI MeProfileStopTimers();

/** Output profile results in the currently selected style. */
void              MEAPI MeProfileOutputResults();

MeReal            MEAPI MeProfileGetSectionTime(const char* codeSection);

MeReal            MEAPI MeProfileGetAllSectionTime();

MeU64             MEAPI MeProfileGetClockSpeed();

/** Sets the ouput mode for profiling information. */
void              MEAPI MeProfileSetOutputParameters(MeProfileOutput output);

#if defined WIN32
void              MEAPI MeProfileLimit(unsigned seconds);
#endif


typedef struct MeTimer {
    MeProfileTimerResult startTime;
    MeProfileTimerResult endTime;
    MeProfileTimerResult totalTime;
    unsigned char isRunning;
} MeTimer;

void              MEAPI MeTimerReset(MeTimer* timer);
void              MEAPI MeTimerStart(MeTimer* timer);
MeReal            MEAPI MeTimerStop(MeTimer* timer);
MeReal            MEAPI MeTimerGetTotalTime(MeTimer* timer);
MeReal            MEAPI MeTimerGetLastTime(MeTimer* timer);


#ifdef __cplusplus
}
#endif

#endif /* _MEPROFILE_H */

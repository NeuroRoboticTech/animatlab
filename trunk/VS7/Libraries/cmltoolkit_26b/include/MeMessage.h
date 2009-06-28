#ifndef _MEMESSAGE_H
#define _MEMESSAGE_H
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
 * Message handling API.
 */

#include "MeCall.h"
#include <stdarg.h>

#ifdef __cplusplus
  extern "C" {
#endif

/* Maximum message size in characters. This should not be redefined by
   client programs; it is compiled in the default message handling
   functions. */

#define MeMAXMESSAGE    5000

/** @var MeInfoLevel
 * Informational messages of higher levels are not printed.
 */
/** @var MeWarningLevel
 * Warning messages of higher levels are not printed.
 */
/** @var MeFatalErrorLevel
 * Fatal error messages of higher levels are not printed.
 */
/** @var MeDebugLevel
 * Debug messages of higher level are not printed.
 */

extern int              MeInfoLevel;
extern int              MeWarningLevel;
extern int              MeFatalErrorLevel;
extern int              MeDebugLevel;

/**
 * The type of a pointer to an output function for the default handler
 * functions
 */
typedef void            (MEAPI *MeShow)(const int level,const char *const string);

/** @var MeInfoShow
 * Default information message handler output function.
 */
/** @var MeWarningShow
 * Default warning message handler output function.
 */
/** @var MeFatalErrorShow
 * Default fatal error message handler output function.
 */
/** @var MeDebugShow
 * Default debug message handler output function.
 */

extern MeShow           MeInfoShow;
extern MeShow           MeWarningShow;
extern MeShow           MeFatalErrorShow;
extern MeShow           MeDebugShow;

/* Default message output functions */

void              MEAPI MeShowStdout(const int level,const char *const string);
void              MEAPI MeShowStderr(const int level,const char *const string);
#if (WIN32 && _MSC_VER)
  void            MEAPI MeShowDialog(const int level,const char *const string);
  void            MEAPI MeShowCrtWarn(const int level,const char *const string);
  void            MEAPI MeShowCrtErr(const int level,const char *const string);
#endif

/* With these you can change the default handling functions */

/**
 * The type of a pointer to a handler function.
 */
typedef void            (MEAPI *MeHandler)(const int level,
                          const char *const format,va_list ap);

/** @var MeInfoHandler
 * Information message handler pointer.
 */
/** @var MeWarningHandler
 * Warning message handler pointer.
 */
/** @var MeFatalErrorHandler
 * Fatal error message handler pointer.
 */
/** @var MeDebugHandler
 * Debug message handler pointer.
 */

extern MeHandler        MeInfoHandler;
extern MeHandler        MeWarningHandler;
extern MeHandler        MeFatalErrorHandler;
extern MeHandler        MeDebugHandler;

/* Default message handler functions */

void              MEAPI MeHandlerInfo(const int level,
                          const char *const format,va_list ap);
void              MEAPI MeHandlerWarning(const int level,
                          const char *const format,va_list ap);
void              MEAPI MeHandlerFatalError(const int level,
                          const char *const format,va_list ap);
void              MEAPI MeHandlerDebug(const int level,
                          const char *const format,va_list ap);

/* With these you can change the current print function or the message
   handler itself. As customary, these functions return the current
   value, so you can redefine easily for the duration of a block as in:

     {
       MeHandler restore = MeSetDebugHandler(newhandler);

       ....

       MeDebugHandler = restore;
     }

   which is nice and clean. */

MeShow            MEAPI MeSetInfoShow(MeShow n);
MeShow            MEAPI MeSetWarningShow(MeShow n);
MeShow            MEAPI MeSetFatalErrorShow(MeShow n);
MeShow            MEAPI MeSetDebugShow(MeShow n);

MeHandler         MEAPI MeSetInfoHandler(MeHandler n);
MeHandler         MEAPI MeSetWarningHandler(MeHandler n);
MeHandler         MEAPI MeSetFatalErrorHandler(MeHandler n);
MeHandler         MEAPI MeSetDebugHandler(MeHandler n);

/* These are the messaging functions. They will invoke the handlers
   above, and the default ones will print the type of message, its level
   and the message itself, which can be built using the same facilities
   as 'printf' (format string, argument list). */

/* Ordinary messages */

#define ME_REPORT(x) \
    do {           \
        MeInfo(0,"(%s line %d): \n", MeShortenPath(__FILE__), __LINE__); \
        x;         \
    } while (0)

void              MEAPI MeInfo(const int level,
                          const char *const format,...);
void              MEAPI MeWarning(const int level,
                          const char *const format,...);
void              MEAPI MeFatalError(const int level,
                          const char *const format,...);

/* Debugging messages */

void              MEAPI MeDebug(const int level,
                          const char *const format,...);

char             *MEAPI MeShortenPath(char *path);

#ifdef _MECHECK
#define MEASSERTMSG(x,msg) { if (! (x)) { MeFatalError msg; }}
#else
#define MEASSERTMSG(x,msg)
#endif

#ifdef __cplusplus
}
#endif

#endif

#ifndef _MEXMLWRITE_H
#define _MEXMLWRITE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:03 $
   
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
    XML writing functions.
*/

#include <stdarg.h>
#include "MeStream.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_XML_LINE_LENGTH     500

void              MEAPI MeXMLWriteStartTag(const char *tag, MeStream stream);
void              MEAPI MeXMLWriteStartTagAttrib(const char *tag, MeStream stream, const char *attribFormat,...);

void              MEAPI MeXMLWriteEndTag(const char *tag, MeStream stream);
void              MEAPI MeXMLWrite(const char *tag, MeStream stream,
                            const char *format,...);
void              MEAPI MeXMLWriteAttrib(const char *tag,MeStream stream,const char* attrib, 
                            const char *format,...);
void              MEAPI MeXMLSetTopLevelIndent(int indent);
int               MEAPI MeXMLGetTopLevelIndent(void);
void              MEAPI MeXMLIndent(void);
void              MEAPI MeXMLDeIndent(void);
void              MEAPI MeXMLSetIndent(int indent);
int               MEAPI MeXMLGetIndent(void);
const char *      MEAPI MeXMLGetVersion(void);

#ifdef __cplusplus
}
#endif


#endif /* _MEXMLWRITE_H */

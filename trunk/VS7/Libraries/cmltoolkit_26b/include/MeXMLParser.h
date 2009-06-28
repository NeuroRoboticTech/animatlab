#ifndef _MEXMLPARSER_H
#define _MEXMLPARSER_H
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
 *  XML parser.
 */

#include <stddef.h>
#include "MeCall.h"
#include "MeStream.h"
#include "MePrecision.h"

#define ME_XML_TAG_BUFFER_SIZE 256
#define ME_XML_DATA_BUFFER_SIZE 1024

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MeXMLInput {
  char buffer[4096];
  int bufptr;
  int bufmax;
  char stack[8];
  int top;
  MeStream stream;
  int line;
  int posn;
  int eof;
  void *userdata;
  char error[ME_XML_DATA_BUFFER_SIZE+256];
} MeXMLInput;

typedef enum _MeXMLError
{
    MeXMLErrorNone,
    MeXMLErrorMalformed,        /* XML is malformed */
    MeXMLErrorInvalidValue,     /* invalid data value  */
    MeXMLErrorEOF,              /* unexpected EOF */
    MeXMLErrorParseFail,        /* XML may be OK, but parse failed with e.g. buffer overflow */
    MeXMLErrors
} MeXMLError;


typedef enum _ActionType {
  MeXMLActionCallback,
  MeXMLActionParse,
  MeXMLActionEnd
} MeXMLActionType;

typedef struct _MeXMLElement {
  char name[ME_XML_DATA_BUFFER_SIZE];   /* whatever's inside the <> */
  MeXMLInput * fi;  /* the input */
  int level;        /* how deep */
  char *attr;       /* ptr to attributes if any, else 0 */
} MeXMLElement;

typedef struct _MeXMLHandler {
  char *name;             /* corresponding tag */
  MeXMLActionType type;   /* type */
  void *fn;               /* function to call */
  MePtrSizeInt offset;    /* struct offset to parse into */
  unsigned int max;       /* expected number of elts for array */
  unsigned int maxstr;    /* max length of string. Used by string array handler */
  MeBool called;          /* 1 if handler was called, ie if tag was in XML
                             file, 0 otherwise */

/* optional callback that gets called after all handlers except an element handler
   has been called. The first arg is a ptr to the filled structure.
   The second arg is the userdata associated with MeXMLInput
*/

  MeXMLError (MEAPI *cb)(void *,void *);
} MeXMLHandler;


typedef MeXMLError (MEAPI *MeXMLCallback)(const MeXMLElement *);
typedef MeXMLError (MEAPI *MeXMLParseFn)(MeXMLInput *, const MeXMLHandler *, void *);


/*
  Parsing functions
*/

MeXMLError MEAPI
MeXMLParseUInt(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseUIntArray(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseInt(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseIntArray(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseFloat(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseFloatArray(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseMeReal(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseMeRealArray(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseDouble(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseDoubleArray(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseString(MeXMLInput *, const MeXMLHandler *, void *);

MeXMLError MEAPI
MeXMLParseStringArray(MeXMLInput *, const MeXMLHandler *, void *);

#define ME_XML_MEMBER_OFFSET(object, member)   offsetof(object, member)

#define ME_XML_ELEMENT_HANDLER(name, proc) \
    { name, MeXMLActionCallback, (void*)proc, 0, 0, 0, 0, 0 }

#define ME_XML_INT_HANDLER(name, object, member, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseInt, ME_XML_MEMBER_OFFSET(object, member), 0, 0, 0, cb}

#define ME_XML_INT_ARRAY_HANDLER(name, object, member, number, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseIntArray, ME_XML_MEMBER_OFFSET(object, member), number, 0, 0, cb}

#define ME_XML_UINT_HANDLER(name, object, member, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseUInt, ME_XML_MEMBER_OFFSET(object, member), 0, 0, 0, cb}

#define ME_XML_UINT_ARRAY_HANDLER(name, object, member, number, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseUIntArray, ME_XML_MEMBER_OFFSET(object, member), number, 0, 0, cb}

#define ME_XML_FLOAT_HANDLER(name, object, member, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseFloat, ME_XML_MEMBER_OFFSET(object, member), 0, 0, 0, cb}

#define ME_XML_FLOAT_ARRAY_HANDLER(name, object, member, number, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseFloatArray, ME_XML_MEMBER_OFFSET(object, member), number, 0, 0, cb}

#define ME_XML_MEREAL_HANDLER(name, object, member, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseMeReal, ME_XML_MEMBER_OFFSET(object, member), 0, 0, 0, cb}

#define ME_XML_MEREAL_ARRAY_HANDLER(name, object, member, number, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseMeRealArray, ME_XML_MEMBER_OFFSET(object, member), number, 0, 0, cb}

#define ME_XML_DOUBLE_HANDLER(name, object, member, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseDouble, ME_XML_MEMBER_OFFSET(object, member), 0, 0, 0, cb}

#define ME_XML_DOUBLE_ARRAY_HANDLER(name, object, member, number, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseDoubleArray, ME_XML_MEMBER_OFFSET(object, member), number, 0, 0, cb}

#define ME_XML_STRING_HANDLER(name, object, member, maxLen, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseString, ME_XML_MEMBER_OFFSET(object, member), maxLen, 0, 0, cb}

#define ME_XML_STRING_ARRAY_HANDLER(name, object, member, maxLen, number, cb) \
    { name, MeXMLActionParse, (void*)MeXMLParseStringArray, ME_XML_MEMBER_OFFSET(object, member), maxLen, number, 0, cb}

#define ME_XML_HANDLER_END         { (char *)NULL, MeXMLActionEnd }



MeXMLInput *     MEAPI   MeXMLInputCreate(MeStream file);
void             MEAPI   MeXMLInputDestroy(MeXMLInput *input);
void             MEAPI   MeXMLInputSetUserData(MeXMLInput * input, void *userData);
void *           MEAPI   MeXMLInputGetUserData(const MeXMLInput * input);
MeXMLError       MEAPI   MeXMLInputProcess(MeXMLInput * input, MeXMLHandler handlers[]);
const char *     MEAPI   MeXMLInputGetErrorString(const MeXMLInput *input);
MeXMLError       MEAPI   MeXMLElementProcess(MeXMLElement *elem, MeXMLHandler handlers[], void *data);
MeXMLInput *     MEAPI   MeXMLElementGetInput(MeXMLElement *elem);
MeBool           MEAPI   MeXMLHandlerWasCalled(MeXMLHandler handlers[],char *name);
void             MEAPI   MeXMLElementHandlerCreate(MeXMLHandler *handler,char *name,void *fn);
void             MEAPI   MeXMLElementHandlerDestroy(MeXMLHandler *handler);
void             MEAPI   MeXMLElementParseEntryCreate(MeXMLHandler *handler,char *name,void *fn, int size, void*ptr);
void             MEAPI   MeXMLElementParseEntryDestroy(MeXMLHandler *handler);

#ifdef __cplusplus
}
#endif

#endif /* _MEXMLPARSER_H */

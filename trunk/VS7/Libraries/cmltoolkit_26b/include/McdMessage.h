#ifndef _MCDMESSAGE_H
#define _MCDMESSAGE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:40 $
   
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

#include "MePrecision.h"

enum
{
  kMcdErrorTypeFatal,
  kMcdErrorTypeWarning,
  kMcdErrorTypeInfo
};

/* Define enum for all error numbers: */
enum {
#define ERR(errNum,errType,msg) errNum
#include "McdCoreErrorList.h"
#undef ERR
};

struct McdErrorDescription
{
  MeI16 m_errNum;
  MeI16 m_errorLevel;
  MeI16 m_errorCount;
  const char* m_description;
};

extern McdErrorDescription gMcdCoreErrorList[];

void McdError(McdErrorDescription* ErrorList, int errorCode, const char *message);

#define McdCoreError(errorCode,message) McdError(gMcdCoreErrorList,(errorCode),(message))

#endif /* _MCDMESSAGE_H */

#if !defined(VX_MESSAGE_H)
#define VX_MESSAGE_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/03/22 15:55:48 $
   
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

// VxMessage.h: interface for error messages
//
//////////////////////////////////////////////////////////////////////

#include "MeMessage.h"

namespace Vx
{

#if defined(_MECHECK) || defined(_DEBUG)

#define VX_FATAL_ERROR(messagearglist) MeFatalError(0, messagearglist )
#define VX_INFO(messagearglist) MeInfo(0, messagearglist )
#define VX_WARNING(messagearglist) MeWarning messagearglist 
#define VX_WARNING_IF(expression, messagearglist) \
            if ((expression)) { VX_WARNING(messagearglist); }
#define VX_WARNING_ONCE(messagearglist) \
{ \
   static bool messageOutput = false; \
   if (!messageOutput) { VX_WARNING(messagearglist); messageOutput = true; } \
}
#define VX_WARNING_ONCE_IF(expression, messagearglist) \
if ((expression)) { \
   static bool messageOutput = false; \
   if (!messageOutput) { VX_WARNING(messagearglist); messageOutput = true; } \
}
#define VX_ASSERT(expression, messagearglist) \
            if (!(expression)) { VX_FATAL_ERROR(messagearglist); }
#else
#define VX_WARNING(arglist) ((void)0)
#define VX_FATAL_ERROR(messagearglist) ((void)0)
#define VX_INFO(messagearglist) MeInfo(0, messagearglist )
#define VX_WARNING_IF(expression, messagearglist, optionalelsecommand) ((void)0)
#define VX_WARNING_ONCE(messagearglist) ((void)0)
#define VX_WARNING_ONCE_IF(expression, messagearglist) ((void)0)
#define VX_ASSERT(expression, messagearglist) ((void)0)
#endif

} // namespace Vx

#endif // VX_MESSAGE_H

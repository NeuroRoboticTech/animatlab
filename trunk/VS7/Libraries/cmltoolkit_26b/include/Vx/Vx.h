#ifndef VX_H
#define  VX_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/12/03 01:21:28 $
   
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

// Vx.h: 
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxPrecision.h"

namespace Vx
{

#define VX_INFINITY MEINFINITY
#ifdef WIN32
#ifdef VX_DLL
#  ifdef VX_EXPORT
	#define VX_DLLSPEC __declspec(dllexport)
#  else
	#define VX_DLLSPEC __declspec(dllimport)
#  endif
#else
#define VX_DLLSPEC
#endif
#define VX_API MEAPI
#else
    #define VX_DLLSPEC
    #define VX_API
#endif

typedef void * VxNode;

} // namespace Vx

#ifndef DONT_USE_VX_NAMESPACE
using namespace Vx;
#endif 

#include "Vx/VxBase.h"
#endif

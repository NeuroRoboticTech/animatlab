#ifndef _MEVERSION_H
#define _MEVERSION_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/11/16 12:11:23 $
   
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
 *  Toolkit central version information
 */

/*
   Product and Version information, in "pieces"
 */


#if defined(VESUVIUS)

#define ME_PRODUCT_NAME                "CMLabs Simulation Inc. Vesuvius"
#define ME_PRODUCT_SHORTNAME           "Vesuvius Library"
#define ME_PRODUCTVERSION_MAJOR        2
#define ME_PRODUCTVERSION_MINOR        0
#undef  ME_PRODUCTVERSION_REV
#define ME_PRODUCTVERSION_QUALIFIER
#ifdef BUILD_VER_STRING
#define ME_PRODUCTVERSION_BUILDNUMBER  BUILD_VER_STRING
#else
#define ME_PRODUCTVERSION_BUILDNUMBER 674
#endif

#else

#define ME_PRODUCT_NAME                "CMLabs Simulations Inc. Vortex Toolkit"
#define ME_PRODUCT_SHORTNAME           "Vortex Toolkit"
#define ME_PRODUCTVERSION_MAJOR        2
#define ME_PRODUCTVERSION_MINOR        5
#define ME_PRODUCTVERSION_REV          0
#define ME_PRODUCTVERSION_QUALIFIER    
#ifdef BUILD_VER_STRING
#define ME_PRODUCTVERSION_BUILDNUMBER  BUILD_VER_STRING
#else
#define ME_PRODUCTVERSION_BUILDNUMBER 674
#endif

#endif

 /*
   Corporate information
 */
#define ME_COMPANY_NAME			"CMLabs Simulations Inc."
#define ME_WEB_SITE			"http://www.cm-labs.com"


/*
   Standardized copyright text for display in samples, examples, and tutorials
 */

#define ME_COPYRIGHT_LINE1   "(c) 2000-2004 CMLabs Simulations Inc. All rights reserved."
#define ME_COPYRIGHT_LINE2   ""
#define ME_COPYRIGHT_LINE3   ""

/*
   Rules to construct the version string(s) from the version information
 */

#define STRINGIZE(value)                          #value
#define CONSTRUCT_VER2(maj,min)                   STRINGIZE(maj.min)
#define CONSTRUCT_VER2QUALIFIED(maj,min,qual)     STRINGIZE(maj.min qual)

#define CONSTRUCT_VER3(maj,min,rev)               STRINGIZE(maj.min.rev)
#define CONSTRUCT_VER3QUALIFIED(maj,min,rev,qual) STRINGIZE(maj.min.rev qual)

#if !defined(ME_PRODUCTVERSION_REV)

#define ME_VERSION_STRING     \
  CONSTRUCT_VER2QUALIFIED(ME_PRODUCTVERSION_MAJOR,ME_PRODUCTVERSION_MINOR,ME_PRODUCTVERSION_QUALIFIER)

#define ME_BUILD_VERSION_STRING     \
  CONSTRUCT_VER2QUALIFIED(ME_PRODUCTVERSION_MAJOR,ME_PRODUCTVERSION_MINOR,ME_PRODUCTVERSION_QUALIFIER(build ME_PRODUCTVERSION_BUILDNUMBER))

#else

#define ME_VERSION_STRING     \
  CONSTRUCT_VER3QUALIFIED(ME_PRODUCTVERSION_MAJOR,ME_PRODUCTVERSION_MINOR,ME_PRODUCTVERSION_REV,ME_PRODUCTVERSION_QUALIFIER)

#define ME_BUILD_VERSION_STRING     \
  CONSTRUCT_VER3QUALIFIED(ME_PRODUCTVERSION_MAJOR,ME_PRODUCTVERSION_MINOR,ME_PRODUCTVERSION_REV,ME_PRODUCTVERSION_QUALIFIER(build ME_PRODUCTVERSION_BUILDNUMBER))

#endif

#ifdef __cplusplus
extern "C" {
#endif

const char *MeToolkitVersionString(void);
#ifdef __cplusplus
}
#endif

#endif /* _MEVERSION_H */


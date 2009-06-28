#ifndef _MEAPPTYPES_H
#define _MEAPPTYPES_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/07/12 21:48:27 $
   
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
 * MeApp demo support structures.
 */


#include "MstTypes.h"
#include "MeViewerTypes.h"

typedef struct MeApp                MeApp;
typedef struct MeAppContactDrawInfo MeAppContactDrawInfo;
typedef struct MeAppMousePickInfo   MeAppMousePickInfo;

struct MeAppMousePickInfo
{
  RGraphic* line;
  MdtBodyID dragBody;

  /* point that was grabbed in the body reference frame. */
  MeVector3 grabPosition;

  /* desired location for grabPosition in world reference frame. */
  MeVector3 desiredPosition;
  MeReal range;
  MeReal mouseSpringStiffness;
  MeReal mouseSpringDamping;
};

/** MeApp structure. Works with one MstUniverse, and one render context. */
struct MeApp
{
  MdtWorldID world;
  McdSpaceID space;
  MstUniverseID universe;
  RRender *rc;
  MeAppContactDrawInfo *contactDrawInfo;
  MeBool drawContacts;
  MeAppMousePickInfo mouseInfo;
  McdLineSegIntersectEnableCallback pickModelCB;
  void *userData;
};

enum MeAppDrawContactEnum { kMeAppDrawContactNormal=0, 
                            kMeAppDrawContactForce, 
                            kMeAppDrawContactTorque, 
                            kMeAppDrawContactPenetration,
                            kMeAppDrawContactDirection,
                            kMeAppDrawContactEnumMax };

struct MeAppContactDrawInfo
{
  RGraphic **contactG;
  float color[4];
  int maxContacts;
  int contactsDrawn;
  MeReal length;
  enum MeAppDrawContactEnum contactType;
};



#endif

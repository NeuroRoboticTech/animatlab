#if !defined(VX_FRAME_H)
#define VX_FRAME_H

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

// VxFrame.h: interface for the VxFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/Vx.h"
#include "MePool.h"

namespace Vx
{

class VxUniverse;
class VxGeometry;
class VxConstraint;
class VxEntity;
class VxPart;
class VxResponseResult;
class VxCollisionGeometry;
class VxIntersectResult;
class VxSceneGraphInterface;

// tolua_begin
typedef void * VxNode;
struct VxID2UNIVERSE;


/** 
 * Vx framework, a singleton, managing all Vortex objects.
 * A frame is created using VxFrame::instance(), after which
 * a particular scenegraph interface may be registered,
 * and VxUniverse objects added to the frame, which will contain
 * individual parts to be simulated.
 * The simulation is advanced with the VxFrame::step() function.
 * 
 */
class VX_DLLSPEC VxFrame: public VxBase
{

public:
  static VxFrame * instance();
  // tolua_end
  static VxFrame *& currentInstance();
  virtual const char* getClassName() const { return "VxFrame"; }

  static void release() { if (s_VxFrame) {delete s_VxFrame; s_VxFrame = 0; } }
  // tolua_begin

  void step();

  // detailed stepping control:
  void build();
  // Called every frame before step
  void preStepUpdate();
  // Called every frame
  void stepCollisionAndDynamics();
  // Called every frame after the Universe step.
  void postStepUpdate();


  // Handling multiple Universes.
  void addUniverse(VxUniverse *pVxUniverse);
  bool removeUniverse(VxUniverse *pVxUniverse);
  VxUniverse *getUniverse(int universeID);
  int getUniverseCount() const;

  // Maping Nodes to Entitys.
  VxEntity *getEntityFromNode(const VxNode node);
  bool setEntityToNode(const VxNode node, VxEntity * entity);
  void removeNodeFromEntity(const VxNode node);

  // tolua_end

  VxSceneGraphInterface* getSGInterface();
  VxSceneGraphInterface* getCurrentSGInterface() { return sgbridge; }
  void setSGInterface(VxSceneGraphInterface* sgint) {sgbridge = sgint;}

  MePool* getBodyPool() { return &mBodyPool; }
  MePool* getConstraintPool() { return &mConstraintPool; }

  bool IsBuilt() const { return !mNeedsToBeBuilt; }

  void setTraversalOptimizeLevel(int level);

  void setWarningLevel(int level);

  int getNewInternalID(int count=1);

  void intersectPair(const VxCollisionGeometry* g1, const VxCollisionGeometry* g2, VxIntersectResult* outResult);

private:

protected:
  VxFrame();
  virtual ~VxFrame();

  static VxFrame *s_VxFrame;

  VxID2UNIVERSE *universeArr;

  VxSceneGraphInterface *sgbridge;

  MePool mBodyPool;
  MePool mConstraintPool;

  bool mNeedsToBeBuilt;

  int mInternalIDStart;

}; // tolua_export


void VxFrameRegisterAllInteractions(VxFrame* frame);

} // namespace Vx

#if defined(LICENSE_CHECK) && defined(VX_DLL)
#include "Vx/vxLicDll.h"
#include "MeCall.h"

namespace Vx
{

void VX_DLLSPEC _cdecl
    VxLicenseRegisterDllFunction(const char* dllName, 
        VxLicDllFunctionType fn, void* userData);
} // namespace Vx

#endif


#endif

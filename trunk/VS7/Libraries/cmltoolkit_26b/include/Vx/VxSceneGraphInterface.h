#if !defined(VX_SCENEGRAPHINTERFACE_H)
#define VX_SCENEGRAPHINTERFACE_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/22 15:58:25 $
   
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

// VxSceneGraphInterface.h: interface to a scenegraph
//
//////////////////////////////////////////////////////////////////////

typedef void* VxSGTransformID;
#include "MePrecision.h"
#include "Vx/Vx.h"

namespace Vx
{

#ifdef __MAKECINT__
#undef MEAPI
#define MEAPI
#endif

class VX_DLLSPEC VxSceneGraphInterface
{
public:
    VxSceneGraphInterface() {}
    virtual ~VxSceneGraphInterface() {}

    virtual int setNodeLocalTransform(VxNode node,VxReal44Ptr inLocalTM) = 0;
    virtual int copyTransformFromSGTM(VxSGTransformID inSgtm, VxReal44Ptr outTm, VxReal3Ptr outScale) = 0;
    virtual int copyTransformToSGTM(VxSGTransformID inSgtm, VxReal44Ptr outTm) = 0;

    virtual int computeNodeAbsoluteTransform(VxNode node, VxReal44Ptr outTm, VxReal3Ptr outScale, VxReal44Ptr tmParentOut) = 0;
    virtual int getRootNode(VxNode node, VxNode *rootOut) = 0; // return root node to start update from for this node

    virtual int addNodeToSceneGraph(VxNode node, void* entity) = 0;
    virtual int removeNodeFromSceneGraph(VxNode node) = 0;

    virtual int buildTraversal() = 0;
    virtual int preStepTraversal() = 0;

    virtual int getEntityFromNode(VxNode node, void** entity) = 0;

    virtual int optimizeLevel(int level) = 0;
};

} // namespace Vx

#endif // VX_SCENEGRAPHINTERFACE_H

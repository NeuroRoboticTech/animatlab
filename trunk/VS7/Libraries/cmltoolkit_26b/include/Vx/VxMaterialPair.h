#if !defined(VX_MATERIALPAIR_H)
#define VX_MATERIALPAIR_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/12/03 01:21:29 $
   
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


// VxMaterialPair.h: interface for the VxMaterialPair class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/Vx.h"
#include "Vx/VxContactProperties.h"

struct MdtBclContactParams;

namespace Vx
{

class VxUniverse;
class VxFrame;


/**
 * Material pair physical parameters. 
 * For any pair of materials this class provides the
 * functionality for altering their properties. There's only one
 * material table per universe.
 * When a collision happens, contacts are generated between objects
 * and physical properties are set from this table indexed by the material
 * IDs that have been set on the colliding parts.
 * The table is always symmetric and only one entry for a given
 * pair of IDs needs to be set.
 * @see VxDynamicsResponse
 *
 */
class VX_DLLSPEC VxMaterialPair: public VxContactProperties, public VxBase
{
    friend class VxUniverse;
    friend class VxFrame;
    
    
public: 
    /*
    VxMaterialPair & operator = (VxMaterialPair &pair)
    {
    mContactParams = pair.mContactParams;
    mVxUniverse = pair.mVxUniverse;
    return *this;
    }
    */
    virtual ~VxMaterialPair();
    VxMaterialPair();
    
    virtual const char* getClassName() const { return "VxMaterialPair"; }
    
protected:
    
    VxUniverse *mVxUniverse;
    
private:
    
};

} // namespace Vx

#endif

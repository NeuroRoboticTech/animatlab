#ifndef VX_COLLISIONGEOMETRY_H
#define VX_COLLISIONGEOMETRY_H

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

// VxCollisionGeometry.h: interface for the VxFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/Vx.h"
#include "Vx/VxGeometry.h"

struct McdModel;

namespace Vx
{

class VxGeometry;

// tolua_begin

/**
 *  The VxCollisionGeometry class 
 *  instantiates a VxGeometry in space and also holds a properties
 *  associated with physical response such as materials and relative
 *  density.
 *  It provides utility functions that
 *  enable the user to:
 *  - Assign material properties to the object
 *  - Compute the mass properties of the object, assuming uniform
 *    density
 *  - Associate the geometry with a part
 *  - Modify the position and orientation of the geometry relative to
 *    the part
 *
*/
class VX_DLLSPEC VxCollisionGeometry: public VxBase
{
// tolua_end
  friend class VxEntity;
  friend class VxSensor;

// tolua_begin
public:
    VxCollisionGeometry(VxGeometry* inGeometry);
    virtual ~VxCollisionGeometry();

    virtual const char* getClassName() const { return "VxCollisionGeometry"; }

    VxCollisionGeometry *clone() const;
    void build();

    void replaceGeometry(VxGeometry* inGeometry);

    /** Returns the geometry */
    VxGeometry* getGeometry() { return mGeometry; }

    bool setMaterialID(int id );
    int getMaterialID() const;

    bool setResponseID(int id );
    int getResponseID() const;

    //bool setRequestID(int id);
    //int getRequestID() const;

    bool setRelativeDensity(VxReal density);
    VxReal getRelativeDensity() const;

    bool computeMassPropertiesFromDensity(VxReal *outMass, VxReal33Ptr outInertiaTensor, 
                                        VxReal3Ptr outCOM, VxReal *outVolume = 0) const;
    bool computetMassPropertiesFromMass(const VxReal inMass, VxReal3Ptr outCOM, 
                                        VxReal33Ptr outInertiaTensor, VxReal *outVolume = 0) const;

    void setRelativeTransform(VxReal44Ptr rTM);
    VxReal44Ptr getRelativeTransformPtr(int collisionID = 0) const;
    void getRelativeTransform(VxReal44Ptr rTM) const;
    void setRelativePosition(VxReal3 v);
    void setRelativePosition(VxReal x, VxReal y, VxReal z);
    void setRelativeOrientation(VxReal3 v);
    void setRelativeOrientation(VxReal x, VxReal y, VxReal z);
    void getTransform(VxReal44Ptr tm) const;
    const VxReal44Ptr getTransformPtr() const;
    VxReal44Ptr getTransformPtr();

    /** Returns the entity containing the collision geometry */
    inline VxEntity *getEntity() {return mEntity;}
    /** Returns the entity containing the collision geometry */
    inline const VxEntity *getEntity() const {return mEntity;}

    void getBoundingBox(VxReal3Ptr minCorner, VxReal3Ptr maxCorner) const;
    void getBoundingSphere(VxReal3Ptr center, VxReal *radius) const;

    void disablePairIntersect(VxCollisionGeometry* other);
    void enablePairIntersect(VxCollisionGeometry* other);

    // tolua_end
    inline McdModel *&getMcdModel() {return mMcdModelID;}
    inline McdModel *getMcdModel() const {return mMcdModelID;};
    inline McdGeometry *&getMcdGeometry()	{return mGeometry->getMcdGeometry();}	
    inline McdGeometry *getMcdGeometry() const {return mGeometry->getMcdGeometry();}
    
    /** Sets the graphics data pointer */
    void setGraphicsData(void *data) { mGraphicsData = data; }
    /** Returns the graphics data pointer */
    void* getGraphicsData() { return mGraphicsData; }

protected:
    void _setEntity(VxEntity *pEntity);

    VxEntity *mEntity;
    McdModel *mMcdModelID;

private:
    VxGeometry* mGeometry;
    void* mGraphicsData;

    // Copy constructor.
    VxCollisionGeometry(const VxCollisionGeometry &){ mGraphicsData  = 0; }
}; // tolua_export

} // namespace Vx

#endif // VX_COLLISIONGEOMETRY_H


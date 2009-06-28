#if !defined(VX_UNIVERSE_H)
#define VX_UNIVERSE_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/04/28 21:40:02 $

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

// VxUniverse.h: interface for the VxUniverse class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/Vx.h"

struct MstUniverse;

namespace Vx
{

class VxEntity;
class VxPart;
class VxConstraint;
class VxIntersectResult;
class VxResponseResult;
class VxDynamicsResponse;
struct VxDESC2MATERIALID;
struct VxID2PART;
struct VxID2CONSTRAINT;
class VxMaterialPair;
class VxCollisionGeometry;

// tolua_begin
/**
 * A universe contains entities and constraints that will be simulated together.
 * Some dynamic properties such as gravity can also be set.
 * Currently, the maximum number of collision geometries and the maximum number
 * of pairs of geometries that can be colliding at any given time must be set
 * as well (setCollisionGeometryMaxCount, setPairMaxCount).
 *
 * After these numbers are set, the user must call configure() before adding any objects into it.
 *
 * The physical simulation is advanced by call step()
 * During the step, collision information can be obtained by deriving a class from
 * the VxIntersectSubscriber and calling setIntersectCB() on this derived class. 
 * VxUniverse will then call the class notify() function with the contact information.
 *
 * To obtain a list dynamics contacts after the step, use getDynamicContactsResponseFirst().
 *
 */
class VX_DLLSPEC VxUniverse: public VxBase
{
  friend class VxMaterialPair;
public:

    // response type
    typedef int ResponseType;
    static const ResponseType kResponsePart;
    static const ResponseType kResponseSensor;
    static const ResponseType kResponseParticle;
    static const ResponseType kResponseUser;
  
  /** 
  * @enum eIntersectEventType
  * Enumerates the sorts of collision events that can be responed to 
  * in VORTEX.
  */
  enum eIntersectEventType
  {
      kEventActive = 0,     /*!< ongoing collision between parts that are unfrozen. */
      kEventInactive,       /*!< ongoing collision between parts that are frozen. Warning: performance penalty if used. */
      kEventFirst,          /*!< first time the parts are intersecting. */
      kEventDisjoint,       /*!< the parts stop touching each other. */
      kEventCount           /*!<   */
  };

  /** 
  * @enum eStepperModel
  * Enumerates the type of stepper available in the dynamics solver.
  */
  enum eStepperModel
  {
      /** This stepper first resolves the collisions separately 
        and then steps the system forward. Full rotational physics is enabled. 
        This is the default.
      */
      kStableStepper = 0,

      /** This simple model is the original "kea" integrator from VORTEX 1.0.
        It makes no distinction between steps where we are colliding and 
        otherwise. Additionally, all inertia tensors will be treated as if 
        they were spherical. This is very robust and efficient for most 
        simulations.
      */
      kKeaStepper,
      kSimpleStepper     // simple stepper
  };

  /** 
  * @enum eLinearSolver
  * Enumerates the type of linear solver available in the dynamics solver.
  */
  enum eLinearSolver
  {
    /**
    * Selection of the linear algebra solver module to be used by the stepper
    * and the lcp solver. The default is the kVBCholeskySolver.
    */
    kVBCholeskySolver,          /** Takes better advantage of sparsity, especially in the cases of long chains. */
    kBVBRRTCholeskySolver,      /** Optimized for intel procesor in single and double presision. */
  };


public:

    /** Virtual class for notification services on collision events. 
        @see VxUniverse::eIntersectEventType, VxUniverse::setIntersectCB and 
        VxUniverse::setMaterialPairIntersectCB. 
    */
    class VxIntersectSubscriber
    {
    public:
        VxIntersectSubscriber() {}
        virtual ~VxIntersectSubscriber() {}

        /** notify for VxUniverse::kEventInactive and VxUniverse::kEventDisjoint collision events. */
        virtual void notify(eIntersectEventType type, VxIntersectResult* res) {}

        /** notify for VxUniverse::kEventActive and VxUniverse::kEventFirst collision events. */
        virtual void notify(eIntersectEventType type, VxIntersectResult* ires, VxResponseResult* dres) {}
    };

    /** @internal */
    class VxFreezeSubscriber
    {
    public:
        VxFreezeSubscriber() {}
        virtual ~VxFreezeSubscriber() {}

        virtual void notify(VxPart* part, bool frozen) {}
    };

public:

    VxUniverse();
    VxUniverse(int collisionGeometryMaxCount, 
        int collisionGeometryPairMaxCount, int materialMaxCount);
    virtual ~VxUniverse();

    virtual const char* getClassName() const { return "VxUniverse"; }

    void configure();
    void step();

    void setAutoDisable(bool b);
    bool getAutoDisable() const;

    void resetDynamics();

    void setCollisionGeometryMaxCount(int lCollisions);
    int  getCollisionGeometryMaxCount() const;
    void setCollisionGeometryPairMaxCount(int lPairs);
    int  getCollisionGeometryPairMaxCount() const;
    int  getCollisionGeometryCount() const;
    void setContactMaxCount(int contacts, int responseid1=0, int responseid2=0);
    int  getContactMaxCount(int responseid1 = 0, int responseid2 = 0);
    void setMaterialsMaxCount(int lMarerial);
    int  getMaterialsMaxCount() const;

    bool registerMaterial(char *matDescription);
    int  getMaterialID(const char *matDescription) const;
    bool setMaterialDescription(char *oldMatDescription, char *newMatDescription);
    const char * getMaterialDescription(int matID);
    bool isValidMaterialID(int id);
    void validMaterialID(VxCollisionGeometry* geometry);
    VxMaterialPair *getMaterialPair(int materialID1, int materialID2);

    void setGravity(VxReal3 v);
    void setGravity(VxReal x, VxReal y, VxReal z);
    void getGravity(VxReal3 v) const;
    void setTimeStep(VxReal lStep);
    VxReal getTimeStep() const;

    void setComplianceParameters (MeReal linearCompliance, MeReal angularCompliance, MeReal linearDamping, MeReal angularDamping, MeReal linearLoss, MeReal angularLoss);

    void setLinearCompliance(VxReal );
    VxReal getLinearCompliance() const;
    void setLinearDamping(VxReal );
    VxReal getLinearDamping() const;
    void setLinearKineticLoss(VxReal );
    VxReal getLinearKineticLoss() const;
    void setAngularCompliance(VxReal );
    VxReal getAngularCompliance() const;
    void setAngularDamping(VxReal );
    VxReal getAngularDamping() const;
    void setAngularKineticLoss(VxReal );
    VxReal getAngularKineticLoss() const;
    void setLcpParam(int maxIteration, int maxScaleBoxIteration, VxReal scaleBoxTolerance);
    void getLcpParam(int *maxIteration, int *maxScaleBoxIteration, VxReal *scaleBoxTolerance);

    void setStepperModel(eStepperModel model);
    eStepperModel getStepperModel() const;
    void setLinearSolver(eLinearSolver solver);
    eLinearSolver getLinearSolver() const;
    void setMaxScaleBoxIterations(int iterCount);
    int getMaxScaleBoxIterations() const;

    // Entities methods.
    bool addEntity(VxEntity *entity);
    bool removeEntity(VxEntity *entity);
    int getEntityCount() const;
    VxEntity *getEntity(int entityID);
    VxEntity *getEntity(int entityID) const;

    // Constraint methods.
    bool addConstraint(VxConstraint *constraint);
    void removeConstraint(VxConstraint *constraint);
    int getConstraintCount() const;
    VxConstraint *getConstraint(int constraintID);
    VxConstraint *getConstraint(int constraintID) const;

    void setMaterialPairIntersectCB(int materialID1, int materialID2, VxIntersectSubscriber* userData);
    void setIntersectCB(ResponseType responseType1, ResponseType responseType2, eIntersectEventType eventID, VxIntersectSubscriber* userData);
 
    bool getDynamicContactsResponseFirst(VxDynamicsResponse *);

    // tolua_end

    // For low level access.
    MstUniverse *getMstUniverse(){return mUniverse;};
    // For debug use.
    void __dump(char *fileName);
    
    void verifyUniverseContent();
    void verifyDynamics(const char* caller, int level);
    void displayContent() const;

    void updateCollisionGeometryTransforms();

    int disablePairIntersect(VxCollisionGeometry* geom1, VxCollisionGeometry* geom2);
    void disableCurrentPairsInvolving(VxCollisionGeometry* geom);

    VxReal getCollisionTime() const;
    VxReal getDynamicsTime() const;

protected:
    friend class VxFrame;
    void preStepUpdate();
    void stepCollisionAndDynamics();
    void postStepUpdate();
    
    VxUniverse(const VxUniverse &);

    void removeConstraint(int constraintID);

    VxID2PART *mPartsArr;
    VxID2CONSTRAINT *mConstraintsArr;
    VxDESC2MATERIALID *mMaterialArr;
    // Only one Universe, for now.
    MstUniverse *mUniverse;
    VxMaterialPair* mMaterialPairList;
    VxReal mTimeStep;
    bool mConfigured;

    VxIntersectSubscriber* mVxIntersectSubscriber;

private:

  void init();

}; // tolua_export

} // namespace Vx

#endif // VX_UNIVERSE_H

#if !defined(VX_CONSTRAINT_H)
#define VX_CONSTRAINT_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/05/05 17:30:56 $

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

// VxConstraint.h: interface for the VxConstraint class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/Vx.h"

struct MdtBaseConstraint;
struct MdtBclLimit;
struct MdtBclSingleLimit;

namespace Vx
{

class VxPart;
class VxUniverse;
class VxAssembly;

// tolua_begin
/** 
 * The base class for constraints.
 * After creating a specific constraint, and adding the constraint to the universe, 
 * one or two parts may be associated using the setParts member function.
 * The constraint geometry must then be defined by setting the attachment position
 * for each part (or using a particular constraint function to do this more easily)
 * and the constraint axis or axes.
 * After the constraint geometry is specified,
 * other properties, such as the position, limits, etc. can then be set.
 * Constraints can also be motorized, locked or disabled.
 *
 * If only first part is set, the first part becomes constrained to
 * the world reference. This allows the programmer to attach parts to 
 * particular locations in space, with the constraints of the joint applying.
 */
class VX_DLLSPEC VxConstraint : public VxBase                                                             
{
// tolua_end   
    friend class VxUniverse;
    
public:
    virtual ~VxConstraint();
    //tolua_begin
    void setName(const char* name) { setNameBase(name, &sNameMap); }
    static VxConstraint* findFromName(const char* name) { return sNameMap ? (VxConstraint*)VxBase::findFromName(name, sNameMap) : 0; }

    void resetDynamics();
    
    virtual void setPartAttachmentPosition(int partID, VxReal3 pos);
    virtual void setPartAttachmentPosition(int partID, VxReal x, VxReal y, VxReal z);
    virtual void getPartAttachmentPosition(int partID, VxReal3Ptr pos);
    virtual void setPartAttachmentAxes(int partID, VxReal3 primary, VxReal3 secondary = 0);
    virtual void getPartAttachmentAxes(int partID, VxReal3Ptr primary, VxReal3Ptr secondary);
    void updateAttachmentFromPart(int partID);

    /** Returns the maximum number of part that can be set to the constraint. */
    int getMaxPartCount() const { return mMaxPartCount; }
    int getPartCount() const;
    void setPart(VxPart* part, int index);
    int addPart(VxPart* part);

    /** Set first and second part (index 0 and 1) this constraint is attached to. */
    void setParts(VxPart *part1, VxPart *part2) { setPart(part1, 0); setPart(part2, 1); }
    /** Set first part (index 0) this constraint is attached to. */
    void setPart1(VxPart *part) { setPart(part, 0); }
    /** Set second part (index 1) this constraint is attached to. */
    void setPart2(VxPart *part) { setPart(part, 1); }

    void getPartAttachmentPositionRel(int partID, VxReal3Ptr pos);
    void setPartAttachmentPositionRel(int partID, VxReal3 pos);
    void setPartAttachmentPositionRel(int partID, VxReal x, VxReal y, VxReal z);
    void getPartAttachmentAxesRel(int partID, VxReal3Ptr primary, VxReal3Ptr secondary);

    /** Returns the part referenced by @a partIndex that is constrained by this constraint. */
    VxPart * getPart(const int i)
    {
        return mPartList[i];
    }

    void lockAll(bool lock);
    bool isLockedAll() const;
    
    // Limits params.
    int getCoordinateCount() const;
    
    void setLimit(VxReal limit, int index, int coordID  = 0);
    VxReal getLimit(int index, int coordID  = 0) const;
    void setUpperLimit(VxReal limit, int coordID  = 0);
    void setLowerLimit(VxReal limit, int coordID  = 0);
    VxReal getUpperLimit(int coordID  = 0) const;
    VxReal getLowerLimit(int coordID  = 0) const;
    void setLimitVelocity(VxReal limitVelocity, int index, int coordID  = 0);
    VxReal getLimitVelocity(int indexVelocity, int coordID  = 0) const;
    void setCoordinateCurrentPosition(VxReal newPos, int coordID  = 0);
    VxReal getCoordinateCurrentPosition(int coordID  = 0) const;
    void setLimitRestitution(VxReal restitution, int index, int coordID  = 0);
    VxReal getLimitRestitution(int index, int coordID  = 0) const;
    void setLimitStiffness(VxReal stiffness, int index, int coordID  = 0);
    VxReal getLimitStiffness(int index, int coordID = 0) const;
    void setLimitDamping(VxReal damping, int index, int coordID = 0);
    VxReal getLimitDamping(int index, int coordID  = 0) const;
    // Drive the joint. The direction of rotation depends on the First part!
    void setMotor(VxReal spinRate, VxReal maxForce, int coordID = 0);
    VxReal getMotorDesiredVelocity(int coordID = 0) const;
    VxReal getMotorMaximumForce(int coordID = 0) const;
    VxReal getLockMaximumForce(int coordID = 0) const;
    void activateMotor(bool activate, int coordID = 0);
    void setLock(VxReal lockValue,VxReal forceLimit, int coordID = 0);
    void setLockPosition(VxReal lockValue, int coordID = 0);
    void setLockVelocity(VxReal velValue, int coordID = 0);
    VxReal getLockVelocity(int coordID = 0) const;
    void setLockStiffnessAndDamping(VxReal stiffness,VxReal damping, int coordID=0 );
    VxReal getLockPosition(int coordID = 0) const;
    void activateLock(bool activate, int coordID = 0);
    bool isOfType(int classType) const;
    virtual void enable(bool val);
    virtual bool isEnabled();
    
    void getPartForce(int body, VxReal3Ptr v);
    void getPartTorque(int body, VxReal3Ptr v);
    
    bool isLocked(int coordID  = 0) const;
    bool isMotorized(int coordID  = 0) const;
    bool isLimited(int coordID  = 0) const;
    void activateLimits(bool activate, int coordID  = 0);
    bool isLimitExeeded(int coordID) const;
    bool isAngular(int coordID = 0) const;

    VxReal getCoordinateVelocity(int coordID) const;
    VxReal getCoordinateForce(int coordID) const;
    VxReal getLambda(int index) const;
    
    void setForceCalculateCoordinatePosition(int coordID, bool b);
    
    VxUniverse* getUniverse() { return mUniverse; }
    
    // tolua_end
    
    /** returns the MdtConstraint */
    MdtBaseConstraint  *getMdtConstraint() { return mConstraint; }
    /** returns the MdtConstraint */
    MdtBaseConstraint  *getMdtConstraint() const  { return mConstraint; }
    
    bool isCoordIDValid(int coordID) const { return (coordID >=0 && coordID < getCoordinateCount()); }
    
    void reset();

    int getConstrainedCoordinateCount() const;
    void setRelaxConstraintStiffnessAndDamping(int coordinate, VxReal stiffness, VxReal damping);
    void setRelaxConstraintStiffnessDampingAndLoss(int coordinate, VxReal stiffness, VxReal damping, VxReal loss);
    void getRelaxConstraintStiffnessDampingAndLoss(int coordinate, VxReal *stiffness, VxReal *damping, VxReal *loss) const;
    void enableRelaxConstraint(int coordinate, bool enable);
    bool isRelaxConstraintEnabled(int coordinate) const;
    VxAssembly* getAssembly() { return mAssembly; }
    
protected:
    friend class VxAssembly;

    void enableBodies();

    VxConstraint(int maxPartCount=2);
    VxConstraint(const VxConstraint &){};
    virtual void _reset();
    bool _setUniverse(VxUniverse *universe);
    
    MdtBclLimit* _getLimit(int coordID);
    MdtBclSingleLimit* _getSingleLimit(int coordID, int index);
    MdtBclLimit* _getLimit(int coordID) const;
    MdtBclSingleLimit* _getSingleLimit(int coordID, int index) const;
    
    MdtBaseConstraint *mConstraint;
    
    VxUniverse *mUniverse;

    int mMaxPartCount;
    VxPart **mPartList;

    bool mEnabled;
    
    bool *mAttachedPosSet;
    bool *mAttachedAxisSet;

    VxAssembly *mAssembly;

private:
    static VxNameMap*  sNameMap;
}; // tolua_export


/** 
* Base class providing a framework for controlling a constraint.
*/
class VxConstraintController
{
public:
    VxConstraintController() : mConstraint(0), mValMax(0), mValInc(0), mMaxGradient(0.1f), mCoordID(0) {}
    virtual void init(VxConstraint* inConstraint, VxReal inInc, VxReal inMax, int inCoordID=0)
    {
        mConstraint = inConstraint;
        mValInc = inInc;
        mValMax = inMax;
        mCoordID = inCoordID;
        mCurrVal = 0;
    }

    virtual void updateEvent(VxReal scale, VxReal )
    {
        mCurrVal = scale*mValInc;
        if (mCurrVal > mValMax)
            mCurrVal = mValMax;
        else if (mCurrVal < -mValMax)
            mCurrVal = -mValMax;
    }
    virtual reset( VxReal inVal )
    {
        mCurrVal = inVal;
    }

protected:
    VxReal mValMax, mValInc, mCurrVal, mMaxGradient;
    VxConstraint* mConstraint;
    int mCoordID;
};

/** 
* Controls the constraint motor velocity. 
* At each update the controller will set velocity += scale*inInc 
* while maintaining |scale*inInc| < inMaxVelocity
*/
class VxMotorController : public VxConstraintController
{
public:
    /** Constructor. Use init() to setup parameters. */
    VxMotorController(bool autoLock = false) : mAutoLock(autoLock) {}

    virtual void init(VxConstraint* inConstraint, VxReal inInc, VxReal inMaxVelocity, int inCoordID=0)
    {
        VxConstraintController::init(inConstraint, inInc, inMaxVelocity, inCoordID);
    }

    void updateEvent(VxReal scale, VxReal force=VX_INFINITY)
    {
        mCurrVal += scale*mValInc;
        if (mCurrVal > mValMax)
            mCurrVal = mValMax;
        else if (mCurrVal < -mValMax)
            mCurrVal = -mValMax;

        if (mAutoLock && MeFabs(mCurrVal) <= mValInc*0.001f)
        {
            mConstraint->setLock(mConstraint->getCoordinateCurrentPosition(mCoordID), force, mCoordID);
            mConstraint->activateLock(true, mCoordID);
        }
        else
        {
            mConstraint->setMotor(mCurrVal, force, mCoordID);
            mConstraint->activateMotor(true, mCoordID);
        }
    }
    bool mAutoLock;
};

/** 
* Controls the constraint lock position. 
* At each update the controller will set the lockposition += scale*inInc 
* while maintaining |scale*inInc| < inMax
*/
class VxLockController : public VxConstraintController
{
public:
    /** Constructor. Use init() to setup parameters. */
    VxLockController() {}

    virtual void init(VxConstraint* inConstraint, VxReal inInc, VxReal inMax, int inCoordID=0)
    {
        VxConstraintController::init(inConstraint, inInc, inMax, inCoordID);
        mCurrVal = mConstraint->getLockPosition(inCoordID);
    }

    void updateEvent(VxReal scale, VxReal force=VX_INFINITY)
    {
        VxConstraintController::updateEvent(scale, force);
        mCurrVal += mConstraint->getLockPosition(mCoordID);

        if (mCurrVal > mValMax) mCurrVal = mValMax;
        if (mCurrVal < -mValMax) mCurrVal = -mValMax;
        mConstraint->setLock(mCurrVal, force, mCoordID);
        mConstraint->activateLock(true, mCoordID);
    }
};

/** 
* Controls the constraint lock velocity. 
* At each update the controller will set the lockvelocity += scale*inInc 
* while maintaining |scale*inInc| < inMax
*/
class VxLockVelocityController : public VxConstraintController
{
public:
    /** Constructor. Use init() to setup parameters. */
    VxLockVelocityController() {}

    virtual void init(VxConstraint* inConstraint, VxReal inInc, VxReal inMax, int inCoordID=0)
    {
        VxConstraintController::init(inConstraint, inInc, inMax, inCoordID);
        mCurrVal = mConstraint->getLockVelocity(inCoordID);
    }

    void updateEvent(VxReal scale, VxReal )
    {
        mCurrVal = mCurrVal + scale * mValInc;

        if (mCurrVal > mValMax) mCurrVal = mValMax;
        if (mCurrVal < -mValMax) mCurrVal = -mValMax;
        mConstraint->setLockVelocity(mCurrVal, mCoordID);
        mConstraint->activateLock(true, mCoordID);
    }
};

/**
* VxLimitMotorController controller controls the coordID's velocity of the limit inLimit. 
* At each update the controller will set the limit motor velocity = scale*inVelocity 
* while maintaining |velocity| < inMaxVelocity
*/
class VxLimitMotorController : public VxConstraintController
{
public:
    /** Constructor. Use init() to setup parameters. */
    VxLimitMotorController() {}

    virtual void init(VxConstraint* inConstraint, VxReal inVelocity, VxReal inMaxVelocity, int inLimit, int inCoordID=0)
    {
        VxConstraintController::init(inConstraint, inVelocity, inMaxVelocity, inCoordID);
        mLimit = inLimit;
        mCurrVal = mConstraint->getLimitVelocity(mLimit, inCoordID);
    }

    void updateEvent(VxReal scale, VxReal force=VX_INFINITY)
    {
        VxConstraintController::updateEvent(scale, force);
        mConstraint->setLimitVelocity(mCurrVal, mLimit, mCoordID);
    }
protected:
    int mLimit; // 0=lower, 1=upper
};

/**
 * VxDesiredPositionController controller for use with VxConstraint.
 * The coordID's motor powers the bodies towards the 'desiredPosition' at 'inMaxVelocity'
 * using 'maxforce' until it gets within +/- 'inGap'. In this region the velocity
 * is proportional to the error.
 */
class VxDesiredPositionController : public VxConstraintController
{
public:
    /** Constructor. Use init() to setup parameters. */
    VxDesiredPositionController() {}

    /** inits the controller for the desired gap and max velocity */
    virtual void init(VxConstraint* inConstraint, VxReal inGap, VxReal inMaxVelocity, int inCoordID=0)
    {
        VxConstraintController::init(inConstraint, inGap, inMaxVelocity, inCoordID);
        mCurrVal = mConstraint->getCoordinateCurrentPosition(mCoordID);
    }

    /** updates the controller for the desired position and maxforce */
    void updateEvent(VxReal desiredPosition, VxReal maxforce=VX_INFINITY)
    {
        VxReal error = mConstraint->getCoordinateCurrentPosition(mCoordID) - desiredPosition;
        mCurrVal = mValMax;

        if (!mConstraint->isMotorized(mCoordID))
            mConstraint->activateMotor(true, mCoordID);

        if (error >= -mValInc)
        {
            if (error > mValInc)
                mCurrVal = -mCurrVal;
            else /* we are in the proportional region */
                mCurrVal *= -error / mValInc;
        }
        mConstraint->setMotor(mCurrVal, maxforce, mCoordID);
    }
};

} // namespace Vx

#endif

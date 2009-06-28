#ifndef VX_CONTACTPROPERTIES_H
#define VX_CONTACTPROPERTIES_H

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

#include "Vx/Vx.h"

struct MdtBaseConstraint;
struct MdtBclContactParams;

namespace Vx
{

class VxPart;
class VxGeometry;


/**
 * Dynamics contact properties.
 * Allow user to specify contact physical properties such as friction, friction model,
 * restitution, etc... that are applied to contacts generated upon collision between
 * contacts.
 */
class VX_DLLSPEC VxContactProperties
{
public:
    // friction types
    typedef int FrictionType;
    static const FrictionType kFrictionTypeNone;            /*!< Frictionless contact. */
    static const FrictionType kFrictionTypeOneDirection;    /*!< Friction only along primary direction, which must be explicitely set by the user in a callback. @see VxDynamicsResponse::setContactDirection() */
    static const FrictionType kFrictionTypeTwoDirection;    /*!< Friction in both directions. */
    static const FrictionType kFrictionTypeUnknown;         /* Invalid contact type. */    

    // friction models
    typedef int FrictionModel;
    static const FrictionModel kFrictionModelBox;            /*!< Box friction model, the default if friction is turned on */ 
    static const FrictionModel kFrictionModelScaledbox;      /*!< Scaled box friction model, to simulate Coulomb friction. Experimental. */ 
    static const FrictionModel kFrictionModelSoil;           /*!< Soil friction model. */
    static const FrictionModel kFrictionModelUnknown;        /* Invalid friction model. */ 

    void setFrictionType(FrictionType type);
    FrictionType getFrictionType() const;

    void setFrictionModel(FrictionModel model);
    void setFrictionModelPrimary(FrictionModel model);
    void setFrictionModelSecondary(FrictionModel model);
    FrictionModel getFrictionModelPrimary() const;
    FrictionModel getFrictionModelSecondary() const;

    void setBoxFrictionForce(VxReal force);
    void setBoxFrictionForcePrimary(VxReal force);
    void setBoxFrictionForceSecondary(VxReal force);
    VxReal getBoxFrictionForcePrimary()  const;
    VxReal getBoxFrictionForceSecondary() const;

    void setFrictionCoefficient(VxReal coeff);
    void setFrictionCoefficientPrimary(VxReal coeff);
    void setFrictionCoefficientSecondary(VxReal coeff);
    VxReal getFrictionCoefficientPrimary() const;
    VxReal getFrictionCoefficientSecondary() const;

    void setSlip(VxReal coeff);
    void setSlipPrimary(VxReal coeff);
    void setSlipSecondary(VxReal coeff);
    VxReal getSlipPrimary() const;
    VxReal getSlipSecondary() const;
    
    void setSlide(VxReal coeff);
    void setSlidePrimary(VxReal coeff);
    void setSlideSecondary(VxReal coeff);
    VxReal getSlidePrimary() const;
    VxReal getSlideSecondary() const;

    void setSoftness(VxReal param);     
    void setDamping(VxReal param);    

    void setAdhesiveForce(VxReal param);      
    void setRestitution(VxReal param);       
    void setRestitutionThreshold(VxReal param);        
    VxReal getSoftness() const; 
    VxReal getDamping() const; 
    VxReal getAdhesiveForce() const;
    VxReal getRestitution() const;
    VxReal getRestitutionThreshold() const;


    void setCohesion(VxReal param);
    void setNormalExponnent(MeReal param); 
    void setNormalCoefficient(MeReal param); 
    void setNormalDamping(MeReal param); 
    void setShearModulus(MeReal param); 
    void setShearDamping(MeReal param); 
    void setInternalShearResistance(MeReal param); 
    void setArea(MeReal param); 
    void setLoad(MeReal param); 
    void setReferenceVelocity(MeReal param); 

    VxReal getCohesion() const;
    VxReal getNormalExponnent() const; 
    VxReal getNormalCoefficient() const;
    VxReal getNormalDamping() const; 
    VxReal getShearModulus() const; 
    VxReal getShearDamping() const; 
    VxReal getInternalShearResistance() const; 
    VxReal getArea() const; 
    VxReal getLoad() const; 
    VxReal getReferenceVelocity() const; 
  


//    void resetToDefault();

    MdtBclContactParams* getMdtBclContactParams() const;

protected:
    void setMdtContact(MdtBaseConstraint* dynamicContact);
    void setMdtBclContactParams(MdtBclContactParams* params);

    VxContactProperties(MdtBclContactParams* params);
    virtual ~VxContactProperties();

    
protected:
    MdtBclContactParams* mMdtBclContactParams;
};

} // namespace Vx

#endif // VX_CONTACTPROPERTIES_H

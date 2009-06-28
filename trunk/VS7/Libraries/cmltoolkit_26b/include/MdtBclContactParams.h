#ifndef _MDTBCLCONTACTPARAMS_H
#define _MDTBCLCONTACTPARAMS_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/10/06 09:42:30 $
   
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
 * Constraint Library Contact Parameters.
 *
 * Definitions of the MdtBclContactParam struct used by MdtBcl Contact.
 * This includes friction, restitution, softness, slip, surface velocity etc.
 */


/**
 * MdtBcl Contact friction type enum.
 */
typedef enum
{
    /**
     * Frictionless contact.
     */
    MdtContactTypeFrictionZero,

    /**
     * Friction only along primary direction.
     */
    MdtContactTypeFriction1D,

    /**
     * Friction in both directions.
     */
    MdtContactTypeFriction2D,

    /**
     * Invalid contact type.
     */
    MdtContactTypeUnknown = -1
}
MdtContactType;



/**
 * MdtBcl Contact options enum.
 *
 * Contact options such as friction (slipperiness) and restitution
 * (bounciness).  Any bitwise combination of these options is valid for use
 * in 'MdtBclContact.direction'.
 *
 * @see MdtBclContact
 * @see MdtBclContactParams
 */
typedef enum
{
    /**
     * Use primary direction vector.
     *
     * Use \a MdtBclContact.direction vector for primary direction
     * (otherwise use auto).
     */
    MdtBclContactOptionUseDirection = 0x00001,

    /**
     * Use restitution at the contact.
     *
     * The restitution factor is specified in \a
     * MdtBclContactParams.restitution.
     */
    MdtBclContactOptionBounce = 0x00002,

    /**
     * Use a simple soft contact model..
     *
     * The softness factor is specified in \a MdtBclContactParams.softness.
     * This factor corresponds to 1/(h.k) in an undamped spring model,
     * where h is the timestep and k is the spring stiffness factor.
     */
    MdtBclContactOptionSoft = 0x00004,

    /**
     * Use a simple adhesive contact model.
     *
     * The maximum adhesive force is specified in \a
     * MdtBclContactParams.max_adhesive_force.
     */
    MdtBclContactOptionAdhesive = 0x00008,

    /**
     * Use a first order slip in the primary direction.
     *
     * The slip velocity is specified in \a MdtBclContactParams.slip1.
     */
    MdtBclContactOptionSlip1 = 0x00010,

    /**
     * Use a first order slip in the secondary direction.
     *
     * The slip velocity is specified in \a MdtBclContactParams.slip2.
     */
    MdtBclContactOptionSlip2 = 0x00020,

    /**
     * Use a surface velocity in the primary direction (like a conveyor belt
     * motion).
     *
     * The slide velocity is specified in \a MdtBclContactParams.slide1.
     */
    MdtBclContactOptionSlide1 = 0x00040,

    /**
     * Use a surface velocity in the secondary direction.
     *
     * The secondary slide velocity is specified in \a
     * MdtBclContactParams.slide1.
     */
    MdtBclContactOptionSlide2 = 0x00080,

    /**
     * This to use a  
     */
    MdtBclContactOptionCoulomb = 0x00100
}
MdtBclContactOption;

/**
 * MdtBcl Friction model enum.
 *
 * Friction model to use along primary or secondary direction.
 */
typedef enum
{
    /**
     * Box-type friction model.  This model requires the user to set
     * explicit bounds on the tangential force to be applied in order to
     * prevent the object from sliding.  When using this friction model,
     use MdtContactParamsSetFriction() to set the maximum tangential force.
     */
    MdtFrictionModelBox = 0,

    /**
     Scaled Box-type friction model: perform one extra iteration for
     getting Coulomb friction approximation.  When using this model, use
     MdtContactParamsSetFrictionCoefficient().  This model will deliver a
     maximum tangential force which will scale approximately with the
     normal force.  Note that the proportionality is not guaranteed and you
     might in fact have to set friction coefficients that are larger than
     1.  The computational load from this model can become large.  
    */

    MdtFrictionModelScaledBox = 1,

    MdtFrictionModelSoil = 2,

    /**
     * Invalid friction model.
     */
    MdtFrictionModelUnknown = -1
}
MdtFrictionModel;

/**
 * MdtBcl Contact constraint parameters struct.
 *
 * If contact type is 1D friction - motion along secondary direction is
 * frictionless, and model2, slip2, slide2 and friction2 are ignored.
 *
 * @see MdtBclContactOption
 */
typedef struct MdtBclContactParams
{
    /**
     * Contact type (zero, 1D or 2D friction).
     */
    MdtContactType type;

    /** @var model1
     * Friction model to use along primary direction.
     */
    /** @var model2
     * Friction model to use along secondary direction.
     */
    MdtFrictionModel model1;
    MdtFrictionModel model2;

    /**
     * Bitwise combination of MdtBclContactOption's.
     *
     * @see MdtBclContactOption.
     */
    int options;

    /** @var restitution
     * Restitution parameter.
     */
    /** @var velThreshold
     * Minimum velocity for restitution.
     */
    /** @var softness
     * Contact softness parameter (soft mode).
     */
    /** @var max_adhesive_force
     * Contact maximum adhesive force parameter (adhesive mode).
     */
    MeReal restitution;
    MeReal velThreshold;
    MeReal softness;
    MeReal damping;
    MeReal max_adhesive_force;

    /** @var friction1
     * Max friction force in primary direction.
     */
    /** @var slip1
     * First order slip in primary direction.
     */
    /** @var friction2
     * Max friction force in secondary direction.
     */
    /** @var slip2
     * First order slip in primary direction.
     */
    MeReal friction1;
    MeReal slip1;
    MeReal friction2;
    MeReal slip2;

/**
 * 
 */
  int bSenseOfPrimaryFriction;
  int bSenseOfSecondaryFriction;


  MeReal friction_primaryUpper;
  MeReal friction_primaryLower;

  MeReal friction_secondaryUpper;
  MeReal friction_secondaryLower;




    /** @var slide1
     * Surface velocity in primary direction.
     */
    /** @var slide2
     * Surface velocity in secondary direction.
     */
    MeReal slide1;
    MeReal slide2;

  /** 
   * Friction coefficients in primary and secondary direction
   */
  MeReal friction_coefficient1;
  MeReal friction_coefficient2;



  MeBool bUseSoftFingerContact;
  MeReal soft_finger_coefficient;

    /**
     *  Soil parameters. 
     */
    MeReal K;                   /* shearing modulus */
    MeReal d_t;                 /* shearing damping constant */
    MeReal tan_phi;             /* tangent of angle of internal shearing resistance*/
    MeReal c;                   /* soil cohesion*/
    MeReal n;                   /* normal contact force law exponnent*/
    MeReal A;                   /* area of contact */
    MeReal W;                   /* approximation of contact load */
    MeReal v0;                  /* reference velocity to compute slip*/
    MeReal pad[10];

    void *userdata; 

}
MdtBclContactParams;

#endif

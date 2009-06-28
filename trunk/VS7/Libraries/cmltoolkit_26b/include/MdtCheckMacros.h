#ifndef _MDTCHECKMACROS_H
#define _MDTCHECKMACROS_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/16 21:54:57 $
   
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

#include "MeMessage.h"


#if defined (_MECHECK)

/*
 * Checks the validity of an MdtWorldID i.e., whether the pointer is non-null.
 */
#define MdtCHECKWORLD(x,f) \
    do { if ((x) == 0) MeFatalError(0,"Invalid MdtWorldID detected" \
        " in %s().\n",#f); } while(0)

/*
 * Checks the validity of an MdtBodyID: non null and version ID.
 */
#define MdtCHECKBODY(x,f) \
    do { if ((x) == 0 || (x)->keaBody.tag != MdtKeaBODYVER1) \
             MeFatalError(0,"Invalid MdtBodyID detected in %s().\n",#f); \
       } while(0)

/*
 * Checks the validity of an MdtBodyID but allows the body to be NULL.
 */
#define MdtCHECKBODY_NULL_OK(x,f) \
    do { if ((x) && (x)->keaBody.tag != MdtKeaBODYVER1) \
             MeFatalError(0,"Invalid MdtBodyID detected in " \
             "%s().\n",#f); } while(0)

/*
 * Checks the validity of an MdtBaseConstraintID : checks non-null
 */
#define MdtCHECKCONSTRAINT(x,f) \
    do { if ((x) == 0) MeFatalError(0,"Invalid MdtConstraintID " \
            "detected in %s().\n",#f); } while(0)


/*
 * Checks that a constraints bodies have been set up. The second body
 * is allowed to be null with this macro.  Check for null constraint.
 */
#define MdtCHECKCONSTRAINTBODIES_NULL_OK(x,f) \
    do { if (  (x)==0  || ((MdtBaseConstraint*)(x))->head.body[0] == 0) \
        MeFatalError(0,"Invalid constraint bodies detected" \
                " in %s().\n",#f); } while(0)

/*
 * Checks that a constraints bodies have been set up. Both bodies
 * must be non zero with this macro.
 */
#define MdtCHECKCONSTRAINTBODIES(x,f) \
    do { if (((MdtBaseConstraint*)(x))->head.body[0] == 0 || \
         ((MdtBaseConstraint*)(x))->head.body[1] == 0) \
        MeFatalError(0,"Invalid constraint bodies detected" \
                " in %s().\n",#f); } while(0)

/*
 * Checks the validity of an MdtHingeID.
 */
#define MdtCHECKHINGE(x,f) \
    do { if ((x) == 0 || (x)->bclHinge.head.tag != MdtBclHINGE) \
             MeFatalError(0,"Invalid MdtHingeID detected in %s().\n",#f); \
       } while(0)

/*
 * Checks the validity of an MdtBSJointID.
 */
#define MdtCHECKBSJOINT(x,f) \
    do { if ((x) == 0 || (x)->bclBSJoint.head.tag != MdtBclBSJOINT) \
             MeFatalError(0,"Invalid MdtBSJointID detected in %s().\n",#f); \
       } while(0)

/*
 * Checks the validity of an MdtContactID.
 */
#define MdtCHECKCONTACT(x,f) \
    do { if ((x) == 0 || (x)->bclContact.head.tag != MdtBclCONTACT) \
             MeFatalError(0,"Invalid MdtContactID detected in %s().\n",#f); \
       } while(0)

/*
 * Checks the validity of a contact params id.
 */
#define MdtCHECKCONTACTPARAMS(x,f) \
    do { if ((x) == 0) MeFatalError(0,"Invalid MdtContactParamsID " \
                        "detected in %s().\n",#f); } while(0)

/*
 * Checks the validity of an MdtPrismaticID.
 */
#define MdtCHECKPRISMATIC(x,f) \
    do { if ((x) == 0 || (x)->bclPrismatic.head.tag != MdtBclPRISMATIC) \
             MeFatalError(0,"Invalid MdtPrismaticID detected in %s().\n",#f); \
       } while(0)

/*
 * Checks the validity of an MdtPrismaticID.
 */
#define MdtCHECKCYLINDRICAL(x,f) \
    do { if ((x) == 0 || (x)->bclCylindrical.head.tag != MdtBclCYLINDRICAL) \
             MeFatalError(0,"Invalid MdtCylindricalID detected in %s().\n",#f); \
       } while(0)


/*
 * Checks the validity of an MdtDistanceID.
 */
#define MdtCHECKDISTANCE(x,f) \
    do { if ((x) == 0 || (x)->bclDistance.head.tag != MdtBclDISTANCE) \
             MeFatalError(0,"Invalid MdtDistanceID detected in %s().\n",#f); \
       } while(0)


/*
 * Checks the validity of an MdtCarWheelID.
 */
#define MdtCHECKCARWHEEL2(x,f) \
    do { if ((x) == 0 || (x)->bclCarWheel2.head.tag != MdtBclCARWHEEL2) \
             MeFatalError(0,"Invalid MdtCarWheel2ID detected in %s().\n",#f); \
       } while(0)

/*
 * Checks the validity of an MdtFixedPathID.
 */
#define MdtCHECKFIXEDPATH(x,f) \
    do { if ((x) == 0 || (x)->bclFixedPath.head.tag != MdtBclFIXEDPATH) \
             MeFatalError(0,"Invalid MdtFixedPathID detected in %s().\n",#f); \
       } while(0)


/*
 * Checks the validity of an MdtRPROJointID.
 */
#define MdtCHECKRPROJOINT(x,f) \
    do { if ((x) == 0 || (x)->bclRPROJoint.head.tag != MdtBclRPROJOINT) \
             MeFatalError(0,"Invalid MdtRPROJointID detected in function %s.\n",#f); \
       } while(0)



/*
 * Checks the validity of an MdtUniversalID.
 */
#define MdtCHECKUNIVERSAL(x,f) \
    do { if ((x) == 0 || (x)->bclUniversal.head.tag != MdtBclUNIVERSAL) \
             MeFatalError(0,"Invalid MdtUniversalID detected in %s().\n",#f); \
       } while(0)

/*
 * Checks the validity of an MdtLinear1ID.
 */
#define MdtCHECKLINEAR1(x,f) \
    do { if ((x) == 0 || (x)->bclLinear1.head.tag != MdtBclLINEAR1) \
             MeFatalError(0,"Invalid MdtLinear1ID detected in %s().\n",#f); \
       } while(0)

/*
 * Checks the validity of an MdtLinear2ID.
 */
#define MdtCHECKLINEAR2(x,f) \
    do { if ((x) == 0 || (x)->bclLinear2.head.tag != MdtBclLINEAR2) \
             MeFatalError(0,"Invalid MdtLinear2ID detected in %s().\n",#f); \
       } while(0)

/*
 * Checks the validity of an MdtLinear3ID.
 */
#define MdtCHECKLINEAR3(x,f) \
    do { if ((x) == 0 || (x)->bclLinear3.head.tag != MdtBclLINEAR3) \
             MeFatalError(0,"Invalid MdtLinear3ID detected in %s().\n",#f); \
       } while(0)

/*
 * Checks the validity of an MdtAngular3ID.
 */
#define MdtCHECKANGULAR3(x,f) \
    do { if ((x) == 0 || (x)->bclAngular3.head.tag != MdtBclANGULAR3) \
             MeFatalError(0,"Invalid MdtAngular3ID detected in function %s.\n",#f); \
       } while(0)

/*
 * Checks the validity of an MdtUserConstraintID.
 */
#define MdtCHECKUSERCONSTRAINT(x,f) \
    do { if ((x) == 0 || (x)->bclUser.head.tag != MdtBclUSER) \
             MeFatalError(0,"Invalid MdtUserConstraintID detected in function %s.\n",#f); \
       } while(0)


/*
 * Checks the validity of an MdtSpringID.
 */
#define MdtCHECKSPRING(x,f) \
    do { if ((x) == 0 || (x)->bclSpring.head.tag != MdtBclSPRING) \
             MeFatalError(0,"Invalid MdtSpringID detected in function %s.\n",#f); \
       } while(0)


/*
 * Checks the validity of an MdtConeLimitID.
 */
#define MdtCHECKCONELIMIT(x,f) \
    do { if ((x) == 0 || (x)->bclConeLimit.head.tag != MdtBclCONELIMIT) \
             MeFatalError(0,"Invalid MdtConeLimitID detected in function %s.\n",#f); \
       } while(0)

#define MdtCHECKGEARRATIO(x,f) \
    do { if ((x) == 0 || (x)->bclGearRatio.head.tag != MdtBclGEARRATIO) \
             MeFatalError(0,"Invalid MdtGearRatioID detected in function %s.\n",#f); \
       } while(0)

#define MdtCHECKSCREWJOINT(x,f) \
    do { if ((x) == 0 || (x)->bclScrewJoint.head.tag != MdtBclSCREWJOINT) \
             MeFatalError(0,"Invalid MdtScrewJointID detected in function %s.\n",#f); \
       } while(0)

#define MdtCHECKWINCHJOINT(x,f) \
    do { if ((x) == 0 || (x)->bclWinchJoint.head.tag != MdtBclWINCHJOINT) \
             MeFatalError(0,"Invalid MdtWinchJointID detected in function %s.\n",#f); \
       } while(0)

#define MdtCHECKDIFFERENTIALJOINT(x,f) \
    do { if ((x) == 0 || (x)->bclDifferential.head.tag != MdtBclDIFFERENTIAL) \
             MeFatalError(0,"Invalid MdtDifferentialJointID detected in function %s.\n",#f); \
       } while(0)

#define MdtCHECKCOUPLEDDISTANCE(x,f) \
    do { if ((x) == 0 || (x)->bclCoupledDistance.head.tag != MdtBclCOUPLEDDISTANCE) \
             MeFatalError(0,"Invalid MdtDifferentialJointID detected in function %s.\n",#f); \
       } while(0)
/*
 * Checks the validity of an MdtDoubleHinge.
 */
#define MdtCHECKDOUBLEHINGE(x,f) \
    do { if ((x) == 0 || (x)->bclDoubleHinge.head.tag != MdtBclDOUBLEHINGE) \
             MeFatalError(0,"Invalid MdtDoubleHinge detected in %s().\n",#f); \
       } while(0)



/*
 * Checks the validity of an MdtSingleLimitID.
 */
#define MdtCHECKSINGLELIMIT(x,f) \
    do { if ((x) == 0) MeFatalError(0,"Invalid MdtSingleLimitID " \
                "detected in %s().\n",#f); } while(0)

/*
 * Checks the validity of an MdtLimitID.
 */
#define MdtCHECKLIMIT(x,f) \
    do { if ((x) == 0) MeFatalError(0,"Invalid MdtLimitID detected" \
                " in %s().\n",#f); } while(0)

/*
 * Checks the validity of a body index (must be 0 or 1).
 */
#define MdtCHECKBODYINDEX(x,f) \
    do { if ((x) > MdtKeaMAXBODYCONSTRAINT) MeFatalError(0, "Invalid body index " \
                "detected in %s().\n",#f); } while(0)

/*
 * Checks the validity of a limit index (must be 0, 1 or 2).
 */
#define MdtCHECKLIMITINDEX3(x,f) \
    do { if ((x) > 2) MeFatalError(0, "Invalid limit index " \
                "detected in %s().\n",#f); } while(0)


/*
 * Checks the validity of a pool i.e., whether the pointer is non-null.
 */
#define MdtCHECKPOOL(x,f) \
    do { if ((x) == 0) MeFatalError(0,"Invalid pool detected" \
        " in %s().\n",#f); } while(0)


#else
#   define MdtCHECKWORLD(x,f)
#   define MdtCHECKBODY(x,f)
#   define MdtCHECKBODY_NULL_OK(x,f)
#   define MdtCHECKCONSTRAINT(x,f)
#   define MdtCHECKCONSTRAINTBODIES_NULL_OK(x,f)
#   define MdtCHECKCONSTRAINTBODIES(x,f)
#   define MdtCHECKHINGE(x,f)
#   define MdtCHECKBSJOINT(x,f)
#   define MdtCHECKCONTACT(x,f)
#   define MdtCHECKCONTACTPARAMS(x,f)
#   define MdtCHECKCYLINDRICAL(x,f)
#   define MdtCHECKDISTANCE(x,f)
#   define MdtCHECKPRISMATIC(x,f)
#   define MdtCHECKCARWHEEL2(x,f)
#   define MdtCHECKFIXEDPATH(x,f)
#   define MdtCHECKFPFOJOINT(x,f)
#   define MdtCHECKRPROJOINT(x,f)
#   define MdtCHECKUNIVERSAL(x,f)
#   define MdtCHECKLINEAR1(x,f)
#   define MdtCHECKLINEAR2(x,f)
#   define MdtCHECKLINEAR3(x,f)
#   define MdtCHECKANGULAR3(x,f)
#   define MdtCHECKUSERCONSTRAINT(x,f)
#   define MdtCHECKSPRING(x,f)
#   define MdtCHECKCONELIMIT(x,f)
#   define MdtCHECKGEARRATIO(x,f) 
#   define MdtCHECKSCREWJOINT(x,f) 
#   define MdtCHECKWINCHJOINT(x,f) 
#   define MdtCHECKDIFFERENTIAL(x,f) 
#   define MdtCHECKDIFFERENTIALJOINT(x,f)
#   define MdtCHECKCOUPLEDDISTANCE(x,f)
#   define MdtCHECKDOUBLEHINGE(x,f) 
#   define MdtCHECKSINGLELIMIT(x,f)
#   define MdtCHECKLIMIT(x,f)
#   define MdtCHECKBODYINDEX(x,f)
#   define MdtCHECKLIMITINDEX3(x,f)
#   define MdtCHECKPOOL(x,f)
#endif


#endif



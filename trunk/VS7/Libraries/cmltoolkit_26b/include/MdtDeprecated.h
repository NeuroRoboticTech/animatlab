#ifndef _MDTDEPRECATED_H
#define  _MDTDEPRECATED_H

/** @file
  Macros for deprecated functions from version 1.
*/


  /** deprecated constraint casting function */
#define MdtAngular3QuaConstraint(j)  (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastAngular3(j) ( ( (j)->bclH.tag == MdtBclANGULAR3 )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtBSJointQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastBSJoint(j) ( ( (j)->bclH.tag == MdtBclBSJOINT )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtCarWheelQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastCarWheel(j) ( ( (j)->bclH.tag == MdtBclCARWHEEL)? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtConeLimitQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastConeLimit(j) ( ( (j)->bclH.tag == MdtBclCONELIMIT  )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtContactQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastContact(j) ( ( (j)->bclH.tag == MdtBclCONTACT  )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtFixedPathQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastFixedPath(j) ( ( (j)->bclH.tag == MdtBclFIXEDPATH  )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtHingeQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastHinge(j) ( ( (j)->bclH.tag == MdtBclHINGE  )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtLinear1QuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastLinear1(j) ( ( (j)->bclH.tag == MdtBclLINEAR1 )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtLinear2QuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastLinear2(j) ( ( (j)->bclH.tag == MdtBclLINEAR2 )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtPrismaticQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastPrismatic(j) ( ( (j)->bclH.tag == MdtBclPRISMATIC )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtRPROJointQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastRPROJoint(j) ( ( (j)->bclH.tag == MdtBclRPROJOINT )? (j) : 0 )

  /** deprecated constraint casting function */
#define   MdtSpringQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define   MdtConstraintDCastSpring(j) ( ( (j)->bclH.tag == MdtBclSPRING )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtUniversalQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastUniversal(j) ( ( (j)->bclH.tag == MdtBclUNIVERSAL )? (j) : 0 )

  /** deprecated constraint casting function */
#define MdtUserConstraintQuaConstraint(j) (j)

  /** deprecated constraint casting function */
#define MdtConstraintDCastUserConstraint(j) ( ( (j)->bclH.tag == MdtBclUSER )? (j) : 0 )




#endif 

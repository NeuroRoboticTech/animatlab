#ifndef _MDTUWTKDRAWCONTACTS_H
#define _MDTUWTKDRAWCONTACTS_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/09/27 21:30:58 $
   
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


/*                                
   Description: Draw MdtWorld conatcts using WTK graphics.
*/

#include "wt.h"
#include "MdtWorld.h"
#include "MePrecision.h"

typedef struct {      
  WTnode  *root;        /* scene graph root              */
  WTnode  *ContactNode; /* group node holding all arrows */
  WTnode  **lines;      /* individual arrows             */   
  int     MaxContacts;
  int     ConatctCount;
} MWDrawContacts;

/*---------------------------------------------------------------------------*/
/* public functions */
unsigned int
MdtuWtkDrawContactsInit(MWDrawContacts *dc,  
          WTnode *root, unsigned char *clr, int max, MeReal length );

void
MdtuWtkDrawContacts(MWDrawContacts *dc, MdtWorldID world);

void 
MdtuWtkDrawContactsDisable(MWDrawContacts *dc);

void 
MdtuWtkDrawContactsDelete(MWDrawContacts *dc );
/*---------------------------------------------------------------------------*/
 

/* assuming ny is normalized */
void 
MdtuFindAxes(const MeVector3 ny, MeMatrix4Ptr bodytm, WTm4 m4)
{
  MeVector3 nx, nz;
  MeReal *v;
  MeReal fdot = 1;
  int i = 0;
  
  while ( fabs(fabs(fdot)-1)<ME_MEDIUM_EPSILON && i < 3) {
    v = &(bodytm[i][0]);
    fdot = MeVector3Dot(ny, v);
    i++;
  }

  MeVector3Cross(nx, ny, v);
  MeVector3Normalize(nx);
  MeVector3Cross(nz, nx, ny); 
  MeVector3Normalize(nz);  // not necessary ? 

  m4[0][0] = nx[0]; m4[0][1] = nx[1]; m4[0][2] = nx[2];
  m4[1][0] = ny[0]; m4[1][1] = ny[1]; m4[1][2] = ny[2];
  m4[2][0] = nz[0]; m4[2][1] = nz[1]; m4[2][2] = nz[2];
}

/* */
void 
MdtuMakeArrow( WTgeometry **ar, MeReal length )
{
  WTgeometry *cone;
  WTpq pq;

  MeReal h_cyl = length*5/7.0f;
  MeReal h_con = length*2/7.0f;
  MeReal r_cyl = length*0.05f;
  MeReal r_con = length*0.075f;
  *ar = WTgeometry_newcylinder(h_cyl, r_cyl, 4, 1, 1);  
  WTpq_init(&pq);  
  pq.p[1] -= h_cyl*0.5f;
  WTgeometry_transform(*ar, &pq);

  cone = WTgeometry_newcone(h_con, r_con, 4, 1);
  WTpq_init(&pq);  
  pq.p[1] -= (h_cyl+h_con*0.5f);
  WTgeometry_transform(cone, &pq);

  WTgeometry_merge(*ar, cone);
}

/* */
unsigned int
MdtuWtkDrawContactsInit(MWDrawContacts *dc, WTnode *root, 
          unsigned char *clr, int max, MeReal length )
{  
  WTgeometry *line, *line_i;   
  int i;
  
  WTm4 m4;  
  WTm4_init(m4);  
 
  dc->root = root; 
  dc->lines = (WTnode**)MeMemoryAPI.create(max*sizeof(WTnode*));
  
  if (!(dc->lines)) return 0;

  dc->MaxContacts = max;   
  dc->ConatctCount = 0;
  dc->ContactNode = WTgroupnode_new(dc->root);
   
  MdtuMakeArrow( &line, length );

  for (i=0; i<dc->MaxContacts; i++) {
    line_i = WTgeometry_copy(line);
    WTgeometry_setrgb(line_i, clr[0], clr[1], clr[2]);     
    dc->lines[i] = WTmovgeometrynode_new(dc->ContactNode,line_i);     
    WTnode_settransform(dc->lines[i], m4);
    WTnode_enable(dc->lines[i], 0);
  }

  return 1;
}

/* */
void 
MdtuWtkDrawContactsDelete(MWDrawContacts *dc)
{
  int j;
  for (j = 0; j < dc->MaxContacts; j++) {
    WTnode_remove(dc->lines[j]);
  }
  WTnode_remove(dc->ContactNode);
  MeMemoryAPI.destroy( dc->lines );
  dc->lines = 0;
}  

/* */
void 
MdtuWtkDrawContactsDisable(MWDrawContacts *dc) 
{
  int j;
  for (j = 0; j < dc->MaxContacts; j++) {
    WTnode_enable(dc->lines[j], 0);
  }
}
 
/* */
void
MdtuWtkDrawContacts(MWDrawContacts *dc, MdtWorldID world)
{    
  MeVector3 pos, normal;
  int i = 0, j;    
  MdtConstraintID c; 
  MdtBody *body;
  MeMatrix4Ptr mptr;
  WTm4 m4;
  WTm4_init(m4);

  c = MdtConstraintGetFirst(world);

  while (c)
  {
    if (c->bclH.tag == MdtBclCONTACT) {
      if (i < dc->MaxContacts) {
        MdtContactGetPosition(c, pos);
        MdtContactGetNormal((MdtContactID)c,normal);
        body = MdtContactGetBody((MdtContactID)c, 0);
        if (!body) body = MdtContactGetBody((MdtContactID)c, 1);
        mptr = MdtBodyGetTransformPtr(body);
        MdtuFindAxes( normal, mptr, m4 );        
        m4[3][0] = pos[0]; 
        m4[3][1] = pos[1]; 
        m4[3][2] = pos[2];
        WTnode_enable(dc->lines[i], 1);
        WTnode_settransform(dc->lines[i], m4);
        i++;
      }
    }
    c = MdtConstraintGetNext(c);
  }   

  dc->ConatctCount = i;

  for (j = i; j < dc->MaxContacts; j++) {
    WTnode_enable(dc->lines[j], 0);
  }
}

#endif

 

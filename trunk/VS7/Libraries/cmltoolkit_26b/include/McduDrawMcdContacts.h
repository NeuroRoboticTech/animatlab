#ifndef _MCDUDRAWMCDCONTACTS_H
#define _MCDUDRAWMCDCONTACTS_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:41 $
   
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

#ifdef WIN32
#include <windows.h>
#endif /* WIN32 */

#ifndef PS2
#include <GL/gl.h>
#endif

/* to use:
   - call McduCreateMcdContactGraphics(rc, contactCount )
      if contactCount is 0, it is assumed that the SetContactArray fn will
      be used and no allocation of contacts is done.
   - at each tick call either McduCollectMcdContacts or McduSetContactArray or set
     a callback in Mst
   - toggle display with  McduToggleMcdContactDrawing
     for example: RUseKey('c',McduToggleMcdContactDrawing);

  
    
    McduCreateMcdContactGraphics(rc, 500);
    MstBridgeSetIntersectActiveCB(bridge, kMstResponseBody, kMstResponseParticleSystem,
                                McduCollectContactPairs, 0);
    McduToggleMcdContactDrawing();


*/

struct MdtContact;

void MEAPI McduDisplayMcdContacts( void*vrc );

static int usedMcdContactsSize = 0;
static int maxMcdContactsSize = 0;
McdContact *usedMcdContacts = 0;
static int resetMcdContacts = 1;

int McduDrawMcdContacts = 0;
RGraphic* McduMcdContactGraphics = 0;

RGraphic* MEAPI McduCreateMcdContactGraphics(RRender *rc, int contactCount)
{
  float color[4] = {1,1,0,1};
  maxMcdContactsSize = contactCount;
  if (contactCount > 0)
    usedMcdContacts = (McdContact*) malloc(sizeof(McdContact)*contactCount);
  return McduMcdContactGraphics =
    McduMcdContactGraphics = RGraphicProceduralCreate(rc,
             McduDisplayMcdContacts, 0,
             color, 0 );
}


RGraphic* MEAPI McduCreateMcdContactGraphicsNoReset(RRender *rc, int contactCount)
{
  resetMcdContacts = 0;
  return McduCreateMcdContactGraphics(rc,contactCount);
}



void MEAPI McduResetMcdContactGraphics()
{
  usedMcdContactsSize = 0;
}

/**
 * free allocated memory
 */
void MEAPI McduDrawContactFreeMemory()
{
  if (usedMcdContacts) free(usedMcdContacts);
  usedMcdContacts = 0;
}


unsigned int MEAPI McduCollectMcdContacts(MdtContact *mdtC, McdContact *mcdC,
            McdIntersectResult *ir)
{
  /* copy contact from mcdC */
  int i=ir->contactCount;
  if (usedMcdContactsSize < maxMcdContactsSize && i) {
    usedMcdContacts[usedMcdContactsSize++] = *mcdC;
  }
  return 1;
}

void MEAPI McduCollectContactPairs(McdIntersectResult*ir, void** mdtCPvoid, void*userData)

{
  MdtContact** mdtCP = (MdtContact**) mdtCPvoid;
  /* copy contacts from ir */
  int i=ir->contactCount;
  while (usedMcdContactsSize < maxMcdContactsSize && i--) {
    usedMcdContacts[usedMcdContactsSize++] = ir->contacts[i];
  }
}

/* alternate way of displaying contacts */
void MEAPI McduSetContactArray(McdContact* a, int size)
{
  usedMcdContacts = a;
  usedMcdContactsSize = size;
}

void MEAPI McduToggleMcdContactDrawing(void)
{
  McduDrawMcdContacts ^= 1;
}

void MEAPI McduDisplayMcdContacts( void*userData )

{
#ifndef PS2
  /* RRender *rc = (RRender*)vrc; */
  /* int index=McduContactGraphics->m_data->m_glUserCallList; */
  int i;
  MeReal *x;
  MeReal *n;
  MeReal d;
  MeReal npx[3];

  /*fprintf(stderr,"<%d>",usedMcdContactsSize );*/
  if (!McduMcdContactGraphics || !McduDrawMcdContacts || usedMcdContactsSize<1)
    return;

  /* glNewList(index, GL_COMPILE);    */
  glPushAttrib(GL_LIGHTING_BIT|GL_LINE_BIT);
  glLineWidth(2);
  glDisable(GL_LIGHTING);

    {
      for ( i = 0; i < usedMcdContactsSize; i++ )
  {
    glBegin(GL_LINES);
    glColor3f(1,1,1);
    x = &usedMcdContacts[i].position[0];
    n = &usedMcdContacts[i].normal[0];
    d = -usedMcdContacts[i].separation;

    APPEND_FV_OR_DV(glVertex3)( x );
    /*glColor3f(0,0.5,1);*/

    npx[0] = x[0]+d*n[0];
    npx[1] = x[1]+d*n[1];
    npx[2] = x[2]+d*n[2];
    APPEND_FV_OR_DV(glVertex3)( npx );

    glColor3f(0,0.5,1);
    APPEND_FV_OR_DV(glVertex3)( npx );

    npx[0] = x[0]+n[0];
    npx[1] = x[1]+n[1];
    npx[2] = x[2]+n[2];
    glColor3f(0,0.5,1);
    APPEND_FV_OR_DV(glVertex3)( npx );

    glEnd();
  }
    }
  if (resetMcdContacts) usedMcdContactsSize = 0;

  glPopAttrib();
  /* glEndList();   */
#endif

}

#endif /* _MCDUDRAWMCDCONTACTS_H */

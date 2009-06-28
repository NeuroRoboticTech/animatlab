#ifndef _RMENU_H
#define _RMENU_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:45 $
   
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

#include "MeViewer.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
   Switch on the main menu.

   @param rc The renderer to display the default menu of.
 */
void   MEAPI RRenderDisplayDefaultMenu(RRender *rc);

/**
   Discover which menu is currently being displayed.

   @param rc The render context in question.
   @return A pointer to the currently displayed menu.
 */
RMenu* MEAPI RRenderGetCurrentMenu(RRender *rc);

/**
   Stop displaying a menu.

   @param rc The render context to cease display of a menu in.
 */
void   MEAPI RRenderHideCurrentMenu(RRender *rc);

/**
   Move the active menu entry one place down.

   @param rc The render context in question.
 */
void   MEAPI RMenuNextEntry(RRender *rc);
/**
   Move the active menu entry one place up.

   @param rc The render context in question.
 */
void   MEAPI RMenuPreviousEntry(RRender *rc);
/**
    Execute the first action of this entry - e.g. reduce the value of a
    value entry.

   @param rc The render context in question.
 */
void   MEAPI RMenuExecute1Entry(RRender *rc);
/**
    Execute the second action of this entry - e.g. increase the value of a
    value entry.

   @param rc The render context in question.
 */
void   MEAPI RMenuExecute2Entry(RRender *rc);

#ifdef __cplusplus
}
#endif

#endif

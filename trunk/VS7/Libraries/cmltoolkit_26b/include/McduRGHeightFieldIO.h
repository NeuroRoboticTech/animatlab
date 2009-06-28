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

#include "McdRGHeightField.h"

#ifdef __cplusplus
extern "C" {
#endif

    
/**
   @file McduRGHeightFieldIO.h

    HeightField loading and saving to/from bmp files.
*/

/**
    Loads a 24 bit per pixel.bmp file into a height matrix.
    This allocate memory that must be freed later.

    @param filename The name of the file to attempt to load.
    @param outHeightMatrix is allocated and contains the height array suitable
    for use with McdRGHeightFieldCreate.
    The height at a given position is a weighted sum of the color components,
    as follows:
     rMult*r + gMult*g + bMult*b + z0, where r,g,b are between 0 and 1.
    @param outXsize will contain the number of points along the x axis.
    @param outYsize will contain the number of points along the y axis.
    @a z0 is added to all heights.
    Returns 0 in case of failure, non zero otherwise.

*/


int MEAPI McduRGHeighFieldCreateHeightMatrixFromBmp(
                                char *filename,
                                MeReal rMult, MeReal gMult, MeReal bMult, MeReal z0,
                                MeReal **outHeightMatrix,
                                int *outXsize, int *outYsize);


/**
    Create a heightfield from a .bmp file.
    @a x0 and @y0 are the x and y coordinates of the left bottom corner of the
    heightfield.
    @a xSize and @a ySize are the sizes in each dimension.
    @a rMult, @a gMult, @bMult are multipliers of the red, greed, and blue
    components of the image, each being between 0 and 1.
    @a z0 is the offset height added to each height.
    Memory for the height matrix is allocated in this function.
    @see McduRGHeighFieldCreateHeightMatrixFromBmp for more detail.

*/
McdRGHeightFieldID MEAPI McduRGHeighFieldCreateFromBmp(
                                char *bmpFileName,
                                MeReal x0, MeReal y0, MeReal xSize, MeReal ySize,
                                MeReal rMult, MeReal gMult, MeReal bMult, MeReal z0
                                );

McdRGHeightFieldID MEAPI McduRGHeighFieldCreateFromBmpEx(
                                char *bmpFileName,
                                MeReal x0, MeReal y0, MeReal xSize, MeReal ySize,
                                MeReal rMult, MeReal gMult, MeReal bMult, MeReal z0, 
                                int *lpWidth, int *lpHeight, MeReal *lpDeltaX, MeReal *lpDeltaY);

/** Save the heightfield geometry to a .bmp of .ppm file.
    If @a coloredFullPrecision is used, the three colors are used to get a hight precision
    in the heights, but as a result the image is probably not useful for editing. A grey scale
    image is produced otherwise. */
int MEAPI McduRGHeighFieldSaveToFile(McdRGHeightFieldID g,char *fileName, int coloredFullPrecision);

#ifdef __cplusplus
} /* extern "C" */
#endif

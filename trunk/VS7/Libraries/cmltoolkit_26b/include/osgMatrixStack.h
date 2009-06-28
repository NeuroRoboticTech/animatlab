#ifndef _OSG_MATRIX_STACK_H_
#define _OSG_MATRIX_STACK_H_

// osgMatrixStack.h: interface for the osgMatrixStack class.
/*
   Copyright (c) 2000-2003 CMLabs Simulations Inc. 
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/26 11:07:47 $
   
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
//////////////////////////////////////////////////////////////////////

#include <osg/Transform>
#include <osg/MatrixTransform>

struct VxMATRIX_STACK;

void getOSGMatrixScale(const osg::Matrix &m,osg::Vec3 &v3);

class osgMatrixStack
{
public:
    osgMatrixStack();
    virtual ~osgMatrixStack();
    
    const osg::Matrix& getTopMatrix() const;
    osg::Matrix* getTopMatrix(osg::Matrix*) const;
    void pushMatrixStack(osg::Matrix *mat);
    void pushMatrixStack();
    void popMatrixStack();
    int preMultiplyMatrixStack(const osg::Matrix &mat);
    int postMultiplyMatrixStack(const osg::Matrix &mat);
    void resetMatrixStack();
        
    int get_size() const;
    
    bool is_empty() const;

    // internal
    osg::Matrix *_get(int index, osg::Matrix * ret);

protected:
    VxMATRIX_STACK *m_stack;
};

#endif // _OSG_MATRIX_STACK_H_
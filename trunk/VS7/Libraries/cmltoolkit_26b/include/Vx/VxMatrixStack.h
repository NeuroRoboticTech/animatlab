#ifndef _VXMAXTRIXSTACK_H
#define _VXMAXTRIXSTACK_H

// VxMatrixStack.h: interface for the VxMatrixStack class.
/*
   Copyright (c) 2000-2003 CMLabs Simulations Inc. 
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/12/13 20:37:18 $
   
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

#include "Vx/Vx.h"
#include "Vx/VxPrecision.h"
#include <vector>

struct VxSTACK;

struct VxMATRIX_STACK;

/**
* @internal
* This class facilitates the use of "vector" as a stack.
* Stores "void" pointers.
*/
class VxStack
{
public:
    VxStack();
    VxStack(VxNode n);
    virtual ~VxStack();
    virtual VxNode getTop();
    virtual void pushStack(VxNode vx); 
    virtual void popStack();
    virtual void reset();    
    
    virtual VxNode get(int index);

    virtual VxNode get_begin();
    
    virtual VxNode get_last();
    
    virtual VxNode get_end();
    
    virtual int get_size();
    
    virtual bool is_empty() const;

protected:
    VxNode _n;
    VxSTACK *m_Stack;
    
};

/** 
* @internal 
* Implements a stack of VxMatrices.
*/
class VxMatrixStack
{
public:
    VxMatrixStack();
    virtual ~VxMatrixStack();
    
    VxReal44Ptr getTopMatrix();
    void pushMatrixStack();
    void pushMatrixStack(VxReal44Ptr *mat);
    void popMatrixStack();
    int preMultiplyMatrixStack(const VxReal44Ptr mat);
    int postMultiplyMatrixStack(const VxReal44Ptr mat);
    void resetMatrixStack();
        
    VxReal44Ptr get(int index);

    int get_size();
    
    VxReal44Ptr get_begin();
    
    VxReal44Ptr get_last();
    
    VxReal44Ptr get_end();
    
    bool is_empty() const;

protected:
    VxReal44 _m;
    VxStack *m_matStack;
    
};

/**
* @internal
* Matrix functions
*/
#ifdef __cplusplus
extern    "C" {
#endif

#define INNERPRODUCT(a,b,r,c) \
(a[r][0] * b[0][c]) +(a[r][1] * b[1][c])+(a[r][2] * b[2][c])+(a[r][3] * b[3][c])

#define SETROW(m,row, v1, v2, v3, v4 )    \
    m[row][0] = (v1); \
    m[row][1] = (v2); \
    m[row][2] = (v3); \
    m[row][3] = (v4);

void VX_API
preMultiply(VxReal44Ptr  m, const VxReal44Ptr other );

void VX_API
postMultiply(VxReal44Ptr m, const VxReal44Ptr other );

#ifdef __cplusplus
}
#endif

#endif

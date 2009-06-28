#ifndef VXVECTOR3_H
#define VXVECTOR3_H
/* -*- mode: C++; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/03/02 18:02:08 $
   
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

#include "Vx/VxPrecision.h"

namespace Vx
{

// tolua_begin
class VX_DLLSPEC VxVector3
{
// tolua_end
public:
    VxReal v[3];
 // tolua_begin
   
public:
    
    VxVector3()
    {
    }
    
    VxVector3(const VxReal t1, const VxReal t2, const VxReal t3)
    {
        v[0] = t1; v[1] = t2; v[2] = t3;
    }
    
    VxVector3(const VxVector3& V)
    {
        v[0] = V[0]; v[1] = V[1]; v[2] = V[2];
    }
    
    void set(const VxReal x, const VxReal y, const VxReal z)
    {
        v[0] = x; v[1] = y; v[2] = z;
    }
    
    void set(const VxVector3& V)
    {
        v[0] = V[0]; v[1] = V[1]; v[2] = V[2];
    }
    
    void get(double *x, double *y, double *z) const
    {
        *x = (double) v[0]; *y = (double) v[1]; *z = (double) v[2];
    }
    
    void get(double *v1) const
    {
        v1[0] = (double) v[0]; v1[1] = (double) v[1]; v1[2] = (double) v[2];
    }
    
    void get(float *x, float *y, float *z) const
    {
        *x = (float) v[0]; *y = (float) v[1]; *z = (float) v[2];
    }
    
    void get(float *v1) const
    {
        v1[0] = (float) v[0]; v1[1] =  (float) v[1]; v1[2] =  (float) v[2];
    }
    
    void set(const double *V)
    {
        v[0] = (VxReal) V[0]; v[1] = (VxReal) V[1]; v[2] = (VxReal) V[2];
    }
    
    void set(const float *V)
    {
        v[0] = (VxReal) V[0]; v[1] = (VxReal) V[1]; v[2] = (VxReal) V[2];
    }
    
    const VxReal&  operator[](unsigned int i) const {return v[i];}
    
    VxReal&  operator[](int i) {return v[i];}
    
// tolua_end
    const VxReal *getReal() const { return v;}
    

    VxVector3& operator=(const VxVector3& v1)
    {
        v[0] = v1[0]; v[1] = v1[1]; v[2] = v1[2];
        return *this;
    }
    
    VxVector3& operator+=(const VxVector3& v1)
    {
        v[0] += v1[0]; v[1] += v1[1]; v[2] += v1[2];
        return *this;
    }
    
    
    VxVector3& operator-=(const VxVector3& v1)
    {
        v[0] -= v1[0]; v[1] -= v1[1]; v[2] -= v1[2];
        return *this;
    }
    
    
    VxVector3& operator*=(const VxVector3& v1)
    {
        v[0] *= v1[0]; v[1] *= v1[1]; v[2] *= v1[2];
        return *this;
    }
    
    VxVector3& operator*=(const VxReal& x)
    {
        v[0] *= x; v[1] *= x; v[2] *= x;
        return *this;
    }
    
    VxVector3& operator/=(const VxReal& x)
    {
        VxReal recipX = (VxReal)(1.0)/x;
        v[0] *= recipX; v[1] *= recipX; v[2] *= recipX;
        return *this;
    }
    
    bool operator==(const VxVector3& v1) const
    {
        return ( v[0]==v1[0] && v[1]==v1[1] && v[2]==v1[2] );
    }
    
    bool operator!=(const VxVector3& v1) const
    {
        return ( v[0]!=v1[0] || v[1]!=v1[1] || v[2]!=v1[2] );
    }
    
// tolua_begin

    VxReal dot(const VxVector3& v1) const
    {
        return v[0]*v1[0] + v[1]*v1[1] + v[2]*v1[2];
    }
    
    VxReal normSquare(void) const {return this->dot(*this);}
    VxReal norm(void) const {return MeSqrt(this->dot(*this));}
    
    VxReal normalize()
    {
        VxReal t = normSquare();
        VX_ASSERT(t>0, ("VxVector3::normalize(): this has 0 norm"));
        t = MeSqrt(t);
        *this /= t;
        return t;
    }
    
    bool normalize( VxReal eps, VxReal* n)
    {
        *n = norm();
        if(*n>eps) 
        {
            *this /= *n;
            return true;
        }
        else
        {
            *n = 0;
            return false; 
        }
    }
    
    VxVector3 operator+(const VxVector3& v2) const
    {
        return VxVector3(v[0] + v2[0], v[1] + v2[1], v[2] + v2[2]);
    }
    
    VxVector3 operator-(const VxVector3& v2) const
    {
        return VxVector3(v[0] - v2[0], v[1] - v2[1], v[2] - v2[2]);
    }
    
    VxVector3 operator*(const VxReal& t) const
    {
        return VxVector3(v[0]*t, v[1]*t, v[2]*t);
    }
    
    VxVector3 operator/(const VxReal& t) const
    {
        VxReal recipT = (VxReal)(1.0)/t;
        return VxVector3(v[0]*recipT, v[1]*recipT, v[2]*recipT);
    }
     
    VxVector3 operator-() const
    {
        return VxVector3(-v[0],-v[1],-v[2]);
    }
    
    const VxVector3& add(const VxVector3 &v1)
    {
        v[0] += v1[0]; 
        v[1] += v1[1]; 
        v[2] += v1[2];
        return *this;
    }
    
    const VxVector3& sub(const VxVector3 &v1)
    {
        v[0] -= v1[0]; 
        v[1] -= v1[1]; 
        v[2] -= v1[2];
        return *this;
    }
    
    const VxVector3& scale(const VxReal s)
    {
        v[0] *= s; 
        v[1] *= s; 
        v[2] *= s;
        return *this;
    }
    
    const VxVector3&  addMultiple(const VxReal a, const VxVector3 &v2)
    {
        v[0] += a*v2[0]; 
        v[1] += a*v2[1]; 
        v[2] += a*v2[2];
        return *this;
    }
    
    const VxVector3& preCross( const VxVector3& v1)
    {
        VxVector3 v2 = *this;
        VX_ASSERT(this != &v1 && this != &v2, ("VxVector3::cross: can't use this as a parameter"));
        v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
        v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
        v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
        return *this;
    }

    const VxVector3& postCross( const VxVector3& v2)
    {
        VxVector3 v1 = *this;
        VX_ASSERT(this != &v1 && this != &v2, ("VxVector3::cross: can't use this as a parameter"));
        v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
        v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
        v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
        return *this;
    }

    const VxVector3& setSum(const VxVector3 &v1, const VxVector3 &v2)
    {
        v[0] = v1[0] + v2[0]; 
        v[1] = v1[1] + v2[1]; 
        v[2] = v1[2] + v2[2];
        return *this;
    }
    
    const VxVector3& setDifference(const VxVector3 &v1, const VxVector3 &v2)
    {
        v[0] = v1[0] - v2[0]; 
        v[1] = v1[1] - v2[1]; 
        v[2] = v1[2] - v2[2];
        return *this;
    }

    const VxVector3&  setLinear(const VxReal a, const VxVector3 &v2, const VxReal b, const VxVector3 &v3)
    {
        v[0] = a*v2[0]+b*v3[0]; 
        v[1] = a*v2[1]+b*v3[1]; 
        v[2] = a*v2[2]+b*v3[2];
        return *this;
    }
    
    const VxVector3& setCross( const VxVector3& v1, const VxVector3& v2 )
    {
        VX_ASSERT(this != &v1 && this != &v2, ("VxVector3::cross: can't use this as a parameter"));
        v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
        v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
        v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
        return *this;
    }

    void project(const VxVector3& normal, VxVector3* nProj, VxVector3* tProj) const
    {
        *nProj = normal;
        nProj->scale(this->dot(normal));
        tProj->setDifference(*this, *nProj);
    }

    const VxVector3& setMin(const VxVector3& v1)
    {
        if (v[0] > v1[0])
            v[0] = v1[0];
        if (v[1] > v1[1])
            v[1] = v1[1];
        if (v[2] > v1[2])
            v[2] = v1[2];
        return *this;
    }
    const VxVector3& setMax(const VxVector3& v1)
    {
        if (v[0] < v1[0])
            v[0] = v1[0];
        if (v[1] < v1[1])
            v[1] = v1[1];
        if (v[2] < v1[2])
            v[2] = v1[2];
        return *this;
    }
    //static VxVector3* cast(VxReal3Ptr v1) { return static_cast<VxVector3*>(v1); }
    static VxVector3* cast(VxReal3Ptr v1) { return (VxVector3*)(v1); }
};
// tolua_end

} // namespace Vx

#endif // VXVECTOR3_H

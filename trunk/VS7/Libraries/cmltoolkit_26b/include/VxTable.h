#ifndef _VXTABLE_H_
#define _VXTABLE_H_

#include "Vx/Vx.h"
#include "Vx/VxPrecision.h"
#include "Vx/VxMessage.h"

namespace Vx
{

struct vx_table_1d
{
    int size;
    Vx::VxReal *x;
    Vx::VxReal *y;
};
struct vx_table_2d
{
    int sizex;
    int sizey;
    Vx::VxReal *x;
    Vx::VxReal *y;
    Vx::VxReal *z;
};

/** @internal */
inline Vx::VxReal linearInterp(Vx::VxReal x, Vx::VxReal x0, Vx::VxReal x1, Vx::VxReal y0, Vx::VxReal y1)
{
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

/** The class VxTable1D contains two arrays X and Y and provides with linear interpolation 
    of a y value from a given x value. The value in the X array are assumed to be in increasing 
    order.
    The data vectors use by the class a allocated by the user. 
*/
class VxTable1D
{
public:
    /** Default constructor. */
    VxTable1D() : size(0), x(0), y(0) {}
    /** Class constructor with input arrays. */
    VxTable1D(int n, Vx::VxReal* inX, Vx::VxReal* inY)
    {
        VxTable1D::set(n, inX, inY);
    }
    /** Class destructor. */
    virtual ~VxTable1D() {}
    
    /** Returns true if the arrays are set and valid. */
    bool empty() const { return size == 0; }

    /** Sets the internal arrays. */
    virtual void set(int n, Vx::VxReal* inX, Vx::VxReal* inY)
    {
        size = n;
        x = inX;
        y = inY;
    }
    /** Verifies that the x array has values in growing order. */
    void checkVector()
    {
        for (int i=0; i<size-1; ++i)
            VX_ASSERT(x[i] < x[i+1], ("VxTable1D::checkVector: must have x[i] < x[i+1].\n"));
    }

    /** interpolate y value */
    Vx::VxReal interp_y(Vx::VxReal inX) const
    {
        if (inX <= x[0])
        {
            return y[0]; // could extrapolate
        }
        else if (inX >= x[size-1])
        {
            return y[size-1]; // could extrapolate
        }
        else
        {
            int i=1;
            while (inX >= x[i]) ++i;

            // linear interpolation
            return linearInterp(inX, x[i-1], x[i], y[i-1], y[i]);
        }
    }
    /** interpolate x, works only if y is increasing with x */
    Vx::VxReal interp_x(Vx::VxReal inY) const
    {
        if (inY <= y[0])
        {
            return x[0]; // could extrapolate
        }
        else if (inY >= y[size-1])
        {
            return x[size-1]; // could extrapolate
        }
        else
        {
            int i=1;
            while (inY > y[i]) ++i;

            // linear interpolation
            return linearInterp(inY, y[i-1], y[i], x[i-1], x[i]);
        }
    }
    Vx::VxReal *x;
    Vx::VxReal *y;
    int size;
};

/** The class VxDynTable1D contains two arrays X and Y and provides with linear interpolation 
    of a y value from a given x value. The value in the X array are assumed to be in increasing 
    order.
    The data vectors are allocated internally and their content copied 
    from the user's data vectors. 
*/
class VxDynTable1D : public VxTable1D
{
public:
    /** Class constructor with double arrays. */
    VxDynTable1D(int n, double* inX, double* inY)
    {
        size = n;
        x = new Vx::VxReal[n];
        y = new Vx::VxReal[n];
        for (int i=0; i<n; ++i)
        {
            x[i] = (Vx::VxReal) inX[i];
            y[i] = (Vx::VxReal) inY[i];
        }
    }
    /** Class constructor with float arrays. */
    VxDynTable1D(int n, float* inX, float* inY)
    {
        size = n;
        x = new Vx::VxReal[n];
        y = new Vx::VxReal[n];
        for (int i=0; i<n; ++i)
        {
            x[i] = (Vx::VxReal) inX[i];
            y[i] = (Vx::VxReal) inY[i];
        }
    }
    /** Class destructor. */
    virtual ~VxDynTable1D()
    {
        delete []x;
        delete []y;
    }

private:
    VxDynTable1D(){}
    void set(int n, Vx::VxReal* inX, Vx::VxReal* inY) {}
};

/** The class VxTable2D contains 3 arrays X, Y and Z and provides with linear interpolation 
    of a z value from a given x and y value. The value in the X and Y arrays are assumed 
    to be in increasing order.
    The data vectors used by the class a allocated by the user. 
*/
class VxTable2D
{
public:
    /** Default class constructor. */
    VxTable2D() : sizex(0), sizey(0), x(0), y(0), z(0) {}
    /** Class constructor with input arrays. */
    VxTable2D(int nx, int ny, Vx::VxReal* inX, Vx::VxReal* inY, Vx::VxReal* inZ)
    {
        set(nx, ny, inX, inY, inZ);
    }
    virtual ~VxTable2D() {}

    /** Returns true if valid arrays are set to the class. */
    inline bool empty() const { return sizex == 0 || sizey == 0; }

    /** @internal */
    inline Vx::VxReal zval(int i, int j) const { return z[i*sizey+j]; }
    /** @internal */
    inline Vx::VxReal* zrow(int i) const { return &z[i*sizey]; }

    /** Sets the internal arrays. */
    void set(int nx, int ny, Vx::VxReal* inX, Vx::VxReal* inY, Vx::VxReal* inZ)
    {
        sizex = nx;
        sizey = ny;
        x = inX;
        y = inY;
        z = inZ;
    }

    /** Verifies that the x array has values in growing order. */
    void checkVector()
    {
        int i;

        for (i=0; i<sizex-1; ++i)
            VX_ASSERT(x[i] < x[i+1], ("VxTable2D::checkVector: must have x[i] < x[i+1].\n"));
        for (i=0; i<sizey-1; ++i)
            VX_ASSERT(y[i] < y[i+1], ("VxTable2D::checkVector: must have y[i] < y[i+1].\n"));
    }

    /** interpolate z value */
    Vx::VxReal interp_z(Vx::VxReal inX, Vx::VxReal inY) const
    {
        if (inX <= x[0])
        {
            VxTable1D t(sizey, y, zrow(0));
            return t.interp_y(inY);
        }
        else if (inX >= x[sizex-1])
        {
            VxTable1D t(sizey, y, zrow(sizex-1));
            return t.interp_y(inY);
        }
        else
        {
            int i=1;
            while (inX > x[i]) ++i;
            if (inY <= y[0])
            {
                return linearInterp(inX, x[i-1], x[i], zval(i-1, 0), zval(i, 0));
            }
            else if (inY >= y[sizey-1])
            {
                return linearInterp(inX, x[i-1], x[i], zval(i-1,sizey-1), zval(i, sizey-1));
            }
            else
            {
                int j=1;
                while (inY > y[j]) ++j;

                Vx::VxReal z0 = linearInterp(inX, x[i-1], x[i], zval(i-1, j-1), zval(i,j-1));
                Vx::VxReal z1 = linearInterp(inX, x[i-1], x[i], zval(i-1, j), zval(i, j));
                return linearInterp(inY, y[j-1], y[j], z0, z1);
            }
        }
    }
    Vx::VxReal *x;
    Vx::VxReal *y;
    Vx::VxReal *z;
    int sizex;
    int sizey;
};

/** The class VxDynTable2D contains 3 arrays X, Y and Z and provides with linear interpolation 
    of a z value from a given x and y value. The value in the X and Y arrays are assumed 
    to be in increasing order.
    The data vectors are allocated internally and their content copied 
    from the user's data vectors. 
*/
class VxDynTable2D : public VxTable2D
{
public:
    /** Class constructor with initial array size. */
    VxDynTable2D(int nx, int ny)
    {
        allocate(nx, ny);
    }
    /** Class constructor with double arrays. */
    VxDynTable2D(int nx, int ny, double* inX, double* inY, double* inZ)
    {
        allocate(nx, ny);
        copyArray(x, inX, sizex);
        copyArray(y, inY, sizey);
        copyArray(z, inZ, sizex*sizey);
    }
    /** Class constructor with float arrays. */
    VxDynTable2D(int nx, int ny, float* inX, float* inY, float* inZ)
    {
        allocate(nx, ny);
        copyArray(x, inX, sizex);
        copyArray(y, inY, sizey);
        copyArray(z, inZ, sizex*sizey);
    }
    /** Class destructor. */
    virtual ~VxDynTable2D()
    {
        delete []x;
        delete []y;
        delete []z;
    }

private:
    VxDynTable2D(){}
    void set(int nx, int ny, Vx::VxReal* inX, Vx::VxReal* inY, Vx::VxReal* inZ) {}
    void allocate(int nx, int ny)
    {
        sizex = nx;
        sizey = ny;
        x = new Vx::VxReal[nx];
        y = new Vx::VxReal[ny];
        z = new Vx::VxReal[nx*ny];
    }
    void copyArray(Vx::VxReal* to, float* from, int size)
    {
        for (int j=0; j<size; ++j)
            to[j] = (Vx::VxReal) from[j];
    }
    void copyArray(Vx::VxReal* to, double* from, int size)
    {
        for (int j=0; j<size; ++j)
            to[j] = (Vx::VxReal) from[j];
    }
};

} // namespace Vx


#endif // _VXTABLE_H_

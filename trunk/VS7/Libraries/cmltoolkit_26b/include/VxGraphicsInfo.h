#ifndef _VX_GRAPHICS_INFO_H_
#define _VX_GRAPHICS_INFO_H_

// VxGraphicsInfo.h: interface for the VxGraphicsInfo class.
//
//////////////////////////////////////////////////////////////////////

class VxParticleSystem;

class VxGraphicsInfo
{
public:
    VxGraphicsInfo(){}
    virtual ~VxGraphicsInfo(){};

    void setVxParticalSystem(VxParticleSystem* ps);
    void setNumberOfColors(int colorCount);
    void setArrayOfColors(float* colorArray);
    void setParticleAgeLimits(VxReal* ageLimits);
    void setParticleSizes(VxReal* sizes);

    void getInterpolatedParticleColorFromAge(VxReal age, float * Col, VxReal *size);
    void getParticleColorAndSize(int i, float* Col, VxReal *size);
    
protected:
  VxParticleSystem*       m_ps;  /**< Particle system this data refers to */

  int              m_NumberOfColors;  /**< Number of colors to interpolate between */
  float*           m_Colors;          /**< Pointer to array containing colors, 4 values (r,g,b,a) for each */
  VxReal*          m_AgeLimits;       /**< Relative particle ages (0..1) to which these colors correspond. 
                                          Initially, particle has color @a m_Colors[0], after reaching relative
                                          age @a m_AgeLimits[1] it will get color @a m_Colors[4], etc. */
  VxReal*          m_Sizes;           /**< Sizes for particles also interpolated using same @a m_ageLimits as colors */
};

#endif // _VX_GRAPHICS_INFO_H_
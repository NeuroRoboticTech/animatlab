#ifndef _VX_OSG_PARTICLE_SYSTEM_
#define _VX_OSG_PARTICLE_SYSTEM_

// VxPsDrawable.h: interface for the VxPsDrawable class.
//
//////////////////////////////////////////////////////////////////////

// OSG includes
#include <osg/Drawable>

class VxParticleSystem;
class VxGraphicsInfo;

#include "Vx/Vx.h"

typedef GLfloat      GLReal;
typedef GLReal       GLReal3[3];
typedef GLReal       GLReal2[2];
typedef GLReal       GLReal4[4];


class vxOSGParticleSystem: public osg::Drawable
{
public:
    vxOSGParticleSystem():osg::Drawable(),m_ps(0),current_alpha_(1)
    {
        reset_bounds_flag_ = false;
        bounds_computed_ = false;
        
        current_color_ = osg::Vec4(1, 0.5f, 0.3f, 1.0f);
        setSupportsDisplayList(false);
        coord3 = 0;
        normal3 = 0;
        tcoord2 = 0;
        color4 = 0;
        
    };
    vxOSGParticleSystem(const vxOSGParticleSystem &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);      
    META_Object(VxOSGdll, vxOSGParticleSystem);
    virtual ~vxOSGParticleSystem();
    
    void setVxParticleSystem(VxParticleSystem* ps);
    void setGraphicsInfo(VxGraphicsInfo *gi) {m_gi = gi;}
    osg::StateSet *setOSGAttributesAndModes(const std::string &texturefile,bool emissive_particles, bool lighting, int texture_unit = 0);
    void update(osg::Matrix &modelview)const;
    void render(const osg::Vec3 &xpos, double size) const;
    void render2(const osg::Vec3 &xpos, double size, osg::Vec3Array* coords4,osg::Vec2Array* tcoords4, int index) const;
    
    VxParticleSystem* getVxPs() {return m_ps;};
    
    void drawImplementation(osg::State &state) const;
    void update_bounds(const osg::Vec3 &p, float r)const ;
    
    virtual bool computeBound() const 
    {
        if (!bounds_computed_) {
            _bbox.set( -10, -10, -10, 100, 100, 100);
        } else {
            _bbox._min = bmin;
            _bbox._max = bmax;
        }
        _bbox_computed = true;
        return true;
    }
    
    osg::Vec3 bmin;
    osg::Vec3 bmax;
    bool reset_bounds_flag_;
    bool bounds_computed_;
    
protected:
    
    vxOSGParticleSystem& operator = (const vxOSGParticleSystem&) { return *this;}
    
    VxParticleSystem* m_ps;
    VxGraphicsInfo *    m_gi;
    std::string m_texturefile;
    osg::Image* m_image;
    int m_numParticles;
    VxReal current_alpha_;
    osg::Vec4 current_color_;
    osg::StateSet *m_state;
    
    GLReal3* coord3;
    GLReal3* normal3;
    GLReal2* tcoord2;
    GLReal4* color4;
};

#endif // _VX_OSG_PARTICLE_SYSTEM_
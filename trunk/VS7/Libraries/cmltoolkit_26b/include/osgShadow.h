#ifndef _OSG_SHADOW_H_
#define _OSG_SHADOW_H_

// osgShadow.h: interface for the osgShadow class.
//
//////////////////////////////////////////////////////////////////////
#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/Node>
#include <osgUtil/TransformCallback>
#include <osgUtil/RenderToTextureStage>
#include <osg/PolygonOffset>
#include <osg/CullFace>
#include <osg/Material>

extern osg::Group* createSkyModel();

const int depth_texture_height = 512;
const int depth_texture_width  = 512;
/*
osg::ref_ptr<osg::RefMatrix> bias = new osg::RefMatrix(0.5f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 0.5f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 0.5f, 0.0f,
                                        0.5f, 0.5f, 0.5f, 1.0f);
*/

class LightTransformCallback: public osg::NodeCallback
{

public:

  LightTransformCallback(float angular_velocity, float height, float radius):
    _angular_velocity(angular_velocity),
    _height(height),
    _radius(radius),
    _previous_traversal_number(-1),
    _previous_time(-1.0f),
    _angle(0)
  {
  }

    void operator()(osg::Node* node, osg::NodeVisitor* nv);

protected:
    
  float                                  _angular_velocity;
  float                                  _height;
  float                                  _radius;
  int                                    _previous_traversal_number;
  double                                 _previous_time;
  float                                  _angle;
};

/*
*/
class RenderToTextureCallback: public osg::NodeCallback
{
public:
  RenderToTextureCallback(osg::Node* subgraph, 
                          osg::Texture2D* texture, 
                          osg::MatrixTransform* light_transform,
                          osg::TexGen* tex_gen):
    _subgraph(subgraph),
    _texture(texture),
    _local_stateset(new osg::StateSet),
    _viewport(new osg::Viewport),
    _light_projection(new osg::RefMatrix),
    _light_transform(light_transform),
    _tex_gen(tex_gen)
  {
    _local_stateset->setAttribute(_viewport.get());
    _local_stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
   
    osg::ref_ptr<osg::PolygonOffset> polygon_offset = new osg::PolygonOffset;
    polygon_offset->setFactor(1.1f);
    polygon_offset->setUnits(4.0f);
    _local_stateset->setAttribute(polygon_offset.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    _local_stateset->setMode(GL_POLYGON_OFFSET_FILL, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

    osg::ref_ptr<osg::CullFace> cull_face = new osg::CullFace;
    cull_face->setMode(osg::CullFace::FRONT);
    _local_stateset->setAttribute(cull_face.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    _local_stateset->setMode(GL_CULL_FACE, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

    _viewport->setViewport(0, 0, depth_texture_width, depth_texture_height);

    float znear = 1.0f * _subgraph->getBound().radius();
    float zfar  = 3.0f * _subgraph->getBound().radius();
    float top   = 0.5f * _subgraph->getBound().radius();
    float right = 0.5f * _subgraph->getBound().radius();
    znear *= 0.8f;
    zfar *= 1.2f;
    _light_projection->makeFrustum(-right, right, -top, top, znear, zfar);
  }

  virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
  {

     osgUtil::CullVisitor* cullVisitor = dynamic_cast<osgUtil::CullVisitor*>(nv);
     if (cullVisitor && _texture.valid() && _subgraph.valid())
      {            
        _render_to_depth_texture(*node, *cullVisitor);
      }

    // must traverse the subgraph            
    traverse(node,nv);
  }
        
  void _render_to_depth_texture(osg::Node& node, osgUtil::CullVisitor& cv);
        
  osg::ref_ptr<osg::Node>                     _subgraph;
  osg::ref_ptr<osg::Texture2D>                _texture;
  osg::ref_ptr<osg::StateSet>                 _local_stateset;
  osg::ref_ptr<osg::Viewport>                 _viewport;
  osg::ref_ptr<osg::RefMatrix>                     _light_projection;
  osg::ref_ptr<osg::MatrixTransform>               _light_transform;
  osg::ref_ptr<osg::TexGen>                        _tex_gen;
};

class osgHField : public osg::HeightField
{
public:
	osg::Node* createBase(const osg::Vec3& center,float radius);
};


class osgShadowTextureCullCallback : public osg::NodeCallback
{
public:
    
      osgShadowTextureCullCallback(osg::Node* shadower,const osg::Vec3& position, const osg::Vec4& ambientLightColor, unsigned int textureUnit);
      
      virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
      {
          
          osgUtil::CullVisitor* cullVisitor = dynamic_cast<osgUtil::CullVisitor*>(nv);
          if (cullVisitor && (_texture.valid() && _shadower.valid()))
          {           
              if (mShowShadow)
                  doPreRender(*node,*cullVisitor);
              else
                  traverse(node,nv);
              
              
          }
          else
          {
              // must traverse the shadower            
              traverse(node,nv);
          }
      }
    
      osg::Material* getShadowMaterial(){return mMaterial;};
      osg::Matrix mLightMatrix;
      osg::Vec3   _position;
      bool mShowShadow;    
      osg::ref_ptr<osg::Texture2D> _texture;

protected:
    // we need this to get round the order dependance
    // of eye linear tex gen...    
    class eyeTexGen : public osg::TexGen
    {
    public:
        
        void setMatrix(const osg::Matrix& matrix)
        {
            _matrix = matrix;
        }
        
        virtual void apply(osg::State& state) const
        {
            glPushMatrix();
            glLoadMatrix(_matrix.ptr());
            TexGen::apply(state);
            glPopMatrix();
        }
        
        osg::Matrix _matrix;
    };
    
    void doPreRender(osg::Node& node, osgUtil::CullVisitor& cv);
    
    osg::ref_ptr<osg::Node>      _shadower;
    osg::Vec4                    _ambientLightColor;
    unsigned int                 _unit;
    osg::ref_ptr<osg::StateSet>  _shadowState;
    osg::ref_ptr<osg::StateSet>  _shadowedState;
    osg::ref_ptr<osg::MatrixTransform> _light_transform;
    osg::Material*               mMaterial;

};


class osgShadow  
{
public:
    osgShadow();
    osgShadow(osg::Node* shadower, osg::Group* terrain,osg::Vec3& center, osg::Vec3& lightPosition,float radius, char *texture);
    virtual ~osgShadow();
    //	osg::HeightField*  readHeightFieldFile(const std::string& filename);
    osg::Node* createHFieldBase(float gridBlocks, float gridBlockVert, float **vertex);
    osg::Group* getShadowerHF();
    osg::Group* getShadowerOnPlane();
    osg::Node* getLightSource();
    osg::Node* getShadowLightSource();
    osg::Light* getLight();
    osg::Light* getShadowLight(){return mShadowLight;};
    osg::Material* getMaterial(){return (mShadowTextureCB)?mShadowTextureCB->getShadowMaterial():0;};
    void setLightPosition(osg::Vec3 &pos);
    void showShadow(bool sh);
    void setShadowedTexture(char *texture);

    void setMaterialAmbient(osg::Vec4 ambLight);
    void setMaterialDiffuse(osg::Vec4 diffuse);
    void setMaterialEmission(osg::Vec4 emmision);
    void setMaterialShininess(float shininess);
    

protected:
    osg::Group* mShadowerOnHField;
    osg::Node* mShadowed;
    osg::Node* mShadower;
    char *mTexture;
    osg::Vec3 mCenter;
    osg::Vec3 mLightPosition;
    float mRadius;
    osg::Light* mShadowLight;
    osg::Light *mLight;
    osg::Matrix mLightMatrix;
    osg::Node* createFlatBase();
    osg::Node* createShadowedScene(unsigned int textureUnit);
    osgShadowTextureCullCallback *mShadowTextureCB;
    osg::LightSource* mLightSource;
    osg::LightSource* mShadowLightSource;
};

#endif // _OSG_SHADOW_H_
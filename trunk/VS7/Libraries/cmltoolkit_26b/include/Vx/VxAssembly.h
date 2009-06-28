#ifndef _VX_ASSEMBLY_HPP_
#define _VX_ASSEMBLY_HPP_

#include "Vx/Vx.h"
#include "Vx/VxEntity.h"
#include "Vx/VxConstraint.h"

namespace Vx
{

class VxSensor;

struct VxID2PART;
struct VxID2CONSTRAINT;

// tolua_begin
class VxAssembly : public VxBase
{
public:
    VxAssembly();
    virtual ~VxAssembly();

    virtual const char* getClassName() const { return "VxAssembly"; }

    bool addEntity(VxEntity *entity);
    bool removeEntity(VxEntity *entity);
    int getEntityCount() const;
    VxEntity *getEntity(int entityID);

    bool addConstraint(VxConstraint *constraint);
    void removeConstraint(VxConstraint *constraint);
    int getConstraintCount() const;
    VxConstraint *getConstraint(int constraintID);

    virtual void transform(const VxReal44Ptr transform);
    virtual void preTransform(const VxReal44Ptr transform);

    void disableCollisionWith(VxSensor* p);

    void saveState();
    void restoreState();
    void saveEntityState(VxEntity* e);
    void restoreEntityState(VxEntity* e);

    bool contains(VxEntity* e) const { return e->getAssembly() == this; }
    bool contains(VxConstraint* c) const { return c->getAssembly() == this; }

    bool getBBox(VxReal3Ptr bmin, VxReal3Ptr bmax);//const;
    bool getBRadius(VxReal3Ptr center, VxReal* radius, VxReal3Ptr bmin=0, VxReal3Ptr bmax=0);//const;


    // tolua_end
protected:
    VxID2PART *mPartsArr;
    VxID2CONSTRAINT *mConstraintsArr;
}; // tolua_export

} // namespace Vx

#endif // _VX_ASSEMBLY_HPP_

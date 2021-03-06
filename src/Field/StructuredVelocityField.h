#ifndef __GEOMTK_StructuredVelocityField__
#define __GEOMTK_StructuredVelocityField__

#include "StructuredField.h"

namespace geomtk {

template <typename MeshType_, typename FieldType_>
class StructuredVelocityField {
public:
    typedef MeshType_ MeshType;
    typedef FieldType_ FieldType;
    typedef StructuredStagger::GridType GridType;
    typedef StructuredStagger::Location Location;
protected:
    vector<FieldType> v;
    FieldType div;
    vector<FieldType> vor;
public:
    StructuredVelocityField();
    virtual ~StructuredVelocityField();

    virtual void
    create(const MeshType &mesh, bool useStagger, bool hasHalfLevel = false);

    virtual void
    applyBndCond(const TimeLevelIndex<2> &timeIdx, bool updateHalfLevel = false);

    const MeshType&
    mesh() const {
        return v[0].mesh();
    }

    FieldType&
    operator()(int compIdx) {
        return v[compIdx];
    }

    const FieldType&
    operator()(int compIdx) const {
        return v[compIdx];
    }

    FieldType&
    divergence() {
        return div;
    }

    const FieldType&
    divergence() const {
        return div;
    }

    const vector<FieldType>&
    vorticity() const {
        return vor;
    }

    virtual void
    calcDivergence(const TimeLevelIndex<2> &timeIdx) = 0;

    virtual void
    calcVorticity(const TimeLevelIndex<2> &timeIdx) = 0;
}; // StructuredVelocityField

} // geomtk

#include "StructuredVelocityField-impl.h"

#endif // __GEOMTK_StructuredVelocityField__

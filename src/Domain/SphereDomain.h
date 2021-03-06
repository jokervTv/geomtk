#ifndef __GEOMTK_SphereDomain__
#define __GEOMTK_SphereDomain__

#include "Domain.h"
#include "SphereCoord.h"
#include "SphereVelocity.h"

namespace geomtk {

// TODO: SphereDomain may still need to be templated for considering
// CubicSphereCoord.
class SphereDomain : public Domain<SphereCoord> {
    double _radius;
public:
    enum ProjectionType {
        STEREOGRAPHIC
    };
    
    SphereDomain();
    SphereDomain(int numDim);
    SphereDomain(VertCoordType type);
    virtual ~SphereDomain();

    virtual void
    init(const string &filePath);

    double&
    radius() {
        return _radius;
    }

    double
    radius() const {
        return _radius;
    }

    virtual double
    calcDistance(const SphereCoord &x, const SphereCoord &y) const;

    virtual double
    calcDistance(const vec &x, const vec &y) const;

    double
    calcDistance(const SphereCoord &x, double lon, double lat) const;

    double
    calcDistance(const SphereCoord &x, double lon, double sinLat, double cosLat) const;

    virtual vec
    diffCoord(const SphereCoord &x, const SphereCoord &y) const;

    virtual vec
    diffCoord(const vec &x, const vec &y) const;

    /**
     *  Transform the given space coordinate 'xo' into a rotated spherical
     *  coordinate system with North Pole coordinate in original one as 'xp'.
     *  The output is put in 'xr'
     *
     *  @param xp the rotated North Pole coordinate.
     *  @param xo the original space coordinate.
     *  @param xr the transformed space coordinate.
     */
    void
    rotate(const SphereCoord &xp, const SphereCoord &xo, SphereCoord &xr) const;
    
    void
    rotate(const SphereCoord &xp, const SphereCoord &xo, double &lonR, double &latR) const;

    /**
     *  Transform the rotated coordinate back to original spherical coordinate
     *  system.
     *
     *  @param xp the rotated North Pole coordinate.
     *  @param xo the original space coordinate.
     *  @param xr the transformed space coordinate.
     */
    void
    rotateBack(const SphereCoord &xp, SphereCoord &xo, const SphereCoord &xr) const;
    
    void
    rotateBack(const SphereCoord &xp, SphereCoord &xo, double lonR, double latR) const;

    void
    project(ProjectionType projType, const SphereCoord &xp, const SphereCoord &xo, vec &xs) const;
    
    void
    projectBack(ProjectionType projType, const SphereCoord &xp, SphereCoord &xo, const vec &xs) const;

    /**
     *  Get a brief about the domain.
     *
     *  @return The brief.
     */
    virtual string
    brief() const;
}; // SphereDomain

} // geomtk

#endif // __GEOMTK_SphereDomain__

#ifndef __SphereDomain__
#define __SphereDomain__

#include "Domain.h"

enum Pole {
    SOUTH_POLE = 0, NORTH_POLE = 1, NOT_POLE = 2
};

class SphereCoord : public SpaceCoord {
protected:
public:
    SphereCoord(int numDim);
    virtual ~SphereCoord();
};

class SphereDomain : public Domain {
    double radius;
public:
    SphereDomain();
    SphereDomain(int numDim);
    virtual ~SphereDomain();

    double getRadius();
    void setRadius(double radius);

    virtual double calcDistance(const SpaceCoord &x, const SpaceCoord &y) const;
    virtual double calcDistance(const SpaceCoord &x, double lon,
                                double lat) const;
    virtual double calcDistance(const SpaceCoord &x, double lon,
                                double sinLat, double cosLat) const;
};

#endif

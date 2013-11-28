#ifndef __Domain__
#define __Domain__

#include "commons.h"

enum BndType {
    PERIODIC, OPEN, POLE, RIGID, INVALID
};

/**
 * @class SpaceCoord
 * @brief This class is used to describe the space coordinate of a point.
 */

class SpaceCoord {
protected:
    vec coords;
public:
    SpaceCoord(int numDim);
    virtual ~SpaceCoord();

    double operator()(int i) const;
    double& operator()(int i);
    vec& operator()();
};

class Domain {
protected:
    int numDim;
    vec axisStarts;
    vec axisEnds;
    vec axisSpans;
    BndType *bndTypeStarts;
    BndType *bndTypeEnds;

public:
    Domain();
    Domain(int numDim);
    virtual ~Domain();

    virtual int getNumDim() const;
    virtual void setAxis(int dim,
                         double start, BndType bndTypeStart,
                         double end, BndType bndTypeEnd);
    virtual double getAxisStart(int dim) const;
    virtual double getAxisEnd(int dim) const;
    virtual double getAxisSpan(int dim) const;
    virtual BndType getAxisStartBndType(int dim) const;
    virtual BndType getAxisEndBndType(int dim) const;
};

#endif

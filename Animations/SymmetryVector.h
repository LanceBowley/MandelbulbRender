//
// Created by lance on 5/2/17.
//

#ifndef RENDER_SYMMETRYVECTORS_H
#define RENDER_SYMMETRYVECTORS_H


#include "Filters/Filter1f.h"
#include "Filters/Filter3f.h"

class SymmetryVector {
public:

    SymmetryVector();
    bool enabled;
    Filter3f* orientation;
    Filter3f* gamma;
    Filter1f* distance;
    Filter1f* intensity;

    void setPosition(Filter3f* _position);
    cv::Vec3f getPosition(float cyclePosition);

private:

    Filter3f* position;

};


#endif //RENDER_SYMMETRYVECTORS_H

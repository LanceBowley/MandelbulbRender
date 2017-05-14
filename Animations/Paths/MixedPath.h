//
// Created by lance on 5/1/17.
//

#ifndef RENDER_MIXEDPATH_H
#define RENDER_MIXEDPATH_H


#include "../Filters/Filter1f.h"
#include "Path.h"

class MixedPath : public Path {
public:

    MixedPath(Path* _path1, Path* _path2, Filter1f* _biasFilter = new Filter1f("0.5"));

    cv::Vec3f getPosition(float cyclePosition);

private:

    Filter1f * biasFilter;
    Path * path1;
    Path * path2;

};


#endif //RENDER_MIXEDPATH_H

//
// Created by lance on 5/3/17.
//

#ifndef RENDER_PIECEWISEFILTER_H
#define RENDER_PIECEWISEFILTER_H

#include "Filter1f.h"

class PiecewiseFilter : public Filter1f {
public:

    PiecewiseFilter();

    void addFilter(Filter1f* filter, float startPosition);
    float val(float cyclePosition);

private:

    std::vector<Filter1f*>* filters;
    std::vector<float> positions;

};


#endif //RENDER_PIECEWISEFILTER_H

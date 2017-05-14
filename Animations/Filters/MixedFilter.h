//
// Created by lance on 5/1/17.
//

#ifndef RENDER_MIXEDFILTER_H
#define RENDER_MIXEDFILTER_H

#include "Filter1f.h"

class MixedFilter : public Filter1f {
public:

    MixedFilter(Filter1f* _filter1, Filter1f* _filter2, Filter1f* _biasFilter);

    float val(float cyclePosition);

private:

    Filter1f* filter1;
    Filter1f* filter2;
    Filter1f* biasFilter;
};


#endif //RENDER_MIXEDFILTER_H

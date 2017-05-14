//
// Created by lance on 5/1/17.
//

#include "MixedFilter.h"

MixedFilter::MixedFilter(Filter1f* _filter1, Filter1f* _filter2, Filter1f* _biasFilter)
{
    filter1 = _filter1;
    filter2 = _filter2;
    biasFilter = _biasFilter;
}

float MixedFilter::val(float cyclePosition)
{
    cyclePosition = getModifiedPosition(cyclePosition);
    float p1 = (1 - biasFilter->val(cyclePosition)) * filter1->val(cyclePosition);
    float p2 = biasFilter->val(cyclePosition) * filter2->val(cyclePosition);
    return getModifiedValue(p1 + p2);
}





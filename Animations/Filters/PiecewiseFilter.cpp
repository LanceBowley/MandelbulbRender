//
// Created by lance on 5/3/17.
//

#include "PiecewiseFilter.h"

PiecewiseFilter::PiecewiseFilter()
{
    filters = new std::vector<Filter1f*>;
}

void PiecewiseFilter::addFilter(Filter1f* filter, float startPosition)
{
    filters->push_back(filter);
    positions.push_back(startPosition);
}

float PiecewiseFilter::val(float cyclePosition)
{
    cyclePosition = getModifiedPosition(cyclePosition);
    unsigned long positionIndex = positions.size() - 1;
    for (unsigned long i = 0; i < positions.size(); i++) {
        if (cyclePosition < positions[i]) break;
        positionIndex = i;
    }
    return getModifiedValue(filters->at(positionIndex)->val(cyclePosition));
}
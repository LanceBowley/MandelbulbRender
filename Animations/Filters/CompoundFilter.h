//
// Created by lance on 5/1/17.
//

#ifndef RENDER_COMPOUNDFILTER1F_H
#define RENDER_COMPOUNDFILTER1F_H

#include "Filter1f.h"
#include "../Paths/Path.h"

class CompoundFilter : public Filter1f {
public:

    CompoundFilter();

    void addSubFilter(Filter1f * subFilter, float subPathCyclePosition, float repetitionsPerCycle = 1);

    float val(float cyclePosition) override;

private:

    std::vector<Filter1f *> * filters;
    std::vector<float> subPathCyclePositions;
    std::vector<float> subPathRepetitionsPerCycle;
};


#endif //RENDER_COMPOUNDFILTER1F_H

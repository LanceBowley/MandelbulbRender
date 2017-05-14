//
// Created by lance on 4/5/17.
//

#include "Path.h"

Path::Path()
{
    filteringEnabled = false;
    filter = new Filter1f("x");
}

float Path::modifyCyclePosition(float _cyclePosition)
{
    /// truncate
    if (_cyclePosition > 1) _cyclePosition = (float) (fabs(_cyclePosition) - fabs(floorf(_cyclePosition)));
    else if (_cyclePosition < 0) _cyclePosition = (float) 1 - modifyCyclePosition(-_cyclePosition);
    return filter->val(_cyclePosition);
}
//
// Created by lance on 5/1/17.
//

#include "CompoundFilter.h"

CompoundFilter::CompoundFilter()
{
    filters = new std::vector<Filter1f*>;
}

void CompoundFilter::addSubFilter(Filter1f * subFilter, float subPathCyclePosition, float repetitionsPerCycle)
{
    if (subPathCyclePosition >= 0 && subPathCyclePosition <= 1)
    {
        filters->push_back(subFilter);
        subPathCyclePositions.push_back(subPathCyclePosition);
        subPathRepetitionsPerCycle.push_back(repetitionsPerCycle);
    }
    else std::cout << "Path can only start between zero and one";
}

float CompoundFilter::val(float cyclePosition)
{
    cyclePosition = getModifiedPosition(cyclePosition);
    unsigned long nPositions = subPathCyclePositions.size();
    float subPathStart; // In path cycle
    float subPathEnd; // In path cycle
    float subPathSpan;
    float currentSpan;
    float repetitionsPerCycle;
    unsigned long subPathIndex = 0;
    bool subPathLoops;
    if (cyclePosition < subPathCyclePositions[0] || cyclePosition > subPathCyclePositions[nPositions - 1])
    {
        subPathIndex = nPositions - 1;
    }
    else
    {
        for (unsigned long i = 0; i < nPositions - 1; i++)
        {
            if (cyclePosition < subPathCyclePositions[i + 1])
            {
                subPathIndex = i;
                break;
            }
        }
    }

    repetitionsPerCycle = subPathRepetitionsPerCycle[subPathIndex];
    subPathStart = subPathCyclePositions[subPathIndex];
    subPathEnd = subPathCyclePositions[(subPathIndex == nPositions - 1) ? 0 : subPathIndex + 1]; // Looped start position or next position
    subPathLoops = (subPathEnd <= subPathStart);
    subPathSpan = (subPathLoops) ? 1 - subPathStart + subPathEnd : subPathEnd - subPathStart;
    currentSpan = (subPathLoops && cyclePosition < subPathStart) ? 1 - subPathStart + cyclePosition : cyclePosition - subPathStart;
    return getModifiedValue(filters->at(subPathIndex)->val((currentSpan / subPathSpan) * repetitionsPerCycle));
}
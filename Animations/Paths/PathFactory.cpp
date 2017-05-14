//
// Created by lance on 5/1/17.
//

#include "PathFactory.h"
#include "../Filters/MixedFilter.h"
#include "../Plotter.h"
#include "../Filters/CompoundFilter.h"

PathFactory::PathFactory()
{
    pathAnalyzer = new PathAnalyzer;
}


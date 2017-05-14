//
// Created by lance on 5/1/17.
//

#ifndef RENDER_FILTER3F_H
#define RENDER_FILTER3F_H

#include "Filter1f.h"

class Filter3f {
public:

    Filter3f(float valX, float valY, float valZ);
    Filter3f(std::string valX, std::string valY, std::string valZ);

    Filter1f* operator [] (int i) { // Getter
        switch (i)
        {
            case 0:
                return xFilter;
            case 1:
                return yFilter;
            case 2:
                return zFilter;
            default:
                std::cout << "Invalid index, returning filter with static value of 1\n";
                return new Filter1f("1");
        }
    }

    void setTruncation(float _lowerBound, float _upperBound);
    cv::Vec3f val(float cyclePosition);
    cv::Vec3f valNormalized(float cyclePosition);

private:

    Filter1f * xFilter;
    Filter1f * yFilter;
    Filter1f * zFilter;
};


#endif //RENDER_FILTER3F_H

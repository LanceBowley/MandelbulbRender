//
// Created by lance on 5/1/17.
//

#ifndef RENDER_FILTER2F_H
#define RENDER_FILTER2F_H


#include "Filter1f.h"

class Filter2f {
public:

    Filter2f(float valX, float valY);
    Filter2f(std::string valX, std::string valY);

    Filter1f* operator [] (int i) {
        switch (i)
        {
            case 0:
                return xFilter;
            case 1:
                return yFilter;
            default:
                std::cout << "Invalid index, returning filter with static value of 1\n";
                return new Filter1f("1");
        }
    }

    void setTruncation(float _lowerBound, float _upperBound);

    cv::Vec2f val(float cyclePosition);

private:

    Filter1f * xFilter;
    Filter1f * yFilter;
};


#endif //RENDER_FILTER2F_H

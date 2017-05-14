//
// Created by lance on 5/1/17.
//

#ifndef RENDER_FILTERNEW_H
#define RENDER_FILTERNEW_H

#include <opencv2/core/matx.hpp>
#include "muParser.h"
#include <math.h>

class Filter1f {
public:

    Filter1f();
    Filter1f(float val);
    Filter1f(std::string expression);

    bool isMirrored;
    float mirroredPosition;

    void setExpression(float val);
    void setExpression(std::string expression);
    void setLowerBound(float _lowerBound);
    void setUpperBound(float _upperBound);
    void setTruncation(float _lowerBound, float _upperBound);

    float getModifiedPosition(float position);
    float getModifiedValue(float value);

    cv::Vec2f getBounds();
    virtual float val(float position = 0);

private:

    mu::Parser * p;
    double x;
    float staticVal;
    float lowerBound;
    float upperBound;
    bool parserInitialized;
    bool isStatic;

    void initializeParser();
    static double sigmoid(double ramp, double center, double x);
    bool isANumber(std::string s);

};


#endif //RENDER_FILTERNEW_H

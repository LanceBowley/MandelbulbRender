//
// Created by lance on 5/1/17.
//

#include "Filter1f.h"

Filter1f::Filter1f()
{
    parserInitialized = false;
    setExpression(1);
    setTruncation(-INFINITY, INFINITY);
    isMirrored = false;
    mirroredPosition = 0.5f;
}

Filter1f::Filter1f(float val)
{
    parserInitialized = false;
    setExpression(val);
    setTruncation(-INFINITY, INFINITY);
    isMirrored = false;
    mirroredPosition = 0.5f;
}

Filter1f::Filter1f(std::string expression)
{
    parserInitialized = false;
    if (!isANumber(expression))
    {
        initializeParser();
        parserInitialized = true;
    }
    setExpression(expression);
    setTruncation(-INFINITY, INFINITY);
    isMirrored = false;
    mirroredPosition = 0.5f;
}

void Filter1f::initializeParser()
{
    p = new mu::Parser;
    p->DefineVar("x", &x);
    p->DefineFun("sigmoid", sigmoid);
}

void Filter1f::setExpression(float val)
{
    staticVal = val;
    isStatic = true;
}

void Filter1f::setExpression(std::string expression)
{
    if (isANumber(expression))
    {
        setExpression(stof(expression));
    } else {
        if (!parserInitialized) initializeParser();
        p->SetExpr(expression);
        isStatic = false;
    }
}

void Filter1f::setLowerBound(float _lowerBound)
{
    lowerBound = _lowerBound;
}

void Filter1f::setUpperBound(float _upperBound)
{
    upperBound = _upperBound;
}

void Filter1f::setTruncation(float _lowerBound, float _upperBound)
{
    setLowerBound(_lowerBound);
    setUpperBound(_upperBound);
}

cv::Vec2f Filter1f::getBounds()
{
    return cv::Vec2f(lowerBound, upperBound);
}

float Filter1f::val(float position)
{
    position = getModifiedPosition(position);
    float val;
    if (!isStatic)
    {
        x = (double) position;
        val = (float) p->Eval();
    } else val = staticVal;
    return getModifiedValue(val);
}

float Filter1f::getModifiedPosition(float position)
{
    if (isMirrored) {
        if (position < mirroredPosition) return position;
        else {
            return 2 * mirroredPosition - position;
        }
    }
    return position;
}

float Filter1f::getModifiedValue(float value)
{
    if (value > upperBound)
    {
        float span = upperBound - lowerBound;
        float stepsUp = floorf((value - lowerBound) / span);
        value -= stepsUp * span;
    }
    else if (value < lowerBound)
    {
        float span = upperBound - lowerBound;
        float stepsDown = floorf(-(lowerBound - value) / span);
        value += stepsDown * span;
        value = upperBound + value;
    }
    return value;
}

double Filter1f::sigmoid(double ramp, double center, double x)
{
    return 1 / (1 + (pow( M_E, - 1.0f * (center - 0.5f + x) * ramp)));
}

bool Filter1f::isANumber(std::string s)
{
    int decimalPoints = 0;
    for(int i = 0; i < s.size(); i++)
    {
        if (s[i] == 46)
        {
            decimalPoints++;
            if (decimalPoints > 1) return false;
        }
        else if (s[i] < 48 || s[i] > 57) return false;
    }
    return true;
}
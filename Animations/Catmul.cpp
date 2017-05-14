//
// Created by lance on 4/10/17.
//

#include <iostream>
#include "Catmul.h"

Catmul::Catmul()
{
    setCurveFactor(0.0f);
    enableDistanceNormalization(true);
    initialized = false;
    loops = false;
}

Catmul::Catmul(std::vector<cv::Vec3f> _points, float _alpha)
{
    enableDistanceNormalization(true);
    setCurveFactor(_alpha);
    loops = false;
    setPoints(_points);
}

void Catmul::setCurveFactor(float _alpha)
{
    alpha = _alpha;
}

void Catmul::splineLoops(bool _loops)
{
    loops = _loops;
    setPoints(points);
}

void Catmul::setPoints(std::vector<cv::Vec3f> _points)
{
    initialized = (_points.size() != 0);
    points = _points;
    nPoints = (int) points.size();
    totalPathDistance = 0;
    float d;
    for (int i = 0; i < nPoints - 1; i++)
    {
        d = getLinearDistance(points[i], points[i + 1]);
        pathDistances.push_back(d);
        totalPathDistance += d;
    }
    if (loops)
    {
        d = getLinearDistance(points[nPoints - 1], points[0]);
        pathDistances.push_back(d);
        totalPathDistance += d;
    }
}

cv::Vec3f Catmul::getPoint(float cyclePosition)
{
    if (initialized)
    {
        float cyclesPerPoint = 1 / (float) nPoints;
        unsigned long previousIndex, currentIndex, nextIndex, leadingIndex;
        float subCyclePosition;
        if (distanceNormalization)
        {
            currentIndex = 0; // JANK, but it works so whatever
            float normalizedDistance = 0;
            for (unsigned long i = 0; i < nPoints; i++)
            {
                if (normalizedDistance <= cyclePosition) currentIndex = i;
                else
                {
                    normalizedDistance -= pathDistances[currentIndex] / totalPathDistance;
                    break;
                };
                normalizedDistance += pathDistances[i] / totalPathDistance;
                if (i == nPoints - 1) normalizedDistance -= pathDistances[currentIndex] / totalPathDistance;
            }
            subCyclePosition = (cyclePosition - normalizedDistance) / (pathDistances[currentIndex] / totalPathDistance);
        }
        else
        {
            currentIndex = (unsigned long) (cyclePosition / cyclesPerPoint);
            subCyclePosition = (cyclePosition - cyclesPerPoint * currentIndex) / cyclesPerPoint;
        }
        previousIndex = (currentIndex == 0) ? (unsigned long) nPoints - 1 : currentIndex - 1;
        nextIndex = (currentIndex == nPoints - 1) ? 0 : currentIndex + 1;
        leadingIndex = (nextIndex == nPoints - 1) ? 0 : nextIndex + 1;

        cv::Vec3f p0 = points[previousIndex];
        cv::Vec3f p1 = points[currentIndex];
        cv::Vec3f p2 = points[nextIndex];
        cv::Vec3f p3 = points[leadingIndex];

        float t0 = 0.0f;
        float t1 = getT(t0, p0, p1);
        float t2 = getT(t1, p1, p2);
        float t3 = getT(t2, p2, p3);

        float t = t1 + (t2 - t1) * subCyclePosition;

        cv::Vec3f A1 = (t1-t)/(t1-t0)*p0 + (t-t0)/(t1-t0)*p1;
        cv::Vec3f A2 = (t2-t)/(t2-t1)*p1 + (t-t1)/(t2-t1)*p2;
        cv::Vec3f A3 = (t3-t)/(t3-t2)*p2 + (t-t2)/(t3-t2)*p3;
        cv::Vec3f B1 = (t2-t)/(t2-t0)*A1 + (t-t0)/(t2-t0)*A2;
        cv::Vec3f B2 = (t3-t)/(t3-t1)*A2 + (t-t1)/(t3-t1)*A3;
        cv::Vec3f C = (t2-t)/(t2-t1)*B1 + (t-t1)/(t2-t1)*B2;

        return C;
    }
    std::cout << "Spline points must be initialized before they can be interpolated\n";
    return cv::Vec3f(0, 0, 0);
}

float Catmul::getT(float t, cv::Vec3f p0, cv::Vec3f p1)
{
    float a = powf((p1.val[0] - p0.val[0]), 2.0f) + powf((p1.val[1] - p0.val[1]), 2.0f) + powf((p1.val[2] - p0.val[2]), 2.0f);
    float b = powf(a, 0.5f);
    float c = powf(b, alpha);

    return (c + t);
}

void Catmul::enableDistanceNormalization(bool enable)
{
    distanceNormalization = enable;
}

float Catmul::getLinearDistance(cv::Vec3f v0, cv::Vec3f v1)
{
    float sumSq = 0;
    for (unsigned long i = 0; i < 3; i++)
    {
        sumSq += powf(v1.val[i] - v0.val[i], 2);
    }
    return sqrtf(sumSq);
}

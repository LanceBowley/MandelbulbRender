//
// Created by lance on 4/10/17.
//

#ifndef RENDER_CALLUMROMSPLINE_H
#define RENDER_CALLUMROMSPLINE_H


#include <opencv2/core/matx.hpp>

class Catmul {
public:

    Catmul();
    Catmul(std::vector<cv::Vec3f> _points, float _alpha = 0.0f);

    void setPoints(std::vector<cv::Vec3f> _points);
    void setCurveFactor(float _alpha);
    void splineLoops(bool _loops);

    cv::Vec3f getPoint(float cyclePosition);
    void enableDistanceNormalization(bool enable);

private:

    float alpha;
    int nPoints;
    bool distanceNormalization;
    float totalPathDistance;
    std::vector<cv::Vec3f> points;
    std::vector<float> pathDistances;
    bool initialized;
    bool loops;

    float getT(float t, cv::Vec3f p0, cv::Vec3f p1);
    float getLinearDistance(cv::Vec3f v0, cv::Vec3f v1);
};


#endif //RENDER_CALLUMROMSPLINE_H

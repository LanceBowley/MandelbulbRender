//
// Created by lance on 5/1/17.
//

#include "PathAnalyzer.h"

PathAnalyzer::PathAnalyzer()
{
    error = 0.001f;
}

float PathAnalyzer::getVelocity(Filter1f * filter, float cyclePosition1, float cyclePosition2, float nFrames)
{
    float p0 = filter->val(cyclePosition1);
    float p1 = filter->val(cyclePosition2);

    float disp = p1 - p0;
    disp = disp / (nFrames);
    return disp;
}

cv::Vec3f PathAnalyzer::getVelocity(Path *path, float cyclePosition1, float cyclePosition2, float nFrames)
{
    cv::Vec3f p0 = path->getPosition(cyclePosition1);
    cv::Vec3f p1 = path->getPosition(cyclePosition2);
    cv::Vec3f disp = p1 - p0;
    // std::cout << std::to_string(p0[0]) + " " + std::to_string(p0[1]) + " " + std::to_string(p0[2]) << "\n";
    // std::cout << std::to_string(p1[0]) + " " + std::to_string(p1[1]) + " " + std::to_string(p1[2]) << "\n";
    cv::Vec3f velocity = disp / nFrames;
    return velocity;
}

float PathAnalyzer::getVelocityMagnitude(Path *path, float cyclePosition1, float cyclePosition2, float nFrames)
{
    cv::Vec3f disp = getVelocity(path, cyclePosition1, cyclePosition2, nFrames);
    return sqrtf(powf(disp[0], 2) + powf(disp[1], 2) + powf(disp[2], 2));
}

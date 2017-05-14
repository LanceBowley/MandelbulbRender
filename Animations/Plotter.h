//
// Created by lance on 4/8/17.
//

#ifndef RENDER_PLOTTER_H
#define RENDER_PLOTTER_H

#include "../Util.h"
#include <opencv2/core/matx.hpp>
#include "Color.h"
#include "Light.h"

class Plotter {
public:

    static void plot2d(std::vector<cv::Vec2f> points, std::vector<cv::Vec2f> domain);
    static void plotFilter(Filter1f * filter, int nPoints);

    static void plot3d(std::vector<cv::Vec3f> points);
    static void plot3d(std::vector<cv::Vec3f> points, cv::Vec2f domain);
    static void plot3d(std::vector<cv::Vec3f> points, cv::Vec2f domain, std::vector<cv::Vec3f> colors);
    static void plot3d(std::vector<cv::Vec3f> points, std::vector<cv::Vec2f> domain);

    static void plot3d(std::vector<std::vector<cv::Vec3f>> points);
    static void plot3d(std::vector<std::vector<cv::Vec3f>> points, cv::Vec2f domain);
    static void plot3d(std::vector<std::vector<cv::Vec3f>> points, cv::Vec2f domain, std::vector<cv::Vec3f> colors);
    static void plot3d(std::vector<std::vector<cv::Vec3f>> points, std::vector<cv::Vec2f> domain);
    static void plot3d(std::vector<std::vector<cv::Vec3f>> pointGroups, std::vector<cv::Vec2f> domain, std::vector<cv::Vec3f> colors, std::vector<std::string> axisLabels = {"X", "Y", "Z"});

    static void plotPath(Path * path, int nPoints);
    static void plotPath(Path * path, int nPoints, cv::Vec2f domain);
    static void plotPath(Path * path, int nPoints, std::vector<cv::Vec2f> domain);
    static void plotPaths(std::vector<Path *> paths, int nPoints);

    static void plotColor(Color * color, int nPoints);
    static void plotColor(Color * color, int nPoints, cv::Vec2f domain);
    static void plotColor(Color * color, int nPoints, std::vector<cv::Vec2f> domain);

    static void plotLight(Light * light, int nPoints);

private:

    static std::vector<cv::Vec2f> getFilterPoints(Filter1f * filter, int nPoints);
    static std::vector<cv::Vec3f> getPathPoints(Path * path, int nPoints);
    static std::vector<cv::Vec3f> getPalette(Color * color, int nPoints);
    static std::vector<cv::Vec2f> getDomain(std::vector<cv::Vec3f> points);
    static std::vector<cv::Vec2f> getDomain(std::vector<std::vector<cv::Vec3f>> pointGroups);
    static cv::Vec2f getValueBounds(std::vector<cv::Vec2f> points);
};

#endif
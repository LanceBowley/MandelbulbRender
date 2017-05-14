//
// Created by lance on 4/8/17.
//

#include "Plotter.h"
#include "Paths/BasicPath.h"

void Plotter::plot2d(std::vector<cv::Vec2f> points, std::vector<cv::Vec2f> domain)
{
    clearFile("python /home/lance/RemoteDesktop/Python/test.py");
    Color colorization(new BasicPath("0", "255 - 255 * x", "255 * x"));
    std::string hexColor;
    clearFile("/home/lance/RemoteDesktop/Python/test.py");
    std::fstream file;
    file.open("/home/lance/RemoteDesktop/Python/test.py");
    file << "from mpl_toolkits.mplot3d import Axes3D\n";
    file << "import matplotlib.pyplot as plt\n";
    file << "import numpy as np\n";
    char domainCmd[100];
    sprintf(domainCmd, "plt.xlim(%f, %f)\n", domain[0][0], domain[0][1]);
    file << domainCmd;
    sprintf(domainCmd, "plt.ylim(%f, %f)\n", domain[1][0], domain[1][1]);
    file << domainCmd;
    char scatterCmd[100];
    unsigned long nPoints = points.size();
    for (unsigned long i = 0; i < nPoints; i++)
    {
        hexColor = "#" + colorization.rgbToHex(colorization.getColor((float) i / (float) nPoints));
        sprintf(scatterCmd, "plt.scatter(%f, %f, color='%s', marker='^')\n", points[i][0], points[i][1], hexColor.c_str());
        file << scatterCmd;
    }
    file << "plt.show()\n";
    file.close();

    sysExec("python /home/lance/RemoteDesktop/Python/test.py");
}

void Plotter::plotFilter(Filter1f * filter, int nPoints)
{
    std::vector<cv::Vec2f> points = getFilterPoints(filter, nPoints);
    cv::Vec2f bounds = getValueBounds(points);
    std::vector<cv::Vec2f> domain = {cv::Vec2f(0, 1), cv::Vec2f(bounds.val[0], bounds.val[1])};
    plot2d(points, domain);
}

void Plotter::plot3d(std::vector<cv::Vec3f> points)
{
    std::vector<cv::Vec2f> domain = getDomain(points);
    plot3d(points, domain);
}

void Plotter::plot3d(std::vector<cv::Vec3f> points, cv::Vec2f domain)
{
    plot3d(points, {domain, domain, domain});
}

void Plotter::plot3d(std::vector<cv::Vec3f> points, cv::Vec2f domain, std::vector<cv::Vec3f> colors)
{
    plot3d({points}, {domain, domain, domain}, colors);
}

void Plotter::plot3d(std::vector<cv::Vec3f> points, std::vector<cv::Vec2f> domain)
{
    std::vector<cv::Vec3f> null = {cv::Vec3f(-1, 0, 0)};
    plot3d({points}, domain, null);
}

void Plotter::plot3d(std::vector<std::vector<cv::Vec3f>> points)
{
    std::vector<cv::Vec2f> domain = getDomain(points);
    plot3d(points, domain);
}

void Plotter::plot3d(std::vector<std::vector<cv::Vec3f>> points, cv::Vec2f domain)
{
    plot3d(points, {domain, domain, domain});
}

void Plotter::plot3d(std::vector<std::vector<cv::Vec3f>> points, cv::Vec2f domain, std::vector<cv::Vec3f> colors)
{
    plot3d(points, {domain, domain, domain}, colors);
}
void Plotter::plot3d(std::vector<std::vector<cv::Vec3f>> points, std::vector<cv::Vec2f> domain)
{
    std::vector<cv::Vec3f> null = {cv::Vec3f(-1, 0, 0)};
    plot3d(points, domain, null);
}

void Plotter::plot3d(std::vector<std::vector<cv::Vec3f>> pointGroups, std::vector<cv::Vec2f> domain, std::vector<cv::Vec3f> colors, std::vector<std::string> axisLabels)
{
    clearFile("python /home/lance/RemoteDesktop/Python/test.py");
    unsigned long nPoints = pointGroups[0].size();
    unsigned long nColors = colors.size();
    bool customColors = false;
    if (colors[0][0] >= -0.5f)
    {
        if (nColors == nPoints) customColors = true;
        else std::cout << "Colors must be one to one with points\n";
    }
    Color colorization;
    if (!customColors)
    {
        colorization = new BasicPath("0", "255 - 255 * x", "255 * x");
    }
    std::string hexColor;
    clearFile("/home/lance/RemoteDesktop/Python/test.py");
    std::fstream file;
    file.open("/home/lance/RemoteDesktop/Python/test.py");
    file << "from mpl_toolkits.mplot3d import Axes3D\n";
    file << "import matplotlib.pyplot as plt\n";
    file << "import numpy as np\n";
    file << "fig = plt.figure()\n";
    file << "ax = fig.gca(projection='3d')\n";
    file << "ax.set_aspect('equal')\n";
    char labelCmd[100];
    sprintf(labelCmd, "ax.set_xlabel('%s')\n", axisLabels[0].c_str());
    file << labelCmd;
    sprintf(labelCmd, "ax.set_ylabel('%s')\n", axisLabels[1].c_str());
    file << labelCmd;
    sprintf(labelCmd, "ax.set_zlabel('%s')\n", axisLabels[2].c_str());
    file << labelCmd;
    char domainCmd[100];
    sprintf(domainCmd, "ax.set_xlim(%f, %f)\n", domain[0][0], domain[0][1]);
    file << domainCmd;
    sprintf(domainCmd, "ax.set_ylim(%f, %f)\n", domain[1][0], domain[1][1]);
    file << domainCmd;
    sprintf(domainCmd, "ax.set_zlim(%f, %f)\n", domain[2][0], domain[2][1]);
    file << domainCmd;
    char scatterCmd[100];
    std::vector<cv::Vec3f> points;
    for (unsigned long i = 0; i < pointGroups.size(); i++)
    {
        points = pointGroups[i];
        for (unsigned long j = 0; j < nPoints; j++)
        {
            if (customColors) hexColor = "#" + colorization.rgbToHex(colors[j]);
            else hexColor = "#" + colorization.rgbToHex(colorization.getColor((float) j / (float) nPoints));
            sprintf(scatterCmd, "ax.scatter(%f, %f, %f, color='%s', marker='^')\n", points[j][0], points[j][1], points[j][2], hexColor.c_str());
            file << scatterCmd;
        }
    }
    file << "plt.show()\n";
    file.close();

    sysExec("python /home/lance/RemoteDesktop/Python/test.py");
}

// No domain given
void Plotter::plotPath(Path * path, int nPoints)
{
    std::vector<cv::Vec3f> points = getPathPoints(path, nPoints);
    std::vector<cv::Vec2f> domain = getDomain(points);
    plot3d(points, domain);
}

void Plotter::plotPath(Path * path, int nPoints, cv::Vec2f domain)
{
    plotPath(path, nPoints, {domain, domain, domain});
}

void Plotter::plotPath(Path * path, int nPoints, std::vector<cv::Vec2f> domain)
{
    std::vector<cv::Vec3f> points = getPathPoints(path, nPoints);
    plot3d(points, domain);
}

void Plotter::plotPaths(std::vector<Path *> paths, int nPoints)
{
    std::vector<std::vector<cv::Vec3f>> points;
    for (unsigned long i = 0; i < paths.size(); i++)
    {
        points.push_back(getPathPoints(paths[i], nPoints));
    }
    std::vector<cv::Vec2f> domain = getDomain(points);
    plot3d(points, domain);
}

void Plotter::plotColor(Color * color, int nPoints)
{
    std::vector<cv::Vec3f> colors = getPalette(color, nPoints);
    plot3d({colors}, {cv::Vec2f(0, 255), cv::Vec2f(0, 255), cv::Vec2f(0, 255)}, colors, {"B", "G", "R"});
}

void Plotter::plotColor(Color * color, int nPoints, cv::Vec2f domain)
{
    plotColor(color, nPoints, {domain, domain, domain});
}

void Plotter::plotColor(Color * color, int nPoints, std::vector<cv::Vec2f> domain)
{
    std::vector<cv::Vec3f> colors = getPalette(color, nPoints);
    plot3d({colors}, domain, colors, {"B", "G", "R"});
}

void Plotter::plotLight(Light * light, int nPoints)
{
    std::vector<cv::Vec3f> colors = getPalette(light->colorization, nPoints);
    std::vector<cv::Vec3f> points = getPathPoints(light->path, nPoints);
    std::vector<cv::Vec2f> domain = getDomain(points);
    plot3d({points}, domain, colors);
}

std::vector<cv::Vec2f> Plotter::getFilterPoints(Filter1f * filter, int nPoints)
{
    std::vector<cv::Vec2f> points;
    float cyclePosition;
    for (int i = 0; i < nPoints; i++)
    {
        cyclePosition = (float) i / nPoints;
        cv::Vec2f point = cv::Vec2f(cyclePosition, filter->val(cyclePosition));
        points.push_back(point);
    }
    return points;
}

std::vector<cv::Vec3f> Plotter::getPathPoints(Path * path, int nPoints)
{
    std::vector<cv::Vec3f> points;
    for (int i = 0; i < nPoints; i++)
    {
        points.push_back(path->getPosition((float) i / nPoints));
    }
    return points;
}

std::vector<cv::Vec3f> Plotter::getPalette(Color * color, int iterations)
{
    std::vector<cv::Vec3f> palette;
    for (int i = 0; i < iterations; i++) {
        palette.push_back(color->getColor((float) i / iterations, Color::Space::BGR));
    }
    return palette;
}

std::vector<cv::Vec2f> Plotter::getDomain(std::vector<cv::Vec3f> points)
{
    std::vector<std::vector<cv::Vec3f>> pointGroups = {points};
    return getDomain(pointGroups);
}

std::vector<cv::Vec2f> Plotter::getDomain(std::vector<std::vector<cv::Vec3f>> pointGroups)
{
    float xMin = pointGroups[0][0].val[0];
    float xMax = pointGroups[0][0].val[0];
    float yMin = pointGroups[0][0].val[1];
    float yMax = pointGroups[0][0].val[1];
    float zMin = pointGroups[0][0].val[2];
    float zMax = pointGroups[0][0].val[2];

    std::vector<cv::Vec3f> points;
    for (unsigned long i = 0; i < pointGroups.size(); i++)
    {
        points = pointGroups[i];
        for (unsigned long j = 1; j < points.size(); j++)
        {
            if (points[j].val[0] < xMin) xMin = points[j].val[0];
            else if (points[j].val[0] > xMax) xMax = points[j].val[0];
            if (points[j].val[1] < yMin) yMin = points[j].val[1];
            else if (points[j].val[1] > yMax) yMax = points[j].val[1];
            if (points[j].val[2] < zMin) zMin = points[j].val[2];
            else if (points[j].val[2] > zMax) zMax = points[j].val[2];
        }
    }
    float xR = xMax - xMin;
    float yR = yMax - yMin;
    float zR = zMax - zMin;
    float xC = (xMax + xMin) / 2;
    float yC = (yMax + yMin) / 2;
    float zC = (zMax + zMin) / 2;
    float r = std::max(xR, std::max(yR, zR));
    float s = 0.5f * r;

    std::vector<cv::Vec2f> domain = {cv::Vec2f(xC - s, xC + s), cv::Vec2f(yC - s, yC + s), cv::Vec2f(zC - s, zC + s)};
    return domain;
}

cv::Vec2f Plotter::getValueBounds(std::vector<cv::Vec2f> points)
{
    float valMin = points[0].val[1];
    float valMax = valMin;

    for (unsigned long i = 1; i < points.size(); i++)
    {
        if (points[i][1] < valMin) valMin = points[i][1];
        else if (points[i][1] > valMax) valMax = points[i][1];
    }
    return cv::Vec2f(valMin, valMax);

}

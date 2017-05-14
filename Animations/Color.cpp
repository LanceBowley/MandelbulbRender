//
// Created by lance % on 4/7/17.
//

#include "Color.h"

Color::Color()
{
    setPathColorSpace(BGR);
}

Color::Color(Path * _path)
{
    setPathColorSpace(BGR);
    path = _path;
}

void Color::setPathColorSpace(Space _colorSpace)
{
    pathColorSpace = _colorSpace;
}

cv::Vec3f Color::getColor(float cyclePosition, Space _colorSpace)
{
    cv::Vec3f pathColor = cropColor(path->getPosition(cyclePosition), pathColorSpace); // TODO: investigate, originally cropping the target color space
    if (_colorSpace != pathColorSpace)
    {
        pathColor = changeColorSpace(pathColor, pathColorSpace, _colorSpace);
    }
    return pathColor;
}

// Functions pulled from "https://tinyurl.com/lq6bcpa"
cv::Vec3f Color::changeColorSpace(cv::Vec3f color, Space intialColorSpace, Space finalColorSpace)
{
    if (intialColorSpace != finalColorSpace)
    {
        if (intialColorSpace == BGR)
        {
            float fR = color[2] / 255;
            float fG = color[1] / 255;
            float fB = color[0] / 255;

            float fH, fS, fV;

            float fCMax = std::max(std::max(fR, fG), fB);
            float fCMin = std::min(std::min(fR, fG), fB);
            float fDelta = fCMax - fCMin;

            if(fDelta > 0) {
                if(fCMax == fR) {
                    fH = 60 * (fmod(((fG - fB) / fDelta), 6));
                } else if(fCMax == fG) {
                    fH = 60 * (((fB - fR) / fDelta) + 2);
                } else if(fCMax == fB) {
                    fH = 60 * (((fR - fG) / fDelta) + 4);
                }

                if(fCMax > 0) {
                    fS = fDelta / fCMax;
                } else {
                    fS = 0;
                }

                fV = fCMax;
            } else {
                fH = 0;
                fS = 0;
                fV = fCMax;
            }

            if(fH < 0) {
                fH = 360 + fH;
            }
            return cv::Vec3f(fH, fS, fV);
        }
        if (intialColorSpace == HSV)
        {
            float fH = color[0];
            float fS = color[1];
            float fV = color[2];

            float fR, fG, fB;

            float fC = fV * fS; // Chroma
            float fHPrime = fmod(fH / 60.0, 6);
            float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
            float fM = fV - fC;

            if(0 <= fHPrime && fHPrime < 1) {
                fR = fC;
                fG = fX;
                fB = 0;
            } else if(1 <= fHPrime && fHPrime < 2) {
                fR = fX;
                fG = fC;
                fB = 0;
            } else if(2 <= fHPrime && fHPrime < 3) {
                fR = 0;
                fG = fC;
                fB = fX;
            } else if(3 <= fHPrime && fHPrime < 4) {
                fR = 0;
                fG = fX;
                fB = fC;
            } else if(4 <= fHPrime && fHPrime < 5) {
                fR = fX;
                fG = 0;
                fB = fC;
            } else if(5 <= fHPrime && fHPrime < 6) {
                fR = fC;
                fG = 0;
                fB = fX;
            } else {
                fR = 0;
                fG = 0;
                fB = 0;
            }

            fR += fM;
            fG += fM;
            fB += fM;

            return cv::Vec3f(fB * 255, fG * 255, fR * 255);
        }
    }
    else std::cout << "No colorspace to convert to, returning color\n";
    return color;
}


cv::Vec3f Color::cropColor(cv::Vec3f color, Space _colorSpace)
{
    float val;
    if (_colorSpace == BGR)
    {
        for (unsigned long i = 0; i < 3; i++)
        {
            val = color[i];
            if (val < 0) color[i] = 0;
            if (val > 255) color[i] = 255;
        }
    }
    if (_colorSpace == HSV)
    {
        if (color[0] > 360) color[0] = 360;
        else if (color[0] < 0) color[0] = 0;

        if (color[1] > 1) color[1] = 1;
        else if (color[1] < 0) color[1] = 0;

        if (color[2] > 1) color[2] = 1;
        else if (color[2] < 0) color[2] = 0;
    }

    return color;
}

// Pulled from "https://tinyurl.com/ldw2sd2"
std::string Color::rgbToHex(cv::Vec3f color)
{
    std::string result;
    char r[255];
    sprintf(r, "%.2X", (int) color.val[2]);
    result.append(r);
    char g[255];
    sprintf(g, "%.2X", (int) color.val[1]);
    result.append(g);
    char b[255];
    sprintf(b, "%.2X", (int) color.val[0]);
    result.append(b);
    return result;
}
//
// Created by lance on 4/5/17.
//

#ifndef RENDER_LIGHT_H
#define RENDER_LIGHT_H

#include <iostream>
#include "../Util.h"
#include "Paths/Path.h"
#include "Color.h"
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "Filters/Filter1f.h"
#include "Paths/BasicPath.h"

class Light {
public:


    Path * path;
    Color * colorization;
    Filter1f * intensity;
    Filter1f * volumetricIntensity;

    Light();
    Light(Path * _path, Color * _colorization, Filter1f * _intensity, Filter1f * _volumtericIntensity);

    void isEnabled(bool _enabled);
    void isVolumetricLightEnabled(bool _enabled);
    void setIntensity(float _intensity);
    void setVolumetricIntensity(float _intensity);

    bool isEnabled();
    bool isVolumetricLightEnabled();
    cv::Vec3f getPosition(float cyclePosition);
    cv::Vec3f getColorization(float cyclePosition);
    float getIntensity(float cyclePosition);
    float getVolumetricIntensity(float cyclePosition);


private:

    bool enabled;
    bool volumetricLightEnabled;
};


#endif //RENDER_LIGHT_H

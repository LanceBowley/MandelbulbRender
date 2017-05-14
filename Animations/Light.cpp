//
// Created by lance on 4/5/17.
//


#include "Light.h"


Light::Light()
{
    isEnabled(true);
    isVolumetricLightEnabled(true);
    path = new BasicPath(0, 0, 0); // Static at origin
    colorization = new Color; // Static and white
    intensity = new Filter1f("1"); // Constant intensity of 1
    intensity->setTruncation(0, INFINITY);
    volumetricIntensity = new Filter1f("0.1");
    volumetricIntensity->setTruncation(0, INFINITY);
}

Light::Light(Path * _path, Color * _colorization, Filter1f * _intensity, Filter1f * _volumtericIntensity)
{
    isEnabled(true);
    isVolumetricLightEnabled(true);
    path = _path;
    colorization = _colorization;
    intensity = _intensity;
    volumetricIntensity = _volumtericIntensity;
}


void Light::isEnabled(bool _enabled)
{
    enabled = _enabled;
}

void Light::isVolumetricLightEnabled(bool enabled)
{
    volumetricLightEnabled = enabled;
}

void Light::setIntensity(float _intensity)
{
    intensity = new Filter1f(std::to_string(_intensity));
}

void Light::setVolumetricIntensity(float _intensity)
{
    volumetricIntensity = new Filter1f(std::to_string(_intensity));
}

bool Light::isEnabled()
{
    return enabled;
}

bool Light::isVolumetricLightEnabled()
{
    return volumetricLightEnabled;
}

cv::Vec3f Light::getPosition(float cyclePosition)
{
    return path->getPosition(cyclePosition);
}

cv::Vec3f Light::getColorization(float cyclePosition)
{
    return colorization->getColor(cyclePosition);
}

float Light::getIntensity(float cyclePosition)
{
    return intensity->val(cyclePosition);
}

float Light::getVolumetricIntensity(float cyclePosition)
{
    return volumetricIntensity->val(cyclePosition);
}



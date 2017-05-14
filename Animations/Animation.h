//
// Created by lance on 3/16/17.
//

#ifndef RENDER_ANIMATION_H
#define RENDER_ANIMATION_H

#include <string>
#include "../Util.h"
#include "FractParam.h"
#include "Light.h"
#include "Camera.h"
#include "SymmetryVector.h"
#include "IFS.h"

/**
 * In general this class will be in charge of...
 *     receiving raw keyframes
 *     adding default parameters to the file
 *     setting new static parameters (dimensions, perspective)
 *     interpolting dynamic parameters
 *     outputting dynamic parameters as a string vector
 */

class Animation {
public:

    std::vector<Light *>* lights;
    std::vector<SymmetryVector *>* symmetryVectors;
    IFS* iFS;
    Camera* camera;
    Filter1f* colorOffset;
    std::vector<Color*>* backgroundColors;
    Filter1f* coloringSpeed;

    bool applyLights;
    bool applySymmetryVectors;
    bool applyIFS;
    bool applyCamera;
    bool applyColorOffset;
    bool applyBackgroundColors;
    bool applyColoringSpeed;

    Animation(std::string _rawKeyframeDirectory);
    void setParameter(std::string tag, std::string value);
    void setParameter(int tagNumber, std::string value);
    void setFramesPerCycle(float _framesPerCycle);
    void stereoIsEnabled(bool enabled);
    void setFractalPalette(Color* color);

    void applyLightingSettings(int frameNumber);
    void applyCameraSettings(int frameNumber);
    void applyColorOffsetSettings(int frameNumber);
    void applySymmetryVectorSettings(int frameNumber);
    void applyIFSSetitngs(int frameNumber);
    void applyBackroundColorSettings(int frameNumber);
    void applyColoringSpeedSettings(int frameNumber);
    void applyAllSettings(int frameNumber);

    bool stereoIsEnabled();
    void write();

private:

    std::string rawKeyframeDirectory;
    std::string keyframeDirectory;
    std::vector<FractParam> keyframes;
    float framesPerCycle;
    bool stereoEnabled;

    void prepareKeyframes();
};


#endif //RENDER_ANIMATION_H

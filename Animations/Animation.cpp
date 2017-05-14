//
// Created by lance on 3/16/17.
//

#include "Animation.h"
#include "Plotter.h"

Animation::Animation(std::string _rawKeyframeDirectory)
{

    applyLights           = true;
    applySymmetryVectors  = true;
    applyIFS              = true;
    applyCamera           = true;
    applyColorOffset      = true;
    applyBackgroundColors = true;
    applyColoringSpeed    = true;

    setFramesPerCycle(100);
    stereoIsEnabled(false);
    rawKeyframeDirectory = _rawKeyframeDirectory;
    keyframeDirectory = "Keyframes";
    prepareKeyframes();

    // Set default lights
    lights = new std::vector<Light *>;

    // set symmetry vectors
    symmetryVectors = new std::vector<SymmetryVector*>;

    // Set default camera
    camera = new Camera;

    // Set color offset
    colorOffset = new Filter1f("0");

    backgroundColors = new std::vector<Color*>;

    coloringSpeed = new Filter1f("1");
}

void Animation::prepareKeyframes()
{
    // Grab all present raw staticKeyframes
    int frameNumber = 0;
    char fileName[25];
    std::vector<FractParam> rawKeyframes;
    sprintf(fileName, "keyframe%05d.fract", frameNumber);

    do
    {
        sprintf(fileName, "keyframe%05d.fract", frameNumber);
        if (!fileIsEmpty(rawKeyframeDirectory + "/" + fileName))
        {
            rawKeyframes.push_back(* new FractParam(readAllLines(rawKeyframeDirectory + "/" + fileName)));
        }
        frameNumber++;
    }
    while (!fileIsEmpty(rawKeyframeDirectory + "/" + fileName));

    // prepare vector of explicit frames by populating a vector with copies of the default parameters
    FractParam * defaultKeyframe = new FractParam(readAllLines("defaults.txt"));
    for (int i = 0; i < rawKeyframes.size(); i++)
    {
        keyframes.push_back(* defaultKeyframe);
    }

    for (int i = 0; i < rawKeyframes.size(); i++) // Iterate through frames
    {
        std::string tag;
        std::string value;
        for (int j = 1; j < rawKeyframes[i].getSize(); j++) // Iterate through raw parameters
        {
            tag = rawKeyframes[i].getTag(j);
            value = rawKeyframes[i].getValue(j);
            keyframes[i].setValue(tag, value);
        }
    }
}

void Animation::setParameter(std::string tag, std::string value)
{
    for (unsigned long i = 0; i < keyframes.size(); i++)
    {
        keyframes.at(i).setValue(tag, value);
    }
}

void Animation::setParameter(int tagNumber, std::string value)
{
    for (unsigned long i = 0; i < keyframes.size(); i++)
    {
        keyframes.at(i).setValue(tagNumber, value);
    }
}

void Animation::setFramesPerCycle(float _framesPerCycle)
{
    if (_framesPerCycle < 1) std::cout << "There must be at least one frame per cycle\n";
    else framesPerCycle = _framesPerCycle;
    setParameter("frames_per_keyframe", std::to_string((int) framesPerCycle));
}

void Animation::stereoIsEnabled(bool enabled)
{
    stereoEnabled = enabled;
    if (stereoEnabled) setParameter("stereo_enabled", "1");
    else setParameter("stereo_enabled", "0");
}

void Animation::setFractalPalette(Color *color)
{
    float nColors = 256; // Counted from defaults
    std::string palette = "";
    for (float i = 0; i < nColors; i++)
    {
        palette += color->rgbToHex(color->getColor(i / (nColors - 1), Color::Space::BGR));
        palette += " ";
    }
    setParameter("palette", palette);
}
void Animation::applyLightingSettings(int frameNumber)
{
    if (applyLights) {
        std::string paramTag;
        for (unsigned long i = 0; i < lights->size(); i++) // At most 4 predefined lights
        {
            float cyclePosition = ((float) frameNumber / framesPerCycle); // TODO investigate the specifcs with index zero?

            paramTag = "aux_light_predefined_" + std::to_string(i + 1) + "_";
            bool enabled = lights->at(i)->isEnabled();
            std::string enabled2 = enabled ? "1" : "0";
            setParameter(paramTag + "enabled", enabled2);

            cv::Vec3f position = lights->at(i)->getPosition(cyclePosition);
            setParameter(paramTag + "x", std::to_string(position.val[0]));
            setParameter(paramTag + "y", std::to_string(position.val[1]));
            setParameter(paramTag + "z", std::to_string(position.val[2])); // It's a left handed coordinate system. Disgraceful

            cv::Scalar color = lights->at(i)->getColorization(cyclePosition);
            setParameter(paramTag + "colour_R", std::to_string(257 * (long) color[2])); // Converts 8 to 16 bit
            setParameter(paramTag + "colour_G", std::to_string(257 * (long) color[1]));
            setParameter(paramTag + "colour_B", std::to_string(257 * (long) color[0]));

            float intensity = lights->at(i)->getIntensity(cyclePosition);
            setParameter(paramTag + "intensity", std::to_string(intensity));

            paramTag = "volumetric_light_enabled_";
            enabled = lights->at(i)->isVolumetricLightEnabled();
            enabled2 = enabled ? "1" : "0";
            setParameter(paramTag + std::to_string(i + 1), enabled2);

            paramTag = "volumetric_light_intensity_";
            setParameter(paramTag + std::to_string(i + 1), std::to_string(lights->at(i)->getVolumetricIntensity(cyclePosition)));
        }
    }
}

void Animation::applyCameraSettings(int frameNumber)
{
    if (applyCamera) {
        float cyclePosition = ((float) frameNumber / framesPerCycle);

        cv::Vec3f position = camera->getPosition(cyclePosition);
        cv::Vec3f orientation = camera->getOrientation(cyclePosition);
        std::string positonTag = "view_point_";
        std::string orientationTag = "angle_";

        setParameter(positonTag + "x", std::to_string(position.val[0]));
        setParameter(positonTag + "y", std::to_string(position.val[1]));
        setParameter(positonTag + "z", std::to_string(position.val[2]));
        setParameter(orientationTag + "alfa", std::to_string(orientation.val[0]));
        setParameter(orientationTag + "beta", std::to_string(orientation.val[1]));
        setParameter(orientationTag + "gamma", std::to_string(orientation.val[2]));
    }
}

void Animation::applyColorOffsetSettings(int frameNumber)
{
    if (applyColorOffset) {
        float offset = colorOffset->val((float) frameNumber / framesPerCycle);
        setParameter("coloring_palette_offset", std::to_string(offset));
    }
}

void Animation::applySymmetryVectorSettings(int frameNumber)
{
    if (applySymmetryVectors) {
        float rpd = (float) M_PI / 180;
        float cyclePosition = ((float) frameNumber / framesPerCycle);
        for (unsigned long i = 0; i < symmetryVectors->size(); i++) {
            std::string vectorTag = "IFS_" + std::to_string(i) + "_";
            SymmetryVector* v = symmetryVectors->at(i);
            bool enabled = v->enabled;
            if (!enabled) setParameter(vectorTag + "enabled", "0");
            else {
                setParameter(vectorTag + "enabled", "1");
                cv::Vec3f position = v->getPosition(cyclePosition);
                setParameter(vectorTag + "x", std::to_string(position[0]));
                setParameter(vectorTag + "y", std::to_string(position[1]));
                setParameter(vectorTag + "z", std::to_string(position[2]));
                cv::Vec3f orientation = v->orientation->val(cyclePosition);
                setParameter(vectorTag + "alfa", std::to_string(orientation[0] * rpd));
                setParameter(vectorTag + "beta", std::to_string(orientation[1] * rpd));
                setParameter(vectorTag + "gamma", std::to_string(orientation[2] * rpd));
                float distance = v->distance->val(cyclePosition);
                setParameter(vectorTag + "distance", std::to_string(distance));
                float intensity = v->intensity->val(cyclePosition);
                setParameter(vectorTag + "intensity", std::to_string(intensity));
            }
        }
    }
}

void Animation::applyIFSSetitngs(int frameNumber)
{
    if (applyIFS) {
        float rpd = (float) M_PI / 180;
        float cyclePosition = ((float) frameNumber / framesPerCycle);
        setParameter("IFS_scale", std::to_string(iFS->scale->val(cyclePosition)));
        cv::Vec3f orientation = iFS->orientation->val(cyclePosition);
        setParameter("IFS_rot_alfa", std::to_string(orientation[0] * rpd));
        setParameter("IFS_rot_beta", std::to_string(orientation[1] * rpd));
        setParameter("IFS_rot_gamma", std::to_string(orientation[2] * rpd));
        cv::Vec3f offset = iFS->offset->val(cyclePosition);
        setParameter("IFS_offsetX", std::to_string(offset[0]));
        setParameter("IFS_offsetY", std::to_string(offset[1]));
        setParameter("IFS_offsetZ", std::to_string(offset[2]));
        cv::Vec3f edge = iFS->edge->val(cyclePosition);
        setParameter("IFS_edgeX", std::to_string(edge[0]));
        setParameter("IFS_edgeY", std::to_string(edge[1]));
        setParameter("IFS_edgeZ", std::to_string(edge[2]));
        bool absX = iFS->absX;
        bool absY = iFS->absY;
        bool absZ = iFS->absZ;
        std::string enabled = absX ? "1" : "0";
        setParameter("IFS_absX", enabled);
        enabled = absY ? "1" : "0";
        setParameter("IFS_absY", enabled);
        enabled = absZ ? "1" : "0";
        setParameter("IFS_absZ", enabled);
    }
}

void Animation::applyBackroundColorSettings(int frameNumber)
{
    if (applyBackgroundColors) {
        float cyclePosition = ((float) frameNumber / framesPerCycle);
        std::string colorTag = "background_color_";
        for (unsigned long i = 1; i < backgroundColors->size() + 1; i++) {
            cv::Vec3f color = backgroundColors->at(i - 1)->getColor(cyclePosition);
            setParameter(colorTag + std::to_string(i) + "_R", std::to_string(257 * (long) color[2]));
            setParameter(colorTag + std::to_string(i) + "_G", std::to_string(257 * (long) color[1]));
            setParameter(colorTag + std::to_string(i) + "_B", std::to_string(257 * (long) color[0]));
        }
    }
}

void Animation::applyColoringSpeedSettings(int frameNumber) {
    if (applyColoringSpeed) {
        float cyclePosition = ((float) frameNumber / framesPerCycle);
        setParameter("coloring_speed", std::to_string(coloringSpeed->val(cyclePosition)));
    }
}

void Animation::applyAllSettings(int frameNumber)
{
    // float cyclePosition = (float) frameNumber / framesPerCycle;
    // if (cyclePosition >= 0 && lights->size() < 3) {
    //     Light* light3 = new Light();
    //     light3->path = new BasicPath("0", "1", "-0.13");
    //     light3->intensity = new Filter1f("0.01 * sigmoid(100, -0.75, x)");
    //     light3->volumetricIntensity = new Filter1f("0.01 * sigmoid(100, -0.75, x)");
    //     Plotter plotter;
    //     plotter.plotFilter(new Filter1f("0.01 * sigmoid(, -0.75, x)"), 100);
    //     Color* light3Color = new Color();
    //     light3Color->setPathColorSpace(Color::Space::BGR);
    //     light3Color->path = new BasicPath("58", "142", "229");
    //     light3->colorization = light3Color;
    //     lights->push_back(light3);
    // }
    applyLightingSettings(frameNumber);
    applyCameraSettings(frameNumber);
    applyColorOffsetSettings(frameNumber);
    applySymmetryVectorSettings(frameNumber);
    applyIFSSetitngs(frameNumber);
    applyBackroundColorSettings(frameNumber);
    applyCameraSettings(frameNumber);
    applyColoringSpeedSettings(frameNumber);
}

bool Animation::stereoIsEnabled()
{
    return stereoEnabled;
}

void Animation::write()
{
    char fileName[25];
    for (int i = 0; i < keyframes.size(); i++)
    {
        std::string fileText;
        for (int j = 0; j < keyframes[i].getSize(); j++)
        {
            fileText += keyframes[i].getTag(j) + " " + keyframes[i].getValue(j) + ";\n";
        }
        std::ofstream keyframeFile;
        sprintf(fileName, "keyframe%05d.fract", i);
        keyframeFile.open(keyframeDirectory + "/" + fileName);
        keyframeFile << fileText;
        keyframeFile.close();
    }
}
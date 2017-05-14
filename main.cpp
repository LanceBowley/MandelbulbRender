#include <iostream>
#include <cstdlib>
#include "Cluster.h"
#include "Animations/Animation.h"
#include "Util.h"
#include "RenderJob.h"
#include "Animations/Light.h"
#include "Animations/Plotter.h"
#include "Animations/Catmul.h"
#include "Animations/Filters/Filter1f.h"
#include "Animations/Filters/Filter3f.h"
#include "Animations/Paths/BasicPath.h"
#include "Animations/Paths/CompoundPath.h"
#include "Animations/Filters/CompoundFilter.h"
#include "Animations/Paths/PathFactory.h"
#include "Animations/Paths/SplinePath.h"
#include "Animations/Filters/PiecewiseFilter.h"

/**
 * Notes
 * Permissions on the home directory are very sensitive because of the keys that branch from there
 * Read and write directory permissions (777) are required for mandelbulber program to operate remotely
 * Keyframe settings order matters (at least "enable_stereo" needs to put in orgainically)
 * OpenCL settings are not stored in saved keyframes
 * keyframe 0 up to the Nth frame must be provided to render the Nth frame
 * Appending to a file with 'echo "message" >> <filePath>' automatically chooses a new line
 *
 * BUGS
 * Stopped running in stereo
 *
 * Running instance was found when the active cluster file was empty
 */

int main() {

    /*Plotter plotter;
    PathAnalyzer pathAnalyzer;

    Animation anim("RawKeyframes");
    anim.stereoIsEnabled(false);
    anim.setFramesPerCycle(600); // 960, 384
    anim.setParameter("image_width", "600"); // 1800
    anim.setParameter("image_height", "300"); // 900
    anim.setParameter("fish_eye", "2"); // '2' is a equirectangular perspective
    anim.setParameter("perspective", "1"); // FOV '1' is 360
    anim.setParameter("stereo_eye_distance", "0.00025");
    anim.setParameter("raytraced_reflections", "0");
    anim.setParameter("specular", "1");
    anim.setParameter("file_destination", "/home/ubuntu/RemoteDesktop/LeftImages/images"); // Image path
    anim.setParameter("file_keyframes", "/home/ubuntu/RemoteDesktop/LeftKeys/keyframe"); // Keyframe path

    // Apply camera settings
    Camera* camera = new Camera; // Velocity at join = [0.000544131, -0.000136614, 0]
    std::vector<cv::Vec3f> cameraPoints = {
            cv::Vec3f(-1.0f, -2.15f, 0.0f),
            cv::Vec3f(-0.35f, -2.15f, 0.0f),
            cv::Vec3f(0.0f, -0.65f, 0.0f),
    };


    Catmul* cameraSpline = new Catmul(cameraPoints, 1.0f);
    SplinePath* cameraPath = new SplinePath();
    cameraPath->filteringEnabled = true;
    Filter1f* cameraPathFilter = new Filter1f("sigmoid(12, 0.2, x)");
    cameraPath->filter = cameraPathFilter;
    cameraPath->spline = cameraSpline;
    camera->path = cameraPath;
    // plotter.plotPath(cameraPath, 100);
    anim.camera = camera;

    //// NEW CAMERA PATH
    cv::Vec3f V = cv::Vec3f(0.000544131f, -0.000136614f, 0.0f);
    Camera* camera2 = new Camera; // Velocity at join = [0.000544131, -0.000136614, 0]
    std::vector<cv::Vec3f> cameraPoints2 = {
            cv::Vec3f(0, 0.888531, 0),
            cv::Vec3f(0, 1.75f, 0),
            cv::Vec3f(-1.1632393f, 1.75f, 0),
            cv::Vec3f(-0.960547f - 300.0f * V[0], -2.16202f + 300.0f * V[1], 0.0f),
            cv::Vec3f(-0.960547f - 150.0f * V[0], -2.16202f + 150.0f * V[1], 0.0f),
            cv::Vec3f(-0.960547f, -2.16202f, 0.0f),
    };
    Catmul* cameraSpline2 = new Catmul(cameraPoints2, 1.0f);
    SplinePath* cameraPath2 = new SplinePath();
    cameraPath2->spline = cameraSpline2;
    Filter1f* pathFilter = new Filter1f("1.006095 * sigmoid(15, -0.16, 1)");
    cameraPath2->filteringEnabled = true;
    cameraPath2->filter = pathFilter;
    std::cout << pathAnalyzer.getVelocity(cameraPath2, 958.0f / 959.0f, 1.0f) << "\n";
    std::cout << cameraPath2->getPosition(1.0f);
    // plotter.plotPath(cameraPath2, 100);
    camera2->path = cameraPath2;
    // anim.camera = camera2;
    ////

    // Apply IFS settings
    IFS* iFS = new IFS();
    iFS->scale = new Filter1f("sigmoid(20, 0.20, x) + 2");
    iFS->offset = new Filter3f("1", "sigmoid(8, 0, x)", "1.6");
    iFS->absX = true;
    iFS->absY = true;
    iFS->absZ = true;
    anim.iFS = iFS;

    // Apply symmetry vectors
    SymmetryVector* symmetryVector1 = new SymmetryVector();
    symmetryVector1->setPosition(new Filter3f("-0.80901699", "0.30901699", "0.5"));
    symmetryVector1->intensity = new Filter1f("1");
    SymmetryVector* symmetryVector2 = new SymmetryVector();
    symmetryVector2->setPosition(new Filter3f("0.5", "-0.80901699", "0.30901699"));
    symmetryVector2->intensity = new Filter1f("1");
    anim.symmetryVectors->push_back(symmetryVector1);
    anim.symmetryVectors->push_back(symmetryVector2);

    // Apply lights
    Light* light1 = new Light;
    light1->path = new BasicPath("0", "0", "-30");
    light1->intensity = new Filter1f(0.1f);
    light1->volumetricIntensity = new Filter1f(1.0f);
    Color* light1Color = new Color;
    light1Color->setPathColorSpace(Color::Space::HSV);
    light1Color->path = new BasicPath("203", "1", "0.45 + 0.07*sin(10 * (x*6.28318))");
    light1->colorization = light1Color;

    Light* light2 = new Light;
    light2->path = new BasicPath("0", "0.5*x", "0");
    light2->path->filteringEnabled = true;
    Filter1f* light2PathFilter = new Filter1f("sigmoid(10, 0, x)");
    light2->path->filter = light2PathFilter;
    light2->intensity = new Filter1f("2");
    light2->volumetricIntensity = new Filter1f("0.1");
    Color* light2Color = new Color;
    light2Color->setPathColorSpace(Color::Space::HSV);
    light2Color->path = new BasicPath("294", "0.39", "1");
    light2->colorization = light2Color;

    anim.coloringSpeed = new Filter1f("2");
    anim.colorOffset = new Filter1f("0");

    anim.lights->push_back(light1);
    anim.lights->push_back(light2);

    anim.applyAllSettings(0);
    anim.write();*/

    Animation anim("RawKeyframes");
    anim.stereoIsEnabled(true);
    anim.setFramesPerCycle(959); // 960, 384
    anim.setParameter("image_width", "2160"); // 1800
    anim.setParameter("image_height", "1080"); // 900
    anim.setParameter("fish_eye", "2"); // '2' is a equirectangular perspective
    anim.setParameter("perspective", "1"); // FOV '1' is 360
    anim.setParameter("stereo_eye_distance", "0.00025");
    anim.setParameter("raytraced_reflections", "0");
    anim.setParameter("specular", "1");
    anim.setParameter("file_destination", "/home/ubuntu/RemoteDesktop/LeftImages/images"); // Image path
    anim.setParameter("file_keyframes", "/home/ubuntu/RemoteDesktop/LeftKeys/keyframe"); // Keyframe path

    cv::Vec3f V = cv::Vec3f(0.000544131f, -0.000136614f, 0.0f);
    Camera* camera2 = new Camera; // Velocity at join = [0.000544131, -0.000136614, 0]
    std::vector<cv::Vec3f> cameraPoints2 = {
            cv::Vec3f(0, 0.888231, 0),
            cv::Vec3f(0, 1.75f, 0),
            cv::Vec3f(-1.1632393f, 1.75f, 0),
            cv::Vec3f(-0.960547f - 300.0f * V[0], -2.16202f + 300.0f * V[1], 0.0f),
            cv::Vec3f(-0.960547f - 150.0f * V[0], -2.16202f + 150.0f * V[1], 0.0f),
            cv::Vec3f(-0.960547f, -2.16202f, 0.0f),
    };
    Catmul* cameraSpline2 = new Catmul(cameraPoints2, 1.0f);
    SplinePath* cameraPath2 = new SplinePath();
    cameraPath2->spline = cameraSpline2;
    Filter1f* pathFilter = new Filter1f("1.006095 * sigmoid(15, -0.16, x)");
    cameraPath2->filteringEnabled = true;
    cameraPath2->filter = pathFilter;
    camera2->path = cameraPath2;
    anim.camera = camera2;

    // Apply IFS settings
    IFS* iFS = new IFS();
    iFS->offset = new Filter3f("1", "1 - x * (1 - sigmoid(8, 0, 0))", "1.6");
    iFS->orientation = new Filter3f("0", "40 - 40 * sigmoid(15, 0, x)", "0"); // @ 40 -> 0
    PiecewiseFilter* scaleFilter = new PiecewiseFilter();
    scaleFilter->addFilter(new Filter1f("2.30239 + 0.7 * sigmoid(20, 0.25, x) - 0.00469 + (2* x * 0.00469)"), 0.0f);
    scaleFilter->addFilter(new Filter1f("3.00239 - 1.0 * sigmoid(20, -0.25, x)"), 0.5f);
    // Filter1f* scaleFilter = new Filter1f("2.30239 + 0.5 * sigmoid(30, 0.25, x) - 0.30239 * sigmoid(-10, 0, x)");
    // Plotter plotter;
    // plotter.plotFilter(scaleFilter, 400);
    // std::cout << scaleFilter->val(0) << "\n";
    // iFS->scale = new Filter1f("2.30239 - 0.299917 * sigmoid(15, 0, x)"); // @ 2.30239 -> 2.002473
    iFS->scale = scaleFilter;
    iFS->absX = true;
    iFS->absY = true;
    iFS->absZ = true;
    anim.iFS = iFS;

    // Apply symmetry vectors
    SymmetryVector* symmetryVector1 = new SymmetryVector();
    symmetryVector1->setPosition(new Filter3f("-0.80901699", "0.30901699", "0.5"));
    symmetryVector1->intensity = new Filter1f("1");
    symmetryVector1->orientation = new Filter3f("0", "14.815 - 14.815 * sigmoid(15, 0, x)", "0"); // @ 14.815 -> 0
    SymmetryVector* symmetryVector2 = new SymmetryVector();
    symmetryVector2->setPosition(new Filter3f("0.5", "-0.80901699", "0.30901699"));
    symmetryVector2->intensity = new Filter1f("1");
    anim.symmetryVectors->push_back(symmetryVector1);
    anim.symmetryVectors->push_back(symmetryVector2);

    // Apply lights
    Light* light1 = new Light;
    light1->path = new BasicPath("0", "0", "-30");
    light1->intensity = new Filter1f("0.1");
    light1->volumetricIntensity = new Filter1f("1");
    Color* light1Color = new Color;
    light1Color->setPathColorSpace(Color::Space::HSV);
    light1Color->path = new BasicPath("203", "1", "0.45");
    light1->colorization = light1Color;

    Light* light2 = new Light;
    light2->isEnabled(true); // turn on at 750 conservatively TODO !!CHECK!!
    light2->path = new BasicPath("0", "0.5*x", "0");
    light2->path->filteringEnabled = true;
    Filter1f* light2PathFilter = new Filter1f("sigmoid(10, 0, x)");
    light2->path->filter = light2PathFilter;
    light2->intensity = new Filter1f("2 * sigmoid(60, -0.4, x)");
    light2->volumetricIntensity = new Filter1f("0.1 * sigmoid(60, -0.4, x)");
    Color* light2Color = new Color;
    light2Color->setPathColorSpace(Color::Space::HSV);
    light2Color->path = new BasicPath("294", "0.39", "1");
    light2->colorization = light2Color;

    anim.lights->push_back(light1);
    anim.lights->push_back(light2);

    // Apply coloring speed
    anim.coloringSpeed = new Filter1f("1.19395 + 0.80605 * x"); // @ 1.19395 -> 2

    //Apply color offset settings
    anim.colorOffset = new Filter1f("7.061522 - 7.061522 * x"); // @ 7.061522 -> 0

    anim.applyAllSettings(0);
    anim.write();


    Cluster cl1("cl1");
    cl1.downloadDirectory("/home/ubuntu/images", "/home/lance", true);
    // cl1.kill();
    // Cluster cl1("cl1", "ami-551c8535", "sg-84de03fc", "AWSKeyPair", "p2.xlarge");
    // cl1.launch(1, 0.30);
    for (int i = 0; i < cl1.getActiveSize(); i++)
    {
        std::cout << cl1.getManualAccess(i) << "\n";
    }
    // RenderJob render(&cl1, &anim);
    // render.clean();
    // render.start(0, 969, false); // TODO go only to 958 :)
    // TODO, check that all images are present and complete from last render!!!!
    return 0;
}
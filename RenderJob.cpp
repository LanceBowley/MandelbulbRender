//
// Created by lance on 1/21/17.
//

#include "RenderJob.h"

RenderJob::RenderJob (Cluster* _cluster, Animation* _anim)
{
    cluster = _cluster;
    anim = _anim;
    thisDirectory = getThisDirectory();
    remoteDirectory = "/home/ubuntu/RemoteDesktop";
}

void RenderJob::flushImageFolders()
{
    cluster->execute("rm /home/ubuntu/RemoteDesktop/LeftImages/*", true);
    cluster->execute("rm /home/ubuntu/RemoteDesktop/RightImages/*", true);
    cluster->execute("rm /home/ubuntu/RemoteDesktop/NewImages/*", true);
    cluster->execute("rm /home/ubuntu/RemoteDesktop/TempImages/*", true);
}

void RenderJob::terminateRenderProcesses()
{
    cluster->execute("pkill mandelbulber", true);
}

void RenderJob::clean()
{
    flushImageFolders();
    terminateRenderProcesses();
}

void RenderJob::start(int startFrame, int endFrame, bool terminateUponCompletion) {
    std::vector<int> serverNumbers = cluster->getActiveServerNumbers();
    std::vector<int> openFrames;
    std::vector<int> workingFrames;
    workingFrames.push_back(-1); // Mitigating strange bug
    for (int i = startFrame; i <= endFrame; i++)
    {
        if (!imageExists(i))
        {
            openFrames.push_back(i);
        }
    }
    for (int i = 0; i < serverNumbers.size(); i++)
    {
        workingFrames.push_back(-1); // Negative one indicates a jobless server
    }
    workingFrames.erase(workingFrames.begin()); // Undoing hack
    bool rendering = true;
    while (rendering)
    {
        for (int i = 0; i < serverNumbers.size(); i++)
        {
            if (workingFrames[i] == -1)
            {
                workingFrames[i] = openFrames[0];
                if (openFrames.size() != 0)
                {
                    openFrames.erase(openFrames.begin());
                }
                else
                {
                    rendering = false;
                }
                anim->applyAllSettings(workingFrames[i]);
                anim->write();
                cluster->at(i).uploadDirectory("/home/lance/RemoteDesktop/Render/Keyframes", "/home/ubuntu/RemoteDesktop/LeftKeys", SERIAL, true);
                std::string frameRequest = getFrameRequest(workingFrames[i]);
                cluster->execute(i, frameRequest, PARALLEL, false);
            }
        }
        if (anim->stereoIsEnabled()) cluster->execute("mv " + remoteDirectory + "/NewImages/* " + remoteDirectory + "/TempImages", true);
        else cluster->execute("mv " + remoteDirectory + "/LeftImages/* " + remoteDirectory + "/TempImages", true);
        cluster->downloadDirectory(remoteDirectory + "/TempImages", thisDirectory + "/Images", true);
        for (int i = 0; i < workingFrames.size(); i++)
        {
            if (imageExists(workingFrames[i]))
            {
                log("Frame number " + std::to_string(workingFrames[i]) + " processed by server " +
                std::to_string(i) + " at " + cluster->at(i).getIP());
                workingFrames[i] = -1;
                cluster->execute(i, "mv " + remoteDirectory + "/TempImages/* " + remoteDirectory + "/Images", PARALLEL, false);
            }
        }
    }
    if (terminateUponCompletion) cluster->kill();
}

std::string RenderJob::getFrameRequest(int frameNumber)
{
    char command1[200];
    sprintf(command1, "mandelbulber-opencl -keyframe -start %01d -end %01d ", frameNumber, frameNumber + 1);
    std::string command2 = command1 + remoteDirectory + "/LeftKeys/keyframe00000.fract";
    return command2;
}

bool RenderJob::imageExists(int frameNumber)
{
    char framePath[25];
    sprintf(framePath, "Images/images%05d.jpg", frameNumber);
    return !fileIsEmpty(framePath);
}
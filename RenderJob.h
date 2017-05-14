//
// Created by lance on 1/21/17.
//

#ifndef RENDER_RENDER_H
#define RENDER_RENDER_H

#include "Cluster.h"
#include "Animations/Animation.h"
#include <vector>


class RenderJob {
public:

    RenderJob (Cluster* _cluster, Animation* _anim);
    void flushImageFolders();
    void terminateRenderProcesses();
    void clean();
    void start(int startFrame, int endFrame, bool terminateUponCompletion = false);

private:

    std::string thisDirectory;
    std::string remoteDirectory;
    Cluster* cluster;
    Animation* anim;

    bool imageExists (int frameNumber);
    std::string getFrameRequest (int frameNumber);
};


#endif //RENDER_RENDER_H

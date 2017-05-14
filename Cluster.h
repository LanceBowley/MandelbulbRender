//
// A cluster is a collection of AWS servers usually grouped to perform a specfic task
//

#ifndef RENDER_CLUSTERAWS_H
#define RENDER_CLUSTERAWS_H

#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include "ServerAWS.h"
#include "json.hpp"
#include "Util.h"

using namespace nlohmann;

class Cluster {
public:

    Cluster(std::string _name);
    Cluster(std::string _name, std::string specName);
    Cluster(std::string _name, std::string ami, std::string securityGroup, std::string key, std::string instanceType);
    void saveLaunchSpecs(std::string specName);
    void loadLaunchSpecs(std::string specName);
    json getClusterSpecs();
    json getClusterSpecs(std::string name);
    void launch(int nServers, double maxPrice);
    ServerAWS at(int serverNumber);
    std::vector<int> getActiveServerNumbers();
    int getActiveSize();
    std::string getManualAccess(int serverNumber);
    void execute(int serverNumber, std::string command, Execution execT, bool hold);
    void execute(std::string command, bool hold);
    void uploadFile(std::string localPath, std::string remoteDirectory, bool hold);
    void uploadDirectory(std::string localDirectory, std::string remoteDirectory, bool hold);
    void downloadDirectory(std::string remoteDirectory, std::string localDirectory, bool hold);
    void executeScript(std::string localBashPath, std::string remoteBashDirectory, bool hold);
    void setup();
    void killProc();
    void kill(int serverNumber);
    void kill(std::string iP);
    void kill();

private:

    std::string key;
    std::string name;
    bool uniqueName;
    bool launched;
    std::string activeClusterFile;
    std::string thisDirectory;
    std::string launchSpecDirectory;
    std::string remoteSetupDirectory;
    json clusterSpecs;
    std::vector<ServerAWS> servers;

    void init();
    void updateClusters();
    bool clusterNameExists (std::string name);
    void buildServers();
    void removeID(std::string clusterName, std::string targetID);
    std::vector<std::string> getActiveInstanceIDs();
};

#endif //RENDER_CLUSTERAWS_H
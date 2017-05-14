//
// Created by lance on 1/20/17.
//

#include "Cluster.h"

/**
 * NOTES
 * Active cluster formatting
 * Cluster format = [{"Name": "name"}, {"LaunchSpecs": "launchSpecs"}, {"InstanceIds": ["ID1", "ID2"]}]
 *
 * Serial and parallel means the same thing as they do in ServerAWS; however, hold inidcates whether a batch of commands
 * is allowed to complete before the next execution
 */

// Load from active cluster file
Cluster::Cluster (std::string _name)
{
    name = _name;
    clusterSpecs["Name"] = name;

    init();
    launched = true;
    if (clusterNameExists(name))
    {
        updateClusters();
        json specs = getClusterSpecs(name);
        clusterSpecs["LaunchSpecs"] = specs["LaunchSpecs"];
        clusterSpecs["InstanceIds"] = specs["InstanceIds"];
        buildServers();
    }
    else
    {
        uniqueName = false;
        std::string message = "Error: \"" + name + "\" is not the name of a valid cluster, servers were not built\n";
        std::cout << message;
        log(message);
    }
}

// Load new cluster from saved launch spec
Cluster::Cluster (std::string _name, std::string specName)
{
    name = _name;
    clusterSpecs["Name"] = name;

    init();
    launched = false;
    updateClusters();
    if (!clusterNameExists(name))
    {
        uniqueName = true;
    }
    loadLaunchSpecs(specName);
}

// Create a new cluster on the fly
Cluster::Cluster (std::string _name, std::string ami, std::string securityGroup, std::string key, std::string instanceType)
{
    name = _name;
    clusterSpecs["Name"] = name;

    init();
    launched = false;
    updateClusters();
    if (clusterNameExists(name))
    {
        uniqueName = false;
    }

    ami = "\"" + ami + "\"";
    securityGroup = "\"" + securityGroup + "\"";
    key = "\"" + key + "\"";
    instanceType = "\"" + instanceType + "\"";

    // Prepare json document
    json launchSpecs = {
            {"\"ImageId\"", ami},
            {"\"KeyName\"", key},
            {"\"SecurityGroupIds\"", {securityGroup}},
            {"\"InstanceType\"", instanceType},
            {"\"Placement\"", {{"\"AvailabilityZone\"", "\"us-west-2b\""}}},
            {"\"IamInstanceProfile\"", {{"\"Arn\"", "\"arn:aws:iam::818369878656:instance-profile/administrator\""}}}
    };
    clusterSpecs["LaunchSpecs"] = launchSpecs;
}

void Cluster::init ()
{
    uniqueName = true;
    thisDirectory = getThisDirectory();
    launchSpecDirectory = thisDirectory + "/LaunchSpecs";
    remoteSetupDirectory = thisDirectory + "/RemoteSetup";
    activeClusterFile = "ActiveClusters.json";
    key = "AWSKeyPair";
}

bool Cluster::clusterNameExists (std::string name)
{
    json clusterSpecs = getClusterSpecs(name);
    return (clusterSpecs["Name"] == name);
}

void Cluster::saveLaunchSpecs (std::string specName)
{
    std::string specFilePath = launchSpecDirectory + "/" + specName + ".json";
    if (fileIsEmpty(specFilePath))
    {
        std::ofstream oFile;
        oFile.open(specFilePath);
        oFile << clusterSpecs["launchSpecs"];
        oFile.close();
    }
    else
    {
        std::string message = "Error: A file with the name " + specName + ".json already exists. The launch specs were not saved\n";
        std::cout << message;
        log(message);
    }
}

void Cluster::loadLaunchSpecs (std::string specName)
{
    json launchSpecs;
    std::string specFilePath = launchSpecDirectory + "/" + specName + ".json";
    if (!fileIsEmpty(specFilePath))
    {
        launchSpecs = jsonRead(specFilePath);
        clusterSpecs["LaunchSpecs"] = launchSpecs;
    }
    else
    {
        std::string message = "Error: A file with the name " + specName + " could not be found\n";
        std::cout << message;
        log(message);
    }
}

// Searches the current active instances, and compares those to the instances that are logged in the active cluster file
// If there are records of instances in the file that do not match any known active instances, then that instance is
// removed from the file
void Cluster::updateClusters ()
{
    std::vector<std::string> activeInstanceIDs = getActiveInstanceIDs();
    std::string activeClusterPath = thisDirectory + "/" + activeClusterFile;

    if (!fileIsEmpty(activeClusterPath)) // Check to see if the cluster file has anything in it
    {
        json loggedClusters;
        loggedClusters = jsonRead(activeClusterPath);
        std::string loggedClusterName;
        json loggedClusterIDs;
        json loggedClusterLaunchSpecs;

        for (json::iterator i = loggedClusters.begin(); i != loggedClusters.end(); ++i) // Iterate through cluster log
        {
            loggedClusterName = (*i)["Name"];
            loggedClusterIDs = (*i)["InstanceIds"];
            loggedClusterLaunchSpecs = (*i)["LaunchSpecs"];
            std::string loggedServerID;

            for (int j = 0; j < loggedClusterIDs.size(); j++)
            {
                bool serverFound = false;
                loggedServerID = loggedClusterIDs[j];
                std::string activeServerID;
                for (int k = 0; k < activeInstanceIDs.size(); k++)
                {
                    activeServerID = activeInstanceIDs[k];
                    if (!loggedServerID.compare(activeServerID))
                    {
                        serverFound = true;
                    }
                }
                if (!serverFound)
                {
                    removeID(loggedClusterName, loggedServerID);
                }
            }
        }
    }
}

// Finds all active instance IDs associated with spot requests
std::vector<std::string> Cluster::getActiveInstanceIDs()
{
    std::string tempFilePath = thisDirectory + "/Temp.json";
    clearFile(tempFilePath);
    std::string command = "aws ec2 describe-spot-instance-requests --query SpotInstanceRequests[*] > " + tempFilePath;
    sysExec(command);
    std::vector<std::string> activeInstanceIDs;
    json spotRequests;
    if (!fileIsEmpty(tempFilePath))
    {
        spotRequests = jsonRead(tempFilePath);
    }
    // clearFile(tempFilePath);
    for (json::iterator it = spotRequests.begin(); it != spotRequests.end(); ++it)
    {
        std::string state = (*it)["State"];
        if (!state.compare("open") || !state.compare("active")) // If the state is not closed
        {
            json instance = (*it)["InstanceId"];
            if (!instance.is_null()) // Not all requests have json attribute "InstanceId"
            {
                activeInstanceIDs.push_back((*it)["InstanceId"]);
            }
        }
    }
    return activeInstanceIDs;
}

json Cluster::getClusterSpecs ()
{
    return getClusterSpecs(name);
}

// Pulls cluster specs from the active cluster file.
// TODO investigate whether this is needed if there is already a cluster specs object
json Cluster::getClusterSpecs (std::string name)
{
    std::string activeClusterPath = thisDirectory + "/" + activeClusterFile;
    json clusterSpecs;
    if (!fileIsEmpty(activeClusterPath))
    {
        json activeLaunchSpecs = jsonRead(activeClusterPath);
        for (int i = 0; i < activeLaunchSpecs.size(); i++)
        {
            json j = activeLaunchSpecs[i];
            if (j["Name"] == name)
            {
                clusterSpecs = j;
            }
        }
    }
    return clusterSpecs;
}

// Finds the IPs of the IDs stored in class variable "clusterSpecs" and instantiates new servers if there are IDs that
// cannot be found within the active cluster servers
void Cluster::buildServers()
{
    std::vector<std::string> IPs;
    std::string tempFilePath = thisDirectory + "/Temp.json";
    std::vector<std::string> IDs = clusterSpecs["InstanceIds"];
    json reservation;
    json instance;
    for (int i = 0; i < IDs.size(); i++)
    {
        clearFile(tempFilePath);
        std::string command = "aws ec2 describe-instances --instance-ids " + IDs[i] + " > " + tempFilePath;
        sysExec(command);
        reservation = jsonRead(tempFilePath)["Reservations"][0];
        instance = reservation;
        instance = instance["Instances"][0];
        std::string ip = instance["PublicIpAddress"];
        IPs.push_back(ip);
        clearFile(tempFilePath);
    }

    ServerAWS server;
    for (int i = 0; i < IDs.size(); i++)
    {
        bool IDfound = false;
        for (int j = 0; j < servers.size(); j++)
        {
            if (!IDs[i].compare(servers[j].getID()))
            {
                IDfound = true;
            }
        }
        if(!IDfound)
        {
            server = ServerAWS(key, IDs[i], IPs[i]);
            servers.push_back(server);
        }
    }
}

// Launches the specified number of servers with the class launch specifications.
// Does this by making getting the current active IDs and then sending a request for a launch. The system then
// continually queries for the new active instances for comparison. If N new active server IDs are found, these
// are added to the cluster specs and the active cluster file is rewritten to reflect this. Finally the servers are
// "built" where the IDs are used to find an associated IP which comes together together to build a server
void Cluster::launch(int nServers, double maxPrice)
{
    if (uniqueName)
    {
        std::vector<std::string> startIDs = getActiveInstanceIDs();
        char request[1000];
        sprintf(request, "aws ec2 request-spot-instances --spot-price \"%f\" --instance-count %d --type \"one-time\" "
                "--launch-specification ", maxPrice, nServers);
        std::string command = request + clusterSpecs["LaunchSpecs"].dump();
        sysExec(command);
        std::vector<std::string> launchedIDs;
        bool connected = false;
        int launchIDsFound;
        while (!connected)
        {
            launchIDsFound = 0;
            launchedIDs.clear(); // Emptied launched IDs in case not all are active at the start
            sleepSeconds(1);
            std::vector<std::string> finishIDs = getActiveInstanceIDs();
            std::string startID;
            std::string finishID;
            // Iterate through new IDs to see if one does not match the old
            for (int i = 0; i < finishIDs.size(); i++)
            {
                finishID = finishIDs[i];
                bool newIDhasMatch = false;
                for (int j = 0; j < startIDs.size(); j++)
                {
                    startID = startIDs[j];
                    if (!finishID.compare(startID))
                    {
                        newIDhasMatch = true;
                    }
                }
                if (!newIDhasMatch)
                {
                    launchIDsFound++;
                    launchedIDs.push_back(finishID);

                    if (launchIDsFound == nServers)
                    {
                        connected = true;
                        for (int k = 0; k < launchedIDs.size(); k++)
                        {
                            std::string message = "Launched instance with ID \"" + launchedIDs[k] + "\"\n";
                            std::cout << message;
                            log(message);
                        }
                    }
                }
            }
        }
        json clusters;
        std::string activeClusterPath = thisDirectory + "/" + activeClusterFile;
        if (!fileIsEmpty(activeClusterPath))
        {
            clusters = jsonRead(activeClusterPath);
        }
        json cluster;
        if (launched)
        {
            for (int i = 0; i < clusters.size(); i++)
            {
                cluster = clusters[i];
                if (cluster["Name"] == name)
                {
                    for (int j = 0; j < launchedIDs.size(); j++)
                    {
                        cluster["InstanceIds"].push_back(launchedIDs[j]);
                        clusterSpecs["InstanceIds"].push_back(launchedIDs[j]);
                    }
                    clusters[i] = cluster;
                }
            }
        }
        else
        {
            for (int j = 0; j < launchedIDs.size(); j++)
            {
                clusterSpecs["InstanceIds"].push_back(launchedIDs[j]);
            }
            clusters.push_back(clusterSpecs);
        }
        jsonWrite(activeClusterPath, clusters);
        buildServers();
        launched = true; // TODO make sure that instances actually launch before declaration
    }
    else
    {
        std::string message = "Error: could not launch instances for a new cluster with name \"" + name + "\". Name already exists\n";
        std::cout << message;
        log(message);
    }
}

// Searches through current IDs to find the target ID inside the active cluster file, if the target ID is found in
// the structure then it is not added, and the clusterspec is updated to match the updated json object
void Cluster::removeID (std::string clusterName, std::string targetID)
{
    std::string activeClusterPath = thisDirectory + "/" + activeClusterFile;
    bool IDfound = false;
    json cluster = clusterSpecs;
    if (!cluster.empty())
    {
        std::vector<std::string> IDs = cluster["InstanceIds"];

        cluster["InstanceIds"] = {};
        for (int i = 0; i < IDs.size(); i++)
        {
            if (IDs[i].compare(targetID)) // ID doesn't compare
            {
                cluster["InstanceIds"].push_back(IDs[i]);
            }
            else
            {
                IDfound = true;
            }
        }
    }
    clusterSpecs = cluster; // TODO make this always correct by removing the clusterName parameter
    json clusters = jsonRead(activeClusterPath);
    json currentCluster;
    for (int i = 0; i < clusters.size(); i++)
    {
        currentCluster = clusters[i];
        std::string currentClusterName = currentCluster["Name"];
        if (!currentClusterName.compare(clusterName))
        {
            currentCluster = cluster;
            clearFile(activeClusterPath);
            if (cluster["InstanceIds"].size() > 0)
            {
                clusters[i] = currentCluster;
                jsonWrite(activeClusterPath, clusters);
            }
            else
            {
                clusters.erase(i);
                if (clusters.size() > 0)
                {
                    jsonWrite(activeClusterPath, clusters);
                }
            }
        }
    }
    std::string message;
    if (IDfound)
    {
        message = "Instance ID \"" + targetID + "\" found and removed from \"" + clusterName + "\"\n";
        std::cout << message;
        log(message);
    }
    else
    {
        message = "Instance ID \"" + targetID + "\" was not found in cluster \"" + clusterName + "\"\n";
        std::cout << message;
        log(message);
    }
}

ServerAWS Cluster::at (int serverNumber)
{
    return servers[serverNumber];
}

std::vector<int> Cluster::getActiveServerNumbers ()
{
    std::vector<int> v;
    for (int i = 0; i < servers.size(); i++)
    {
        if (servers.at((unsigned long) i).isActive())
        {
            v.push_back(i);
        }
    }
    return v;
}

int Cluster::getActiveSize ()
{
    return (int) getActiveServerNumbers().size();
}

std::string Cluster::getManualAccess (int serverNumber)
{
    return servers[serverNumber].getManualAccess();
}

void Cluster::execute(int serverNumber, std::string command, Execution execT, bool hold)
{
    servers.at((unsigned long) serverNumber).execute(command, execT, hold);
}

void Cluster::execute(std::string command, bool hold)
{
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        if (servers.at(i).isActive()) {
            execute((int) i, command, PARALLEL, true);
        }
    }
    runCommandQueue(hold);
}

void Cluster::uploadFile(std::string localPath, std::string remoteDirectory, bool hold)
{
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        if (servers.at(i).isActive())
        {
            servers.at(i).uploadFile(localPath, remoteDirectory, PARALLEL, true);
        }
    }
    runCommandQueue(hold);
}

void Cluster::uploadDirectory(std::string localDirectory, std::string remoteDirectory, bool hold)
{
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        if (servers.at(i).isActive())
        {
            servers.at(i).uploadDirectory(localDirectory, remoteDirectory, PARALLEL, true);
        }
    }
    runCommandQueue(hold);
}

void Cluster::downloadDirectory(std::string remoteDirectory, std::string localDirectory, bool hold)
{
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        if (servers.at(i).isActive())
        {
            servers.at(i).downloadDirectory(remoteDirectory, localDirectory, PARALLEL, true);
        }
    }
    runCommandQueue(hold);
}

void Cluster::executeScript(std::string localBashPath, std::string remoteBashDirectory, bool hold)
{
    unsigned long localPathLength = localBashPath.size();
    unsigned long lastSlashStart = localBashPath.find_last_of("/");
    unsigned long fileNameLength = localPathLength - lastSlashStart - 1;
    std::string fileName = localBashPath.substr(lastSlashStart + 1, fileNameLength);

    uploadFile(localBashPath, remoteBashDirectory, true);
    std::string command = "sudo bash " + remoteBashDirectory + "/" + fileName;
    execute(command, hold);
}

void Cluster::setup()
{
    executeScript(thisDirectory + "/RemoteSetup/SetupHeadless.sh", "/home/ubuntu", true);
    sleepSeconds(90);
    executeScript(thisDirectory + "/RemoteSetup/SetupHeadless2.sh", "/home/ubuntu", true);
}

void Cluster::killProc()
{
    execute("killall5 -9", true);
}

void Cluster::kill(int serverNumber)
{
    std::string message;
    std::string serverNumberString = std::to_string(serverNumber);
    if (serverNumber >= servers.size() || serverNumber < 0)
    {
        message = "Error: cannot terminate non-existant server #" + serverNumberString + "\n";
        std::cout << message;
        log(message);
    }
    else if (!servers[serverNumber].isActive())
    {
        message = "Error: cannot terminate already terminated server number #" + serverNumberString + "\n";
        std::cout << message;
        log(message);
    }
    else
    {
        std::string serverID = servers[serverNumber].getID();
        servers[serverNumber].kill();
        message = "Terminated server number #" + serverNumberString + " in cluster \"" + name +"\"\n";
        std::cout << message;
        log(message);
        removeID(name, serverID);
    }
}

void Cluster::kill(std::string iP)
{
    std::vector<int> s = getActiveServerNumbers();

    for (unsigned long i = 0; i < s.size(); i++)
    {
        if (servers[s[i]].getIP() == iP)
        {
            kill(s[i]);
        }
    }
}

void Cluster::kill()
{
    if (servers.size() > 0)
    {
        for (int i = 0; i < servers.size(); i++)
        {
            if (servers[i].isActive())
            {
                kill(i);
                sleepSeconds(0.1);
            }
        }
    }
    else
    {
        std::string message = "Error: no servers left to terminate\n";
        std::cout << message;
        log(message);
    }
}
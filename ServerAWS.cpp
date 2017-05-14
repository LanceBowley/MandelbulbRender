//
// Created by lance on 1/20/17.
//

/**
 * NOTES
 *
 *
 * AMIs
 * c3_U14.04_default_10GB: ami-2a7cc74a
 *
 * */
#include "ServerAWS.h"

ServerAWS::ServerAWS()
{
    key = "";
    id = "";
    ipAddress = "";
    keyPath = "";
    active = false;
}

ServerAWS::ServerAWS(const ServerAWS &server)
{
    *this = server;
}

ServerAWS::ServerAWS(std::string _key, std::string _id, std::string _ipAddress)
{
    key = _key;
    id = _id;
    ipAddress = _ipAddress;
    keyPath = getThisDirectory() + "/" + key + ".pem";
    publicDNS = getPublicDNS(BASIC);
    active = true;
}

std::string ServerAWS::getID()
{
    return id;
}

std::string ServerAWS::getIP()
{
    return ipAddress;
}

bool ServerAWS::isActive()
{
    return active;
}

std::string ServerAWS::getManualAccess()
{
    std::string command = "ssh -o StrictHostKeyChecking=no -i " + keyPath + " " + publicDNS; // keyFlag is proven
    return command;
}

void ServerAWS::execute(std::string remoteCommand, Execution execType, bool hold)
{
    if (active) {
        std::string connect = "ssh -o StrictHostKeyChecking=no -i " + keyPath + " " + publicDNS + " "; // keyFlag is proven
        std::string command = connect + remoteCommand;

        manageCommand(command, execType, hold);
    }
    else {
        inactiveServerError();
    }
}

void ServerAWS::uploadFile(std::string localPath, std::string remoteDirectory, Execution execType, bool hold)
{
    if (active) {
        std::string command = "scp -o StrictHostKeyChecking=no -i " + keyPath + " " + localPath + " " + publicDNS + ":" + remoteDirectory;
        manageCommand(command, execType, hold);
    }
    else {
        inactiveServerError();
    }
}

void ServerAWS::uploadDirectory(std::string localDirectory, std::string remoteDirectory, Execution execType, bool hold)
{
    if (active) {
        std::string command = "scp -o StrictHostKeyChecking=no -i " + keyPath + " " + localDirectory + "/* " + publicDNS + ":" + remoteDirectory;
        manageCommand(command, execType, hold);
    }
    else {
        inactiveServerError();
    }
}

void ServerAWS::downloadDirectory(std::string remoteDirectory, std::string localDirectory, Execution execType,
                                  bool hold)
{
    if (active) {
        std::string command = "scp -o StrictHostKeyChecking=no -i " + keyPath + " -r " + publicDNS + ":" + remoteDirectory + "/* " + localDirectory;
        manageCommand(command, execType, hold);
    }
    else {
        inactiveServerError();
    }
}

void ServerAWS::kill()
{
    if (active) {
        std::string command = "aws ec2 terminate-instances --instance-ids " + id;
        manageCommand(command, SERIAL, false);
        active = false;
    }
    else {
        inactiveServerError();
    }
}

void ServerAWS::manageCommand(std::string command, Execution execType, bool hold)
{
    if (execType == PARALLEL)
    {
        command += " &";
    }
    appendCommandToQueue(command);
    if (!hold)
    {
        runCommandQueue();
    }
}

std::string ServerAWS::getPublicDNS(DNS_Type type)
{
    std::string pubDNS;
    std::string ipFrag;
    std::string rawIP = ipAddress;
    unsigned long dotIndex;
    switch (type)
    {
        case FULL:
            for (int i = 0; i < 4; i++)
            {
                dotIndex = rawIP.find("."); // Finds first index of
                ipFrag += rawIP.substr(0, dotIndex); // Start index, len of substring
                ipFrag += (i == 3) ? "" : "-";
                rawIP.erase(0, dotIndex + 1); // Erase start index to dot (inclusive)
            }
            pubDNS = "ubuntu@ec2-" + ipFrag + ".us-west-2.compute.amazonaws.com";
            break;
        default:
            pubDNS = "ubuntu@" + ipAddress;
    }
    return pubDNS;
}

void ServerAWS::inactiveServerError()
{
    std::string message = "Unable to sysExec on server with ID " + id + ". Server was previously terminated\n";
    std::cout << message;
    log(message);
}
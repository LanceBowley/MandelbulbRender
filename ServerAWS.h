//
// Created by lance on 1/20/17.
//

#ifndef MANAGESERVERS_SERVER_H
#define MANAGESERVERS_SERVER_H

#include <string>
#include <fstream>
#include "Util.h"
#include "Util.h"

/**
 * NOTES
 * execType defines how a command gets appended to the exec file. Serial appends such that the commands get executed one
 * after the next. Parallel appends such that all commands are executed simultaneously. When a command is held it is
 * appended without being executed
 *
 * ssh flag setting to prevent query for connection permission
 * -o StrictHostKeyChecking=no
 *
 * AMIs
 * U14.04_default_10GB: ami-2a7cc74a
 * U14.04_default_25GB: ami-0c23986c
 *
 */

enum DNS_Type
{
    BASIC = 0,
    FULL = 1
};

class ServerAWS {
public:

    ServerAWS ();
    ServerAWS (const ServerAWS &server);
    ServerAWS (std::string _key, std::string _id, std::string _ipAddress);

    std::string getID ();
    std::string getIP ();
    bool isActive ();
    std::string getManualAccess ();
    void execute(std::string command, Execution execType = SERIAL, bool hold = false);
    void uploadFile(std::string localPath, std::string remoteDirectory, Execution execType = SERIAL, bool hold = false);
    void uploadDirectory(std::string localDirectory, std::string remoteDirectory, Execution execType = SERIAL,
                         bool hold = false);
    void downloadDirectory(std::string remoteDirectory, std::string localDirectory, Execution execType = SERIAL,
                           bool hold = false);
    void kill();

private:

    std::string key;
    std::string id;
    std::string ipAddress;
    std::string publicDNS;
    std::string keyPath;
    bool active;

    void manageCommand (std::string command, Execution execType, bool hold);
    std::string getPublicDNS (DNS_Type type);
    void inactiveServerError();
};


#endif //MANAGESERVERS_SERVER_H

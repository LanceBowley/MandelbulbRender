//
// Created by lance on 1/21/17.
//

#include "Util.h"


std::string getThisDirectory ()
{
    std::string dir;
    system("bash ./Env.sh");
    std::ifstream envFile;
    envFile.open("EnvVar.txt");
    getline(envFile, dir);
    system("> EnvVar.txt");
    return dir;
}

void clearFile (std::string fileName)
{
    std::fstream file;
    file.open(fileName, std::fstream::out|std::fstream::trunc);
    file.close();
}

bool fileIsEmpty (std::string fileName)
{
    bool empty = false;
    std::ifstream file;
    file.open(fileName);
    std::string line1;
    std::getline(file, line1);
    if(!line1.compare("null") || !line1.compare(""))
    {
        empty = true;
    }
    return empty;
}

nlohmann::json jsonRead (std::string fileName)
{
    nlohmann::json j;
    std::fstream file;
    file.open(fileName);
    file >> j;
    file.close();
    return j;
}

void jsonWrite (std::string fileName, nlohmann::json j)
{
    std::fstream file;
    clearFile(fileName);
    file.open(fileName);
    file << std::setw(4) << j << std::endl;
    file.close();
}

std::vector<std::string> jsonFileToVector (std::string fileName)
{
    std::string s;
    std::vector<std::string> v;
    nlohmann::json j = jsonRead(fileName);
    for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
        s = *it;
        v.push_back(s);
    }
    return v;
}

void log (std::string entry)
{
    std::fstream file;
    file.open("log.txt", std::fstream::app|std::fstream::out);
    file << entry;
}

void sleepSeconds (float seconds)
{
    int microSeconds = (int) seconds*1000000;
    usleep((__useconds_t) microSeconds);
}

void runCommandQueue (bool hold)
{
    std::string execPath = getThisDirectory() + "/Exec.sh";
    if (hold)
    {
        sysExec("echo \"wait\" >> " + execPath + "\n");
    }
    sysExec("bash " + execPath);
    clearFile(execPath);
}

void sysExec(std::string command)
{
    // command = "sudo " + command;
    const char * c = command.c_str();
    system(c);
}

void appendCommandToQueue(std::string command1)
{
    std::string execPath = getThisDirectory() + "/Exec.sh";
    std::string command2 = "echo \"" + command1 + "\" >> " + execPath + "\n";
    sysExec(command2);
}

std::vector<std::string> readAllLines(std::string filePath)
{
    std::string line;
    std::vector<std::string> lines;
    std::fstream file;
    file.open(filePath);
    while (getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();
    return lines;
}

std::vector<cv::Vec3f> generateRandomPoints(int nPoints)
{
    srand(time(NULL));

    std::vector<cv::Vec3f> points;;

    for(int i=0; i<nPoints; i++)
    {
        points.push_back(cv::Vec3f(rand()%100, rand()%100, rand()%100));
    }
    return points;
}

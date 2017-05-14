//
// Created by lance on 1/21/17.
//

#ifndef RENDER_BASH_H
#define RENDER_BASH_H

#include <string>
#include <bits/ios_base.h>
#include <ios>
#include <fstream>
#include <zconf.h>
#include <opencv2/core/matx.hpp>
#include "json.hpp"

enum Execution
{
    SERIAL = 0,
    PARALLEL = 1
};

std::string getThisDirectory();
void clearFile (std::string fileName);
bool fileIsEmpty (std::string fileName);
void log (std::string entry);
void sleepSeconds (float seconds);
nlohmann::json jsonRead (std::string fileName);
void jsonWrite (std::string fileName, nlohmann::json j);
std::vector<std::string> jsonFileToVector (std::string fileName);
void runCommandQueue(bool hold = false);
void sysExec(std::string command);
void appendCommandToQueue(std::string command1);
std::vector<std::string> readAllLines(std::string filePath);
std::vector<cv::Vec3f> generateRandomPoints(int nPoints);

#endif //RENDER_BASH_H

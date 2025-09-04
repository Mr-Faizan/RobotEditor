#ifndef ROBOTLIB_H
#define ROBOTLIB_H

#include <iostream>
#include <string>
#include <stdexcept> 
#include <vector>
#include <fstream>

#include <miniz/miniz.h>
#include <filesystem>
#include <cstring> // for memset

#include "RobotLib_global.h"
#include "robot.h"
#include "nlohmann/json.hpp"

#include "jsonkeys2.h"

#include "importvcmx.h"

using namespace std;
using json = nlohmann::json;

class ROBOTLIB_EXPORT RobotLib
{
private:
    vector<Robot> robotCollection;

public:
    RobotLib();
    ~RobotLib();
    // Robot management
    Robot initializeNewRobot();
    Robot createRobot();
    Joint createJoint();
    JointDynamics createDynamics();

    void addRobot(const Robot &robot);
    void removeRobot(const string &name);
    Robot& getRobot(const string &name);
    const vector<Robot> &getRobots() const;
    Robot& getRobotById(int robotId);
    const Robot *getRobotById2(int robotId) const;

    bool updateAndSaveRobotData(const std::string &filePath, const json &json);
    Robot loadFromFile(const string &filePath);
    double getNumberFromJson(const json &value);
    // Load and save data
    Robot loadFromJson(const json jsonData);
    bool saveToJson(const std::string &filePath, Robot &robot) const;
    string zipRobotPackage(const string& folderPath);
    bool extractRobotPackage(const string& reFilePath, const string& destDir);



    // Print data
    void printData() const;
    // New function to initiate vcmxDataExtractor
    Robot importRobotFromVCMX(const string &filePath);
    Robot parseRobotFromVCMX(const string &robotDataFolderPath);
};

#endif // ROBOTLIB_H

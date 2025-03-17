#ifndef ROBOTLIB_H
#define ROBOTLIB_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "RobotLib_global.h"
#include "robot.h"
#include "nlohmann/json.hpp"

#include "jsonkeys2.h"

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
    Robot createRobot();
    void addRobot(const Robot &robot);
    void removeRobot(const string &name);
    Robot& getRobot(const string &name);
    const vector<Robot> &getRobots() const;
    Robot& getRobotById(int robotId);
    const Robot *getRobotById2(int robotId) const;

    bool updateAndSaveRobotData(const std::string &filePath, const json &json, int robotId);
    bool loadFromFile(const string &filePath, Robot &robot);
    // Load and save data
    bool loadFromJson(const json jsonData, Robot &robot);
    bool saveToJson(const std::string &filePath, Robot &robot) const;

    // Print data
    void printData() const;
};

#endif // ROBOTLIB_H

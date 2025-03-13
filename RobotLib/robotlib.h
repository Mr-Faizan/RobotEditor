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
    Robot *getRobot(const string &name);
    const vector<Robot> &getRobots() const;
    const Robot *getRobotById(int robotId) const;

    // Load and save data
    bool loadFromJson(const string &filePath, Robot &robot);
    bool saveToJson(const std::string &filePath, int robotId) const;

    // Print data
    void printData() const;
};

#endif // ROBOTLIB_H

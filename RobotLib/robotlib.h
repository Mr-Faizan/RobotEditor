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
    vector<Robot> robots;

public:
    RobotLib();
    ~RobotLib();
    // Robot management
    void addRobot(const Robot &robot);
    void removeRobot(const string &name);
    Robot *getRobot(const string &name);
    const vector<Robot> &getRobots() const;

    // Load and save data
    bool loadFromJson(const string &filePath);
    bool saveToJson(const string &filePath) const;

    // Print data
    void printData() const;
};

#endif // ROBOTLIB_H

#ifndef ROBOTLIB_H
#define ROBOTLIB_H

#include <vector>
#include <fstream>

#include "RobotLib_global.h"
#include "robot.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class ROBOTLIB_EXPORT RobotLib
{
private:
    std::vector<Robot> robots;

public:
    RobotLib();
    // Robot management
    void addRobot(const Robot &robot);
    void removeRobot(const std::string &name);
    Robot *getRobot(const std::string &name);
    const std::vector<Robot> &getRobots() const;

    // Load and save data
    bool loadFromJson(const std::string &filePath);
    bool saveToJson(const std::string &filePath) const;
};

#endif // ROBOTLIB_H

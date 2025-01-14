#ifndef JOINT_H
#define JOINT_H

#include <String>
#include "dynamics.h"
#include "kinematics.h"
#include "visualization.h"

class Joint
{
public:
    Joint(const std::string& name, const std::string& type, float range);

private:
    std::string name;
    std::string type;
    float range;
    Dynamics* dynamics;
    Kinematics* kinematics;
    Visualization* visualization;
};

#endif // JOINT_H

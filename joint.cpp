#include "joint.h"

Joint::Joint(const std::string& name, const std::string& type, float range)
    : name(name), type(type), range(range), dynamics(nullptr), kinematics(nullptr), visualization(nullptr)
{
    // Initialize dynamics, kinematics, and visualization for the joint
    dynamics = new JointDynamics();
    kinematics = new JointKinematics();
    visualization = new JointVisualization();
}

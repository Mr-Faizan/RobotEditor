#include "robot.h"

using namespace std;

// Initialize the static member variable
int Robot::lastAssignedId = 1000;

Robot::Robot()
    : id(++lastAssignedId),
      name(""),
      manufacturer(""),
      payload(0),
      footPrint(0),
      maxReach(0),
      repeatability(0),
      weight(0),
      dof(0)
{
    // Add a default joint
    //addJoint();
    
}

Robot::Robot(const string &name)
    : id(++lastAssignedId),
      name(name),
      manufacturer(""),
      payload(0),
      footPrint(0),
      maxReach(0),
      repeatability(0),
      weight(0),
      dof(0),
      jointCounter(0)
{
    // Add a default joint
    //addJoint();
}

Robot::Robot(const string &name, const string &manufacturer, double payload, double footPrint, double maxReach, double repeatability, double weight, int dof)
    : id(++lastAssignedId),
      name(name),
      manufacturer(manufacturer),
      payload(payload),
      footPrint(footPrint),
      maxReach(maxReach),
      repeatability(repeatability),
      weight(weight),
      dof(dof),
      jointCounter(0)
{
    // Add a default joint
    //addJoint();
}

void Robot::addJoint(const Joint &joint)
{
    joints.push_back(joint);
}


Joint& Robot::createAndAddJoint()
{
    Joint newJoint;
    
    // newJoint.setJointNumber(JointKeys2::Joint + std::to_string(++jointCounter));
    joints.push_back(newJoint);

    return joints.back();
}

Joint &Robot::getJointByJointNumber(const std::string &jointNumber)
{
    for (auto &joint : joints)
    {
        if (joint.getJointNumber() == jointNumber)
        {
            return joint;
        }
    }
    throw std::runtime_error("Joint not found");
}
